//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2012 EMC Corp.
//
//	@filename:
//		CScalarSubqueryAny.cpp
//
//	@doc:
//		Implementation of scalar subquery ANY operator
//---------------------------------------------------------------------------

#include "gpopt/operators/CScalarSubqueryAny.h"

#include "gpos/base.h"

#include "gpopt/base/CUtils.h"

using namespace gpopt;

//---------------------------------------------------------------------------
//	@function:
//		CScalarSubqueryAny::CScalarSubqueryAny
//
//	@doc:
//		Ctor
//
//---------------------------------------------------------------------------
CScalarSubqueryAny::CScalarSubqueryAny(CMemoryPool *mp, IMDId *scalar_op_mdid,
									   const CWStringConst *pstrScalarOp,
									   const CColRef *colref)
	: CScalarSubqueryQuantified(mp, scalar_op_mdid, pstrScalarOp, colref)
{
}

//---------------------------------------------------------------------------
//	@function:
//		CScalarSubqueryAny::CScalarSubqueryAny
//
//	@doc:
//		Ctor
//
//---------------------------------------------------------------------------
CScalarSubqueryAny::CScalarSubqueryAny(
	CMemoryPool *mp, IMdIdArray *scalar_op_mdids,
	const CWStringConst *pstrScalarOp, CColRefArray *colref_array,
	CScalarBoolOp::EBoolOperator testexpr_booloptype)
	: CScalarSubqueryQuantified(mp, scalar_op_mdids, pstrScalarOp, colref_array,
								testexpr_booloptype)
{
}

//---------------------------------------------------------------------------
//	@function:
//		CScalarSubqueryAny::PopCopyWithRemappedColumns
//
//	@doc:
//		Return a copy of the operator with remapped columns
//
//---------------------------------------------------------------------------
COperator *
CScalarSubqueryAny::PopCopyWithRemappedColumns(CMemoryPool *mp,
											   UlongToColRefMap *colref_mapping,
											   BOOL must_exist)
{
	CWStringConst *pstrScalarOp =
		GPOS_NEW(mp) CWStringConst(mp, PstrOp()->GetBuffer());

	if (FMultipleColumns())
	{
		IMdIdArray *mdids = MdIdOps();
		CColRefArray *colref_array =
			CUtils::PdrgpcrRemap(mp, Pcrs(), colref_mapping, must_exist);
		return GPOS_NEW(mp) CScalarSubqueryAny(mp, mdids, pstrScalarOp,
											   colref_array, GetBoolOptype());
	}

	IMDId *scalar_op_mdid = MdIdOp();
	scalar_op_mdid->AddRef();

	CColRef *colref = CUtils::PcrRemap(Pcr(), colref_mapping, must_exist);
	return GPOS_NEW(mp)
		CScalarSubqueryAny(mp, scalar_op_mdid, pstrScalarOp, colref);
}

// EOF
