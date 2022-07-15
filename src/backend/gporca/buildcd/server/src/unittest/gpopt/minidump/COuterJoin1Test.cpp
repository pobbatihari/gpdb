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
#include "unittest/gpopt/minidump/COuterJoin1Test.h"

using namespace gpopt;

ULONG COuterJoin1Test::m_ulTestCounter = 0;  // start from first test

GPOS_RESULT
COuterJoin1Test::EresUnittest()
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
COuterJoin1Test::EresUnittest_RunTests()
{
	const CHAR *rgszMdpFiles[] =
	{
"../data/dxl/minidump/ExpandNAryJoinGreedyWithLOJOnly.mdp",
"../data/dxl/minidump/NaryWithLojAndNonLojChilds.mdp",
"../data/dxl/minidump/LOJ_bb_mpph.mdp",
"../data/dxl/minidump/LOJ-Condition-False.mdp",
"../data/dxl/minidump/LeftJoin-With-Pred-On-Inner.mdp",
"../data/dxl/minidump/LeftJoin-With-Pred-On-Inner2.mdp",
"../data/dxl/minidump/LeftJoin-With-Col-Const-Pred.mdp",
"../data/dxl/minidump/LeftJoin-With-Coalesce.mdp",
"../data/dxl/minidump/LOJWithFalsePred.mdp",
"../data/dxl/minidump/LeftJoin-DPv2-With-Select.mdp",
"../data/dxl/minidump/DPv2GreedyOnly.mdp",
"../data/dxl/minidump/DPv2MinCardOnly.mdp",
"../data/dxl/minidump/DPv2QueryOnly.mdp",
"../data/dxl/minidump/LOJ-PushDown.mdp",
"../data/dxl/minidump/LeftJoinDPv2JoinOrder.mdp",
	};

	return CTestUtils::EresUnittest_RunTests
						(
						rgszMdpFiles,
						&m_ulTestCounter,
						GPOS_ARRAY_SIZE(rgszMdpFiles)
						);
}

// EOF
