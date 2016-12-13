/**********************************************************************************************/
/* FBL_ASCIIToFromNumeric.h																	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_ASCIIToFromNumeric_h
	#define _FBL_ASCIIToFromNumeric_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
/// ascii string -> vint32;
/// @inStr - buffer containing string to be converted;
/// @return - resulting vint32 value;
///
FBL_SHARED_EXP vint32	va_atol( const char* inStr );


/**********************************************************************************************/
/// vint32	-> ascii string;
/// @inNum - vint32 to be converted to the string;
/// @outBuf - buffer containing result string;
/// @return - pointer to the end zero after number;
///
FBL_SHARED_EXP char*	va_ltoa( vint32 inNum, char* outBuf );


/**********************************************************************************************/
/// ascii string -> vuint32;
/// @inStr - buffer containing string to be converted;
/// @return - resulting vuint32 value;
///
FBL_SHARED_EXP vuint32	va_atoul( const char* inStr );


/**********************************************************************************************/
/// vuint32 -> ascii string;
/// assumes that outBuf is at least 20 chars vint32;
/// @inNum - vuint32 to be converted to the string;
/// @outBuf - buffer containing result string;
/// @return - pointer to the outBuf;
///
FBL_SHARED_EXP char*	va_ultoa( vuint32 inNum, char* outBuf );


/**********************************************************************************************/
/// ascii string -> double;
/// assumes that outBuf at least 32 chars vint32;
/// @inStr - buffer containing string to be converted;
/// @return - resulting double value;
///
FBL_SHARED_EXP double	va_atof( const char* inStr );


/**********************************************************************************************/
FBL_SHARED_EXP char*	va_ftoa( double inNum, char* outBuff );


/**********************************************************************************************/
/// ascii string -> vint64
/// @inStr - string to be converted to vint64;
/// @return - resulting vint64 value;
///
FBL_SHARED_EXP vint64	va_atoll( const char* inStr );


/**********************************************************************************************/
/// vint64	-> ascii string
///
FBL_SHARED_EXP char*	va_lltoa( vint64 inNum, char* outBuf );


/**********************************************************************************************/
/// ascii string -> vuint64
///
FBL_SHARED_EXP vuint64	va_atoull( const char* inStr );


/**********************************************************************************************/
/// vuint64 -> ascii string
///
FBL_SHARED_EXP char*	va_ulltoa( vuint64 inNum, char* outBuf );


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_ASCIIToFromNumeric_h
