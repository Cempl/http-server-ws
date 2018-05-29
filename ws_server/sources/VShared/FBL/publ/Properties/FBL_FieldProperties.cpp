/**********************************************************************************************/
/* FBL_Property.cpp                                                      					  */
/*                                                                        					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Properties/FBL_FieldProperties.h>


/**********************************************************************************************/
FBL_Begin_Namespace


#pragma mark -


/**********************************************************************************************/
Prop_MaxLen::Prop_MaxLen( vuint32 inMaxLen )
:
	PropertyLong( "maxlen", (vint32) inMaxLen )
{
}


/**********************************************************************************************/
Prop_MaxLen::~Prop_MaxLen( void ) 
{
}


#pragma mark -

/**********************************************************************************************/
Prop_SegmentSize::Prop_SegmentSize( vuint32 inSegmentSize )
:
	PropertyLong( String("segmentSize"), (vint32)inSegmentSize )
{
}


/**********************************************************************************************/
Prop_SegmentSize::~Prop_SegmentSize( void ) 
{
}


#pragma mark -

/**********************************************************************************************/
Prop_Precision::Prop_Precision( vuint16 inPrecision )
:
	PropertyLong( String("Precision"), (vint32)inPrecision )
{
}


/**********************************************************************************************/
Prop_Precision::~Prop_Precision( void ) 
{
}


/**********************************************************************************************/
Prop_Scale::Prop_Scale( vuint16 inScale )
:
	PropertyLong( String("Scale"), (vint32)inScale )
{
}


/**********************************************************************************************/
Prop_Scale::~Prop_Scale( void ) 
{
}


#pragma mark -

/**********************************************************************************************/
Prop_Method::Prop_Method( 
	const UChar* inMethodText,
	TMETHOD_LANGUAGE inLanguage )
:
	PropertyString( String("METHOD_TEXT"), inMethodText ) 								
{
	mLanguage = inLanguage;
}


/**********************************************************************************************/
Prop_Method::Prop_Method( 
	const String&	inMethodText,
	TMETHOD_LANGUAGE inLanguage )
:
	PropertyString( String("METHOD_TEXT"), inMethodText ) 								
{
	mLanguage = inLanguage;
}


/**********************************************************************************************/
Prop_Method::~Prop_Method( void )
{
}

#pragma mark -


/**********************************************************************************************/
Prop_MethodSql::Prop_MethodSql( 
	const UChar* inMethodText )
:
	Prop_Method( inMethodText, kMethod_SQL )
{
}


/**********************************************************************************************/
Prop_MethodSql::Prop_MethodSql( 
	const String& inMethodText )
:
	Prop_Method( inMethodText, kMethod_SQL )
{
}


/**********************************************************************************************/
Prop_MethodSql::~Prop_MethodSql( void )
{
}

#pragma mark -


/**********************************************************************************************/
Prop_Target::Prop_Target( 
	I_Table_Ptr inTarget )
:
	PropertyIUnknown( String("Target"), inTarget )
{
}


/**********************************************************************************************/
Prop_Target::~Prop_Target( void )
{
}


#pragma mark -


/**********************************************************************************************/
Prop_Encoding::Prop_Encoding( 
	const UChar* inEncoding )
:
	PropertyString( String("Encoding"), inEncoding )
{
}


/**********************************************************************************************/
Prop_Encoding::~Prop_Encoding( void )
{
}


#pragma mark -


/**********************************************************************************************/
Prop_LocaleName::Prop_LocaleName( 
	const UChar* inLocale )
:
	PropertyString( String("LocaleName"), inLocale )
{
}


/**********************************************************************************************/
Prop_LocaleName::Prop_LocaleName( 
	const String& inLocale )
:
	PropertyString( String("LocaleName"), inLocale )
{
} 							


/**********************************************************************************************/
Prop_LocaleName::~Prop_LocaleName( void )
{
}


#pragma mark -
#pragma mark === Link On Values ===


/**********************************************************************************************/
Prop_Key::Prop_Key( I_Field_Ptr inKey ):
	PropertyIUnknown( "Key", inKey )
{
}


/**********************************************************************************************/
Prop_Key::~Prop_Key( void )
{
}


/**********************************************************************************************/
Prop_Ptr::Prop_Ptr( I_Field_Ptr inPtr ):
	PropertyIUnknown( "Ptr", inPtr )
{
}


/**********************************************************************************************/
Prop_Ptr::~Prop_Ptr( void )
{
}


#pragma mark -


/**********************************************************************************************/
Prop_OnDeletion::Prop_OnDeletion( EOnDeletion inOnDeletion ):
	PropertyLong( "OnDeletion", (vint32)inOnDeletion )
{
}


/**********************************************************************************************/
Prop_OnDeletion::~Prop_OnDeletion( void )
{
}


/**********************************************************************************************/
Prop_OnUpdate::Prop_OnUpdate( EOnUpdate inOnUpdate ) :
	PropertyLong( "OnUpdate", (vint32)inOnUpdate )
{
}


/**********************************************************************************************/
Prop_OnUpdate::~Prop_OnUpdate( void )
{
}


/**********************************************************************************************/
I_PropertyContainer_Ptr Create_Link_On_Value_Properties(
	I_Field_Ptr		inKey,
	I_Field_Ptr		inPtr,
	EOnDeletion		inOnDeletion,
	EOnUpdate		inOnUpdate )
{
	I_PropertyContainer_Ptr props = new PropertyContainer();
	props->Add( new Prop_Key( inKey ) );
	props->Add( new Prop_Ptr( inPtr ) );
	props->Add( new Prop_OnDeletion( inOnDeletion ) );
	props->Add( new Prop_OnUpdate( inOnUpdate ) );

	return props;
}


#pragma mark -
#pragma mark === RDB Link ===


/**********************************************************************************************/
Prop_RDB_Keys::Prop_RDB_Keys( ArrayOfFields_Ptr inKeys ):
	PropertyIUnknown( "Keys", inKeys )
{
}


/**********************************************************************************************/
Prop_RDB_Keys::~Prop_RDB_Keys( void )
{
}


/**********************************************************************************************/
Prop_RDB_Ptrs::Prop_RDB_Ptrs( ArrayOfFields_Ptr inPtrs ):
	PropertyIUnknown( "Ptrs", inPtrs )
{
}


/**********************************************************************************************/
Prop_RDB_Ptrs::~Prop_RDB_Ptrs( void )
{
}


/**********************************************************************************************/
I_PropertyContainer_Ptr Create_Link_On_Values_N_Properties(
	ArrayOfFields_Ptr	inKeys,
	ArrayOfFields_Ptr	inPtrs,
	EOnDeletion			inOnDeletion,
	EOnUpdate			inOnUpdate
)
{
	I_PropertyContainer_Ptr props = new PropertyContainer();
	props->Add( new Prop_RDB_Keys( inKeys ) );
	props->Add( new Prop_RDB_Ptrs( inPtrs ) );
	props->Add( new Prop_OnDeletion( inOnDeletion ) );
	props->Add( new Prop_OnUpdate( inOnUpdate ) );

	return props;
}


#pragma mark -
#pragma mark === Constraints ===


/**********************************************************************************************/
Prop_IndexType::Prop_IndexType( EIndexType inIndexType ):
	PropertyLong( "IndexType", (vint32)inIndexType )
{
}


/**********************************************************************************************/
Prop_IndexType::~Prop_IndexType( void )
{
}


#pragma mark -


/**********************************************************************************************/
Prop_PrimaryKey::Prop_PrimaryKey( I_Field_Ptr inField )
:
	PropertyIUnknown( "PrimaryKey", inField )
{
}


/**********************************************************************************************/
Prop_PrimaryKey::Prop_PrimaryKey( ArrayOfFields_Ptr	inFields )
:
	PropertyIUnknown( "PrimaryKey", inFields )
{
}


/**********************************************************************************************/
Prop_PrimaryKey::~Prop_PrimaryKey( void )
{
}


#pragma mark -


/**********************************************************************************************/
Prop_Unique::Prop_Unique( I_Field_Ptr inField )
:
	PropertyIUnknown( "Unique", inField )
{
}


/**********************************************************************************************/
Prop_Unique::Prop_Unique( ArrayOfFields_Ptr	inFields )
:
	PropertyIUnknown( "Unique", inFields )
{
}


/**********************************************************************************************/
Prop_Unique::~Prop_Unique( void )
{
}


#pragma mark -


/**********************************************************************************************/
Prop_Indexed::Prop_Indexed( I_Field_Ptr inField )
:
	PropertyIUnknown( "Index", inField )
{
}


/**********************************************************************************************/
Prop_Indexed::Prop_Indexed( ArrayOfFields_Ptr	inFields )
:
	PropertyIUnknown( "Index", inFields )
{
}


/**********************************************************************************************/
Prop_Indexed::~Prop_Indexed( void )
{
}


#pragma mark -


/**********************************************************************************************/
// Foreign key properties.
I_PropertyContainer_Ptr Create_ForeignKey_Properties(
	ArrayOfFields_Ptr	inKeys,
	ArrayOfFields_Ptr	inPtrs,
	EOnDeletion			inOnDeletion,
	EOnUpdate			inOnUpdate )
{
	return Create_Link_On_Values_N_Properties( inKeys, inPtrs, inOnDeletion, inOnUpdate );
}


/**********************************************************************************************/
I_PropertyContainer_Ptr Create_ForeignKey_Properties(
	I_Field_Ptr		inKey,
	I_Field_Ptr		inPtr,
	EOnDeletion		inOnDeletion,
	EOnUpdate		inOnUpdate )
{
	return Create_Link_On_Value_Properties( inKey, inPtr, inOnDeletion, inOnUpdate );
}


#pragma mark -


/**********************************************************************************************/
I_PropertyContainer_Ptr Create_Binary_Link_Properties(
	I_Table_Ptr		inLeftTable,
	I_Table_Ptr		inRightTable,
	LinkType		inLeftType,
	LinkType		inRightType,
	EOnDeletion		inOnDelete,
	EStorageType	inStorageType,
	ELinkStorageKind inStorageKind )
{
	I_PropertyContainer_Ptr props = new PropertyContainer();
	props->Add( new Prop_LeftTable( inLeftTable ) );
	props->Add( new Prop_RightTable( inRightTable ) );
	props->Add( new PropertyLong( "LeftType", inLeftType ) );
	props->Add( new PropertyLong( "RightType", inRightType ) );
	props->Add( new Prop_OnDeletion( inOnDelete ) );
	props->Add( new PropertyLong( "StorageType", static_cast<vint32>(inStorageType)) );
	props->Add( new PropertyLong( "StorageKind", static_cast<vint32>(inStorageKind)) );

	return props;
}


#pragma mark -


/**********************************************************************************************/
Prop_LeftTable::Prop_LeftTable( I_Table_Ptr inPredicate )
	:
	PropertyIUnknown( "LeftTable", inPredicate )
{
}


/**********************************************************************************************/
Prop_LeftTable::~Prop_LeftTable( void )
{
}


#pragma mark -


/**********************************************************************************************/
Prop_RightTable::Prop_RightTable( I_Table_Ptr inPredicate )
	:
	PropertyIUnknown( "RightTable", inPredicate )
{
}


/**********************************************************************************************/
Prop_RightTable::~Prop_RightTable( void )
{
}


#pragma mark -

/**********************************************************************************************/
Prop_IsSingleByte::Prop_IsSingleByte( bool inValue )
:
	PropertyBoolean(String("IsSingleByte"), inValue)
{
}


/**********************************************************************************************/
Prop_IsSingleByte::~Prop_IsSingleByte( void )
{
}


#pragma mark -

/**********************************************************************************************/
Prop_LinkName::Prop_LinkName( const UChar* inLinkName )
:
	PropertyString( String("LINK_NAME"), inLinkName ) 								
{
}


/**********************************************************************************************/
Prop_LinkName::Prop_LinkName( const String&	inLinkName )
:
	PropertyString( String("LINK_NAME"), inLinkName ) 								
{
}


/**********************************************************************************************/
Prop_LinkName::~Prop_LinkName( void )
{
}


#pragma mark -

/**********************************************************************************************/
Prop_EnumType::Prop_EnumType( I_Type_Enumerated_Ptr inpType )
	:
PropertyIUnknown( String("EnumType"), inpType )
{
}


/**********************************************************************************************/
Prop_EnumType::~Prop_EnumType( void )
{
}


/**********************************************************************************************/
FBL_End_Namespace
