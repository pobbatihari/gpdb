//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2018 Pivotal Software, Inc.
//---------------------------------------------------------------------------
#ifndef GPOPT_CMCVCardinalityTest_H
#define GPOPT_CMCVCardinalityTest_H

#include "gpos/base.h"

namespace gpopt
{
	class CMCVCardinalityTest
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

	}; // class CMCVCardinalityTest
}

#endif // !GPOPT_CMCVCardinalityTest_H

// EOF

