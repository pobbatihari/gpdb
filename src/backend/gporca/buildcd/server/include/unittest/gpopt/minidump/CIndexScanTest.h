//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2018 Pivotal Software, Inc.
//---------------------------------------------------------------------------
#ifndef GPOPT_CIndexScanTest_H
#define GPOPT_CIndexScanTest_H

#include "gpos/base.h"

namespace gpopt
{
	class CIndexScanTest
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

	}; // class CIndexScanTest
}

#endif // !GPOPT_CIndexScanTest_H

// EOF

