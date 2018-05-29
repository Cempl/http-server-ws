/**********************************************************************************************/
/* FBL_Binary_Stream.h																		  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_Binary_Stream_h 
	#define _FBL_Binary_Stream_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Interfaces/FBL_I_IStream.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_OStream.h>

#include <VShared/FBL/publ/Streams/FBL_Stream_Imp.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_CLASS(Binary_Stream);


/**********************************************************************************************/
// the temporary buffer size used when copying from stream to stream
//
#define BUF_TEMP_SIZE 4096


/**********************************************************************************************/
// size of statically declared array
//
#define FBL_SIZEOF(array)   (sizeof(array)/sizeof(array[0]))


/**********************************************************************************************/
// 
class Binary_Stream : public Stream_Imp,
					  public I_IStream,
					  public I_OStream
{

    protected://///////////////////////////////////////////////////////////////////////////////////

							Binary_Stream( void );
virtual 					~Binary_Stream( void );


    public://///////////////////////////////////////////////////////////////////////////////////

// I_OStream API:

    // ---------------------
    // Output methods:

virtual I_OStream&          put(
								  const void* inBuffer, 
								  vuint32 		inHowMuch) override;

virtual I_OStream&          put(I_IStream& stream_in) override{ stream_in.get(*this); return * (I_OStream*) this; }

virtual I_OStream&  		put( bool inValue ) override		{ return put((void*) &inValue, sizeof(inValue)); }
virtual I_OStream&			put( vuint8 inValue ) override	{ return put((void*) &inValue, sizeof(inValue)); }
virtual I_OStream&			put( char inValue ) override		{ return put((void*) &inValue, sizeof(inValue)); }
virtual I_OStream&			put( vint16 inValue ) override	{ return put((void*) &inValue, sizeof(inValue)); }		
virtual I_OStream&			put( vuint16 inValue ) override	{ return put((void*) &inValue, sizeof(inValue)); }		
virtual I_OStream&			put( vint32 inValue ) override		{ return put((void*) &inValue, sizeof(inValue)); }		
virtual I_OStream&			put( vuint32 inValue ) override	{ return put((void*) &inValue, sizeof(inValue)); }		
#if FBL_MAC
virtual I_OStream&			put( size_t inValue ) override;
#endif // FBL_MAC
virtual I_OStream&			put( float inValue ) override	{ return put((void*) &inValue, sizeof(inValue)); }		
virtual I_OStream&			put( double inValue ) override	{ return put((void*) &inValue, sizeof(inValue)); }		
virtual I_OStream&			put( vint64 inValue ) override	{ return put((void*) &inValue, sizeof(inValue)); }		
virtual I_OStream&			put( vuint64 inValue ) override	{ return put((void*) &inValue, sizeof(inValue)); }		
virtual I_OStream&			put( const UChar* inString, tslen inLen = -1 ) override;
virtual I_OStream&			put( const char* inString, tslen inLen = -1 ) override;


    public://///////////////////////////////////////////////////////////////////////////////////

    // ---------------------
    // Input methods:
    
virtual I_IStream&          get(
							  void* 		inBuffer, 
							  vuint32 		inHowMuch ) override;

virtual I_IStream&          get( I_OStream& streamOut ) override;

virtual I_IStream&			get( bool& outValue ) override 	{ return get(&outValue, sizeof(outValue)); }		
virtual I_IStream&			get( vuint8& outValue ) override	{ return get(&outValue, sizeof(outValue)); }
virtual I_IStream&			get( char& outValue ) override	{ return get(&outValue, sizeof(outValue)); }

virtual I_IStream&			get( vint16& outValue ) override	{ return get(&outValue, sizeof(outValue)); }		
virtual I_IStream&			get( vuint16& outValue ) override	{ return get( (void*) &outValue, sizeof(outValue)); }		
virtual I_IStream&			get( vint32& outValue ) override	{ return get(&outValue, sizeof(outValue)); }		
virtual I_IStream&			get( vuint32& outValue ) override	{ return get(&outValue, sizeof(outValue)); }		
#if FBL_MAC
virtual I_IStream&			get( size_t& outValue ) override;
#endif // FBL_MAC
virtual I_IStream&			get( float& outValue ) override	{ return get(&outValue, sizeof(outValue)); }		
virtual I_IStream&			get( double& outValue ) override	{ return get(&outValue, sizeof(outValue)); }	
virtual I_IStream&			get( vint64& outValue ) override	{ return get(&outValue, sizeof(outValue)); }		
virtual I_IStream&			get( vuint64& outValue ) override	{ return get(&outValue, sizeof(outValue)); }

virtual I_IStream&          get( UChar* inBuffer, tslen inChars ) override;


	// ---------------------
	// Indent methods:

virtual void				IndentRight( void ) override;
virtual void				IndentLeft( void ) override;
virtual I_OStream&			WriteIndent( void ) override;


	protected://///////////////////////////////////////////////////////////////////////////////////

    // ---------------------
    // Write-back buffer support:

		char *              AllocSpaceWBack( vint32 sizeNeeded );
		vuint32            	GetWBack( void* inBuf, vuint32 inSize );
 

	protected://////////////////////////////////////////////////////////////////////////////////////

		char *              mWback; 	///< Write back buffer or NULL if none.
		vuint32              	mWbacksize; ///< The size of the buffer.
		vuint32              	mWbackcur;	///< The current position in the buffer.

};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_Binary_Stream_h


