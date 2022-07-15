//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2018 Pivotal Software, Inc.
//---------------------------------------------------------------------------
#ifndef GPOPT_CSetop4Test_H
#define GPOPT_CSetop4Test_H

#include "gpos/base.h"

namespace gpopt
{
	class CSetop4Test
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

	}; // class CSetop4Test
}

#endif // !GPOPT_CSetop4Test_H

// EOF

