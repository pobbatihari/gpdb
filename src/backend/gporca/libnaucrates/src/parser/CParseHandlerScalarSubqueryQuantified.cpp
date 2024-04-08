//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2011 EMC Corp.
//
//	@filename:
//		CParseHandlerScalarSubqueryQuantified.cpp
//
//	@doc:
//		Implementation of the SAX parse handler class for ANY and ALL subquery
//		operators.
//---------------------------------------------------------------------------

#include "naucrates/dxl/parser/CParseHandlerScalarSubqueryQuantified.h"

#include "naucrates/dxl/operators/CDXLOperatorFactory.h"
#include "naucrates/dxl/operators/CDXLScalarSubqueryAll.h"
#include "naucrates/dxl/parser/CParseHandlerFactory.h"
#include "naucrates/dxl/parser/CParseHandlerLogicalOp.h"
#include "naucrates/dxl/parser/CParseHandlerScalarValuesList.h"

using namespace gpdxl;


XERCES_CPP_NAMESPACE_USE

//---------------------------------------------------------------------------
//	@function:
//		CParseHandlerScalarSubqueryQuantified::CParseHandlerScalarSubqueryQuantified
//
//	@doc:
//		Constructor
//
//---------------------------------------------------------------------------
CParseHandlerScalarSubqueryQuantified::CParseHandlerScalarSubqueryQuantified(
	CMemoryPool *mp, CParseHandlerManager *parse_handler_mgr,
	CParseHandlerBase *parse_handler_root)
	: CParseHandlerScalarOp(mp, parse_handler_mgr, parse_handler_root),
	  m_dxl_op(nullptr)
{
}


//---------------------------------------------------------------------------
//	@function:
//		CParseHandlerScalarSubqueryQuantified::StartElement
//
//	@doc:
//		Invoked by Xerces to process an opening tag
//
//---------------------------------------------------------------------------
void
CParseHandlerScalarSubqueryQuantified::StartElement(
	const XMLCh *const element_uri, const XMLCh *const element_local_name,
	const XMLCh *const element_qname, const Attributes &attrs)
{
	// is this a subquery any or subquery all operator
	if (0 == XMLString::compareString(
				 CDXLTokens::XmlstrToken(EdxltokenScalarSubqueryAny),
				 element_local_name) ||
		0 == XMLString::compareString(
				 CDXLTokens::XmlstrToken(EdxltokenScalarSubqueryAll),
				 element_local_name))
	{
		Edxltoken dxl_token = EdxltokenScalarSubqueryAll;
		if (0 == XMLString::compareString(
					 CDXLTokens::XmlstrToken(EdxltokenScalarSubqueryAny),
					 element_local_name))
		{
			dxl_token = EdxltokenScalarSubqueryAny;
		}

		// parse operator name
		const XMLCh *xmlszScalarOpName = CDXLOperatorFactory::ExtractAttrValue(
			attrs, EdxltokenOpName, dxl_token);

		CWStringDynamic *op_name_str =
			CDXLUtils::CreateDynamicStringFromXMLChArray(
				m_parse_handler_mgr->GetDXLMemoryManager(), xmlszScalarOpName);
		CMDName *md_op_name = GPOS_NEW(m_mp) CMDName(m_mp, op_name_str);
		GPOS_DELETE(op_name_str);

		BOOL isNonScalarSubq =
			CDXLOperatorFactory::ExtractConvertAttrValueToBool(
				m_parse_handler_mgr->GetDXLMemoryManager(), attrs,
				EdxltokenIsNonScalarSubquery, dxl_token,
				true,  // is optional
				false  // default value
			);

		if (isNonScalarSubq)
		{
			// parse operator ids
			const XMLCh *xmlszTypes = CDXLOperatorFactory::ExtractAttrValue(
				attrs, EdxltokenOpNos, dxl_token);

			IMdIdArray *mdids = CDXLOperatorFactory::ExtractConvertMdIdsToArray(
				m_parse_handler_mgr->GetDXLMemoryManager(), xmlszTypes,
				EdxltokenOpNos, dxl_token);

			// parse column ids
			ULongPtrArray *colids =
				CDXLOperatorFactory::ExtractConvertValuesToArray(
					m_parse_handler_mgr->GetDXLMemoryManager(), attrs,
					EdxltokenColIds, dxl_token);

			INT TypeIndex = CDXLOperatorFactory::ExtractConvertAttrValueToInt(
				m_parse_handler_mgr->GetDXLMemoryManager(), attrs,
				EdxltokenSubqueryTestExprBoolOpType, dxl_token);

			if (EdxltokenScalarSubqueryAny == dxl_token)
			{
				m_dxl_op = GPOS_NEW(m_mp)
					CDXLScalarSubqueryAny(m_mp, mdids, md_op_name, colids,
										  (EdxlBoolExprType) TypeIndex);
			}
			else
			{
				m_dxl_op = GPOS_NEW(m_mp)
					CDXLScalarSubqueryAll(m_mp, mdids, md_op_name, colids,
										  (EdxlBoolExprType) TypeIndex);
			}
		}
		else
		{
			// parse column id
			ULONG colid = CDXLOperatorFactory::ExtractConvertAttrValueToUlong(
				m_parse_handler_mgr->GetDXLMemoryManager(), attrs,
				EdxltokenColId, dxl_token);

			// parse operator id
			IMDId *mdid_op = CDXLOperatorFactory::ExtractConvertAttrValueToMdId(
				m_parse_handler_mgr->GetDXLMemoryManager(), attrs,
				EdxltokenOpNo, dxl_token);

			if (EdxltokenScalarSubqueryAny == dxl_token)
			{
				m_dxl_op = GPOS_NEW(m_mp)
					CDXLScalarSubqueryAny(m_mp, mdid_op, md_op_name, colid);
			}
			else
			{
				m_dxl_op = GPOS_NEW(m_mp)
					CDXLScalarSubqueryAll(m_mp, mdid_op, md_op_name, colid);
			}
		}
	}
	else if (0 == XMLString::compareString(
					  CDXLTokens::XmlstrToken(EdxltokenScalarValuesList),
					  element_local_name))
	{
		CParseHandlerBase *parse_handler_scalar_vlist =
			CParseHandlerFactory::GetParseHandler(
				m_mp, CDXLTokens::XmlstrToken(EdxltokenScalarValuesList),
				m_parse_handler_mgr, this);
		m_parse_handler_mgr->ActivateParseHandler(parse_handler_scalar_vlist);

		// store parse handlers
		this->Append(parse_handler_scalar_vlist);

		parse_handler_scalar_vlist->startElement(
			element_uri, element_local_name, element_qname, attrs);
	}
	else if (0 == XMLString::compareString(
					  CDXLTokens::XmlstrToken(EdxltokenScalarIdent),
					  element_local_name))
	{
		CParseHandlerBase *parse_handler_scalar_child =
			CParseHandlerFactory::GetParseHandler(
				m_mp, CDXLTokens::XmlstrToken(EdxltokenScalar),
				m_parse_handler_mgr, this);
		m_parse_handler_mgr->ActivateParseHandler(parse_handler_scalar_child);

		// store parse handlers
		this->Append(parse_handler_scalar_child);

		parse_handler_scalar_child->startElement(
			element_uri, element_local_name, element_qname, attrs);
	}
	else
	{
		CParseHandlerBase *parse_handler_logical_child =
			CParseHandlerFactory::GetParseHandler(
				m_mp, CDXLTokens::XmlstrToken(EdxltokenLogical),
				m_parse_handler_mgr, this);
		m_parse_handler_mgr->ActivateParseHandler(parse_handler_logical_child);

		// store parse handlers
		this->Append(parse_handler_logical_child);
		parse_handler_logical_child->startElement(
			element_uri, element_local_name, element_qname, attrs);
	}
}

//---------------------------------------------------------------------------
//	@function:
//		CParseHandlerScalarSubqueryQuantified::EndElement
//
//	@doc:
//		Invoked by Xerces to process a closing tag
//
//---------------------------------------------------------------------------
void
CParseHandlerScalarSubqueryQuantified::EndElement(
	const XMLCh *const,										   // element_uri,
	const XMLCh *const element_local_name, const XMLCh *const  // element_qname
)
{
	if (0 != XMLString::compareString(
				 CDXLTokens::XmlstrToken(EdxltokenScalarSubqueryAll),
				 element_local_name) &&
		0 != XMLString::compareString(
				 CDXLTokens::XmlstrToken(EdxltokenScalarSubqueryAny),
				 element_local_name))
	{
		CWStringDynamic *str = CDXLUtils::CreateDynamicStringFromXMLChArray(
			m_parse_handler_mgr->GetDXLMemoryManager(), element_local_name);
		GPOS_RAISE(gpdxl::ExmaDXL, gpdxl::ExmiDXLUnexpectedTag,
				   str->GetBuffer());
	}

	// construct node from parsed components
	GPOS_ASSERT(nullptr != m_dxl_op);
	GPOS_ASSERT(2 == this->Length());

	m_dxl_node = GPOS_NEW(m_mp) CDXLNode(m_mp, m_dxl_op);

	CParseHandlerScalarOp *parse_handler_scalar_child =
		dynamic_cast<CParseHandlerScalarOp *>((*this)[0]);

	if (nullptr == parse_handler_scalar_child)
	{
		CParseHandlerScalarValuesList *parse_handler_scalar_Vlist_child =
			dynamic_cast<CParseHandlerScalarValuesList *>((*this)[0]);
		AddChildFromParseHandler(parse_handler_scalar_Vlist_child);
	}
	else
	{
		// add scalar child
		AddChildFromParseHandler(parse_handler_scalar_child);
	}

	CParseHandlerLogicalOp *parse_handler_logical_child =
		dynamic_cast<CParseHandlerLogicalOp *>((*this)[1]);

	AddChildFromParseHandler(parse_handler_logical_child);

#ifdef GPOS_DEBUG
	m_dxl_op->AssertValid(m_dxl_node, false /* validate_children */);
#endif	// GPOS_DEBUG

	// deactivate handler
	m_parse_handler_mgr->DeactivateHandler();
}

// EOF
