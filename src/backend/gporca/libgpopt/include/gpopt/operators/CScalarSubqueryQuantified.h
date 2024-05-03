//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2011 EMC Corp.
//
//	@filename:
//		CScalarSubqueryQuantified.h
//
//	@doc:
//		Parent class for quantified subquery operators
//---------------------------------------------------------------------------
#ifndef GPOPT_CScalarSubqueryQuantified_H
#define GPOPT_CScalarSubqueryQuantified_H

#include "gpos/base.h"

#include "gpopt/operators/CScalar.h"
#include "gpopt/operators/CScalarBoolOp.h"
#include "gpopt/xforms/CSubqueryHandler.h"

namespace gpopt
{
using namespace gpos;

// fwd declarations
class CExpressionHandle;

//---------------------------------------------------------------------------
//	@class:
//		CScalarSubqueryQuantified
//
//	@doc:
//		Parent class for quantified subquery operators (ALL/ANY subqueries);
//		A quantified subquery expression has two children:
//		- Logical child: the inner logical expression
//		- Scalar child:	the scalar expression in the outer expression that
//		is used in quantified comparison;
//
//		Example: SELECT * from R where a+b = ANY (SELECT c from S);
//		- logical child: (SELECT c from S)
//		- scalar child : (a+b)
//
//---------------------------------------------------------------------------
class CScalarSubqueryQuantified : public CScalar
{
private:
	// mdids of comparison operators
	IMdIdArray *m_scalar_op_mdids;

	// name of comparison operator
	const CWStringConst *m_pstrScalarOp;

	// column references used in comparison
	CColRefArray *m_pcrs;

	// scalar subquery testexpr boolop type
	CScalarBoolOp::EBoolOperator m_testexprBoolopType;

protected:
	// ctor
	CScalarSubqueryQuantified(CMemoryPool *mp, IMdIdArray *scalar_op_mdids,
							  const CWStringConst *pstrScalarOp,
							  CColRefArray *colref_array,
							  CScalarBoolOp::EBoolOperator testexpr_booloptype);

	// dtor
	~CScalarSubqueryQuantified() override;

public:
	CScalarSubqueryQuantified(const CScalarSubqueryQuantified &) = delete;

	// operator name accessor
	const CWStringConst *PstrOp() const;

	// column reference accessor
	CColRefArray *
	Pcrs()
	{
		return m_pcrs;
	}

	// is multi-column scalar subquery
	// Example query:
	//   select * from foo where (a, b) IN (select c, d from bar);
	BOOL
	FMultipleColumns() const
	{
		return (nullptr != m_scalar_op_mdids && m_scalar_op_mdids->Size() > 1);
	}

	// return subquery testexpr boolop type
	CScalarBoolOp::EBoolOperator
	GetBoolOptype() const
	{
		return m_testexprBoolopType;
	}

	// array of mdid operators
	IMdIdArray *
	MdIdOps() const
	{
		return m_scalar_op_mdids;
	}

	// return the type of the scalar expression
	IMDId *MdidType() const override;

	// operator specific hash function
	ULONG HashValue() const override;

	// match function
	BOOL Matches(COperator *pop) const override;

	// sensitivity to order of inputs
	BOOL
	FInputOrderSensitive() const override
	{
		return true;
	}

	// return locally used columns
	CColRefSet *PcrsUsed(CMemoryPool *mp, CExpressionHandle &exprhdl) override;

	// derive partition consumer info
	CPartInfo *PpartinfoDerive(CMemoryPool *mp,
							   CExpressionHandle &exprhdl) const override;

	// conversion function
	static CScalarSubqueryQuantified *
	PopConvert(COperator *pop)
	{
		GPOS_ASSERT(nullptr != pop);
		GPOS_ASSERT(EopScalarSubqueryAny == pop->Eopid() ||
					EopScalarSubqueryAll == pop->Eopid());

		return dynamic_cast<CScalarSubqueryQuantified *>(pop);
	}

	// print
	IOstream &OsPrint(IOstream &os) const override;

};	// class CScalarSubqueryQuantified
}  // namespace gpopt

#endif	// !GPOPT_CScalarSubqueryQuantified_H

// EOF
