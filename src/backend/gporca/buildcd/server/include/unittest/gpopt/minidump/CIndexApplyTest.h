//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2018 Pivotal Software, Inc.
//---------------------------------------------------------------------------
#ifndef GPOPT_CIndexApplyTest_H
#define GPOPT_CIndexApplyTest_H

#include "gpos/base.h"

namespace gpopt
{
	class CIndexApplyTest
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

	}; // class CIndexApplyTest
}

#endif // !GPOPT_CIndexApplyTest_H

// EOF

