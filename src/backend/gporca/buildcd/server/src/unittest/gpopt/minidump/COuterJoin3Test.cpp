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
#include "unittest/gpopt/minidump/COuterJoin3Test.h"

using namespace gpopt;

ULONG COuterJoin3Test::m_ulTestCounter = 0;  // start from first test

GPOS_RESULT
COuterJoin3Test::EresUnittest()
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
COuterJoin3Test::EresUnittest_RunTests()
{
	const CHAR *rgszMdpFiles[] =
	{
"../data/dxl/minidump/LOJ_IDF_no_convert_outer_ref_predicate_with_NULL.mdp",
"../data/dxl/minidump/LOJ_IDF_no_convert_outer_ref_predicate_with_inner_ref.mdp",
"../data/dxl/minidump/LOJ_convert_to_inner_with_inner_predicate_IDF.mdp",
"../data/dxl/minidump/LOJ_dont_convert_to_inner_with_inner_predicate_IDF_const.mdp",
"../data/dxl/minidump/LOJ_dont_convert_to_inner_with_outer_predicate_INDF.mdp",
"../data/dxl/minidump/LOJ_dont_convert_to_inner_with_outer_predicate_INDF_const.mdp",
"../data/dxl/minidump/LOJ-With-Agg.mdp",
"../data/dxl/minidump/LOJReorderWithComplexPredicate.mdp",
"../data/dxl/minidump/LOJReorderComplexNestedLOJs.mdp",
"../data/dxl/minidump/LOJReorderWithIDF.mdp",
"../data/dxl/minidump/LOJReorderWithNestedLOJAndFilter.mdp",
"../data/dxl/minidump/LOJReorderWithSimplePredicate.mdp",
"../data/dxl/minidump/LOJReorderWithOneSidedFilter.mdp",
"../data/dxl/minidump/LOJ-HashJoin-MultiDistKeys-WithComplexPreds.mdp",
"../data/dxl/minidump/LOJ_convert_to_inner_with_and_predicate.mdp",
"../data/dxl/minidump/LOJ_convert_to_inner_with_or_predicate.mdp",
"../data/dxl/minidump/LOJNonNullRejectingPredicates.mdp",
"../data/dxl/minidump/LOJNullRejectingPredicates.mdp",
"../data/dxl/minidump/LOJNullRejectingZeroPlacePredicates.mdp",
	};

	return CTestUtils::EresUnittest_RunTests
						(
						rgszMdpFiles,
						&m_ulTestCounter,
						GPOS_ARRAY_SIZE(rgszMdpFiles)
						);
}

// EOF
