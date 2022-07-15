//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2018 Pivotal Software, Inc.
//---------------------------------------------------------------------------
#ifndef GPOPT_CPartTbl5Test_H
#define GPOPT_CPartTbl5Test_H

#include "gpos/base.h"

namespace gpopt
{
	class CPartTbl5Test
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

	}; // class CPartTbl5Test
}

#endif // !GPOPT_CPartTbl5Test_H

// EOF

