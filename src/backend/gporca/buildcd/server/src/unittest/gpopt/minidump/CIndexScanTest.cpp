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
#include "unittest/gpopt/minidump/CIndexScanTest.h"

using namespace gpopt;

ULONG CIndexScanTest::m_ulTestCounter = 0;  // start from first test

GPOS_RESULT
CIndexScanTest::EresUnittest()
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
CIndexScanTest::EresUnittest_RunTests()
{
	const CHAR *rgszMdpFiles[] =
	{
"../data/dxl/minidump/BTreeIndex-Against-InList.mdp",
"../data/dxl/minidump/BTreeIndex-Against-InListLarge.mdp",
"../data/dxl/minidump/BTreeIndex-Against-ScalarSubquery.mdp",
"../data/dxl/minidump/IndexScan-AOTable.mdp",
"../data/dxl/minidump/IndexScan-DroppedColumns.mdp",
"../data/dxl/minidump/IndexScan-BoolTrue.mdp",
"../data/dxl/minidump/IndexScan-BoolFalse.mdp",
"../data/dxl/minidump/IndexScan-Relabel.mdp",
"../data/dxl/minidump/IndexGet-OuterRefs.mdp",
"../data/dxl/minidump/LogicalIndexGetDroppedCols.mdp",
"../data/dxl/minidump/NewBtreeIndexScanCost.mdp",
"../data/dxl/minidump/IndexScan-ORPredsNonPart.mdp",
"../data/dxl/minidump/IndexScan-ORPredsAOPart.mdp",
"../data/dxl/minidump/IndexScan-AndedIn.mdp",
"../data/dxl/minidump/SubqInIndexPred.mdp",
	};

	return CTestUtils::EresUnittest_RunTests
						(
						rgszMdpFiles,
						&m_ulTestCounter,
						GPOS_ARRAY_SIZE(rgszMdpFiles)
						);
}

// EOF
