/**********************************************************************************************/
/* FBL_MD5.h																				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

/* MD5.H - header file for MD5C.C
 */

/* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.
 */



#ifndef _FBL_MD5_h
	#define _FBL_MD5_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Util_classes/FBL_String.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
#ifndef PROTOTYPES
#define PROTOTYPES 1				/* Assume prototypes */
#endif

#define DIGEST_SIZE		32

/**********************************************************************************************/
// POINTER defines a generic pointer type
typedef vuint8* POINTER;
// MD5 context
typedef struct 
{
	vuint32 state[4];		// state (ABCD)
	vuint32 count[2];		// number of bits, modulo 2^64 (lsb first)
	vuint8 buffer[64];	// input buffer

} VALENTINA_MD5;


/**********************************************************************************************/
// PROTO_LIST is defined depending on how PROTOTYPES is defined above.
//If using PROTOTYPES, then PROTO_LIST returns the list, otherwise it
//  returns an empty list.
#if PROTOTYPES
#define PROTO_LIST(list) list
#else
#define PROTO_LIST(list) ()
#endif


/**********************************************************************************************/
// MD5 functions
//
#ifdef __cplusplus
extern "C" {
#endif

		void				MD5Init		PROTO_LIST	( (VALENTINA_MD5 *) );

		void				MD5Update	PROTO_LIST	( (VALENTINA_MD5 *, vuint8*, vuint32) );

		void				MD5Final	PROTO_LIST	( (vuint8 [16], VALENTINA_MD5 *) );

#ifdef __cplusplus
}
#endif


/**********************************************************************************************/
FBL_SHARED_EXP void GetMD5( const String& inString, vuint8* outDigest );


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_MD5_h
