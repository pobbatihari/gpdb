//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2018 Pivotal Software, Inc.
//---------------------------------------------------------------------------
#ifndef GPOPT_CMotionHazardTest_H
#define GPOPT_CMotionHazardTest_H

#include "gpos/base.h"

namespace gpopt
{
	class CMotionHazardTest
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

	}; // class CMotionHazardTest
}

#endif // !GPOPT_CMotionHazardTest_H

// EOF

