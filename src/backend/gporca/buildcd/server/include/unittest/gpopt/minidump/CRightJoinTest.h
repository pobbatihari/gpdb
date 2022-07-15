//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2018 Pivotal Software, Inc.
//---------------------------------------------------------------------------
#ifndef GPOPT_CRightJoinTest_H
#define GPOPT_CRightJoinTest_H

#include "gpos/base.h"

namespace gpopt
{
	class CRightJoinTest
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

	}; // class CRightJoinTest
}

#endif // !GPOPT_CRightJoinTest_H

// EOF

