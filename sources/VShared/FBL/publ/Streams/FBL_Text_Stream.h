/**********************************************************************************************/
/* FBL_Text_Stream.h																		  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_Text_Stream_h
	#define _FBL_Text_Stream_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Interfaces/FBL_I_IStream.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_OStream.h>

#include <VShared/FBL/publ/Streams/FBL_Stream_Imp.h>

#include <VShared/FBL/publ/Util_Strings/FBL_ustdio.h>

#include <VShared/FBL/publ/Util_classes/FBL_StringConversion.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
const vuint32 kMaxIndent = 64; // 0..63


/**********************************************************************************************/
// Returns pointer to static array with kMaxIndent UChar('\t').
//
const UChar* GetIdents( void );


/**********************************************************************************************/
SMART_CLASS(Text_Stream);


/**********************************************************************************************/
// Text mode stream implementation based on UTF-16 encoding.
//
class Text_Stream : public Stream_Imp,
					public I_IStream,
					public I_OStream
{
	protected://////////////////////////////////////////////////////////////////////////////////
	
							Text_Stream( void );
							

	public://///////////////////////////////////////////////////////////////////////////////////


    // ---------------------
    // Input methods:

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
virtual I_IStream&          get( UChar* inBuffer, tslen inChars ) override;


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

	// Indent methods:

virtual void				IndentRight( void ) override;
virtual void				IndentLeft( void ) override;

virtual I_OStream&			WriteIndent( void ) override;


	protected://////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// Derived classes API:

virtual void				Overflow( void );
virtual void				PutFailed( void );
virtual void				Underflow( void );
virtual void				GetFailed( void );


	protected://////////////////////////////////////////////////////////////////////////////////

		void				SkipSpaces( void );

		void				PutIt( const UChar* inCh, tslen inLen = -1 );

		UChar				GetChar( void );

							///	Turns read position back by once UChar.
		void				UnGet( void );

		template<class T, vuint32 kTStrMaxLen, class FN>
		T					StrToNum( FN inConvertFN );


	protected://////////////////////////////////////////////////////////////////////////////////

		vuint32 			mCurrIndent;

		String				mEndLine;
};


/**********************************************************************************************/
bool IsDelim( UChar ch );


/**********************************************************************************************/
template<class T, vuint32 kTStrMaxLen, class FN>
T Text_Stream::StrToNum( FN inConvertFN )
{
	SkipSpaces();

	UChar c = 0;
	UChar buf[kTStrMaxLen + 1];
	UChar *p = buf;

	c = GetChar();
	if( vu_isdigit(c) || c == UChar('-') || c == UChar('+') )
	{
		do
		{
			*p++ = c;
		}
		while( !IsDelim(c = GetChar()) && vuint32(p - buf) < kTStrMaxLen );
	}
	else
	{
		// Wrong input format
		*p = 0;
		throw xExternalError(ERR_STREAM_READ_ERROR, "" );
	}

	*p = 0;
	return static_cast<T>(inConvertFN(buf));
}


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_Text_Stream_h
