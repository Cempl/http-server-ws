/**********************************************************************************************/
/* FBL_String_Stream.cpp                                                         			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

// FBL:
#include <VShared/FBL/publ/Streams/FBL_String_Stream.h>
#include <VShared/FBL/publ/Util_Strings/FBL_ustdio.h>

#include <VShared/FBL/publ/Util_Strings/FBL_UnicodeToFromNumeric.h>
#include <VShared/FBL/publ/Util_Strings/FBL_String_Len_Const.h>


/**********************************************************************************************/
FBL_Begin_Namespace

/**********************************************************************************************/
const UChar LF = UChar('\n');

/**********************************************************************************************/
String_Stream::String_Stream( String* inString ) 
: 
	mpString( inString ),
	mCurrIndent(0)
{
	FBL_CHECK( mpString );
}


/**********************************************************************************************/
vint32 String_Stream::get_LastError( void ) const 
{
	return 0;
}


/**********************************************************************************************/
vuint32 String_Stream::get_LastCount( void ) const 
{
	return 0;
}


/**********************************************************************************************/
flength String_Stream::get_ByteSize( void ) const 
{
	return mpString->length();
}


/**********************************************************************************************/
vint32 String_Stream::get_Handle( void ) const 
{
	return 0;
}


/**********************************************************************************************/
flength String_Stream::Seek( 
	 flength 	inOffset, 
	 ESeekMode 	inMode ) 
{
	argused2( inOffset, inMode );
	return 0;
}


/**********************************************************************************************/
flength String_Stream::Tell( void ) const 
{
	return 0;
}


/**********************************************************************************************/
void String_Stream::Flush( void )
{
}


/**********************************************************************************************/
void String_Stream::Close( void )
{
}


/**********************************************************************************************/
void String_Stream::Reset( void )
{
}


/**********************************************************************************************/
bool String_Stream::get_Eof( void ) const  
{
	return false; 
}


/**********************************************************************************************/
bool String_Stream::CanRead( void ) const 
{ 
	return false; 
}


/**********************************************************************************************/
I_IStream& String_Stream::get(  
	void* 	inBuffer, 
	vuint32 	inHowMuch )
{
	fixme_argused2( inBuffer, inHowMuch );
	return *this;
}


/**********************************************************************************************/
I_IStream& String_Stream::get(I_OStream& streamOut)
{
	fixme_argused1( streamOut );
	return *this;
}


/**********************************************************************************************/
I_IStream& String_Stream::get( bool& outValue )
{
	fixme_argused1( outValue );
	return *this;
}


/**********************************************************************************************/
I_IStream& String_Stream::get( vuint8& outValue )
{
	fixme_argused1( outValue );
	return *this;
}


/**********************************************************************************************/
I_IStream& String_Stream::get( char& outValue )
{
	fixme_argused1( outValue );
	return *this;
}


/**********************************************************************************************/
I_IStream& String_Stream::get( vint16& outValue )
{
	fixme_argused1( outValue );
	return *this;
}


/**********************************************************************************************/
I_IStream& String_Stream::get( vuint16& outValue )
{
	fixme_argused1( outValue );
	return *this;
}


/**********************************************************************************************/
I_IStream& String_Stream::get( vint32& outValue )
{
	fixme_argused1( outValue );
	return *this;
}


/**********************************************************************************************/
I_IStream& String_Stream::get( vuint32& outValue )
{
	fixme_argused1( outValue );
	return *this;
}


/**********************************************************************************************/
#if FBL_MAC
I_IStream& String_Stream::get( size_t& outValue )
{
	#if FBL_64
	return get( (vuint64&)outValue );
	#else
	return get( (vuint32&)outValue );	
	#endif // FBL_64
}
#endif // FBL_MAC


/**********************************************************************************************/
I_IStream& String_Stream::get( float& outValue )
{
	fixme_argused1( outValue );
	return *this;
}


/**********************************************************************************************/
I_IStream& String_Stream::get( double& outValue )
{
	fixme_argused1( outValue );
	return *this;
}


/**********************************************************************************************/
I_IStream& String_Stream::get( vint64& outValue )
{
	fixme_argused1( outValue );
	return *this;
}


/**********************************************************************************************/
I_IStream& String_Stream::get( vuint64& outValue )
{
	fixme_argused1( outValue );
	return *this;
}


/**********************************************************************************************/
I_IStream& String_Stream::get( 
	UChar* inBuffer,
	tslen inChars )
{
	fixme_argused2( inBuffer, inChars );
	return *this;
}


/**********************************************************************************************/
I_OStream& String_Stream::put(  
	const void* inBuffer, 
	vuint32 		inHowMuch )
{
	argused2(inBuffer, inHowMuch);
	return *this;
}


/**********************************************************************************************/
I_OStream& String_Stream::put( I_IStream& stream_in )
{
	argused1(stream_in);
	return *this;
}


/**********************************************************************************************/
I_OStream& String_Stream::put( bool inValue )
{
	UChar ch[kBoolStrMaxLen + 1];
	u_ltou(inValue, ch);

	mpString->append( ch, kBoolStrMaxLen + 1 );
	return *this;
}


/**********************************************************************************************/
I_OStream& String_Stream::put( vuint8 inValue )
{
	UChar ch[kUCharStrMaxLen + 1];
	u_ltou(inValue, ch);

	mpString->append( ch, kUCharStrMaxLen + 1 );
	return *this;
}


/**********************************************************************************************/
I_OStream& String_Stream::put( char inValue )
{
	UChar ch;
	vu_FromDefaultCPToUnicode( &inValue, 1, &ch, 1 );

	if( ch == LF )
	{
		// Insert line break.
#if FBL_WIN
		static String endline( "\n" );
#elif FBL_MAC
		static String endline( "\r" );
#else
		static String endline( "\n" );
#endif // FBL_WIN

		mpString->append( endline.c_str(), endline.length() );
	}
	else
	{
		mpString->append( (const UChar*)&inValue, 1 );
	}

	return *this;
}

/**********************************************************************************************/
I_OStream& String_Stream::put( vint16 inValue )
{
	mpString->addInt( inValue );
	return *this;
}


/**********************************************************************************************/
I_OStream& String_Stream::put( vuint16 inValue )
{
	mpString->addUInt( inValue );
	return *this;
}


/**********************************************************************************************/
I_OStream& String_Stream::put( vint32 inValue )
{
	mpString->addInt( inValue );
	return *this;
}


/**********************************************************************************************/
I_OStream& String_Stream::put( vuint32 inValue )
{
	mpString->addUInt( inValue );
	return *this;
}


/**********************************************************************************************/
#if FBL_MAC
I_OStream& String_Stream::put( size_t inValue )
{
	#if FBL_64
	return put( (vuint64)inValue );
	#else
	return put( (vuint32)inValue );	
	#endif // FBL_64
}
#endif // FBL_MAC


/**********************************************************************************************/
I_OStream& String_Stream::put( float inValue )
{
	mpString->addDouble( inValue );
	return *this;
}


/**********************************************************************************************/
I_OStream& String_Stream::put( double inValue )
{
	mpString->addDouble( inValue );
	return *this;
}


/**********************************************************************************************/
I_OStream& String_Stream::put( vint64 inValue )
{
	mpString->addLLong( inValue );
	return *this;
}


/**********************************************************************************************/
I_OStream& String_Stream::put( vuint64 inValue )
{
	mpString->addULLong( inValue );
	return *this;
}


/**********************************************************************************************/
I_OStream& String_Stream::put( const UChar* inString, tslen inLen )
{
	mpString->append( inString, inLen );
	return *this;
}


/**********************************************************************************************/
I_OStream& String_Stream::put( const char* inString, tslen inLen )
{
	mpString->append( inString, inLen );
	return *this;
}


/**********************************************************************************************/
/*
vuint32 String_Stream::DoPut( const void* inBuffer, vuint32 inHowMuch)
{
	// TODO: ???
	// Probably we need to implement set of get/put methods instead of Text_Stream inheritance.
	mString->append( (const UChar*)inBuffer, inHowMuch/2 );
	return inHowMuch;
}
*/

/**********************************************************************************************/
/*
vuint32 String_Stream::DoGet( void* inBuffer, vuint32 inHowMuch )
{
	return 0;
}
*/


/**********************************************************************************************/
void String_Stream::IndentRight( void )
{
	mCurrIndent++;
}


/**********************************************************************************************/
void String_Stream::IndentLeft( void )
{
	mCurrIndent--;
}


/**********************************************************************************************/
I_OStream& String_Stream::WriteIndent( void )
{
	vint32 index = (mCurrIndent < kMaxIndent) ? mCurrIndent : kMaxIndent - 1;
	put( GetIdents(), index );

	return *this;
}


/**********************************************************************************************/
FBL_End_Namespace
