//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2018 Pivotal Software, Inc.
//---------------------------------------------------------------------------
#ifndef GPOPT_CAntiSemiJoinTest_H
#define GPOPT_CAntiSemiJoinTest_H

#include "gpos/base.h"

namespace gpopt
{
	class CAntiSemiJoinTest
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

	}; // class CAntiSemiJoinTest
}

#endif // !GPOPT_CAntiSemiJoinTest_H

// EOF

