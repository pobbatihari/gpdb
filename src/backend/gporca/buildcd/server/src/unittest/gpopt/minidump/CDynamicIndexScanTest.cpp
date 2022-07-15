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
#include "unittest/gpopt/minidump/CDynamicIndexScanTest.h"

using namespace gpopt;

ULONG CDynamicIndexScanTest::m_ulTestCounter = 0;  // start from first test

GPOS_RESULT
CDynamicIndexScanTest::EresUnittest()
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
CDynamicIndexScanTest::EresUnittest_RunTests()
{
	const CHAR *rgszMdpFiles[] =
	{
"../data/dxl/minidump/DynamicIndexScan-Homogenous.mdp",
"../data/dxl/minidump/DynamicIndexScan-BoolTrue.mdp",
"../data/dxl/minidump/DynamicIndexScan-BoolFalse.mdp",
"../data/dxl/minidump/DynamicIndexScan-Heterogenous-NoDTS.mdp",
"../data/dxl/minidump/DynamicIndexScan-Heterogenous-Union.mdp",
"../data/dxl/minidump/DynamicIndexScan-Heterogenous-Overlapping.mdp",
"../data/dxl/minidump/DynamicIndexScan-Heterogenous.mdp",
"../data/dxl/minidump/DynamicIndexScan-DefaultPartition.mdp",
"../data/dxl/minidump/DynamicIndexScan-DefaultPartition-2.mdp",
"../data/dxl/minidump/DynamicIndexGetDroppedCols.mdp",
"../data/dxl/minidump/DynamicIndexScan-Homogenous-UnsupportedConstraint.mdp",
"../data/dxl/minidump/DynamicIndexScan-Heterogenous-UnsupportedConstraint.mdp",
"../data/dxl/minidump/DynamicIndexScan-Heterogenous-UnsupportedPredicate.mdp",
"../data/dxl/minidump/DynamicIndexScan-Heterogenous-PartSelectEquality.mdp",
"../data/dxl/minidump/DynamicIndexScan-Heterogenous-PartSelectRange.mdp",
"../data/dxl/minidump/DynamicIndexScan-OpenEndedPartitions.mdp",
"../data/dxl/minidump/DynamicIndexScan-Relabel.mdp",
"../data/dxl/minidump/DynamicIndexScan-DroppedColumns.mdp",
"../data/dxl/minidump/DynamicIndexGet-OuterRefs.mdp",
	};

	return CTestUtils::EresUnittest_RunTests
						(
						rgszMdpFiles,
						&m_ulTestCounter,
						GPOS_ARRAY_SIZE(rgszMdpFiles)
						);
}

// EOF
