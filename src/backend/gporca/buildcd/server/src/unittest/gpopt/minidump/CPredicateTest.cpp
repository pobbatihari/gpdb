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
#include "unittest/gpopt/minidump/CPredicateTest.h"

using namespace gpopt;

ULONG CPredicateTest::m_ulTestCounter = 0;  // start from first test

GPOS_RESULT
CPredicateTest::EresUnittest()
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
CPredicateTest::EresUnittest_RunTests()
{
	const CHAR *rgszMdpFiles[] =
	{
"../data/dxl/minidump/ConvertBoolConstNullToConstTableFalseFilter.mdp",
"../data/dxl/minidump/TranslateOneTimeFilterConjunctQuals.mdp",
"../data/dxl/minidump/TranslateFilterDisjunctQuals.mdp",
"../data/dxl/minidump/TranslateFilterWithCTEAndTableScanIntoFilterAndOneTimeFilter.mdp",
"../data/dxl/minidump/InferPredicates.mdp",
"../data/dxl/minidump/InferPredicatesForLimit.mdp",
"../data/dxl/minidump/InferPredicatesBCC-vc-vc.mdp",
"../data/dxl/minidump/InferPredicatesBCC-vc-txt.mdp",
"../data/dxl/minidump/InferPredicatesBCC-txt-txt.mdp",
"../data/dxl/minidump/InferPredicatesBCC-oid-oid.mdp",
"../data/dxl/minidump/InferPredicatesBCC-vcpart-txt.mdp",
"../data/dxl/minidump/OR-WithIsNullPred.mdp",
"../data/dxl/minidump/Int2Predicate.mdp",
"../data/dxl/minidump/IN-Numeric.mdp",
"../data/dxl/minidump/DeduplicatePredicates.mdp",
"../data/dxl/minidump/AddEqualityPredicates.mdp",
"../data/dxl/minidump/EqualityPredicateOverDate.mdp",
"../data/dxl/minidump/AddPredsInSubqueries.mdp",
"../data/dxl/minidump/ExtractPredicateFromDisjWithComputedColumns.mdp",
"../data/dxl/minidump/Join-With-Subq-Preds-1.mdp",
"../data/dxl/minidump/Non-Hashjoinable-Pred.mdp",
"../data/dxl/minidump/Non-Hashjoinable-Pred-2.mdp",
"../data/dxl/minidump/Factorized-Preds.mdp",
"../data/dxl/minidump/IN.mdp",
"../data/dxl/minidump/OR.mdp",
"../data/dxl/minidump/AvoidConstraintDerivationForLike.mdp",
"../data/dxl/minidump/NLJ-DistCol-No-Broadcast.mdp",
"../data/dxl/minidump/NLJ-EqAllCol-No-Broadcast.mdp",
"../data/dxl/minidump/NLJ-EqDistCol-InEqNonDistCol-No-Broadcast.mdp",
"../data/dxl/minidump/NLJ-InEqDistCol-EqNonDistCol-Redistribute.mdp",
"../data/dxl/minidump/CorrelatedNLJWithTrueCondition.mdp",
"../data/dxl/minidump/InferPredicatesInnerOfLOJ.mdp",
"../data/dxl/minidump/InferredPredicatesConstraintSimplification.mdp",
"../data/dxl/minidump/NoPushdownPredicateWithCTEAnchor.mdp",
"../data/dxl/minidump/InferPredicatesForPartSQ.mdp",
"../data/dxl/minidump/InferPredicatesForQuantifiedSQ.mdp",
"../data/dxl/minidump/DoubleNDVCardinalityEquals.mdp",
"../data/dxl/minidump/NotWellDefinedDisjunctConjunctPredicates.mdp",
	};

	return CTestUtils::EresUnittest_RunTests
						(
						rgszMdpFiles,
						&m_ulTestCounter,
						GPOS_ARRAY_SIZE(rgszMdpFiles)
						);
}

// EOF
