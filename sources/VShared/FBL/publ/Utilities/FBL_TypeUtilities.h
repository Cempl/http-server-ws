/**********************************************************************************************/
/* FBL_TypeUtilities.h																	  	  */
/*                                                                       					  */
/* Copyright Paradigma 1993-2006															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_TypeUtilities_h 
	#define _FBL_TypeUtilities_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Type_Enumerated.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
FBL_SHARED_EXP const String& GetEnumValueAsString( 
	const String&			inValue,
	I_Type_Enumerated_Ptr	inpEnumType );


/**********************************************************************************************/
FBL_SHARED_EXP vuint16 GetEnumValueAsInt( 
	const String&			inValue,
	I_Type_Enumerated_Ptr	inpEnumType );


/**********************************************************************************************/
FBL_SHARED_EXP vuint16 GetLocaleEnumValueAsInt(
	const String&			inLocale,
	const String&			inValue,
	I_Type_Enumerated_Ptr	inpEnumType );


/**********************************************************************************************/
FBL_SHARED_EXP void CheckValue( 
	vuint16					inValue,
	I_Type_Enumerated_Ptr	inpEnumType );


/**********************************************************************************************/
FBL_SHARED_EXP const String& ConvertEnumValueTo(
	const String&			inLocale,
	const String&			inValue,
	I_Type_Enumerated_Ptr	inpEnumType );


/**********************************************************************************************/
FBL_SHARED_EXP const String& ConvertEnumValueFrom(
	const String&			inLocale,
	const String&			inValue,
	I_Type_Enumerated_Ptr	inpEnumType );


/**********************************************************************************************/
FBL_SHARED_EXP const String& GetSqliteAffinityTypeByName( const char* inType );


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_TypeUtilities_h


