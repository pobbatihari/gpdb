//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2011 EMC, Corp.
//
//	@filename:
//		CDXLScalarSubqueryAll.cpp
//
//	@doc:
//		Implementation of subquery ALL
//---------------------------------------------------------------------------

#include "naucrates/dxl/operators/CDXLScalarSubqueryAll.h"

#include "gpos/string/CWStringDynamic.h"

#include "naucrates/dxl/operators/CDXLNode.h"
#include "naucrates/dxl/xml/CXMLSerializer.h"

using namespace gpos;
using namespace gpdxl;
using namespace gpmd;

//---------------------------------------------------------------------------
//	@function:
//		CDXLScalarSubqueryAll::CDXLScalarSubqueryAll
//
//	@doc:
//		Constructor
//
//---------------------------------------------------------------------------
CDXLScalarSubqueryAll::CDXLScalarSubqueryAll(
	CMemoryPool *mp, IMdIdArray *scalar_op_mdids, CMDName *scalar_op_mdname,
	ULongPtrArray *colids, EdxlBoolExprType testexpr_booloptype)
	: CDXLScalarSubqueryQuantified(mp, scalar_op_mdids, scalar_op_mdname,
								   colids, testexpr_booloptype)
{
}

//---------------------------------------------------------------------------
//	@function:
//		CDXLScalarSubqueryAll::GetDXLOperator
//
//	@doc:
//		Operator type
//
//---------------------------------------------------------------------------
Edxlopid
CDXLScalarSubqueryAll::GetDXLOperator() const
{
	return EdxlopScalarSubqueryAll;
}


//---------------------------------------------------------------------------
//	@function:
//		CDXLScalarSubqueryAll::GetOpNameStr
//
//	@doc:
//		Operator name
//
//---------------------------------------------------------------------------
const CWStringConst *
CDXLScalarSubqueryAll::GetOpNameStr() const
{
	return CDXLTokens::GetDXLTokenStr(EdxltokenScalarSubqueryAll);
}

// EOF
