//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2018 Pivotal Software, Inc.
//---------------------------------------------------------------------------
#ifndef GPOPT_CIndexScanOnLeafTest_H
#define GPOPT_CIndexScanOnLeafTest_H

#include "gpos/base.h"

namespace gpopt
{
	class CIndexScanOnLeafTest
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

	}; // class CIndexScanOnLeafTest
}

#endif // !GPOPT_CIndexScanOnLeafTest_H

// EOF

