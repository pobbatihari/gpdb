//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2018 Pivotal Software, Inc.
//---------------------------------------------------------------------------
#ifndef GPOPT_CSubquery2Test_H
#define GPOPT_CSubquery2Test_H

#include "gpos/base.h"

namespace gpopt
{
	class CSubquery2Test
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

	}; // class CSubquery2Test
}

#endif // !GPOPT_CSubquery2Test_H

// EOF

