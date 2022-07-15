//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2018 Pivotal Software, Inc.
//---------------------------------------------------------------------------
#ifndef GPOPT_CDqaTest_H
#define GPOPT_CDqaTest_H

#include "gpos/base.h"

namespace gpopt
{
	class CDqaTest
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

	}; // class CDqaTest
}

#endif // !GPOPT_CDqaTest_H

// EOF

