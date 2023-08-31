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

#ifndef GPDXL_CDXLSCALARFIELDSELECT_H
#define GPDXL_CDXLSCALARFIELDSELECT_H

#include "gpos/base.h"

#include "naucrates/dxl/operators/CDXLScalar.h"
#include "naucrates/md/IMDId.h"

namespace gpdxl
{
using namespace gpos;
using namespace gpmd;

//---------------------------------------------------------------------------
//	@class:
//		CDXLScalarAggref
//
//	@doc:
//		Class for representing DXL FIELDSELECT
//
//---------------------------------------------------------------------------
class CDXLScalarFieldSelect : public CDXLScalar
{
private:
	// type of the field
	IMDId *m_dxl_field_type;

	// collation OID of the field
	IMDId *m_dxl_field_collation;

	// output typmod (usually -1)
	INT m_dxl_type_modifier;

	// attribute number of field to extract
	SINT m_dxl_field_number;

	// private copy ctor
	CDXLScalarFieldSelect(const CDXLScalarFieldSelect &);

public:
	// ctor/dtor
	CDXLScalarFieldSelect(CMemoryPool *mp, IMDId *field_type,
						  IMDId *field_collation, INT type_modifier,
						  SINT field_number);

	virtual ~CDXLScalarFieldSelect();

	// ident accessors
	virtual Edxlopid GetDXLOperator() const;

	// DXL operator name
	virtual const CWStringConst *GetOpNameStr() const;

	// serialize operator in DXL format
	virtual void SerializeToDXL(CXMLSerializer *xml_serializer,
								const CDXLNode *dxlnode) const;

	// mdid of the field
	IMDId *GetDXLFieldType() const;

	// collation mdid of the field
	IMDId *GetDXLFieldCollation() const;

	// output type mode
	INT GetDXLTypeModifier() const;

	// attribute number of the field
	SINT GetDXLFieldNumber() const;

	// conversion function
	static CDXLScalarFieldSelect *
	Cast(CDXLOperator *dxl_op)
	{
		GPOS_ASSERT(NULL != dxl_op);
		GPOS_ASSERT(EdxlopScalarFieldSelect == dxl_op->GetDXLOperator());

		return dynamic_cast<CDXLScalarFieldSelect *>(dxl_op);
	}

	// does the operator return a boolean result
	virtual BOOL
	HasBoolResult(CMDAccessor *	 //md_accessor
	) const
	{
		return true;
	}

#ifdef GPOS_DEBUG
	// checks whether the operator has valid structure, i.e. number and
	// types of child nodes
	virtual void AssertValid(const CDXLNode *dxlnode,
							 BOOL validate_children) const;
#endif	// GPOS_DEBUG
};
}  // namespace gpdxl

#endif	// !GPDB_CDXLSCALARFIELDSELECT_H

// EOF
