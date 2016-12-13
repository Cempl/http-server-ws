/**********************************************************************************************/
/* FBL_UnicodeToFromNumeric.h																  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_UnicodeToFromNumeric_h
	#define _FBL_UnicodeToFromNumeric_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
/// unicode string -> vint32;
/// @inStr - buffer containing string to be converted;
/// @return - resulting vint32 value;
///
FBL_SHARED_EXP vint32 u_utol( const UChar* inStr );


/**********************************************************************************************/
/// vint32	-> unicode string;
/// assumes that outBuf is at least 20 chars vint32;
/// @inNum - vint32 to be converted to the string;
/// @outBuf - buffer containing result string;
/// @return - pointer to the end zero after number;
///
FBL_SHARED_EXP UChar* u_ltou( vint32 inNum, UChar* outBuf );


/**********************************************************************************************/
/// unicode string -> vuint32;
/// @inStr - buffer containing string to be converted;
/// @return - resulting vuint32 value;
///
FBL_SHARED_EXP vuint32 u_ustoul( const UChar* inStr );


/**********************************************************************************************/
/// vuint32 -> unicode string;
/// assumes that outBuf is at least 20 chars vint32;
/// @inNum - vuint32 to be converted to the string;
/// @outBuf - buffer containing result string;
/// @return - pointer to the outBuf;
///
FBL_SHARED_EXP UChar* u_ultous( vuint32 inNum, UChar* outBuf );


/**********************************************************************************************/
/// unicode string -> double;
/// assumes that outBuf at least 32 chars vint32;
/// @inStr - buffer containing string to be converted;
/// @return - resulting double value;
///
FBL_SHARED_EXP double	u_utof( const UChar* inStr );


/**********************************************************************************************/
FBL_SHARED_EXP UChar* u_ftos( double inNum, UChar* outBuff );


/**********************************************************************************************/
/// unicode string -> vint64
/// @inStr - string to be converted to vint64;
/// @return - resulting vint64 value;
///
FBL_SHARED_EXP vint64	u_utoll( const UChar* inStr );


/**********************************************************************************************/
/// vint64	-> unicode string
///
FBL_SHARED_EXP UChar*	u_lltou( vint64 inNum, UChar* outBuf );


/**********************************************************************************************/
/// unicode string -> vuint64
///
FBL_SHARED_EXP vuint64	u_ustoull( const UChar* inStr );


/**********************************************************************************************/
/// vuint64 -> unicode string
///
FBL_SHARED_EXP UChar*	u_ulltous( vuint64 inNum, UChar* outBuf );


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_UnicodeToFromNumeric_h
