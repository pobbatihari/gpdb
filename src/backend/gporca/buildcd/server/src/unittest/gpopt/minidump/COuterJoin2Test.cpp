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
#include "unittest/gpopt/minidump/COuterJoin2Test.h"

using namespace gpopt;

ULONG COuterJoin2Test::m_ulTestCounter = 0;  // start from first test

GPOS_RESULT
COuterJoin2Test::EresUnittest()
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
COuterJoin2Test::EresUnittest_RunTests()
{
	const CHAR *rgszMdpFiles[] =
	{
"../data/dxl/minidump/LOJ-IsNullPred.mdp",
"../data/dxl/minidump/Select-Proj-OuterJoin.mdp",
"../data/dxl/minidump/OuterJoin-With-OuterRefs.mdp",
"../data/dxl/minidump/Join-Disj-Subqs.mdp",
"../data/dxl/minidump/EffectOfLocalPredOnJoin.mdp",
"../data/dxl/minidump/EffectOfLocalPredOnJoin2.mdp",
"../data/dxl/minidump/EffectOfLocalPredOnJoin3.mdp",
"../data/dxl/minidump/LeftJoin-UnsupportedFilter-Cardinality.mdp",
"../data/dxl/minidump/LeftOuter2InnerUnionAllAntiSemiJoin.mdp",
	};

	return CTestUtils::EresUnittest_RunTests
						(
						rgszMdpFiles,
						&m_ulTestCounter,
						GPOS_ARRAY_SIZE(rgszMdpFiles)
						);
}

// EOF
