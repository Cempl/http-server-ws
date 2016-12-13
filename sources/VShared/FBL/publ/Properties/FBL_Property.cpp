/**********************************************************************************************/
/* FBL_Property.cpp                                                      					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Properties/FBL_Property.h>
#include <VShared/FBL/publ/Util_Strings/FBL_ustdio.h>

#include <VShared/FBL/publ/Errors/FBL_Exceptions.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_RemoteProxy.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_Connectable.h>




#ifdef _MSC_VER
#pragma warning(disable: 4312)
#endif //_MSC_VER


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// Class implements base functionality for all properties.
//
Property_Imp::Property_Imp( const UChar* inName )
:
	mName(inName)
{
}


/**********************************************************************************************/
Property_Imp::Property_Imp( const String& inName )
:
	mName(inName)
{
}


/**********************************************************************************************/
Property_Imp::~Property_Imp( void )
{
}


#pragma mark -


/**********************************************************************************************/
const String& Property_Imp::get_Name( void ) const 
{
	return mName;
}


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning( disable : 4702 )
#endif
/**********************************************************************************************/
vint32 Property_Imp::get_Long( void ) const
{
	THROW_ERROR_INTERNAL_1( ERR_INTERNAL_FUNC_MUST_NOT_BE_CALLED, "Property_Imp::get_Long" );
	return 0;
}
#ifdef _MSC_VER
#pragma warning(pop)
#endif


/**********************************************************************************************/
void Property_Imp::put_Long( vint32 inValue )
{
	argused1( inValue );
	THROW_ERROR_INTERNAL_1( ERR_INTERNAL_FUNC_MUST_NOT_BE_CALLED, "Property_Imp::put_Long" );
}


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning( disable : 4702 )
#endif
/**********************************************************************************************/
bool Property_Imp::get_Boolean( void ) const
{
	THROW_ERROR_INTERNAL_1( ERR_INTERNAL_FUNC_MUST_NOT_BE_CALLED, "Property_Imp::get_Boolean" );
	return false;
}
#ifdef _MSC_VER
#pragma warning(pop)
#endif


/**********************************************************************************************/
void Property_Imp::put_Boolean( bool inValue )
{
	argused1( inValue );
	THROW_ERROR_INTERNAL_1( ERR_INTERNAL_FUNC_MUST_NOT_BE_CALLED, "Property_Imp::put_Boolean" );
}


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning( disable : 4702 )
#endif
/**********************************************************************************************/
const String& Property_Imp::get_String( void ) const
{
	THROW_ERROR_INTERNAL_1( ERR_INTERNAL_FUNC_MUST_NOT_BE_CALLED, "Property_Imp::get_String" );
	return String::sEmpty();
}
#ifdef _MSC_VER
#pragma warning(pop)
#endif


/**********************************************************************************************/
void Property_Imp::put_String( const UChar* inValue )
{
	argused1( inValue );
	THROW_ERROR_INTERNAL_1( ERR_INTERNAL_FUNC_MUST_NOT_BE_CALLED, "Property_Imp::put_String" );
}


/**********************************************************************************************/
void Property_Imp::put_String( const String& inValue )
{
	argused1( inValue );
	THROW_ERROR_INTERNAL_1( ERR_INTERNAL_FUNC_MUST_NOT_BE_CALLED, "Property_Imp::put_String" );
}


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning( disable : 4702 )
#endif
/**********************************************************************************************/
I_Unknown_Ptr Property_Imp::get_I_Unknown( void ) const
{
	THROW_ERROR_INTERNAL_1( ERR_INTERNAL_FUNC_MUST_NOT_BE_CALLED, "Property_Imp::get_I_Unknown" );
	return nullptr;
}
#ifdef _MSC_VER
#pragma warning(pop)
#endif


/**********************************************************************************************/
void Property_Imp::put_I_Unknown( I_Unknown_Ptr inValue )
{
	argused1( inValue );
	THROW_ERROR_INTERNAL_1( ERR_INTERNAL_FUNC_MUST_NOT_BE_CALLED, "Property_Imp::put_I_Unknown" );
}


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning( disable : 4702 )
#endif
/**********************************************************************************************/
const I_PropertyContainer_Ptr Property_Imp::get_Collection( void ) const
{
	THROW_ERROR_INTERNAL_1( ERR_INTERNAL_FUNC_MUST_NOT_BE_CALLED, "Property_Imp::get_Collection" );
	return nullptr;
}
#ifdef _MSC_VER
#pragma warning(pop)
#endif


/**********************************************************************************************/
void Property_Imp::put_Collection( const I_PropertyContainer_Ptr inValue )
{
	argused1( inValue );
	THROW_ERROR_INTERNAL_1( ERR_INTERNAL_FUNC_MUST_NOT_BE_CALLED, "Property_Imp::put_Collection" );
}


/**********************************************************************************************/
void Property_Imp::To( I_OStream_Ptr inStream, bool inBlock ) const
{
	argused1(inBlock);
	(*inStream.get()) << get_Type();
}


/**********************************************************************************************/
void Property_Imp::To( I_PacketSnd* inPacket, bool inBlock ) const
{
	argused1(inBlock);
	(*inPacket) << get_Type();
}


#pragma mark -


/**********************************************************************************************/
// class implements boolean property
//
/**********************************************************************************************/
PropertyBoolean::PropertyBoolean( const UChar* inName )
:
	Property_Imp( inName )
{
}


/**********************************************************************************************/
PropertyBoolean::PropertyBoolean( const String& inName )
:
	Property_Imp( inName )
{
}


/**********************************************************************************************/
PropertyBoolean::PropertyBoolean( const String& inName, bool inValue )
:
	Property_Imp( inName )
,
	mValue(inValue)
{
}


/**********************************************************************************************/
PropertyBoolean::~PropertyBoolean( void )
{
}


#pragma mark -


/**********************************************************************************************/
I_Property_Ptr PropertyBoolean::Clone( void ) const 
{
	PropertyBoolean_Ptr cloned = new PropertyBoolean( Property_Imp::get_Name().c_str(), mValue );
	return cloned;
}


/**********************************************************************************************/
PROPERTY_TYPE PropertyBoolean::get_Type( void ) const 
{
	return TYPE_BOOLEAN;
}


/**********************************************************************************************/
bool PropertyBoolean::get_Boolean( void ) const
{
	return mValue;
}


/**********************************************************************************************/
void PropertyBoolean::put_Boolean( bool inValue )
{
	mValue = inValue;
}


/**********************************************************************************************/
const String& PropertyBoolean::get_String( void ) const
{
	static String one( "1" );
	static String zero( "0" );

	return mValue ? one : zero;
}


/**********************************************************************************************/
void PropertyBoolean::From( I_IStream_Ptr inStream, bool inBlock )
{
	fixme_argused1(inBlock);
	(*inStream.get()) >> mValue;
}


/**********************************************************************************************/
void PropertyBoolean::From( I_PacketRcv* inPacket, bool inBlock )
{
	fixme_argused1(inBlock);
	mValue = inPacket->get_BoolParam();
}


/**********************************************************************************************/
void PropertyBoolean::To( I_OStream_Ptr inStream, bool inBlock ) const
{
	Property_Imp::To(inStream, inBlock);
	(*inStream.get()) << mValue;
}

/**********************************************************************************************/
void PropertyBoolean::To( I_PacketSnd* inPacket, bool inBlock ) const
{
	argused1(inBlock);

	inPacket->put_BoolParam( mValue );
}


#pragma mark -

 
/**********************************************************************************************/
// class implements vint32 property
//
/**********************************************************************************************/
PropertyLong::PropertyLong( const UChar* inName )
:
	Property_Imp( inName )
{
}


/**********************************************************************************************/
PropertyLong::PropertyLong( const String& inName )
:
	Property_Imp( inName )
{
}


/**********************************************************************************************/
PropertyLong::PropertyLong( const String& inName, vint32 inValue )
:
	Property_Imp( inName )
,
	mValue( inValue )
{
}


/**********************************************************************************************/
PropertyLong::~PropertyLong( void )
{
}


#pragma mark -


/**********************************************************************************************/
I_Property_Ptr PropertyLong::Clone( void ) const 
{
	PropertyLong_Ptr cloned = new PropertyLong( Property_Imp::get_Name(), mValue );

	return cloned;
}


/**********************************************************************************************/
PROPERTY_TYPE PropertyLong::get_Type( void ) const 
{
	return TYPE_LONG;
}


/**********************************************************************************************/
vint32 PropertyLong::get_Long( void ) const
{
	return mValue;
}


/**********************************************************************************************/
void PropertyLong::put_Long( vint32 inValue )
{
	mValue = inValue;
}

/**********************************************************************************************/
void PropertyLong::From( I_IStream_Ptr inStream, bool inBlock )
{
	fixme_argused1(inBlock);
	(*inStream.get()) >> mValue;
}


/**********************************************************************************************/
void PropertyLong::From( I_PacketRcv* inPacket, bool inBlock )
{
	fixme_argused1(inBlock);
	mValue = inPacket->get_LongParam();
}


/**********************************************************************************************/
void PropertyLong::To( I_OStream_Ptr inStream, bool inBlock ) const
{
	Property_Imp::To(inStream, inBlock);
	(*inStream.get()) << mValue;
}


/**********************************************************************************************/
void PropertyLong::To( I_PacketSnd* inPacket, bool inBlock ) const
{
	argused1(inBlock);

	inPacket->put_LongParam( mValue );
}


#pragma mark -


/**********************************************************************************************/
// class implements string property
//
/**********************************************************************************************/
PropertyString::PropertyString( const UChar* inName )
:
	Property_Imp( inName )
{
}


/**********************************************************************************************/
PropertyString::PropertyString( const String& inName )
:
	Property_Imp( inName )
{
}


/**********************************************************************************************/
PropertyString::PropertyString( const String& inName, const String& inValue )
:
	Property_Imp( inName )
,
	mValue(inValue)
{
}


/**********************************************************************************************/
PropertyString::~PropertyString( void )
{
}


#pragma mark -


/**********************************************************************************************/
I_Property_Ptr PropertyString::Clone( void ) const 
{
	return new PropertyString( Property_Imp::get_Name(), mValue.c_str() );
}


/**********************************************************************************************/
PROPERTY_TYPE PropertyString::get_Type( void ) const 
{
	return TYPE_STRING;
}


/**********************************************************************************************/
const String& PropertyString::get_String( void ) const
{
	return mValue;
}


/**********************************************************************************************/
void PropertyString::put_String( const UChar* inValue )
{
	if( inValue )
		mValue = inValue;
}


/**********************************************************************************************/
void PropertyString::put_String( const String& inValue )
{
	mValue = inValue;
}


/**********************************************************************************************/
void PropertyString::From( I_IStream_Ptr inStream, bool inBlock )
{
	fixme_argused1(inBlock);
	(*inStream.get()) >> mValue;
}


/**********************************************************************************************/
void PropertyString::From( I_PacketRcv* inPacket, bool inBlock )
{
	FBL_CHECK(inPacket);
	fixme_argused1(inBlock);
	inPacket->get_StringParam( mValue );
}


/**********************************************************************************************/
void PropertyString::To( I_OStream_Ptr inStream, bool inBlock ) const
{
	Property_Imp::To(inStream, inBlock);
	(*inStream.get()) << mValue;
}


/**********************************************************************************************/
void PropertyString::To( I_PacketSnd* inPacket, bool inBlock ) const
{
	FBL_CHECK(inPacket);
	argused1(inBlock);
	inPacket->put_StringParam( mValue );
}


#pragma mark -


/**********************************************************************************************/
// class implements I_Unknown property
//
/**********************************************************************************************/
PropertyIUnknown::PropertyIUnknown( const UChar* inName )
	:
Property_Imp( inName )
{
}


/**********************************************************************************************/
PropertyIUnknown::PropertyIUnknown( const String& inName )
	:
Property_Imp( inName )
{
}


/**********************************************************************************************/
PropertyIUnknown::PropertyIUnknown( 
	const String&	inName, 
	I_Unknown_Ptr	inValue )
:
	Property_Imp( inName ),
	mValue( inValue )
{
}


/**********************************************************************************************/
PropertyIUnknown::~PropertyIUnknown( void )
{
}


#pragma mark -


/**********************************************************************************************/
I_Property_Ptr PropertyIUnknown::Clone( void ) const 
{
	PropertyIUnknown_Ptr cloned = new PropertyIUnknown( Property_Imp::get_Name(), mValue );

	return cloned;
}


/**********************************************************************************************/
PROPERTY_TYPE PropertyIUnknown::get_Type( void ) const 
{
	return TYPE_I_UNKNOWN;
}


/**********************************************************************************************/
I_Unknown_Ptr PropertyIUnknown::get_I_Unknown( void ) const
{
	return mValue;
}


/**********************************************************************************************/
void PropertyIUnknown::put_I_Unknown( I_Unknown_Ptr inValue )
{
	mValue = inValue;
}


/**********************************************************************************************/
void PropertyIUnknown::From( I_IStream_Ptr inStream, bool inBlock )
{
	argused2(inStream, inBlock);
	FBL_CHECK(false);
	// IS: 24.11.2009
	// Not implemented - because it is vShared.dll but ProxyPtr might be
	// different for Client and Server. And we MUST NOT implement such
	// code in vShared. See FBL_Types.h
/*
	argused1(inBlock);

	FBL_CHECK( inStream );
	if( inStream )
	{
		ProxyPtr remoteId;
		*inStream >> remoteId;

		I_RemoteProxy* proxy = reinterpret_cast<I_RemoteProxy*>(remoteId);
		mValue = proxy->get_I_Unknown();
	}
*/
}


/**********************************************************************************************/
void PropertyIUnknown::From( I_PacketRcv* inPacket, bool inBlock )
{
	argused2(inPacket, inBlock);
	FBL_CHECK(false);
	// IS: 24.11.2009
	// Not implemented - because it is vShared.dll but ProxyPtr might be
	// different for Client and Server. And we MUST NOT implement such
	// code in vShared. See FBL_Types.h
/*
	argused1(inBlock);

	FBL_CHECK( inPacket );
	if( inPacket )
	{
		ProxyPtr remoteId = inPacket->get_RemoteIDParam();

		I_RemoteProxy* proxy = reinterpret_cast<I_RemoteProxy*>(remoteId);
		mValue = proxy->get_I_Unknown();
	}
*/
}


/**********************************************************************************************/
void PropertyIUnknown::To( I_OStream_Ptr inStream, bool inBlock ) const
{
	argused2(inStream, inBlock);	
	FBL_CHECK(false);
	// IS: 24.11.2009
	// Not implemented - because it is vShared.dll but ProxyPtr might be
	// different for Client and Server. And we MUST NOT implement such
	// code in vShared. See FBL_Types.h
/*
	argused1(inBlock);
	Property_Imp::To(inStream, inBlock);

	I_Connectable* pConnectable = dcast<I_Connectable*>(mValue.get());
	FBL_CHECK(pConnectable);
	if( pConnectable )
	{
		ProxyPtr remoteId = (ProxyPtr) pConnectable->get_RemoteID();
		FBL_CHECK(remoteId != 0);
		*inStream << remoteId;
	}
*/
}


/**********************************************************************************************/
void PropertyIUnknown::To( I_PacketSnd* inPacket, bool inBlock ) const
{
	argused2(inPacket, inBlock);	
	FBL_CHECK(false);
	// IS: 24.11.2009
	// Not implemented - because it is vShared.dll but ProxyPtr might be
	// different for Client and Server. And we MUST NOT implement such
	// code in vShared. See FBL_Types.h
/*
	argused1(inBlock);
	I_Connectable_Ptr pConnectable = QI( mValue, I_Connectable );
	FBL_CHECK(pConnectable);
	if( pConnectable )
	{
		ProxyPtr remoteId = (ProxyPtr) pConnectable->get_RemoteID();
		FBL_CHECK(remoteId != 0);
		inPacket->put_RemoteIDParam( remoteId );
	}
*/
}


#pragma mark - 


/**********************************************************************************************/
// class implements list property
//
/**********************************************************************************************/
PropertyCollection::PropertyCollection( const UChar* inName )
	:
Property_Imp( inName )
{
	mValue = nullptr;
}


/**********************************************************************************************/
PropertyCollection::PropertyCollection( const String& inName )
	:
Property_Imp( inName )
{
	mValue = nullptr;
}


/**********************************************************************************************/
PropertyCollection::~PropertyCollection( void )
{
	try
	{
		mValue->Clear();
	}
	SWALLOW_CATCH
}


#pragma mark -


/**********************************************************************************************/
I_Property_Ptr PropertyCollection::Clone( void ) const 
{
	I_Property_Ptr cloned = new PropertyCollection( Property_Imp::get_Name().c_str() );
	cloned->put_Collection( mValue );

	return cloned;
}


/**********************************************************************************************/
PROPERTY_TYPE PropertyCollection::get_Type( void ) const 
{
	return TYPE_COLLECTION;
}


/**********************************************************************************************/
I_Unknown_Ptr PropertyCollection::get_I_Unknown( void ) const
{
	return mValue;
}


/**********************************************************************************************/
void PropertyCollection::put_I_Unknown( I_Unknown_Ptr inValue )
{
	I_PropertyContainer_Ptr pPropCollection = fbl_dynamic_cast< I_PropertyContainer >( inValue  );

	FBL_CHECK( pPropCollection != nullptr );

	put_Collection( pPropCollection );
}


/**********************************************************************************************/
const I_PropertyContainer_Ptr PropertyCollection::get_Collection( void ) const
{
	return mValue;
}


/**********************************************************************************************/
void PropertyCollection::put_Collection( const I_PropertyContainer_Ptr inValue )
{
	mValue->Clear();

	if( inValue )
	{
		vuint32 count = inValue->get_Count() + 1;
		for( vuint32 index = 1; index < count; index++ )
			mValue->Add( inValue->get_Property(index) );
	}
}


/**********************************************************************************************/
FBL_End_Namespace
