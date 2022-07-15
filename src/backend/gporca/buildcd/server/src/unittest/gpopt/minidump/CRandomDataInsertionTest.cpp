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
#include "unittest/gpopt/minidump/CRandomDataInsertionTest.h"

using namespace gpopt;

ULONG CRandomDataInsertionTest::m_ulTestCounter = 0;  // start from first test

GPOS_RESULT
CRandomDataInsertionTest::EresUnittest()
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
CRandomDataInsertionTest::EresUnittest_RunTests()
{
	const CHAR *rgszMdpFiles[] =
	{
"../data/dxl/minidump/AddRedistributeBeforeInsert-1.mdp",
"../data/dxl/minidump/AddRedistributeBeforeInsert-2.mdp",
"../data/dxl/minidump/AddRedistributeBeforeInsert-3.mdp",
"../data/dxl/minidump/AddRedistributeBeforeInsert-4.mdp",
"../data/dxl/minidump/AddRedistributeBeforeInsert-5.mdp",
"../data/dxl/minidump/DontAddRedistributeBeforeInsert-1.mdp",
"../data/dxl/minidump/DontAddRedistributeBeforeInsert-2.mdp",
	};

	return CTestUtils::EresUnittest_RunTests
						(
						rgszMdpFiles,
						&m_ulTestCounter,
						GPOS_ARRAY_SIZE(rgszMdpFiles)
						);
}

// EOF
