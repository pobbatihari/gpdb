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
#include "unittest/gpopt/minidump/CPartTblDPETest.h"

using namespace gpopt;

ULONG CPartTblDPETest::m_ulTestCounter = 0;  // start from first test

GPOS_RESULT
CPartTblDPETest::EresUnittest()
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
CPartTblDPETest::EresUnittest_RunTests()
{
	const CHAR *rgszMdpFiles[] =
	{
"../data/dxl/minidump/PartTbl-AvoidRangePred-DPE.mdp",
"../data/dxl/minidump/PartTbl-DPE-Correlated-NLOuter.mdp",
"../data/dxl/minidump/PartTbl-DPE.mdp",
"../data/dxl/minidump/PartTbl-DPE-WindowFunction.mdp",
"../data/dxl/minidump/PartTbl-DPE-GroupBy.mdp",
"../data/dxl/minidump/PartTbl-DPE-Limit.mdp",
"../data/dxl/minidump/DPE-with-unsupported-pred.mdp",
"../data/dxl/minidump/DPE-SemiJoin.mdp",
"../data/dxl/minidump/DPE-IN.mdp",
"../data/dxl/minidump/DPE-NOT-IN.mdp",
"../data/dxl/minidump/HJN-DPE-Bitmap-Outer-Child.mdp",
"../data/dxl/minidump/NLJ-Broadcast-DPE-Outer-Child.mdp",
"../data/dxl/minidump/PartTbl-MultiWayJoinWithDPE.mdp",
"../data/dxl/minidump/PartTbl-MultiWayJoinWithDPE-2.mdp",
"../data/dxl/minidump/PartTbl-LeftOuterHashJoin-DPE-IsNull.mdp",
"../data/dxl/minidump/PartTbl-LeftOuterNLJoin-DPE-IsNull.mdp",
"../data/dxl/minidump/PartTbl-List-DPE-Varchar-Predicates.mdp",
"../data/dxl/minidump/PartTbl-List-DPE-Int-Predicates.mdp",
"../data/dxl/minidump/PartTbl-DPE-PartitionSelectorRewindability.mdp",
"../data/dxl/minidump/PartTbl-RightOuterHashJoin-DPE-IsNull.mdp",
"../data/dxl/minidump/SPE-IS-NULL.mdp",
"../data/dxl/minidump/JoinOrderDPE.mdp",
	};

	return CTestUtils::EresUnittest_RunTests
						(
						rgszMdpFiles,
						&m_ulTestCounter,
						GPOS_ARRAY_SIZE(rgszMdpFiles)
						);
}

// EOF
