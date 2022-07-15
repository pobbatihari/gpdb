//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2018 Pivotal Software, Inc.
//---------------------------------------------------------------------------
#ifndef GPOPT_CGistIndexTest_H
#define GPOPT_CGistIndexTest_H

#include "gpos/base.h"

namespace gpopt
{
	class CGistIndexTest
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

	}; // class CGistIndexTest
}

#endif // !GPOPT_CGistIndexTest_H

// EOF

