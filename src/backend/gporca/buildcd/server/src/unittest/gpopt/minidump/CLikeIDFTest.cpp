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
#include "unittest/gpopt/minidump/CLikeIDFTest.h"

using namespace gpopt;

ULONG CLikeIDFTest::m_ulTestCounter = 0;  // start from first test

GPOS_RESULT
CLikeIDFTest::EresUnittest()
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
CLikeIDFTest::EresUnittest_RunTests()
{
	const CHAR *rgszMdpFiles[] =
	{
"../data/dxl/minidump/LIKE-Pattern-green.mdp",
"../data/dxl/minidump/LIKE-Pattern-green-2.mdp",
"../data/dxl/minidump/LIKE-Pattern-Empty.mdp",
"../data/dxl/minidump/Nested-Or-Predicates.mdp",
"../data/dxl/minidump/Join-IDF.mdp",
"../data/dxl/minidump/LikePredStatsNotComparable.mdp",
"../data/dxl/minidump/PredStatsNotComparable.mdp",
"../data/dxl/minidump/ExtractPredicateFromDisj.mdp",
"../data/dxl/minidump/CollapseNot.mdp",
"../data/dxl/minidump/IDF-NotNullConstant.mdp",
"../data/dxl/minidump/IDF-NullConstant.mdp",
"../data/dxl/minidump/INDF-NotNullConstant.mdp",
"../data/dxl/minidump/INDF-NullConstant.mdp",
"../data/dxl/minidump/NullConstant-INDF-Col.mdp",
"../data/dxl/minidump/Join-INDF.mdp",
"../data/dxl/minidump/Join-INDF-NoBroadcast.mdp",
"../data/dxl/minidump/TimeTypeStatsNotComparable.mdp",
"../data/dxl/minidump/Join-INDF-Nulls-Not-Collocated.mdp",
	};

	return CTestUtils::EresUnittest_RunTests
						(
						rgszMdpFiles,
						&m_ulTestCounter,
						GPOS_ARRAY_SIZE(rgszMdpFiles)
						);
}

// EOF
