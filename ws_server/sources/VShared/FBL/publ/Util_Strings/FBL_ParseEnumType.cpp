/**********************************************************************************************/
/* FBL_ParseEnumType.cpp																	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include "FBL_ParseEnumType.h"

#include <VShared/FBL/publ/Util_classes/FBL_StringConversion.h>


/**********************************************************************************************/
FBL_Begin_Namespace


#pragma mark -


/**********************************************************************************************/
/*	Field fld_specific_info contains description of type,
	for enum it is a list of values and their localization ( if defined by user ) in format:
	('v1','v2','v3'), 'lang1':('lang1_v1','lang1_v2','lang1_v3'), 'lang2':('lang2_v1','lang2_v2','lang2_v3')
	or just:
	('v1','v2','v3')

	where v1,v2,v3   - values for English language
	lang1            - name of some other language
	lang_v1          - name for value v1 in lang1 language

	Returns list of locale names as string with COMA delimiter:
	"'en','lang1','lang2'"
	*/
static ArrayOfStringsObj_Ptr EnumType_GetLocales(const char* inStr, vuint32 inLen)
{
	const char* p = inStr;

	auto throwError = [&]( void )
	{
		size_t pos = (p == nullptr) ? 0 : p - inStr;
		String strPos;
		strPos << (vuint64)pos;
		FBL_Throw( xExternalError(ERR_EXTERNAL_INVALID_PARAMETER, strPos.getBufferA() ) );
	};

	if( p == nullptr )
	{
		throwError();
	}

	const char* pe = inStr + (inLen ? inLen : strlen(inStr));
	if( p == pe )
	{
		throwError();
	}

	bool inParens		= false; // ()
	bool localeExpected = false;

	ArrayOfStringsObj_Ptr plocaleName_list = new ArrayOfStringsObj();
	plocaleName_list->AddItem( "en" );

	while (p < pe)
	{
		switch (*p)
		{
			case ' ':
				p++;
			break;

			case ',':
				p++;
				if (inParens == false)
					localeExpected = true;
			break;

			case '(':
				inParens = true;
				p++;
			break;

			case ')':
				inParens = false;
				p++;
			break;

			case '\'':
			{
				const char* pLocaleStart = ++p;

				while( p < pe && *p != '\'' )
					p++;
						
				if (p == pe)
					throwError();

				const char* pLocaleEnd = p++;

				if( localeExpected )
				{
					// add the locale name to the list
					plocaleName_list->AddItem( String( pLocaleStart, tslen(pLocaleEnd - pLocaleStart) ));

					localeExpected = false;
				}				
			}break;

			case ':':
				p++;
				if (inParens)
					throwError();
			break;

			default:
				p++;
			break;
		}
	}

	return plocaleName_list;
}


/**********************************************************************************************/
static ArrayOfStringsObj_Ptr EnumType_GetLocaleItems( const char* inStr, vuint32 inLen, const char* inLocaleName)
{	
	const char* p = inStr;

	auto throwError = [&]( void )
	{
		size_t pos = (p == nullptr) ? 0 : p - inStr;
		String strPos;
		strPos << (vuint64)pos;
		FBL_Throw( xExternalError(ERR_EXTERNAL_INVALID_PARAMETER, strPos.getBufferA() ) );
	};

	if( p == nullptr )
	{
		throwError();
	}

	const char* pe = inStr + (inLen ? inLen : strlen(inStr));
	if( p == pe )
	{
		throwError();
	}

	String localeName = "en"; // en local by default

	bool foundedLocale	= false; // this flag will be true, if we find needed locale
	bool inParens		= false; // ()
	bool localeExpected = false;

	ArrayOfStringsObj_Ptr plocaleItems_list = new ArrayOfStringsObj();

	// we break cycle, if we find needed locale
	while ( p < pe && ( (foundedLocale == false) || inParens ) )
	{
		switch (*p)
		{
			case ' ':
				p++;
			break;

			case ',':
				p++;
				if (inParens == false)
					localeExpected = true;
			break;

			case '(':
				inParens = true;
				p++;
			break;

			case ')':
				inParens = false;
				p++;
			break;

			case '\'':
			{				
				const char* pQouteStart = ++p;

				while( p < pe && *p != '\'' )
				{
					p++;
				}
						
				if (p == pe)
					throwError();

				const char* pQouteEnd = p++;

				if( localeName == inLocaleName )
				{
					foundedLocale = true;

					//add the item name to the list
					plocaleItems_list->AddItem( String( pQouteStart, tslen(pQouteEnd - pQouteStart) ) );
				}	

				if( localeExpected )
				{
					localeName = String( pQouteStart, tslen(pQouteEnd - pQouteStart) );

					localeExpected = false;
				}
			}break;

			case ':':
				p++;
				if (inParens)
					throwError();
			break;

			default:
				p++;
			break;
		}
	}

	return plocaleItems_list;
}


#pragma mark -
#pragma mark ===== Entry Points ======


/**********************************************************************************************/
ArrayOfStringsObj_Ptr EnumType_GetLocales( const String& inStr )
{
	ArrayOfStringsObj_Ptr res;

	if( inStr.isSingleByte() )
	{
		res = EnumType_GetLocales( inStr.getBufferA(), inStr.length() );
	}
	else // we have UTF16, 2 bytes strings.
	{
		// WE convert UTF16 into UTF-8, so we can use above char* algs. 
		StFromUTF16 fromUTF16( inStr, "UTF-8" );

		res = EnumType_GetLocales( fromUTF16.c_str(), fromUTF16.length() );

		// res is array, which contains Strings.
		// If we working in KERNEL mode, they should be already in UTF16, so ok.
		//
		// If we working in CLIENT mode, which uses single-byte connection, 
		// then strings should be alrady in single byte encoding. Again ok.
	}

	return res;  // <-- Problem here
}


/**********************************************************************************************/
ArrayOfStringsObj_Ptr EnumType_GetLocaleItems(const String& inStr, const String& inLocaleName)
{
	ArrayOfStringsObj_Ptr res;

	if( inStr.isSingleByte() )
	{
		if( inLocaleName.isSingleByte() )
		{
			res = EnumType_GetLocaleItems( inStr.getBufferA(), inStr.length(), inLocaleName.getBufferA() );
		}
		else // if the inLocaleName is in UTF-16, 2 bytes strings
		{
			StFromUTF16 fromUTF16_LocaleName(inLocaleName);
			res = EnumType_GetLocaleItems( inStr.getBufferA(), inStr.length(), fromUTF16_LocaleName.c_str() );
		}
	}
	else // we have UTF16, 2 bytes strings.
	{
		// WE convert UTF16 into UTF-8, so we can use above char* algs. 
		StFromUTF16 fromUTF16_Str( inStr, "UTF-8" );

		if( inLocaleName.isSingleByte() )
		{
			res = EnumType_GetLocaleItems( fromUTF16_Str.c_str(), fromUTF16_Str.length(), inLocaleName.getBufferA() );
		}
		else // if the inLocaleName is in UTF-16, 2 bytes strings
		{
			StFromUTF16 fromUTF16_LocaleName(inLocaleName);
			res = EnumType_GetLocaleItems( fromUTF16_Str.c_str(), fromUTF16_Str.length(), fromUTF16_LocaleName.c_str() );
		}


		// res is array, which contains Strings.
		// If we working in KERNEL mode, they should be already in UTF16, so ok.
		//
		// If we working in CLIENT mode, which uses single-byte connection, 
		// then strings should be alrady in single byte encoding. Again ok.
	}

	return res;
}


/**********************************************************************************************/
FBL_End_Namespace
