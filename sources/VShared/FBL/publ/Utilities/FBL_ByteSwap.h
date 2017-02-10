/**********************************************************************************************/
/* FBL_ByteSwap.h																		 	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_ByteSwap_h 
	#define _FBL_ByteSwap_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>
	
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
inline bool IsBigEndianNow( void )
{
#if FBL_BIG_ENDIAN
	return true;
#else
	return false;
#endif //FBL_BIG_ENDIAN
}


/**********************************************************************************************/
// Swap in place:
//
inline void Swap_2_bytes( vint16* inValue )
{
	char* p = (char*) inValue;

	char p0 = p[0];

	p[0]	 = p[1];
	p[1]	 = p0;
}

/**********************************************************************************************/
// Swap by copy:
//
inline void Swap_2_bytes( const vint16* inValue, vint16* outValue )
{
	FBL_CHECK( inValue != nullptr ); 

	char* p = (char*) inValue;
	char* d = (char*) outValue;

	d[1]	= p[0];
	d[0]	= p[1];
}


/**********************************************************************************************/
// Swap in place:
//
inline void Swap_4_bytes( vint32* inValue )
{
	FBL_CHECK( inValue != nullptr ); 

	char* p = (char*) inValue;

	// swap 0 and 3 bytes:
	char p0 = p[0];
	char p1 = p[1];
	
	p[0] = p[3];
	p[1] = p[2];
	p[2] = p1;
	p[3] = p0;

}

/**********************************************************************************************/
// Swap by copy:
//
inline void Swap_4_bytes( const vint32* inValue, vint32* outValue )
{
	FBL_CHECK( inValue != nullptr ); 

	char* p = (char*) inValue;
	char* d = (char*) outValue;

	d[0] = p[3];
	d[1] = p[2];
	d[2] = p[1];
	d[3] = p[0];
}


/**********************************************************************************************/
// Swap in place:
//
inline void Swap_8_bytes( vuint64* inValue )
{
	FBL_CHECK( inValue != nullptr ); 

	char* p = (char*) inValue;

	// swap 0 and 3 bytes:
	char p0 = p[0];
	char p1 = p[1];
	char p2 = p[2];
	char p3 = p[3];

	p[0] = p[7];
	p[1] = p[6];
	p[2] = p[5];
	p[3] = p[4];
	p[4] = p3;
	p[5] = p2;
	p[6] = p1;
	p[7] = p0;
}


/**********************************************************************************************/
// Swap in place:
//
inline void Swap_8_bytes( double* inValue )
{
	Swap_8_bytes( (vuint64*) inValue );
}

/**********************************************************************************************/
// Swap by copy:
//
inline void Swap_8_bytes( const vuint64* inValue, vuint64* outValue )
{
	FBL_CHECK( inValue != nullptr ); 

	char* p = (char*) inValue;
	char* d = (char*) outValue;

	d[0] = p[7];
	d[1] = p[6];
	d[2] = p[5];
	d[3] = p[4];
	d[4] = p[3];
	d[5] = p[2];
	d[6] = p[1];
	d[7] = p[0];
}


/**********************************************************************************************/
// Swap by copy:
//
inline void Swap_8_bytes( const double* inValue, double* outValue )
{
	Swap_8_bytes( (const vuint64*) inValue, (vuint64*) outValue );
}


/**********************************************************************************************/
// Swap in place:
//
/*inline void Swap_10_bytes( ldouble* inValue )
{
	FBL_CHECK( inValue != NULL ); 

	char* p = (char*) inValue;

	// swap 0 and 3 bytes:
	char p0 = p[0];
	char p1 = p[1];
	char p2 = p[2];
	char p3 = p[3];
	char p4 = p[4];
	
	p[0] = p[9];
	p[1] = p[8];
	p[2] = p[7];
	p[3] = p[6];
	p[4] = p[5];
	
	p[5] = p4;
	p[6] = p3;
	p[7] = p2;
	p[8] = p1;
	p[9] = p0;
}*/

/**********************************************************************************************/
// Swap by copy:
//
/*inline void Swap_10_bytes( const ldouble* inValue, ldouble* outValue )
{
	FBL_CHECK( inValue != NULL ); 

	char* p = (char*) inValue;
	char* d = (char*) outValue;

	d[0] = p[9];
	d[1] = p[8];
	d[2] = p[7];
	d[3] = p[6];
	d[4] = p[5];
	d[5] = p[4];
	d[6] = p[3];
	d[7] = p[2];
	d[8] = p[1];
	d[9] = p[0];
}*/


#pragma mark -


/**********************************************************************************************/
// Swap by copy:
// in fact for Medium we return 3 bytes in Windows byte order.
//
inline void* Long_Medium( const vint32* inValue, void* outValue )
{
	char* p = (char*) inValue;
	char* d = (char*) outValue;

#if FBL_BIG_ENDIAN

	d[0] = p[3];
	d[1] = p[2];
	d[2] = p[1];

#else // FBL_BIG_ENDIAN

	d[0] = p[0];
	d[1] = p[1];
	d[2] = p[2];

#endif //FBL_BIG_ENDIAN

	return outValue;
}


/**********************************************************************************************/
// Swap by copy:
// in fact for Medium we return 3 bytes in Windows byte order.
//
inline vint32* Medium_Long( const void* inValue, vint32* outValue )
{
	char* p = (char*) inValue;
	char* d = (char*) outValue;

#if FBL_BIG_ENDIAN

	if( p[2] >= 0x00 )
			d[0] = (char) 0x00;	// positive
	else	d[0] = (char) 0xFF;	// negative

	d[1] = p[2];
	d[2] = p[1];
	d[3] = p[0];

#else // FBL_BIG_ENDIAN

	d[0] = p[0];
	d[1] = p[1];
	d[2] = p[2];

	if( p[2] >= 0x00 )
			d[3] = (char) 0x00u;	// positive
	else	d[3] = (char) 0xFFu;	// negative

#endif //FBL_BIG_ENDIAN

	return outValue;
}


/**********************************************************************************************/
// Swap by copy:
// in fact for Medium we return 3 bytes in Windows byte order.
//
inline vint32* UMedium_Long( const void* inValue, vint32* outValue )
{
	char* p = (char*) inValue;
	char* d = (char*) outValue;

#if FBL_BIG_ENDIAN

	d[0] = 0;
	d[1] = p[2];
	d[2] = p[1];
	d[3] = p[0];

#else // FBL_BIG_ENDIAN

	d[0] = p[0];
	d[1] = p[1];
	d[2] = p[2];
	d[3] = 0;

#endif //FBL_BIG_ENDIAN

	return outValue;
}


#pragma mark -


/**********************************************************************************************/
// Swap of array of Longs in place:
//
inline void Swap_2_bytesArray( vint16* inArray, vuint32 inCount )
{
	FBL_CHECK( inArray != nullptr ); 

	for( vuint32 i = 0; i < inCount; ++i )
		Swap_2_bytes( inArray++ );
}


/**********************************************************************************************/
// Swap of array of Longs by copy:
//
inline void Swap_2_bytesArray( const vint16* inArray, vint16* inDest, vuint32 inCount )
{
	FBL_CHECK( inArray != nullptr ); 

	for( vuint32 i = 0; i < inCount; ++i )
		Swap_2_bytes( inArray++, inDest++ );
}

	
/**********************************************************************************************/
// Swap of array of Longs in place:
//
inline void Swap_4_bytesArray( vint32* inArray, vuint32 inCount )
{
	FBL_CHECK( inArray != nullptr ); 

	for( vuint32 i = 0; i < inCount; ++i )
		Swap_4_bytes( inArray++ );
}


/**********************************************************************************************/
// Swap of array of Longs by copy:
//
inline void Swap_4_bytesArray( const vint32* inArray, vint32* inDest, vuint32 inCount )
{
	FBL_CHECK( inArray != nullptr ); 

	for( vuint32 i = 0; i < inCount; ++i )
		Swap_4_bytes( inArray++, inDest++ );
}


#pragma mark -

	
/**********************************************************************************************/
template <class T>
class policy_NoSwap
{
	public://///////////////////////////////////////////////////////////////////////////////////
 		
		void				Swap( void*	inValue ) { 
								argused1(inValue);  
							}
		
		void				Swap( void*	inValue, void* outValue ) { 
								argused2(inValue, outValue);  
							}
};


/**********************************************************************************************/
template <class T>
class policy_Swap
{
	public://///////////////////////////////////////////////////////////////////////////////////
 		
		void				Swap( void*	inValue ) 
							{
								switch( sizeof(T) )
								{
									case 2:	Swap_2_bytes( (vint16*) inValue ); 	break;
									case 4: Swap_4_bytes( (vint32*) inValue );	break;
								    case 8:	Swap_8_bytes( (double*) inValue );	break;
								}
							}
		
		void				Swap( void*	inValue, void* outValue )		
							{
								switch( sizeof(T) )
								{
									case 2:	Swap_2_bytes((vint16*) inValue, (vint16*) outValue); break;
									case 4: Swap_4_bytes((vint32*) inValue, (vint32*) outValue); break;
								    case 8:	Swap_8_bytes((double*) inValue, (double*) outValue); break;
								}
							}
};


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_ByteSwap_h

