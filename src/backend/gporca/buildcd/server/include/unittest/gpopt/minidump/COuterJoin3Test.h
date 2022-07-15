//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2018 Pivotal Software, Inc.
//---------------------------------------------------------------------------
#ifndef GPOPT_COuterJoin3Test_H
#define GPOPT_COuterJoin3Test_H

#include "gpos/base.h"

namespace gpopt
{
	class COuterJoin3Test
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

	}; // class COuterJoin3Test
}

#endif // !GPOPT_COuterJoin3Test_H

// EOF

