/**********************************************************************************************/
/* FBL_TypeUtilities.cpp 						                                  			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Util_classes/FBL_ToUChar.h>

#include <VShared/FBL/publ/Utilities/FBL_TypeUtilities.h>
#include <VShared/FBL/publ/Util_Strings/FBL_ustdio.h>

#include <string>

/**********************************************************************************************/
Std_Using_Namespace


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// HELPER function. Find appropriate ident in inpEnumType
//
const String& GetEnumValueAsString( 
	const String&			inValue,
	I_Type_Enumerated_Ptr	inpEnumType )
{
	FBL_CHECK( inpEnumType );

	// Trying to treat inValue as Ident name
	vuint16 value = inpEnumType->get_IdentAsLong( inValue );
	if( value == 0 )
	{
		// It could be
		// 1. 0 or empty (allowed value - means the same as NULL)
		// 2. Corresponded value ( 1 for Monday and so on)
		// 3. Not existed value - error

		if( inValue.isEmpty() == false && inValue != "0" )
		{
			value = (vuint16)inValue.parseUInt();
			if( value )
			{
				const String& ident = inpEnumType->get_IdentAsString( (vuint16)inValue.parseUInt() );
				if( ident.isEmpty() )
				{
					I_Type_Ptr pType = QI( inpEnumType, I_Type );
					FBL_Throw( xTypeError(
						ERR_TYPE_BAD_DEFAULT_VALUE, 
						pType->get_TypeName().c_str(), 
						inValue.c_str() ) );
				}

				return ident;
			}
			else
			{
				I_Type_Ptr pType = QI( inpEnumType, I_Type );
				FBL_Throw( xTypeError(
					ERR_TYPE_BAD_DEFAULT_VALUE, 
					pType->get_TypeName().c_str(), 
					inValue.c_str() ) );
			}
		}
	}

	return inValue;
}


/**********************************************************************************************/
// HELPER function. Find appropriate ident value in inpEnumType
//
vuint16 GetEnumValueAsInt(
	const String&			inValue,
	I_Type_Enumerated_Ptr	inpEnumType )
{
	// Trying to treat inStr as Ident name
	vuint16 res = inpEnumType->get_IdentAsLong( inValue );
	if( res  == 0 )
	{
		// It could be
		// 1. 0 or empty (allowed value - means the same as NULL)
		// 2. Corresponded value ( 1 for Monday and so on)
		// 3. Not existed value - error

		if( inValue.isEmpty() == false && inValue != "0" )
		{
			res = (vuint16)inValue.parseUInt();
			if( res )
			{
				const String& ident = inpEnumType->get_IdentAsString( res );
				if( ident.isEmpty() )
				{
					I_Type_Ptr pType = QI( inpEnumType, I_Type );
					FBL_Throw( xValueError(
						ERR_ENUM_VALUE_INVALID, 
						ToUCharPtr(res).c_str(),
						pType->get_TypeName().c_str() ) );
				}
			}
			else
			{
				I_Type_Ptr pType = QI( inpEnumType, I_Type );
				FBL_Throw( xValueError(
					ERR_ENUM_VALUE_INVALID, 
					inValue.c_str(),
					pType->get_TypeName().c_str() ) );
			}
		}
	}

	return res;
}


/**********************************************************************************************/
// HELPER function. Find appropriate ident value in inpEnumType
//
vuint16 GetLocaleEnumValueAsInt(
	const String&			inLocale,
	const String&			inValue,
	I_Type_Enumerated_Ptr	inpEnumType )
{
	// Trying to treat inStr as Ident name
	vuint16 res = inpEnumType->get_LocaleIdentAsLong( inLocale, inValue );
	if( res  == 0 )
	{
		// It could be
		// 1. 0 or empty (allowed value - means the same as NULL)
		// 2. Corresponded value ( 1 for Monday and so on)
		// 3. Not existed value - error

		if( inValue.isEmpty() == false && inValue != "0" )
		{
			res = (vuint16)inValue.parseUInt();
			if( res )
			{
				const String& ident = inpEnumType->get_LocaleIdentAsString( inLocale, res );
				if( ident.isEmpty() )
				{
					I_Type_Ptr pType = QI( inpEnumType, I_Type );
					FBL_Throw( xValueError(
						ERR_ENUM_VALUE_INVALID, 
						ToUCharPtr(res).c_str(),
						pType->get_TypeName().c_str() ) );
				}
			}
			else
			{
				I_Type_Ptr pType = QI( inpEnumType, I_Type );
				FBL_Throw( xValueError(
					ERR_ENUM_VALUE_INVALID, 
					inValue.c_str(),
					pType->get_TypeName().c_str() ) );
			}
		}
	}

	return res;
}


/**********************************************************************************************/
// HELPER function. Check inValue to be valid ident value for inpEnumType
//
void CheckValue( 
	vuint16					inValue,
	I_Type_Enumerated_Ptr	inpEnumType )
{
	if( inValue )
	{
		const String& ident = inpEnumType->get_IdentAsString( inValue );
		if( ident.isEmpty() )
		{
			I_Type_Ptr pType = QI( inpEnumType, I_Type );
			FBL_Throw( xValueError(
				ERR_ENUM_VALUE_INVALID, 
				ToUCharPtr(inValue).c_str(),
				pType->get_TypeName().c_str() ) );
		}
	}	
}


/**********************************************************************************************/
const String& ConvertEnumValueTo(
	const String&			inLocale,
	const String&			inValue,
	I_Type_Enumerated_Ptr	inpEnumType )
{
	FBL_CHECK( inpEnumType );

	vuint16 val = GetEnumValueAsInt( inValue, inpEnumType );
	return inpEnumType->get_LocaleIdentAsString( inLocale, val );
}


/**********************************************************************************************/
const String& ConvertEnumValueFrom(
	const String&			inLocale,
	const String&			inValue,
	I_Type_Enumerated_Ptr	inpEnumType )
{
	FBL_CHECK( inpEnumType );

	vuint16 val = GetLocaleEnumValueAsInt( inLocale, inValue, inpEnumType );
	return inpEnumType->get_IdentAsString( val );
}


/**********************************************************************************************/
// Helper function to get affinity sqlite type
//
const String& GetSqliteAffinityTypeByName( const char* inType )
{
	// Following https://www.sqlite.org/datatype3.html
	//
	
	// Patterns:
static std::string int_str( "int" );

static std::string char_str( "char" );
static std::string clob_str( "clob" );
static std::string text_str( "text" );

static std::string blob_str( "blob" );

static std::string real_str( "real" );
static std::string floa_str( "floa" );
static std::string doub_str( "doub" );

	// Possible results:
static String sTextType		( "text" 	);
static String sNumericType	( "numeric" );
static String sIntegerType	( "integer" );
static String sRealType		( "real" 	);
static String sBlobType		( "blob" 	);


	std::string typeLwr( inType );
    std::transform( typeLwr.begin(), typeLwr.end(), typeLwr.begin(), ::tolower );


	if( typeLwr.find( int_str ) != std::string::npos )
	{
		return sIntegerType;
	}
	else if( typeLwr.find( blob_str ) != std::string::npos )
	{
		return sBlobType;
	}
	else if(	typeLwr.find( char_str ) != std::string::npos
			||	typeLwr.find( clob_str ) != std::string::npos
			||	typeLwr.find( text_str ) != std::string::npos )
	{
		return sTextType;
	}
	else if(	typeLwr.find( real_str ) != std::string::npos
			||	typeLwr.find( floa_str ) != std::string::npos
			||	typeLwr.find( doub_str ) != std::string::npos )
	{
		return sRealType;
	}
	else
	{
		return sNumericType;
	}
}


/**********************************************************************************************/
FBL_End_Namespace
