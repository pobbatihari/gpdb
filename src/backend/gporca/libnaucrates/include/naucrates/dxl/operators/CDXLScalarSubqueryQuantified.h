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
	// mdids of OpExpr types
	IMdIdArray *m_scalar_op_mdids;

	// name of scalar comparison operator
	CMDName *m_scalar_op_mdname;

	// colids produced by the relational child of the Subquery operator
	ULongPtrArray *m_colids;

	// multi-column scalar subquery testexpr boolop type
	EdxlBoolExprType m_testexprBoolopType;

public:
	CDXLScalarSubqueryQuantified(CDXLScalarSubqueryQuantified &) = delete;

	// ctor
	CDXLScalarSubqueryQuantified(CMemoryPool *mp, IMdIdArray *scalar_op_mdids,
								 CMDName *mdname, ULongPtrArray *colids,
								 EdxlBoolExprType testexpr_booloptype);

	~CDXLScalarSubqueryQuantified() override;

	// operator mdids
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

	// is multi-column scalar subquery
	BOOL
	FMultipleColumns() const
	{
		return (nullptr != m_scalar_op_mdids && m_scalar_op_mdids->Size() > 1);
	}

	// serialize operator in DXL format
	void SerializeToDXL(CXMLSerializer *, const CDXLNode *) const override;

	const CWStringConst *GetBoolOpTypeStr() const;

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
