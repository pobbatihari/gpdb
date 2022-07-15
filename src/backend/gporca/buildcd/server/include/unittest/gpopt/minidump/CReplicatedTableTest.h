//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2018 Pivotal Software, Inc.
//---------------------------------------------------------------------------
#ifndef GPOPT_CReplicatedTableTest_H
#define GPOPT_CReplicatedTableTest_H

#include "gpos/base.h"

namespace gpopt
{
	class CReplicatedTableTest
	{
		private:

			// counter used to mark last successful test
			static
			gpos::ULONG m_ulTestCounter;

		public:

			// unittests
			static
			gpos::GPOS_RESULT EresUnittest();

			static
			gpos::GPOS_RESULT EresUnittest_RunTests();

	}; // class CReplicatedTableTest
}

#endif // !GPOPT_CReplicatedTableTest_H

// EOF

