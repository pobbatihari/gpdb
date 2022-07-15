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
#include "unittest/gpopt/minidump/CIndexScanOnLeafTest.h"

using namespace gpopt;

ULONG CIndexScanOnLeafTest::m_ulTestCounter = 0;  // start from first test

GPOS_RESULT
CIndexScanOnLeafTest::EresUnittest()
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
CIndexScanOnLeafTest::EresUnittest_RunTests()
{
	const CHAR *rgszMdpFiles[] =
	{
"../data/dxl/minidump/IndexOnLeaf-AddNewPartitionToRootTableContainingHeterogenousIndex.mdp",
"../data/dxl/minidump/IndexOnLeaf-AddPartitionToRootWithHomogenousIndex.mdp",
"../data/dxl/minidump/IndexOnLeaf-AddPartitionToRootWithHomogenousIndex.mdp",
"../data/dxl/minidump/IndexOnLeaf-IndexOnPartitionsWithDifferentStorageTypes.mdp",
"../data/dxl/minidump/IndexOnLeaf-NonOverlappingHeterogenousIndex-ANDPredicate-AO.mdp",
"../data/dxl/minidump/IndexOnLeaf-NonOverlappingHeterogenousIndex-ANDPredicate-HEAP.mdp",
"../data/dxl/minidump/IndexOnLeaf-NonOverlappingHomogenousIndexesOnRoot-ANDPredicate-AO.mdp",
"../data/dxl/minidump/IndexOnLeaf-NonOverlappingHomogenousIndexesOnRoot-ORPredicate-AO.mdp",
"../data/dxl/minidump/IndexOnLeaf-OverlappingHeterogenousIndex-ANDPredicate-AO.mdp",
"../data/dxl/minidump/IndexOnLeaf-OverlappingHeterogenousIndex-ANDPredicate-HEAP.mdp",
"../data/dxl/minidump/IndexOnLeaf-OverlappingHeterogenousIndex-ORPredicate-AO.mdp",
"../data/dxl/minidump/IndexOnLeaf-OverlappingHomogenousIndexesOnRoot-ANDPredicate-AO.mdp",
"../data/dxl/minidump/IndexOnLeaf-OverlappingHomogenousIndexesOnRoot-AO.mdp",
"../data/dxl/minidump/IndexOnLeaf-OverlappingHomogenousIndexesOnRoot-ORPredicate-AO.mdp",
"../data/dxl/minidump/IndexOnLeaf-SingleColumnHeterogenousIndexOnRoot-1-AO.mdp",
"../data/dxl/minidump/IndexOnLeaf-SingleColumnHeterogenousIndexOnRoot-1-HEAP.mdp",
"../data/dxl/minidump/IndexOnLeaf-SingleColumnHeterogenousIndexOnRoot-2-AO.mdp",
"../data/dxl/minidump/IndexOnLeaf-SingleColumnHeterogenousIndexOnRoot-2-HEAP.mdp",
"../data/dxl/minidump/NonOverlappingHomogenousIndexesOnRoot-ANDPredicate-HEAP.mdp",
"../data/dxl/minidump/OverlappingHomogenousIndexesOnRoot-ANDPredicate-HEAP.mdp",
"../data/dxl/minidump/OverlappingHomogenousIndexesOnRoot-HEAP.mdp",
"../data/dxl/minidump/SingleColumnHomogenousIndexOnRoot-AO.mdp",
"../data/dxl/minidump/SingleColumnHomogenousIndexOnRoot-HEAP.mdp",
	};

	return CTestUtils::EresUnittest_RunTests
						(
						rgszMdpFiles,
						&m_ulTestCounter,
						GPOS_ARRAY_SIZE(rgszMdpFiles)
						);
}

// EOF
