/**********************************************************************************************/
/* FBL_Value_text.cpp                                                    					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Values/FBL_Value_Text.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_Localizable.h> 
#include <VShared/FBL/publ/Util_Strings/FBL_ustdio.h>
#include <VShared/FBL/publ/Utilities/FBL_memory.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
Value_text_null::Value_text_null( void ) 
:
	Value_string_null()
{		  
}


/**********************************************************************************************/
// We use 128 as initial size of Value_text. Each new assigment of value may change this.
// 
Value_text_null::Value_text_null( 
	I_Localizable_Ptr inLocalizable ) 
:
	Value_string_null( 128, inLocalizable )
{
}


/**********************************************************************************************/
Value_text_null::Value_text_null( const_reference inOther ) 
:
	Value_string_null( inOther )
{
	DoAssign( (UChar*)inOther.begin(), (UChar*)inOther.end() );
}


/**********************************************************************************************/
Value_text_null::~Value_text_null( void )
{
} 	
							

#pragma mark -
#pragma mark === Constructors for TESTS ===


#if FBL_TEST_CODE

/**********************************************************************************************/
Value_text_null::Value_text_null( 
	const UChar* inStart, 
	const UChar* inEnd ) 
:
	Value_string_null( (tslen)0 )
{
	if( inStart == NULL )
	{
		put_IsNull( true );
	}
	else
	{
		DoAssign( inStart, inEnd );
		put_IsNull( false );
	}
}


/**********************************************************************************************/
Value_text_null::Value_text_null( 
	const char* inStart, 
	const char* inEnd ) 
:
	Value_string_null( inStart, inEnd )
{
}


#endif // FBL_TEST_CODE


#pragma mark -


/**********************************************************************************************/
I_Value_Ptr Value_text_null::Clone( bool inCopyData ) const  
{ 
	//I_Value_Ptr result;
	Value_string* result;
	
	if( inCopyData == true )
	{
		result = new this_type( *this );
	}
	else
	{
		result = new this_type( mpLocalizable );
		result->put_Allocated( get_Allocated() );
		result->put_IsNull( true );
	}
	
	result->put_IsRemote( mIsRemote );
	result->put_IsSingleByte( mIsSingleByte );

	return result;
}


//**********************************************************************************************/
void Value_text_null::Assign( const I_Value& inValue ) 
{
	vuint32 chars;

	if( inValue.get_Category() == vcString )
		chars = inValue.get_Length();
	else
		chars = inValue.get_MaxChars();

	ResizeChars( (tslen) chars );

	Value_string_null::DoAssign( inValue );
}


/**********************************************************************************************/
void Value_text_null::put_String( 
	const UChar* inBegin, 
	const UChar* inEnd )
{
	if( inBegin == nullptr )
	{
		put_IsNull( true );
	}
	else
	{
		if ( !inEnd )
			inEnd = inBegin + vu_strlen( inBegin );
	
		DoAssign( inBegin, inEnd );

		mIsNull = false;
	}
}


/**********************************************************************************************/
void Value_text_null::put_String( 
	const char* inBegin, 
	const char* inEnd,
	I_Converter* inConverter )
{
	if ( !inEnd )
		inEnd = inBegin + strlen( inBegin );
	
	if( mIsRemote )
	{
		argused1(inConverter);

		mIsSingleByte = true;
		DoAssign( inBegin, inEnd );		
	}
	else
	{
		tslen len = tslen(inEnd - inBegin);

		UChar* begin_ptr = nullptr;
		UChar* end_ptr = nullptr;

		if( len > 0 )
		{
			I_Converter* pCnv = inConverter ? inConverter : mpLocalizable->get_IOConverter();
			if( pCnv == nullptr ) 
				pCnv = GetLocalizableSys()->get_IOConverter(); // sys settings
				
			// Allocate memory for value.
			const vuint32 uBytes = vuint32( (len + 1) * sizeof(UChar) );
			
			begin_ptr = end_ptr = new UChar[ uBytes ];
			memset( begin_ptr, 0, uBytes );

			end_ptr = mpLocalizable->ConvertToUnicode( 
						inBegin, len, begin_ptr, len + 1, pCnv );

			DoAssign( begin_ptr, end_ptr );
			
			delete [] begin_ptr;
		}
		else
		{
			DoAssign( begin_ptr, end_ptr );
		}
	}
	
	put_IsNull( false );	
}


/**********************************************************************************************/
void Value_text_null::put_Boolean( bool inValue )
{
	ResizeChars( kBoolStrMaxLen );
	Value_string_null::put_Boolean( inValue );
}


/**********************************************************************************************/
void Value_text_null::put_Byte( vuint8 inValue )
{
	ResizeChars( kUCharStrMaxLen );
	Value_string_null::put_Byte( inValue );
}


/**********************************************************************************************/
void Value_text_null::put_Short( vint16 inValue )
{
	ResizeChars( kShortStrMaxLen );
	Value_string_null::put_Short( inValue );
}


/**********************************************************************************************/
void Value_text_null::put_UShort( vuint16 inValue )
{
	ResizeChars( kUShortStrMaxLen );
	Value_string_null::put_UShort( inValue );
}


/**********************************************************************************************/
void Value_text_null::put_Medium( vint32 inValue )
{
	ResizeChars( kLongStrMaxLen );
	Value_string_null::put_Medium( inValue );
}


/**********************************************************************************************/
void Value_text_null::put_Long( vint32 inValue )
{
	ResizeChars( kLongStrMaxLen );
	Value_string_null::put_Long( inValue );
}


/**********************************************************************************************/
void Value_text_null::put_ULong( vuint32 inValue )
{
	ResizeChars( kULongStrMaxLen );
	Value_string_null::put_ULong( inValue );
}


/**********************************************************************************************/
void Value_text_null::put_LLong( vint64 inValue )
{
	ResizeChars( kLLongStrMaxLen );
	Value_string_null::put_LLong( inValue );
}


/**********************************************************************************************/
void Value_text_null::put_ULLong( vuint64 inValue )
{
	ResizeChars( kULLongStrMaxLen );
	Value_string_null::put_ULLong( inValue );
}


/**********************************************************************************************/
void Value_text_null::put_Float( float inValue )
{
	ResizeChars( kFloatStrMaxLen );
	Value_string_null::put_Float( inValue );
}


/**********************************************************************************************/
void Value_text_null::put_Double( double inValue )
{
	ResizeChars( kDoubleStrMaxLen );
	Value_string_null::put_Double( inValue );
}


#pragma mark -
#pragma mark ====== I_Serializable ======= 


/**********************************************************************************************/
void Value_text_null::From( I_IStream_Ptr inStream, bool inBlock )
{
	argused1(inBlock);

	vuint32 len;
	inStream->get( len );
	if( len )
	{
		*inStream >> mIsSingleByte;

		// If we have I/O converter != NULL then
		// client must send us strings in single 
		// byte encoding.
		if( mpLocalizable && mpLocalizable->get_IOConverter() && mIsSingleByte )
		{
			Resize(len * sizeof(UChar));
			put_ByteLength(len * sizeof(UChar));

			// Receive singlebyte string.
			MemPtr<char> mem(len);
			inStream->get(reinterpret_cast<void*>(mem.begin()), (vuint32) mem.get_Size());

			// Convert it to Unicode.
			m_pEndU = mpLocalizable->ConvertToUnicode( mem, (tslen)mem.get_Size(), m_pStartU, tslen(len + 1) );

			mIsSingleByte = false;
		}
		// Client send us text as Unicode. 
		// So we do not need to convert it.
		else
		{
			Resize(len * sizeof(UChar));
			put_ByteLength(len * sizeof(UChar));

			// Receive unicode string.
			FBL_CHECK(len <= get_Allocated());
			inStream->get((void*)m_pStartU, len * sizeof(UChar));
			m_pEndU = m_pStartU + len;
		}

		put_IsNull(false);
	}
}


#pragma mark -
#pragma mark ====== Value_string ======= 


/**********************************************************************************************/
// OVERRIDEN because for Value_text we need do resize before convert.
//
void Value_text_null::DoAssignFromUTF16( 
	const Value_string* inValue,
	I_Converter* 		inConverter ) 
{
	bool isNull = inValue->get_IsNull();
	if( !isNull )
	{
		// At first we resize this TEXT valule if needed.
		this->Resize( inValue->get_ByteLength() + 1 ); // add one byte 
	
		// Convert from UTF16 to storage encoding of this value.
		UErrorCode errCode = 0;
	
		vuint32 BytesInThisValue = inConverter->fromUChars(
									(char*)m_pStartU, (tslen)((char*)m_pBufferEndU - (char*)m_pStartU), 
									(const UChar*)inValue->begin(), inValue->get_Length(), 
									&errCode );
									
		put_ByteLength( BytesInThisValue ); 							
	}

	this->put_IsNull( isNull );		
}




#pragma mark -
#pragma mark ====== THIS CLASS API ======= 


/**********************************************************************************************/
// Low level method. It works with bytes.
//
// Differ from put_Allocated(), that it do not change size of buffer in some cases
// so we get better speed. It change size only if we ned more space or if we need
// significantly smaller buffer.
//
// This method only allocate buffer. Length of this value becomes 0.
//
void Value_text_null::Resize( vuint32 inBytes )
{
	vuint32 curr_size = get_Allocated();
	
	if( inBytes > curr_size ||					// we need more space, so grow
		inBytes < (curr_size - inBytes) )		// inBytes < (curr_size / 2), so reduse it.
	{
		put_Allocated( inBytes );
	}		
}


/**********************************************************************************************/
// Changes the length of the value_taxt using chars count.
//
void Value_text_null::ResizeChars( tslen inChars )
{
	vuint32 bytes;

/*
	if( mIsSingleByte )
		bytes = (inChars + 1) * sizeof(char);
	else
*/
		bytes = (inChars + 1) * sizeof(UChar);

	Resize( bytes );		
}


#pragma mark -


/**********************************************************************************************/
// non-virtual:
// Copy given string as is.
//
void Value_text_null::DoAssign( 
	const UChar* inStart, 
	const UChar* inEnd) 
{
	// Calculate characters on input string.
	tslen chars;
	
	if( inEnd )
		chars = tslen(inEnd - inStart);
	else
		chars = inStart ? vu_strlen(inStart) : 0;
	
	ResizeChars( chars );

	if( chars )
		vu_memcpy( m_pStartU, inStart, (vint32)chars );

	m_pEndU = m_pStartU + chars;
	*m_pEndU = 0;
}


/**********************************************************************************************/
// non-virtual:
// Copy given string as is.
//
void Value_text_null::DoAssign( 
	const char* inStart, 
	const char* inEnd) 
{
	// Calculate characters on input string.
	tslen chars;
	
	if( inEnd )
		chars = tslen(inEnd - inStart);
	else
		chars = inStart ? (tslen) strlen(inStart) : (tslen) 0;
	
	ResizeChars( chars );

	if( chars )
		memcpy( m_pStartU, inStart, (vuint32) chars );
		
	// We work with points as with single byte pointers: char*
	if( mIsSingleByte )
		m_pEndU = (UChar*)((char*)m_pStartU + chars);
	else
		m_pEndU = (UChar*)((char*)m_pStartU) + chars;
	*((char*)m_pEndU) = 0;
}


/**********************************************************************************************/
FBL_End_Namespace
