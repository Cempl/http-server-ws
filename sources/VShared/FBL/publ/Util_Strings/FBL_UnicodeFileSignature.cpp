/**********************************************************************************************/
/* FBL_UnicodeFileSignature.cpp                                         					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Util_Strings/FBL_UnicodeFileSignature.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
const vuint8* Get_FileSignature_UTF_7( vuint32* outLength ) 
{
static vuint8 Sign[] = { 0x38, 0x39, 0x2B, 0x2F };

	*outLength = sizeof(Sign);

 	return Sign;
}


/**********************************************************************************************/
const vuint8* Get_FileSignature_UTF_8( vuint32* outLength ) 
{
static vuint8 Sign[] = { 0xEF, 0xBB, 0xBF };	// UTF-8

	*outLength = sizeof(Sign);

 	return Sign;
}


/**********************************************************************************************/
const vuint8* Get_FileSignature_UTF_16( vuint32* outLength ) 
{
#if FBL_LITTLE_ENDIAN

static vuint8 Sign[] = { 0xFF, 0xFE };	// UTF-16LE

#else // BIG ENDIAN

static vuint8 Sign[] = { 0xFE, 0xFF };	// UTF-16BE

#endif 

	*outLength = sizeof(Sign);

 	return Sign;
}


/**********************************************************************************************/
const vuint8* Get_FileSignature_UTF_32( vuint32* outLength ) 
{
#if FBL_LITTLE_ENDIAN

static vuint8 Sign[] = { 0xFF, 0xFE, 0x00, 0x00 };	// UTF-32LE

#else // BIG ENDIAN
	
static vuint8 Sign[] = { 0x00, 0x00, 0xFE, 0xFF };	// UTF-32BE

#endif // FBL_LITTLE_ENDIAN

	*outLength = sizeof(Sign);

 	return Sign;
}


#pragma mark -


/**********************************************************************************************/
const vuint8* Get_FileSignature_UTF_SCCU( vuint32* outLength ) 
{
static vuint8 Sign[] = { 0x0E, 0xFE, 0xFF };

	*outLength = sizeof(Sign);

 	return Sign;
}


/**********************************************************************************************/
const vuint8* Get_FileSignature_UTF_BOCU_1( vuint32* outLength ) 
{
static vuint8 Sign[] = { 0xFB, 0xEE, 0x28 };

	*outLength = sizeof(Sign);

 	return Sign;
}


/**********************************************************************************************/
const vuint8* Get_FileSignature_UTF_EBCDIC( vuint32* outLength ) 
{
static vuint8 Sign[] = { 0xDD, 0x73, 0x66, 0x73 };

	*outLength = sizeof(Sign);

 	return Sign;
}


/**********************************************************************************************/
FBL_End_Namespace
