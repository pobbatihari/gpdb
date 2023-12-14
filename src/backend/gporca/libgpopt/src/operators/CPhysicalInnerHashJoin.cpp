//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2011 EMC Corp.
//
//	@filename:
//		CPhysicalInnerHashJoin.cpp
//
//	@doc:
//		Implementation of inner hash join operator
//---------------------------------------------------------------------------

#include "gpopt/operators/CPhysicalInnerHashJoin.h"

#include "gpos/base.h"

#include "gpopt/base/CDistributionSpecHashed.h"
#include "gpopt/base/CDistributionSpecNonSingleton.h"
#include "gpopt/base/CDistributionSpecReplicated.h"
#include "gpopt/base/CUtils.h"
#include "gpopt/operators/CExpressionHandle.h"
#include "gpopt/operators/CPredicateUtils.h"
#include "gpopt/operators/CScalarCmp.h"

using namespace gpopt;

// number of non-redistribute requests created by InnerHashJoin
#define GPOPT_NON_HASH_DIST_REQUESTS 4


//---------------------------------------------------------------------------
//	@function:
//		CPhysicalInnerHashJoin::CPhysicalInnerHashJoin
//
//	@doc:
//		Ctor
//
//---------------------------------------------------------------------------
CPhysicalInnerHashJoin::CPhysicalInnerHashJoin(
	CMemoryPool *mp, CExpressionArray *pdrgpexprOuterKeys,
	CExpressionArray *pdrgpexprInnerKeys, IMdIdArray *hash_opfamilies,
	BOOL is_null_aware, CXform::EXformId origin_xform)
	: CPhysicalHashJoin(mp, pdrgpexprOuterKeys, pdrgpexprInnerKeys,
						hash_opfamilies, is_null_aware, origin_xform)
{
	// number of total requests created by InnerHashJoin
	ULONG ulDistrReqs = GPOPT_NON_HASH_DIST_REQUESTS + NumDistrReq();
	SetDistrRequests(ulDistrReqs);

	if ((GPOPT_FDISABLED_XFORM(CXform::ExfExpandNAryJoinDP) &&
		 GPOPT_FDISABLED_XFORM(CXform::ExfExpandNAryJoinDPv2)) ||
		this->OriginXform() == CXform::ExfExpandNAryJoinGreedy)
	{
		SetPartPropagateRequests(2);
	}
	else
	{
		SetPartPropagateRequests(1);
	}
}


//---------------------------------------------------------------------------
//	@function:
//		CPhysicalInnerHashJoin::~CPhysicalInnerHashJoin
//
//	@doc:
//		Dtor
//
//---------------------------------------------------------------------------
CPhysicalInnerHashJoin::~CPhysicalInnerHashJoin() = default;


//
//---------------------------------------------------------------------------
//	@function:
//		CPhysicalHashJoin::PdsRequiredOuterReplicated
//
//	@doc:
//		Create (broadcast, non-singleton) optimization request
//
//---------------------------------------------------------------------------
CDistributionSpec *
CPhysicalInnerHashJoin::PdsRequiredOuterReplicated(
	CMemoryPool *mp,
	CExpressionHandle &,  // exprhdl
	CDistributionSpec *,  // pdsInput
	ULONG child_index, CDrvdPropArray *pdrgpdpCtxt) const
{
	GPOS_ASSERT(EceoRightToLeft == Eceo());

	if (1 == child_index)
	{
		// require inner child to be non-singleton
		return GPOS_NEW(mp) CDistributionSpecNonSingleton();
	}
	GPOS_ASSERT(0 == child_index);

	// require a matching distribution from outer child
	CDistributionSpec *pdsInner =
		CDrvdPropPlan::Pdpplan((*pdrgpdpCtxt)[0])->Pds();
	GPOS_ASSERT(nullptr != pdsInner);

	if (CDistributionSpec::EdtUniversal == pdsInner->Edt())
	{
		// inner child is universal, request outer child to execute on
		// a single host to avoid duplicates
		return GPOS_NEW(mp) CDistributionSpecSingleton();
	}

	if (CDistributionSpec::EdtStrictReplicated == pdsInner->Edt())
	{
		// inner child is strict replicated, request outer child to
		// execute non-singleton
		return GPOS_NEW(mp) CDistributionSpecNonSingleton();
	}

	// otherwise, request outer child to deliver replicated distribution
	return GPOS_NEW(mp) CDistributionSpecReplicated(
		CDistributionSpec::EdtReplicated, false /*ignore_broadcast_threshold*/,
		false /*fAllowEnforced*/);
}

//---------------------------------------------------------------------------
//	@function:
//		CPhysicalInnerHashJoin::Ped
//
//	@doc:
//		Compute required distribution of the n-th child;
//		this function creates four optimization requests to join children:
//		First three optimization request are created using CPhysicalHashJoin::Ped()
// 		Req(1 to N)  (redistribute, redistribute)
// 		Req(N + 1) (hashed, broadcast)
// 		Req(N + 2) (non-singleton, broadcast)
// 		Req(N + 3) (singleton, singleton)
//		Req(N + 4) (broadcast, non-singelton)
//
//		we always check the distribution delivered by the first child (as
//		given by child optimization order), and then match the delivered
//		distribution on the second child
//
//---------------------------------------------------------------------------

CEnfdDistribution *
CPhysicalInnerHashJoin::Ped(CMemoryPool *mp, CExpressionHandle &exprhdl,
							CReqdPropPlan *prppInput, ULONG child_index,
							CDrvdPropArray *pdrgpdpCtxt, ULONG ulOptReq)
{
	GPOS_ASSERT(2 > child_index);

	const ULONG ulHashDistributeRequests = NumDistrReq();

	// The variable ulHashDistributeRequests contains the count of redistribute
	// requests and is set to 1 because there is a single redistribute request
	// (optreq (1 to N)). The subsequent check (OptReq < (1+3)) ensures that we
	// invoke HashJoin::Ped() for optimization requests ranging from 1 to (N + 3).
	if (ulOptReq < ulHashDistributeRequests + 3 ||
		exprhdl.NeedsSingletonExecution() || exprhdl.HasOuterRefs())
	{
		return CPhysicalHashJoin::Ped(mp, exprhdl, prppInput, child_index,
									  pdrgpdpCtxt, ulOptReq);
	}

	GPOS_ASSERT(ulOptReq == ulHashDistributeRequests + 3);

	CEnfdDistribution::EDistributionMatching dmatch =
		Edm(prppInput, child_index, pdrgpdpCtxt, ulOptReq);
	CDistributionSpec *const pdsInput = prppInput->Ped()->PdsRequired();

	// requests N+4 is (broadcast, non-singleton)
	return GPOS_NEW(mp)
		CEnfdDistribution(PdsRequiredOuterReplicated(mp, exprhdl, pdsInput,
													 child_index, pdrgpdpCtxt),
						  dmatch);
}

//---------------------------------------------------------------------------
//	@function:
//		CPhysicalInnerHashJoin::PdshashedCreateMatching
//
//	@doc:
//		Helper function for creating a matching hashed distribution
//
//---------------------------------------------------------------------------
CDistributionSpecHashed *
CPhysicalInnerHashJoin::PdshashedCreateMatching(
	CMemoryPool *mp, CDistributionSpecHashed *pdshashed,
	ULONG
		ulSourceChild  // index of child that delivered the given hashed distribution
) const
{
	GPOS_ASSERT(nullptr != pdshashed);

	CDistributionSpecHashed *pdshashedMatching =
		PdshashedMatching(mp, pdshashed, ulSourceChild);

	// create a new spec with input and the output spec as equivalents, as you don't want to lose
	// the already existing equivalent specs of pdshashed
	// NB: The matching spec is added at the beginning.
	pdshashedMatching->Pdrgpexpr()->AddRef();
	pdshashed->AddRef();
	if (nullptr != pdshashedMatching->Opfamilies())
	{
		pdshashedMatching->Opfamilies()->AddRef();
	}
	CDistributionSpecHashed *pdsHashedMatchingEquivalents =
		GPOS_NEW(mp) CDistributionSpecHashed(
			pdshashedMatching->Pdrgpexpr(),
			pdshashedMatching->FNullsColocated(),
			pdshashed,	// matching distribution spec is equivalent to passed distribution spec
			pdshashedMatching->Opfamilies());
	pdshashedMatching->Release();
	return pdsHashedMatchingEquivalents;
}


//---------------------------------------------------------------------------
//	@function:
//		CPhysicalInnerHashJoin::PdsDeriveFromHashedChildren
//
//	@doc:
//		Derive hash join distribution from hashed children;
//		return nullptr if derivation failed
//
//---------------------------------------------------------------------------
CDistributionSpec *
CPhysicalInnerHashJoin::PdsDeriveFromHashedChildren(
	CMemoryPool *mp, CDistributionSpec *pdsOuter,
	CDistributionSpec *pdsInner) const
{
	GPOS_ASSERT(nullptr != pdsOuter);
	GPOS_ASSERT(nullptr != pdsInner);

	CDistributionSpecHashed *pdshashedOuter =
		CDistributionSpecHashed::PdsConvert(pdsOuter);
	CDistributionSpecHashed *pdshashedInner =
		CDistributionSpecHashed::PdsConvert(pdsInner);

	if (pdshashedOuter->IsCoveredBy(PdrgpexprOuterKeys()) &&
		pdshashedInner->IsCoveredBy(PdrgpexprInnerKeys()))
	{
		// if both sides are hashed on subsets of hash join keys, join's output can be
		// seen as distributed on outer spec or (equivalently) on inner spec,
		// so create a new spec and mark outer and inner as equivalent
		CDistributionSpecHashed *combined_hashed_spec =
			pdshashedOuter->Combine(mp, pdshashedInner);
		return combined_hashed_spec;
	}

	return nullptr;
}


//---------------------------------------------------------------------------
//	@function:
//		CPhysicalInnerHashJoin::PdsDeriveFromReplicatedOuter
//
//	@doc:
//		Derive hash join distribution from a replicated outer child;
//
//---------------------------------------------------------------------------
CDistributionSpec *
CPhysicalInnerHashJoin::PdsDeriveFromReplicatedOuter(
	CMemoryPool *mp,
	CDistributionSpec *
#ifdef GPOS_DEBUG
		pdsOuter
#endif	// GPOS_DEBUG
	,
	CDistributionSpec *pdsInner) const
{
	GPOS_ASSERT(nullptr != pdsOuter);
	GPOS_ASSERT(nullptr != pdsInner);
	GPOS_ASSERT(CDistributionSpec::EdtStrictReplicated == pdsOuter->Edt() ||
				CDistributionSpec::EdtTaintedReplicated == pdsOuter->Edt());

	// if outer child is replicated, join results distribution is defined by inner child
	if (CDistributionSpec::EdtHashed == pdsInner->Edt())
	{
		CDistributionSpecHashed *pdshashedInner =
			CDistributionSpecHashed::PdsConvert(pdsInner);
		if (pdshashedInner->IsCoveredBy(PdrgpexprInnerKeys()))
		{
			// inner child is hashed on a subset of inner hashkeys,
			// return a hashed distribution equivalent to a matching outer distribution
			return PdshashedCreateMatching(mp, pdshashedInner,
										   1 /*ulSourceChild*/);
		}
	}

	// otherwise, pass-through inner distribution
	pdsInner->AddRef();
	return pdsInner;
}


//---------------------------------------------------------------------------
//	@function:
//		CPhysicalInnerHashJoin::PdsDeriveFromHashedOuter
//
//	@doc:
//		Derive hash join distribution from a hashed outer child;
//		return nullptr if derivation failed
//
//---------------------------------------------------------------------------
CDistributionSpec *
CPhysicalInnerHashJoin::PdsDeriveFromHashedOuter(CMemoryPool *mp,
												 CDistributionSpec *pdsOuter,
												 CDistributionSpec *
#ifdef GPOS_DEBUG
													 pdsInner
#endif	// GPOS_DEBUG
) const
{
	GPOS_ASSERT(nullptr != pdsOuter);
	GPOS_ASSERT(nullptr != pdsInner);

	GPOS_ASSERT(CDistributionSpec::EdtHashed == pdsOuter->Edt());

	CDistributionSpecHashed *pdshashedOuter =
		CDistributionSpecHashed::PdsConvert(pdsOuter);
	if (pdshashedOuter->IsCoveredBy(PdrgpexprOuterKeys()))
	{
		// outer child is hashed on a subset of outer hashkeys,
		// return a hashed distribution equivalent to a matching outer distribution
		return PdshashedCreateMatching(mp, pdshashedOuter, 0 /*ulSourceChild*/);
	}

	return nullptr;
}


//---------------------------------------------------------------------------
//	@function:
//		CPhysicalInnerHashJoin::PdsDerive
//
//	@doc:
//		Derive distribution
//
//---------------------------------------------------------------------------
CDistributionSpec *
CPhysicalInnerHashJoin::PdsDerive(CMemoryPool *mp,
								  CExpressionHandle &exprhdl) const
{
	CDistributionSpec *pdsOuter = exprhdl.Pdpplan(0 /*child_index*/)->Pds();
	CDistributionSpec *pdsInner = exprhdl.Pdpplan(1 /*child_index*/)->Pds();

	if (CDistributionSpec::EdtUniversal == pdsOuter->Edt())
	{
		// if outer is universal, pass through inner distribution
		pdsInner->AddRef();
		return pdsInner;
	}

	if (CDistributionSpec::EdtHashed == pdsOuter->Edt() &&
		CDistributionSpec::EdtHashed == pdsInner->Edt())
	{
		CDistributionSpec *pdsDerived =
			PdsDeriveFromHashedChildren(mp, pdsOuter, pdsInner);
		if (nullptr != pdsDerived)
		{
			return pdsDerived;
		}
	}
	// If the distribution of the outer child is Strict/Tainted Replicated,
	// derive the join distribution based on the distribution of the inner child.
	if (CDistributionSpec::EdtStrictReplicated == pdsOuter->Edt() ||
		CDistributionSpec::EdtTaintedReplicated == pdsOuter->Edt())
	{
		return PdsDeriveFromReplicatedOuter(mp, pdsOuter, pdsInner);
	}

	if (CDistributionSpec::EdtHashed == pdsOuter->Edt())
	{
		CDistributionSpec *pdsDerived =
			PdsDeriveFromHashedOuter(mp, pdsOuter, pdsInner);
		if (nullptr != pdsDerived)
		{
			return pdsDerived;
		}
	}

	// otherwise, pass through outer distribution
	pdsOuter->AddRef();
	return pdsOuter;
}

CPartitionPropagationSpec *
CPhysicalInnerHashJoin::PppsRequired(CMemoryPool *mp,
									 CExpressionHandle &exprhdl,
									 CPartitionPropagationSpec *pppsRequired,
									 ULONG child_index,
									 CDrvdPropArray *pdrgpdpCtxt,
									 ULONG ulOptReq) const
{
	return PppsRequiredForJoins(mp, exprhdl, pppsRequired, child_index,
								pdrgpdpCtxt, ulOptReq);
}

CPartitionPropagationSpec *
CPhysicalInnerHashJoin::PppsDerive(CMemoryPool *mp,
								   CExpressionHandle &exprhdl) const
{
	return PppsDeriveForJoins(mp, exprhdl);
}

// EOF
