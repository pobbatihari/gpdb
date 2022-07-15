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
#include "unittest/gpopt/minidump/CSubquery2Test.h"

using namespace gpopt;

ULONG CSubquery2Test::m_ulTestCounter = 0;  // start from first test

GPOS_RESULT
CSubquery2Test::EresUnittest()
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
CSubquery2Test::EresUnittest_RunTests()
{
	const CHAR *rgszMdpFiles[] =
	{
"../data/dxl/minidump/Subq2PartialDecorrelate.mdp",
"../data/dxl/minidump/Subq2CorrSQInLOJOn.mdp",
"../data/dxl/minidump/Subq2NotInWhereLOJ.mdp",
"../data/dxl/minidump/Subq2OuterRef2InJoin.mdp",
"../data/dxl/minidump/Subq2OuterRefMultiLevelInOn.mdp",
"../data/dxl/minidump/Index-Join-With-Subquery-In-Pred.mdp",
"../data/dxl/minidump/NestedSubqLimitBindings.mdp",
	};

	return CTestUtils::EresUnittest_RunTests
						(
						rgszMdpFiles,
						&m_ulTestCounter,
						GPOS_ARRAY_SIZE(rgszMdpFiles)
						);
}

// EOF
