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
#include "unittest/gpopt/minidump/CArrayCmpTest.h"

using namespace gpopt;

ULONG CArrayCmpTest::m_ulTestCounter = 0;  // start from first test

GPOS_RESULT
CArrayCmpTest::EresUnittest()
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
CArrayCmpTest::EresUnittest_RunTests()
{
	const CHAR *rgszMdpFiles[] =
	{
"../data/dxl/minidump/ArrayConcat.mdp",
"../data/dxl/minidump/ArrayRef.mdp",
"../data/dxl/minidump/FoldedArrayCmp.mdp",
"../data/dxl/minidump/IN-ArrayCmp.mdp",
"../data/dxl/minidump/NOT-IN-ArrayCmp.mdp",
"../data/dxl/minidump/ArrayCmpAll.mdp",
"../data/dxl/minidump/UDA-AnyArray.mdp",
"../data/dxl/minidump/InClauseWithMCV.mdp",
"../data/dxl/minidump/CastedInClauseWithMCV.mdp",
"../data/dxl/minidump/FilterScalarCast.mdp",
"../data/dxl/minidump/IN-Nulls-ArrayCmpAny.mdp",
"../data/dxl/minidump/ArrayCmp-IN-ManyElements.mdp",
"../data/dxl/minidump/ArrayCmpAnyEmpty.mdp",
"../data/dxl/minidump/ArrayCmpAllEmpty.mdp",
"../data/dxl/minidump/ArrayCmpAnyEmptyLessThan.mdp",
"../data/dxl/minidump/ArrayCmpAll-Disjunct.mdp",
"../data/dxl/minidump/ArrayCmpAll-Disjunct-2.mdp",
	};

	return CTestUtils::EresUnittest_RunTests
						(
						rgszMdpFiles,
						&m_ulTestCounter,
						GPOS_ARRAY_SIZE(rgszMdpFiles)
						);
}

// EOF
