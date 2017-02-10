/**********************************************************************************************/
/* FBL_I_VNETW_Packet.h                                                      				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_VNETW_Packet_h
	#define _FBL_I_VNETW_Packet_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>

// FINAL: 
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace

interface I_Converter;
interface I_OStream;

/**********************************************************************************************/
typedef vuint32		COMMAND_NAME;
typedef vuint16		VERSION_TYPE;
#define kUnknownCmd 0xFFFFFFFF
#define kTestCmd	0xEEEEEEEE


/**********************************************************************************************/
//
//
// We don't use smart ptrs (I_Unknown inheritance) here because it makes creating stack objects 
// imposible.
//
//

/**********************************************************************************************/
/// Interface of output packet object.
// 
interface I_PacketSnd
{

virtual						~I_PacketSnd( void ){}


virtual void        	    Send( void ) = 0; 

virtual void				Init( 
								 RemoteID		inRemoteID, 
								 COMMAND_NAME	inCommandName,
								 VERSION_TYPE	inVersion  ) = 0;

virtual void				put_Converter( I_Converter* inpConverter ) = 0;
virtual I_Converter*		get_Converter( void ) = 0;
virtual bool				get_IsSingleByte( void ) const = 0;

							// Caller must set correct param count.
virtual	void				put_ParamCount( vuint16 inParamCount ) = 0;
virtual	vuint16				get_ParamCount( void ) const = 0;

virtual RemoteID			get_RemoteID( void )const = 0;

virtual VERSION_TYPE		get_Version( void ) const = 0;

virtual COMMAND_NAME		get_Command( void )const = 0;

virtual	I_OStream&			get_OStream( void ) = 0;

virtual	void				put_BinaryParam( 
								const void* 	inBuffer, 
								vuint32 			inHowMuch ) = 0;
		
virtual	void				put_BoolParam( bool inParam ) = 0;
		
virtual	void				put_UCharParam( vuint8 inParam ) = 0;	 
virtual	void				put_CharParam( char inParam ) = 0;
		
virtual	void				put_UCharParam2( UChar inParam ) = 0;
		
virtual	void				put_ShortParam( vint16 inParam ) = 0;
virtual	void				put_UShortParam( vuint16 inParam ) = 0;
		
virtual	void				put_LongParam( vint32 inParam ) = 0;
virtual	void				put_ULongParam( vuint32 inParam ) = 0;	 

virtual	void				put_RemoteIDParam( ProxyPtr inParam ) = 0;	 

virtual	void				put_FloatParam( float inParam ) = 0;
virtual	void				put_DoubleParam( double inParam ) = 0;

virtual	void				put_LLongParam( vint64 inParam ) = 0;
virtual	void				put_ULLongParam( vuint64 inParam ) = 0;

							// should be used for UTF16 strings.
virtual	void				put_StringParam( 
								const UChar*	inParam, 
								tslen			inLength = -1 ) = 0;

							// should be used for single byte encodings and UTF8.
virtual	void				put_StringParam( const char* inParam, tslen inLength = -1 ) = 0;

virtual	void				put_StringParam( const String&	inStr ) = 0;

};


/**********************************************************************************************/
/// Interface of input packet object.
// 
interface I_PacketRcv
{
virtual						~I_PacketRcv( void ){}

virtual vuint32				get_Length( void ) = 0;

virtual void				SetStartPosition( void ) = 0;

virtual	void				RewindToStartPosition( void ) = 0;

virtual	void				Recieve( void ) = 0;

virtual void				put_Converter( I_Converter* inpConverter ) = 0;
virtual I_Converter*		get_Converter( void ) = 0;
virtual bool				get_IsSingleByte( void ) const = 0;

virtual	vuint16				get_ParamCount( void ) const = 0;

virtual RemoteID			get_RemoteID( void )const = 0;

virtual VERSION_TYPE		get_Version( void ) const = 0;

virtual COMMAND_NAME		get_Command( void )const = 0;

virtual	void				get_BinaryParam( void* 	outBuffer, vuint32 inHowMuch ) = 0;
		
virtual	bool				get_BoolParam( void ) = 0;

virtual	vuint8				get_UCharParam( void ) = 0;
virtual	char				get_CharParam( void ) = 0;
		
virtual	UChar				get_UCharParam2( void ) = 0;
		
virtual	vint16				get_ShortParam( void ) = 0;
virtual	vuint16				get_UShortParam( void ) = 0;	 
		
virtual	vint32				get_LongParam( void ) = 0;
virtual	vuint32				get_ULongParam( void ) = 0;

virtual	ProxyPtr			get_RemoteIDParam( void ) = 0;	 

virtual	float				get_FloatParam( void ) = 0;
virtual	double				get_DoubleParam( void ) = 0;	 

virtual	vint64				get_LLongParam( void ) = 0;
virtual	vuint64				get_ULLongParam( void ) = 0;	 


							// should be used for UTF16 strings.
virtual	void				get_StringParam( UChar* outParam, tslen inLength ) = 0;
virtual	UChar*				get_UCharStrParam( void ) = 0;	 

							// should be used for single byte encodings and UTF8.
virtual	void				get_StringParam( char* outParam, tslen inLength ) = 0;
virtual	char*				get_CharStrParam( void ) = 0;	 
virtual	void				get_CharStrParam( String& outStr ) = 0;	 

virtual void				get_StringParam( String& outStr ) = 0;

virtual ERROR_TYPE			get_ErrorCode( void ) = 0;
virtual const String&		get_ErrorString( void ) = 0;

}; 


/**********************************************************************************************/
// Simple "stack-used" class which performs fixing for param count in PacketSnd.
//
class FixParamCount : public noncopyable
{
	private://////////////////////////////////////////////////////////////////////////////////// 

							FixParamCount( const FixParamCount& inSrc ); 
		FixParamCount&		operator=( const FixParamCount& inSrc );
	
	
	protected:////////////////////////////////////////////////////////////////////////////////// 
    
    						FixParamCount( void ) 
							: 	mpPacket(nullptr), 
								mParamCount(0) 
							{
							}
	
   
    public://///////////////////////////////////////////////////////////////////////////////////

    						FixParamCount( I_PacketSnd& inPacket, vuint16 inParamCount = 1 )
							:
								mpPacket( &inPacket ),
								mParamCount( (vuint16)( mpPacket->get_ParamCount() + inParamCount ) )
    						{
    							// Actual param count is: (Start paramCount + inParamCount).
    						}


    						FixParamCount( I_PacketSnd* inPacket, vuint16 inParamCount = 1 )
    						:
    							mpPacket( inPacket ),
								mParamCount( (vuint16)(mpPacket->get_ParamCount() + inParamCount) )
    						{
    							FBL_CHECK( mpPacket );
								
    							// Actual param count is: (Start paramCount + inParamCount).
    						}

							
virtual						~FixParamCount()
							{
								mpPacket->put_ParamCount( mParamCount );
							}
							

    protected:////////////////////////////////////////////////////////////////////////////////// 

		I_PacketSnd*		mpPacket;
		vuint16				mParamCount;	
};


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_I_Seriazible_h
