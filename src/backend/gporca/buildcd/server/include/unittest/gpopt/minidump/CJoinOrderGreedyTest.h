//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2018 Pivotal Software, Inc.
//---------------------------------------------------------------------------
#ifndef GPOPT_CJoinOrderGreedyTest_H
#define GPOPT_CJoinOrderGreedyTest_H

#include "gpos/base.h"

namespace gpopt
{
	class CJoinOrderGreedyTest
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

	}; // class CJoinOrderGreedyTest
}

#endif // !GPOPT_CJoinOrderGreedyTest_H

// EOF

