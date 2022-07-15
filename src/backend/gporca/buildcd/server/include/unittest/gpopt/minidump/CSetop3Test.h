//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2018 Pivotal Software, Inc.
//---------------------------------------------------------------------------
#ifndef GPOPT_CSetop3Test_H
#define GPOPT_CSetop3Test_H

#include "gpos/base.h"

namespace gpopt
{
	class CSetop3Test
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

	}; // class CSetop3Test
}

#endif // !GPOPT_CSetop3Test_H

// EOF

