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
#include "unittest/gpopt/minidump/CPartTbl3Test.h"

using namespace gpopt;

ULONG CPartTbl3Test::m_ulTestCounter = 0;  // start from first test

GPOS_RESULT
CPartTbl3Test::EresUnittest()
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
CPartTbl3Test::EresUnittest_RunTests()
{
	const CHAR *rgszMdpFiles[] =
	{
"../data/dxl/minidump/PartTbl-SQNotExists.mdp",
"../data/dxl/minidump/PartTbl-SQAny.mdp",
"../data/dxl/minidump/PartTbl-SQAll.mdp",
"../data/dxl/minidump/PartTbl-SQScalar.mdp",
"../data/dxl/minidump/PartTbl-HJ3.mdp",
"../data/dxl/minidump/PartTbl-HJ4.mdp",
"../data/dxl/minidump/PartTbl-HJ5.mdp",
"../data/dxl/minidump/PartTbl-JoinOverGbAgg.mdp",
"../data/dxl/minidump/PartTbl-JoinOverGbAgg-2.mdp",
"../data/dxl/minidump/PartTbl-NonConstSelect.mdp",
"../data/dxl/minidump/PartTbl-VolatileFunc.mdp",
"../data/dxl/minidump/PartTbl-DateTime.mdp",
"../data/dxl/minidump/PartTbl-ComplexRangePredicate-DefaultPart.mdp",
"../data/dxl/minidump/PartTbl-ComplexRangePredicate-NoDefaultPart.mdp",
"../data/dxl/minidump/PartTbl-LASJ.mdp",
	};

	return CTestUtils::EresUnittest_RunTests
						(
						rgszMdpFiles,
						&m_ulTestCounter,
						GPOS_ARRAY_SIZE(rgszMdpFiles)
						);
}

// EOF
