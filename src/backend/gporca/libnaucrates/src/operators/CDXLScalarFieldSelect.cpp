//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2023 VMware, Inc. or its affiliates. All Rights Reserved.
//
//	@filename:
//		CDXLScalarFieldSelect.cpp
//
//	@doc:
//		Implementation of DXL Scalar FIELDSELECT operator
//---------------------------------------------------------------------------

#include "naucrates/dxl/operators/CDXLScalarFieldSelect.h"

#include "gpopt/mdcache/CMDAccessor.h"
#include "naucrates/dxl/operators/CDXLNode.h"
#include "naucrates/dxl/xml/CXMLSerializer.h"

using namespace gpos;
using namespace gpdxl;

//---------------------------------------------------------------------------
//	@function:
//		CDXLScalarFieldSelect::CDXLScalarFieldSelect
//
//	@doc:
//		Ctor
//
//---------------------------------------------------------------------------
CDXLScalarFieldSelect::CDXLScalarFieldSelect(CMemoryPool *mp, IMDId *field_mdid,
											 IMDId *coll_mdid, INT mode_type,
											 INT field_num)
	: CDXLScalar(mp),
	  m_result_field_mdid(field_mdid),
	  m_result_coll_mdid(coll_mdid),
	  m_output_type_mode(mode_type),
	  m_field_num(field_num)
{
	GPOS_ASSERT(m_result_field_mdid->IsValid());
}

//---------------------------------------------------------------------------
//	@function:
//		CDXLScalarFieldSelect::~CDXLScalarFieldSelect
//
//	@doc:
//		Dtor
//
//---------------------------------------------------------------------------
CDXLScalarFieldSelect::~CDXLScalarFieldSelect()
{
	m_result_field_mdid->Release();
	m_result_coll_mdid->Release();
}

//---------------------------------------------------------------------------
//	@function:
//		CDXLScalarFieldSelect::GetDXLOperator
//
//	@doc:
//		Operator type
//
//---------------------------------------------------------------------------
Edxlopid
CDXLScalarFieldSelect::GetDXLOperator() const
{
	return EdxlopScalarFieldSelect;
}

//---------------------------------------------------------------------------
//	@function:
//		CDXLScalarFieldSelect::GetDXLFieldMDId
//
//	@doc:
//		Returns mdid of the field
//
//---------------------------------------------------------------------------
IMDId *
CDXLScalarFieldSelect::GetDXLFieldMDId() const
{
	return m_result_field_mdid;
}

//---------------------------------------------------------------------------
//	@function:
//		CDXLScalarFieldSelect::GetDXLCollMDId
//
//	@doc:
//		Returns collation mdid of the field
//
//---------------------------------------------------------------------------
IMDId *
CDXLScalarFieldSelect::GetDXLCollMDId() const
{
	return m_result_coll_mdid;
}

//---------------------------------------------------------------------------
//	@function:
//		CDXLScalarFieldSelect::GetDXLModeType
//
//	@doc:
//		Returns output type mode
//
//---------------------------------------------------------------------------
INT
CDXLScalarFieldSelect::GetDXLModeType() const
{
	return m_output_type_mode;
}

//---------------------------------------------------------------------------
//	@function:
//		CDXLScalarFieldSelect::ReturnsSet
//
//	@doc:
//		Returns attribute number of the field to extract
//
//---------------------------------------------------------------------------
USINT
CDXLScalarFieldSelect::GetDXLFieldNumber() const
{
	return m_field_num;
}

//---------------------------------------------------------------------------
//	@function:
//		CDXLScalarFieldSelect::GetOpNameStr
//
//	@doc:
//		Operator name
//
//---------------------------------------------------------------------------
const CWStringConst *
CDXLScalarFieldSelect::GetOpNameStr() const
{
	return CDXLTokens::GetDXLTokenStr(EdxltokenScalarFieldSelect);
}

//---------------------------------------------------------------------------
//	@function:
//		CDXLScalarFieldSelect::SerializeToDXL
//
//	@doc:
//		Serialize operator in DXL format
//
//---------------------------------------------------------------------------
void
CDXLScalarFieldSelect::SerializeToDXL(CXMLSerializer *xml_serializer,
									  const CDXLNode *dxlnode) const
{
	const CWStringConst *element_name = GetOpNameStr();

	xml_serializer->OpenElement(
		CDXLTokens::GetDXLTokenStr(EdxltokenNamespacePrefix), element_name);

	m_result_field_mdid->Serialize(
		xml_serializer,
		CDXLTokens::GetDXLTokenStr(EdxltokenScalarFieldSelectFieldType));
	xml_serializer->AddAttribute(
		CDXLTokens::GetDXLTokenStr(EdxltokenScalarFieldSelectModeType),
		m_output_type_mode);
	m_result_coll_mdid->Serialize(
		xml_serializer, CDXLTokens::GetDXLTokenStr(EdxltokenScalarFieldSelectCollId));
	xml_serializer->AddAttribute(
		CDXLTokens::GetDXLTokenStr(EdxltokenScalarFieldSelectFieldNum), m_field_num);

	dxlnode->SerializeChildrenToDXL(xml_serializer);

	xml_serializer->CloseElement(
		CDXLTokens::GetDXLTokenStr(EdxltokenNamespacePrefix), element_name);
}

#ifdef GPOS_DEBUG
//---------------------------------------------------------------------------
//	@function:
//		CDXLScalarFieldSelect::AssertValid
//
//	@doc:
//		Checks whether operator node is well-structured
//
//---------------------------------------------------------------------------
void
CDXLScalarFieldSelect::AssertValid(const CDXLNode *dxlnode,
								   BOOL validate_children) const
{
	const ULONG arity = dxlnode->Arity();
	for (ULONG ul = 0; ul < arity; ++ul)
	{
		CDXLNode *child_dxlnode = (*dxlnode)[ul];
		GPOS_ASSERT(EdxloptypeScalar ==
					child_dxlnode->GetOperator()->GetDXLOperatorType());

		if (validate_children)
		{
			child_dxlnode->GetOperator()->AssertValid(child_dxlnode,
													  validate_children);
		}
	}
}
#endif	// GPOS_DEBUG

// EOF
