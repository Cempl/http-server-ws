/**********************************************************************************************/
/* VSDK_Objects.cpp			                                                   				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

// On default we do not specify here stdafx.h
// So we set "Not Using Precompiled Header" in the properties of this file.
// You can change this in your projects.
// #include <stdafx.h>

#include <Examples/Shared/stdafx.h>
#include <VSDK/VSDK2.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// Constructor of VDK_Database and VDK_Table set these pointers to this,
// so following constructors of sub-objects (Tables and Fields) can use this as 
// the CURRENT super-object.
// 
// NOTE: this will work if creation of hierarchy of objects happens by single thread.
// To provide thread-safe solution here it is possible
// a) protect these two pointers by mutex, which lock/unlock developer
// b) add the VDK_Table and VDK_Field constructors that simply get as the first parameter 
//		a supervisor object (then syntax will be less ellegant).
// c) else?
//
FBL::I_Database*	gVDK_Database	= NULL;
FBL::I_Table*		gVDK_Table		= NULL;


/**********************************************************************************************/
VDK_Database::VDK_Database( EStorageType 	inStorage )
:
	mpSqlDatabase( VSQL::CreateSqlDatabase( inStorage ) ),
	mpFblDatabase( mpSqlDatabase->get_BaseDatabase().get() )
{
	gVDK_Database = mpFblDatabase;
}


/**********************************************************************************************/
VDK_Database::VDK_Database( I_Connection_Ptr	inpConnection )
:
	mpSqlDatabase( VCLIENT::CreateSqlDatabase( inpConnection ) ),
	mpFblDatabase( mpSqlDatabase->get_BaseDatabase().get() )
{
	gVDK_Database = mpFblDatabase;
}


#pragma mark -


/**********************************************************************************************/
VDK_Table::VDK_Table( const String& inName ) :
	mpTable( gVDK_Database->CreateTable( inName ) )
{	
	gVDK_Table = mpTable.get();
}


#pragma mark -


/**********************************************************************************************/
VDK_Field::VDK_Field( 
	 I_Field_Ptr inFld ) 
:
	mpField( inFld )
{		
}


#pragma mark -


/**********************************************************************************************/
// HELPER MACRO to implement many numeric filed types.
//
#define VDK_NUMERIC_CLASS_IMPL(cname)												\
VDK_##cname::VDK_##cname(															\
	const String&	inName,															\
	FieldAttributes inFlags,														\
	const UChar*	inMethodText)													\
:																					\
	VDK_Field( Create##cname##Field( gVDK_Table, inName, inFlags, inMethodText ) )	\
{																					\
}

/**********************************************************************************************/
VDK_NUMERIC_CLASS_IMPL( Boolean	)
VDK_NUMERIC_CLASS_IMPL( Byte	)
VDK_NUMERIC_CLASS_IMPL( Short	)
VDK_NUMERIC_CLASS_IMPL( UShort	)
VDK_NUMERIC_CLASS_IMPL( Medium	)
VDK_NUMERIC_CLASS_IMPL( UMedium	)
VDK_NUMERIC_CLASS_IMPL( Long	)
VDK_NUMERIC_CLASS_IMPL( ULong	)
VDK_NUMERIC_CLASS_IMPL( LLong	)
VDK_NUMERIC_CLASS_IMPL( ULLong	)
VDK_NUMERIC_CLASS_IMPL( Float	)
VDK_NUMERIC_CLASS_IMPL( Double	)

VDK_NUMERIC_CLASS_IMPL( Date	)
VDK_NUMERIC_CLASS_IMPL( Time	)
VDK_NUMERIC_CLASS_IMPL( DateTime )

#pragma mark -


/**********************************************************************************************/
VDK_String::VDK_String( 
	const String&	inName,
	vuint32			inMaxLength, 
	FieldAttributes inFlags,
	const UChar*	inMethodText ) 
:
	VDK_Field( CreateStringField( gVDK_Table, inName, inMaxLength, inFlags, inMethodText ) )
,
	mpFldString(NULL)
{	
}

/**********************************************************************************************/
FBL::I_FldString_Ptr VDK_String::ToIString( void )
{
	if( !mpFldString )
		mpFldString = dynamic_cast<I_FldString*>( mpField.get() );

	return mpFldString;
}

/**********************************************************************************************/
FBL::Const_I_FldString_Ptr	VDK_String::ToIString( void ) const
{
	if( !mpFldString )
		mpFldString = dynamic_cast<I_FldString*>( mpField.get() );

	return mpFldString;
}

#pragma mark -


/**********************************************************************************************/
VDK_VarChar::VDK_VarChar( 
	const String&	inName,
	vuint32			inMaxLength, 
	FieldAttributes inFlags,
	const UChar*	inMethodText ) 
:
	VDK_Field( CreateVarCharField( gVDK_Table, inName, inMaxLength, inFlags, inMethodText ) )
,
	mpFldString(NULL)
{	
}


/**********************************************************************************************/
FBL::I_FldString_Ptr VDK_VarChar::ToIString( void )
{
	if( !mpFldString )
		mpFldString = dynamic_cast<I_FldString*>( mpField.get() );

	return mpFldString;
}

/**********************************************************************************************/
FBL::Const_I_FldString_Ptr	VDK_VarChar::ToIString( void ) const
{
	if( !mpFldString )
		mpFldString = dynamic_cast<I_FldString*>( mpField.get() );

	return mpFldString;
}


#pragma mark -


/**********************************************************************************************/
VDK_Text::VDK_Text( 
	const String&	inName,
	vuint32			inSegmentSize, 
	FieldAttributes inFlags,
	const UChar*	inMethodText ) 
:
	VDK_Field( CreateTextField( gVDK_Table, inName, inSegmentSize, inFlags, inMethodText ) )
,
	mpFldString(NULL),
	mpFldBlob(NULL)
{	
}

/**********************************************************************************************/
FBL::I_FldString_Ptr VDK_Text::ToIString( void )
{
	if( !mpFldString )
		mpFldString = dynamic_cast<I_FldString*>( mpField.get() );

	return mpFldString;
}

/**********************************************************************************************/
FBL::Const_I_FldString_Ptr	VDK_Text::ToIString( void ) const
{
	if( !mpFldString )
		mpFldString = dynamic_cast<I_FldString*>( mpField.get() );

	return mpFldString;
}


/**********************************************************************************************/
FBL::I_FldBlob_Ptr VDK_Text::ToIBlob( void )
{
	if( !mpFldBlob )
		mpFldBlob = dynamic_cast<I_FldBlob*>( mpField.get() );

	return mpFldBlob;
}

/**********************************************************************************************/
FBL::Const_I_FldBlob_Ptr VDK_Text::ToIBlob( void ) const
{
	if( !mpFldBlob )
		mpFldBlob = dynamic_cast<I_FldBlob*>( mpField.get() );

	return mpFldBlob;
}


#pragma mark -


/**********************************************************************************************/
VDK_BLOB::VDK_BLOB( 
	const String&	inName,
	vuint32			inSegmentSize ) 
:
	VDK_Field( CreateBLOBField( gVDK_Table, inName, inSegmentSize ) )
,
	mpFldBlob(NULL)
{	
}

/**********************************************************************************************/
FBL::I_FldBlob_Ptr VDK_BLOB::ToIBlob( void )
{
	if( !mpFldBlob )
		mpFldBlob = dynamic_cast<I_FldBlob*>( mpField.get() );

	return mpFldBlob;
}

/**********************************************************************************************/
FBL::Const_I_FldBlob_Ptr VDK_BLOB::ToIBlob( void ) const
{
	if( !mpFldBlob )
		mpFldBlob = dynamic_cast<I_FldBlob*>( mpField.get() );

	return mpFldBlob;
}


#pragma mark -


/**********************************************************************************************/
VDK_Picture::VDK_Picture( 
	const String&	inName,
	vuint32			inSegmentSize ) 
:
	VDK_Field( CreatePictureField( gVDK_Table, inName, inSegmentSize ) )
,
	mpFldBlob(NULL)
{	
}


/**********************************************************************************************/
FBL::I_FldBlob_Ptr VDK_Picture::ToIBlob( void )
{
	if( !mpFldBlob )
		mpFldBlob = dynamic_cast<I_FldBlob*>( mpField.get() );

	return mpFldBlob;
}

/**********************************************************************************************/
FBL::Const_I_FldBlob_Ptr VDK_Picture::ToIBlob( void ) const
{
	if( !mpFldBlob )
		mpFldBlob = dynamic_cast<I_FldBlob*>( mpField.get() );

	return mpFldBlob;
}


/**********************************************************************************************/
FBL::I_FldPicture_Ptr VDK_Picture::ToIPicture( void )
{
	if( !mpFldPicture )
		mpFldPicture = dynamic_cast<I_FldPicture*>( mpField.get() );

	return mpFldPicture;
}

/**********************************************************************************************/
FBL::Const_I_FldPicture_Ptr VDK_Picture::ToIPicture( void ) const
{
	if( !mpFldPicture )
		mpFldPicture = dynamic_cast<I_FldPicture*>( mpField.get() );

	return mpFldPicture;
}


#pragma mark -


/**********************************************************************************************/
VDK_ObjectPtr::VDK_ObjectPtr( 
	const String&	inName,
	VDK_Table*		inTarget,
	EOnDeletion		inOnDeletion,
	FieldAttributes inFlags,
	const String&	inLinkName ) 
:
	VDK_Field( CreateObjectPtr( 
					gVDK_Table, inName, inTarget->get(), inOnDeletion, inFlags, inLinkName ) )
,
	mpFldObjectPtr(NULL)
{	
}

/**********************************************************************************************/
FBL::I_FldObjectPtr_Ptr VDK_ObjectPtr::ToIObjectPtr( void )
{
	if( !mpFldObjectPtr )
		mpFldObjectPtr = dynamic_cast<I_FldObjectPtr*>( mpField.get() );

	return mpFldObjectPtr;
}

/**********************************************************************************************/
FBL::Const_I_FldObjectPtr_Ptr VDK_ObjectPtr::ToIObjectPtr( void ) const
{
	if( !mpFldObjectPtr )
		mpFldObjectPtr = dynamic_cast<I_FldObjectPtr*>( mpField.get() );

	return mpFldObjectPtr;
}

#pragma mark -


/**********************************************************************************************/
VDK_BinaryLink::VDK_BinaryLink( 
	const String&	inName,
	VDK_Table&		inLeftTable,
	VDK_Table&		inRightTable,
	LinkType		inLeftPower,
	LinkType		inRightPower,
	EOnDeletion		inOnDelete,
	EStorageType	inStorageType )
:
	mpLink2(NULL)
{
	// props:			
	I_PropertyContainer_Ptr props;
	
	props = Create_Binary_Link_Properties( 
				inLeftTable.get(),		inRightTable.get(), 
				LinkType(inLeftPower),	LinkType(inRightPower), 
				EOnDeletion(inOnDelete), 
				EStorageType(inStorageType) );

	// create
	mpLink = gVDK_Database->CreateLink( 
							inName, kBinaryRelation, props, false );
}

/**********************************************************************************************/
FBL::I_Link2_Ptr VDK_BinaryLink::ToILink2( void )
{
	if( !mpLink2 )
		mpLink2 = dynamic_cast<I_Link2*>( mpLink.get() );

	return mpLink2;
}

/**********************************************************************************************/
FBL::Const_I_Link2_Ptr VDK_BinaryLink::ToILink2( void ) const
{
	if( !mpLink2 )
		mpLink2 = dynamic_cast<I_Link2*>( mpLink.get() );

	return mpLink2;
}


/**********************************************************************************************/
FBL_End_Namespace