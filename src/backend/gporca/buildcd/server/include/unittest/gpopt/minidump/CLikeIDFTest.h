//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2018 Pivotal Software, Inc.
//---------------------------------------------------------------------------
#ifndef GPOPT_CLikeIDFTest_H
#define GPOPT_CLikeIDFTest_H

#include "gpos/base.h"

namespace gpopt
{
	class CLikeIDFTest
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

	}; // class CLikeIDFTest
}

#endif // !GPOPT_CLikeIDFTest_H

// EOF

