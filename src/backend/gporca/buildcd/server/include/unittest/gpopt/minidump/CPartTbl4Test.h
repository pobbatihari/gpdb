//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2018 Pivotal Software, Inc.
//---------------------------------------------------------------------------
#ifndef GPOPT_CPartTbl4Test_H
#define GPOPT_CPartTbl4Test_H

#include "gpos/base.h"

namespace gpopt
{
	class CPartTbl4Test
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

	}; // class CPartTbl4Test
}

#endif // !GPOPT_CPartTbl4Test_H

// EOF

