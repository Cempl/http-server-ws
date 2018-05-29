/**********************************************************************************************/
/* FBL_Property.h	                                                      					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_Property_h
	#define _FBL_Property_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Property.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Serializable.h>

#include <VShared/FBL/publ/Properties/FBL_PropertyContainer.h>

#include <VShared/FBL/publ/Util_classes/FBL_String.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_CLASS( PropertyBoolean );
SMART_CLASS( PropertyLong );
SMART_CLASS( PropertyString );
SMART_CLASS( PropertyCollection );
SMART_CLASS( PropertyIUnknown );


/**********************************************************************************************/
// Class implements base functionality for all properties.
//
class FBL_SHARED_EXP_CLASS Property_Imp :	
	public I_Property,
	public noncopyable
{
		// imp - do not make IUNKNOWN

	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// Construction/Destruction
	 	
							Property_Imp( const UChar* inName );
							Property_Imp( const String& inName );
							
virtual 					~Property_Imp( void );


	public://///////////////////////////////////////////////////////////////////////////////////

// I_Property interface:

	// ---------------------
	// Properties:

virtual const String& 		get_Name( void ) const override;


	// ---------------------
	// Value Properties:

virtual vint32 				get_Long( void ) const override;
virtual void 				put_Long( vint32 inValue ) override;

virtual bool 				get_Boolean( void ) const override;
virtual void 				put_Boolean( bool inValue ) override;

virtual const String&		get_String( void ) const override;
virtual void				put_String( const UChar* inValue ) override;
virtual void				put_String( const String& inValue ) override;

virtual I_Unknown_Ptr		get_I_Unknown( void ) const override;
virtual void				put_I_Unknown( I_Unknown_Ptr inValue ) override;

virtual const I_PropertyContainer_Ptr	
							get_Collection( void ) const override;
virtual void				put_Collection( const I_PropertyContainer_Ptr inValue ) override;

// I_Serializable interface:

virtual void				To( I_OStream_Ptr inStream,	bool inBlock = true ) const;
virtual void				To( I_PacketSnd*  inPacket,	bool inBlock = true ) const;


	private:////////////////////////////////////////////////////////////////////////////////////
	 
	 	String				mName;			// The name of this property.
};


/**********************************************************************************************/
// Class implements Boolean property.
//
class FBL_SHARED_EXP_CLASS PropertyBoolean : 
	public Property_Imp,
	public I_Serializable
{
		IMPLEMENT_UNKNOWN(PropertyBoolean)											
		BEGIN_INTERFACE_TABLE(PropertyBoolean)										
			IMPLEMENT_INTERFACE(I_Property)										
			IMPLEMENT_INTERFACE(I_Serializable)										
		END_INTERFACE_TABLE()
  
	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// Construction/Destruction
	 	
							PropertyBoolean( const UChar* inName );
							PropertyBoolean( const String& inName );

							PropertyBoolean( const String& inName, bool inValue );
							
virtual 					~PropertyBoolean( void );


	public://///////////////////////////////////////////////////////////////////////////////////

// I_Property interface:

virtual	I_Property_Ptr		Clone( void ) const override;

	// ---------------------
	// Properties:

virtual PROPERTY_TYPE		get_Type( void ) const override;

	// ---------------------
	// Value Properties:

virtual bool 				get_Boolean( void ) const override;
virtual void 				put_Boolean( bool inValue ) override;

virtual const String&		get_String( void ) const override;

// I_Serializable interface:

virtual void				From( I_IStream_Ptr inStream, bool inBlock = true ) override;
virtual void				From( I_PacketRcv* 	inPacket, bool inBlock = true ) override;

virtual void				To( I_OStream_Ptr inStream, bool inBlock = true ) const override;
virtual void				To( I_PacketSnd* inPacket, bool inBlock = true ) const override;


	private:////////////////////////////////////////////////////////////////////////////////////
	 
		bool				mValue;			// The value contained in this property.
};


/**********************************************************************************************/
// class implements vint32 property
//
class FBL_SHARED_EXP_CLASS PropertyLong : 
	public Property_Imp,
	public I_Serializable
{
		IMPLEMENT_UNKNOWN(PropertyLong)											
		BEGIN_INTERFACE_TABLE(PropertyLong)										
			IMPLEMENT_INTERFACE(I_Property)										
			IMPLEMENT_INTERFACE(I_Serializable)										
		END_INTERFACE_TABLE()

	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// Construction/Destruction
	 	
							PropertyLong( const UChar* inName );
							PropertyLong( const String& inName );
							PropertyLong( const String& inName, vint32 inValue );
virtual 					~PropertyLong( void );


	public://///////////////////////////////////////////////////////////////////////////////////

// I_Property interface:

virtual	I_Property_Ptr		Clone( void ) const override;

	// ---------------------
	// Properties:

virtual PROPERTY_TYPE		get_Type( void ) const override;

	// ---------------------
	// Value Properties:

virtual vint32 				get_Long( void ) const override;
virtual void 				put_Long( vint32 inValue ) override;


// I_Serializable interface:

virtual void				From( I_IStream_Ptr inStream, bool inBlock = true ) override;
virtual void				From( I_PacketRcv* 	inPacket, bool inBlock = true ) override;

virtual void				To( I_OStream_Ptr inStream, bool inBlock = true ) const override;
virtual void				To( I_PacketSnd* inPacket, bool inBlock = true ) const override;


	private:////////////////////////////////////////////////////////////////////////////////////
	 
		vint32				mValue;			// The value contained in this property.
};


/**********************************************************************************************/
// class implements string property
//
class FBL_SHARED_EXP_CLASS PropertyString : 
	public Property_Imp,
	public I_Serializable
{
		IMPLEMENT_UNKNOWN(PropertyString)											
		BEGIN_INTERFACE_TABLE(PropertyString)										
			IMPLEMENT_INTERFACE(I_Property)										
			IMPLEMENT_INTERFACE(I_Serializable)										
		END_INTERFACE_TABLE()

	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// Construction/Destruction
	 	
							PropertyString( const UChar* inName );
							PropertyString( const String& inName );							
							PropertyString( const String& inName, const String& inValue );

virtual 					~PropertyString( void );


	public://///////////////////////////////////////////////////////////////////////////////////

// I_Property interface:

virtual	I_Property_Ptr		Clone( void ) const override;

	// ---------------------
	// Properties:

virtual PROPERTY_TYPE		get_Type( void ) const override;

	// ---------------------
	// Value Properties:

virtual const String&		get_String( void ) const override;
virtual void				put_String( const UChar* inValue ) override;
virtual void				put_String( const String& inValue ) override;


// I_Serializable interface:

virtual void				From( I_IStream_Ptr inStream, bool inBlock = true ) override;
virtual void				From( I_PacketRcv* 	inPacket, bool inBlock = true ) override;

virtual void				To( I_OStream_Ptr inStream, bool inBlock = true ) const override;
virtual void				To( I_PacketSnd* inPacket, bool inBlock = true ) const override;


	private:////////////////////////////////////////////////////////////////////////////////////
	 
		String				mValue;			// The value contained in this property.
};


/**********************************************************************************************/
// class implements property that keep I_Unknow object.
//
class FBL_SHARED_EXP_CLASS PropertyIUnknown : 
	public Property_Imp,
	public I_Serializable
{
		IMPLEMENT_UNKNOWN(PropertyIUnknown)											
		BEGIN_INTERFACE_TABLE(PropertyIUnknown)										
			IMPLEMENT_INTERFACE(I_Property)										
			IMPLEMENT_INTERFACE(I_Serializable)										
		END_INTERFACE_TABLE()
		
	
	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// Construction/Destruction
	 	
							PropertyIUnknown( const UChar* inName );
							PropertyIUnknown( const String& inName );
							PropertyIUnknown( const String& inName, I_Unknown_Ptr inValue );
virtual 					~PropertyIUnknown( void );


	public://///////////////////////////////////////////////////////////////////////////////////

// I_Property interface:

virtual	I_Property_Ptr		Clone( void ) const override;

	// ---------------------
	// Properties:

virtual PROPERTY_TYPE		get_Type( void ) const override;

	// ---------------------
	// Value Properties:

virtual I_Unknown_Ptr		get_I_Unknown( void ) const override;
virtual void				put_I_Unknown( I_Unknown_Ptr inValue ) override;

// I_Serializable interface:

virtual void				From( I_IStream_Ptr inStream, bool inBlock = true ) override;
virtual void				From( I_PacketRcv* 	inPacket, bool inBlock = true ) override;

virtual void				To( I_OStream_Ptr inStream, bool inBlock = true ) const override;
virtual void				To( I_PacketSnd* inPacket, bool inBlock = true ) const override;


	private:////////////////////////////////////////////////////////////////////////////////////
	 
		I_Unknown_Ptr		mValue;			// The value contained in this property.
};


/**********************************************************************************************/
// class implements collection property
// 
class FBL_SHARED_EXP_CLASS PropertyCollection : public Property_Imp
{
	IMPLEMENT_UNKNOWN(PropertyCollection)											
	BEGIN_INTERFACE_TABLE(PropertyCollection)										
	IMPLEMENT_INTERFACE(I_Property)										
	END_INTERFACE_TABLE()
	
public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// Construction/Destruction
	 	
							PropertyCollection( const UChar* inName );
							PropertyCollection( const String& inName );
virtual 					~PropertyCollection( void );


	public://///////////////////////////////////////////////////////////////////////////////////

// I_Property interface:

virtual	I_Property_Ptr		Clone( void ) const override;


	// ---------------------
	// Properties:

virtual PROPERTY_TYPE		get_Type( void ) const override;


	// ---------------------
	// Value Properties:
	// ERRORS: put methods - errFuncMustNotBeCalled, get methods - errParameter.

virtual I_Unknown_Ptr		get_I_Unknown( void ) const override;
virtual void				put_I_Unknown( I_Unknown_Ptr inValue ) override;

virtual const I_PropertyContainer_Ptr	
							get_Collection( void ) const override;
virtual void				put_Collection( const I_PropertyContainer_Ptr inValue ) override;


	private:////////////////////////////////////////////////////////////////////////////////////
	 
	I_PropertyContainer_Ptr	mValue;			// The value contained in this property.
};


/**********************************************************************************************/
inline 
I_IStream& operator>>( I_IStream& inStream, PROPERTY_TYPE& outType )
{
	vint16 Type;
	inStream >> Type;
	outType = static_cast<PROPERTY_TYPE>(Type);
	return inStream;
}

inline 
I_PacketRcv& operator>>( I_PacketRcv& inPacket, PROPERTY_TYPE& outType )
{
	vint16 Type = inPacket.get_ShortParam();
	outType = static_cast<PROPERTY_TYPE>(Type);
	return inPacket;
}


inline 
I_OStream& operator<<( I_OStream& inStream, PROPERTY_TYPE inType )
{
	inStream << static_cast<vint16>(inType);
	return inStream;
}

inline 
I_PacketSnd& operator<<( I_PacketSnd& inPacket, PROPERTY_TYPE inType )
{
	inPacket.put_ShortParam( static_cast<vint16>(inType) );
	return inPacket;
}


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_Property_h
