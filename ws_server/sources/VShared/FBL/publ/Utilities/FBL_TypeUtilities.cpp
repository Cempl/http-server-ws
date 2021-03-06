/**********************************************************************************************/
/* FBL_TypeUtilities.cpp 						                                  			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
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
String Type2String( FIELD_TYPE inType )
{
	switch( inType )
	{
		case kTypeEmpty:
			return "Empty";
		case kTypeEnum:
			return "Enum";
		case kTypeBoolean:
			return "Boolean";
		case kTypeByte:
			return "Byte";
		case kTypeShort:
			return "Short";
		case kTypeUShort:
			return "UShort";
		case kTypeMedium:
			return "Medium";
		case kTypeUMedium:
			return "UMedium";
		case kTypeLong:
			return "Long";
		case kTypeULong:
			return "ULong";
		case kTypeLLong:
			return "LLong";
		case kTypeULLong:
			return "ULLong";
		case kTypeFloat:
			return "Float";
		case kTypeDouble:
			return "Double";
		case kTypeLDouble:
			return "LDouble";
		case kTypeDecimal:
			return "Decimal";
		case kTypeDate:
			return "Date";
		case kTypeTime:
			return "Time";
		case kTypeDateTime:
			return "DateTime";
		case kTypeString:
			return "String";
		case kTypeVarChar:
			return "VarChar";
		case kTypeFixedBinary:
			return "FixedBinary";
		case kTypeVarBinary:
			return "VarBinary";
		case kTypeBLOB:
			return "BLOB";
		case kTypeText:
			return "Text";
		case kTypePicture:
			return "Picture";
		case kTypeSound:
			return "Sound";
		case kTypeMovie:
			return "Movie";
		case kTypeRecID:
			return "RecID";
		case kTypeOID:
			return "OID";
		case kTypeObjectPtr:
			return "ObjectPtr";
		case kTypeObjectsPtr:
			return "ObjectsPtr";
		case kTypeTimeStamp:
			return "TimeStamp";
		case kTypeEnum8:
			return "Enum8";
		case kTypeEnum16:
			return "Enum16";
		case kTypeSerial32:
			return "Serial32";
		case kTypeSerial64:
			return "Serial64";
		case kTypeMoney:
			return "Money";
		case kTypeVariant:
			return "Variant";
		case kTypeCompound:
			return "Compound";
		case kTypeUserDefined:
			return "UserDefined";

		default:
			return String::sEmpty();
	};
}


/**********************************************************************************************/
FIELD_TYPE String2Type( const String& inStr )
{
	if( inStr.caseCompare( "Empty", 5 ) == 0 )
		return kTypeEmpty;
	else if( inStr.caseCompare( "Enum", 4 ) == 0 )
		return kTypeEnum;
	else if( inStr.caseCompare( "Boolean", 7 ) == 0 )
		return kTypeBoolean;
	else if( inStr.caseCompare( "Byte", 4 ) == 0 )
		return kTypeByte;
	else if( inStr.caseCompare( "Short", 5 ) == 0 )
		return kTypeShort;
	else if( inStr.caseCompare( "UShort", 6 ) == 0 )
		return kTypeUShort;
	else if( inStr.caseCompare( "Medium", 6 ) == 0 )
		return kTypeMedium;
	else if( inStr.caseCompare( "UMedium", 7 ) == 0 )
		return kTypeUMedium;
	else if( inStr.caseCompare( "Long", 4 ) == 0 )
		return kTypeLong;
	else if( inStr.caseCompare( "ULong", 5 ) == 0 )
		return kTypeULong;
	else if( inStr.caseCompare( "LLong", 5 ) == 0 )
		return kTypeLLong;
	else if( inStr.caseCompare( "ULLong", 6 ) == 0 )
		return kTypeULLong;
	else if( inStr.caseCompare( "Float", 5 ) == 0 )
		return kTypeFloat;
	else if( inStr.caseCompare( "Double", 6 ) == 0 )
		return kTypeDouble;
	else if( inStr.caseCompare( "LDouble", 7 ) == 0 )
		return kTypeLDouble;
	else if( inStr.caseCompare( "Decimal", 7 ) == 0 )
		return kTypeDecimal;
	else if( inStr.caseCompare( "Date", 4 ) == 0 )
		return kTypeDate;
	else if( inStr.caseCompare( "Time", 4 ) == 0 )
		return kTypeTime;
	else if( inStr.caseCompare( "DateTime", 8 ) == 0 )
		return kTypeDateTime;
	else if( inStr.caseCompare( "String", 6 ) == 0 )
		return kTypeString;
	else if( inStr.caseCompare( "VarChar", 7 ) == 0 )
		return kTypeVarChar;
	else if( inStr.caseCompare( "FixedBinary", 11 ) == 0 )
		return kTypeFixedBinary;
	else if( inStr.caseCompare( "VarBinary", 9 ) == 0 )
		return kTypeVarBinary;
	else if( inStr.caseCompare( "BLOB", 4 ) == 0 )
		return kTypeBLOB;
	else if( inStr.caseCompare( "Text", 4 ) == 0 )
		return kTypeText;
	else if( inStr.caseCompare( "Picture", 7 ) == 0 )
		return kTypePicture;
	else if( inStr.caseCompare( "Sound", 5 ) == 0 )
		return kTypeSound;
	else if( inStr.caseCompare( "Movie", 5 ) == 0 )
		return kTypeMovie;
	else if( inStr.caseCompare( "RecID", 5 ) == 0 )
		return kTypeRecID;
	else if( inStr.caseCompare( "OID", 3 ) == 0 )
		return kTypeOID;
	else if( inStr.caseCompare( "ObjectPtr", 9 ) == 0 )
		return kTypeObjectPtr;
	else if( inStr.caseCompare( "ObjectsPtr", 10 ) == 0 )
		return kTypeObjectsPtr;
	else if( inStr.caseCompare( "TimeStamp", 9 ) == 0 )
		return kTypeTimeStamp;
	else if( inStr.caseCompare( "Enum8", 5 ) == 0 )
		return kTypeEnum8;
	else if( inStr.caseCompare( "Enum16", 6 ) == 0 )
		return kTypeEnum16;
	else if( inStr.caseCompare( "Serial32", 8 ) == 0 )
		return kTypeSerial32;
	else if( inStr.caseCompare( "Serial64", 8 ) == 0 )
		return kTypeSerial64;
	else if( inStr.caseCompare( "Money", 5 ) == 0 )
		return kTypeMoney;
	else if( inStr.caseCompare( "Variant", 7 ) == 0 )
		return kTypeVariant;
	else if( inStr.caseCompare( "Compound", 8 ) == 0 )
		return kTypeCompound;
	else if( inStr.caseCompare( "UserDefined", 11 ) == 0 )
		return kTypeUserDefined;

	return 	kTypeEmpty;
}


#pragma mark -

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
