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
#include "unittest/gpopt/minidump/CSetop4Test.h"

using namespace gpopt;

ULONG CSetop4Test::m_ulTestCounter = 0;  // start from first test

GPOS_RESULT
CSetop4Test::EresUnittest()
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
CSetop4Test::EresUnittest_RunTests()
{
	const CHAR *rgszMdpFiles[] =
	{
"../data/dxl/minidump/PushSelectDownUnionAllOfCTG.mdp",
"../data/dxl/minidump/Push-Subplan-Below-Union.mdp",
"../data/dxl/minidump/Intersect-OuterRefs.mdp",
"../data/dxl/minidump/PushSelectWithOuterRefBelowUnion.mdp",
"../data/dxl/minidump/PushGbBelowUnion.mdp",
"../data/dxl/minidump/UnionGbSubquery.mdp",
"../data/dxl/minidump/MS-UnionAll-4.mdp",
"../data/dxl/minidump/AnyPredicate-Over-UnionOfConsts.mdp",
"../data/dxl/minidump/EquivClassesUnion.mdp",
"../data/dxl/minidump/EquivClassesIntersect.mdp",
"../data/dxl/minidump/IndexScanWithNestedCTEAndSetOp.mdp",
"../data/dxl/minidump/Blocking-Spool-Parallel-Union-All.mdp",
	};

	return CTestUtils::EresUnittest_RunTests
						(
						rgszMdpFiles,
						&m_ulTestCounter,
						GPOS_ARRAY_SIZE(rgszMdpFiles)
						);
}

// EOF
