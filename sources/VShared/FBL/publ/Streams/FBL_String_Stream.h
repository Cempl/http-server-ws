/**********************************************************************************************/
/* FBL_String_Stream.h																		  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_String_Stream_h
	#define _FBL_String_Stream_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Streams/FBL_Text_Stream.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_CLASS(String_Stream);


/**********************************************************************************************/
// Text mode stream implementation based on UTF-16 encoding.
// (At least in the first implementation).
//
class FBL_SHARED_EXP_CLASS String_Stream : 
	public I_IStream,
	public I_OStream
{
		IMPLEMENT_UNKNOWN(String_Stream)											
		BEGIN_INTERFACE_TABLE(String_Stream)										
			IMPLEMENT_INTERFACE(I_IStream)										
			IMPLEMENT_INTERFACE(I_OStream)										
		END_INTERFACE_TABLE()
 
	public://///////////////////////////////////////////////////////////////////////////////////
	
							String_Stream( String* inString );
							

	public://///////////////////////////////////////////////////////////////////////////////////
	
	// I_Stream:

virtual vint32             	get_LastError( void ) const override;
virtual vuint32             get_LastCount( void ) const override;
virtual flength             get_ByteSize( void ) const override;
virtual	vint32				get_Handle( void ) const override; 

virtual flength             Seek( 
								flength 	inOffset, 
								ESeekMode 	inMode ) override;

virtual flength             Tell( void ) const override;
virtual	void				Flush( void ) override;	
virtual	void				Close( void ) override;	
virtual void                Reset( void ) override;



	public://///////////////////////////////////////////////////////////////////////////////////

	// I_IStream API:

virtual bool                get_Eof( void ) const override;

virtual vuint32				PopZombiPacket( void ) override
								{ return 0; }
virtual void				PushZombiPacket( void ) override
								{}
virtual void				Invalidate( void ) override
								{}
				
virtual bool                CanRead( void ) const override;

virtual I_IStream&          get(  void* 	inBuffer, 
								vuint32 	inHowMuch ) override;

virtual I_IStream&          get(I_OStream& streamOut) override;
virtual I_IStream&			get( bool& outValue ) override;
virtual I_IStream&			get( vuint8& outValue ) override;
virtual I_IStream&			get( char& outValue ) override;
virtual I_IStream&			get( vint16& outValue ) override;
virtual I_IStream&			get( vuint16& outValue ) override;
virtual I_IStream&			get( vint32& outValue ) override;
virtual I_IStream&			get( vuint32& outValue ) override;
#if FBL_MAC
virtual I_IStream&			get( size_t& outValue ) override;
#endif // FBL_MAC
virtual I_IStream&			get( float& outValue ) override;
virtual I_IStream&			get( double& outValue ) override;
virtual I_IStream&			get( vint64& outValue ) override;
virtual I_IStream&			get( vuint64& outValue ) override;
virtual I_IStream&          get( UChar* inBuffer, 
								tslen inChars ) override;

	public://///////////////////////////////////////////////////////////////////////////////////

	// Interface I_OStream

virtual I_OStream&          put(  const void* 	inBuffer, 
								vuint32 		inHowMuch ) override;

virtual I_OStream&          put( I_IStream& stream_in ) override;
virtual I_OStream&  		put( bool inValue ) override;
virtual I_OStream&			put( vuint8 inValue ) override;
virtual I_OStream&			put( char inValue ) override;
virtual I_OStream&			put( vint16 inValue ) override;
virtual I_OStream&			put( vuint16 inValue ) override;
virtual I_OStream&			put( vint32 inValue ) override;
virtual I_OStream&			put( vuint32 inValue ) override;
#if FBL_MAC
virtual I_OStream&			put( size_t inValue ) override;
#endif // FBL_MAC
virtual I_OStream&			put( float inValue ) override;
virtual I_OStream&			put( double inValue ) override;
virtual I_OStream&			put( vint64 inValue ) override;
virtual I_OStream&			put( vuint64 inValue ) override;
virtual I_OStream&			put( const UChar* inString, tslen inLen = -1 ) override;
virtual I_OStream&			put( const char* inString, tslen inLen = - 1 ) override;

virtual void				IndentRight( void ) override;
virtual void				IndentLeft( void ) override;
virtual I_OStream&			WriteIndent( void ) override;


	private:////////////////////////////////////////////////////////////////////////////////////

		String*				mpString;

		vuint32 			mCurrIndent;
};


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_String_Stream_h
