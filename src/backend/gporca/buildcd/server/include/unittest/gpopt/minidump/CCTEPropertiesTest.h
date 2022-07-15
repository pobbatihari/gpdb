//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2018 Pivotal Software, Inc.
//---------------------------------------------------------------------------
#ifndef GPOPT_CCTEPropertiesTest_H
#define GPOPT_CCTEPropertiesTest_H

#include "gpos/base.h"

namespace gpopt
{
	class CCTEPropertiesTest
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

	}; // class CCTEPropertiesTest
}

#endif // !GPOPT_CCTEPropertiesTest_H

// EOF

