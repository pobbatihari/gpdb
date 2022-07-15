//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2018 Pivotal Software, Inc.
//---------------------------------------------------------------------------
#ifndef GPOPT_CDynamicIndexScanTest_H
#define GPOPT_CDynamicIndexScanTest_H

#include "gpos/base.h"

namespace gpopt
{
	class CDynamicIndexScanTest
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

	}; // class CDynamicIndexScanTest
}

#endif // !GPOPT_CDynamicIndexScanTest_H

// EOF

