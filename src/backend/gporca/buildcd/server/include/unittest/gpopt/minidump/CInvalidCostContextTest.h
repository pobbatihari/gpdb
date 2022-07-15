//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2018 Pivotal Software, Inc.
//---------------------------------------------------------------------------
#ifndef GPOPT_CInvalidCostContextTest_H
#define GPOPT_CInvalidCostContextTest_H

#include "gpos/base.h"

namespace gpopt
{
	class CInvalidCostContextTest
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

	}; // class CInvalidCostContextTest
}

#endif // !GPOPT_CInvalidCostContextTest_H

// EOF

