/**********************************************************************************************/
/* FBL_I_LinkOnValues.h	                                                      				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_LinkOnValues_h 
	#define _FBL_I_LinkOnValues_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE(I_Field);
SMART_INTERFACE(I_LinkOnValues);


/**********************************************************************************************/
// Interface of LinkOnValues link (aka RDB link, aka ForeignKey link).
//
//StubInterface
interface FBL_SHARED_EXP_CLASS I_LinkOnValues : public I_Unknown
{

virtual						~I_LinkOnValues( void );


	//----------------------
	// PK-FK Methods:

							/// Returns the count of the fields paticipated in the key.
virtual vuint16 			get_FieldCount( void ) const = 0;

							/// Returns the field (key) from the PrimaryKey by index.
virtual I_Field_Ptr			get_PrimaryKeyField( vuint16 inIndex ) const = 0;

							/// Returns the field (ptr) from the ForeignKey by index.
virtual I_Field_Ptr			get_ForeignKeyField( vuint16 inIndex ) const = 0;

}; 
 

/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_I_LinkOnValues_h
