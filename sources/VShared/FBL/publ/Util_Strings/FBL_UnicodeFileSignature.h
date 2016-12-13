/**********************************************************************************************/
/* FBL_UnicodeFileSignature.h																  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_UnicodeFileSignature_h
	#define _FBL_UnicodeFileSignature_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>


// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
FBL_SHARED_EXP const vuint8* Get_FileSignature_UTF_7( vuint32* outLength );
FBL_SHARED_EXP const vuint8* Get_FileSignature_UTF_8( vuint32* outLength );
FBL_SHARED_EXP const vuint8* Get_FileSignature_UTF_16( vuint32* outLength );
FBL_SHARED_EXP const vuint8* Get_FileSignature_UTF_32( vuint32* outLength );

FBL_SHARED_EXP const vuint8* Get_FileSignature_UTF_SCCU( vuint32* outLength );
FBL_SHARED_EXP const vuint8* Get_FileSignature_UTF_BOCU_1( vuint32* outLength );
FBL_SHARED_EXP const vuint8* Get_FileSignature_UTF_EBCDIC( vuint32* outLength );


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_UnicodeFileSignature_h
