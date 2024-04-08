//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2012 EMC, Corp.
//
//	@filename:
//		CDXLScalarSubqueryQuantified.h
//
//	@doc:
//		Base class for ANY/ALL subqueries
//---------------------------------------------------------------------------

#ifndef GPDXL_CDXLScalarSubqueryQuantified_H
#define GPDXL_CDXLScalarSubqueryQuantified_H

#include "gpos/base.h"

#include "naucrates/dxl/operators/CDXLNode.h"
#include "naucrates/dxl/operators/CDXLScalar.h"
#include "naucrates/dxl/operators/CDXLScalarBoolExpr.h"
#include "naucrates/dxl/operators/CDXLScalarValuesList.h"
#include "naucrates/md/CMDName.h"
#include "naucrates/md/IMDId.h"

namespace gpdxl
{
using namespace gpmd;

//---------------------------------------------------------------------------
//	@class:
//		CDXLScalarSubqueryQuantified
//
//	@doc:
//		Base class for ANY/ALL subqueries
//
//---------------------------------------------------------------------------
class CDXLScalarSubqueryQuantified : public CDXLScalar
{
public:
	// indices of the subquery elements in the children array
	enum Edxlsqquantified
	{
		EdxlsqquantifiedIndexScalar,
		EdxlsqquantifiedIndexRelational,
		EdxlsqquantifiedIndexSentinel
	};

private:
	// id of the scalar comparison operator
	IMDId *m_scalar_op_mdid;

	// id of the scalar comparison operator
	IMdIdArray *m_scalar_op_mdids;

	// name of scalar comparison operator
	CMDName *m_scalar_op_mdname;

	// colid produced by the relational child of the AnySubquery operator
	ULONG m_colid;

	// colids produced by the relational child of the non-scalar Subquery operator
	ULongPtrArray *m_colids;

	// non-scalar boolop type
	EdxlBoolExprType m_testexprBoolopType;

	// is non-scalar subquery
	BOOL m_isNonScalarSubq = false;

public:
	CDXLScalarSubqueryQuantified(CDXLScalarSubqueryQuantified &) = delete;

	// ctor
	CDXLScalarSubqueryQuantified(CMemoryPool *mp, IMDId *scalar_op_mdid,
								 CMDName *mdname, ULONG colid);

	// ctor
	CDXLScalarSubqueryQuantified(CMemoryPool *mp, IMdIdArray *scalar_op_mdids,
								 CMDName *mdname, ULongPtrArray *colids,
								 EdxlBoolExprType testexpr_booloptype);

	// ctor for Non-SCalar subquery (BOOLEXPR)
	~CDXLScalarSubqueryQuantified() override;

	// serialize the array of output arg types into a comma-separated string
	CWStringDynamic *GetMdIdOpArrayStr() const;

	// scalar operator id
	IMDId *
	GetScalarOpMdId() const
	{
		return m_scalar_op_mdid;
	}

	// non-scalar operator id's
	IMdIdArray *
	GetScalarOpMdIds() const
	{
		return m_scalar_op_mdids;
	}

	// scalar operator name
	const CMDName *
	GetScalarOpMdName() const
	{
		return m_scalar_op_mdname;
	}

	// subquery colid
	ULONG
	GetColId() const
	{
		return m_colid;
	}

	// subquery colids
	ULongPtrArray *
	GetColIds() const
	{
		return m_colids;
	}

	// return subquery testexpr boolop type
	EdxlBoolExprType
	GetBoolOpType() const
	{
		return m_testexprBoolopType;
	}

	// is subquery non-scalar
	BOOL
	IsNonScalarSubq() const
	{
		return m_isNonScalarSubq;
	}

	// serialize operator in DXL format
	void SerializeToDXL(CXMLSerializer *, const CDXLNode *) const override;

	// conversion function
	static CDXLScalarSubqueryQuantified *
	Cast(CDXLOperator *dxl_op)
	{
		GPOS_ASSERT(nullptr != dxl_op);
		GPOS_ASSERT(EdxlopScalarSubqueryAll == dxl_op->GetDXLOperator() ||
					EdxlopScalarSubqueryAny == dxl_op->GetDXLOperator());

		return dynamic_cast<CDXLScalarSubqueryQuantified *>(dxl_op);
	}

	// does the operator return a boolean result
	BOOL
	HasBoolResult(CMDAccessor *	 //md_accessor
	) const override
	{
		return true;
	}

#ifdef GPOS_DEBUG
	// checks whether the operator has valid structure, i.e. number and
	// types of child nodes
	void AssertValid(const CDXLNode *dxlnode,
					 BOOL validate_children) const override;
#endif	// GPOS_DEBUG
};
}  // namespace gpdxl

#endif	// !GPDXL_CDXLScalarSubqueryQuantified_H

// EOF
