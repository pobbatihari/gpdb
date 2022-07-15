//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2018 Pivotal Software, Inc.
//---------------------------------------------------------------------------
#ifndef GPOPT_CTypeModifierTest_H
#define GPOPT_CTypeModifierTest_H

#include "gpos/base.h"

namespace gpopt
{
	class CTypeModifierTest
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

	}; // class CTypeModifierTest
}

#endif // !GPOPT_CTypeModifierTest_H

// EOF

