//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2023 VMware, Inc. or its affiliates. All Rights Reserved.
//
//	@filename:
//		CScalarFieldSelect.h
//
//	@doc:
//		Class for representing FIELDSELECT
//---------------------------------------------------------------------------

#ifndef GPOPT_CSCALARFIELDSELECT_H
#define GPOPT_CSCALARFIELDSELECT_H

#include "gpos/base.h"

#include "gpopt/operators/CScalar.h"
#include "naucrates/md/IMDId.h"

namespace gpopt
{
using namespace gpos;
using namespace gpmd;

//---------------------------------------------------------------------------
//	@class:
//		CScalarFieldSelect
//
//	@doc:
//		Class for representing scalar FieldSelect
//
//---------------------------------------------------------------------------
class CScalarFieldSelect : public CScalar
{
private:
	// type of the field
	IMDId *m_field_type;

	// collation OID of the field
	IMDId *m_field_collation;

	// output typmod (usually -1)
	INT m_type_modifier;

	// attribute number of field to extract
	SINT m_field_number;

	// private copy ctor
	CScalarFieldSelect(const CScalarFieldSelect &);

public:
	// ctor/dtor
	CScalarFieldSelect(CMemoryPool *mp, IMDId *field_type,
					   IMDId *field_collation, INT type_modifier,
					   SINT field_number);

	virtual ~CScalarFieldSelect();

	// ident accessors
	virtual EOperatorId
	Eopid() const
	{
		return EopScalarFieldSelect;
	}

	// operator name
	virtual const CHAR *
	SzId() const
	{
		return "CScalarFieldSelect";
	}

	// operator specific hash function
	virtual ULONG HashValue() const;

	// match function
	virtual BOOL Matches(COperator *pop) const;


	// mdid of the field
	virtual IMDId *
	MdidType() const
	{
		return m_field_type;
	}

	// field collation mdid
	IMDId *
	FieldCollation() const
	{
		return m_field_collation;
	}

	// output mode type
	virtual INT
	TypeModifier() const
	{
		return m_type_modifier;
	}

	// attribute number of field
	SINT
	FieldNumber() const
	{
		return m_field_number;
	}

	// conversion function
	static CScalarFieldSelect *
	PopConvert(COperator *pop)
	{
		GPOS_ASSERT(NULL != pop);
		GPOS_ASSERT(EopScalarFieldSelect == pop->Eopid());

		return dynamic_cast<CScalarFieldSelect *>(pop);
	}

	// sensitivity to order of inputs
	virtual BOOL
	FInputOrderSensitive() const
	{
		return true;
	}

	// return a copy of the operator with remapped columns
	virtual COperator *
	PopCopyWithRemappedColumns(CMemoryPool *,		//mp,
							   UlongToColRefMap *,	//colref_mapping,
							   BOOL					//must_exist
	)
	{
		return PopCopyDefault();
	}
};
}  // namespace gpopt

#endif	// !GPDB_CSCALARFIELDSELECT_H

// EOF
