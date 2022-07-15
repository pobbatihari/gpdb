//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2018 Pivotal Software, Inc.
//---------------------------------------------------------------------------
#ifndef GPOPT_CConstraintNegationTest_H
#define GPOPT_CConstraintNegationTest_H

#include "gpos/base.h"

namespace gpopt
{
	class CConstraintNegationTest
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

	}; // class CConstraintNegationTest
}

#endif // !GPOPT_CConstraintNegationTest_H

// EOF

