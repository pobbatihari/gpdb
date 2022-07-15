//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2018 Pivotal Software, Inc.
//---------------------------------------------------------------------------
#ifndef GPOPT_CSetop2Test_H
#define GPOPT_CSetop2Test_H

#include "gpos/base.h"

namespace gpopt
{
	class CSetop2Test
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

	}; // class CSetop2Test
}

#endif // !GPOPT_CSetop2Test_H

// EOF

