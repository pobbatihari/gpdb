//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2018 Pivotal Software, Inc.
//---------------------------------------------------------------------------
#ifndef GPOPT_CBitmapScanTest_H
#define GPOPT_CBitmapScanTest_H

#include "gpos/base.h"

namespace gpopt
{
	class CBitmapScanTest
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

	}; // class CBitmapScanTest
}

#endif // !GPOPT_CBitmapScanTest_H

// EOF

