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
#include "unittest/gpopt/minidump/CMCVCardinalityTest.h"

using namespace gpopt;

ULONG CMCVCardinalityTest::m_ulTestCounter = 0;  // start from first test

GPOS_RESULT
CMCVCardinalityTest::EresUnittest()
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
CMCVCardinalityTest::EresUnittest_RunTests()
{
	const CHAR *rgszMdpFiles[] =
	{
"../data/dxl/minidump/BpCharMCVCardinalityEquals.mdp",
"../data/dxl/minidump/BpCharMCVCardinalityGreaterThan.mdp",
"../data/dxl/minidump/TextMCVCardinalityEquals.mdp",
"../data/dxl/minidump/TextMCVCardinalityGreaterThan.mdp",
"../data/dxl/minidump/VarcharMCVCardinalityEquals.mdp",
"../data/dxl/minidump/VarcharMCVCardinalityGreaterThan.mdp",
"../data/dxl/minidump/Citext-Cardinality.mdp",
"../data/dxl/minidump/Name-Cardinality.mdp",
"../data/dxl/minidump/Char-Cardinality.mdp",
	};

	return CTestUtils::EresUnittest_RunTests
						(
						rgszMdpFiles,
						&m_ulTestCounter,
						GPOS_ARRAY_SIZE(rgszMdpFiles)
						);
}

// EOF
