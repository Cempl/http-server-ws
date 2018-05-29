/**********************************************************************************************/
/* FBL_Type2String.cpp                                                   					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Values/FBL_Type2String.h>

#include <VShared/FBL/publ/Util_Strings/FBL_ustdio.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// Use anonymous namespace to disable access to this struct directly.
//
struct Type2String_en_US
{
	VALUE_TYPE	 Type;
	const char*  Name;

} lTypeMap[] = 
{
	{ kTypeEmpty,		"Empty" },
	
	{ kTypeEnum,		"Enum" },
	{ kTypeBoolean,		"Boolean" },
	{ kTypeByte,		"Byte" },
	{ kTypeShort,		"Short" },
	{ kTypeUShort,		"Unsigned Short" },
	{ kTypeMedium,		"Medium" },
	{ kTypeUMedium,		"Unsigned Medium" },
	{ kTypeLong,		"Long" },
	{ kTypeULong,		"Unsigned Long" },
	{ kTypeLLong,		"Long Long" },
	{ kTypeULLong,		"Unsigned Long Long" },

	{ kTypeFloat,		"Float" },
	{ kTypeDouble,		"Double" },
	{ kTypeLDouble,		"Long Double" },
	{ kTypeDecimal,		"Decimal" },
	
	{ kTypeDate,		"Date" },
	{ kTypeTime,		"Time" },
	{ kTypeDateTime,	"DateTime" },

	{ kTypeString,		"String" },
	{ kTypeVarChar,		"VarChar" },

	{ kTypeFixedBinary,	"Fixed Binary" },
	{ kTypeVarBinary,	"VarBinary" },

	{ kTypeBLOB,		"BLOB" },
	{ kTypeText,		"Text" },
	{ kTypePicture,		"Picture" },
	{ kTypeSound,		"Sound" },

	{ kTypeRecID,		"RecID" },
	{ kTypeOID,			"OID" },

	{ kTypeObjectPtr,	"ObjectPtr" },
	{ kTypeObjectsPtr,	"ObjectsPtr" },

	{ kTypeTimeStamp,	"TimeStamp" },

	{ kTypeEnum8,		"Enum8" },
	{ kTypeEnum16,		"Enum16" },

	{ kTypeMoney,		"Money" },

	{ kTypeVariant,		"Variant" },

	{ kTypeCompound,	"Compound" }
};


/**********************************************************************************************/
// TEMPORARY: should be replaced! at least on binary search by sorted array.
//
String TypeCode2String( VALUE_TYPE inType, const char inLocale[] )
{
	argused1(inLocale);

	vuint32 Count = sizeof(lTypeMap) / sizeof(Type2String_en_US);
	for( vuint32 i = 0; i < Count; ++i )
	{
		if( inType == lTypeMap[i].Type )
			return lTypeMap[i].Name;
	}

	//FBL_CHECK(false);
	// return String::sEmpty();
static String strName;
	strName.setUInt(inType);
	return strName;	
}


/**********************************************************************************************/
// TEMPORARY: should be replaced !!!
// 
VALUE_TYPE String2TypeCode( const UChar* inTypeStr, const char inLocale[] )
{
	argused1(inLocale);

	if( !inTypeStr )
	{
		return static_cast<VALUE_TYPE>(-1);
	}

	switch( *inTypeStr )
	{
		case 'L':
			if( vu_strcmp(inTypeStr, "Long") == 0 )
			{
				return kTypeLong;
			}
			else if( vu_strcmp(inTypeStr, "Long Long") == 0 )
			{
				return kTypeLLong;
			}
			else if( vu_strcmp(inTypeStr, "Long Double") == 0 )
			{
				return kTypeLDouble;
			}
		break;

		case 'S':
			if( vu_strcmp(inTypeStr, "Short") == 0 )
			{
				return kTypeShort;
			}
			else if( vu_strcmp(inTypeStr, "String") == 0 )
			{
				return kTypeString;
			}
			break;

		case 'U':
			if( vu_strcmp(inTypeStr, "Unsigned Short") == 0 )
			{
				return kTypeUShort;
			}
			else if( vu_strcmp(inTypeStr, "Unsigned Medium") == 0 )
			{
				return kTypeUMedium;
			}
			else if( vu_strcmp(inTypeStr, "Unsigned Long") == 0 )
			{
				return kTypeULong;
			}
			else if( vu_strcmp(inTypeStr, "Unsigned Long Long") == 0 )
			{
				return kTypeULLong;
			}
			break;

		case 'V':
			if( vu_strcmp(inTypeStr, "VarChar") == 0 )
			{
				return kTypeVarChar;
			}
			else if( vu_strcmp(inTypeStr, "VarBinary") == 0 )
			{
				return kTypeVarBinary;
			}
			else if( vu_strcmp(inTypeStr, "Variant") == 0 )
			{
				return kTypeVariant;
			}
			break;

		case 'F':
			if( vu_strcmp(inTypeStr, "Float") == 0 )
			{
				return kTypeFloat;
			}
			else if( vu_strcmp(inTypeStr, "Fixed Binary") == 0 )
			{
				return kTypeFixedBinary;
			}
			break;

		case 'D':
			if( vu_strcmp(inTypeStr, "Date") == 0 )
			{
				return kTypeDate;
			}
			else if( vu_strcmp(inTypeStr, "Double") == 0 )
			{
				return kTypeDouble;
			}
			else if( vu_strcmp(inTypeStr, "DateTime") == 0 )
			{
				return kTypeDateTime;
			}
			else if( vu_strcmp(inTypeStr, "Decimal") == 0 )
			{
				return kTypeDecimal;
			}
			break;

		case 'T':
			if( vu_strcmp(inTypeStr, "Time") == 0 )
			{
				return kTypeTime;
			}
			else if( vu_strcmp(inTypeStr, "Text") == 0 )
			{
				return kTypeText;
			}
			else if( vu_strcmp(inTypeStr, "TimeStamp") == 0 )
			{
				return kTypeTimeStamp;
			}
			break;

		case 'B':
			if( vu_strcmp(inTypeStr, "Boolean") == 0 )
			{
				return kTypeBoolean;
			}
			else if( vu_strcmp(inTypeStr, "Byte") == 0 )
			{
				return kTypeByte;
			}
			else if( vu_strcmp(inTypeStr, "BLOB") == 0 )
			{
				return kTypeBLOB;
			}
			break;

		case 'O':
			if( vu_strcmp(inTypeStr, "ObjectPtr") == 0 )
			{
				return kTypeObjectPtr;
			}
			else if( vu_strcmp(inTypeStr, "ObjectsPtr") == 0 )
			{
				return kTypeObjectsPtr;
			}
			else if( vu_strcmp(inTypeStr, "OID") == 0 )
			{
				return kTypeOID;
			}
			break;

		case 'E':
			if( vu_strcmp(inTypeStr, "Empty") == 0 )
			{
				return kTypeEmpty;
			}
			else if( vu_strcmp(inTypeStr, "Enum") == 0 )
			{
				return kTypeEnum;
			}
			else if( vu_strcmp(inTypeStr, "Enum8") == 0 )
			{
				return kTypeEnum8;
			}
			else if( vu_strcmp(inTypeStr, "Enum16") == 0 )
			{
				return kTypeEnum16;
			}
			break;

		case 'M':
			if( vu_strcmp(inTypeStr, "Medium") == 0 )
			{
				return kTypeMedium;
			}
			else if( vu_strcmp(inTypeStr, "Money") == 0 )
			{
				return kTypeMoney;
			}
			break;
			
		default:
			if( vu_strcmp(inTypeStr, "RecID") == 0 )
			{
				return kTypeRecID;
			}
			else if( vu_strcmp(inTypeStr, "Picture") == 0 )
			{
				return kTypePicture;
			}
			else if( vu_strcmp(inTypeStr, "Sound") == 0 )
			{
				return kTypeSound;
			}
			break;
	}

	return static_cast<VALUE_TYPE>(-1);
}


/**********************************************************************************************/
const char* GetInternalTypeCodeString( FIELD_TYPE inType )
{
	const char* str = nullptr;
	switch( inType )
	{
		case kTypeEnum:
			str = "ENUM";
			break;
		case kTypeBoolean:
			str = "BOOLEAN";
			break;
		case kTypeByte:
			str = "BYTE";
			break;
		case kTypeShort:
			str = "SHORT";
			break;
		case kTypeUShort:
			str = "USHORT";
			break;
		case kTypeMedium:
			str = "MEDIUM";
			break;
		case kTypeUMedium:
			str = "UMEDIUM";
			break;
		case kTypeLong:
			str = "LONG";
			break;
		case kTypeULong:
			str = "ULONG";
			break;
		case kTypeLLong:
			str = "LLONG";
			break;
		case kTypeULLong:
			str = "ULLONG";
			break;

		case kTypeFloat:
			str = "FLOAT";
			break;
		case kTypeDouble:
			str = "DOUBLE";
			break;
		case kTypeLDouble:
			str = "LDOUBLE";
			break;
		case kTypeDecimal:
			str = "DECIMAL";
			break;

		case kTypeDate:
			str = "DATE";
			break;
		case kTypeTime:
			str = "TIME";
			break;
		case kTypeDateTime:
			str = "DATETIME";
			break;

		case kTypeString:
			str = "STRING";
			break;
		case kTypeVarChar:
			str = "VARCHAR";
			break;

		case kTypeFixedBinary:
			str = "FIXEDBINARY";
			break;
		case kTypeVarBinary:
			str = "VARBINARY";
			break;
		case kTypeVariant:
			str = "VARIANT";
			break;

		case kTypeBLOB:
			str = "BLOB";
			break;
		case kTypeText:
			str = "TEXT";
			break;
		case kTypePicture:
			str = "PICTURE";
			break;
		case kTypeSound:
			str = "SOUND";
			break;
		case kTypeMovie:
			str = "MOVIE";
			break;

		case kTypeRecID:
			str = "RECID";
			break;
		case kTypeOID:
			str = "OID";
			break;

		case kTypeObjectPtr:
			str = "OBJECTPTR";
			break;
		case kTypeObjectsPtr:
			str = "OBJECTSPTR";
			break;

		case kTypeTimeStamp:
			str = "TIMESTAMP";
			break;

		case kTypeEnum8:
			str = "ENUM8";
			break;
		case kTypeEnum16:
			str = "ENUM16";
			break;
			
		case kTypeMoney:
			str = "MONEY";
			break;
	};

	return str;
}


/**********************************************************************************************/
FBL_End_Namespace
