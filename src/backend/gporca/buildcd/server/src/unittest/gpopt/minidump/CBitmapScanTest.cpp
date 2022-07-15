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
#include "unittest/gpopt/minidump/CBitmapScanTest.h"

using namespace gpopt;

ULONG CBitmapScanTest::m_ulTestCounter = 0;  // start from first test

GPOS_RESULT
CBitmapScanTest::EresUnittest()
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
CBitmapScanTest::EresUnittest_RunTests()
{
	const CHAR *rgszMdpFiles[] =
	{
"../data/dxl/minidump/IndexedNLJBitmap.mdp",
"../data/dxl/minidump/BitmapIndex-ChooseHashJoin.mdp",
"../data/dxl/minidump/BitmapTableScan-AO-Btree-PickOnlyHighNDV.mdp",
"../data/dxl/minidump/BitmapIndex-Against-InList.mdp",
"../data/dxl/minidump/BitmapTableScan-AO-Btree-PickIndexWithNoGap.mdp",
"../data/dxl/minidump/BitmapTableScan-ComplexConjDisj.mdp",
"../data/dxl/minidump/BitmapTableScan-ConjDisjWithOuterRefs.mdp",
	};

	return CTestUtils::EresUnittest_RunTests
						(
						rgszMdpFiles,
						&m_ulTestCounter,
						GPOS_ARRAY_SIZE(rgszMdpFiles)
						);
}

// EOF
