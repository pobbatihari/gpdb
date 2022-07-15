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
#include "unittest/gpopt/minidump/CFullJoinTest.h"

using namespace gpopt;

ULONG CFullJoinTest::m_ulTestCounter = 0;  // start from first test

GPOS_RESULT
CFullJoinTest::EresUnittest()
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
CFullJoinTest::EresUnittest_RunTests()
{
	const CHAR *rgszMdpFiles[] =
	{
"../data/dxl/minidump/FOJ-Condition-False.mdp",
"../data/dxl/minidump/ExpandFullOuterJoin.mdp",
"../data/dxl/minidump/ExpandFullOuterJoin2.mdp",
"../data/dxl/minidump/FullOuterJoin-NullRejectingLHS1.mdp",
"../data/dxl/minidump/FullOuterJoin-NullRejectingLHS2.mdp",
"../data/dxl/minidump/FullJoin-InnerNotOnDistributionColumn.mdp",
"../data/dxl/minidump/FullJoin-NotOnDistributionColumn.mdp",
"../data/dxl/minidump/FullJoin-2PredicateOnDistColumns.mdp",
"../data/dxl/minidump/FullJoin-OnDistributionColumn.mdp",
"../data/dxl/minidump/FullJoin-SwappedPredicates.mdp",
"../data/dxl/minidump/FullJoin-Replicated.mdp",
"../data/dxl/minidump/FullJoin-SubqueryWithRedistribute.mdp",
"../data/dxl/minidump/FullJoin-SubquerySingleton.mdp",
"../data/dxl/minidump/FullJoin-Subquery-CastedPredicates.mdp",
"../data/dxl/minidump/FullJoin-Caps.mdp",
	};

	return CTestUtils::EresUnittest_RunTests
						(
						rgszMdpFiles,
						&m_ulTestCounter,
						GPOS_ARRAY_SIZE(rgszMdpFiles)
						);
}

// EOF
