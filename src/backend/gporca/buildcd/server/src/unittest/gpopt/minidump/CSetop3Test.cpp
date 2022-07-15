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
#include "unittest/gpopt/minidump/CSetop3Test.h"

using namespace gpopt;

ULONG CSetop3Test::m_ulTestCounter = 0;  // start from first test

GPOS_RESULT
CSetop3Test::EresUnittest()
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
CSetop3Test::EresUnittest_RunTests()
{
	const CHAR *rgszMdpFiles[] =
	{
"../data/dxl/minidump/UnionWithOuterRefs.mdp",
"../data/dxl/minidump/UnionAll.mdp",
"../data/dxl/minidump/Union-Distributed-Table-With-Const-Table.mdp",
"../data/dxl/minidump/ExceptAllCompatibleDataType.mdp",
"../data/dxl/minidump/UnionAllCompatibleDataType.mdp",
"../data/dxl/minidump/UnionOfDQAQueries.mdp",
"../data/dxl/minidump/Union-Volatile-Func.mdp",
"../data/dxl/minidump/Intersect-Volatile-Func.mdp",
"../data/dxl/minidump/Except-Volatile-Func.mdp",
"../data/dxl/minidump/UnionWithCTE.mdp",
	};

	return CTestUtils::EresUnittest_RunTests
						(
						rgszMdpFiles,
						&m_ulTestCounter,
						GPOS_ARRAY_SIZE(rgszMdpFiles)
						);
}

// EOF
