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
#include "unittest/gpopt/minidump/CSetop2Test.h"

using namespace gpopt;

ULONG CSetop2Test::m_ulTestCounter = 0;  // start from first test

GPOS_RESULT
CSetop2Test::EresUnittest()
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
CSetop2Test::EresUnittest_RunTests()
{
	const CHAR *rgszMdpFiles[] =
	{
"../data/dxl/minidump/Cascaded-UnionAll-Same-Cols-Order.mdp",
"../data/dxl/minidump/Cascaded-UnionAll-Differing-Cols.mdp",
"../data/dxl/minidump/Cascaded-UnionAll-Differing-Cols-Order.mdp",
"../data/dxl/minidump/Union-Over-UnionAll.mdp",
"../data/dxl/minidump/Nested-Setops.mdp",
"../data/dxl/minidump/Nested-Setops-2.mdp",
"../data/dxl/minidump/Except.mdp",
"../data/dxl/minidump/InnerJoinOverJoinExcept.mdp",
"../data/dxl/minidump/InnerJoinOverJoinExceptAll.mdp",
"../data/dxl/minidump/IsNullUnionAllIsNotNull.mdp",
"../data/dxl/minidump/UnionAllWithTruncatedOutput.mdp",
"../data/dxl/minidump/Union-OuterRefs-Output.mdp",
"../data/dxl/minidump/Union-OuterRefs-Casting-Output.mdp",
"../data/dxl/minidump/Union-OuterRefs-InnerChild.mdp",
"../data/dxl/minidump/MS-UnionAll-2.mdp",
	};

	return CTestUtils::EresUnittest_RunTests
						(
						rgszMdpFiles,
						&m_ulTestCounter,
						GPOS_ARRAY_SIZE(rgszMdpFiles)
						);
}

// EOF
