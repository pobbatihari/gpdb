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
#include "unittest/gpopt/minidump/CPartTbl4Test.h"

using namespace gpopt;

ULONG CPartTbl4Test::m_ulTestCounter = 0;  // start from first test

GPOS_RESULT
CPartTbl4Test::EresUnittest()
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
CPartTbl4Test::EresUnittest_RunTests()
{
	const CHAR *rgszMdpFiles[] =
	{
"../data/dxl/minidump/PartTbl-Relabel-Equality.mdp",
"../data/dxl/minidump/PartTbl-Relabel-Range.mdp",
"../data/dxl/minidump/PartTbl-DisablePartSelection.mdp",
"../data/dxl/minidump/PartTbl-DisablePartSelectionJoin.mdp",
"../data/dxl/minidump/PartTbl-JoinOverUnion-1.mdp",
"../data/dxl/minidump/PartTbl-JoinOverUnion-2.mdp",
"../data/dxl/minidump/PartTbl-JoinOverIntersect.mdp",
"../data/dxl/minidump/PartTbl-JoinOverExcept.mdp",
"../data/dxl/minidump/PartTbl-RangeJoinPred.mdp",
"../data/dxl/minidump/PartTbl-ArrayIn.mdp",
"../data/dxl/minidump/PartTbl-ArrayCoerce.mdp",
"../data/dxl/minidump/PartTbl-Disjunction.mdp",
"../data/dxl/minidump/PartTbl-ComplexPredicate1.mdp",
"../data/dxl/minidump/PartTbl-ComplexPredicate2.mdp",
"../data/dxl/minidump/PartTbl-ComplexPredicate3.mdp",
"../data/dxl/minidump/PartTbl-ComplexPredicate4.mdp",
"../data/dxl/minidump/PartTbl-ComplexPredicate5.mdp",
"../data/dxl/minidump/LargeMultiColumnInList.mdp",
	};

	return CTestUtils::EresUnittest_RunTests
						(
						rgszMdpFiles,
						&m_ulTestCounter,
						GPOS_ARRAY_SIZE(rgszMdpFiles)
						);
}

// EOF
