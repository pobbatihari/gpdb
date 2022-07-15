//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2018 Pivotal Software, Inc.
//---------------------------------------------------------------------------
#ifndef GPOPT_COpfamiliesTest_H
#define GPOPT_COpfamiliesTest_H

#include "gpos/base.h"

namespace gpopt
{
	class COpfamiliesTest
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

	}; // class COpfamiliesTest
}

#endif // !GPOPT_COpfamiliesTest_H

// EOF

