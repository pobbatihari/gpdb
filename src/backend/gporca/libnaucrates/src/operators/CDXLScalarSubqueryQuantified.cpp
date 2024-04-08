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
	m_isNonScalarSubq = true;
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
	if (IsNonScalarSubq())
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
//		CDXLScalarSubqueryQuantified::GetMdIdOpArrayStr
//
//	@doc:
//		Serialize the array of MdIds into a comma-separated string
//
//---------------------------------------------------------------------------
CWStringDynamic *
CDXLScalarSubqueryQuantified::GetMdIdOpArrayStr() const
{
	GPOS_ASSERT(nullptr != m_scalar_op_mdids);
	CWStringDynamic *str = GPOS_NEW(m_mp) CWStringDynamic(m_mp);

	const ULONG len = m_scalar_op_mdids->Size();
	for (ULONG ul = 0; ul < len; ul++)
	{
		IMDId *mdid = (*m_scalar_op_mdids)[ul];
		if (ul == len - 1)
		{
			// last element: do not print a comma
			str->AppendFormat(GPOS_WSZ_LIT("%ls"), mdid->GetBuffer());
		}
		else
		{
			str->AppendFormat(
				GPOS_WSZ_LIT("%ls%ls"), mdid->GetBuffer(),
				CDXLTokens::GetDXLTokenStr(EdxltokenComma)->GetBuffer());
		}
	}

	return str;
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

	if (IsNonScalarSubq())
	{
		xml_serializer->AddAttribute(
			CDXLTokens::GetDXLTokenStr(EdxltokenIsNonScalarSubquery),
			IsNonScalarSubq());
		CWStringDynamic *output_arg_type_array_str = GetMdIdOpArrayStr();
		xml_serializer->AddAttribute(CDXLTokens::GetDXLTokenStr(EdxltokenOpNos),
									 output_arg_type_array_str);
		GPOS_DELETE(output_arg_type_array_str);

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
