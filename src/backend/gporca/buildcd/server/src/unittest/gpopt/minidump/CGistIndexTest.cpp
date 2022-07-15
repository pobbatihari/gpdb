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
#include "unittest/gpopt/minidump/CGistIndexTest.h"

using namespace gpopt;

ULONG CGistIndexTest::m_ulTestCounter = 0;  // start from first test

GPOS_RESULT
CGistIndexTest::EresUnittest()
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
CGistIndexTest::EresUnittest_RunTests()
{
	const CHAR *rgszMdpFiles[] =
	{
"../data/dxl/minidump/Gist-AOCOTable-NonLossy-BitmapIndexPlan.mdp",
"../data/dxl/minidump/Gist-NonPart-Lossy-BitmapIndexPlan.mdp",
"../data/dxl/minidump/Gist-NonPart-Lossy-IndexPlan.mdp",
"../data/dxl/minidump/Gist-PartTable-Lossy-IndexPlan.mdp",
"../data/dxl/minidump/Gist-PartTable-NonLossy-BitmapIndexPlan.mdp",
"../data/dxl/minidump/Gist-PartialIndex-TableScan.mdp",
"../data/dxl/minidump/Gist-NestedLoopJoin-Lossy-IndexPlan.mdp",
"../data/dxl/minidump/Gist-NestedLoopJoin-Postgis-IndexPlan.mdp",
"../data/dxl/minidump/Gist-OrderBy-BitmapPlan.mdp",
"../data/dxl/minidump/Gist-OrderBy-IndexPlan.mdp",
	};

	return CTestUtils::EresUnittest_RunTests
						(
						rgszMdpFiles,
						&m_ulTestCounter,
						GPOS_ARRAY_SIZE(rgszMdpFiles)
						);
}

// EOF
