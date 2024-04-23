//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2011 EMC Corp.
//
//	@filename:
//		CScalarSubqueryQuantified.cpp
//
//	@doc:
//		Implementation of quantified subquery operator
//---------------------------------------------------------------------------

#include "gpopt/operators/CScalarSubqueryQuantified.h"

#include "gpos/base.h"

#include "gpopt/base/CColRefSet.h"
#include "gpopt/base/CColRefSetIter.h"
#include "gpopt/base/CDrvdPropScalar.h"
#include "gpopt/base/COptCtxt.h"
#include "gpopt/operators/CExpressionHandle.h"
#include "gpopt/xforms/CSubqueryHandler.h"
#include "naucrates/md/IMDScalarOp.h"
#include "naucrates/md/IMDTypeBool.h"

using namespace gpopt;
using namespace gpmd;

//---------------------------------------------------------------------------
//	@function:
//		CScalarSubqueryQuantified::CScalarSubqueryQuantified
//
//	@doc:
//		Ctor
//
//---------------------------------------------------------------------------
CScalarSubqueryQuantified::CScalarSubqueryQuantified(
	CMemoryPool *mp, IMDId *scalar_op_mdid, const CWStringConst *pstrScalarOp,
	const CColRef *colref)
	: CScalar(mp),
	  m_scalar_op_mdid(scalar_op_mdid),
	  m_pstrScalarOp(pstrScalarOp),
	  m_pcr(colref)
{
	GPOS_ASSERT(scalar_op_mdid->IsValid());
	GPOS_ASSERT(nullptr != pstrScalarOp);
	GPOS_ASSERT(nullptr != colref);
}

//---------------------------------------------------------------------------
//	@function:
//		CScalarSubqueryQuantified::CScalarSubqueryQuantified
//
//	@doc:
//		Ctor
//
//---------------------------------------------------------------------------
CScalarSubqueryQuantified::CScalarSubqueryQuantified(
	CMemoryPool *mp, IMdIdArray *scalar_op_mdids,
	const CWStringConst *pstrScalarOp, CColRefArray *colref_array,
	CScalarBoolOp::EBoolOperator testexpr_booloptype)
	: CScalar(mp),
	  m_scalar_op_mdids(scalar_op_mdids),
	  m_pstrScalarOp(pstrScalarOp),
	  m_colref_array(colref_array),
	  m_testexprBoolopType(testexpr_booloptype)

{
	m_scalar_op_mdid = (*scalar_op_mdids)[0];
	GPOS_ASSERT(nullptr != scalar_op_mdids);
	GPOS_ASSERT(nullptr != pstrScalarOp);
	GPOS_ASSERT(nullptr != colref_array);
}

//---------------------------------------------------------------------------
//	@function:
//		CScalarSubqueryQuantified::~CScalarSubqueryQuantified
//
//	@doc:
//		Dtor
//
//---------------------------------------------------------------------------
CScalarSubqueryQuantified::~CScalarSubqueryQuantified()
{
	GPOS_DELETE(m_pstrScalarOp);
	if (FMultipleColumns())
	{
		m_scalar_op_mdids->Release();
		m_colref_array->Release();
	}
	else
	{
		m_scalar_op_mdid->Release();
	}
}

//---------------------------------------------------------------------------
//	@function:
//		CScalarSubqueryQuantified::PstrOp
//
//	@doc:
//		Operator name
//
//---------------------------------------------------------------------------
const CWStringConst *
CScalarSubqueryQuantified::PstrOp() const
{
	return m_pstrScalarOp;
}

//---------------------------------------------------------------------------
//	@function:
//		CScalarSubqueryQuantified::MdIdOp
//
//	@doc:
//		Scalar operator metadata id
//
//---------------------------------------------------------------------------
IMDId *
CScalarSubqueryQuantified::MdIdOp() const
{
	return m_scalar_op_mdid;
}

//---------------------------------------------------------------------------
//	@function:
//		CScalarSubqueryQuantified::MdidType
//
//	@doc:
//		Type of scalar's value
//
//---------------------------------------------------------------------------
IMDId *
CScalarSubqueryQuantified::MdidType() const
{
	CMDAccessor *md_accessor = COptCtxt::PoctxtFromTLS()->Pmda();
	IMDId *mdid_type =
		md_accessor->RetrieveScOp(m_scalar_op_mdid)->GetResultTypeMdid();

	GPOS_ASSERT(
		md_accessor->PtMDType<IMDTypeBool>()->MDId()->Equals(mdid_type));

	return mdid_type;
}

//---------------------------------------------------------------------------
//	@function:
//		CScalarSubqueryQuantified::HashValue
//
//	@doc:
//		Operator specific hash function
//
//---------------------------------------------------------------------------
ULONG
CScalarSubqueryQuantified::HashValue() const
{
	return gpos::CombineHashes(
		COperator::HashValue(),
		FMultipleColumns()
			? gpos::CombineHashes(gpos::HashPtr<IMdIdArray>(m_scalar_op_mdids),
								  gpos::HashPtr<CColRefArray>(m_colref_array))
			: gpos::CombineHashes(m_scalar_op_mdid->HashValue(),
								  gpos::HashPtr<CColRef>(m_pcr)));
}


//---------------------------------------------------------------------------
//	@function:
//		CScalarSubqueryQuantified::Matches
//
//	@doc:
//		Match function on operator level
//
//---------------------------------------------------------------------------
BOOL
CScalarSubqueryQuantified::Matches(COperator *pop) const
{
	if (pop->Eopid() != Eopid())
	{
		return false;
	}

	// match if contents are identical
	CScalarSubqueryQuantified *popSsq =
		CScalarSubqueryQuantified::PopConvert(pop);

	return FMultipleColumns() ? popSsq->Pcrs()->Equals(m_colref_array) &&
									popSsq->MdIdOps()->Equals(m_scalar_op_mdids)
							  : popSsq->Pcr() == m_pcr &&
									popSsq->MdIdOp()->Equals(m_scalar_op_mdid);
}


//---------------------------------------------------------------------------
//	@function:
//		CScalarSubqueryQuantified::PcrsUsed
//
//	@doc:
//		Locally used columns
//
//---------------------------------------------------------------------------
CColRefSet *
CScalarSubqueryQuantified::PcrsUsed(CMemoryPool *mp, CExpressionHandle &exprhdl)
{
	// used columns is an empty set unless subquery column is an outer reference
	CColRefSet *pcrs = GPOS_NEW(mp) CColRefSet(mp);

	CColRefSet *pcrsChildOutput =
		exprhdl.DeriveOutputColumns(0 /* child_index */);

	if (FMultipleColumns())
	{
		CColRefArray *colref_array = Pcrs();
		for (ULONG ulCol = 0; ulCol < colref_array->Size(); ulCol++)
		{
			if (!pcrsChildOutput->FMember((*colref_array)[ulCol]))
			{
				// subquery column is not produced by
				// relational child, add it to used columns
				pcrs->Include((*colref_array)[ulCol]);
			}
		}
	}
	else
	{
		if (!pcrsChildOutput->FMember(m_pcr))
		{
			// subquery column is not produced by relational child,
			// add it to used columns
			pcrs->Include(m_pcr);
		}
	}
	return pcrs;
}


//---------------------------------------------------------------------------
//	@function:
//		CScalarSubqueryQuantified::DerivePartitionInfo
//
//	@doc:
//		Derive partition consumers
//
//---------------------------------------------------------------------------
CPartInfo *
CScalarSubqueryQuantified::PpartinfoDerive(CMemoryPool *,  // mp,
										   CExpressionHandle &exprhdl) const
{
	CPartInfo *ppartinfoChild = exprhdl.DerivePartitionInfo(0);
	GPOS_ASSERT(nullptr != ppartinfoChild);
	ppartinfoChild->AddRef();
	return ppartinfoChild;
}


//---------------------------------------------------------------------------
//	@function:
//		CScalarSubqueryQuantified::OsPrint
//
//	@doc:
//		Debug print
//
//---------------------------------------------------------------------------
IOstream &
CScalarSubqueryQuantified::OsPrint(IOstream &os) const
{
	os << SzId();
	os << "(" << PstrOp()->GetBuffer() << ")";
	os << "[";
	if (FMultipleColumns())
	{
		for (ULONG col = 0; col < m_colref_array->Size(); col++)
		{
			((*m_colref_array)[col])->OsPrint(os);
			if (col < m_colref_array->Size() - 1)
			{
				os << ", ";
			}
		}
	}
	else
	{
		m_pcr->OsPrint(os);
	}
	os << "]";

	return os;
}


// EOF
