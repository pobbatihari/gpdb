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
#include "unittest/gpopt/minidump/CSetop1Test.h"

using namespace gpopt;

ULONG CSetop1Test::m_ulTestCounter = 0;  // start from first test

GPOS_RESULT
CSetop1Test::EresUnittest()
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
CSetop1Test::EresUnittest_RunTests()
{
	const CHAR *rgszMdpFiles[] =
	{
"../data/dxl/minidump/ValueScanWithDuplicateAndSelfComparison.mdp",
"../data/dxl/minidump/PushGbBelowNaryUnionAll.mdp",
"../data/dxl/minidump/PushGbBelowNaryUnion-1.mdp",
"../data/dxl/minidump/PushGbBelowNaryUnion-2.mdp",
"../data/dxl/minidump/MS-UnionAll-1.mdp",
"../data/dxl/minidump/MS-UnionAll-5.mdp",
"../data/dxl/minidump/MS-UnionAll-6.mdp",
"../data/dxl/minidump/MS-UnionAll-7.mdp",
"../data/dxl/minidump/ManyTextUnionsInSubquery.mdp",
	};

	return CTestUtils::EresUnittest_RunTests
						(
						rgszMdpFiles,
						&m_ulTestCounter,
						GPOS_ARRAY_SIZE(rgszMdpFiles)
						);
}

// EOF
