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
#include "unittest/gpopt/minidump/CSemiJoinTest.h"

using namespace gpopt;

ULONG CSemiJoinTest::m_ulTestCounter = 0;  // start from first test

GPOS_RESULT
CSemiJoinTest::EresUnittest()
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
CSemiJoinTest::EresUnittest_RunTests()
{
	const CHAR *rgszMdpFiles[] =
	{
"../data/dxl/minidump/SemiJoin2InnerJoin.mdp",
"../data/dxl/minidump/SemiJoin2Select-1.mdp",
"../data/dxl/minidump/SemiJoin2Select-2.mdp",
"../data/dxl/minidump/SemiJoin2Select-EnforceSubplan.mdp",
"../data/dxl/minidump/InSubqWithPrjListOuterRefNoInnerRefColumnIn.mdp",
"../data/dxl/minidump/InSubqWithPrjListOuterRefNoInnerRefConstIn.mdp",
"../data/dxl/minidump/NestedInSubqWithPrjListOuterRefNoInnerRef.mdp",
"../data/dxl/minidump/InEqualityJoin.mdp",
"../data/dxl/minidump/Correlated-SemiJoin.mdp",
"../data/dxl/minidump/CorrelatedSemiJoin-True.mdp",
"../data/dxl/minidump/CorrelatedIN-LeftSemiJoin-True.mdp",
"../data/dxl/minidump/CorrelatedIN-LeftSemiNotIn-True.mdp",
"../data/dxl/minidump/InSubqWithPrjListOuterRefNoInnerRefColumnPlusConstIn.mdp",
"../data/dxl/minidump/CorrelatedIN-LeftSemiJoin-Limit.mdp",
"../data/dxl/minidump/CorrelatedLeftSemiNLJoinWithLimit.mdp",
"../data/dxl/minidump/PushFilterToSemiJoinLeftChild.mdp",
"../data/dxl/minidump/SubqOuterReferenceInClause.mdp",
	};

	return CTestUtils::EresUnittest_RunTests
						(
						rgszMdpFiles,
						&m_ulTestCounter,
						GPOS_ARRAY_SIZE(rgszMdpFiles)
						);
}

// EOF
