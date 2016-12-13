/**********************************************************************************************/
/* FBL_Value_Binary.cpp                                                    					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_IStream.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_OStream.h>

#include <VShared/FBL/publ/Values/FBL_Value_Binary.h>
#include <VShared/FBL/publ/Utilities/FBL_ByteToHex.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
Value_Raw::Value_Raw( void )  
: 
	mDeltaPlus( 0 ),
	mIsRemote( false )
{ 
	InitSelf(); 
}


/**********************************************************************************************/
Value_Raw::Value_Raw( vuint32 inSize, vuint32 inDeltaPlus )  
: 
	mDeltaPlus( (vuint8) inDeltaPlus ),
	mIsRemote( false )
{ 
	Alloc( inSize ); 
}

 
/**********************************************************************************************/
Value_Raw::Value_Raw( const Value_Raw& inOther )  
:
	mDeltaPlus( inOther.mDeltaPlus ),
	mIsRemote( false )
{ 
	InitSelf();

	Alloc( inOther.get_Allocated() );

	if( vuint32 len = inOther.get_Length() )
	{
		char* pb = (char*)inOther.begin();
		memcpy( m_pStart, pb, len );
		m_pEnd = m_pStart + len;
	}
}


/**********************************************************************************************/
Value_Raw::~Value_Raw( void )  
{
	if( m_pStart )
		delete [] m_pStart;	
}


#pragma mark -
 

/**********************************************************************************************/
void Value_Raw::put_ByteLength( vuint32 inNewLength )
{
	char* ps = m_pStart;
	char* pe = ps + inNewLength;

	m_pEnd = pe;

	// Fixed size Binary field should erase rest of buffer.
	// VarBinary field can skip this step, because into VarChar page will be 
	// copies only data itself.
	if( this->get_Type() == kTypeFixedBinary )
	{
		vuint32 leftBytes = vuint32( m_pBufferEnd - pe );
		memset( pe, 0, leftBytes );
	}
}


/**********************************************************************************************/
vuint32 Value_Raw::get_ByteLengthForIndex( void ) const 
{
	vuint32 len = get_ByteLength();
	
	if( len > 255 )
		len = 255;

	return len;
}


/**********************************************************************************************/
void Value_Raw::Assign( const I_Value& inValue )  
{
	if( inValue.get_IsNull() )
	{
		put_IsNull( true );
	}
	else
	{
		if( get_Type() == inValue.get_Type() )
		{
			vuint32 Len = static_cast<vuint32>(inValue.end() - inValue.begin());
			put_Data( (vuint8*)inValue.begin(), Len );
		}
		else
		{
			ConvertValue( &inValue,	this );
		}
	}
}


/**********************************************************************************************/
bool Value_Raw::put_Allocated( vuint32 inSize ) 
{
	vuint32 ByteSize = get_Allocated();

	if( inSize == ByteSize )
	{
		// Already allocated
		return true;
	}
	else if( inSize < ByteSize )
	{
		if( TruncateTo(inSize) != inSize )
		{
			return false;
		}
	}
	else
	{
		if( GrowBy(inSize - ByteSize) != inSize )
		{
			return false;
		}
	}

	return true;
}


/**********************************************************************************************/
bool Value_Raw::get_IsNull( void ) const 
{ 
	return false; 
}


/**********************************************************************************************/
void Value_Raw::put_IsNull( bool inValue ) 
{ 
	if( inValue )
	{
		m_pEnd = m_pStart;
		if( m_pEnd )
		{
			*m_pEnd = 0;
		}
	}
}


//#if FBL_TEST_CODE


/**********************************************************************************************/
void Value_Raw::Init( void )  
{
	if( vuint32 Len = get_Allocated() )
	{
		memset( m_pStart, 'a', Len );
		m_pEnd = m_pStart + Len;
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
void Value_Raw::Increment( void )  		
{ 
	if( m_pStart )
	{ 
		char c = *m_pStart < 'z' ? ++(*m_pStart) : 'a';
		memset( m_pStart, c, get_Length() );
	}
}

//#endif // FBL_TEST_CODE


/**********************************************************************************************/
void Value_Raw::InitSelf( void )  
{
	m_pStart 	 = nullptr;
	m_pBufferEnd = nullptr;
	m_pEnd 	 	 = nullptr;
}


/**********************************************************************************************/
void Value_Raw::Clear( void ) 
{
	if( m_pStart )
	{
		delete [] m_pStart;	
		InitSelf();	
	}
}						


/**********************************************************************************************/
void Value_Raw::Alloc( vuint32 inSize ) 
{
	if( inSize )
	{
		m_pStart = m_pEnd = new char[ inSize ];
		m_pBufferEnd = m_pStart + inSize;
	}
	else
	{
		m_pStart = m_pEnd = m_pBufferEnd = nullptr;
	}
}


/**********************************************************************************************/
const char*	Value_Raw::AllocCopy( const char* inStart, const char* inEnd ) 
{
	FBL_CHECK( inStart <= inEnd ); 
	vuint32 size = vuint32(inEnd - inStart);
	
	if( size )
	{	
		Alloc( size );
		memcpy( m_pStart, inStart, size );
		m_pEnd = m_pStart + size;
	}
	
	return m_pStart;
}


/**********************************************************************************************/
vuint32 Value_Raw::TruncateTo( vuint32 inNewSize )
{
	// For speed consideration this method does not perform any checking.
	vuint32 OldLen = get_Length();
	vuint32 NewLen = OldLen > inNewSize ? inNewSize : OldLen;

	char* NewBuf = nullptr;
	if( inNewSize )
	{
		NewBuf = new char[inNewSize];
		if( !NewBuf )
		{
			return OldLen;
		}
		
		// Copy and free old content
		memcpy(NewBuf, m_pStart, NewLen );
	}
	delete [] m_pStart;

	// Set pointers
	m_pStart = NewBuf;
	m_pBufferEnd = m_pStart + inNewSize;
	m_pEnd = m_pStart + NewLen;

	return NewLen;
}


/**********************************************************************************************/
vuint32 Value_Raw::GrowBy( vuint32 inBySize )
{
	// For speed consideration this method does not perform 
	// any checking of input parameters.

	vuint32 OldLen = get_Length();
	vuint32 NewSize = get_Allocated() + inBySize;

	char* NewBuf = new char[NewSize];
	if( !NewBuf )
	{
		// Return the old size.
		return NewSize - inBySize;
	}
	
	// Copy and free old content
	memcpy(NewBuf, m_pStart, OldLen);
	delete []m_pStart;

	// Set pointers
	m_pStart = NewBuf;
	m_pBufferEnd = m_pStart + NewSize;
	m_pEnd = m_pStart + OldLen;

	if( gEraseInformation )
	{
		// Fill additional memory with 0.
		vuint32 Rest = vuint32(m_pBufferEnd - m_pEnd);
		if( Rest )
		{
			memset(m_pEnd, 0, Rest);
		}
	}

	return NewSize;
}


/**********************************************************************************************/
void Value_Raw::put_String( const UChar* inBegin, const UChar* inEnd )
{
	// There is no sence to work with Binary data as with zero terminated and probably
	// multibyte string.
	// 
	
	put_String( (const char*)inBegin, (const char*)inEnd );
}


/**********************************************************************************************/
void Value_Raw::put_String( const char* inBegin, const char* inEnd )
{
	if ( !inEnd )
		inEnd = inBegin + strlen( inBegin );

	vuint32 Len = vuint32(inEnd - inBegin);
	put_Data( (vuint8*)inBegin, Len );
}


/**********************************************************************************************/
char* Value_Raw::get_String( char* outString, tslen inBufferChars ) const
{
	// The size of input buffer (inBufferChars) cannot be -1 !
	// Because this is unsafe then to write to  the memory 
	// location pointed by outString - because we don't know the size !
	if( outString == nullptr || inBufferChars <= 0 )
		return nullptr;

	tslen length = (tslen) get_Length();
	if( length < inBufferChars )
		inBufferChars = length;
		
	memcpy( outString, m_pStart, (vuint32) inBufferChars );

	return outString + inBufferChars;
}


/**********************************************************************************************/
String Value_Raw::get_String( tslen inLimit ) const
{
	// There is no sense to work with Binary data as with zero terminated and probably
	// multi byte string.
	// 
	tslen Limit = inLimit;
	if( Limit == -1 )
	{
		Limit = static_cast<tslen>(get_Length());
	}

	return String(m_pStart, Limit);
}


/**********************************************************************************************/
void Value_Raw::put_String( const String& inStr )
{
	if( inStr.isSingleByte() )
		put_String( inStr.getBufferA() );
	else
		put_String( inStr.c_str() );
}


/**********************************************************************************************/
UChar* Value_Raw::get_String( UChar* outString, tslen inBufferChars ) const
{
	// There is no sence to work with Binary data as with zero terminated and probably
	// multibyte string.
	// 
	return (UChar*)get_String( (char*)outString, inBufferChars );
}


/**********************************************************************************************/
vuint32 Value_Raw::put_Length( vuint32 inNewLen )
{
	vuint32 Size = vuint32(m_pBufferEnd - m_pStart);
	
	vuint32 NewLen = inNewLen;
	if( NewLen > Size )
	{
		NewLen = Size;
	}

	m_pEnd = m_pStart + NewLen;
	return vuint32(m_pEnd - m_pStart);
}


/**********************************************************************************************/
void Value_Raw::From( I_IStream_Ptr inStream, bool inBlock )
{
	argused1(inBlock);

	Clear();

	bool IsNull;
	inStream->get(IsNull);
	if( !IsNull )
	{
		put_IsNull(false);

		vuint32 Len;
		inStream->get(Len);
		Alloc(Len);

		if( Len > 0 )
		{
			vuint32 Available = get_Allocated();
			Available = Len > Available ? Available : Len;

			inStream->get(reinterpret_cast<void*>(m_pStart), Available);
			m_pEnd = m_pStart + Len;
		}
	}
	else
	{
		put_IsNull(true);
	}
} 


/**********************************************************************************************/
void Value_Raw::From( I_PacketRcv* inPacket, bool inBlock )
{
	argused1(inBlock);

	Clear();

	bool IsNull = inPacket->get_BoolParam();
	if( !IsNull )
	{
		put_IsNull(false);

		vuint32 Len = inPacket->get_ULongParam();
		Alloc(Len);

		if( Len > 0 )
		{
			vuint32 Available = get_Allocated();
			Available = Len > Available ? Available : Len;

			inPacket->get_BinaryParam(reinterpret_cast<void*>(m_pStart), Available);
			m_pEnd = m_pStart + Len;
		}
	}
	else
	{
		put_IsNull(true);
	}
} 


/**********************************************************************************************/
void Value_Raw::To( I_OStream_Ptr inStream, bool inBlock ) const
{
	argused1(inBlock);

	bool IsNull = bool( m_pStart == nullptr );
	inStream->put(IsNull);
	
	if( !IsNull )
	{
		vuint32 Len = vuint32(m_pEnd - m_pStart);
		inStream->put(Len);
		if( Len )
		{
			inStream->put(reinterpret_cast<void*>(m_pStart), Len);
		}
	}
}


/**********************************************************************************************/
void Value_Raw::To( I_PacketSnd* inPacket, bool inBlock ) const
{
	argused1(inBlock);

	FixParamCount fixParamCount( inPacket );
	
	bool IsNull = bool( m_pStart == nullptr );
	inPacket->put_BoolParam(IsNull);
	
	if( !IsNull )
	{
		vuint32 Len = vuint32(m_pEnd - m_pStart);
		inPacket->put_ULongParam(Len);
		if( Len )
		{
			inPacket->put_BinaryParam(reinterpret_cast<void*>(m_pStart), Len);
		}
	}
}


/**********************************************************************************************/
vuint32 Value_Raw::get_Data( vuint8* outDataBuffer, vuint32 inBufferSize ) const 
{
	FBL_CHECK(outDataBuffer);
	if( !outDataBuffer || inBufferSize == 0 )
	{
		return 0;
	}

	vuint32 MyLen = get_Length();
	if( MyLen == 0 )
	{
		return 0;
	}

	vuint32 ToCopy = inBufferSize > MyLen ? MyLen : inBufferSize;
	memcpy(outDataBuffer, m_pStart, ToCopy);

	// Return the count of copied bytes.
	return ToCopy;
}


/**********************************************************************************************/
void Value_Raw::put_Data( vuint8* inDataBuffer, vuint32 inBufferSize ) 
{
	vuint8* pBegin = inDataBuffer;
	vuint8* pEnd = inDataBuffer + inBufferSize;

	if( pBegin == nullptr || inBufferSize == 0 )
	{
		put_IsNull( true );
	}
	else
	{
		put_IsNull( false );
		vuint32 OldLen = get_Length();
		vuint32 Available = get_Allocated();
		vuint32 Assigned = vuint32(pEnd - pBegin);
		Assigned = Assigned > Available ? Available : Assigned;

		if( Assigned > 0)
		{
			memcpy( m_pStart, pBegin, Assigned );
			m_pEnd = m_pStart + Assigned;
		}

		if( gEraseInformation )
		{
			// Fill the peace of space that has been freed after old value with 0.
			vuint32 NewLen = get_Length();
			vint32 Diff = static_cast<vint32>(OldLen - NewLen);
			if( Diff > 0 )
			{
				memset(m_pEnd, 0, static_cast<vuint32>(Diff));
			}
		}		
	}
}


/**********************************************************************************************/
FBL_End_Namespace

