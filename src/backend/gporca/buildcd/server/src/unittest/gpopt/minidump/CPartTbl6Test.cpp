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
#include "unittest/gpopt/minidump/CPartTbl6Test.h"

using namespace gpopt;

ULONG CPartTbl6Test::m_ulTestCounter = 0;  // start from first test

GPOS_RESULT
CPartTbl6Test::EresUnittest()
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
CPartTbl6Test::EresUnittest_RunTests()
{
	const CHAR *rgszMdpFiles[] =
	{
"../data/dxl/minidump/PartTbl-PredicateWithCast.mdp",
"../data/dxl/minidump/PartTbl-IDFList.mdp",
"../data/dxl/minidump/PartTbl-IDFNull.mdp",
"../data/dxl/minidump/PartTbl-PredicateWithCast.mdp",
"../data/dxl/minidump/PartTbl-EqPredicateWithCastRange.mdp",
"../data/dxl/minidump/PartTbl-PredicateWithCastList.mdp",
"../data/dxl/minidump/PartTbl-IDFWithCast.mdp",
"../data/dxl/minidump/PartTbl-PredicateWithCastMultiLevelList.mdp",
"../data/dxl/minidump/PartTbl-WindowFuncPredPushDown.mdp",
"../data/dxl/minidump/PartTbl-WindowFuncPartialPredPushDown.mdp",
"../data/dxl/minidump/PartTbl-WindowFuncNoPredPushDown.mdp",
"../data/dxl/minidump/PartTbl-WindowFuncNoDisjunctPredPushDown.mdp",
"../data/dxl/minidump/PartTbl-WindowFuncSinglePredPushDown.mdp",
	};

	return CTestUtils::EresUnittest_RunTests
						(
						rgszMdpFiles,
						&m_ulTestCounter,
						GPOS_ARRAY_SIZE(rgszMdpFiles)
						);
}

// EOF
