/**********************************************************************************************/
/* FBL_EnumToString.cpp                                                          			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Util_Strings/FBL_EnumToString.h>

#include <VShared/FBL/publ/Headers/FBL.h>


/**********************************************************************************************/
FBL_Begin_Namespace

/**********************************************************************************************/
const char* TypeCategory2String( TYPE_CATEGORY inTypeCategory )
{
	switch( inTypeCategory )
	{
		case vcUnknownType:
			return "vcUnknownType";
		case vcIntegerType:
			return "vcIntegerType";
		case vcUIntegerType:
			return "vcUIntegerType";
		case vcRealType:
			return "vcRealType";
		case vcStringType:
			return "vcStringType";
		case vcDateType:
			return "vcDateType";
		case vcTimeType:
			return "vcTimeType";
		case vcDateTimeType:
			return "vcDateTimeType";
		case vcBinaryType:
			return "vcBinaryType";
		case vcScalarType:
			return "vcScalarType";
		case vcCompositeType:
			return "vcCompositeType";
		case vcEnumeratedType:
			return "vcEnumeratedType";
		case vcCollectionType:
			return "vcCollectionType";
		case vcMoneyType:
			return "vcMoneyType";
		default:
			return nullptr;
	};
}


/**********************************************************************************************/
const char* DatabaseMode2String( DbMode inMode )
{
	switch( inMode )
	{
		case kDscDatBlbInd:
			return "kDscDatBlbInd";
		case kDsc_DatBlbInd:
			return "kDsc_DatBlbInd";
		case kDsc_DatBlb_Ind:
			return "kDsc_DatBlb_Ind";
		case kDsc_Dat_Blb_Ind:
			return "kDsc_Dat_Blb_Ind";
		case kDscDatBlb_Ind:
			return "kDscDatBlb_Ind";
		case kDscDat_Blb_Ind:
			return "kDscDat_Blb_Ind";
		case kDscDatInd_Blb:
			return "kDscDatInd_Blb";
		case kDsc_DatInd_Blb:
			return "kDsc_DatInd_Blb";
		default:
			return nullptr;
	};
}


/**********************************************************************************************/
const char* Bool2String( bool inValue )
{
	return (inValue) ? "true" : "false";
}

/**********************************************************************************************/
const char* StorageType2String( EStorageType inStorageType )
{
	switch( inStorageType )
	{
		case kStorage_Default:
			return "kStorage_Default";		
		case kStorage_Disk:
			return "kStorage_Disk";
		case kStorage_RAM:
			return "kStorage_RAM";
		default:
			return nullptr;
	};
}


/**********************************************************************************************/
const char* DateFormat2String( EDateFormat inDateFormat )
{
	switch( inDateFormat )
	{
		case kMDY:
			return "kMDY";
		case kDMY:
			return "kDMY";
		case kYMD:
			return "kYMD";
		case kMYD:
			return "kMYD";
		case kDYM:
			return "kDYM";
		case kYDM:
			return "kYDM";
		default:
			return nullptr;
	};
}


/**********************************************************************************************/
const char* DumpSortOrder2String( EDumpSortOrder inDumpSortedOrder )
{
	switch( inDumpSortedOrder )
	{
		case kSchemaDefined:
			return "kSchemaDefined";
		case kSorted:
			return "kSorted";
		default:
			return nullptr;
	};
}


/**********************************************************************************************/
const char* ColAttribValue2String( UColAttributeValue inValue )
{
	switch( inValue )
	{
		case UCOL_DEFAULT:
			return "kDefault";
		case UCOL_PRIMARY:
			return "kPrimary";
		case UCOL_SECONDARY:
			return "kSecondary";
		case UCOL_TERTIARY:
			return "kTertiary";
		case UCOL_QUATERNARY:
			return "kQuaternary";
		case UCOL_IDENTICAL:
			return "kIdentical";
		case UCOL_OFF:
			return "kOff";
		case UCOL_ON:
			return "kOn";
		case UCOL_SHIFTED:
			return "kShifted";
		case UCOL_NON_IGNORABLE:
			return "kNonIgnorable";
		case UCOL_LOWER_FIRST:
			return "kLowerFirst";
		case UCOL_UPPER_FIRST:
			return "kUpperFirst";
		default:
			return nullptr;
	};
}


/**********************************************************************************************/
const char* TableKind2String( ETableKind inTableKind )
{
	switch( inTableKind )
	{
		case kTblPermanent:
			return "kTblPermanent";
		case kTblTemporary:
			return "kTblTemporary";
		case kTblSystem:
			return "kTblSystem";
		case kTblResultTable:
			return "kTblResultTable";
		default:
			return nullptr;
	};
}


/**********************************************************************************************/
const char* LinkType2String( LinkType inLinkType )
{
	switch( inLinkType )
	{
		case 0:
			return "Many";
		case 1:
			return "One";
		default:
			return nullptr;
	}
}


/**********************************************************************************************/
const char* OnDelete2String( EOnDeletion inValue )
{
	switch( inValue )
	{
		case kOnDelete_SetNull:
			return "SetNull";
		case kOnDelete_Cascade:
			return "Cascade";
		case kOnDelete_Restrict:
			return "Restrict";
		case kOnDelete_Default:
			return "Default";
		default:
			return nullptr;
	}
}


/**********************************************************************************************/
const char* OnUpdate2String( EOnUpdate inValue )
{
	switch( inValue )
	{
		case kOnUpdate_SetNull:
			return "SetNull";
		case kOnUpdate_Cascade:
			return "Cascade";
		case kOnUpdate_Restrict:
			return "Restrict";
		case kOnUpdate_Default:
			return "Default";
		default:
			return nullptr;
	}
}


/**********************************************************************************************/
const char* LinkKind2String( ELinkKind inValue )
{
	switch( inValue )
	{
		case kObjectPtr:
			return "ObjectPtr";
		case kObjectsPtr:
			return "ObjectsPtr";
		case kBinaryRelation:
			return "BinaryLink";
		case kRdbRelation:
			return "RdbLink";
		case kPredicateRelation:
			return "PredicateLink";
		case kBinaryWithOrderRelation:
			return "BinaryWithOrderLink";
		default:
			return nullptr;
	}
}


#pragma mark - 

/**********************************************************************************************/
TYPE_CATEGORY String2TypeCategory( const char* inStr )
{
	if( inStr == nullptr )
		return 	vcUnknownType;

	// We cannot optimize here...
	if( stricmp(inStr, "vcUnknownType") == 0 )
		return vcUnknownType;
	else if( stricmp(inStr, "vcIntegerType") == 0 )
		return vcIntegerType;
	else if( stricmp(inStr, "vcUIntegerType") == 0 )
		return vcUIntegerType;
	else if( stricmp(inStr, "vcRealType") == 0 )
		return vcRealType;
	else if( stricmp(inStr, "vcStringType") == 0 )
		return vcStringType;
	else if( stricmp(inStr, "vcDateType") == 0 )
		return vcDateType;
	else if( stricmp(inStr, "vcTimeType") == 0 )
		return vcTimeType;
	else if( stricmp(inStr, "vcDateTimeType") == 0 )
		return vcDateTimeType;
	else if( stricmp(inStr, "vcBinaryType") == 0 )
		return vcBinaryType;
	else if( stricmp(inStr, "vcScalarType") == 0 )
		return vcScalarType;
	else if( stricmp(inStr, "vcCompositeType") == 0 )
		return vcCompositeType;
	else if( stricmp(inStr, "vcEnumeratedType") == 0 )
		return vcEnumeratedType;
	else if( stricmp(inStr, "vcCollectionType") == 0 )
		return vcCollectionType;
	else if( stricmp(inStr, "vcMoneyType") == 0 )
		return vcMoneyType;
    
    return vcUnknownType;
}


/**********************************************************************************************/
DbMode String2DatabaseMode( const char* inStr )
{
	if( inStr == nullptr )
		return DbMode(-1);

	// We cannot optimize here...
	if( stricmp(inStr, "kDscDatBlbInd") == 0 )
		return kDscDatBlbInd;
	else if( stricmp(inStr, "kDsc_DatBlbInd") == 0 )
		return kDsc_DatBlbInd;
	else if( stricmp(inStr, "kDsc_DatBlb_Ind") == 0 )
		return kDsc_DatBlb_Ind;
	else if( stricmp(inStr, "kDsc_Dat_Blb_Ind") == 0 )
		return kDsc_Dat_Blb_Ind;
	else if( stricmp(inStr, "kDscDatBlb_Ind") == 0 )
		return kDscDatBlb_Ind;
	else if( stricmp(inStr, "kDscDat_Blb_Ind") == 0 )
		return kDscDat_Blb_Ind;
	else if( stricmp(inStr, "kDscDatInd_Blb") == 0 )
		return kDscDatInd_Blb;
	else if( stricmp(inStr, "kDsc_DatInd_Blb") == 0 )
		return kDsc_DatInd_Blb;

	return DbMode(-1);
}


/**********************************************************************************************/
EDateFormat String2DateFormat( const char* inStr )
{
	if( inStr == nullptr )
		return EDateFormat(-1);

	// We cannot optimize here...
	if( stricmp(inStr, "kMDY") == 0 )
		return kMDY;
	else if( stricmp(inStr, "kDMY") == 0 )
		return kDMY;
	else if( stricmp(inStr, "kYMD") == 0 )
		return kYMD;
	else if( stricmp(inStr, "kMYD") == 0 )
		return kMYD;
	else if( stricmp(inStr, "kDYM") == 0 )
		return kDYM;
	else if( stricmp(inStr, "kYDM") == 0 )
		return kYDM;

	return EDateFormat(-1);
}


/**********************************************************************************************/
EDumpSortOrder String2DumpSortOrder( const char* inStr )
{
	if( inStr == nullptr )
		return EDumpSortOrder(0);

	// We cannot optimize here...
	if( stricmp(inStr, "kSchemaDefined") == 0 )
		return kSchemaDefined;
	else if( stricmp(inStr, "kSorted") == 0 )
		return kSorted;

	return EDumpSortOrder(0);
}


/**********************************************************************************************/
UColAttributeValue String2ColAttribValue( const char* inStr )
{
	if( inStr == nullptr )
		return UColAttributeValue(-1);

	// We cannot optimize here...
	if( stricmp(inStr, "kDefault") == 0 )
		return UCOL_DEFAULT;
	else if( stricmp(inStr, "kPrimary") == 0 )
		return UCOL_PRIMARY;
	else if( stricmp(inStr, "kSecondary") == 0 )
		return UCOL_SECONDARY;
	else if( stricmp(inStr, "kTertiary") == 0 )
		return UCOL_TERTIARY;
	else if( stricmp(inStr, "kQuaternary") == 0 )
		return UCOL_QUATERNARY;
	else if( stricmp(inStr, "kIdentical") == 0 )
		return UCOL_IDENTICAL;
	else if( stricmp(inStr, "kOff") == 0 )
		return UCOL_OFF;
	else if( stricmp(inStr, "kOn") == 0 )
		return UCOL_ON;
	else if( stricmp(inStr, "kShifted") == 0 )
		return UCOL_SHIFTED;
	else if( stricmp(inStr, "kNonIgnorable") == 0 )
		return UCOL_NON_IGNORABLE;
	else if( stricmp(inStr, "kLowerFirst") == 0 )
		return UCOL_LOWER_FIRST;
	else if( stricmp(inStr, "kUpperFirst") == 0 )
		return UCOL_UPPER_FIRST;

	return UColAttributeValue(-1);
}


/**********************************************************************************************/
EStorageType String2StorageType( const char* inStr )
{
	if( inStr == nullptr )
		return kStorage_Default;

	// We cannot optimize here...
	if( stricmp(inStr, "kStorage_Default") == 0 )
		return kStorage_Default;
	else if( stricmp(inStr, "kStorage_Disk") == 0 )
		return kStorage_Disk;
	else if( stricmp(inStr, "kStorage_RAM") == 0 )
		return kStorage_RAM;

	return kStorage_Default;
}


/**********************************************************************************************/
FBL_End_Namespace














