/**********************************************************************************************/
/* FBL_I_StringFactory.h 		                                                      		  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_StringFactory_h 
	#define _FBL_I_StringFactory_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE(I_String);

interface I_Converter;


/**********************************************************************************************/
/**	This interface provides an abstraction of a factory 
 	for creating strings.
*/
interface FBL_SHARED_EXP_CLASS I_StringFactory : public I_Unknown
{

virtual 					~I_StringFactory( void );

	// ---------------------
	// Factory methods:

virtual I_String* 			CreateString( const UChar* inStr ) = 0;

virtual I_String* 			CreateString( 
								const UChar* inStart, 
								const UChar* inEnd ) = 0;

virtual I_String* 			CreateString( 
								const UChar* inStr, 
								tslen inLength ) = 0;

virtual I_String* 			CreateString( 
								const char* inStr, 
								tslen inLen, 
								const char* inEncoding ) = 0;

virtual I_String* 			CreateString( 
								const char* inStr, 
								tslen inLen, 
								I_Converter* inConverter ) = 0;

virtual I_String* 			CreateString( 
								tslen inCapacity, 
								UChar c, 
								tslen inCount ) = 0;

virtual I_String* 			CreateString( 
								tslen inCapacity, 
								char c, 
								tslen inCount ) = 0;
}; 


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_I_StringFactory_h
