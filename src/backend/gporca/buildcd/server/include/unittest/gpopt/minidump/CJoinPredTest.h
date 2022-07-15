//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2018 Pivotal Software, Inc.
//---------------------------------------------------------------------------
#ifndef GPOPT_CJoinPredTest_H
#define GPOPT_CJoinPredTest_H

#include "gpos/base.h"

namespace gpopt
{
	class CJoinPredTest
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

	}; // class CJoinPredTest
}

#endif // !GPOPT_CJoinPredTest_H

// EOF

