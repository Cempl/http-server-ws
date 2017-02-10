/**********************************************************************************************/
/* FBL_Value_Numeric.h 	                                                   					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_Value_Numeric_h 
	#define _FBL_Value_Numeric_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Interfaces/FBL_I_Value.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Serializable.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_IStream.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_OStream.h>

#include <VShared/FBL/publ/Util_classes/FBL_StringConversion.h>
#include <VShared/FBL/publ/Values/FBL_Type2String.h>

#include <VShared/FBL/publ/Values/FBL_Convert.h>
#include <VShared/FBL/publ/Values/FBL_type_traits.h>
#include <VShared/FBL/publ/Values/FBL_Value_StoragePolicy.h>


// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// Implements Value based on any fundamental C++ numeric type.
//
// Value is stored directly in the object of this class, so it is self-contained and it is
// comfortable to transfer a value between objects.
// 
// class <traits> must be one of type_traits defined in FBL_type_traits.h
// kFldTypeIndex is costant define in FBL_Constants.h for each database field type. 
// 
template 
<
	vuint32 		kFldTypeIndex, 
	VALUE_CATEGORY 	VC,
	class 			policyStorage
>
class FBL_SHARED_EXP_CLASS Value_Numeric : 
	public I_Value, 
	public I_Serializable,
	public policyStorage
{
		IMPLEMENT_UNKNOWN(Value_Numeric)											
		BEGIN_INTERFACE_TABLE(Value_Numeric)										
			IMPLEMENT_INTERFACE(I_Value)										
			IMPLEMENT_INTERFACE(I_Serializable)										
		END_INTERFACE_TABLE()


	public://///////////////////////////////////////////////////////////////////////////////////

		typedef policyStorage 								PS;
		typedef typename PS::ps_traits						ps_traits;
		typedef typename ps_traits::value_type				value_type;

		typedef       Value_Numeric<kFldTypeIndex,VC,PS>	this_type;
		typedef       smart_ptr<this_type>					pointer;
		typedef const pointer								const_pointer;
		typedef       this_type&							reference;
		typedef const this_type&							const_reference;
	

	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// Construction:
	
							Value_Numeric( void )  :
								mIsRemote( false )
							{
							}
							
							Value_Numeric( const_reference inOther )  :
								PS( value_type(inOther) ),
								mIsRemote( false )
							{
							}


	public://///////////////////////////////////////////////////////////////////////////////////

// I_Value API:

virtual	I_Value_Ptr			Clone( bool inCopyData = true ) const override
							{
								I_Value_Ptr result;

								if( inCopyData == true )
									result = new this_type( *this );
								else
								{
									result = new this_type();
									result->put_IsNull( true );
								}

								result->put_IsRemote( mIsRemote );

								return result;
							}

	// ---------------------
	// Properties:

virtual vuint32 			get_Allocated( void ) const override		{ return sizeof(value_type); }
virtual bool				put_Allocated( vuint32 inSize ) override	{ argused1(inSize); return false; }

virtual vuint32 			get_ByteLength( void ) const override		{ return sizeof(value_type); }
virtual void 				put_ByteLength( vuint32 inNewLength ) override { argused1(inNewLength); }

virtual	vuint32				get_ByteLengthForIndex( void ) const override
							{
								vuint32 len = get_ByteLength();
								
								if( len > 255 )
									len = 255;

								return len;
							}

virtual VALUE_CATEGORY		get_Category( void ) const override 	{ return VC; }

virtual bool				get_IsNullable( void ) const override 	{ return false; }
virtual bool 				get_IsNull( void ) const override 		{ return false; }
virtual void 				put_IsNull( bool inValue ) override
							{ 
								if(inValue)
								{
									value_type* pv = PS::get_ValuePtr();
									 *pv = value_type(0); 
								}
							}
							
virtual bool				get_IsRemote( void ) const override 	{ return mIsRemote; }
virtual void 				put_IsRemote( bool inValue ) override 	{ mIsRemote = inValue; }

virtual vuint32 			get_Length( void ) const override 		{ return sizeof(value_type); }

virtual vuint32				get_MaxChars( void ) const override
								{ return ps_traits::max_text_length(); }

virtual vuint32				get_StorageSize( void ) const override 	{ return get_Allocated(); }
virtual VALUE_TYPE			get_Type( void ) const override 		{ return kFldTypeIndex; }
	
virtual String				get_TypeString( const char inLocale[] = "en_US" ) const override
								{ return TypeCode2String(kFldTypeIndex, inLocale);}

virtual COMPARE_TYPE		get_CompareType( void ) const override
							{
								return kBinaryCompare;
							}
	
virtual void				put_CompareType( COMPARE_TYPE inValue ) override
							{
								argused1( inValue );
							}

	public://///////////////////////////////////////////////////////////////////////////////////
	
	// ---------------------
	// Value access
	
virtual const char*			begin( void ) const	override { return (const char*)PS::get_ValuePtr(); }
virtual const char*			end( void )   const	override { return begin() + get_Allocated(); }

virtual	void				Assign( const I_Value& inValue ) override
							{
								try
								{
									if( inValue.get_IsNull() )
										put_IsNull( true );
									else
									{
										put_IsNull( false );
										
										ConvertValue( &inValue, this );
									}
								}
								catch(...)
								{
									// RZ TODO: I think we need LOG this error.
								}
							}

virtual vuint32				CopyToIndex( void* outBuffer, bool inSwapBytes ) const override
							{ 
								*(value_type*) outBuffer = *PS::get_ValuePtr(); 
								
								if( inSwapBytes )
									SwapBytes(outBuffer);
                                
                                return sizeof(value_type);
							}

virtual void 				CopyFromIndex( const void* inBuffer, bool inSwapBytes ) override
							{ 
								*PS::get_ValuePtr() =  *(value_type*) inBuffer; 
								put_IsNull( false ); // index contains only non-NULL values.
								
								if( inSwapBytes )
									SwapBytes();
							}


	// ---------------------
	// Value compare methods:

virtual	int					Compare( 
								const I_Value& 	inOther,
								COMPARE_TYPE 	inCompareType = kNatureCompare ) const override
							{
								argused1( inCompareType );

								// RZ 8/30/06: we must be able compare ObjectPtr, ULong and RecID values.
								// They all have different type but the same size. 
								FBL_CHECK( get_Type() == inOther.get_Type() ||
											(get_Category() == inOther.get_Category() && 
											 get_ByteLength() == inOther.get_ByteLength()) ); 

								int res;

								if( inOther.get_IsNull() == true )
									res = 1;
								else
								{
									res = ps_traits::compare( 
														*PS::get_ValuePtr(), 
														*(value_type*) inOther.begin() );
								}
								return res;
							}

virtual	int					CompareIndexValues( 
								const void* inLeft,
								const void* inRight,
								bool		inSwapBytes) const override
							{
								if( inSwapBytes )
								{
									value_type	left  = *(value_type*)inLeft;
									value_type	right = *(value_type*)inRight;
									SwapBytes(&left);
									SwapBytes(&right);
									return ps_traits::compare( left, right );
								}
								else
								{
									return ps_traits::compare( 
												*(value_type*) inLeft, 
												*(value_type*) inRight );
								}
							}

virtual	int					CompareToIndexValue( 
								Const_I_Value_Ptr 	inTestValue,
								const void* 		inIndexValue,
								vuint32				inParam,
								bool				inSwapBytes ) const override
							{
								argused1(inParam);

								if( inSwapBytes )
								{
									value_type	indexVal = *(value_type*)inIndexValue;
									SwapBytes(&indexVal);
									return ps_traits::compare( *(value_type*)inTestValue->begin(), indexVal );
								}
								else
								{
									return ps_traits::compare( 
													*(value_type*) inTestValue->begin(), 
													*(value_type*) inIndexValue );
								}
							}


	public://///////////////////////////////////////////////////////////////////////////////////
	
	// ---------------------
	// to/from string:

virtual String				get_String( tslen inLimit = -1 ) const override
							{
								String s;
								if( inLimit != 0 )
								{
									tslen MaxLen = static_cast<tslen>(ps_traits::max_text_length());
									UChar* pBuffer = s.getBuffer(MaxLen + 1);
									UChar* pEnd = ps_traits::to_str( PS::get_ValuePtr(), pBuffer, MaxLen );
									s.releaseBuffer(static_cast<tslen>(pEnd - pBuffer));

									// not -1 and less than maxLen
									if( inLimit > 0 && inLimit < MaxLen )
									{
										s.truncate( inLimit );
									}
								}
								return s;
							}

virtual void				put_String( const String& inStr ) override
							{
								if( inStr.isSingleByte() )
									put_String( inStr.getBufferA(), nullptr );
								else
									put_String( inStr.c_str(), inStr.end() );
							}

virtual UChar*				get_String( UChar* outString, tslen inBufferChars ) const override
							{
								// The size of input buffer (inBufferChars) cannot be -1 !
								// Because this is unsafe then to write to  the memory 
								// location pointed by outString - because we don't know the size !
								if( outString == nullptr || inBufferChars <= 0 )
									return nullptr;

								UChar* pEnd = nullptr;

								tslen MaxLen = static_cast<tslen>(ps_traits::max_text_length());
								if( inBufferChars < MaxLen )
								{
									String res( get_String( inBufferChars ) );
									tslen len = res.length();
									memcpy( outString, res.begin(), (size_t)len * sizeof(UChar) );
									*(outString + len) = 0;
									pEnd = outString + len;
								}
								else
								{
									pEnd = ps_traits::to_str( PS::get_ValuePtr(), outString, inBufferChars );
								}

								return pEnd;
							}

virtual void				put_String( const UChar* inBegin, const UChar* inEnd ) override
							{
								argused1( inEnd );
								ps_traits::from_str( inBegin, PS::get_ValuePtr() );
							}

virtual void				put_String( const char* inBegin, const char* inEnd = nullptr ) override
							{
								argused1( inEnd );
								ps_traits::from_str( inBegin, PS::get_ValuePtr() );
							}

							/** Get a string representation of value (ASCII).
								@param outString - Buffer for string representation of value.
								@param inBufferChars - size of buffer in characters. */
virtual char*				get_String( 
								char* 	outString, 
								tslen 	inBufferChars ) const override
							{
								// The size of input buffer (inBufferChars) cannot be -1 !
								// Because this is unsafe then to write to  the memory 
								// location pointed by outString - because we don't know the size !
								if( outString == nullptr || inBufferChars <= 0 )
									return nullptr;

								char* pEnd = nullptr;

								tslen MaxLen = static_cast<tslen>(ps_traits::max_text_length());
								if( inBufferChars < MaxLen )
								{
									String res( get_String( inBufferChars ) );
									size_t len = (size_t)res.length();
									memcpy( outString, res.getBufferA(), len );
									*(outString + len) = 0;
									pEnd = outString + len;
								}
								else
								{
									pEnd = ps_traits::to_str( PS::get_ValuePtr(), outString, inBufferChars );
								}

								return pEnd;
							}

	// ---------------------
	// Byte swapping:

virtual void				SwapBytes( void ) override
								{ PS::SwapBytes(); }

virtual void				SwapBytes( void* inValue ) const override
								{ PS::SwapBytes(inValue); }
							
virtual void				SwapBytes( const void* inSrc, void* inDest ) override
								{ PS::SwapBytes(inSrc, inDest); }

	// ---------------------
	// Numeric properties:

virtual void				put_Boolean( bool inValue ) override
								{ ( *PS::get_ValuePtr() ) = ( value_type )inValue; }

virtual bool				get_Boolean( void ) const override
								{ return ( bool )( *PS::get_ValuePtr() ); }

virtual void				put_Byte( vuint8 inValue ) override
								{ ( *PS::get_ValuePtr() ) = ( value_type )inValue; }

virtual vuint8				get_Byte( void ) const override
								{ return ( vuint8 )( *PS::get_ValuePtr() ); }

virtual void				put_Short( vint16 inValue ) override
								{ ( *PS::get_ValuePtr() ) = ( value_type )inValue; }

virtual vint16				get_Short( void ) const override
								{ return ( vint16 )( *PS::get_ValuePtr() ); }

virtual void				put_UShort( vuint16 inValue ) override
								{ ( *PS::get_ValuePtr() ) = ( value_type )inValue; }

virtual vuint16				get_UShort( void ) const override
								{ return ( vuint16 )( *PS::get_ValuePtr() ); }

virtual void				put_Medium( vint32 inValue ) override
								{ ( *PS::get_ValuePtr() ) = ( value_type )inValue; }

virtual vint32				get_Medium( void ) const override
								{ return ( vint32 )( *PS::get_ValuePtr() ); }

virtual void				put_Long( vint32 inValue ) override
								{ ( *PS::get_ValuePtr() ) = ( value_type )inValue; }

virtual vint32				get_Long( void ) const override
								{ return ( vint32 )( *PS::get_ValuePtr() ); }

virtual void				put_ULong( vuint32 inValue ) override
								{ ( *PS::get_ValuePtr() ) = ( value_type )inValue; }

virtual vuint32				get_ULong( void ) const override
								{ return ( vuint32 )( *PS::get_ValuePtr() ); }

virtual void				put_LLong( vint64 inValue ) override
								{ ( *PS::get_ValuePtr() ) = ( value_type )inValue; }

virtual vint64				get_LLong( void ) const override
								{ return ( vint64 )( *PS::get_ValuePtr() ); }

virtual void				put_ULLong( vuint64 inValue ) override
								{ ( *PS::get_ValuePtr() ) = ( value_type )inValue; }

virtual vuint64				get_ULLong( void ) const override
								{ return ( vuint64 )( *PS::get_ValuePtr() ); }

virtual void				put_Float( float inValue ) override
								{ ( *PS::get_ValuePtr() ) = ( value_type )inValue; }

virtual float				get_Float( void ) const override
								{ return ( float )( *PS::get_ValuePtr() ); }

virtual void				put_Double( double inValue ) override
								{ ( *PS::get_ValuePtr() ) = ( value_type )inValue; }

virtual double				get_Double( void ) const override
								{ return ( double )( *PS::get_ValuePtr() ); }

	// ---------------------
	// Serialization to/from char buffer (for ValueVariant):

virtual vuint32 			get_BinaryRepresentationByteLength( void ) const override
							{
								return get_ByteLength();
							}

virtual void				FromBinaryRepresentation( const char* inpBuffer ) override
							{
								ps_traits::FromBinaryRepresentation(
											PS::get_ValuePtr(), 
											inpBuffer );
							}

virtual void				ToBinaryRepresentation( char* outpBuffer ) const override
							{
								ps_traits::ToBinaryRepresentation( 
											PS::get_ValuePtr(), 
											outpBuffer );
							}


	public://///////////////////////////////////////////////////////////////////////////////////

// interface I_Serializable:

virtual void				From( I_IStream_Ptr inStream, bool inBlock = true ) override
							{
								argused1(inBlock);
								value_type* pv = PS::get_ValuePtr();
								inStream->get(*pv);
							}

virtual void				From( I_PacketRcv* inPacket, bool inBlock = true ) override
							{
								ps_traits::From( 
											PS::get_ValuePtr(), 
											inPacket,
											inBlock );
							}


virtual void				To( I_OStream_Ptr inStream, bool inBlock = true ) const override
							{
								argused1(inBlock);
								const value_type* pv = PS::get_ValuePtr();
								inStream->put(*pv);
							}
							
virtual void				To( I_PacketSnd* inPacket, bool inBlock = true ) const override
							{
								ps_traits::To( 
											PS::get_ValuePtr(), 
											inPacket,
											inBlock );
							}

							


	public://///////////////////////////////////////////////////////////////////////////////////

		void				operator=( value_type inValue )
								{ *PS::get_ValuePtr() = inValue; }

							// Easy way to get value of expected type. Used mainly in tests.
							operator value_type( void ) const  
								{ return *PS::get_ValuePtr(); }


	protected:////////////////////////////////////////////////////////////////////////////////// 

		bool				mIsRemote; // implements <IsRemote> property.


	public://///////////////////////////////////////////////////////////////////////////////////

// Section of class with TEST - helper methods.
// for internal use.
 
#if FBL_TEST_CODE

							Value_Numeric( value_type inValue )  :
								PS( inValue ),
								mIsRemote( false )
							{
							}

#endif // FBL_TEST_CODE

virtual void				Init( void ) override
							{ 
								put_IsNull( false );
								*(value_type*) PS::get_ValuePtr() = 1; 
							}
							
virtual void 				Increment( void ) override
							{ 
								value_type* pv = (value_type*) PS::get_ValuePtr();
								ps_traits::increment( pv );
							}
};


#pragma mark -


/**********************************************************************************************/
// Implements Value based on any fundamental C++ numeric type that can be nullable.
// 
#pragma mark class Value_Numeric_Nullable

template 
<
	vuint32 		kFldTypeIndex, 
	VALUE_CATEGORY 	VC,
	class 			policyStorage
>
class FBL_SHARED_EXP_CLASS Value_Numeric_Nullable : 
	public Value_Numeric<kFldTypeIndex, VC, policyStorage>,
	protected policyNullable<policyStorage>
{
	public://///////////////////////////////////////////////////////////////////////////////////

		typedef Value_Numeric<kFldTypeIndex,VC,policyStorage>	inherited;

		typedef policyStorage 									PS;
		typedef policyNullable<policyStorage>					PN;

		typedef typename PS::ps_traits							ps_traits;		
		typedef typename ps_traits::value_type					value_type;
		
		typedef Value_Numeric_Nullable<kFldTypeIndex,VC,PS>		this_type;

		typedef       smart_ptr<this_type>						pointer;
		typedef const pointer									const_pointer;
		typedef       this_type&								reference;
		typedef const this_type&								const_reference;


	public://///////////////////////////////////////////////////////////////////////////////////

	// --- Construction ---
	
							Value_Numeric_Nullable( void )  { }
							
							Value_Numeric_Nullable( const_reference inOther ) 
							:
								Value_Numeric<kFldTypeIndex, VC, policyStorage>(),
								policyNullable<policyStorage>()
							{
								if( inOther.get_IsNull() == false ) 
								{	
									// then we copy value
									inherited::operator=( value_type(inOther) );

									this_type::put_IsNull( false );
								}
								// else we already have flag mIsNull = true.
							}


	public://///////////////////////////////////////////////////////////////////////////////////

// I_Value API:

virtual	I_Value_Ptr			Clone( bool inCopyData = true ) const override
							{
								I_Value_Ptr result;

								if( inCopyData == true )
									result = new this_type( *this );
								else
								{
									result = new this_type();
									result->put_IsNull( true );
								}

								result->put_IsRemote( inherited::mIsRemote );

								return result;
							}

	// ---------------------
	// Properties:

virtual bool				get_IsNullable( void ) const override  { return true; }

virtual bool 				get_IsNull( void ) const override
								{ return PN::get_IsNull(); }
								
virtual void 				put_IsNull( bool inValue ) override
								{ PN::put_IsNull( PS::get_ValuePtr(), inValue ); }
							
// interface I_Value:

	// ---------------------
	// Value compare methods:

virtual	int					Compare( 
								const I_Value& 	inOther,
								COMPARE_TYPE 	inCompareType = kNatureCompare ) const override
							{
								argused1( inCompareType );

								// RZ 8/30/06: we must be able compare ObjectPtr, ULong and RecID values.
								// They all have different type but the same size. 
								FBL_CHECK( inherited::get_Type()       == inOther.get_Type() ||
										 ( inherited::get_Category()   == inOther.get_Category() && 
										   inherited::get_ByteLength() == inOther.get_ByteLength()) ); 

								// If one of values (this or inOther) have null...
								if( PN::get_IsNull() )
							    {
								    if( inOther.get_IsNull() )
								        return 0; // Both are NULL
								    else 
								        return -1; // Any NULL is less then NOT NULL                               
							    }
							    else 
							    {
							    	if( inOther.get_IsNull() )
							           	return 1; // Any NOT NULL is greater then NULL
							    	else
							    	{
										// ... have not null values - compare them.
										return ps_traits::compare( 
														*PS::get_ValuePtr(), 
														*(value_type*) inOther.begin() );    	
							    	}
							    }								
							}


	public://///////////////////////////////////////////////////////////////////////////////////

virtual UChar*				get_String( UChar* outString, tslen inBufferChars ) const override
							{
								argused1( inBufferChars );
								
								// we convert to string only if NOT NULL.
								// otherwise we get empty string.
								if( PN::get_IsNull() == false )
									return  Value_Numeric<kFldTypeIndex, VC, policyStorage>::get_String( outString, inBufferChars );
								else
									return outString;
							}

virtual String				get_String( tslen inLimit = -1 ) const override
							{
								// we convert to string only if NOT NULL.
								// otherwise we get empty string.
								if( PN::get_IsNull() == false )
									return Value_Numeric<kFldTypeIndex, VC, policyStorage>::get_String( inLimit );
								else
									return String::sEmpty();
							}

virtual char*				get_String( 
								char* 	outString, 
								tslen 	inBufferChars ) const override
							{
								// we convert to string only if NOT NULL.
								// otherwise we get empty string.
								if( PN::get_IsNull() == false )
									return Value_Numeric<kFldTypeIndex, VC, policyStorage>::get_String( outString, inBufferChars );
								else
									return outString;								
							}


virtual void				put_String( const UChar* inBegin, const UChar* inEnd ) override
							{
								argused1( inEnd );
	
								if( inBegin )
									ps_traits::from_str( inBegin, PS::get_ValuePtr() );
								
								put_IsNull( false );								
							}

virtual void				put_String( const char* inBegin, const char* inEnd = nullptr ) override
							{
								argused1( inEnd );
								
								if( inBegin )
									ps_traits::from_str( inBegin, PS::get_ValuePtr() );
								
								put_IsNull( false );
							}


	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// Numeric properties:

virtual void				put_Boolean( bool inValue ) override
							{ 
								PN::put_IsNull( PS::get_ValuePtr(), false );
								( *PS::get_ValuePtr() ) = ( value_type )inValue; 
							}

virtual void				put_Byte( vuint8 inValue ) override
							{ 
								PN::put_IsNull( PS::get_ValuePtr(), false );
								( *PS::get_ValuePtr() ) = ( value_type )inValue; 
							}

virtual void				put_Short( vint16 inValue ) override
							{ 
								PN::put_IsNull( PS::get_ValuePtr(), false );
								( *PS::get_ValuePtr() ) = ( value_type )inValue; 
							}

virtual void				put_UShort( vuint16 inValue ) override
							{ 
								PN::put_IsNull( PS::get_ValuePtr(), false );
								( *PS::get_ValuePtr() ) = ( value_type )inValue; 
							}

virtual void				put_Medium( vint32 inValue ) override
							{ 
								PN::put_IsNull( PS::get_ValuePtr(), false );
								( *PS::get_ValuePtr() ) = ( value_type )inValue; 
							}

virtual void				put_Long( vint32 inValue ) override
							{ 
								PN::put_IsNull( PS::get_ValuePtr(), false );
								( *PS::get_ValuePtr() ) = ( value_type )inValue; 
							}

virtual void				put_ULong( vuint32 inValue ) override
							{ 
								PN::put_IsNull( PS::get_ValuePtr(), false );
								( *PS::get_ValuePtr() ) = ( value_type )inValue; 
							}

virtual void				put_LLong( vint64 inValue ) override
							{ 
								PN::put_IsNull( PS::get_ValuePtr(), false );
								( *PS::get_ValuePtr() ) = ( value_type )inValue; 
							}

virtual void				put_ULLong( vuint64 inValue ) override
							{ 
								PN::put_IsNull( PS::get_ValuePtr(), false );
								( *PS::get_ValuePtr() ) = ( value_type )inValue; 
							}

virtual void				put_Float( float inValue ) override
							{ 
								PN::put_IsNull( PS::get_ValuePtr(), false );
								( *PS::get_ValuePtr() ) = ( value_type )inValue; 
							}

virtual void				put_Double( double inValue ) override
							{ 
								PN::put_IsNull( PS::get_ValuePtr(), false );
								( *PS::get_ValuePtr() ) = ( value_type )inValue; 
							}

	public://///////////////////////////////////////////////////////////////////////////////////

// Section of class with TEST - helper methods.
// for internal use.
 
#if FBL_TEST_CODE

							Value_Numeric_Nullable( value_type inValue )  : 
								inherited( inValue )
							{	
								put_IsNull( false ); // because some value was asigned.		
							}
																	
#endif // FBL_TEST_CODE

}; 


#pragma mark -
#pragma mark ====== In-place Values =======


/**********************************************************************************************/
typedef Value_Numeric<kTypeBoolean	, vcUInteger	, policyInPlace_bool>		Value_bool;
typedef Value_Numeric<kTypeByte		, vcUInteger	, policyInPlace_byte>		Value_byte;

typedef Value_Numeric<kTypeShort	, vcInteger		, policyInPlace_short>		Value_short;
typedef Value_Numeric<kTypeUShort	, vcUInteger	, policyInPlace_ushort>		Value_ushort;

typedef Value_Numeric<kTypeLong		, vcInteger		, policyInPlace_long>		Value_long;
typedef Value_Numeric<kTypeULong	, vcUInteger	, policyInPlace_ulong>		Value_ulong;

typedef Value_Numeric<kTypeLLong	, vcInteger		, policyInPlace_llong>		Value_llong;
typedef Value_Numeric<kTypeULLong	, vcUInteger	, policyInPlace_ullong>		Value_ullong;

//typedef Value_Numeric<kTypeFloat	, vcReal		, policyInPlace_float>		Value_float;
//typedef Value_Numeric<kTypeDouble	, vcReal		, policyInPlace_double>		Value_double;

typedef Value_Numeric<kTypeMedium	, vcInteger		, policyInPlace_long>		Value_medium;
typedef Value_Numeric<kTypeUMedium	, vcUInteger	, policyInPlace_ulong>		Value_umedium;

typedef Value_Numeric<kTypeObjectPtr, vcUInteger	, policyInPlace_ulong>		Value_objectPtr;


/**********************************************************************************************/
typedef Value_Numeric_Nullable<kTypeBoolean	, vcUInteger	, policyInPlace_bool>	Value_bool_null;
typedef Value_Numeric_Nullable<kTypeByte	, vcUInteger	, policyInPlace_byte>	Value_byte_null;
	
typedef Value_Numeric_Nullable<kTypeShort	, vcInteger		, policyInPlace_short> 	Value_short_null;
typedef Value_Numeric_Nullable<kTypeUShort	, vcUInteger	, policyInPlace_ushort>	Value_ushort_null;

typedef Value_Numeric_Nullable<kTypeLong	, vcInteger		, policyInPlace_long>	Value_long_null;
typedef Value_Numeric_Nullable<kTypeULong	, vcUInteger	, policyInPlace_ulong> 	Value_ulong_null;

typedef Value_Numeric_Nullable<kTypeLLong	, vcInteger		, policyInPlace_llong> 	Value_llong_null;
typedef Value_Numeric_Nullable<kTypeULLong	, vcUInteger	, policyInPlace_ullong>	Value_ullong_null;

//typedef Value_Numeric_Nullable<kTypeFloat	, vcReal		, policyInPlace_float>	Value_float_null;
//typedef Value_Numeric_Nullable<kTypeDouble, vcReal		, policyInPlace_double>	Value_double_null;

typedef Value_Numeric_Nullable<kTypeMedium	, vcInteger		, policyInPlace_long>	Value_medium_null;
typedef Value_Numeric_Nullable<kTypeUMedium	, vcUInteger	, policyInPlace_ulong> 	Value_umedium_null;


// BLOB and PICTURE values ARE in fact ULONG values !!!
typedef Value_Numeric_Nullable<kTypeULong, vcUInteger, policyInPlace_ulong>	 Value_blob_null;
typedef Value_blob_null	 Value_picture_null;

typedef Value_Numeric_Nullable<kTypeObjectPtr,vcUInteger, policyInPlace_ulong>	Value_objectPtr_null;


/**********************************************************************************************/
FBL_SMART_PTR( Value_bool );
FBL_SMART_PTR( Value_byte );

FBL_SMART_PTR( Value_short );
FBL_SMART_PTR( Value_ushort );

FBL_SMART_PTR( Value_long );
FBL_SMART_PTR( Value_ulong );

//FBL_SMART_PTR( Value_float );
//FBL_SMART_PTR( Value_double );

FBL_SMART_PTR( Value_medium );
FBL_SMART_PTR( Value_umedium );

FBL_SMART_PTR( Value_objectPtr );


/**********************************************************************************************/
FBL_SMART_PTR( Value_bool_null );
FBL_SMART_PTR( Value_byte_null );

FBL_SMART_PTR( Value_short_null );
FBL_SMART_PTR( Value_ushort_null );

FBL_SMART_PTR( Value_long_null );
FBL_SMART_PTR( Value_ulong_null );

FBL_SMART_PTR( Value_llong_null );
FBL_SMART_PTR( Value_ullong_null );

//FBL_SMART_PTR( Value_float_null );
//FBL_SMART_PTR( Value_double_null );

FBL_SMART_PTR( Value_medium_null );
FBL_SMART_PTR( Value_umedium_null );


#pragma mark -
#pragma mark ====== Indirect Values =======


/**********************************************************************************************/
typedef Value_Numeric<kTypeBoolean,  vcUInteger, policyIndirect_bool>	Value_bool_ind;
typedef Value_Numeric<kTypeByte,	 vcUInteger, policyIndirect_byte>	Value_byte_ind;

typedef Value_Numeric<kTypeShort, 	 vcInteger,  policyIndirect_short>	Value_short_ind;
typedef Value_Numeric<kTypeUShort, 	 vcUInteger, policyIndirect_ushort>	Value_ushort_ind;

typedef Value_Numeric<kTypeLong, 	 vcInteger,  policyIndirect_long>	Value_long_ind;
typedef Value_Numeric<kTypeULong,	 vcUInteger, policyIndirect_ulong>	Value_ulong_ind;

typedef Value_Numeric<kTypeLLong, 	 vcInteger,  policyIndirect_llong>	Value_llong_ind;
typedef Value_Numeric<kTypeULLong, 	 vcUInteger, policyIndirect_ullong>	Value_ullong_ind;

typedef Value_Numeric<kTypeFloat, 	 vcReal, 	 policyIndirect_float>	Value_float_ind;
typedef Value_Numeric<kTypeDouble, 	 vcReal, 	 policyIndirect_double>	Value_double_ind;

typedef Value_Numeric<kTypeMedium,	 vcInteger,  policyIndirect_long>	Value_medium_ind;
typedef Value_Numeric<kTypeUMedium,	 vcUInteger, policyIndirect_ulong>	Value_umedium_ind;

typedef Value_Numeric<kTypeObjectPtr,vcUInteger, policyIndirect_ulong>	Value_objectPtr_ind;


/**********************************************************************************************/
typedef Value_Numeric_Nullable<kTypeBoolean, vcUInteger, policyIndirect_bool>	Value_bool_null_ind;
typedef Value_Numeric_Nullable<kTypeByte, 	 vcUInteger, policyIndirect_byte>	Value_byte_null_ind;
	
typedef Value_Numeric_Nullable<kTypeShort,   vcInteger,  policyIndirect_short>	Value_short_null_ind;
typedef Value_Numeric_Nullable<kTypeUShort,  vcUInteger, policyIndirect_ushort>	Value_ushort_null_ind;

typedef Value_Numeric_Nullable<kTypeLong, 	 vcInteger,  policyIndirect_long>	Value_long_null_ind;
typedef Value_Numeric_Nullable<kTypeULong, 	 vcUInteger, policyIndirect_ulong>	Value_ulong_null_ind;

typedef Value_Numeric_Nullable<kTypeLLong, 	 vcInteger,  policyIndirect_llong>	Value_llong_null_ind;
typedef Value_Numeric_Nullable<kTypeULLong,  vcUInteger, policyIndirect_ullong>	Value_ullong_null_ind;

typedef Value_Numeric_Nullable<kTypeFloat, 	 vcReal, 	 policyIndirect_float>	Value_float_null_ind;
typedef Value_Numeric_Nullable<kTypeDouble,  vcReal, 	 policyIndirect_double>	Value_double_null_ind;

typedef Value_Numeric_Nullable<kTypeMedium,	 vcInteger,  policyIndirect_long>	Value_medium_null_ind;
typedef Value_Numeric_Nullable<kTypeUMedium, vcUInteger, policyIndirect_ulong>	Value_umedium_null_ind;


#pragma mark -


/**********************************************************************************************/
FBL_SMART_PTR( Value_bool );
FBL_SMART_PTR( Value_long );
FBL_SMART_PTR( Value_llong );
 

/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_Value_Numeric_h
