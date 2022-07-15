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
#include "unittest/gpopt/minidump/CRightJoinTest.h"

using namespace gpopt;

ULONG CRightJoinTest::m_ulTestCounter = 0;  // start from first test

GPOS_RESULT
CRightJoinTest::EresUnittest()
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
CRightJoinTest::EresUnittest_RunTests()
{
	const CHAR *rgszMdpFiles[] =
	{
"../data/dxl/minidump/RightJoinHashed.mdp",
"../data/dxl/minidump/RightJoinRedistribute.mdp",
"../data/dxl/minidump/RightJoinReplicated.mdp",
"../data/dxl/minidump/RightJoinBothReplicated.mdp",
"../data/dxl/minidump/RightJoinDPS.mdp",
"../data/dxl/minidump/RightJoinNoDPSNonDistKey.mdp",
"../data/dxl/minidump/RightJoinTVF.mdp",
	};

	return CTestUtils::EresUnittest_RunTests
						(
						rgszMdpFiles,
						&m_ulTestCounter,
						GPOS_ARRAY_SIZE(rgszMdpFiles)
						);
}

// EOF
