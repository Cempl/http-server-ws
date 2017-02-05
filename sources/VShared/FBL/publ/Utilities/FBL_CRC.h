/**********************************************************************************************/
/* FBL_CRC.h																				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_CRC_h
	#define _FBL_CRC_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace

/**********************************************************************************************/
// use ANSI C bit operators
#if FBL_LITTLE_ENDIAN	
	#define HTON32(v)    ( ((v&0xff000000)>>24) | ((v&0xff0000)>>8) | ((v&0xff00)<<8) | ((v&0xff)<<24) )
#else
	#define HTON32(v)    (v)
#endif // FBL_LITTLE_ENDIAN


/**********************************************************************************************/
// CRC-32-IEEE 802.3  (V.42, MPEG-2, PNG [15], POSIX cksum)
#define CRC32_QUOTIENT 0x04c11db7


/**********************************************************************************************/
// This code is copyright � 1993 Richard Black. All rights are reserved.
// You may use this code only if it includes a statement to that effect.
//
struct CRC32_Table
{
	public://///////////////////////////////////////////////////////////////////////////////////

						CRC32_Table() 
						{
							vuint32 crc;

							unsigned i, j;
							for( i = 0; i < 256; i++ )
							{
								crc = i << 24;
								for( j = 0; j < 8; j++ )
								{
									if( crc & 0x80000000 )
										crc = (crc << 1) ^ CRC32_QUOTIENT;
									else
										crc = crc << 1;
								}

								mpCRC_Table[i] = HTON32(crc);
							}
						}


	public://///////////////////////////////////////////////////////////////////////////////////

		vuint32			mpCRC_Table[256];
};


/**********************************************************************************************/
// Helpers
//
vuint32 crc32B( 
	const vuint8*	inpData, 
	unsigned		inLen, 
	vuint32			inCRC );

/**********************************************************************************************/	
vuint32 crc32DW( 
	const vuint8*	inpData, 
	unsigned		inLen, 
	vuint32			inCRC );


/**********************************************************************************************/
// Calculate CRC32.
FBL_SHARED_EXP vuint32 CRC32( 
	const vuint8*	inpData, 
	unsigned		inLen );


/**********************************************************************************************/
FBL_End_Namespace

#endif // _FBL_CRC_h
