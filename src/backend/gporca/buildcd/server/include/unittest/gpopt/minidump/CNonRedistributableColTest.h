//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2018 Pivotal Software, Inc.
//---------------------------------------------------------------------------
#ifndef GPOPT_CNonRedistributableColTest_H
#define GPOPT_CNonRedistributableColTest_H

#include "gpos/base.h"

namespace gpopt
{
	class CNonRedistributableColTest
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

	}; // class CNonRedistributableColTest
}

#endif // !GPOPT_CNonRedistributableColTest_H

// EOF

