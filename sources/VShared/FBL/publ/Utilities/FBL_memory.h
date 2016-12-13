/**********************************************************************************************/
/* FBL_memory.h																				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_memory_h
	#define _FBL_memory_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// The same as memcpy() but accepts UChar*
//
FBL_SHARED_EXP UChar* vu_memcpy( UChar* inDest, const UChar* inSrc, tslen inSize );


/**********************************************************************************************/
// The same as memset() but accepts UChar*
//
FBL_SHARED_EXP UChar* vu_memset( UChar* inDest, UChar c, tslen count );


/**********************************************************************************************/
// Returns the size of installed PHYSICAL RAM on this computer.
//
FBL_SHARED_EXP vuint64 getTotalSystemMemory( void );
FBL_SHARED_EXP vuint64 getMemoryUsedByThisApp( void );


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_memory_h
