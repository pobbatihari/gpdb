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
#include "unittest/gpopt/minidump/CLeftOuterIndexApplyTest.h"

using namespace gpopt;

ULONG CLeftOuterIndexApplyTest::m_ulTestCounter = 0;  // start from first test

GPOS_RESULT
CLeftOuterIndexApplyTest::EresUnittest()
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
CLeftOuterIndexApplyTest::EresUnittest_RunTests()
{
	const CHAR *rgszMdpFiles[] =
	{
"../data/dxl/minidump/IndexApply-LeftOuter-NLJoin.mdp",
"../data/dxl/minidump/LOJ-IndexApply-MultiIndexes.mdp",
"../data/dxl/minidump/LOJ-IndexApply-Negative-NonEqual-Predicate.mdp",
"../data/dxl/minidump/LOJ-IndexApply-DistKey-Multiple-Predicates.mdp",
"../data/dxl/minidump/LOJ-IndexApply-MasterOnly-Table.mdp",
"../data/dxl/minidump/LOJ-IndexApply-NonDistKey.mdp",
"../data/dxl/minidump/LOJ-IndexApply-CompsiteKey-Equiv.mdp",
"../data/dxl/minidump/LOJ-IndexApply-CompsiteKey-NoMotion.mdp",
"../data/dxl/minidump/LOJ-IndexApply-MultiDistKeys-IndexKeys.mdp",
"../data/dxl/minidump/LOJ-IndexApply-WithComplexPredicates.mdp",
"../data/dxl/minidump/LOJ-IndexApply-MultiDistKeys-WithComplexPreds.mdp",
"../data/dxl/minidump/LOJ-IndexApply-MultiDistKey-MultiIndexKey-NoExtraFilter.mdp",
"../data/dxl/minidump/LOJ-IndexApply-MultiDistKey-MultiIndexKey.mdp",
"../data/dxl/minidump/LOJ-IndexApply-MultiDistKeys-Bitmap.mdp",
"../data/dxl/minidump/LOJ-IndexApply-MultiDistKeys-Bitmap-WithComplexPreds.mdp",
"../data/dxl/minidump/LOJ-DynBitmapIndex.mdp",
"../data/dxl/minidump/LOJ-DynBitmapIndexWithSelect.mdp",
"../data/dxl/minidump/LOJ-DynBtreeIndex.mdp",
"../data/dxl/minidump/LOJ-DynBtreeIndexWithSelect.mdp",
	};

	return CTestUtils::EresUnittest_RunTests
						(
						rgszMdpFiles,
						&m_ulTestCounter,
						GPOS_ARRAY_SIZE(rgszMdpFiles)
						);
}

// EOF
