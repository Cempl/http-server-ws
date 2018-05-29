/**********************************************************************************************/
/* FBL_StrToBin.h                                                        					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_StrToBin_h 
	#define _FBL_StrToBin_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
class String;

/**********************************************************************************************/
FBL_SHARED_EXP String DataToBase64(
	const char*		inData,
	vuint32			inSize,
	bool			inDoWrap = true );


/**********************************************************************************************/
FBL_SHARED_EXP String DataFromBase64(
	const String&	inData );


/**********************************************************************************************/
FBL_SHARED_EXP String DataToBinString(	
	vuint8* 			inData, 
	vuint32 			inBytes );

FBL_SHARED_EXP void DataToBinString( 
	vuint8* 	inData, 
	UChar*		outStr,
	vuint32& 	ioLen );

/**********************************************************************************************/
// Returns new allocated buffer that a caller must destroy self using delete[].
//
FBL_SHARED_EXP vuint8* DataFromBinString( 
	const UChar*		inStr, 
	vuint32& 			ioLen );

FBL_SHARED_EXP void DataFromBinString( 
	const UChar*		inStr, 
	vuint8*				outStr,
	vuint32& 			ioLen );

FBL_SHARED_EXP vuint8* DataFromBinString( 
	const char*			inStr, 
	vuint32& 			ioLen );

 
/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_StrToBin_h

