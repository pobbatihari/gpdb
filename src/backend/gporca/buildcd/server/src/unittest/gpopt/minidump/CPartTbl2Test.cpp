//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2018 Pivotal Software, Inc.
//---------------------------------------------------------------------------

#include "gpos/base.h"
#include "gpos/memory/CAutoMemoryPool.h"
#include "gpos/task/CAutoTraceFlag.h"
#include "gpos/test/CUnittest.h"
#include "gpopt/exception.h"
#include "gpopt/minidump/CMinidumperUtils.h"
#include "unittest/gpopt/CTestUtils.h"
#include "unittest/gpopt/minidump/CPartTbl2Test.h"

using namespace gpopt;

ULONG CPartTbl2Test::m_ulTestCounter = 0;  // start from first test

GPOS_RESULT
CPartTbl2Test::EresUnittest()
{
	CUnittest rgut[] =
		{
		GPOS_UNITTEST_FUNC(EresUnittest_RunTests),
		};

	GPOS_RESULT eres = CUnittest::EresExecute(rgut, GPOS_ARRAY_SIZE(rgut));

	// reset metadata cache
	CMDCache::Reset();

	return eres;
}

// Run all Minidump-based tests with plan matching
GPOS_RESULT
CPartTbl2Test::EresUnittest_RunTests()
{
	const CHAR *rgszMdpFiles[] =
	{
"../data/dxl/minidump/Part-Selection-IN.mdp",
"../data/dxl/minidump/Part-Selection-NOT-IN.mdp",
"../data/dxl/minidump/Part-Selection-ConstArray-1.mdp",
"../data/dxl/minidump/Part-Selection-ConstArray-2.mdp",
"../data/dxl/minidump/PartTbl-WindowFunction.mdp",
"../data/dxl/minidump/PartTbl-MultiWayJoin.mdp",
"../data/dxl/minidump/PartTbl-AsymmetricRangePredicate.mdp",
"../data/dxl/minidump/PartTbl-NEqPredicate.mdp",
"../data/dxl/minidump/PartTbl-SQExists.mdp",
	};

	return CTestUtils::EresUnittest_RunTests
						(
						rgszMdpFiles,
						&m_ulTestCounter,
						GPOS_ARRAY_SIZE(rgszMdpFiles)
						);
}

// EOF
