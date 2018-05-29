/**********************************************************************************************/
/* FBL_Algs_VarInt_Positive.h																  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/
// The variable-length integer encoding is as follows:
//
// KEY:
//         A = 0xxxxxxx    7 bits of data and one flag bit
//         B = 1xxxxxxx    7 bits of data and one flag bit
//         C = xxxxxxxx    8 bits of data
//
//  7 bits - A
// 14 bits - BA
// 21 bits - BBA
// 28 bits - BBBA
// 35 bits - BBBBA
// 42 bits - BBBBBA
// 49 bits - BBBBBBA
// 56 bits - BBBBBBBA
// 64 bits - BBBBBBBBC
/**********************************************************************************************/


#include <VShared/FBL/publ/Headers/FBL.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// Forward declaration.
// DO NOT call this method directly! Use GetVarInt32() macro, see below.
//
FBL_SHARED_EXP vuint8 		GetVarInt32_( const unsigned char*, vuint32* );


#pragma mark -


/**********************************************************************************************/
// Routines to read and write variable-length integers.
//
						// Calculates how many compressed bytes will be required for v.
FBL_SHARED_EXP int 		VarIntLen64( vuint64 v );

						// Compress the given vuint64 number into the given buffer.
						// Returns how many compressed bytes was writen into buffer.
FBL_SHARED_EXP int 		PutVarInt64( unsigned char*, vuint64 );

						// Uncompress a vuint64 number from the given buffer.
						// Returns how many compressed bytes was read.
FBL_SHARED_EXP vuint8 	GetVarInt64( const unsigned char*, vuint64* );


/**********************************************************************************************/
// The common case is for a VarInt to be a single byte.
// The following macros handles the common case without a procedure call,
// but then call the procedure for larger varints.
//
inline int PutVarInt32( unsigned char* A, vuint32 B )
{
  return (vuint8) (((vuint32)(B) < (vuint32)0x80)
  			? (*(A) = (unsigned char)(B)), 1 \
  			: PutVarInt64(A, B) );
}

inline int GetVarInt32( const unsigned char* A, vuint32* B )
{
	return (vuint8) ((*(A) < (vuint8)0x80)
    		? ((*B) = (vuint32)*(A)), 1
            : GetVarInt32_(A, B) );
}


/**********************************************************************************************/
FBL_End_Namespace
