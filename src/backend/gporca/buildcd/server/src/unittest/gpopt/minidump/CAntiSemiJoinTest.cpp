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
#include "unittest/gpopt/minidump/CAntiSemiJoinTest.h"

using namespace gpopt;

ULONG CAntiSemiJoinTest::m_ulTestCounter = 0;  // start from first test

GPOS_RESULT
CAntiSemiJoinTest::EresUnittest()
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
CAntiSemiJoinTest::EresUnittest_RunTests()
{
	const CHAR *rgszMdpFiles[] =
	{
"../data/dxl/minidump/AntiSemiJoin2Select-1.mdp",
"../data/dxl/minidump/AntiSemiJoin2Select-2.mdp",
"../data/dxl/minidump/NOT-IN-NotNullBoth.mdp",
"../data/dxl/minidump/NOT-IN-NullInner.mdp",
"../data/dxl/minidump/NOT-IN-NullOuter.mdp",
"../data/dxl/minidump/Correlated-AntiSemiJoin.mdp",
"../data/dxl/minidump/CorrelatedAntiSemiJoin-True.mdp",
"../data/dxl/minidump/Correlated-LASJ-With-Outer-Col.mdp",
"../data/dxl/minidump/NotInToLASJ.mdp",
"../data/dxl/minidump/Correlated-LASJ-With-Outer-Const.mdp",
"../data/dxl/minidump/Correlated-LASJ-With-Outer-Expr.mdp",
"../data/dxl/minidump/LeftOuter2InnerUnionAllAntiSemiJoin-Tpcds.mdp",
	};

	return CTestUtils::EresUnittest_RunTests
						(
						rgszMdpFiles,
						&m_ulTestCounter,
						GPOS_ARRAY_SIZE(rgszMdpFiles)
						);
}

// EOF
