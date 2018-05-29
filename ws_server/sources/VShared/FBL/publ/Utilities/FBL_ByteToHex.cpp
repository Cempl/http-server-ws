/**********************************************************************************************/
/* FBL_ByteToHex.cpp                                                          				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Util_classes/FBL_String.h>
#include <VShared/FBL/publ/Utilities/FBL_ByteToHex.h>
#include <VShared/FBL/publ/Util_Strings/FBL_ustdio.h>

#include <ctype.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
const char* sHex[] = 
{
	"00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "0A", "0B", "0C", "0D", "0E", "0F",
	"10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F",
	"20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2A", "2B", "2C", "2D", "2E", "2F",
	"30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3A", "3B", "3C", "3D", "3E", "3F",
	"40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "4A", "4B", "4C", "4D", "4E", "4F",
	"50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "5A", "5B", "5C", "5D", "5E", "5F",
	"60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "6A", "6B", "6C", "6D", "6E", "6F",
	"70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "7A", "7B", "7C", "7D", "7E", "7F",
	"80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "8A", "8B", "8C", "8D", "8E", "8F",
	"90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "9A", "9B", "9C", "9D", "9E", "9F",
	"A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9", "AA", "AB", "AC", "AD", "AE", "AF",
	"B0", "B1", "B2", "B3", "B4", "B5", "B6", "B7", "B8", "B9", "BA", "BB", "BC", "BD", "BE", "BF",
	"C0", "C1", "C2", "C3", "C4", "C5", "C6", "C7", "C8", "C9", "CA", "CB", "CC", "CD", "CE", "CF",
	"D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8", "D9", "DA", "DB", "DC", "DD", "DE", "DF",
	"E0", "E1", "E2", "E3", "E4", "E5", "E6", "E7", "E8", "E9", "EA", "EB", "EC", "ED", "EE", "EF",
	"F0", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "FA", "FB", "FC", "FD", "FE", "FF"
};


/**********************************************************************************************/
String DataToHexString( 
	vuint8* 	inData, 
	vuint32 	inBytes,
	bool		inHexWithData, 
	vuint32 	inDepth )
{
	if( inDepth > 0 )
		inDepth++;
	
	tslen MaxLength;
		
	vuint32 line;
	if( inBytes % 16 == 0 )
		line =  inBytes/16;
	else 
		line =  inBytes/16 + 1;
	
	if( inHexWithData )	
		MaxLength =tslen(line * ( 50 + inDepth ) + inBytes); // 50 = 48(for left part) + 2('\t' + '\n') 
	else
		MaxLength = tslen(inBytes * 3 + line * inDepth);


	String str;
	UChar* p   = str.getBuffer(MaxLength);
	UChar* tmp = p;
	const char* Hex;
	vuint32 ColumnCount = 1;
	
	
	// 49 = 48 + '\t'
	vuint16 len = vuint16(49 + inDepth);
	 
	for( vuint32 i = 1; i <= inBytes ; ++i )
	{
		if( ColumnCount == 1 )
		{
			for( vuint32 j = 1; j <= inDepth; ++j )
				*p++ = UChar('\t');
			tmp += len;
		}
		
		if( inHexWithData )
		{
			if( isprint( *inData ) )
				*tmp++ = (UChar) *inData;
			else 
				*tmp++ = UChar('.');
		}		
			
		Hex = ByteToHex( *inData++ );	
		
		*p++ = *Hex++;
		*p++ = *Hex++;
		
		if( (ColumnCount < 16) && (i != inBytes))
		{
			*p++ = UChar(' ');
			ColumnCount++;
		}
		else
		{
			if( inHexWithData )
			{
				*p++ = UChar(' ');
				if( i == inBytes )
				{
					vuint32 Count = line * 48 - i * 3;
					for( vuint32 j = 1; j <= Count; ++j )
						*p++ = ' ';
				}
				*p = UChar('\t');
				*tmp++ = UChar('\n');
				p += 18;
			}
			else
			{
				*p++ = UChar('\n');
			}

			ColumnCount = 1;
		}	
	} 
	
	str.releaseBuffer( MaxLength );
	return str;
}


/**********************************************************************************************/
String DataToHexString_Simple(
	vuint8* 	inData, 
	vuint32 	inBytes )
{
	tslen MaxLength = inBytes * 3;

	String str;
	UChar* p   = str.getBuffer(MaxLength);
	const char* Hex;

	for( vuint32 i = 1; i <= inBytes ; ++i )
	{
		Hex = ByteToHex( *inData++ );	
		
		*p++ = *Hex++;
		*p++ = *Hex++;

		*p++ = UChar(' ');
	}
	
	str.releaseBuffer( MaxLength );
	return str;
}


/**********************************************************************************************/
vuint8* DataFromHexString( 
	const UChar*		inStr, 
	vuint32& 			ioLen )
{
	if( inStr == nullptr ) 
		return nullptr;
	
	// Check string "format"
	// It could be 
	// 1. x'01 02 \n\r03 '
	// 2. x'010203'
	//
	bool spaced( (ioLen > 2) && vu_isspace(*(inStr + 2)) );

	vuint32 MaxLength = (spaced) ? ioLen / 3 + 1 : ioLen + 1;
    
    vuint8* Buffer = new vuint8[ MaxLength ];

	UChar* pS  = (UChar*) inStr;
	UChar* pE  = pS + ioLen; 
	vuint8* pD = Buffer;

	ioLen = 0;

	// Positioning on first not space.
	while( vu_isspace(*pS) )
		pS++;
		
	while( pS < pE )
	{
		*pD++ = (vuint8) (HexToByte(*pS) * 16 + HexToByte(*(pS+1)));
		ioLen++;

		pS += (spaced) ? 3 : 2;

		while( spaced && vu_isspace(*pS) )
			pS++;
	}
	
	return Buffer;
}


/**********************************************************************************************/
vuint8* DataFromHexString( 
	const char*			inStr, 
	vuint32& 			ioLen )
{
	if( inStr == nullptr ) 
		return nullptr;
	
	// Check string "format"
	// It could be 
	// 1. x'01 02 \n\r03 '
	// 2. x'010203'
	//
	bool spaced( (ioLen > 2) && isspace( (unsigned char)*(inStr + 2)) );

	vuint32 MaxLength = (spaced) ? ioLen / 3 + 1 : ioLen + 1;
    
    vuint8* Buffer = new vuint8[ MaxLength ];

	char* pS  = (char*) inStr;
	char* pE  = pS + ioLen; 
	vuint8* pD = Buffer;

	ioLen = 0;
	if( pS < pE )
	{
		// Positioning on first not space.
		while( isspace( (unsigned char)*pS ) )
		{
			pS++;
			if( pS == pE )
				break;
		}
		
		while( pS < pE )
		{
			*pD++ = (vuint8) (HexToByte(*pS) * 16 + HexToByte(*(pS+1)));
			ioLen++;

			pS += (spaced) ? 3 : 2;

			if( pS >= pE )
				break;

			while( spaced && isspace( (unsigned char)*pS ) )
				pS++;
		}
	}
	
	return Buffer;
}


/**********************************************************************************************/
FBL_End_Namespace
