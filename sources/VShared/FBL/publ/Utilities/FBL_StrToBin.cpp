/**********************************************************************************************/
/* FBL_StrToBin.cpp                                                          				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include "FBL_StrToBin.h"

// VSAHRED:
#include <VShared/FBL/publ/Util_classes/FBL_String.h>
#include <VShared/FBL/publ/Util_Strings/FBL_ustdio.h>

// ANCI-C
#include <ctype.h>

// STD
#include <cstring>
#include <string>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
static const char base64chars[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


/**********************************************************************************************/
// aaaack but it's fast and const should make it shared text page.
//
/*
static const unsigned char pr2six[256] =
{
	// ASCII table.
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
	64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
	64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
};
*/


/**********************************************************************************************/
/*
static int base64_decode_len( const char *inpEncoded )
{
	int res = 0;

	const unsigned char* pInData = (const unsigned char*)inpEncoded;

	while( pr2six[ *(pInData++) ] <= 63 );

	size_t nprbytes = ( pInData - (const unsigned char*)inpEncoded ) - 1;
	res = ( ((int)nprbytes + 3) / 4 ) * 3;

	return res + 1;
}
*/


/**********************************************************************************************/
/*
static int base64_decode_binary( unsigned char* outpBufDecoded, const char* inpBufEncoded )
{
	const unsigned char* pInData = (const unsigned char*) inpBufEncoded;

	while( pr2six[*(pInData++)] <= 63 );
	
	size_t nprbytes = ( pInData - (const unsigned char*)inpBufEncoded ) - 1;
	int decodedLength = ( ((int)nprbytes + 3) / 4 ) * 3;

	pInData = (const unsigned char*) inpBufEncoded;

	while( nprbytes > 4 )
	{
		*(outpBufDecoded++) = (unsigned char)(pr2six[*pInData] << 2 | pr2six[pInData[1]] >> 4);
		*(outpBufDecoded++) = (unsigned char)(pr2six[pInData[1]] << 4 | pr2six[pInData[2]] >> 2);
		*(outpBufDecoded++) = (unsigned char)(pr2six[pInData[2]] << 6 | pr2six[pInData[3]]);
		pInData += 4;
		nprbytes -= 4;
	}

	// Note: (nprbytes == 1) would be an error, so just ingore that case.
	if( nprbytes > 1 )
		*(outpBufDecoded++) = (unsigned char)(pr2six[*pInData] << 2 | pr2six[pInData[1]] >> 4);
	if( nprbytes > 2 )
		*(outpBufDecoded++) = (unsigned char)(pr2six[pInData[1]] << 4 | pr2six[pInData[2]] >> 2);
	if( nprbytes > 3 )
		*(outpBufDecoded++) = (unsigned char)(pr2six[pInData[2]] << 6 | pr2six[pInData[3]]);

	decodedLength -= (4 - (int)nprbytes) & 3;
	return decodedLength;
}
*/


/**********************************************************************************************/
String DataFromBase64( const String& inData )
{
	String fblResult;
	fixme_argused1( inData );
	
//	const char* pEncodedData = inData.getBufferA();
//	int decodedLength = base64_decode_len( pEncodedData );
//	if( decodedLength > 0 )
//	{
//		char* pDecodedBuf = fblResult.getBufferA( decodedLength );
//		int actualDecodedLength = base64_decode_binary( (unsigned char*)pDecodedBuf, pEncodedData );
//		// actualDecodedLength <= decodedLength
//	}
	
	return fblResult;
}

///**********************************************************************************************/
//// inData	= base64 encoded data
//// ioLen	= base64 encoded data length
//// res		= base64 decoded data
//// outLen	= base64 decoded data length
////
//vuint8* DataFromBase64( 
//	const char*			inpData,
//	vuint32& 			outLen )
//{
//	vuint8* pDecodedBuf = NULL;
//	
//	if( inpData )
//	{
//		int decodedMaxLength = base64_decode_len( inpData );
//		if( decodedMaxLength > 0 )
//		{
//			pDecodedBuf = new vuint8[ decodedMaxLength ];
//			int actualDecodedLength = base64_decode_binary( (unsigned char*)pDecodedBuf, inpData );
//		}
//	}
//	
//	return pDecodedBuf;
//}


/**********************************************************************************************/
String DataToBase64(
	const char*	inData,
	vuint32		inSize,
	bool		inDoWrap )
{
	String res;
	
	if( inSize && inData )
	{
		long i = 0;
		for( ; i < (long) inSize - 2 ; i += 3 )
		{
			if( inDoWrap && !( i % 54 ) )
				res += '\n';

			res += base64chars[ vuint8( (   inData[ i ] >> 2 ) & 0x3F ) ];
			res += base64chars[ vuint8( ( ( inData[ i ] & 0x3 ) << 4 ) | ( (int) ( inData[ i + 1 ] & 0xF0 ) >> 4 ) ) ];
			res += base64chars[ vuint8( ( ( inData[ i + 1 ] & 0xF ) << 2 ) | ( (int) ( inData[ i + 2 ] & 0xC0 ) >> 6 ) ) ];
			res += base64chars[ vuint8(     inData[ i + 2 ] & 0x3F ) ];
		}

		if( i < (long) inSize )
		{
			res += base64chars[ vuint8( ( inData[ i ] >> 2 ) & 0x3F ) ];
			
			if( i == (long) inSize - 1 )
			{
				res += base64chars[ vuint8( ( ( inData[ i ] & 0x3 ) << 4 ) ) ];
				res += '=';
			}
			else 
			{
				res += base64chars[ vuint8( ( ( inData[ i ] & 0x3 ) << 4 ) | ( (int) ( inData[ i + 1 ] & 0xF0 ) >> 4 ) ) ];
				res += base64chars[ vuint8( ( ( inData[ i + 1 ] & 0xF ) << 2 ) ) ];
			}

			res += '=';
		}
	}
	
	return res;
}


/**********************************************************************************************/
#define BITS_PER_BYTE	8
#define BITHIGH			0x80


/**********************************************************************************************/
// inData = { 0, 1, 15, 240, 170, 255 };
// inBytes = 6
// res = b'000000000000000100001111111100001010101011111111'
//
String DataToBinString( 
	vuint8* 	inData, 
	vuint32 	inBytes )
{
	String res;

	vuint32 ExpectedSymbolLength = inBytes * BITS_PER_BYTE;

	UChar* Buffer = res.getBuffer( ExpectedSymbolLength );
	DataToBinString( inData, Buffer, inBytes );
	res.releaseBuffer( ExpectedSymbolLength );
	
	return res;
}


/**********************************************************************************************/
// inData = { 0, 1, 15, 240, 170, 255 };
// ioLen = 6
// outStr = b'000000000000000100001111111100001010101011111111' - no terminated 0 !!!
// ioLen = 48
// We believe that outStr was allocated with enough space to keep inBytes * 8 symbols.
//
void DataToBinString( 
	vuint8* 	inData, 
	UChar*		outStr,
	vuint32& 	ioLen )
{
	FBL_CHECK( outStr );
	
	if( inData )
	{ 
		vuint32 ExpectedSymbolLength	= ioLen * BITS_PER_BYTE;
		UChar*	pD						= outStr;
		memset( outStr, 0, ExpectedSymbolLength * sizeof(UChar) );

		vuint8* pS  = inData;
		vuint8* pE  = pS + ioLen; 

		ioLen = ExpectedSymbolLength;
		vuint8 x = 0;	
		
		// Reading source data byte by byte.
		while( pS < pE )
		{
			// Printing all the bits for the current byte.
			x = *pS;
			for( int j = 0; j < BITS_PER_BYTE; ++j )
			{
				*pD++ = (x & BITHIGH) ? UChar('1') : UChar('0');
				x <<= 1;
			}
			
			// Next source byte:
			++pS;
		}
	}	
}


/**********************************************************************************************/
// inData = b'000000000000000100001111111100001010101011111111'
// ioLen = 6*8 = 48
// res = { 0, 1, 15, 240, 170, 255 };
// ioLen = 6
//
vuint8* DataFromBinString( 
	const UChar*		inStr, 
	vuint32& 			ioLen )
{
	if( inStr == nullptr ) 
		return nullptr;
	
	vuint32 BinLength	= ioLen / BITS_PER_BYTE;
    vuint8* Buffer		= new vuint8[ BinLength ];
	DataFromBinString( inStr, Buffer, ioLen );
	return Buffer;
}


/**********************************************************************************************/
// inData = b'000000000000000100001111111100001010101011111111'
// ioLen = 6*8 = 48
// res = { 0, 1, 15, 240, 170, 255 };
// ioLen = 6
// We believe that outStr was allocated enough to keep ioLen/8 symbols
//
void DataFromBinString( 
	const UChar*		inStr, 
	vuint8*				outStr,
	vuint32& 			ioLen )
{
	FBL_CHECK( outStr );
	
	if( inStr ) 
	{
		vuint32 BinLength	= ioLen / BITS_PER_BYTE;
		vuint8* pD			= outStr;
		memset( outStr, 0, BinLength );

		UChar* pS  = (UChar*) inStr;
		UChar* pE  = pS + ioLen; 

		// Set target buffer length.
		ioLen = BinLength;
		vuint8 x = BITHIGH;
		
		// Reading source string UChar by UChar.
		while( pS < pE )
		{
			// If current src UChar is set to '1', then set appropriate bit in the target buffer.
			if( *pS == UChar('1') )
			{
				*pD |= x;
			}
			else if( *pS != UChar('0') )
			{
				// There is neither '1' nor '0' occured - no much deal - treat it as '0'.
				FBL_CHECK( false );
			}

			// Choose next bit in the mask until no bits in the byte-mask remain.
			// Then start it from the first bit in byte-mask again.
			x >>= 1;
			if (x == 0)
			{
				x = BITHIGH;
				pD++;
			}
				
			// Next source UChar:
			++pS;
		}
	}
}


/**********************************************************************************************/
// inData = b'000000000000000100001111111100001010101011111111'
// ioLen = 6*8 = 48
// res = { 0, 1, 15, 240, 170, 255 };
// ioLen = 6
//
vuint8* DataFromBinString( 
	const char*			inStr, 
	vuint32& 			ioLen )
{
	if( inStr == nullptr ) 
		return nullptr;
	
	vuint32 BinLength	= ioLen / BITS_PER_BYTE;
    vuint8* Buffer		= new vuint8[ BinLength ];
	vuint8* pD			= Buffer;
	memset( Buffer, 0, BinLength );

	char* pS = (char*) inStr;
	char* pE = pS + ioLen; 

	// Set target buffer length.
	ioLen = BinLength;
	vuint8 x = BITHIGH;
	
	// Reading source string byte by byte.
	while( pS < pE )
	{
		// If current src byte is set to '1', then set appropriate bit in the target buffer.
		if( *pS == '1' )
		{
			*pD |= x;
		}
		else if( *pS != '0' )
		{
			// There is neither '1' nor '0' occured - no much deal - treat it as '0'.
			FBL_CHECK( false );
		}

		// Choose next bit in the mask until no bits in the byte-mask remain.
		// Then start it from the first bit in byte-mask again.
		x >>= 1;
		if (x == 0)
		{
			x = BITHIGH;
			pD++;
		}
			
		// Next source byte:
		++pS;
	}
	
	return Buffer;
}


/**********************************************************************************************/
FBL_End_Namespace
