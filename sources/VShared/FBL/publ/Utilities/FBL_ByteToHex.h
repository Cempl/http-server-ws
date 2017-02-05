/**********************************************************************************************/
/* FBL_ByteToHex.h                                                        					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_ByteToHex_h 
	#define _FBL_ByteToHex_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
class String;


/**********************************************************************************************/
FBL_SHARED_EXP extern const char* sHex[];


/**********************************************************************************************/
FBL_SHARED_EXP String DataToHexString(	
	vuint8* 			inData, 
	vuint32 			inBytes, 
	bool				inHexWithData = false, 
	vuint32 			inDepth = 0 );


/**********************************************************************************************/
// Returns new allocated buffer that a caller must destroy self using delete[].
//
FBL_SHARED_EXP vuint8* DataFromHexString( 
	const UChar*		inStr, 
	vuint32& 			ioLen );

FBL_SHARED_EXP vuint8* DataFromHexString( 
	const char*			inStr, 
	vuint32& 			ioLen );

 
#pragma mark -

 
/**********************************************************************************************/
inline vuint8 HexToByte( UChar ch )
{
	// PRE: ch is '0'..9 or 'A'..'F'

	vuint8 res;

	if( ch <= UChar('9') )
	 		res = vuint8(ch - UChar('0'));
	else	res = vuint8(ch - UChar('A') + 10);
	
	return res; 
}


/**********************************************************************************************/
inline vuint8 HexToByte( char ch )
{
	// PRE: ch is '0'..9 or 'A'..'F'

	vuint8 res;

	if( ch <= '9' )
	 		res = vuint8(ch - '0');
	else	res = vuint8(ch - 'A' + 10);
	
	return res; 
}


#pragma mark -

 
/**********************************************************************************************/
inline const char* ByteToHex( vuint8 inNumber )
{	
	return sHex[inNumber];
}


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_ByteToHex_h

