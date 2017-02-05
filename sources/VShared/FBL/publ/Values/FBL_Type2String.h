/**********************************************************************************************/
/* FBL_Type2String.h																		  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_Type2String_h
	#define _FBL_Type2String_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// Used to map type codes to its string representation.
//
FBL_SHARED_EXP String TypeCode2String( VALUE_TYPE inType, const char inLocale[] = "en_US" );

// Used to map type's string representation to its type code.
//
FBL_SHARED_EXP VALUE_TYPE String2TypeCode( const UChar* inTypeStr, const char inLocale[] = "en_US" );


/**********************************************************************************************/
FBL_SHARED_EXP const char* GetInternalTypeCodeString( FIELD_TYPE inType );


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_Type2String_h
