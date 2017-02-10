/**********************************************************************************************/
/* FBL_ParseEnumType.h																		  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_ParseEnumType_h
	#define _FBL_ParseEnumType_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Util_classes/FBL_String.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
/*
	These two methods intended to be used on the result of
	 VSQL command "SHOW TYPES" to parse ENUM type string, which is in format
		('v1','v2','v3'), 'lang1':('lang1_v1','lang1_v2','lang1_v3'), 'lang2':('lang2_v1','lang2_v2','lang2_v3')

	These methods are good helpers for developers who want to display strings of ENUM
	in the correct locale of user in the GUI of their app.

	@EnumType_GetLocales() returns array of locales.
	For the above example this is { "en", "lang1", "lang2" }

	@EnumType_GetLocaleItems() can be used to extract items of the specific locale.
	For the above exable, for locale 'lang1', result will be { "lang1_v1", "lang1_v2", "lang1_v3" }

	Both methods put into array strings without single quotes.

	These methods are capable to work in LOCAL and CLIENT modes.
*/

FBL_SHARED_EXP ArrayOfStringsObj_Ptr EnumType_GetLocales( const String& inStr );

FBL_SHARED_EXP ArrayOfStringsObj_Ptr EnumType_GetLocaleItems( const String& inStr, const String& inLocaleName );


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_ParseEnumType_h
