//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2018 Pivotal Software, Inc.
//---------------------------------------------------------------------------

#include "gpos/base.h"
#include "gpos/memory/CAutoMemoryPool.h"
#include "gpos/task/CAutoTraceFlag.h"
#include "gpos/test/CUnittest.h"
#include "gpopt/exception.h"
#include "gpopt/minidump/CMinidumperUtils.h"
#include "unittest/gpopt/CTestUtils.h"
#include "unittest/gpopt/minidump/CReplicatedTableTest.h"

using namespace gpopt;

ULONG CReplicatedTableTest::m_ulTestCounter = 0;  // start from first test

GPOS_RESULT
CReplicatedTableTest::EresUnittest()
{
	CUnittest rgut[] =
		{
		GPOS_UNITTEST_FUNC(EresUnittest_RunTests),
		};

	GPOS_RESULT eres = CUnittest::EresExecute(rgut, GPOS_ARRAY_SIZE(rgut));

	// reset metadata cache
	CMDCache::Reset();

	return eres;
}

// Run all Minidump-based tests with plan matching
GPOS_RESULT
CReplicatedTableTest::EresUnittest_RunTests()
{
	const CHAR *rgszMdpFiles[] =
	{
"../data/dxl/minidump/ReplicatedTableSelect.mdp",
"../data/dxl/minidump/ReplicatedTableInsert.mdp",
"../data/dxl/minidump/ReplicatedTable-CTAS.mdp",
"../data/dxl/minidump/ReplicatedHashJoinReplicated.mdp",
"../data/dxl/minidump/ReplicatedJoinHashDistributedTable.mdp",
"../data/dxl/minidump/ReplicatedJoinRandomDistributedTable.mdp",
"../data/dxl/minidump/ReplicatedLOJHashDistributedTable.mdp",
"../data/dxl/minidump/ReplicatedLOJRandomDistributedTable.mdp",
"../data/dxl/minidump/ReplicatedLOJReplicated.mdp",
"../data/dxl/minidump/ReplicatedNLJReplicated.mdp",
"../data/dxl/minidump/ReplicatedTableAggregate.mdp",
"../data/dxl/minidump/ReplicatedTableCTE.mdp",
"../data/dxl/minidump/ReplicatedTableGroupBy.mdp",
"../data/dxl/minidump/ReplicatedJoinPartitionedTable.mdp",
"../data/dxl/minidump/ReplicatedTableInClause.mdp",
"../data/dxl/minidump/ReplicatedTableSequenceInsert.mdp",
	};

	return CTestUtils::EresUnittest_RunTests
						(
						rgszMdpFiles,
						&m_ulTestCounter,
						GPOS_ARRAY_SIZE(rgszMdpFiles)
						);
}

// EOF
