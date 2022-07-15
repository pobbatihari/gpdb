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
#include "unittest/gpopt/minidump/CICGMiscTest.h"

using namespace gpopt;

ULONG CICGMiscTest::m_ulTestCounter = 0;  // start from first test

GPOS_RESULT
CICGMiscTest::EresUnittest()
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
CICGMiscTest::EresUnittest_RunTests()
{
	const CHAR *rgszMdpFiles[] =
	{
"../data/dxl/minidump/BroadcastSkewedHashjoin.mdp",
"../data/dxl/minidump/OrderByNullsFirst.mdp",
"../data/dxl/minidump/ConvertHashToRandomSelect.mdp",
"../data/dxl/minidump/ConvertHashToRandomInsert.mdp",
"../data/dxl/minidump/HJN-DeeperOuter.mdp",
"../data/dxl/minidump/CTAS.mdp",
"../data/dxl/minidump/CTAS-Random.mdp",
"../data/dxl/minidump/CheckAsUser.mdp",
"../data/dxl/minidump/CTAS-random-distributed-from-replicated-distributed-table.mdp",
"../data/dxl/minidump/ProjectRepeatedColumn1.mdp",
"../data/dxl/minidump/ProjectRepeatedColumn2.mdp",
"../data/dxl/minidump/NLJ-BC-Outer-Spool-Inner.mdp",
"../data/dxl/minidump/Self-Comparison.mdp",
"../data/dxl/minidump/Self-Comparison-Nullable.mdp",
"../data/dxl/minidump/SelectCheckConstraint.mdp",
"../data/dxl/minidump/ExpandJoinOrder.mdp",
"../data/dxl/minidump/SelectOnBpchar.mdp",
"../data/dxl/minidump/EqualityJoin.mdp",
"../data/dxl/minidump/EffectsOfJoinFilter.mdp",
"../data/dxl/minidump/InnerJoin-With-OuterRefs.mdp",
"../data/dxl/minidump/UDA-AnyElement-1.mdp",
"../data/dxl/minidump/UDA-AnyElement-2.mdp",
"../data/dxl/minidump/Project-With-NonScalar-Func.mdp",
"../data/dxl/minidump/SixWayDPv2.mdp",
"../data/dxl/minidump/Join-Varchar-Equality.mdp",
	};

	return CTestUtils::EresUnittest_RunTests
						(
						rgszMdpFiles,
						&m_ulTestCounter,
						GPOS_ARRAY_SIZE(rgszMdpFiles)
						);
}

// EOF
