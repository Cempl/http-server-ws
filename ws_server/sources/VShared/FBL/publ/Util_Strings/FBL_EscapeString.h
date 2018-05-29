/**********************************************************************************************/
/* FBL_EscapeString.h																		  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_EscapeString_h
	#define _FBL_EscapeString_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
FBL_SHARED_EXP void AddXmlEncoding_( UChar* inpStart, UChar* inpEnd, String& outValue );
FBL_SHARED_EXP void RemoveXmlEncoding_( UChar* inpStart, UChar* inpEnd, String& outValue );
FBL_SHARED_EXP void AddXmlEncoding_( char* inpStart, char* inpEnd, String& outValue );
FBL_SHARED_EXP void RemoveXmlEncoding_( char* inpStart, char* inpEnd, String& outValue );


/**********************************************************************************************/
// RZ 3/3/10 added this new form of escape function. Exactly as in mySQL.
//
// TODO: This form is more effective than below, which self allocated and return pointer.
// So I think, we should deprecate them.
//
/**********************************************************************************************/
/// Prepends a back slash \ character in the input string with \ (backslash); 
///
/// These functions put result escaped character into given inBuffer.
/// SIZE of this buffer should be (inLen * 2) .
///
/// RETURNS the length of escaped string produced in the inBuffer.
/// 
FBL_SHARED_EXP vuint32 EscapeStringU( UChar* inBuffer, const UChar* inStr, vuint32 inLen );
FBL_SHARED_EXP vuint32 EscapeStringA( char*  inBuffer, const char*  inStr, vuint32 inLen );


#pragma mark -


/**********************************************************************************************/
/// Note that this function dynamically allocates memory for the resulting string, 
/// so the returned UChar* pointer must be freed (with FreeString()) to avoid memory leak.
/// 
/// inLen parameter can be ZERO, then function self will calculate strlen() of inStr.
///
FBL_SHARED_EXP UChar* EscapeStringU( const UChar* inStr, vuint32 inLen = 0 );
FBL_SHARED_EXP char*  EscapeStringA( const char*  inStr, vuint32 inLen = 0 );


/**********************************************************************************************/
// Deallocates dynamically allocated by EscapeString() memory.
//
FBL_SHARED_EXP void FreeStringU( const UChar* inStr );
FBL_SHARED_EXP void FreeStringA( const char*  inStr );


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_EscapeString_h
