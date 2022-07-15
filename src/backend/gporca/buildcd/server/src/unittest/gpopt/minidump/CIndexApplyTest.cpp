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
#include "unittest/gpopt/minidump/CIndexApplyTest.h"

using namespace gpopt;

ULONG CIndexApplyTest::m_ulTestCounter = 0;  // start from first test

GPOS_RESULT
CIndexApplyTest::EresUnittest()
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
CIndexApplyTest::EresUnittest_RunTests()
{
	const CHAR *rgszMdpFiles[] =
	{
"../data/dxl/minidump/IndexNLJ-IndexGet-OuterRef.mdp",
"../data/dxl/minidump/IndexApply_NestLoopWithNestParamTrue.mdp",
"../data/dxl/minidump/IndexApply1.mdp",
"../data/dxl/minidump/IndexApply1-CalibratedCostModel.mdp",
"../data/dxl/minidump/IndexApply2.mdp",
"../data/dxl/minidump/IndexApply3.mdp",
"../data/dxl/minidump/IndexApply4.mdp",
"../data/dxl/minidump/IndexApply-IndexCondDisjointWithHashedDistr.mdp",
"../data/dxl/minidump/IndexApply-IndexCondIntersectWithHashedDistr.mdp",
"../data/dxl/minidump/IndexApply-IndexCondMatchHashedDistr.mdp",
"../data/dxl/minidump/IndexApply-IndexCondSubsetOfHashedDistr.mdp",
"../data/dxl/minidump/IndexApply-IndexCondSupersetOfHashedDistr.mdp",
"../data/dxl/minidump/IndexApply-IndexOnMasterOnlyTable.mdp",
"../data/dxl/minidump/IndexApply-PartTable.mdp",
"../data/dxl/minidump/IndexApply-Heterogeneous-NoDTS.mdp",
"../data/dxl/minidump/IndexApply-Heterogeneous-DTS.mdp",
"../data/dxl/minidump/IndexApply-No-Motion-Below-Join.mdp",
"../data/dxl/minidump/IndexApply-Redistribute-Const-Table.mdp",
"../data/dxl/minidump/IndexApply-PartResolverExpand.mdp",
"../data/dxl/minidump/IndexApply-InnerSelect-Basic.mdp",
"../data/dxl/minidump/IndexApply-InnerSelect-PartTable.mdp",
"../data/dxl/minidump/IndexApply-PartKey-Is-IndexKey.mdp",
"../data/dxl/minidump/IndexApply-InnerSelect-Heterogeneous-DTS.mdp",
"../data/dxl/minidump/IndexApply-Heterogeneous-BothSidesPartitioned.mdp",
"../data/dxl/minidump/IndexApply-InnerSelect-PartTable2.mdp",
"../data/dxl/minidump/IndexApply-ForPartialIndex.mdp",
"../data/dxl/minidump/CastedScalarIf-On-Index-Key.mdp",
"../data/dxl/minidump/IndexApply-MultiDistKey-WithComplexPreds.mdp",
"../data/dxl/minidump/IndexApply-MultiDistKeys-Bitmap.mdp",
"../data/dxl/minidump/IndexApply-MultiDistKeys-Bitmap-WithComplexPreds.mdp",
"../data/dxl/minidump/IndexApply-MultiDistKeys-IncompletePDS-3-DistCols.mdp",
"../data/dxl/minidump/BitmapIndexNLJWithProject.mdp",
"../data/dxl/minidump/BitmapIndexNLOJWithProject.mdp",
"../data/dxl/minidump/BitmapIndexNLJWithProjectNoFilt.mdp",
"../data/dxl/minidump/BtreeIndexNLJWithProjectNoPart.mdp",
"../data/dxl/minidump/BitmapIndexNLOJWithProjectNonPart.mdp",
"../data/dxl/minidump/BtreeIndexNLOJWithProject.mdp",
	};

	return CTestUtils::EresUnittest_RunTests
						(
						rgszMdpFiles,
						&m_ulTestCounter,
						GPOS_ARRAY_SIZE(rgszMdpFiles)
						);
}

// EOF
