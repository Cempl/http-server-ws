/**********************************************************************************************/
/* FBL_I_Type_Composite.h		                                           					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_Type_Composite_h
	#define _FBL_I_Type_Composite_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Interfaces/FBL_I_Type.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE(I_Type_Composite);

/**********************************************************************************************/
// 
//StubInterface
interface FBL_SHARED_EXP_CLASS I_Type_Composite : public I_Unknown
{

virtual 					~I_Type_Composite( void );

	public://///////////////////////////////////////////////////////////////////////////////////

	//
	// Composite type specific
	//

	// Member count

							/// Return count of data members.
							/// CREATE TYPE metric_meter AS DOUBLE;	-- 0 member (or better not I_CompositeType)
							/// CREATE TYPE bug_status AS ENUM ('unknown', 'new'); -- 0 members
							///  (or better not I_CompositeType)
							///
							/// CREATE TYPE Rectangle UNDER Shape2D
							///	AS(
							///		width DOUBLE,
							///		length DOUBLE
							///	)	-- 2 members
virtual vuint16				get_MemberCount( void ) const = 0;


virtual void				AddMember( I_Type_Ptr	inpType ) = 0;

							/// For Rectangle it is vcReal for each member.
virtual I_Type_Ptr			get_MemberType( vuint16 inIndex ) const = 0;

							/// For Rectangle it is 'width', 'length'
virtual I_Type_Ptr			get_MemberType( const String& inMemberName ) const = 0;


	// VarName
							// For Rectangle.with it is "with"
virtual const String&		get_VarName( vuint16 inIndex ) const = 0;

virtual void				put_VarName( vuint16 inIndex, const String& inName ) = 0;

};


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_I_Type_Composite
