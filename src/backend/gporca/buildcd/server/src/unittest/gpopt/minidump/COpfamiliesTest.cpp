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
#include "unittest/gpopt/minidump/COpfamiliesTest.h"

using namespace gpopt;

ULONG COpfamiliesTest::m_ulTestCounter = 0;  // start from first test

GPOS_RESULT
COpfamiliesTest::EresUnittest()
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
COpfamiliesTest::EresUnittest_RunTests()
{
	const CHAR *rgszMdpFiles[] =
	{
"../data/dxl/minidump/JoinCitextVarchar.mdp",
"../data/dxl/minidump/JoinDefaultOpfamiliesUsingNonDefaultOpfamilyOp.mdp",
"../data/dxl/minidump/MultiColumnAggWithDefaultOpfamilies.mdp",
"../data/dxl/minidump/JoinTinterval.mdp",
"../data/dxl/minidump/FullJoin-NonDefaultOpfamily.mdp",
"../data/dxl/minidump/JoinAbsEqWithoutOpfamilies.mdp",
"../data/dxl/minidump/3WayJoinUsingOperatorsOfNonDefaultOpfamily.mdp",
"../data/dxl/minidump/Distinct-LegacyOpfamily.mdp",
	};

	return CTestUtils::EresUnittest_RunTests
						(
						rgszMdpFiles,
						&m_ulTestCounter,
						GPOS_ARRAY_SIZE(rgszMdpFiles)
						);
}

// EOF
