//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2018 Pivotal Software, Inc.
//---------------------------------------------------------------------------
#ifndef GPOPT_CLeftOuterIndexApplyTest_H
#define GPOPT_CLeftOuterIndexApplyTest_H

#include "gpos/base.h"

namespace gpopt
{
	class CLeftOuterIndexApplyTest
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

	}; // class CLeftOuterIndexApplyTest
}

#endif // !GPOPT_CLeftOuterIndexApplyTest_H

// EOF

