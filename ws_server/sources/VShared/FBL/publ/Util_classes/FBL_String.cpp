/**********************************************************************************************/
/* FBL_String.cpp                                                      						  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Util_classes/FBL_String.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_IStream.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_OStream.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_StringFactory.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
const char*		 	String::sEmptyPtrA 	= nullptr; 
const UChar*	 	String::sEmptyPtr 	= nullptr; 
I_StringFactory* 	String::sFactory 	= nullptr;

const String*		String::sEmptyString = nullptr;


#pragma mark -
#pragma mark === Construction/destruction ===


/**********************************************************************************************/
const String& String::sEmpty( void )
{
	if( !sEmptyString )
		sEmptyString = new String();
	
	return *sEmptyString;
}


/**********************************************************************************************/
// Should be called on shutdown. So we get chance to destroy singlenton object(s)
// to avoid reporting about leaks.
//
void String::Cleanup( void )
{
	if( sEmptyString )
	{
		delete sEmptyString;
		sEmptyString = nullptr;
	}
}


/**********************************************************************************************/
String::String( void )
{
	mpString = sFactory->CreateString(nullptr);
	mpString->AddRef();
}


/**********************************************************************************************/
String::String( const String& inStr )
: 
	mpString( inStr.mpString->Clone() )
{
	mpString->AddRef();
}


/**********************************************************************************************/
String::String( String&& inStr )
:
	mpString(nullptr)
{
	std::swap( mpString, inStr.mpString );
}


/**********************************************************************************************/
String::String( Const_I_String_Ptr inStr )
: 
	mpString( inStr->Clone() )
{
	mpString->AddRef();
}


/**********************************************************************************************/
String::String( 
	const UChar*	inStr,
	tslen			inLength )
{
	mpString = sFactory->CreateString(inStr, inLength);
	mpString->AddRef();
}


/**********************************************************************************************/
String::String( 
	const UChar*	inStart, 
	const UChar*	inEnd )
{
	mpString = sFactory->CreateString(inStart, inEnd);
	mpString->AddRef();
}


/**********************************************************************************************/
String::String( 
	const char*		inStr, 
	tslen			inLen,
	const char*		inEncoding,
	EStringImpType	inImpType )
{
	argused1(inImpType);

	mpString = sFactory->CreateString(inStr, inLen, inEncoding);
	mpString->AddRef();
}


/**********************************************************************************************/
String::String( 
	const char*		inStr, 
	tslen			inLen,
	I_Converter*	inConverter,
	EStringImpType	inImpType )
{
	argused1(inImpType);

	mpString = sFactory->CreateString(inStr, inLen, inConverter);
	mpString->AddRef();
}


/**********************************************************************************************/
String::String( 
	tslen			inCapacity, 
	char			c, 
	tslen			inCount ) 
{
	mpString = sFactory->CreateString(inCapacity, c, inCount);
	mpString->AddRef();
}


/**********************************************************************************************/
String::String( 
	tslen			inCapacity, 
	UChar			c, 
	tslen			inCount ) 
{
	mpString = sFactory->CreateString(inCapacity, c, inCount);
	mpString->AddRef();
}


/**********************************************************************************************/
String::~String( void )
{
	// We will use this as flag for the availability of DLL.
	if( String::sFactory == nullptr )
	{
		// SPECIAL CASE: we destroy string NOT having already its DLL.
		
		// We can only enter here while destroying GLOBAL String variable.
		// We cannot release pointer now because it points 
		// to the DLL that is already unloaded from the process
		// address space. So trying to release it will raise access violation.
		// We just null it.
		// This may seems as a memory leak. But we must note 
		// that DLL is actually unloaded when process is going to terminate.
		// So the system self will free "leaked" process memory.
		mpString = nullptr;
	}
	else
	{
		if( mpString )
			mpString->Release();
	}
}


#pragma mark -
#pragma mark === Assignment === 


/**********************************************************************************************/
String&	String::operator= ( const UChar* inStr )
{ 
	// This is just workaround: mpString must not be NULL!
	if( !mpString )
		mpString = sFactory->CreateString(inStr);
	else
		mpString->SetTo(inStr);
	
	return *this;
}


/**********************************************************************************************/
String&	String::operator=( const String& inText )
{
	mpString->SetTo(inText.mpString);
	return *this;
}


/**********************************************************************************************/
String&	String::operator=( String&& inText )
{
	// This is "move =" - so inText is some tmp object and we put there this.mpString
	// to take care of "old" mpString.
	std::swap( mpString, inText.mpString );
	return *this;
}


/**********************************************************************************************/
String& String::operator= ( const char* inText )
{
	mpString->SetTo(inText);
	return *this;
}


/**********************************************************************************************/
String&	String::setTo( const String& inText )
{
	mpString->SetTo(inText.mpString);
	return *this;
}


/**********************************************************************************************/
String&	String::setTo( const UChar* inSrc, tslen inSrcLength )
{
	mpString->SetTo(inSrc, inSrcLength);
	return *this;
}


/**********************************************************************************************/
String&	String::setTo( const char* inSrc, tslen inSrcLength )
{
	mpString->SetTo(inSrc, inSrcLength);
	return *this;
}



/**********************************************************************************************/
String& String::clear( void ) 
{
	mpString->Remove();
	return *this;
}


#pragma mark -
#pragma mark === Comparison - bitwise only - for international comparison use collation === 


/**********************************************************************************************/
bool String::operator==( const String& inStr ) const
{
	int cmpres = mpString->Compare(inStr.mpString);
	
	return cmpres == 0;
}


/**********************************************************************************************/
bool String::operator!=( const String& inStr ) const
{
	int cmpres = mpString->Compare(inStr.mpString);
	
	return cmpres != 0;
}


/**********************************************************************************************/
bool String::operator>( const String& inStr ) const
{
	int cmpres = mpString->Compare(inStr.mpString);
	
	return cmpres > 0;
}


/**********************************************************************************************/
bool String::operator<( const String& inStr ) const
{
	int cmpres = mpString->Compare(inStr.mpString);
	
	return cmpres < 0;
}


/**********************************************************************************************/
bool String::operator>=( const String& inStr ) const
{
	int cmpres = mpString->Compare(inStr.mpString);
	
	return cmpres >= 0; 
}


/**********************************************************************************************/
bool String::operator<=( const String& inStr ) const
{
	int cmpres = mpString->Compare(inStr.mpString);
	
	return cmpres <= 0;
}


#pragma mark -


/**********************************************************************************************/
int String::compare( const String& inStr ) const
{
	return mpString->Compare(inStr.mpString);
}


/**********************************************************************************************/
int String::compare( const UChar* inSrc, tslen inLen ) const
{
	return mpString->Compare(inSrc, inLen);
}


/**********************************************************************************************/
int String::compare( const char* inSrc, tslen inLen ) const
{
	return mpString->Compare(inSrc, inLen);
}


/**********************************************************************************************/
bool String::operator==( const UChar* inStr ) const
{
	return mpString->Compare(inStr) == 0;
}


/**********************************************************************************************/
bool String::operator!=( const UChar* inStr ) const
{
	return !operator ==(inStr);
}


/**********************************************************************************************/
bool String::operator==( const char* inStr ) const
{
	return mpString->Compare(inStr) == 0;
}


/**********************************************************************************************/
bool String::operator!=( const char* inStr ) const
{
	return !operator ==(inStr);
}


#pragma mark -
#pragma mark === Comparison - bitwise only - for international comparison use collation === 


/**********************************************************************************************/
int String::caseCompare( const String& inStr, vuint32 inOptions ) const
{
	return mpString->CaseCompare(inStr.mpString, inOptions);
}


/**********************************************************************************************/
int String::caseCompare( const UChar* inSrc, tslen inLen, vuint32 inOptions ) const
{
	return mpString->CaseCompare(inSrc, inLen, inOptions);
}


/**********************************************************************************************/
int String::caseCompare( const char* inSrc, tslen inLen, vuint32 inOptions ) const
{
	return mpString->CaseCompare(inSrc, inLen, inOptions);
}


/**********************************************************************************************/
bool String::startsWith( const String& inStr ) const
{
	return mpString->StartsWith(inStr.mpString);
}


/**********************************************************************************************/
bool String::endsWith( const String& inStr ) const
{
	return mpString->EndsWith(inStr.mpString);
}


#pragma mark -
#pragma mark === Length & buffer operations === 


/**********************************************************************************************/
tslen String::getCapacity( void ) const 
{
	return mpString->get_Capacity();
}


/**********************************************************************************************/
tslen String::length( void ) const 
{
	return mpString->get_Length();
}


/**********************************************************************************************/
bool String::isEmpty( void ) const 
{
	// FIXME: this is workaround - mpString must not be NULL!
	if (!mpString) return true;
	return mpString->get_IsEmpty();
}


#pragma mark -


/**********************************************************************************************/
// returns read only buffer
//
const UChar* String::getBuffer( void ) const
{
	return mpString->GetBufferU();
}


/**********************************************************************************************/
// returns RW buffer. Must be release by releaseBuffer().
//
UChar* String::getBuffer( tslen inCapacity )
{
	return mpString->GetBufferU(inCapacity);
}


/**********************************************************************************************/
void String::releaseBuffer( tslen inNewLength )
{
	mpString->ReleaseBufferU(inNewLength);
}


/**********************************************************************************************/
char* String::getBufferA( tslen inCapacity )
{
	return mpString->GetBufferA(inCapacity);
}


/**********************************************************************************************/
const char* String::getBufferA( const char* inEncoding ) const
{
	return mpString->GetBufferA(inEncoding);
}


/**********************************************************************************************/
void String::releaseBufferA( tslen inNewLength )
{
	mpString->ReleaseBufferA(inNewLength);
}


#pragma mark -


/**********************************************************************************************/
const UChar* String::c_str( void ) const 
{
	return mpString->GetBufferU();
}


/**********************************************************************************************/
const UChar* String::begin( void ) const
{
	return mpString->GetBufferU();
}


/**********************************************************************************************/
const UChar* String::end( void ) const
{
	const UChar* pStr = mpString->GetBufferU();
	return pStr ? pStr + mpString->get_Length() : pStr;
}


#pragma mark -
#pragma mark === Assigment of numbers to String === 


/**********************************************************************************************/
String& String::setInt( vint32 inValue )
{
	mpString->ConvertInt_10(inValue);
	return *this;
}


/**********************************************************************************************/
String& String::setUInt( vuint32 inValue )
{
	mpString->ConvertUInt_10(inValue);
	return *this;
}


/**********************************************************************************************/
String& String::setLLong( vint64 inValue )
{
	mpString->ConvertLLong_10(inValue);
	return *this;
}


/**********************************************************************************************/
String& String::setULLong( vuint64 inValue )
{
	mpString->ConvertULLong_10(inValue);
	return *this;
}


/**********************************************************************************************/
String& String::setDouble( double inValue )
{
	mpString->ConvertDouble(inValue);
	return *this;
}


/**********************************************************************************************/
String& String::setIntAsHex ( vint32 inValue )
{
	mpString->ConvertInt_16(inValue);
	return *this;
}


/**********************************************************************************************/
String& String::setLLongAsHex( vint64 inValue )
{
	mpString->ConvertLLong_16(inValue);
	return *this;
}


#pragma mark -
#pragma mark === Appending of numbers to String === 


/**********************************************************************************************/
String& String::addInt( vint32 inValue )
{
	mpString->ConvertAndAppendInt_10(inValue);
	return *this;
}


/**********************************************************************************************/
String& String::addUInt( vuint32 inValue )
{
	mpString->ConvertAndAppendUInt_10(inValue);
	return *this;
}


/**********************************************************************************************/
String& String::addLLong( vint64 inValue )
{
	mpString->ConvertAndAppendLLong_10(inValue);
	return *this;
}


/**********************************************************************************************/
String& String::addULLong( vuint64 inValue )
{
	mpString->ConvertAndAppendULLong_10(inValue);
	return *this;
}


/**********************************************************************************************/
String& String::addDouble( double inValue )
{
	mpString->ConvertAndAppendDouble(inValue);
	return *this;
}


/**********************************************************************************************/
String& String::operator<< ( const String& inValue )
{
	mpString->Append(inValue.mpString);
	return *this;
}


#pragma mark -
#pragma mark === Extractions of numbers from String === 


/**********************************************************************************************/
vint32 String::parseInt( void ) const
{
	return mpString->ParseInt();
}


/**********************************************************************************************/
vuint32 String::parseUInt( void ) const
{
	return mpString->ParseUInt();
}


/**********************************************************************************************/
vint64 String::parseLLong( void ) const
{
	return mpString->ParseLLong();
}


/**********************************************************************************************/
vuint64 String::parseULLong( void ) const
{
	return mpString->ParseULLong();
}


/**********************************************************************************************/
double String::parseDouble( void ) const
{
	return mpString->ParseDouble();
}


#pragma mark - 
#pragma mark === Appending to string ===


/**********************************************************************************************/
String&	String::operator+=( UChar ch )
{
	mpString->Append(ch);
	return *this;
}


/**********************************************************************************************/
String&	String::operator+=( const String& inSrc )
{
	mpString->Append(inSrc.mpString);
	return *this;
}


/**********************************************************************************************/
String& String::operator+=( const char* inSrc )
{
	mpString->Append(inSrc, -1);
	return *this;
}


/**********************************************************************************************/
String&	String::operator+=( const UChar* inSrc )
{
	mpString->Append(inSrc, -1);
	return *this;
}


#pragma mark - 
#pragma mark ===  ===


/**********************************************************************************************/
String&	String::append(
	const char* inSrc,
	tslen		inSrcLength )
{
	mpString->Append(inSrc, inSrcLength);
	return *this;
}


/**********************************************************************************************/
String&	String::append(	const UChar* inSrc, tslen inSrcLen )
{
	mpString->Append(inSrc, inSrcLen);
	return *this;
}


#pragma mark -
#pragma mark === Substrings extraction ===


/**********************************************************************************************/
String String::mid( tslen inFirst, tslen inCount ) const
{
	return String(mpString->Mid(inFirst, inCount));
}


/**********************************************************************************************/
String String::left( tslen inCount ) const
{
	return String(mpString->Left(inCount));
}


/**********************************************************************************************/
String String::right( tslen inCount ) const
{
	return String(mpString->Right(inCount));
}


#pragma mark -
#pragma mark === String manipulation ===


/**********************************************************************************************/
String&	String::insert(	tslen inStart, const String& inText )
{
	mpString->Insert(inStart, inText.mpString);
	return *this;
}


/**********************************************************************************************/
String& String::remove( void )
{
	mpString->Remove();
	return *this;
}


/**********************************************************************************************/
String&	String::remove(	tslen inStart, tslen inLength )
{
	mpString->Remove(inStart, inLength);
	return *this;
}


/**********************************************************************************************/
String&	String::reverse( void )
{
	mpString->Reverse();
	return *this;
}


/**********************************************************************************************/
bool String::truncate( tslen inTargetLength )
{
	return mpString->Truncate(inTargetLength);
}


/**********************************************************************************************/
String&	String::trim( void )
{
	mpString->Trim();
	return *this;
}


/**********************************************************************************************/
String&	String::replace( 
	tslen	inStart, 
	tslen	inLength, 
	const String& inText)
{
	mpString->Replace(inStart, inLength, inText.mpString);
	return *this;
}


#pragma mark -
#pragma mark === Case manipulation ===


/**********************************************************************************************/
String&	String::toUpper( void )
{
	mpString->ToUpper();
	return *this;
}


/**********************************************************************************************/
String&	String::toLower( void )
{
	mpString->ToLower();
	return *this;
}


#pragma mark -
#pragma mark === Find operation ===


/**********************************************************************************************/
vint32 String::find( UChar ch, tslen inStart ) const 
{
	return mpString->Find(ch, inStart);
}


/**********************************************************************************************/
vint32 String::find( const UChar* str, tslen inStart ) const 
{
	return mpString->Find(str, inStart);
}


/**********************************************************************************************/
vint32 String::reverseFind( UChar ch ) const 
{
	return mpString->ReverseFind(ch);
}


/**********************************************************************************************/
vint32 String::findOneOf( const UChar* inCharSet, tslen inStart ) const 
{
	return mpString->FindOneOf(inCharSet, inStart);
}


/**********************************************************************************************/
vint32 String::findOneOf( const char* inCharSet, tslen inStart ) const 
{
	return mpString->FindOneOf(inCharSet, inStart);
}


/**********************************************************************************************/
tslen String::indexOf( const String& inStr ) const
{
	return mpString->IndexOf(inStr.mpString);
}


#pragma mark -
#pragma mark === Find operation ===


/**********************************************************************************************/
UChar String::operator[]( tslen inOffset ) const
{
	return mpString->get_CharAt(inOffset);
}


/**********************************************************************************************/
UChar String::charAt( tslen inOffset ) const
{
	return mpString->get_CharAt(inOffset);
}


/**********************************************************************************************/
String& String::setCharAt( tslen inOffset, UChar inCh)
{
	mpString->put_CharAt(inOffset, inCh);
	return *this;
}


/**********************************************************************************************/
String& String::setCharAt( tslen inOffset, char inCh)
{
	mpString->put_CharAt(inOffset, inCh);
	return *this;
}

/**********************************************************************************************/
UChar* String::getBufferOwnership( void ) 
{
	return mpString->GetBufferOwnershipU();
}


/**********************************************************************************************/
char* String::getBufferOwnershipA( void ) 
{
	return mpString->GetBufferOwnershipA();
}


#pragma mark -
#pragma mark === XML methods ===


/**********************************************************************************************/
String& String::addXmlEncoding( void )
{
	mpString->AddXmlEncoding();
	return *this;
}


/**********************************************************************************************/
String& String::removeXmlEncoding( void )
{
	mpString->RemoveXmlEncoding();
	return *this;
}


#pragma mark -
#pragma mark === JSON methods ===


/**********************************************************************************************/
String& String::addJsonEncoding( void )
{
	mpString->AddJsonEncoding();
	return *this;
}


/**********************************************************************************************/
String& String::escape( UChar inChar )
{
	mpString->Escape(inChar);
	return *this;
}


#pragma mark -


/**********************************************************************************************/
bool String::isSingleByte( void ) const 
{
	return mpString->get_IsSingleByte();
}


#pragma mark -


/**********************************************************************************************/
I_OStream& operator <<( I_OStream& inStream, const String& inStr )
{
	tslen len = inStr.length();
	inStream.put( len );
	if( len > 0 )
	{
		inStream << inStr.isSingleByte();
		if( inStr.isSingleByte() )
			inStream.put( reinterpret_cast<const void*>(inStr.getBufferA()), static_cast<vuint32>(len) );
		else
			inStream.put( reinterpret_cast<const void*>(inStr.c_str()), static_cast<vuint32>(len) * sizeof(UChar) );
	}

	return inStream;
}


/**********************************************************************************************/
I_IStream& operator >>( I_IStream& inStream, String& inStr )
{
	tslen len;
	inStream >> len;
	if( len )
	{
		bool isSingleByte;
		inStream >> isSingleByte;
		if( isSingleByte )
		{
			char* p = inStr.getBufferA(len);
			inStream.get(p, len);
			inStr.releaseBufferA(len);
		}
		else
		{
			UChar* p = inStr.getBuffer(len);
			inStream.get(p, len);
			inStr.releaseBuffer(len);
		}
	}
	else
	{
		inStr = "";
	}

	return inStream;
}


#pragma mark -


/**********************************************************************************************/
// we will check for zero end in loop, instead of calculating it by lenght().
void c2pstrcpy( 
	const char* inSrc, 
	StringPtr 	inDest, 
	vuint16		inMaxChars )
{
	if( inSrc == nullptr || inDest == nullptr )
		return;

	if( inMaxChars > 255 )
		inMaxChars = 255;
	
	char* p 	= (char*) inSrc;
	char* end 	= p + inMaxChars;
	char* pd 	= (char*)(inDest + 1);
	
	while( *p && p < end ){
		*pd++ = *p++;
	}
	
	*inDest = (vuint8)(p - inSrc);	// length of string.	
}


/**********************************************************************************************/
void p2cstrcpy( 
	const StringPtr inSrc, 
	char* 			inDest, 
	vuint16			inBuffSize )
{
	if( inBuffSize > inSrc[0] )
		inBuffSize = inSrc[0];
		
	memcpy( inDest, inSrc + 1, inBuffSize );
	inDest[inBuffSize] = 0;
}
 

/**********************************************************************************************/
FBL_End_Namespace
