/**********************************************************************************************/
/* FBL_EscapeString.cpp 					                                  				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Util_Strings/FBL_EscapeString.h>
#include <VShared/FBL/publ/Util_Strings/FBL_ustdio.h>
#include <VShared/FBL/publ/Utilities/FBL_ByteToHex.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
//
UChar xml_s1_uchar[]	= { UChar('&'), UChar('a'), UChar('m'), UChar('p'), UChar(';') };
UChar xml_s2_uchar[]	= { UChar('&'), UChar('l'), UChar('t'), UChar(';') };
UChar xml_s3_uchar[]	= { UChar('&'), UChar('g'), UChar('t'), UChar(';') };
UChar xml_s4_uchar[]	= { UChar('&'), UChar('q'), UChar('u'), UChar('o'), UChar('t'), UChar(';') };

char xml_s1_char[]		= { char('&'),	char('a'),	char('m'),	char('p'),	char(';') };
char xml_s2_char[]		= { char('&'),	char('l'),	char('t'),	char(';') };
char xml_s3_char[]		= { char('&'),	char('g'),	char('t'),	char(';') };
char xml_s4_char[]		= { char('&'),	char('q'),	char('u'),	char('o'),	char('t'),	char(';') };

// ATTENTION. Now we are trying to use XML 1.1 instead of 1.0. Because of improvements 
// with 'control' characters usage. 
// Usage of 0x1 - 0x1F  were not allowed in 1.0 but allowed in 1.1 as
// symbol references (&#x1; ...)
// On the other hand symbols 0x7F - 0x9F were allowed as direct symbols in 1.0 but allowed
// as references only in 1.1
//
UChar xml_x_uchar[] = { UChar('&'), UChar('#'), UChar('x')	};
char xml_x_char[]	= { char('&'),	char('#'),	char('x')	};


/**********************************************************************************************/
void AddXmlEncoding_( UChar* inpStart, UChar* inpEnd, String& outValue )
{
	// Set pointers.
	const UChar* p  = inpStart;
	const UChar* e  = inpEnd;
	
	
	while( p < e )
	{
		if( (*p < UChar(0x20)) ||
			( *p > UChar(0x7E) && *p < UChar(0xA0) ) )
		{			
			// &#x1; - &#x1F;
			// &#x7F; - &#x9F;
			outValue += UChar('&');
			outValue += UChar('#');
			outValue += UChar('x');
			outValue.append( ByteToHex( vuint8(*p) ), 2 );
			outValue += UChar(';');
		}
		else
		{
			switch( *p ) 
			{
				case UChar('&'): 
				{
					outValue.append( xml_s1_uchar, 5 );
				}break;	

				case UChar('<'):
				{
					outValue.append( xml_s2_uchar, 4 );
				}break;				
				
				case UChar('>'):
				{
					outValue.append( xml_s3_uchar, 4 );
				}break;				

				case UChar('"'): 
				{
					outValue.append( xml_s4_uchar, 6 );
				}break;	

				default: 
				{
					outValue += *p;
				}break;
			}	
		}

		p++;
	}
}


/**********************************************************************************************/
void RemoveXmlEncoding_( UChar* inpStart, UChar* inpEnd, String& outValue )
{
	// Set pointers
	const UChar* p  = inpStart;
	const UChar* e  = inpEnd;
	
//	// IS: 20061028
//	// On http://www.valentina-db.com/bt/view.php?id=1943
//	// Strange. Works fine on PPC and Windows and leads to crash on MacIntel.
//	// When tslen ToAllocate = mLen - (3-1) + 1; we write last symbol out of the NewStr. 
//	//
//	
//	// we know that at least one escape exists, so size will be less at least 3 chars.
//	//tslen ToAllocate = mLen - (3-1) + 1;
//	tslen ToAllocate = mLen;
//	MemPtr<UChar> NewStr( (vuint32) ToAllocate );	
//	UChar* pn = NewStr.begin();


	UChar ch = 0;
	
	while( p < e )
	{
		switch( *p ) 
		{
			case UChar('&'):
			{		
				// Actually, we do it in wrong way. The parser self should prepare the string.
				// But currently expat supports only XML 1.0 - not 1.1 So we need use CDATA for
				// string fields and encode some symbol set there using xml symbol references.
				// Usually it means that data must come to destination as is but we use it as
				// a trick - for blind the xml-parser and decode the data manually.
				// 
				if( vu_strncmp( xml_x_uchar, p, 3 ) == 0 )
				{			
					// We wrote two symbols for any hex code always!
					// So we expect to get it here as two symbols.
					// It could be just &#x1; but it is minor bug because of it is very
					// temporary solution at all. (see comments above).

					// &#x1; - &#x1F;
					// &#x7F; - &#x9F;					
					ch = UChar( (HexToByte(*(p+3)) * 16 + HexToByte(*(p+4))) );
					p += 6;

				}
				else if( vu_strncmp( xml_s1_uchar, p, 4 ) == 0 )	// "&amp;"
				{
					ch = UChar('&');
					p += 5;
				}
				else if( vu_strncmp( xml_s2_uchar, p, 3 ) == 0 )	// "&lt;"
				{
					ch = UChar('<');
					p += 4;
				}
				else if( vu_strncmp( xml_s3_uchar, p, 3 ) == 0 )	// "&gt;"
				{
					ch = UChar('>');
					p += 4;
				}
				else if( vu_strncmp( xml_s4_uchar, p, 5 ) == 0 )	// "&quot;"
				{
					ch = UChar('"');
					p += 6;
				}
				else
				{
					ch = *p++;
				}

			}break;
					
			default:
			{
				ch = *p++;
			}break;
		}	
		
//		*pn++ = ch;
		outValue += ch;
	}
}


/**********************************************************************************************/
void AddXmlEncoding_( char* inpStart, char* inpEnd, String& outValue )
{
	// Set pointers.
	const char* p  = inpStart;
	const char* e  = inpEnd;
	
	
	while( p < e )
	{
		if( (*p < vuint8(0x20)) ||
			( *p > vuint8(0x7E) /*&& *p < vuint8(0xA0)*/ ) )
		{			
			// &#x1; - &#x1F;
			// &#x7F; - &#x9F;
			outValue += char('&');
			outValue += char('#');
			outValue += char('x');
			outValue.append( ByteToHex( *p ), 2 );
			outValue += char(';');
		}
		else
		{
			switch( *p ) 
			{
				case char('&'):
				{
					outValue.append( xml_s1_char, 5 );
				}break;	

				case char('<'):
				{
					outValue.append( xml_s2_char, 4 );
				}break;				
				
				case char('>'):
				{
					outValue.append( xml_s3_char, 4 );
				}break;				

				case char('"'):
				{
					outValue.append( xml_s4_char, 6 );
				}break;	

				default: 
				{
					outValue += *p;
				}break;
			}	
		}

		p++;
	}
}


/**********************************************************************************************/
void RemoveXmlEncoding_( char* inpStart, char* inpEnd, String& outValue )
{
	// Set pointers
	const char* p  = inpStart;
	const char* e  = inpEnd;
	char ch = 0;
	
	while( p < e )
	{
		switch( *p ) 
		{
			case char('&'):
			{		
				// Actually, we do it in wrong way. The parser self should prepare the string.
				// But currently expat supports only XML 1.0 - not 1.1 So we need use CDATA for
				// string fields and encode some symbol set there using xml symbol references.
				// Usually it means that data must come to destination as is but we use it as
				// a trick - for blind the xml-parser and decode the data manually.
				// 
				if( strncmp( xml_x_char, p, 3 ) == 0 )
				{			
					// We wrote two symbols for any hex code always!
					// So we expect to get it here as two symbols.
					// It could be just &#x1; but it is minor bug because of it is very
					// temporary solution at all. (see comments above).

					// &#x1; - &#x1F;
					// &#x7F; - &#x9F;					
					ch = char( (HexToByte(*(p+3)) * 16 + HexToByte(*(p+4))) );
					p += 6;

				}
				else if( strncmp( xml_s1_char, p, 4 ) == 0 )	// "&amp;"
				{
					ch = char('&');
					p += 5;
				}
				else if( strncmp( xml_s2_char, p, 3 ) == 0 )	// "&lt;"
				{
					ch = char('<');
					p += 4;
				}
				else if( strncmp( xml_s3_char, p, 3 ) == 0 )	// "&gt;"
				{
					ch = char('>');
					p += 4;
				}
				else if( strncmp( xml_s4_char, p, 5 ) == 0 )	// "&quot;"
				{
					ch = char('"');
					p += 6;
				}
				else
				{
					ch = *p++;
				}

			}break;
					
			default:
			{
				ch = *p++;
			}break;
		}	
		
//		*pn++ = ch;
		outValue += ch;
	}
}


#pragma mark -

/**********************************************************************************************/
vuint32 EscapeStringU( UChar* inBuffer, const UChar* inStr, vuint32 inLen )
{
    argused1( inLen );    
    
		  UChar* pDest = inBuffer;
	const UChar* pSrc  = inStr;
	
	while( 1 )
	{
		switch( *pSrc )
		{
			case UChar('\''):
			case UChar('\\'):
			{
				*pDest++ = UChar('\\');
			}break;
				
			case UChar('\0'):
			{
				*pDest = UChar('\0');
				goto exit;
			}break;
		}
		
		*pDest++ = *pSrc++;
	}
	
exit:
	return static_cast<vuint32>((pDest - inBuffer));
}


/**********************************************************************************************/
vuint32 EscapeStringA( char*  inBuffer, const char*  inStr, vuint32 inLen )
{
    argused1( inLen );    

	char* 		pDest = inBuffer;
	const char* pSrc  = inStr;
	
	while( 1 )
	{
		switch( *pSrc )
		{
			case '\'':
			case '\\':
			{
				*pDest++ = '\\';
			}break;
				
			case '\0':
			{
				*pDest = '\0';
				goto exit;
			}break;
		}
		
		*pDest++ = *pSrc++;
	}
	
exit:
	return static_cast<vuint32>((pDest - inBuffer));
}

#pragma mark -


/**********************************************************************************************/
// Pointer returned by this function must be deleted only by DeleteEscapedStringU()!
//
UChar* EscapeStringU( const UChar* inStr, vuint32 inLen )
{
	if( inStr == nullptr )
		return nullptr;

	if( inLen == 0 )
		inLen = vu_strlen(inStr);

	// In the worse case, each character may need to be encoded as 
	// using two chars, and we need room for the terminating null byte too
	UChar* pNew = new UChar[2 * inLen + 1];

	EscapeStringU( pNew, inStr, inLen );
	
	return pNew;
}


/**********************************************************************************************/
// Pointer returned by this function must be deleted only by DeleteEscapedStringU()!
//
char* EscapeStringA( const char* inStr, vuint32 inLen )
{
	if( inStr == nullptr )
		return nullptr;

	if( inLen == 0 )
		inLen = static_cast<vuint32>(strlen(inStr));

	// In the worse case, each character may need to be encoded as 
	// using two chars, and we need room for the terminating null byte too
	char* pNew = new char[2 * inLen + 1];

	EscapeStringA( pNew, inStr, inLen );
	
	return pNew;
}


/**********************************************************************************************/
void FreeStringU( const UChar* inStr )
{
	delete [] inStr;
}


/**********************************************************************************************/
void FreeStringA( const char* inStr )
{
	delete [] inStr;
}


/**********************************************************************************************/
FBL_End_Namespace
