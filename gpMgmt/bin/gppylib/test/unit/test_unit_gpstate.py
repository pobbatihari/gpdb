
from pygresql import pgdb

from gppylib.gparray import Segment, GpArray
from gppylib.operations.startSegments import StartSegmentsResult

from gppylib import gparray
from gppylib.programs.clsSystemState import *
from gppylib.system.configurationInterface import GpConfigurationProvider
from gppylib.test.unit.gp_unittest import GpTestCase, run_tests

#
# Creation helpers for gparray.Segment.
import imp
import logging
import os
import signal
import sys
import time
import unittest
import mock
from mock import Mock, call, patch

from gppylib.gparray import Segment, GpArray, SegmentPair
from gppylib.test.unit.gp_unittest import GpTestCase, run_tests



def create_segment(**kwargs):
    """
    Takes the same arguments as gparray.Segment's constructor, but with friendly
    defaults to minimize typing.
    """
    content        = kwargs.get('content', 0)
    role           = kwargs.get('role', gparray.ROLE_PRIMARY)
    preferred_role = kwargs.get('preferred_role', role)
    dbid           = kwargs.get('dbid', 0)
    mode           = kwargs.get('mode', gparray.MODE_SYNCHRONIZED)
    status         = kwargs.get('status', gparray.STATUS_UP)
    hostname       = kwargs.get('hostname', 'localhost')
    address        = kwargs.get('address', 'localhost')
    port           = kwargs.get('port', 15432)
    datadir        = kwargs.get('datadir', '/tmp/')

    return gparray.Segment(content, preferred_role, dbid, role, mode, status,
                           hostname, address, port, datadir)

def create_primary(**kwargs):
    """Like create_segment() but with the role overridden to ROLE_PRIMARY."""
    kwargs['role'] = gparray.ROLE_PRIMARY
    return create_segment(**kwargs)

def create_mirror(**kwargs):
    """Like create_segment() but with the role overridden to ROLE_MIRROR."""
    kwargs['role'] = gparray.ROLE_MIRROR
    return create_segment(**kwargs)

class ReplicationInfoTestCase(unittest.TestCase):
    """
    A test case for GpSystemStateProgram._add_replication_info().
    """

    def setUp(self):
        """
        Every test starts with a primary, a mirror, and a GpStateData instance
        that is ready to record information for the mirror segment. Feel free to
        ignore these if they are not helpful.
        """
        self.primary = create_primary(dbid=1)
        self.mirror = create_mirror(dbid=2)

        self.data = GpStateData()
        self.data.beginSegment(self.primary)
        self.data.beginSegment(self.mirror)

        # Implementation detail for the mock_pg_[table] functions. _pg_rows maps
        # a query fragment to the set of rows that should be returned from
        # dbconn.execSQL() for a matching query. Reset this setup for every
        # test.
        self._pg_rows = {}

    def _get_rows_for_query(self, *args):
        """
        Mock implementation of dbconn.execSQL() for these unit tests. Don't use
        this directly; use one of the mock_pg_xxx() helpers.
        """
        query = args[1]
        rows = None

        # Try to match the execSQL() query against one of our stored fragments.
        for fragment in self._pg_rows:
            if fragment in query:
                rows = self._pg_rows[fragment]
                break

        if rows is None:
            self.fail(
                'Expected one of the query fragments {!r} to be in the query {!r}.'.format(
                    self._pg_rows.keys(), query
                )
            )

        # Mock out the cursor's rowcount, fetchall(), and fetchone().
        # fetchone.side_effect conveniently lets us return one row from the list
        # at a time.
        cursor = mock.MagicMock()
        cursor.rowcount = len(rows)
        cursor.fetchall.return_value = rows
        cursor.fetchone.side_effect = rows
        return cursor

    def mock_pg_stat_replication(self, mock_execSQL, rows):
        self._pg_rows['pg_stat_replication'] = rows
        mock_execSQL.side_effect = self._get_rows_for_query

    def stub_replication_entry(self, **kwargs):
        # The row returned here must match the order and contents expected by
        # the pg_stat_replication query performed in _add_replication_info().
        # It's put here so there is just a single place to fix the tests if that
        # query changes.
        return (
            kwargs.get('application_name', 'gp_walreceiver'),
            kwargs.get('state', 'streaming'),
            kwargs.get('sent_location', '0/0'),
            kwargs.get('flush_location', '0/0'),
            kwargs.get('flush_left', 0),
            kwargs.get('replay_location', '0/0'),
            kwargs.get('replay_left', 0),
            kwargs.get('backend_start', None)
        )

    def mock_pg_stat_activity(self, mock_execSQL, rows):
        self._pg_rows['pg_stat_activity'] = rows
        mock_execSQL.side_effect = self._get_rows_for_query

    def stub_activity_entry(self, **kwargs):
        # The row returned here must match the order and contents expected by
        # the pg_stat_activity query performed in _add_replication_info(); see
        # stub_replication_entry() above.
        return (
            kwargs.get('backend_start', None),
        )

    def test_add_replication_info_adds_unknowns_if_primary_is_down(self):
        self.primary.status = gparray.STATUS_DOWN
        GpSystemStateProgram._add_replication_info(self.data, self.primary, self.mirror)

        self.assertEqual('Unknown', self.data.getStrValue(self.mirror, VALUE__REPL_SENT_LOCATION))
        self.assertEqual('Unknown', self.data.getStrValue(self.mirror, VALUE__REPL_FLUSH_LOCATION))
        self.assertEqual('Unknown', self.data.getStrValue(self.mirror, VALUE__REPL_REPLAY_LOCATION))

    @mock.patch('gppylib.db.dbconn.connect', autospec=True)
    def test_add_replication_info_adds_unknowns_if_connection_cannot_be_made(self, mock_connect):
        # Simulate a connection failure in dbconn.connect().
        mock_connect.side_effect = pgdb.InternalError('connection failure forced by unit test')
        GpSystemStateProgram._add_replication_info(self.data, self.primary, self.mirror)

        self.assertEqual('Unknown', self.data.getStrValue(self.mirror, VALUE__REPL_SENT_LOCATION))
        self.assertEqual('Unknown', self.data.getStrValue(self.mirror, VALUE__REPL_FLUSH_LOCATION))
        self.assertEqual('Unknown', self.data.getStrValue(self.mirror, VALUE__REPL_REPLAY_LOCATION))

    @mock.patch('gppylib.db.dbconn.execSQL', autospec=True)
    @mock.patch('gppylib.db.dbconn.connect', autospec=True)
    def test_add_replication_info_adds_unknowns_if_pg_stat_replication_has_no_entries(self, mock_connect, mock_execSQL):
        self.mock_pg_stat_replication(mock_execSQL, [])

        GpSystemStateProgram._add_replication_info(self.data, self.primary, self.mirror)

        self.assertEqual('Unknown', self.data.getStrValue(self.mirror, VALUE__REPL_SENT_LOCATION))
        self.assertEqual('Unknown', self.data.getStrValue(self.mirror, VALUE__REPL_FLUSH_LOCATION))
        self.assertEqual('Unknown', self.data.getStrValue(self.mirror, VALUE__REPL_REPLAY_LOCATION))

    @mock.patch('gppylib.db.dbconn.execSQL', autospec=True)
    @mock.patch('gppylib.db.dbconn.connect', autospec=True)
    def test_add_replication_info_adds_unknowns_if_pg_stat_replication_has_too_many_mirrors(self, mock_connect, mock_execSQL):
        self.mock_pg_stat_replication(mock_execSQL, [
            self.stub_replication_entry(application_name='gp_walreceiver'),
            self.stub_replication_entry(application_name='gp_walreceiver'),
        ])

        GpSystemStateProgram._add_replication_info(self.data, self.primary, self.mirror)

        self.assertEqual('Unknown', self.data.getStrValue(self.mirror, VALUE__REPL_SENT_LOCATION))
        self.assertEqual('Unknown', self.data.getStrValue(self.mirror, VALUE__REPL_FLUSH_LOCATION))
        self.assertEqual('Unknown', self.data.getStrValue(self.mirror, VALUE__REPL_REPLAY_LOCATION))

    @mock.patch('gppylib.db.dbconn.execSQL', autospec=True)
    @mock.patch('gppylib.db.dbconn.connect', autospec=True)
    def test_add_replication_info_populates_correctly_from_pg_stat_replication(self, mock_connect, mock_execSQL):
        # Set up the row definition.
        self.mock_pg_stat_replication(mock_execSQL, [
            self.stub_replication_entry(
                sent_location='0/1000',
                flush_location='0/0800',
                flush_left=2048,
                replay_location='0/0000',
                replay_left=4096,
            )
        ])

        GpSystemStateProgram._add_replication_info(self.data, self.primary, self.mirror)

        self.assertEqual('Streaming', self.data.getStrValue(self.mirror, VALUE__MIRROR_STATUS))
        self.assertEqual('0/1000', self.data.getStrValue(self.mirror, VALUE__REPL_SENT_LOCATION))
        self.assertEqual('0/0800 (2048 bytes left)', self.data.getStrValue(self.mirror, VALUE__REPL_FLUSH_LOCATION))
        self.assertEqual('0/0000 (4096 bytes left)', self.data.getStrValue(self.mirror, VALUE__REPL_REPLAY_LOCATION))

    @mock.patch('gppylib.db.dbconn.execSQL', autospec=True)
    @mock.patch('gppylib.db.dbconn.connect', autospec=True)
    def test_add_replication_info_omits_lag_info_if_WAL_locations_are_identical(self, mock_connect, mock_execSQL):
        # Set up the row definition.
        self.mock_pg_stat_replication(mock_execSQL, [
            self.stub_replication_entry(
                sent_location='0/1000',
                flush_location='0/1000',
                flush_left=0,
                replay_location='0/1000',
                replay_left=0,
            )
        ])

        GpSystemStateProgram._add_replication_info(self.data, self.primary, self.mirror)

        self.assertEqual('0/1000', self.data.getStrValue(self.mirror, VALUE__REPL_SENT_LOCATION))
        self.assertEqual('0/1000', self.data.getStrValue(self.mirror, VALUE__REPL_FLUSH_LOCATION))
        self.assertEqual('0/1000', self.data.getStrValue(self.mirror, VALUE__REPL_REPLAY_LOCATION))

    @mock.patch('gppylib.db.dbconn.execSQL', autospec=True)
    @mock.patch('gppylib.db.dbconn.connect', autospec=True)
    def test_add_replication_info_adds_unknowns_if_pg_stat_replication_is_incomplete(self, mock_connect, mock_execSQL):
        # Set up the row definition.
        self.mock_pg_stat_replication(mock_execSQL, [
            self.stub_replication_entry(
                sent_location=None,
                flush_location=None,
                flush_left=None,
                replay_location=None,
                replay_left=None,
            )
        ])

        GpSystemStateProgram._add_replication_info(self.data, self.primary, self.mirror)

        self.assertEqual('Unknown', self.data.getStrValue(self.mirror, VALUE__REPL_SENT_LOCATION))
        self.assertEqual('Unknown', self.data.getStrValue(self.mirror, VALUE__REPL_FLUSH_LOCATION))
        self.assertEqual('Unknown', self.data.getStrValue(self.mirror, VALUE__REPL_REPLAY_LOCATION))

    @mock.patch('gppylib.db.dbconn.execSQL', autospec=True)
    @mock.patch('gppylib.db.dbconn.connect', autospec=True)
    def test_add_replication_info_closes_connections(self, mock_connect, mock_execSQL):
        self.mock_pg_stat_replication(mock_execSQL, [])

        GpSystemStateProgram._add_replication_info(self.data, self.primary, self.mirror)

        assert mock_connect.return_value.close.called

    @mock.patch('gppylib.db.dbconn.execSQL', autospec=True)
    @mock.patch('gppylib.db.dbconn.connect', autospec=True)
    def test_add_replication_info_displays_full_backup_state_on_primary(self, mock_connect, mock_execSQL):
        self.mock_pg_stat_replication(mock_execSQL, [
            self.stub_replication_entry(
                application_name='some_backup_utility',
                state='backup',
                sent_location='0/0', # this matches the real-world behavior but is unimportant to the test
                flush_location=None,
                flush_left=None,
                replay_location=None,
                replay_left=None,
            )
        ])

        GpSystemStateProgram._add_replication_info(self.data, self.primary, self.mirror)

        self.assertEqual('Copying files from primary', self.data.getStrValue(self.primary, VALUE__MIRROR_STATUS))

    @mock.patch('gppylib.db.dbconn.execSQL', autospec=True)
    @mock.patch('gppylib.db.dbconn.connect', autospec=True)
    def test_add_replication_info_displays_full_backup_start_timestamp_on_primary(self, mock_connect, mock_execSQL):
        self.mock_pg_stat_replication(mock_execSQL, [
            self.stub_replication_entry(
                application_name='some_backup_utility',
                state='backup',
                sent_location='0/0', # this matches the real-world behavior but is unimportant to the test
                flush_location=None,
                flush_left=None,
                replay_location=None,
                replay_left=None,
                backend_start='1970-01-01 00:00:00.000000-00'
            )
        ])

        GpSystemStateProgram._add_replication_info(self.data, self.primary, self.mirror)

        self.assertEqual('1970-01-01 00:00:00.000000-00', self.data.getStrValue(self.primary, VALUE__MIRROR_RECOVERY_START))

    @mock.patch('gppylib.db.dbconn.execSQL', autospec=True)
    @mock.patch('gppylib.db.dbconn.connect', autospec=True)
    def test_add_replication_info_displays_simultaneous_backup_and_replication(self, mock_connect, mock_execSQL):
        self.mock_pg_stat_replication(mock_execSQL, [
            self.stub_replication_entry(
                application_name='some_backup_utility',
                state='backup',
                sent_location='0/0', # this matches the real-world behavior but is unimportant to the test
                flush_location=None,
                flush_left=None,
                replay_location=None,
                replay_left=None,
            ),
            self.stub_replication_entry(
                state='streaming',
            ),
        ])

        GpSystemStateProgram._add_replication_info(self.data, self.primary, self.mirror)

        self.assertEqual('Copying files from primary', self.data.getStrValue(self.primary, VALUE__MIRROR_STATUS))
        self.assertEqual('Streaming', self.data.getStrValue(self.mirror, VALUE__MIRROR_STATUS))

    @mock.patch('gppylib.db.dbconn.execSQL', autospec=True)
    @mock.patch('gppylib.db.dbconn.connect', autospec=True)
    def test_add_replication_info_displays_status_when_pg_rewind_is_active_and_mirror_is_down(self, mock_connect, mock_execSQL):
        self.mock_pg_stat_replication(mock_execSQL, [])
        self.mirror.status = gparray.STATUS_DOWN
        self.mock_pg_stat_activity(mock_execSQL, [self.stub_activity_entry()])

        GpSystemStateProgram._add_replication_info(self.data, self.primary, self.mirror)

        self.assertEqual('Rewinding history to match primary timeline', self.data.getStrValue(self.primary, VALUE__MIRROR_STATUS))

    @mock.patch('gppylib.db.dbconn.execSQL', autospec=True)
    @mock.patch('gppylib.db.dbconn.connect', autospec=True)
    def test_add_replication_info_does_not_update_mirror_status_when_mirror_is_down_and_there_is_no_recovery_underway(self, mock_connect, mock_execSQL):
        self.mock_pg_stat_replication(mock_execSQL, [])
        self.mirror.status = gparray.STATUS_DOWN
        self.mock_pg_stat_activity(mock_execSQL, [])

        self.data.switchSegment(self.primary)
        self.data.addValue(VALUE__MIRROR_STATUS, 'previous value')
        GpSystemStateProgram._add_replication_info(self.data, self.primary, self.mirror)

        # The mirror status should not have been touched in this case.
        self.assertEqual('previous value', self.data.getStrValue(self.primary, VALUE__MIRROR_STATUS))

    @mock.patch('gppylib.db.dbconn.execSQL', autospec=True)
    @mock.patch('gppylib.db.dbconn.connect', autospec=True)
    def test_add_replication_info_displays_start_time_when_pg_rewind_is_active_and_mirror_is_down(self, mock_connect, mock_execSQL):
        mock_date = '1970-01-01 00:00:00.000000-00'
        self.mock_pg_stat_replication(mock_execSQL, [self.stub_replication_entry()])
        self.mock_pg_stat_activity(mock_execSQL, [self.stub_activity_entry(backend_start=mock_date)])
        self.mirror.status = gparray.STATUS_DOWN

        GpSystemStateProgram._add_replication_info(self.data, self.primary, self.mirror)

        self.assertEqual(mock_date, self.data.getStrValue(self.primary, VALUE__MIRROR_RECOVERY_START))

    @mock.patch('gppylib.db.dbconn.execSQL', autospec=True)
    @mock.patch('gppylib.db.dbconn.connect', autospec=True)
    def test_add_replication_info_does_not_query_pg_stat_activity_when_mirror_is_up(self, mock_connect, mock_execSQL):
        self.mock_pg_stat_replication(mock_execSQL, [])
        self.mock_pg_stat_activity(mock_execSQL, [self.stub_activity_entry()])

        GpSystemStateProgram._add_replication_info(self.data, self.primary, self.mirror)

        for call in mock_execSQL.mock_calls:
            args = call[1]  # positional args are the second item in the tuple
            query = args[1] # query is the second argument to execSQL()
            self.assertFalse('pg_stat_activity' in query)

    def test_set_mirror_replication_values_complains_about_incorrect_kwargs(self):
        with self.assertRaises(TypeError):
            GpSystemStateProgram._set_mirror_replication_values(self.data, self.mirror, badarg=1)

class GpStateDataTestCase(unittest.TestCase):


    def test_switchSegment_sets_current_segment_correctly(self):
        data = GpStateData()
        primary = create_primary(dbid=1)
        mirror = create_mirror(dbid=2)

        data.beginSegment(primary)
        data.beginSegment(mirror)

        data.switchSegment(primary)
        data.addValue(VALUE__HOSTNAME, 'foo')
        data.addValue(VALUE__ADDRESS, 'bar')

        data.switchSegment(mirror)
        data.addValue(VALUE__DATADIR, 'baz')
        data.addValue(VALUE__PORT, 'abc')

        self.assertEqual('foo', data.getStrValue(primary, VALUE__HOSTNAME))
        self.assertEqual('bar', data.getStrValue(primary, VALUE__ADDRESS))
        self.assertEqual('baz', data.getStrValue(mirror, VALUE__DATADIR))
        self.assertEqual('abc', data.getStrValue(mirror, VALUE__PORT))

        # Make sure that neither the mirror nor the primary were accidentally
        # updated in lieu of the other.
        self.assertEqual('', data.getStrValue(mirror, VALUE__HOSTNAME))
        self.assertEqual('', data.getStrValue(primary, VALUE__DATADIR))

class GpState(GpTestCase):
    def setUp(self):
        # because gpstate does not have a .py extension,
        # we have to use imp to import it
        # if we had a gpstate.py, this is equivalent to:
        #   import gpstate
        #   self.subject = gpstate
        gpstate_file = os.path.abspath(os.path.dirname(__file__) + "/../../../gpstate")
        self.subject = imp.load_source('gpstate', gpstate_file)
        self.subject.logger = Mock(spec=['log', 'warn', 'info', 'debug', 'error', 'warning', 'fatal'])
        self.os_environ = dict(MASTER_DATA_DIRECTORY='/tmp/mdd', GPHOME='/tmp/gphome', GP_MGMT_PROCESS_COUNT=1,
                               LANGUAGE=None, GP_COMMAND_FAULT_POINT='', HOME='/tmp', PGPASSFILE='/tmp/.pgpass')

        self.mock_gp = Mock()
        self.mock_pgconf = Mock()
        self.mock_os = Mock()
        self.mock_conn = Mock()
        self.mock_catalog = Mock()
        self.mock_gperafile = Mock()
        self.mock_unix = Mock()
        self.gparray = self.createGpArrayWith4Primary4Mirrors()

        self.apply_patches([
            patch('os.getenv', side_effect=self._get_env),
            patch('gppylib.db.dbconn.execSQLForSingletonRow', return_value=["foo"]),
            patch('gpstate.gp', return_value=self.mock_gp),
            #patch.object(GpSegStopCmd, "__init__", return_value=None),
            patch('gpstate.pgconf', return_value=self.mock_pgconf),
            patch('gpstate.os', return_value=self.mock_os),
            patch('gpstate.dbconn.connect', return_value=self.mock_conn),
            patch('gpstate.catalog', return_value=self.mock_catalog),
            patch('gpstate.unix', return_value=self.mock_unix),
            patch('gpstate.GpEraFile', return_value=self.mock_gperafile),
            patch('gpstate.GpArray.initFromCatalog'),
            patch('gpstate.gphostcache.unix.Ping'),
            patch('gpstate.RemoteOperation'),
            patch('gpstate.base.WorkerPool'),
            patch('gpstate.socket.gethostname'),
            patch('gppylib.programs.clsAddMirrors.configInterface.getConfigurationProvider', return_value=Mock())
        ])
        self.mock_gp = self.get_mock_from_apply_patch('gp')
        self.mock_gplog_log_to_file_only = self.get_mock_from_apply_patch("log_to_file_only")
        self.mock_gp.get_masterdatadir.return_value = 'masterdatadir'
        self.mock_gp.GpCatVersion.local.return_value = 1
        self.mock_gp.GpCatVersionDirectory.local.return_value = 1
        self.config_provider_mock = mock.MagicMock(spec=GpConfigurationProvider)
        self.config_provider_mock.initializeProvider.return_value = self.config_provider_mock

        sys.argv = ["gpstate"]  # reset to relatively empty args list

    def setup_gpstate(self):
        parser = self.subject.GpSystemStateProgram.createParser()
        options, args = parser.parse_args()
        gpstate = self.subject.GpSystemStateProgram.createProgram(options, args)
        return gpstate

    def tearDown(self):
        super(GpState, self).tearDown()

    def createGpArrayWith4Primary4Mirrors(self):
        self.master = Segment.initFromString(
            "1|-1|p|p|s|u|mdw|mdw|5432|/data/master")

        self.primary0 = Segment.initFromString(
            "2|0|p|p|s|u|sdw1|sdw1|40000|/data/primary0")
        self.primary1 = Segment.initFromString(
            "3|1|p|p|s|u|sdw1|sdw1|40001|/data/primary1")
        self.primary2 = Segment.initFromString(
            "4|2|p|p|s|u|sdw2|sdw2|40002|/data/primary2")
        self.primary3 = Segment.initFromString(
            "5|3|p|p|s|u|sdw2|sdw2|40003|/data/primary3")

        self.mirror0 = Segment.initFromString(
            "6|0|m|m|s|u|sdw2|sdw2|50000|/data/mirror0")
        self.mirror1 = Segment.initFromString(
            "7|1|m|m|s|u|sdw2|sdw2|50001|/data/mirror1")
        self.mirror2 = Segment.initFromString(
            "8|2|m|m|s|u|sdw1|sdw1|50002|/data/mirror2")
        self.mirror3 = Segment.initFromString(
            "9|3|m|m|s|u|sdw1|sdw1|50003|/data/mirror3")
        return GpArray([self.master, self.primary0, self.primary1, self.primary2, self.primary3, self.mirror0, self.mirror1, self.mirror2, self.mirror3])

    def get_info_messages(self):
        return [args[0][0] for args in self.subject.logger.info.call_args_list]

    def get_error_messages(self):
        return [args[0][0] for args in self.subject.logger.error.call_args_list]

    def _get_env(self, arg,default=None):
        return self.os_environ[arg]


    def test_gpstate_output_when_netstate_ss_exists(self):
        sys.argv = ["gpstate", "-e"]
        gpstate = self.setup_gpstate()
        return_code = gpstate.run()
        messages = [msg[0][0] for msg in self.subject.logger.info.call_args_list]


if __name__ == '__main__':
    run_tests()