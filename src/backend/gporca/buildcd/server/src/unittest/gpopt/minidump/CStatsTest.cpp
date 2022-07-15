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
#include "unittest/gpopt/minidump/CStatsTest.h"

using namespace gpopt;

ULONG CStatsTest::m_ulTestCounter = 0;  // start from first test

GPOS_RESULT
CStatsTest::EresUnittest()
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
CStatsTest::EresUnittest_RunTests()
{
	const CHAR *rgszMdpFiles[] =
	{
"../data/dxl/minidump/Stat-Derivation-Leaf-Pattern.mdp",
"../data/dxl/minidump/MissingBoolColStats.mdp",
"../data/dxl/minidump/JoinColWithOnlyNDV.mdp",
"../data/dxl/minidump/UnsupportedStatsPredicate.mdp",
"../data/dxl/minidump/StatsFilter-AnyWithNewColStats.mdp",
"../data/dxl/minidump/EquiJoinOnExpr-Supported.mdp",
"../data/dxl/minidump/EquiJoinOnExpr-Unsupported.mdp",
	};

	return CTestUtils::EresUnittest_RunTests
						(
						rgszMdpFiles,
						&m_ulTestCounter,
						GPOS_ARRAY_SIZE(rgszMdpFiles)
						);
}

// EOF
