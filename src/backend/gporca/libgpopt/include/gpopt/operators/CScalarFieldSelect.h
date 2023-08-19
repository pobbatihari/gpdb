//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2023 VMware, Inc. or its affiliates. All Rights Reserved.
//
//	@filename:
//		CScalarFieldSelect.h
//
//	@doc:
//		Class for representing  FIELDSELECT
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
	IMDId *m_field_mdid;

	// collation OID of the field
	IMDId *m_coll_mdid;

	// output typmod (usually -1)
	INT m_mode_type;

	// attribute number of field to extract
	USINT m_field_num;

public:
	CScalarFieldSelect(const CScalarFieldSelect &) = delete;

	// ctor/dtor
	CScalarFieldSelect(CMemoryPool *mp, IMDId *field_mdid, IMDId *coll_mdid,
					   INT mode_type, USINT field_num);

	~CScalarFieldSelect() override;

	// ident accessors
	EOperatorId
	Eopid() const override
	{
		return EopScalarFieldSelect;
	}

	// operator name
	const CHAR *
	SzId() const override
	{
		return "CScalarFieldSelect";
	}

	// match function
	BOOL Matches(COperator *pop) const override;


	// mdid of the field
	IMDId *
	MdidType() const override
	{
		return m_field_mdid;
	}

	// field collation mdid
	IMDId *
	GetColl_Id() const
	{
		return m_coll_mdid;
	}

	// output mode type
	INT
	ModeType() const
	{
		return m_mode_type;
	}

	// attribute number of field
	USINT
	FieldNumber() const
	{
		return m_field_num;
	}

	// conversion function
	static CScalarFieldSelect *
	PopConvert(COperator *pop)
	{
		GPOS_ASSERT(nullptr != pop);
		GPOS_ASSERT(EopScalarFieldSelect == pop->Eopid());

		return dynamic_cast<CScalarFieldSelect *>(pop);
	}

	// sensitivity to order of inputs
	BOOL
	FInputOrderSensitive() const override
	{
		return true;
	}

	// return a copy of the operator with remapped columns
	COperator *
	PopCopyWithRemappedColumns(CMemoryPool *,		//mp,
							   UlongToColRefMap *,	//colref_mapping,
							   BOOL					//must_exist
							   ) override
	{
		return PopCopyDefault();
	}
};
}  // namespace gpopt

#endif	// !GPDB_CSCALARFIELDSELECT_H

// EOF
