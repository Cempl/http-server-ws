/**********************************************************************************************/
/* FBL_Text_Stream.cpp                                                         				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

// FBL:
#include <VShared/FBL/publ/Streams/FBL_Text_Stream.h>
#include <VShared/FBL/publ/Util_Strings/FBL_ustdio.h>

#include <VShared/FBL/publ/Util_Strings/FBL_UnicodeToFromNumeric.h>
#include <VShared/FBL/publ/Util_Strings/FBL_String_Len_Const.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
const UChar* GetIdents( void )
{
	//
	// Here we will keep "\t\t\t\t" string to be used for INDENT.
	// We do not care about end zero, because LENGTH should be specified always.
	//
	static const UChar sIndents[kMaxIndent] = 
	{ 
		'\t', '\t', '\t', '\t', '\t', '\t', '\t', '\t', 
		'\t', '\t', '\t', '\t', '\t', '\t', '\t', '\t', 
		'\t', '\t', '\t', '\t', '\t', '\t', '\t', '\t', 
		'\t', '\t', '\t', '\t', '\t', '\t', '\t', '\t', 
		'\t', '\t', '\t', '\t', '\t', '\t', '\t', '\t', 
		'\t', '\t', '\t', '\t', '\t', '\t', '\t', '\t', 
		'\t', '\t', '\t', '\t', '\t', '\t', '\t', '\t', 
		'\t', '\t', '\t', '\t', '\t', '\t', '\t', '\t'
	};	

	return sIndents;
}


/**********************************************************************************************/
static const UChar CR = UChar('\r');
static const UChar LF = UChar('\n');


/**********************************************************************************************/
bool IsDelim( UChar ch )
{
    return ch == UChar(' ') || ch == UChar('\n') || ch == UChar('\t');
}


#pragma mark -


/**********************************************************************************************/
Text_Stream::Text_Stream( void ) 
:
	mCurrIndent( 0 ),

#if FBL_WIN
	mEndLine( "\r\n" )
#elif FBL_MAC
	mEndLine( "\r" )
#elif FBL_UNIX
	mEndLine( "\n" )
#else
#error Define correct behaviour for your platform!
#endif // FBL_WIN

{ 
}


/**********************************************************************************************/
I_IStream& Text_Stream::get( void* inBuffer, vuint32 inHowMuch )
{
	vuint32 readBytes = DoGet( (void*)inBuffer, inHowMuch );	
	argused1(readBytes);

	// Hmm, why we loose this info ???

	return *this;
}


/**********************************************************************************************/
I_IStream& Text_Stream::get( UChar* inBuffer, tslen inChars )
{
	// Read into given buffer 
	vuint32 readBytes = DoGet( (void*)inBuffer, inChars * sizeof(UChar) );	
	argused1(readBytes);

	return *this;
}


/**********************************************************************************************/
I_IStream& Text_Stream::get(I_OStream& streamOut)
{
	argused1(streamOut);
	return *this;
}


/**********************************************************************************************/
I_IStream& Text_Stream::get( bool& outValue )
{
	outValue = StrToNum<bool, kBoolStrMaxLen>(&u_utol);
	return *this;
}


/**********************************************************************************************/
I_IStream& Text_Stream::get( vuint8& outValue )
{
	outValue = StrToNum<vuint8, kUCharStrMaxLen>(&u_utol);
	return *this;
}


/**********************************************************************************************/
I_IStream& Text_Stream::get( char& outValue )
{
	UChar ch;
	
	get( (void*) &ch, sizeof(UChar) );
	
	vu_FromUnicodeToDefaultCP( &ch, 1, &outValue, 1 );
	
	return *this;
}


/**********************************************************************************************/
I_IStream& Text_Stream::get( vint16& outValue )
{
	outValue = StrToNum<vint16, kShortStrMaxLen>(&u_utol);
	return *this;
}

/**********************************************************************************************/
I_IStream& Text_Stream::get( vuint16& outValue )
{
	outValue = StrToNum<vuint16, kUShortStrMaxLen>(&u_ustoul);
	return *this;
}

/**********************************************************************************************/
I_IStream& Text_Stream::get( vint32& outValue )
{
	outValue = StrToNum<vint32, kLongStrMaxLen>(&u_utol);
	return *this;
}

/**********************************************************************************************/
I_IStream& Text_Stream::get( vuint32& outValue )
{
	outValue = StrToNum<vuint32, kULongStrMaxLen>(&u_ustoul);
	return *this;
}

#if FBL_MAC
/**********************************************************************************************/
I_IStream& Text_Stream::get( size_t& outValue )
{
	#if FBL_64
	return get( (vuint64&)outValue );
	#else
	return get( (vuint32&)outValue );	
	#endif // FBL_64
}
#endif // FBL_MAC

/**********************************************************************************************/
I_IStream& Text_Stream::get( float& outValue )
{
	outValue = StrToNum<float, kFloatStrMaxLen>(&u_utof);
	return *this;
}

/**********************************************************************************************/
I_IStream& Text_Stream::get( double& outValue )
{
	outValue = StrToNum<double, kDoubleStrMaxLen>(&u_utof);
	return *this;
}

/**********************************************************************************************/
I_IStream& Text_Stream::get( vint64& outValue )
{
	outValue = StrToNum<vint64, kLLongStrMaxLen>(&u_utoll);
	return *this;
}

/**********************************************************************************************/
I_IStream& Text_Stream::get( vuint64& outValue )
{
	outValue = StrToNum<vuint64, kULLongStrMaxLen>(&u_ustoull);
	return *this;
}


#pragma mark -


/**********************************************************************************************/
I_OStream& Text_Stream::put( const void* inBuffer, vuint32 inHowMuch )
{
	argused1(inBuffer);
	argused1(inHowMuch);

	return *this;
}


/**********************************************************************************************/
I_OStream& Text_Stream::put( I_IStream& stream_in )
{
	argused1(stream_in);

	return *this;
}


/**********************************************************************************************/
I_OStream& Text_Stream::put( bool inValue )
{
	UChar ch[kBoolStrMaxLen + 1];
	u_ltou(inValue, ch);
	PutIt(ch);

	return *this;
}


/**********************************************************************************************/
I_OStream& Text_Stream::put( vuint8 inValue )
{
	UChar ch[kUCharStrMaxLen + 1];
	u_ltou(inValue, ch);
	PutIt(ch);

	return *this;
}


/**********************************************************************************************/
I_OStream& Text_Stream::put( char inValue )
{
	UChar ch;
	vu_FromDefaultCPToUnicode( &inValue, 1, &ch, 1 );
	
	put(&ch, 1);
	
	return *this;
}


/**********************************************************************************************/
I_OStream& Text_Stream::put( vint16 inValue )
{
	UChar ch[kShortStrMaxLen + 1];
	u_ltou(inValue, ch);
	PutIt(ch);

	return *this;
}


/**********************************************************************************************/
I_OStream& Text_Stream::put( vuint16 inValue )
{
	UChar ch[kUShortStrMaxLen + 1];
	u_ultous(inValue, ch);
	PutIt(ch);

	return *this;
}

/**********************************************************************************************/
I_OStream& Text_Stream::put( vint32 inValue )
{
	UChar ch[kLongStrMaxLen + 1];
	u_ltou(inValue, ch);
	PutIt(ch);

	return *this;
}


/**********************************************************************************************/
I_OStream& Text_Stream::put( vuint32 inValue )
{
	UChar ch[kULongStrMaxLen + 1];
	u_ultous(inValue, ch);
	PutIt(ch);

	return *this;
}

/**********************************************************************************************/
#if FBL_MAC
I_OStream& Text_Stream::put( size_t inValue )
{
	#if FBL_64
	return put( (vuint64)inValue );
	#else
	return put( (vuint32)inValue );	
	#endif // FBL_64
}
#endif // FBL_MAC

/**********************************************************************************************/
I_OStream& Text_Stream::put( float inValue )
{
	UChar ch[kFloatStrMaxLen + 1];
	u_ftos(inValue, ch);
	PutIt(ch);

	return *this;
}


/**********************************************************************************************/
I_OStream& Text_Stream::put( double inValue )
{
	UChar ch[kDoubleStrMaxLen + 1];
	u_ftos(inValue, ch);
	PutIt(ch);

	return *this;
}


/**********************************************************************************************/
I_OStream& Text_Stream::put( vint64 inValue )
{
	UChar ch[kLLongStrMaxLen + 1];
	u_lltou(inValue, ch);
	PutIt(ch);

	return *this;
}


/**********************************************************************************************/
I_OStream& Text_Stream::put( vuint64 inValue )
{
	UChar ch[kULLongStrMaxLen + 1];
	u_ulltous(inValue, ch);
	PutIt(ch);

	return *this;
}


/**********************************************************************************************/
I_OStream& Text_Stream::put( const UChar* inString, tslen inLen )
{
	FBL_CHECK( inString );
	if( !inString )
		return *this;

	tslen Len = inLen;
	if( Len == -1 )
	{
		Len = vu_strlen(inString);
	}

#if FBL_UNIX
	
	// There is no need to map anything.
	PutIt(inString, Len);

#else

	// Determine whether LF code present
	const UChar* s = inString;
	const UChar* e = inString + Len;
	const UChar* p = s;

	// Marks the start of string containing to LF codes.
	const UChar* nlfs = s;	// no_lf_start

	while( p < e )
	{
#if FBL_WIN
		// /r/n for win - so react on the first symbol
		if( *p == CR )
#else
		// /n/r for mac or \n for unix - so react on the first symbol
		if( *p == LF )
#endif // FBL_WIN
		{
			tslen Len2 = static_cast<tslen>(p - nlfs);
			PutIt(nlfs, Len2);

			// Insert line break.
			PutIt( mEndLine.c_str(), mEndLine.length() );

			// Reset start of string containing no LF codes.
			nlfs = ++p;
		}
		else
		{
			p++;
		}
	}

	if( nlfs[0] != '\0' )
	{
		tslen Len3 = static_cast<tslen>(p - nlfs);
		PutIt(nlfs, Len3);
	}

#endif // FBL_UNIX

	return *this;
}


/**********************************************************************************************/
I_OStream& Text_Stream::put( const char* inString, tslen inLen )
{
	StToUTF16 cnv( inString, inLen );
	put( cnv.c_str(), cnv.length() );
	
	return *this;
}


#pragma mark -


/**********************************************************************************************/
void Text_Stream::IndentRight( void )
{
	mCurrIndent++;
}


/**********************************************************************************************/
void Text_Stream::IndentLeft( void )
{
	mCurrIndent--;
}


/**********************************************************************************************/
I_OStream& Text_Stream::WriteIndent( void )
{
	vint32 index = (mCurrIndent < kMaxIndent) ? mCurrIndent : kMaxIndent - 1;

	PutIt( GetIdents(), index );
	
	return *this;
}


#pragma mark -


/**********************************************************************************************/
void Text_Stream::PutIt( const UChar* inCh, tslen inLen )
{
	if( !inLen )
		return;

	vuint32 ToWrite = vuint32( inLen );
	
	if( ToWrite == vuint32(-1) )
		ToWrite = (vu_strlen(inCh)/* + 1*/) * sizeof(UChar);
	else
		ToWrite = inLen * sizeof(UChar);

	vuint32 Got = DoPut( inCh, ToWrite );
	
	if( Got != ToWrite )
	{
		//	Cannot write as many as needed.
		//	So, let the derived class defines the behaviour for this case.
		Overflow();
	}
	else if( Got == vuint32(-1) )
	{
		// We can reach here in case when, for example, disk is full 
		// (for the file-based text stream) or socket is closed or 
		// connection was reset (for the socket-based stream) so its 
		// up to user to know what to do in this case. Again, call 
		// the derived class' implementation...
		PutFailed();
	}
}


/**********************************************************************************************/
void Text_Stream::Overflow( void )
{
	// TODO: write the correct reaction
}


/**********************************************************************************************/
void Text_Stream::PutFailed( void )
{
	// TODO: write the correct reaction
}


/**********************************************************************************************/
void Text_Stream::Underflow( void )
{
	// TODO: write the correct reaction
}


/**********************************************************************************************/
void Text_Stream::GetFailed( void )
{
	// TODO: write the correct reaction
}


/**********************************************************************************************/
UChar Text_Stream::GetChar( void )
{
	UChar ch;

	do
	{
		vuint32 Size = DoGet(&ch, sizeof(UChar));
		if( Size != sizeof(UChar) )
		{
			//	Cannot read as many as needed.
			//	So, let the derived class defines the behaviour for this case.
			Underflow();
		}
		else if( Size == vuint32(-1) )
		{
			GetFailed();
		}

#if FBL_MAC
		
		// To get the same behaviour on Mac just replace \r with \n.
		if( ch == CR )
		{
			ch = LF;
		}

#endif // FBL_MAC
	}
	// Skip CR codes
	while(ch == CR);

	return ch;
}


/**********************************************************************************************/
void Text_Stream::SkipSpaces( void )
{
	while( IsDelim(GetChar()) )
		;
	
	UnGet();
}


/**********************************************************************************************/
void Text_Stream::UnGet( void )
{
	Seek(sizeof(UChar), kFromCurrentBack);
}


/**********************************************************************************************/
FBL_End_Namespace
