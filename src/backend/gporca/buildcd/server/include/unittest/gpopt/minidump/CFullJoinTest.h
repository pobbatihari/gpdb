//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2018 Pivotal Software, Inc.
//---------------------------------------------------------------------------
#ifndef GPOPT_CFullJoinTest_H
#define GPOPT_CFullJoinTest_H

#include "gpos/base.h"

namespace gpopt
{
	class CFullJoinTest
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

	}; // class CFullJoinTest
}

#endif // !GPOPT_CFullJoinTest_H

// EOF

