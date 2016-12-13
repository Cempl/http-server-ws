/**********************************************************************************************/
/* FBL_Value_String.cpp                                                    					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Values/FBL_Value_String.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_IStream.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_OStream.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Localizable.h> 

#include <VShared/FBL/publ/Utilities/FBL_memory.h>
#include <VShared/FBL/publ/Util_Strings/FBL_ustdio.h>

#include <VShared/FBL/publ/Values/FBL_Convert.h>
//#include <VShared/FBL/publ/Values/FBL_type_traits.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
const UChar* const pNullStr		= nullptr;

// We cannot use L"", so we do trick init:
const char zero2bytes[2]		= { 0 };
const UChar* const pEmptyStr	= (UChar*) zero2bytes;


/**********************************************************************************************/
void Convert_str_str( const I_Value* inValue, I_Value* outValue );
void Convert_str_str( const I_Value* inValue, I_Value* outValue )
{
	const UChar* ps  = (const UChar*) inValue->begin();
	const UChar* pse = (const UChar*) inValue->end();

	UChar* pDest = (UChar*) outValue->begin();
	
	UChar* pd = pDest;
	UChar* pde = pd + outValue->get_Allocated();

	while( *ps && ps < pse && pd < pde )
	{
		*pd++ = *ps++; 		
	}

	*pd = 0;

	vuint32 bytes = vuint32(pd - pDest) * sizeof(UChar);
	outValue->put_ByteLength( bytes );
}


/**********************************************************************************************/
void Convert_bin_str( const I_Value* inValue, I_Value* outValue );
void Convert_bin_str( const I_Value* inValue, I_Value* outValue )
{
	Value_string_Ptr pStrValue = fbl_dynamic_cast<Value_string>( outValue );
	FBL_CHECK( pStrValue );

	if( pStrValue->get_Localizable()->get_IOConverter() )
	{
		// We want to convert using IoConverter.
		// Because bin-value was sending following IOConvert rules.
		//
		outValue->put_String( inValue->begin(), inValue->end() );
	}
	else
	{
		Convert_str_str( inValue, outValue );
	}
}


#pragma mark -
#pragma mark === Value_string ===


/**********************************************************************************************/
Value_string::Value_string( void )
:
	mpLocalizable( GetLocalizableSys().get() ),
	mIsRemote( false ) // factory ::CreateValue() right after this will assign it. Redo?  // RZ 21/12/07 
{
	Allocate( 0 ); 

	mIsSingleByte = false;
	
	mNeedConvertedIsInited = false;
	mNeedConvertToStorageEncoding = false;
}


/**********************************************************************************************/
// Value_string( 20 ) -- means that we can store 20 chars. We add 1 for end zero.
//
Value_string::Value_string( 
	tslen 				inChars, 
	I_Localizable_Ptr 	inLocalizable )
:
	mpLocalizable( inLocalizable ? inLocalizable.get() : GetLocalizableSys().get() ),
	mIsRemote( false ) // factory ::CreateValue() right after this will assign it. Redo?  // RZ 21/12/07 
{ 
	m_pStartU = nullptr;
	
	vuint32 bytes = (inChars + 1) * sizeof(UChar); 	
	Allocate( bytes ); 

	mIsSingleByte = false;

	mNeedConvertedIsInited = false;
	mNeedConvertToStorageEncoding = false;
}


/**********************************************************************************************/
Value_string::Value_string( const_reference inOther )
:
	mpLocalizable( inOther.mpLocalizable )
{
	vuint32 bytes = inOther.get_Allocated();
	Allocate( bytes );
	
	Assign( (UChar*)inOther.begin(), (UChar*)inOther.end() );

	mIsRemote = inOther.mIsRemote;
	mIsSingleByte = inOther.mIsSingleByte;

	mNeedConvertedIsInited = inOther.mNeedConvertedIsInited;
	mNeedConvertToStorageEncoding = inOther.mNeedConvertToStorageEncoding;
}


/**********************************************************************************************/
Value_string::~Value_string( void )
{
	if( m_pStartU )
		delete [] m_pStartU;
} 	
		  		  
#pragma mark -
		  
		
/**********************************************************************************************/
// This method check ONCE for this value if it need convert value from storageEncoding.
// This is used only by CompareToIndexValue() and CompareIndexValues().
// So this method must and will be called ONLY and ONLY for values that are used by an Index.
// Note, that we implement LAZY call of this method, only when value need this flag.
//
void Value_string::SetupConversionFlag( void ) const
{
	FBL_CHECK( mNeedConvertedIsInited == false ); 

	if( mpLocalizable )
	{
		const String& s = mpLocalizable->get_StorageEncoding();
		if( s.caseCompare("UTF-16", 6, 0) == 0 || s.caseCompare("UTF16", 5, 0) == 0 )
			mNeedConvertToStorageEncoding = false;
		else
			mNeedConvertToStorageEncoding = true;
	}
	else
	{
		mNeedConvertToStorageEncoding = false;
	}
	
	 mNeedConvertedIsInited = true;
} 


#pragma mark -
#pragma mark === Constructors for TESTS ===


#if FBL_TEST_CODE


/**********************************************************************************************/
Value_string::Value_string( 
	const UChar* 		inStart, 
	const UChar* 		inEnd,
    I_Localizable_Ptr 	inLocalizable )
:
	mpLocalizable( inLocalizable ? inLocalizable.get() : GetLocalizableSys().get() ),
	mIsRemote( false )
{ 
	FBL_CHECK( inEnd ? (inStart <= inEnd) : true );
	
	tslen chars = inEnd ? tslen(inEnd - inStart) 
						: (inStart ? tslen(vu_strlen(inStart) + 1) 	: 21 ); 

	m_pStartU = NULL;
	
	vuint32 bytes = chars * sizeof(UChar); 
	Allocate( bytes ); 

	Assign( inStart, inEnd );

	mIsSingleByte = false;

	mNeedConvertedIsInited = false;
	mNeedConvertToStorageEncoding = false;
}


/**********************************************************************************************/
Value_string::Value_string(
	const char* 		inStart, 
	const char* 		inEnd,
    I_Localizable_Ptr 	inLocalizable )
:
	mpLocalizable( inLocalizable ? inLocalizable.get() : GetLocalizableSys().get() ),
	mIsRemote( false )
{
	FBL_CHECK( inEnd ? (inStart <= inEnd) : true );

	tslen chars;
	if( inStart )
		chars = inEnd   ? (tslen) (inEnd - inStart - 1) 
                        : (tslen) strlen(inStart);
	else
		chars = 20; // Hmm. For tests ?

	m_pStartU = NULL;
	
	vuint32 bytes = (chars + 1) * sizeof(UChar); 
	Allocate( bytes ); 

	m_pEndU = mpLocalizable->ConvertToUnicode( 
				inStart, chars, m_pStartU, chars );
	
	FBL_CHECK( *m_pEndU == 0 );
	
	// Value still stays Unicode.
	mIsSingleByte = false;

	mNeedConvertedIsInited = false;
	mNeedConvertToStorageEncoding = false;
}


/**********************************************************************************************/
Value_string::Value_string( tslen inChars, int dummy )
:
	mpLocalizable( NULL ),
	mIsRemote( false )
{ 
	argused1(dummy);

	m_pStartU = NULL;
	
	vuint32 bytes = (inChars + 1) * sizeof(UChar); 	
	Allocate( bytes ); 

	mIsSingleByte = false;

	mNeedConvertedIsInited = false;
	mNeedConvertToStorageEncoding = false;
}


#endif // FBL_TEST_CODE
		
		
#pragma mark -
#pragma mark === I_Value ===
					

/**********************************************************************************************/
I_Value_Ptr Value_string::Clone( bool inCopyData ) const 
{ 
	//I_Value_Ptr result;
	Value_string* result;

	if( inCopyData == true )
	{
		result = new this_type( *this );
	}
	else
	{
		vuint32 chars = get_MaxChars();
		result = new this_type( (tslen)chars, mpLocalizable );
	}

	result->put_IsRemote( mIsRemote );
	result->put_IsSingleByte( mIsSingleByte );

	return result;
}


#pragma mark -

							
/**********************************************************************************************/
// Returns the size of allocated buffer in bytes (!) including end-zero byte(s).
//
vuint32 Value_string::get_Allocated( void ) const   	
{ 
	if( m_pBufferEndU > m_pStartU )
	{
		char* ps = (char*) m_pStartU;
		char* pe = (char*) m_pBufferEndU;
		
		vuint32 bytes = vuint32(pe - ps);
		return bytes;
	}
	else
	{
		return 0;
	}
}


/**********************************************************************************************/
// Change the size of allocated buffer to have inByteSize bytes (including end-zero).
// Old data are not copied.
//
bool Value_string::put_Allocated( vuint32 inByteSize )   	
{
	if( get_Allocated() != inByteSize )
	{
		if( m_pStartU )
			delete [] m_pStartU;			
		Allocate( inByteSize );
	}

	return true; 
}


/**********************************************************************************************/
// Returns the size of allocated buffer including end-zero byte(s).
//
// HAVE SENSE only for fixed string. NOT for a VarChar or Text.
//
vuint32 Value_string::get_StorageSize( void ) const   	
{ 
	vuint32 bytes = get_Allocated();

	return bytes - (mIsSingleByte ? sizeof(char) : sizeof(UChar) );
}


/**********************************************************************************************/
// This function uses memset() and count bytes, so it is safe for single-byte strings also!
//
void Value_string::put_ByteLength( vuint32 inBytes )
{
	// IS: 17/05/2007
	// FBL_CHECK and guard is added because of if we had inBytes really greater than we had
	// allocated buffer in this value we will get huge leftBytes and crash soon.
	// ( It is from Jochen db and queries. There is UTF8 db so we have DoAssignFromUTF16(
	// working. Convert return buffer_overflow - but we ignore it. Also convert 
	// return the size of the buffer which would be enough to convert UTF16 to char* without loss
	// data. We have no choice there - the size of value intsernal buffer depends on field maxsize. 
	// So we have to just ignore that overflow and work with  stripped data. 
	// Well, but we pass here inBytes - that convert result -
	// not real size of stripped data but "convert-wish-to-be" size.)
	//
	// Have to check passed inBytes to be not more than internal buffer size!
	//

	vuint32 internalBufferSize = get_Allocated();
	if( inBytes > internalBufferSize )
		inBytes = internalBufferSize;
		

	char* ps = (char*) m_pStartU;
	char* pe = ps + inBytes;

	m_pEndU = (UChar*) pe;

	// Fixed size String field should erase rest of buffer.
	// VarChar field can skip this step, because into VarChar page will be 
	// copies only string itself.
	if( this->get_Type() == kTypeString )
	{
		FBL_CHECK( (char*)m_pBufferEndU >= pe );
		//Guard
		if( (char*)m_pBufferEndU >= pe )
		{
			vuint32 leftBytes = vuint32( (char*)m_pBufferEndU - pe );
			memset( pe, 0, leftBytes );
		}
	}
}


/**********************************************************************************************/
vuint32 Value_string::get_ByteLength( void ) const 
{
	vuint32 res = ( mIsSingleByte ) ? vuint32( (char*)m_pEndU - (char*)m_pStartU )
    								: vuint32(m_pEndU - m_pStartU) * sizeof(UChar);

	return res;
}


/**********************************************************************************************/
// Returns in range 0..256, including 1-byte length prefix.
//
vuint32 Value_string::get_ByteLengthForIndex( void ) const 
{
	vuint32 lenBytes = get_ByteLength();
	
	// in the index we store strings as a pascal string.
	// i.e. first byte is used for LengthInBytes. Note, not in chars!
	
	// so we have 255 bytes for DATA.
	// if this is UTF16 - then this is 127 chars.
	
	if( lenBytes > 255 )
		lenBytes = (mIsSingleByte) ? 127 : 255;
    
	return 1 + lenBytes;
}


/**********************************************************************************************/
VALUE_CATEGORY Value_string::get_Category( void ) const   
{ 
	return vcString; 
}


/**********************************************************************************************/
bool Value_string::get_IsNull( void ) const 
{ 
	return false; 
}


/**********************************************************************************************/
void Value_string::put_IsNull( bool inValue )  	
{
	if( inValue )
	{
		m_pEndU = m_pStartU;	// clear data also.

		if( m_pEndU )
			*m_pEndU = 0;
	}	
}


/**********************************************************************************************/
vuint32 Value_string::get_Length( void ) const  
{
	vuint32 res = ( mIsSingleByte ) ? vuint32( (char*)m_pEndU - (char*)m_pStartU )
									: vuint32( m_pEndU - m_pStartU );

	return res;
}


/**********************************************************************************************/
// Returns the max number of chars that can hold this value.
// It is the same as was specified on field creation: String[40], so returns 40.
//
vuint32 Value_string::get_MaxChars( void ) const 
{ 
	vuint32 bytes = get_Allocated();

	return bytes / sizeof(UChar) - 1;
}


/**********************************************************************************************/
VALUE_TYPE Value_string::get_Type( void ) const  
{ 
	return kTypeString; 
}


#pragma mark -


/**********************************************************************************************/
const char* Value_string::begin( void ) const 
{ 
	return reinterpret_cast<char*>(m_pStartU);
}


/**********************************************************************************************/
const char* Value_string::end( void ) const 
{ 
	return reinterpret_cast<char*>(m_pEndU); 
}


/**********************************************************************************************/
void Value_string::Assign( const I_Value& inValue ) 
{
	DoAssign( inValue );
}


/**********************************************************************************************/
// Index store strings in the pascal style.
// The first byte is the length in bytes.
// The next bytes keeps the string itself without end zero. 
//
// If INDEX has single byte encoding, then THIS value already is a converted string.
// 
vuint32 Value_string::CopyToIndex( void* outBuffer, bool inSwapBytes ) const 
{
	argused1(inSwapBytes);

	vuint32 lenBytes = get_ByteLengthForIndex() - 1; // e.g. for 10 UTF-16 chars we get here 20 bytes.
	vuint8* p = (vuint8*) outBuffer;
	
	*p = vuint8( lenBytes );

	// Copy the string without end zero.
	memcpy( p + 1, begin(), lenBytes );
    
    return 1 + lenBytes;
    
// RZ 2014-08-02  Minor ISSUE ???
// So if we have UTF16 string, longer of 256 bytes, then we get here lenBytes = 255
// although 254/2=127 UChars only we have to copy. But we copy yet 1 byte extra from 128th Uchar.
// FIX seems can be in get_ByteLengthForIndex() of Value_string
// to return MaxLength of UTF16 string 254, but not 255
//	AFTER such fix: new strings will use 1 byte less. Existed strings from index will work as before.
//  So it seems we are safe ...
}


/**********************************************************************************************/
// See comment of Value_string::CopyToIndex() for details.
// 
void Value_string::CopyFromIndex( const void* inBuffer, bool inSwapBytes ) 
{
	argused1(inSwapBytes);

	vuint8* p = (vuint8*) inBuffer;

	vuint32 lenBytes = vuint32( *p );
	lenBytes = FBL::Min( lenBytes, this->get_Allocated() );

	memcpy( m_pStartU, p + 1, lenBytes ); 

	m_pEndU = (UChar*)( ((char*) m_pStartU) + lenBytes );

	put_IsNull( false ); // index contains only non-NULL values.
}


#pragma mark -


/**********************************************************************************************/
int Value_string::Compare( 
	const I_Value& 	inOther,
	COMPARE_TYPE 	inCompareType ) const 
{
	FBL_CHECK( get_Type() == inOther.get_Type() ); 
	
	vint32 res = 0;
	switch( inCompareType  )
	{
		case kBinaryCompare:
		{
			tslen thisLength = (tslen) get_ByteLength();
			tslen inOtherLength = (tslen) inOther.get_ByteLength();
			
			if( thisLength > inOtherLength )
				res = 1;
			else if( thisLength < inOtherLength )
				res = -1;
			else
				res = memcmp( m_pStartU, inOther.begin(), thisLength );
		} break;
	
		case kNatureCompare:
		{
			I_Collator* pCollator = mpLocalizable->get_Collator();
	
			res = vu_strcoll( 	m_pStartU,	
								(tslen) get_Length(),
								(const UChar*) inOther.begin(), 
								(tslen) inOther.get_Length(),
								pCollator );
		} break;		
	}
	
	return res;
}


/**********************************************************************************************/
// In the index we keep strings in the pascal style.
// So we have in 'this' a value with ZERO END.
// and we have in the 'inOther' pascal string with {Length:Data}
//
int Value_string::CompareToIndexValue( 
	Const_I_Value_Ptr 	inTestValue,		// C-string
	const void* 		inIndexValue,		// Pascal-string
	vuint32				inParam,			// [LimitLength], ZERO to ignore.
	bool				inSwapBytes ) const 
{
	argused1(inSwapBytes);
	int res = 0;

	// get params of str1:
    // We use get_ByteLengthForIndex() to get length truncated to 255 data-bytes for long strings.
    //
	const UChar* pStr1	= (const UChar*) inTestValue->begin();
	vuint32 Len1InBytes = inTestValue->get_ByteLengthForIndex() - 1;
	tslen   Len1InChars = tslen( mIsSingleByte ? Len1InBytes : Len1InBytes / sizeof(UChar) );


	// get params of str2:
	vuint8* pPascalStr2 = (vuint8*) inIndexValue;

	const UChar* pStr2 = (const UChar*) (pPascalStr2 + 1);
	vuint8 Len2InBytes = *pPascalStr2;

	// -----------------
	if( mNeedConvertedIsInited == false )	// LAZY init of the mNeedConvertToStorageEncoding flag.
		SetupConversionFlag();

	if( mNeedConvertToStorageEncoding )
	{
		UChar indexStr[256];
		
		UChar* pEnd = mpLocalizable->ConvertToUnicode(
										(char*) pStr2, 	Len2InBytes, 
										indexStr, 		256,
										mpLocalizable->get_StorageConverter());										
		FBL_CHECK( *pEnd == 0 );
		argused1( pEnd );

		tslen  Len2InChars = tslen( mIsSingleByte ? Len2InBytes : (pEnd-indexStr) );

		// -----------------
		// Now we need to correct lengths if inParam presents
		if( inParam )
		{
			tslen LimitLen = tslen( inParam );

			if( Len2InChars > LimitLen )
				Len2InChars = LimitLen;
			
			if( Len1InChars > LimitLen )
				Len1InChars = LimitLen;
		}

		res = vu_strcoll( 	pStr1,	  Len1InChars, 
							indexStr, Len2InChars, 
							mpLocalizable->get_Collator() );
	}
	else // we have UTF16 in the index.
	{
		tslen  Len2InChars = tslen( mIsSingleByte ? Len2InBytes : Len2InBytes / sizeof(UChar) );
	
		// -----------------
		// Now we need to correct lengths if inParam presents
		if( inParam )
		{
			tslen LimitLen = tslen( inParam );

			if( Len2InChars > LimitLen )
				Len2InChars = LimitLen;
			
			if( Len1InChars > LimitLen )
				Len1InChars = LimitLen;
		}
	
		res = vu_strcoll( 	pStr1, Len1InChars,
							pStr2, Len2InChars, 
							mpLocalizable->get_Collator() );
	}

	return res;
}


/**********************************************************************************************/
int Value_string::CompareIndexValues( 
	const void* inLeft,
	const void* inRight,
	bool		inSwapBytes ) const 
{
	argused1(inSwapBytes);

	vuint8* pPascalLeft  = (vuint8*) inLeft;
	vuint8* pPascalRight = (vuint8*) inRight;

	// Calculate length
	vuint8 Len1InBytes = *pPascalLeft;
	vuint8 Len2InBytes = *pPascalRight;

	const UChar* ps1 = (const UChar*)(pPascalLeft  + 1);
	const UChar* ps2 = (const UChar*)(pPascalRight + 1);	
	
	if( mNeedConvertedIsInited == false )	// LAZY init of the mNeedConvertToStorageEncoding flag.
		SetupConversionFlag();

	if( mNeedConvertToStorageEncoding )
	{
		UChar left [256];
		UChar right[256];
		
		UChar* pLeftEnd = mpLocalizable->ConvertToUnicode( 
										(char*)ps1, Len1InBytes, 
										left, 		256,
										mpLocalizable->get_StorageConverter());
										
		FBL_CHECK( *pLeftEnd == 0 );
		argused1( pLeftEnd );								

		vuint8 Len1InChars = vuint8( mIsSingleByte ? Len1InBytes : (pLeftEnd-left) );
								
		UChar* pRightEnd = mpLocalizable->ConvertToUnicode( 
										(char*)ps2, Len2InBytes, 
										 right, 	256,
										 mpLocalizable->get_StorageConverter());

		FBL_CHECK( *pRightEnd == 0 );								
		argused1( pRightEnd );								

		vuint8 Len2InChars = vuint8( mIsSingleByte ? Len2InBytes : (pRightEnd-right) );

		return vu_strcoll( left, Len1InChars, right, Len2InChars, mpLocalizable->get_Collator() );
	}
	else // we have UTF16 in the index.
	{
		vuint8 Len1InChars = vuint8( mIsSingleByte ? Len1InBytes : Len1InBytes / sizeof(UChar) );
		vuint8 Len2InChars = vuint8( mIsSingleByte ? Len2InBytes : Len2InBytes / sizeof(UChar) );

	
		// compare:
		return vu_strcoll( ps1, Len1InChars, ps2, Len2InChars, mpLocalizable->get_Collator() );
	}
}


#pragma mark -


/**********************************************************************************************/
String Value_string::get_String( tslen inLimit ) const
{
	tslen myLen = get_Length();

	if( inLimit != -1 ) // some limited is specified.
	{
		if( inLimit < myLen )
			myLen = inLimit;
	}

	if( mIsSingleByte )
		return String( (char*) m_pStartU, myLen );
	else
		return String( m_pStartU, myLen );
}


/**********************************************************************************************/
void Value_string::put_String( const String& inStr )
{
	if( inStr.isSingleByte() )
	{
		const char* ps = inStr.getBufferA();
		put_String( ps, ps + inStr.length() );
	}
	else
	{
		put_String( inStr.c_str(), inStr.end() );
	}
}


/**********************************************************************************************/
UChar* Value_string::get_String( 
	UChar*	outString, 
	tslen	inBufferChars ) const
{
	// The size of input buffer (inBufferChars) cannot be -1 !
	// Because this is unsafe then to write to  the memory 
	// location pointed by outString - because we don't know the size !
	if( outString == nullptr || inBufferChars <= 0 )
		return nullptr;

	if( m_pStartU )
	{
		UChar* s = m_pStartU;

		// We stop if get end-zero of value
		//	OR "logical" end of value
		//	OR if just one byte left in buffer.
		while( (*s && s != m_pEndU) && inBufferChars-- )
			*outString++ = *s++; 

		*outString = 0;
	}
	
	return outString;
}


/**********************************************************************************************/
// then we make function to get string as char*.
// else we cannot have such function because UChar will be as char.
//
char* Value_string::get_String( 
	char*	outString, 
	tslen	inBufferChars ) const
{
	// The size of input buffer (inBufferChars) cannot be -1 !
	// Because this is unsafe then to write to  the memory 
	// location pointed by outString - because we don't know the size !
	if( outString == nullptr || inBufferChars <= 0 )
		return nullptr;

	if( mIsRemote )
	{
		if( mIsSingleByte )
		{
			tslen toCopy = Min(
						static_cast<tslen>( inBufferChars ), 
						static_cast<tslen>( get_Length() ) );

			strncpy( outString, (char*) m_pStartU, toCopy );
			outString[toCopy] = '\0';

			return outString + toCopy;
		}
		else
		{
			// User told us on connect that it will use Unicode strings.
			// But now it want single byte string. Unfortunately we cannot 
			// use here mpLocalizable, because it is NULL (GetLocalizable()
			// at the moment returns NULL on the client and user may not 
			// specified ). 
			FBL_CHECK( 0 );
			return nullptr;
		}
	}
	else
	{
		char* pEnd = mpLocalizable->ConvertFromUnicode(
							m_pStartU, static_cast<tslen>(m_pEndU - m_pStartU), 
							outString, inBufferChars );
							
		*pEnd = 0;					
	
		return pEnd;
	}	
}


#pragma mark -


/**********************************************************************************************/
// This operation DO NOT change the size of buffer. 
// it may truncate data if they are > of buffer size.
//
void Value_string::put_String( 
	const UChar* inBegin, 
	const UChar* inEnd )
{
	if ( inBegin && inEnd == nullptr )
	{
		inEnd = inBegin + vu_strlen( inBegin );
	}

	Assign( inBegin, inEnd );
}


/**********************************************************************************************/
void Value_string::put_String( 
	const char* inBegin, 
	const char* inEnd )
{
	put_String( inBegin, inEnd, nullptr );
}


#pragma mark -


/**********************************************************************************************/
// virtual:
//
void Value_string::DoAssignFromUTF16( 
	const Value_string* inValue,
	I_Converter* 		inConverter ) 
{
	bool isNull = inValue->get_IsNull();
	if( !isNull )
	{
		UErrorCode errCode = 0;

		vuint32 BytesInThisValue = inConverter->fromUChars(
			(char*)m_pStartU, (tslen)((char*)m_pBufferEndU - (char*)m_pStartU), 
			(const UChar*)inValue->begin(), inValue->get_Length(), 
			&errCode );

		put_ByteLength( BytesInThisValue ); 							
	}

	this->put_IsNull( isNull );		
}


/**********************************************************************************************/
char* Value_string::get_String( 
	char* 		outString, 
	tslen 		inBufferChars,
	I_Converter* inConverter ) const
{
	char* pEnd;
	
	if( inBufferChars > 0 )
	{
		pEnd = mpLocalizable->ConvertFromUnicode(
						m_pStartU, (tslen)(m_pEndU - m_pStartU), 
						outString, inBufferChars - 1, inConverter );

		*pEnd = 0;
	}
	else
	{
		pEnd = outString;
	}

	return pEnd;
}


/**********************************************************************************************/
void Value_string::put_String( 
	const char* inBegin, 
	const char* inEnd,
	I_Converter* inConverter )
{
	if( inBegin == nullptr )
	{
		put_IsNull( true );
		return;
	}

	if( inEnd == nullptr )
		inEnd = inBegin + strlen( inBegin );
	
	tslen len = static_cast<tslen>(inEnd - inBegin);

	if( mIsRemote )
	{
		// IS: 03.02.2010
		// http://www.valentina-db.com/bt/view.php?id=4915
		// get_MaxChars() returns actually get_Allocated()*sizeof(UChar).
		// We need just not overwrite the allocated memory writing "len" bytes
		// into m_pStartU.
		// Note:
		// mIsSingleByte actually means - "not UTF16". But mIsSingleByte == true for UTF8!
		// In other words we allocated twice-byte buffer already (a lot of reasons).
		// It should be enough to store any string (even if each symbol takes 2 bytes
		// in UTF8 like it is in UTF16).
		// The only problem can be here:
		// assume 'x' take 2 bytes in UTF16 but takes 3 bytes in UTF8 -
		// in this case the data may be stripped. But it is very rare case.
		//

		tslen minlen = Min( len, (tslen) get_Allocated() - 1 );
		
		strncpy( (char*) m_pStartU, inBegin, (vuint32) minlen );
		// Attention.
		// We need to save pointer to the end of string.
		// minlen - character count!
		char* tmp_pEnd = (char*)m_pStartU + minlen;
		m_pEndU = (UChar*) tmp_pEnd;
		
		put_IsNull( false );

		mIsSingleByte = true;
	}
	else
	{
		I_Converter* pCnv = inConverter ? inConverter : mpLocalizable->get_IOConverter();
		if( pCnv == nullptr ) 
			pCnv = GetLocalizableSys()->get_IOConverter(); // sys settings
			
		// THIS function can assume that inBegin string can be bigger of value buffer.
		// Trick is that we send to ConvertToUnicode() size of buffer without end zero bytes.
		// It will convert all possible chars and we need self put end zero into returned pointer.		
		m_pEndU = mpLocalizable->ConvertToUnicode( 
					inBegin, len, m_pStartU, tslen(m_pBufferEndU - m_pStartU - 1), pCnv );
				
		*m_pEndU = 0;			


		put_IsNull( false );

		mIsSingleByte = false;
	}
}


#pragma mark -


/**********************************************************************************************/
bool Value_string::get_Boolean( void ) const
{ 
	bool result;
	Convert_Str_To_Type( m_pStartU, &result ); 
	return result;
}

/**********************************************************************************************/
vuint8 Value_string::get_Byte( void ) const
{ 
	vuint8 result;
	Convert_Str_To_Type( m_pStartU, &result ); 
	return result;
}

/**********************************************************************************************/
vint16 Value_string::get_Short( void ) const
{ 
	vint16 result;
	Convert_Str_To_Type( m_pStartU, &result ); 
	return result;
}

/**********************************************************************************************/
vuint16 Value_string::get_UShort( void ) const
{ 
	vuint16 result;
	Convert_Str_To_Type( m_pStartU, &result ); 
	return result;
}

/**********************************************************************************************/
vint32 Value_string::get_Medium( void ) const
{ 
	vint32 result;
	Convert_Str_To_Type( m_pStartU, &result ); 
	return result;
}

/**********************************************************************************************/
vint32 Value_string::get_Long( void ) const
{ 
	vint32 result;
	Convert_Str_To_Type( m_pStartU, &result ); 
	return result;
}

/**********************************************************************************************/
/// Get value as vuint32.
vuint32 Value_string::get_ULong( void ) const
{ 
	vuint32 result;
	Convert_Str_To_Type( m_pStartU, &result ); 
	return result;
}

/**********************************************************************************************/
vint64 Value_string::get_LLong( void ) const
{ 
	vint64 result;
	Convert_Str_To_Type( m_pStartU, &result ); 
	return result;
}

/**********************************************************************************************/
vuint64 Value_string::get_ULLong( void ) const
{ 
	vuint64 result;
	Convert_Str_To_Type( m_pStartU, &result ); 
	return result;
}

/**********************************************************************************************/
float Value_string::get_Float( void ) const
{ 
	float result;
	Convert_Str_To_Type( m_pStartU, &result ); 
	return result;
}

/**********************************************************************************************/
double Value_string::get_Double( void ) const
{ 
	double result;
	Convert_Str_To_Type( m_pStartU, &result ); 
	return result;
}


#pragma mark -


/**********************************************************************************************/
void Value_string::put_Boolean( bool inValue )
{ 
	char* pe = Convert_Type_To_Str( &inValue, m_pStartU, get_MaxChars() ); 
	put_ByteLength( vuint32( pe - (char*)m_pStartU ) );
}

/**********************************************************************************************/
void Value_string::put_Byte( vuint8 inValue )
{ 
	char* pe = Convert_Type_To_Str( &inValue, m_pStartU, get_MaxChars() ); 
	put_ByteLength( vuint32( pe - (char*)m_pStartU ) );
}

/**********************************************************************************************/
void Value_string::put_Short( vint16 inValue )
{ 
	char* pe = Convert_Type_To_Str( &inValue, m_pStartU, get_MaxChars() ); 
	put_ByteLength( vuint32( pe - (char*)m_pStartU ) );
}

/**********************************************************************************************/
void Value_string::put_UShort( vuint16 inValue )
{ 
	char* pe = Convert_Type_To_Str( &inValue, m_pStartU, get_MaxChars() ); 
	put_ByteLength( vuint32( pe - (char*)m_pStartU ) );
}

/**********************************************************************************************/
void Value_string::put_Medium( vint32 inValue )
{ 
	char* pe = Convert_Type_To_Str( &inValue, m_pStartU, get_MaxChars() ); 
	put_ByteLength( vuint32( pe - (char*)m_pStartU ) );
}

/**********************************************************************************************/
void Value_string::put_Long( vint32 inValue )
{ 
	char* pe = Convert_Type_To_Str( &inValue, m_pStartU, get_MaxChars() ); 
	put_ByteLength( vuint32( pe - (char*)m_pStartU ) );
}

/**********************************************************************************************/
void Value_string::put_ULong( vuint32 inValue )
{ 
	char* pe = Convert_Type_To_Str( &inValue, m_pStartU, get_MaxChars() ); 
	put_ByteLength( vuint32( pe - (char*)m_pStartU ) );
}

/**********************************************************************************************/
void Value_string::put_LLong( vint64 inValue )
{ 
	char* pe = Convert_Type_To_Str( &inValue, m_pStartU, get_MaxChars() ); 
	put_ByteLength( vuint32( pe - (char*)m_pStartU ) );
}

/**********************************************************************************************/
void Value_string::put_ULLong( vuint64 inValue )
{ 
	char* pe = Convert_Type_To_Str( &inValue, m_pStartU, get_MaxChars() ); 
	put_ByteLength( vuint32( pe - (char*)m_pStartU ) );
}

/**********************************************************************************************/
void Value_string::put_Float( float inValue )
{ 
	char* pe = Convert_Type_To_Str( &inValue, m_pStartU, get_MaxChars() ); 
	put_ByteLength( vuint32( pe - (char*)m_pStartU ) );
}

/**********************************************************************************************/
void Value_string::put_Double( double inValue )
{ 
	char* pe = Convert_Type_To_Str( &inValue, m_pStartU, get_MaxChars() ); 
	put_ByteLength( vuint32( pe - (char*)m_pStartU ) );
}


#pragma mark -


/**********************************************************************************************/
void Value_string::LocateEnd( void )
{
	m_pEndU = m_pStartU;

	if( get_IsSingleByte() )
	{
		char* pe = (char*) m_pEndU;
		while( *pe != 0 )
		{
			++pe;
		}
		m_pEndU = (UChar*)pe;
	}
	else
	{
		while( *m_pEndU != 0 )
		{
			++m_pEndU;
		}
	}
} 


#pragma mark -


/**********************************************************************************************/
// Allocate buffer having inBytes including end-zero.
// Prev buffer must be deleted before it is called.
//
void Value_string::Allocate( vuint32 inBytes )
{
	if( inBytes == 0 )
	{
		m_pStartU = m_pEndU = m_pBufferEndU = nullptr;
		return;
	}

	// Allocate memory for value.
	m_pStartU = m_pEndU = (UChar*) new char[ inBytes ];

	// Initialize pointers.
	m_pBufferEndU = (UChar*) ((char*)m_pStartU + inBytes );

	// init space of value:
	memset( m_pStartU, 0, inBytes );
}


/**********************************************************************************************/
const UChar* Value_string::Assign( 
	const UChar* inStart, 
	const UChar* inEnd ) 
{
	FBL_CHECK( inEnd ? (inStart <= inEnd) : true );

	if( inStart == nullptr )
	{
		put_IsNull( true );
	}
	else
	{
		DoAssign( inStart, inEnd );
		put_IsNull( false );
	}

	return m_pStartU;
}


/**********************************************************************************************/
// non-virtual:
//
void Value_string::DoAssign( const I_Value& inValue ) 
{
	try
	{
		if( inValue.get_IsNull() )
		{
			put_IsNull( true );
		}
		else
		{
			put_IsNull( false );

			if( get_Type() == inValue.get_Type() )
				DoAssign( (UChar*)inValue.begin(), (UChar*)inValue.end() );
			else
				ConvertValue( &inValue,	this );
		}
	}
	catch(...)
	{
		// RZ TODO: I think we need LOG this error.
	}
}


/**********************************************************************************************/
// non-virtual:
//
void Value_string::DoAssign( 
	const UChar* inStart, 
	const UChar* inEnd) 
{
	// Calculate characters on input string.
	vuint32 chars = inEnd ? vuint32(inEnd - inStart)
						: (inStart ? vu_strlen(inStart) : 0);
	if( chars )
	{
		chars = Min( (vuint32)( m_pBufferEndU - m_pStartU - 1 ), chars);		
		vu_memcpy( m_pStartU, inStart, tslen(chars) );
	}

	m_pEndU = m_pStartU + chars;
	*m_pEndU = 0;
}


#pragma mark -


/**********************************************************************************************/
// This function is only used in server code and never should be called from client.
//
void Value_string::From( I_IStream_Ptr inStream, bool inBlock )
{
	argused1(inBlock);

	vuint32 len;
	inStream->get( len );
	if( len )
	{
		bool isSingleByte;
		*inStream >> isSingleByte;
		
		// Make sure value is large enought to hold the whole string.
		vuint32 newByteLen = (len + 2) * (mIsSingleByte ? 1 : 2);
		if( get_MaxChars() < len)
			Allocate(newByteLen);

		// If we have I/O converter != NULL then
		// client must send us strings in the single byte encoding.
		if( mpLocalizable && mpLocalizable->get_IOConverter() && isSingleByte )
		{
			// Receive singlebyte string.
			MemPtr<char> mem(len);
			inStream->get( (void*) mem.begin(), (vuint32)mem.get_Size() );

			// Convert it to Unicode.
			// IS: 14/03/2006
			// We use get_MaxChars()+1 because source buffer has no terminator
			// so we need convert exactly len symbols.
			m_pEndU = mpLocalizable->ConvertToUnicode( 
						mem, (tslen) mem.get_Size(), m_pStartU, (tslen) get_MaxChars() + 1 );
		}
		else if( !isSingleByte )
		{
			// Client send us text as Unicode. 
			// So we do not need to convert it.
			// Receive unicode string.
			//FBL_CHECK(len <= get_Length());	// wrong idea
			inStream->get((void*)m_pStartU, len * sizeof(UChar));
			m_pEndU = m_pStartU + len;
			*m_pEndU = 0;

			// We can convert Unicode strings which have different byte ordering.
			if( mpLocalizable && mpLocalizable->get_IOConverter() )
			{
				// Convert it to Unicode.
				vint32 byteSize = (vint32)( len * sizeof(UChar) );
				
				m_pEndU = mpLocalizable->ConvertToUnicode( 
							(char*)m_pStartU, byteSize, m_pStartU, byteSize);
			}
		}
	}
	else
	{
		Assign(static_cast<UChar*>(nullptr));
	}
}


/**********************************************************************************************/
void Value_string::From( I_PacketRcv* inPacket, bool inBlock )
{
	argused1(inBlock);

	// We guarantee that mIsRemote could be presented only in the client!!!
	if( mIsRemote )
	{
		From_OnClientSide( inPacket );
	}
	else
	{
		From_OnServerSide( inPacket );
	}
}


/**********************************************************************************************/
void Value_string::From_OnClientSide( I_PacketRcv* inPacket )
{
	//
	// From Server To Client. (Client side now).
	//	

	vuint32 len = inPacket->get_ULongParam();	// Length in bytes
	if( len )
	{
		bool isSingleByte = inPacket->get_BoolParam();

		// Make sure value is large enough to hold the whole string.
		vuint32 newByteLen = (len + 2);
		if( get_Allocated() < newByteLen )
			put_Allocated(newByteLen);

		mIsSingleByte = isSingleByte;

		// Server send value already prepared to be used in client as is.
		// (Convert to the client encoding was done before sending on the server side.)
		//
		inPacket->get_BinaryParam( (void*)m_pStartU, len );
		m_pEndU = (UChar*)((char*)m_pStartU + len);						
		*m_pEndU = 0;
	}
	else
	{
		// Empty data.
		//Assign(static_cast<UChar*>(0));
		put_String( "" );
		mIsSingleByte = inPacket->get_IsSingleByte();
	}
}


/**********************************************************************************************/
void Value_string::From_OnServerSide( I_PacketRcv* inPacket )
{
	// From Client to Server. (Server side now).
	//
	vuint32 len = inPacket->get_ULongParam();	// Length in bytes
	if( len )
	{
		bool isSingleByte = inPacket->get_BoolParam();

		// Make sure value is large enough to hold the whole string.
		vuint32 newByteLen = (isSingleByte) ?  (len + 1) * sizeof(UChar) : (len + sizeof(UChar));
		if( get_Allocated() < newByteLen )
			put_Allocated(newByteLen);

		// Client promised encoding on connect and therefor we have converter in
		// each packet accordingly to that promise. 
		// So we use it instead of localizable properties to unpack the data from the packet.
		
		if( isSingleByte )
		{
			// Single byte encoding from client.

			// Receive string.
			MemPtr<char> mem(len);
			inPacket->get_BinaryParam( (void*) mem.begin(), len );

			// Convert single byte (getting from the packet) to Unicode ( any kernel
			// object operates in UTF16 only ).
			// We use get_MaxChars()+1 because source buffer has no terminator
			// so we need convert exactly len symbols.
			//m_pEndU = mpLocalizable->ConvertToUnicode( 
			//			mem, (tslen) len, m_pStartU, (tslen) get_MaxChars() + 1 );

			// IS: 2011-06-07
			// http://www.valentina-db.com/bt/view.php?id=5549
			// Value is a special thing. It should use localizable converter in order
			// to get IOEncoding working.

			I_Converter* pConverter = mpLocalizable->get_IOConverter();
			m_pEndU = mpLocalizable->ConvertToUnicode( 
										mem, 
										(tslen) len, 
										m_pStartU, 
										(tslen) 
										get_MaxChars() + 1,
										(pConverter) ? pConverter : inPacket->get_Converter() );
		}
		else
		{
			From_OnServerSide_U( inPacket, len );
		}
	}
	else
	{
		// Empty data.
		//Assign(static_cast<UChar*>(0));
		put_String( "" );
		// IS: 12.03.2010
		// Florian trap here - server receive a value from his V4MD app
		// and kernel string-field value becomes marked as single-byte.
		// but actually it should not be single byte - never on server side.
		//mIsSingleByte = inPacket->get_IsSingleByte();
	}
}


/**********************************************************************************************/
void Value_string::From_OnServerSide_U( I_PacketRcv* inPacket, vuint32 inByteLength  )
{
	// ATTENTION!
	// Don' call it directly! Call order is important.
	// It is just helper for From_OnServerSide. 

	I_Converter* pConvertor = inPacket->get_Converter();
	if( pConvertor )
	{
		// Client and server both unicoded but on the different platform.
		inPacket->get_BinaryParam((void*)m_pStartU, inByteLength);
		m_pEndU = (UChar*)((char*)m_pStartU + inByteLength);		
		*m_pEndU = 0;

		// Convert it to Unicode using packet converter.
		UErrorCode	Status = U_ZERO_ERROR;					
		pConvertor->toUChars(
						m_pStartU, 
						inByteLength, 
						(const char*)m_pStartU, 
						inByteLength, 
						&Status);

		if( !U_SUCCESS(Status) )
			THROW_ERROR_INTERNAL( ERR_INTERNAL_ERROR ); // It is hard to imagine we could be here.
	}
	else
	{
		// Just copy as is:
		// Client and server both unicoded on the same platform.
		inPacket->get_BinaryParam((void*)m_pStartU, inByteLength );
		m_pEndU = (UChar*)((char*)m_pStartU + inByteLength);		
		*m_pEndU = 0;				
	}
}


#pragma mark -


/**********************************************************************************************/
void Value_string::To( I_OStream_Ptr inStream, bool inBlock ) const
{
	argused1(inBlock);

	vuint32 len = get_Length();
	inStream->put(len);
	if( len )
	{
		inStream->put(mIsSingleByte);
		if( mIsSingleByte )
		{
			inStream->put((void*)m_pStartU, len);
		}
		else
		{
			// We can convert Unicode strings which have different byte ordering.
			
			if( mIsRemote )
			{
				// Client do not convert anything!
				inStream->put((void*)m_pStartU, len * sizeof(UChar));
			}
			else // Convert it to Unicode.
			{
				// This branch works for SERVER only. 
				vuint32 byteSize = len * sizeof(UChar);
				MemPtr<char> mem( byteSize );

				mpLocalizable->ConvertFromUnicode(
					m_pStartU, (vint32)get_Length(), mem.begin(), (vint32)mem.get_Size());

				inStream->put((void*)mem.begin(), (vuint32)mem.get_Size());
			}
		}
	}
}


/**********************************************************************************************/
void Value_string::To( I_PacketSnd* inPacket, bool inBlock ) const
{
	argused1(inBlock);

	if( mIsRemote )
	{
		To_OnClientSide( inPacket );
	}
	else
	{
		To_OnServerSide( inPacket );
	}
}


/**********************************************************************************************/
void Value_string::To_OnClientSide( I_PacketSnd* inPacket ) const
{
	//
	// Client To Server.
	//

	FixParamCount fixParamCount( inPacket );
	
	// byte length - not symbol count.
	vuint32 len = get_ByteLength();	
	if( len )
	{
		// Client sends value as is - always.
		inPacket->put_ULongParam( len );
		inPacket->put_BoolParam( mIsSingleByte );
		inPacket->put_BinaryParam((void*)m_pStartU, len );
	}
	else
	{
		inPacket->put_ULongParam( 0 );
	}
}


/**********************************************************************************************/
void Value_string::To_OnServerSide( I_PacketSnd* inPacket ) const
{
	//
	// Server To Client.
	//

	FixParamCount fixParamCount( inPacket );
	
	// byte length - not symbol count.
	vuint32 lenBytes = get_ByteLength();	
	if( lenBytes )
	{
		// If client requires single-byte result.
		if( inPacket->get_IsSingleByte() )
		{				
			if( mIsSingleByte )
			{
				// Seems to be impossible case because kernel always works using UTF16.
				inPacket->put_ULongParam( lenBytes );
				inPacket->put_BoolParam( true );
				inPacket->put_BinaryParam( (void*) m_pStartU, lenBytes );
			}
			else
			{
				// To single byte encoding. There is no end zero in this buffer.
				MemPtr<char> mem( lenBytes );
				

				// IS: 2011-06-07
				// http://www.valentina-db.com/bt/view.php?id=5549
				// Value is a special thing. It should use localizable converter in order
				// to get IOEncoding working.

				//char* pEnd = mpLocalizable->ConvertFromUnicode(
				//	m_pStartU, (vint32)get_Length(), mem.begin(), (vint32)lenBytes, inPacket->get_Converter() );

				I_Converter* pConverter = mpLocalizable->get_IOConverter();
				char* pEnd = mpLocalizable->ConvertFromUnicode(
												m_pStartU, 
												(vint32)get_Length(), 
												mem.begin(), 
												(vint32)lenBytes,
												(pConverter) ? pConverter : inPacket->get_Converter() );


				inPacket->put_ULongParam( vuint32(pEnd - mem.begin()) );
				inPacket->put_BoolParam( true );
				inPacket->put_BinaryParam( (void*) mem.begin(), vuint32(pEnd - mem.begin()) );
			}
		}
		else
		{
			// If client requires Unicoded result.
			To_OnServerSide_U( inPacket, lenBytes );
		}
	}
	else
	{
		inPacket->put_ULongParam( 0 );
	}
}


/**********************************************************************************************/
void Value_string::To_OnServerSide_U( I_PacketSnd* inPacket, vuint32 inByteLength ) const
{
	// ATTENTION!
	// Don' call it directly! Call order is important.
	// It is just helper for To_OnServerSide. 

	I_Converter* pConvertor = inPacket->get_Converter();
	if( pConvertor )
	{
		// Client and server both unicoded but on the different platform.
		MemPtr<char> mem(inByteLength * sizeof(UChar));

		UErrorCode	status = U_ZERO_ERROR;
		pConvertor->fromUChars( mem, (vuint32)mem.get_Size(), m_pStartU, inByteLength, &status );
		if( !U_SUCCESS(status) )
			THROW_ERROR_INTERNAL( ERR_INTERNAL_ERROR ); // It is hard to imagine we could be here.

		inPacket->put_ULongParam( (vuint32)mem.get_Size() );
		inPacket->put_BoolParam(false);	
		inPacket->put_BinaryParam(reinterpret_cast<void*>(mem.begin()), (vuint32) mem.get_Size() );
	}
	else
	{
		// Client and server both unicoded on the same platform.
		inPacket->put_ULongParam( inByteLength );
		inPacket->put_BoolParam( false );
		inPacket->put_BinaryParam((void*)m_pStartU, inByteLength );
	}
}


#pragma mark -


//#if FBL_TEST_CODE


/**********************************************************************************************/
void Value_string::Init( void )  
{ 
	vuint32 Len = get_MaxChars(); 

	if( Len )
	{
		if( mIsSingleByte )
		{
			Len /= sizeof(char);
			memset( (char*)m_pStartU, char('a'), static_cast<vint32>(Len) );

			m_pEndU = (UChar*)( (char*)m_pStartU + Len );
			*m_pEndU = 0;
		}
		else
		{
			Len /= sizeof(UChar);
			
			vu_memset( m_pStartU, UChar('a'), static_cast<vint32>(Len) );
			
			m_pEndU = m_pStartU + Len;
			*m_pEndU = 0;
		}
		
		put_IsNull( false );
	}
	else
	{
		// We don't go on this way!!!
		FBL_CHECK( false );
		put_IsNull( true );
	}
}


/**********************************************************************************************/
void Value_string::Increment( void )  		
{ 
	if( m_pStartU )
	{ 
		if( mIsSingleByte )
		{
			char c = *((char*)m_pStartU);
			c = (c < 'z') 
				? (c + 1)
				: ('a');

			memset( (char*)m_pStartU, c, static_cast<tslen>(get_Length()) );
		}
		else
		{
			UChar c = *m_pStartU;
			c = (c < UChar('z')) 
				? UChar(c + 1)
				: UChar('a');

			vu_memset( m_pStartU, c, static_cast<tslen>(get_Length()) );
		}
	}
}

//#endif /// FBL_TEST_CODE


#pragma mark -
#pragma mark === Value_string_null ===


/**********************************************************************************************/
Value_string_null::Value_string_null( void ) 
:
	Value_string()
{		  
	mIsNull	= true;
}


/**********************************************************************************************/
Value_string_null::Value_string_null( 
	tslen 				inChars, 
	I_Localizable_Ptr 	inLocalizable ) 
:
	Value_string( inChars, inLocalizable )
{		  
	mIsNull	= true;
}


/**********************************************************************************************/
Value_string_null::Value_string_null( const_reference inOther ) 
:
	Value_string( inOther )
{
	put_IsNull( inOther.get_IsNull() );	
}


/**********************************************************************************************/
Value_string_null::~Value_string_null( void )
{
} 	
	

#pragma mark -
#pragma mark === Constructors for TESTS ===


#if FBL_TEST_CODE


/**********************************************************************************************/
Value_string_null::Value_string_null( 
	const UChar* 		inStart,
	const UChar* 		inEnd,
    I_Localizable_Ptr 	inLocalizable )
:
	Value_string( inStart, inEnd, inLocalizable )
{
	put_IsNull( inStart == NULL );
}


/**********************************************************************************************/
Value_string_null::Value_string_null(
	const char* 		inStart,
	const char* 		inEnd,
    I_Localizable_Ptr 	inLocalizable )
:
	Value_string( inStart, inEnd, inLocalizable )
{
	put_IsNull( inStart == NULL );
}


#endif // FBL_TEST_CODE


#pragma mark -
							

/**********************************************************************************************/
I_Value_Ptr Value_string_null::Clone( bool inCopyData ) const  
{ 
	//I_Value_Ptr result;
	Value_string* result;

	if( inCopyData == true )
	{
		result = new this_type( *this );
	}
	else
	{
		vuint32 chars = get_MaxChars();
		result = new this_type( (tslen) chars, mpLocalizable ); 
	}

	result->put_IsRemote( mIsRemote );
	result->put_IsSingleByte( mIsSingleByte );

	return result;
}


/**********************************************************************************************/
bool Value_string_null::get_IsNull( void ) const 
{ 
	return mIsNull; 
}


/**********************************************************************************************/
void Value_string_null::put_IsNull( bool inValue )  	
{ 
	mIsNull = inValue;
	
	if( inValue )
	{
		m_pEndU = m_pStartU;	// clear data also.

		// value_text can have NULL pointers. so we guard this by if()
		if( m_pEndU )
			*m_pEndU = 0;
	}
}


/**********************************************************************************************/
int Value_string_null::Compare( 
	const I_Value& 	inOther,
	COMPARE_TYPE 	inCompareType ) const 
{
	FBL_CHECK( get_Type() == inOther.get_Type() );

	bool otherIsNull = inOther.get_IsNull();
    
	if( otherIsNull || mIsNull )
	{
		// ... we check only isNull flags.
		return mIsNull == otherIsNull ? 0 
									  : ( mIsNull == true ? -1 : 1 );		
	}
	else
	{
		// ... have not null string - compare chars.
		return Value_string::Compare( inOther, inCompareType );
	}
}


#pragma mark -


/**********************************************************************************************/
void Value_string_null::put_Boolean( bool inValue )
{ 
	put_IsNull( false );
	char* pe = Convert_Type_To_Str( &inValue, m_pStartU, get_MaxChars() ); 
	put_ByteLength( vuint32( pe - (char*)m_pStartU ) );
}

/**********************************************************************************************/
void Value_string_null::put_Byte( vuint8 inValue )
{ 
	put_IsNull( false );
	char* pe = Convert_Type_To_Str( &inValue, m_pStartU, get_MaxChars() ); 
	put_ByteLength( vuint32( pe - (char*)m_pStartU ) );
}


/**********************************************************************************************/
void Value_string_null::put_Short( vint16 inValue )
{ 
	put_IsNull( false );
	char* pe = Convert_Type_To_Str( &inValue, m_pStartU, get_MaxChars() ); 
	put_ByteLength( vuint32( pe - (char*)m_pStartU ) );
}

/**********************************************************************************************/
void Value_string_null::put_UShort( vuint16 inValue )
{ 
	put_IsNull( false );
	char* pe = Convert_Type_To_Str( &inValue, m_pStartU, get_MaxChars() ); 
	put_ByteLength( vuint32( pe - (char*)m_pStartU ) );
}

/**********************************************************************************************/
void Value_string_null::put_Medium( vint32 inValue )
{ 
	put_IsNull( false );
	char* pe = Convert_Type_To_Str( &inValue, m_pStartU, get_MaxChars() ); 
	put_ByteLength( vuint32( pe - (char*)m_pStartU ) );
}

/**********************************************************************************************/
void Value_string_null::put_Long( vint32 inValue )
{ 
	put_IsNull( false );
	char* pe = Convert_Type_To_Str( &inValue, m_pStartU, get_MaxChars() ); 
	put_ByteLength( vuint32( pe - (char*)m_pStartU ) );
}

/**********************************************************************************************/
void Value_string_null::put_ULong( vuint32 inValue )
{ 
	put_IsNull( false );
	char* pe = Convert_Type_To_Str( &inValue, m_pStartU, get_MaxChars() ); 
	put_ByteLength( vuint32( pe - (char*)m_pStartU ) );
}

/**********************************************************************************************/
void Value_string_null::put_LLong( vint64 inValue )
{ 
	put_IsNull( false );
	char* pe = Convert_Type_To_Str( &inValue, m_pStartU, get_MaxChars() ); 
	put_ByteLength( vuint32( pe - (char*)m_pStartU ) );
}

/**********************************************************************************************/
void Value_string_null::put_ULLong( vuint64 inValue )
{ 
	put_IsNull( false );
	char* pe = Convert_Type_To_Str( &inValue, m_pStartU, get_MaxChars() ); 
	put_ByteLength( vuint32( pe - (char*)m_pStartU ) );
}

/**********************************************************************************************/
void Value_string_null::put_Float( float inValue )
{ 
	put_IsNull( false );
	char* pe = Convert_Type_To_Str( &inValue, m_pStartU, get_MaxChars() ); 
	put_ByteLength( vuint32( pe - (char*)m_pStartU ) );
}

/**********************************************************************************************/
void Value_string_null::put_Double( double inValue )
{ 
	put_IsNull( false );
	char* pe = Convert_Type_To_Str( &inValue, m_pStartU, get_MaxChars() ); 
	put_ByteLength( vuint32( pe - (char*)m_pStartU ) );
} 


/**********************************************************************************************/
FBL_End_Namespace
