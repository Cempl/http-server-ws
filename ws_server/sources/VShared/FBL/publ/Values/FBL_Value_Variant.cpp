/**********************************************************************************************/
/* FBL_Value_Variant.cpp                                                    				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Algorithms/FBL_Algs_VarInt_Positive.h>

#include <VShared/FBL/publ/Factories/FBL_Value_Factory.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_Database.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_IStream.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_OStream.h>

#include <VShared/FBL/publ/Util_classes/FBL_StringConversion.h>

#include <VShared/FBL/publ/Values/FBL_Value_Variant.h>
#include <VShared/FBL/publ/Values/FBL_Value_String.h>


/**********************************************************************************************/
FBL_Begin_Namespace


#pragma mark -
#pragma mark ====== class Value_Variant_imp =======


/**********************************************************************************************/
Value_Variant_imp::Value_Variant_imp( I_Database_Ptr inpDb )
:
	mpDb(inpDb.get()),
	mIsSingleByte(false)
{
}


/**********************************************************************************************/
Value_Variant_imp::Value_Variant_imp( const Value_Variant_imp& inOther )
:
	Value_Raw_imp( inOther )
{
	mpDb = inOther.mpDb;
	mIsSingleByte = inOther.mIsSingleByte;
}


/**********************************************************************************************/
Value_Variant_imp::~Value_Variant_imp( void )
{
}


#pragma mark -
#pragma mark ===== interface I_Value ======


/**********************************************************************************************/
vuint32 Value_Variant_imp::get_BinaryRepresentationByteLength( void ) const
{
	return 0;
}


/**********************************************************************************************/
void Value_Variant_imp::FromBinaryRepresentation( const char* inpBuffer )
{
	argused1(inpBuffer);
	FBL_Throw( xFeatureError(
					ERR_FEATURE_NOT_SUPPORTED,
					"FromBinaryRepresentation() for Variant-type value" ) );
}


/**********************************************************************************************/
void Value_Variant_imp::ToBinaryRepresentation( char* outpBuffer ) const
{
	argused1(outpBuffer);
	FBL_Throw( xFeatureError(
					ERR_FEATURE_NOT_SUPPORTED,
					"ToBinaryRepresentation() for Variant-type value" ) );
}


#pragma mark -
#pragma mark ===== interface I_ValueVariant ======


/**********************************************************************************************/
vuint32 Value_Variant_imp::get_VariantValueType( void ) const
{
	vuint32 res = 0;

	if( !mIsNull )
    	GetVarInt32( (const unsigned char*) m_pStart, &res );
	
	return res;
}


/**********************************************************************************************/
I_Value_Ptr Value_Variant_imp::get_VariantValue( void ) const
{
	I_Value_Ptr pRes;

	if( mIsNull == false )
	{
		vuint32	variantType;
		int sizeOfType = GetVarInt32( (const unsigned char*) m_pStart, &variantType );
		
		if( variantType )
		{
			// Hack - datetime values expect I_Database* as param - not length.
			// So we pass NULL (value with own DtFormat).
			switch( variantType )
			{
				case kTypeDate:
				case kTypeTime:
				case kTypeDateTime:
				{
					pRes = FBL::internal::CreateValue(
							variantType,
							fNone,
							(void*)(mpDb),
							NULL,
							false );
				} break;
				
				case kTypeString:
				case kTypeVarChar:
				case kTypeText:
				{
					vuint32 length = 0;
					Value_string_Ptr pStringRes = new Value_string( length, nullptr );
					pStringRes->put_IsSingleByte( get_IsSingleByte() );
					
					pRes = pStringRes;
				} break;
				
				case kTypeEnum:
				case kTypeEnum8:
				case kTypeEnum16:
				{
					FBL_Throw( xFeatureError( ERR_FEATURE_NOT_SUPPORTED, "Enum-type value as Variant" ) );
				} break;
					
				case kTypeObjectPtr:
				case kTypeObjectsPtr:
				{
					FBL_Throw( xFeatureError( ERR_FEATURE_NOT_SUPPORTED, "ObjectPtr-type value as Variant" ) );
				} break;
				
				case kTypeVariant:
				{
					FBL_Throw( xFeatureError( ERR_FEATURE_NOT_SUPPORTED, "Variant-type value as Variant" ) );
				} break;
				
				case kTypeCompound:
				{
					FBL_Throw( xFeatureError( ERR_FEATURE_NOT_SUPPORTED, "Compound-type value as Variant" ) );
				} break;
				
				case kTypeUserDefined:
				{
					FBL_Throw( xFeatureError( ERR_FEATURE_NOT_SUPPORTED, "UserDefined-type value as Variant" ) );
				} break;

				case kTypePicture:
				{
					// Special case, because FBL::internal::CreateValue() creates special
					// Value_picture_null which is actually typedef to Value_BLOB which, in turn,
					// typedef to Value_ulong (we need it in fields - BLOB's field value is
					// a "first segment number"...).
					pRes = new Value_picture();
				} break;
				
				default:
				{
					// The value self will allocate enough memory to read own data
					// ( pascal-string stored as serialized data)
					vuint32 length = 0;
					pRes = FBL::internal::CreateValue(
							variantType,
							fNone,
							&length,
							NULL,
							false );
				}
			}
			
			if( pRes )
			{
				pRes->put_IsRemote( get_IsRemote() );
				pRes->FromBinaryRepresentation( m_pStart + sizeOfType );
			}
		}
	}
	
	return pRes;
}


/**********************************************************************************************/
void Value_Variant_imp::put_VariantValue( Const_I_Value_Ptr inpValue )
{
	if( inpValue == nullptr || inpValue->get_IsNull() )
	{
		put_IsNull( true );
	}
	else
	{
		// CALC SIZES:
		vuint32	variantType = inpValue->get_Type();
		
		// The whole binary data is: { TYPE, VALUE_DATA }
		// 1) TYPE "header" (variant_type stored as VarInt)
		//	 this is a min info to create the I_Value
		//   (maxlength is not important, since particular value like StringValue serializes itself as a "pascal-string");
		// 2) serialized value data
        //
        int SizeOfType = VarIntLen64( variantType );
		vuint32 variantSize = SizeOfType + inpValue->get_BinaryRepresentationByteLength();
		
		// ALLOCATE:
		put_Allocated( variantSize );
		put_IsNull( false );
		
		
		// TYPE-Header:
		(void) PutVarInt32( (unsigned char*) m_pStart, variantType );
		
		// DATA:
		inpValue->ToBinaryRepresentation( m_pStart + SizeOfType );
        
        // SETUP other value pointers:
		m_pEnd = m_pStart + variantSize;
	}
}


/**********************************************************************************************/
// OVERRIDEN because we need to allocate enough space in variant_value for data.
// Binary_value is always space-limited (so it would be data-cutting). Variant_value is not
// space_limited because it could be almost any data packed inside.
//
void Value_Variant_imp::put_Data(
	vuint8* 	inDataBuffer,
	vuint32 	inBufferSize )
{
	put_Allocated( inBufferSize );
    
	Value_Raw_imp::put_Data( inDataBuffer, inBufferSize );
}


#pragma mark -
#pragma mark ===== interface I_Serializable ======


/**********************************************************************************************/
void Value_Variant_imp::From( I_PacketRcv* 	inPacket, bool inBlock )
{
	//
	// This variant value may hold ValueString.
	// We have to send/receive such stringValue data converted to/from client encoding.
	//

	// We have to convert binary data if
	// - not-null value;
	// - AND we are on server side (client never do converting self);
	// - AND current client uses single-byte encoding
	// - AND there is StringValue data in this Variant;
	//
	
	argused1(inBlock);

	Clear();

	bool IsNull = inPacket->get_BoolParam();
	if( IsNull == false )
	{
		put_IsNull(false);
		
        
        // EXTRACT the TYPE of variant value from packet:
		vuint32 actualDataType = From_DataType( inPacket );
		
        
        // EXTRACT DATA from packet.
		if( mIsRemote ) // Client-side (receive data "as is"):
		{
			From_AsPureData( inPacket, actualDataType );
		}
		else // Server-side (receive data "as is" for not-string data or "UTF16 client"):
		{
			I_Converter* pConverter = inPacket->get_Converter();
			if(	pConverter && (		actualDataType == kTypeString
								||	actualDataType == kTypeVarChar
								||	actualDataType == kTypeText ) )
			{
				From_OnServerSide_AsSingleByteStringData( inPacket, actualDataType );
			}
			else // Not a string value or "UTF16 client":
			{
				From_AsPureData( inPacket, actualDataType );
			}
		}
	}
	else
	{
		put_IsNull(true);
	}
}


/**********************************************************************************************/
// This variant value may hold ValueString.
// We have to send/receive such stringValue data converted to/from client encoding.
//
// We have to convert binary data if
// - not-null value;
// - AND we are on the server side (client never do converting self);
// - AND the current client uses single-byte encoding
// - AND there is StringValue data in this Variant;
//
void Value_Variant_imp::To( I_PacketSnd*  inPacket, bool inBlock ) const
{
	argused1(inBlock);

	FixParamCount fixParamCount( inPacket );
	
	bool IsNull = bool( m_pStart == nullptr );
	inPacket->put_BoolParam( IsNull );
	
	if( IsNull == false )
	{
		vuint32 actualDataType;
        int sizeOfType = GetVarInt32( (const unsigned char*) m_pStart, &actualDataType );
		
        // Send the TYPE of variant value to packet:
		To_DataType( inPacket, actualDataType );
		
		if( mIsRemote ) // Client side (send data "as is"):
		{
			To_AsPureData( inPacket, sizeOfType );
		}
		else // Server side (send data "as is" for not-string data or "UTF16 client"):
		{
			I_Converter* pConverter = inPacket->get_Converter();
			if(	pConverter && (		actualDataType == kTypeString
								||	actualDataType == kTypeVarChar
								||	actualDataType == kTypeText ) )
			{
				To_OnServerSide_AsSingleByteStringData( inPacket, sizeOfType );
			}
			else // Not a string value or "UTF16 client":
			{
				To_AsPureData( inPacket, sizeOfType );
			}
		}
	}
}


#pragma mark -
#pragma mark ===== THIS CLASS ======


/**********************************************************************************************/
// Binary representation for String data is: "dataType + dataByteLength + pureData":
//
// This method is similar to Value_Variant_imp::put_VariantValue() by code.
//
void Value_Variant_imp::put_StringData(
	vuint32		inActualDataType,
	const void*	inDataBuffer,
	vuint32 	inBufferSize )
{
	// CALC SIZES:
    int SizeOfType = VarIntLen64( inActualDataType );
	vuint32 variantSize = SizeOfType + sizeof(inBufferSize) + inBufferSize;
	FBL_CHECK( variantSize > 0 );

	// ALLOCATE:
	Alloc( variantSize );

	// TYPE-Header:
	(void) PutVarInt32( (unsigned char*) m_pStart, inActualDataType );		// 1 - 4 bytes
	
	// STRING-LENGTH:
	memcpy( m_pStart + SizeOfType, &inBufferSize, sizeof(inBufferSize) );	// 4 bytes

	// DATA:
	memcpy( m_pStart + SizeOfType + sizeof(inBufferSize), inDataBuffer, inBufferSize );

    // SETUP other value pointers:
	m_pEnd = m_pStart + variantSize;
}


#pragma mark -
#pragma mark ===== FROM Methods ======


/**********************************************************************************************/
// EXTRACTS from inPacket the TYPE and returns it.
//
vuint32 Value_Variant_imp::From_DataType( I_PacketRcv* inPacket )
{
	vuint32 actualDataType = inPacket->get_ULongParam();
	return actualDataType;
}


/**********************************************************************************************/
// This method is called by From() { type = From_DataType(), From_AsPureData( type ); }
//
// It extracts from the packet DATA of variant value directly into prepared buffer of this value.
//	This method is similar to Value_Variant_imp::put_VariantValue() by code.
//
void Value_Variant_imp::From_AsPureData( I_PacketRcv* inPacket, vuint32 inActualDataType )
{
	//
	// Binary representation for any data is: "dataType + pureData":
	//
    int SizeOfType = VarIntLen64( inActualDataType );
	vuint32 rawDataLen = inPacket->get_ULongParam();
    
	vuint32 variantSize = SizeOfType + rawDataLen;
	FBL_CHECK( variantSize > 0 );
    
	Alloc( variantSize );

	// TYPE-Header:
	(void) PutVarInt32( (unsigned char*) m_pStart, inActualDataType );

	// Put raw data:
	inPacket->get_BinaryParam( reinterpret_cast<void*>(m_pStart + SizeOfType), rawDataLen );


	// SETUP other value pointers:
	m_pEnd = m_pStart + variantSize;
}


/**********************************************************************************************/
// Prepare and receive single-byte String-value
// (from client to server side):
//
// ATTENTION, hack!
// Here we use internal knowledge of string_value BinaryRepresentation (pascal string
// - "byte_length + raw_data). But we don't want to create value_string just to call this
// algorithm.
//
void Value_Variant_imp::From_OnServerSide_AsSingleByteStringData(
	I_PacketRcv* inPacket, vuint32 inActualDataType )
{
	FBL_CHECK( inPacket->get_Converter() );
	FBL_CHECK( mIsRemote == false );
	
	vuint32 rawDataLen = inPacket->get_ULongParam();
	MemPtr<char> buff( rawDataLen );
    
	inPacket->get_BinaryParam( reinterpret_cast<void*>(buff.begin()), rawDataLen );
	
	// buff contains "dataByteLength + pureData" for string data.
	// We have to convert pureData only:
	StToUTF16 convertedValue(
			buff.begin() + sizeof(vuint32),
			rawDataLen - sizeof(vuint32),
			inPacket->get_Converter() );
	
	put_StringData(
		inActualDataType,
		convertedValue.c_str(),
		convertedValue.length() * sizeof(UChar) );
}


#pragma mark -
#pragma mark ===== TO Methods ======


/**********************************************************************************************/
// EXTRACTS from inPacket the TYPE and returns it.
//
void Value_Variant_imp::To_DataType( I_PacketSnd* inPacket, vuint32 inValue ) const
{
	inPacket->put_ULongParam( inValue );
}


/**********************************************************************************************/
// Binary representation for any data is: "dataType + pureData":
//
void Value_Variant_imp::To_AsPureData(
	I_PacketSnd*  	inPacket,
    int				inSizeOfType ) const
{
	char* pPureDataStart = m_pStart + inSizeOfType;
	
	vuint32 pureDataLen = vuint32( m_pEnd - pPureDataStart );
	FBL_CHECK( pureDataLen );

	inPacket->put_ULongParam( pureDataLen );
	inPacket->put_BinaryParam( reinterpret_cast<void*>(pPureDataStart), pureDataLen );
}


/**********************************************************************************************/
// Prepare and send single-byte String-value
// (from server to client side):
//
// ATTENTION, hack!
// Here we use internal knowledge of string_value BinaryRepresentation (pascal string
// - "byte_length + raw_data). But we don't want to create value_string just to call this
// algorithm.
//
void Value_Variant_imp::To_OnServerSide_AsSingleByteStringData(
	I_PacketSnd*  	inPacket,
    int				inSizeOfType ) const
{
	FBL_CHECK( inPacket->get_Converter() );
	FBL_CHECK( mIsRemote == false );

	// UTF16 PTR
	UChar* pPureDataStart = (UChar*)(
					m_pStart			// Start of binary data
				+	inSizeOfType		// actual data type (string_value in this case)
				+	sizeof(vuint32)		// string_value's byteLength
				);

	StFromUTF16 convertedValue(
			pPureDataStart,
			(UChar*)m_pEnd,
			inPacket->get_Converter() );

	vuint32 convertedDataLength = convertedValue.length();

	// PREPARE UTF8 buffer:
	MemPtr<char> buff( sizeof(convertedDataLength) + convertedDataLength );
	{
        char* pBinaryValueBufferPtr = buff.begin();
        
        // Write dataLength:
        memcpy( pBinaryValueBufferPtr, &convertedDataLength, sizeof(convertedDataLength) );
        pBinaryValueBufferPtr += sizeof(convertedDataLength);
        
        // Write data:
        memcpy( pBinaryValueBufferPtr, convertedValue.c_str(), convertedDataLength );
	}
	
    
    // COPY UTF8 STR to PACKET:
	inPacket->put_ULongParam( buff.get_Size() );
	inPacket->put_BinaryParam(reinterpret_cast<const void*>(buff.begin()), buff.get_Size());
}


#pragma mark -
#pragma mark ====== class Value_Variant =======


/**********************************************************************************************/
Value_Variant::Value_Variant( I_Database_Ptr inpDb )
:
	Value_Variant_imp( inpDb )
{
}


/**********************************************************************************************/
Value_Variant::Value_Variant( const Value_Variant& inOther )
:
	Value_Variant_imp( inOther )
{
}


/**********************************************************************************************/
Value_Variant::~Value_Variant( void )
{
}


/**********************************************************************************************/
I_Value_Ptr Value_Variant::Clone( bool inCopyData ) const
{
	I_Value_Ptr result;

	if( inCopyData == true )
		result = new Value_Variant( *this );
	else
	{
		result = new Value_Variant( mpDb );
	}

	result->put_IsRemote( mIsRemote );

	return result;
}


#pragma mark -
#pragma mark ====== class Value_Variant_null =======


/**********************************************************************************************/
Value_Variant_null::Value_Variant_null( I_Database_Ptr inpDb )
:
	Value_Variant( inpDb )
{
	mIsNull = true;
}


/**********************************************************************************************/
Value_Variant_null::Value_Variant_null( const Value_Variant_null& inOther )
:
	Value_Variant( inOther )
{
	put_IsNull( inOther.get_IsNull() );
}


/**********************************************************************************************/
Value_Variant_null::~Value_Variant_null( void )
{
}


/**********************************************************************************************/
I_Value_Ptr Value_Variant_null::Clone( bool inCopyData ) const
{
	I_Value_Ptr result;

	if( inCopyData == true )
		result = new Value_Variant_null( *this );
	else
	{
		result = new Value_Variant_null( mpDb );
	}

	result->put_IsRemote( mIsRemote );

	return result;
}


/**********************************************************************************************/
FBL_End_Namespace

