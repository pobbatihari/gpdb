//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2012 EMC, Corp.
//
//	@filename:
//		CDXLScalarSubqueryQuantified.cpp
//
//	@doc:
//		Implementation of quantified subquery operator
//---------------------------------------------------------------------------

#include "naucrates/dxl/operators/CDXLScalarSubqueryQuantified.h"

#include "gpos/string/CWStringDynamic.h"

#include "naucrates/dxl/CDXLUtils.h"
#include "naucrates/dxl/operators/CDXLNode.h"
#include "naucrates/dxl/xml/CXMLSerializer.h"

using namespace gpos;
using namespace gpdxl;
using namespace gpmd;

//---------------------------------------------------------------------------
//	@function:
//		CDXLScalarSubqueryQuantified::CDXLScalarSubqueryQuantified
//
//	@doc:
//		Constructor
//
//---------------------------------------------------------------------------
CDXLScalarSubqueryQuantified::CDXLScalarSubqueryQuantified(
	CMemoryPool *mp, IMDId *scalar_op_mdid, CMDName *scalar_op_mdname,
	ULONG colid)
	: CDXLScalar(mp),
	  m_scalar_op_mdid(scalar_op_mdid),
	  m_scalar_op_mdname(scalar_op_mdname),
	  m_colid(colid)
{
	GPOS_ASSERT(scalar_op_mdid->IsValid());
	GPOS_ASSERT(nullptr != scalar_op_mdname);
}

//---------------------------------------------------------------------------
//	@function:
//		CDXLScalarSubqueryQuantified::CDXLScalarSubqueryQuantified
//
//	@doc:
//		Constructor
//
//---------------------------------------------------------------------------
CDXLScalarSubqueryQuantified::CDXLScalarSubqueryQuantified(
	CMemoryPool *mp, IMdIdArray *scalar_op_mdids, CMDName *scalar_op_mdname,
	ULongPtrArray *colids, EdxlBoolExprType testexpr_booloptype)
	: CDXLScalar(mp),
	  m_scalar_op_mdids(scalar_op_mdids),
	  m_scalar_op_mdname(scalar_op_mdname),
	  m_colids(colids),
	  m_testexprBoolopType(testexpr_booloptype)
{
	GPOS_ASSERT(nullptr != scalar_op_mdids);
	GPOS_ASSERT(nullptr != scalar_op_mdname);
	GPOS_ASSERT(nullptr != m_colids);
}

//---------------------------------------------------------------------------
//	@function:
//		CDXLScalarSubqueryQuantified::~CDXLScalarSubqueryQuantified
//
//	@doc:
//		Destructor
//
//---------------------------------------------------------------------------
CDXLScalarSubqueryQuantified::~CDXLScalarSubqueryQuantified()
{
	if (FMultipleColumns())
	{
		m_colids->Release();
		m_scalar_op_mdids->Release();
	}
	else
	{
		m_scalar_op_mdid->Release();
	}
	GPOS_DELETE(m_scalar_op_mdname);
}


//---------------------------------------------------------------------------
//	@function:
//		CDXLScalarSubqueryQuantified::SerializeToDXL
//
//	@doc:
//		Serialize operator in DXL format
//
//---------------------------------------------------------------------------
void
CDXLScalarSubqueryQuantified::SerializeToDXL(CXMLSerializer *xml_serializer,
											 const CDXLNode *dxlnode) const
{
	const CWStringConst *element_name = GetOpNameStr();
	xml_serializer->OpenElement(
		CDXLTokens::GetDXLTokenStr(EdxltokenNamespacePrefix), element_name);

	// serialize operator id and name
	xml_serializer->AddAttribute(CDXLTokens::GetDXLTokenStr(EdxltokenOpName),
								 m_scalar_op_mdname->GetMDName());

	if (FMultipleColumns())
	{
		xml_serializer->AddAttribute(
			CDXLTokens::GetDXLTokenStr(EdxltokenMultiColumnScalarSubquery),
			FMultipleColumns());
		CWStringDynamic *mdids =
			CDXLUtils::SerializeToCommaSeparatedString(m_mp, m_scalar_op_mdids);
		;
		xml_serializer->AddAttribute(CDXLTokens::GetDXLTokenStr(EdxltokenOpNos),
									 mdids);
		GPOS_DELETE(mdids);

		CWStringDynamic *colids = CDXLUtils::Serialize(m_mp, m_colids);
		GPOS_ASSERT(nullptr != colids);
		xml_serializer->AddAttribute(
			CDXLTokens::GetDXLTokenStr(EdxltokenColIds), colids);
		GPOS_DELETE(colids);

		xml_serializer->AddAttribute(
			CDXLTokens::GetDXLTokenStr(EdxltokenSubqueryTestExprBoolOpType),
			GetBoolOpType());
	}
	else
	{
		m_scalar_op_mdid->Serialize(xml_serializer,
									CDXLTokens::GetDXLTokenStr(EdxltokenOpNo));
		xml_serializer->AddAttribute(CDXLTokens::GetDXLTokenStr(EdxltokenColId),
									 m_colid);
	}

	dxlnode->SerializeChildrenToDXL(xml_serializer);
	xml_serializer->CloseElement(
		CDXLTokens::GetDXLTokenStr(EdxltokenNamespacePrefix), element_name);
}

#ifdef GPOS_DEBUG
//---------------------------------------------------------------------------
//	@function:
//		CDXLScalarSubqueryQuantified::AssertValid
//
//	@doc:
//		Checks whether operator node is well-structured
//
//---------------------------------------------------------------------------
void
CDXLScalarSubqueryQuantified::AssertValid(const CDXLNode *dxlnode,
										  BOOL validate_children) const
{
	GPOS_ASSERT(2 == dxlnode->Arity());

	CDXLNode *pdxlnScalarChild = (*dxlnode)[EdxlsqquantifiedIndexScalar];
	CDXLNode *pdxlnRelationalChild =
		(*dxlnode)[EdxlsqquantifiedIndexRelational];

	GPOS_ASSERT(EdxloptypeScalar ==
				pdxlnScalarChild->GetOperator()->GetDXLOperatorType());
	GPOS_ASSERT(EdxloptypeLogical ==
				pdxlnRelationalChild->GetOperator()->GetDXLOperatorType());

	dxlnode->AssertValid(validate_children);
}
#endif	// GPOS_DEBUG

// EOF
