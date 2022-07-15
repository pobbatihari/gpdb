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
#include "unittest/gpopt/minidump/CPartTbl1Test.h"

using namespace gpopt;

ULONG CPartTbl1Test::m_ulTestCounter = 0;  // start from first test

GPOS_RESULT
CPartTbl1Test::EresUnittest()
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
CPartTbl1Test::EresUnittest_RunTests()
{
	const CHAR *rgszMdpFiles[] =
	{
"../data/dxl/minidump/NoPartConstraint-WhenNoDefaultPartsAndIndices.mdp",
"../data/dxl/minidump/PartConstraint-WhenIndicesAndNoDefaultParts.mdp",
"../data/dxl/minidump/PartConstraint-WithOnlyDefaultPartInfo.mdp",
"../data/dxl/minidump/PartConstraint-WhenDefaultPartsAndIndices.mdp",
"../data/dxl/minidump/CorrelatedNLJ-PartSelector-Subplan.mdp",
"../data/dxl/minidump/DonotPushPartConstThruLimit.mdp",
"../data/dxl/minidump/Select-Over-PartTbl.mdp",
"../data/dxl/minidump/PartSelectorOnJoinSide.mdp",
"../data/dxl/minidump/PartSelectorOnJoinSide2.mdp",
	};

	return CTestUtils::EresUnittest_RunTests
						(
						rgszMdpFiles,
						&m_ulTestCounter,
						GPOS_ARRAY_SIZE(rgszMdpFiles)
						);
}

// EOF
