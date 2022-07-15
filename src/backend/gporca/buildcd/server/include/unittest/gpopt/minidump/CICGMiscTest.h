//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2018 Pivotal Software, Inc.
//---------------------------------------------------------------------------
#ifndef GPOPT_CICGMiscTest_H
#define GPOPT_CICGMiscTest_H

#include "gpos/base.h"

namespace gpopt
{
	class CICGMiscTest
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

	}; // class CICGMiscTest
}

#endif // !GPOPT_CICGMiscTest_H

// EOF

