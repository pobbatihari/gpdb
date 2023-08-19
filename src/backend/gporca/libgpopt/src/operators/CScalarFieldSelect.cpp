//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2023 VMware, Inc. or its affiliates. All Rights Reserved.
//
//	@filename:
//		CScalarFieldSelect.cpp
//
//	@doc:
//		Implementation of scalar FIELDSELECT
//---------------------------------------------------------------------------

#include "gpopt/operators/CScalarFieldSelect.h"

#include "gpos/base.h"

using namespace gpopt;
using namespace gpmd;


//---------------------------------------------------------------------------
//	@function:
//		CScalarFieldSelect::CScalarFieldSelect
//
//	@doc:
//		Ctor
//
//---------------------------------------------------------------------------
CScalarFieldSelect::CScalarFieldSelect(CMemoryPool *mp, IMDId *field_type,
									   IMDId *coll_id, INT mode_type,
									   USINT field_number)
	: CScalar(mp),
	  m_field_mdid(field_type),
	  m_coll_mdid(coll_id),
	  m_mode_type(mode_type),
	  m_field_num(field_number)
{
	GPOS_ASSERT(m_field_mdid->IsValid());
}

//---------------------------------------------------------------------------
//	@function:
//		CScalarFieldSelect::~CScalarFieldSelect
//
//	@doc:
//		Dtor
//
//---------------------------------------------------------------------------
CScalarFieldSelect::~CScalarFieldSelect()
{
	m_field_mdid->Release();
	m_coll_mdid->Release();
}


//---------------------------------------------------------------------------
//	@function:
//		CScalarFieldSelect::Matches
//
//	@doc:
//		Match function on operator level
//
//---------------------------------------------------------------------------
BOOL
CScalarFieldSelect::Matches(COperator *pop) const
{
	if (pop->Eopid() != Eopid())
	{
		return false;
	}
	CScalarFieldSelect *popFieldSelect = CScalarFieldSelect::PopConvert(pop);

	// match attribute field number and type of the field
	return popFieldSelect->MdidType()->Equals(MdidType()) &&
		   popFieldSelect->FieldNumber() == FieldNumber();
}


// EOF
