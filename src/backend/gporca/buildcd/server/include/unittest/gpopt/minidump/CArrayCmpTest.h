//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2018 Pivotal Software, Inc.
//---------------------------------------------------------------------------
#ifndef GPOPT_CArrayCmpTest_H
#define GPOPT_CArrayCmpTest_H

#include "gpos/base.h"

namespace gpopt
{
	class CArrayCmpTest
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

	}; // class CArrayCmpTest
}

#endif // !GPOPT_CArrayCmpTest_H

// EOF

