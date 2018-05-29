/**********************************************************************************************/
/* FBL_Value_Variant.h 		                                               					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_Value_Variant_h
	#define _FBL_Value_Variant_h
#pragma once


// FBL:
#include <VShared/FBL/publ/Interfaces/FBL_I_Value.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_ValueVariant.h>
#include <VShared/FBL/publ/Values/FBL_Value_Binary.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// Variant value in short:
//
// -	Internally it is kind of binary-value.
//
// -	It's data is a binary representation of some original value.
//
// -	We keep that binary representation as short and simple as possible.
//		For example - there is no nullable sign, maxlength and so on.
//		This means that we keep no original-value's meta-data.
//
// -	For now, we don't support the following value-types to be packed into variant value:
//			- Compound value;
//			- Variant value;
//			- Enum value;
//			- ObjectPtr value;
//			- User-defined value.
//
// -	Original NULL-value will not be stored as a binary representaion.
//		We do Variant_value::put_IsNull( true ) instead.
//		This means that original-value's type of NULL will be lost.
//			write:  float(NULL)   -> VARIANT(NULL),    int(NULL)->VARIANT(NULL)
//			read:   first of all, as always, test get_IsNull(), then try to access the value.
/**********************************************************************************************/


/**********************************************************************************************/
class FBL_SHARED_EXP_CLASS Value_Variant_imp :
	public Value_Raw_imp,
    public I_ValueVariant
{
	public://///////////////////////////////////////////////////////////////////////////////////

	// --- Construction ---
		
							Value_Variant_imp( I_Database_Ptr inpDb = nullptr );
							Value_Variant_imp( const Value_Variant_imp& inOther );
	
virtual 					~Value_Variant_imp( void );
							

	public://///////////////////////////////////////////////////////////////////////////////////

// interface I_Value:

	// ---------------------
	// Properties:

virtual VALUE_CATEGORY		get_Category( void ) const override
								{ return vcVariant; }

virtual VALUE_TYPE			get_Type( void ) const override
								{ return kTypeVariant; }

virtual String				get_TypeString( const char inLocale[] = "en_US" ) const override
								{ return TypeCode2String(kTypeVariant, inLocale); }
	
virtual COMPARE_TYPE		get_CompareType( void ) const override
							{
								return kBinaryCompare;
							}
	
virtual void				put_CompareType( COMPARE_TYPE inValue ) override
							{
								argused1( inValue );
							}
	
	// ---------------------
	// Serialization to/from char buffer (for ValueVariant):

virtual vuint32 			get_BinaryRepresentationByteLength( void ) const override;

virtual void				FromBinaryRepresentation( const char* inpBuffer ) override;
virtual void				ToBinaryRepresentation( char* outpBuffer ) const override;

	
	public://///////////////////////////////////////////////////////////////////////////////////

// Interface I_ValueVariant:

virtual vuint32				get_VariantValueType( void ) const override;
virtual I_Value_Ptr			get_VariantValue( void ) const  override;
virtual	void				put_VariantValue( Const_I_Value_Ptr inpValue ) override;

virtual void				put_Data(
								vuint8* 	inDataBuffer, 
								vuint32 	inBufferSize )  override;
	
    
	public://///////////////////////////////////////////////////////////////////////////////////

// Interface I_Serializable:

virtual void				From( I_PacketRcv* 	inPacket, bool inBlock = true ) override;
virtual void				To( I_PacketSnd*  inPacket, bool inBlock = true ) const override;


	public://///////////////////////////////////////////////////////////////////////////////////

// THIS CLASS:

			// <IsSingleByte>
    
		bool				get_IsSingleByte( void ) const   { return mIsSingleByte; }
		void				put_IsSingleByte( bool inValue ) { mIsSingleByte = inValue; }


	protected://////////////////////////////////////////////////////////////////////////////////

		void				put_StringData(
								vuint32			inActualDataType,
								const void*		inDataBuffer,
								vuint32 		inBufferSize );

	// FROM Methods:

		vuint32				From_DataType(
        						I_PacketRcv* 	inPacket );
	
		void				From_AsPureData(
        						I_PacketRcv* 	inPacket,
                                vuint32 		inActualDataType );
    
		void				From_OnServerSide_AsSingleByteStringData(
        						I_PacketRcv* 	inPacket,
                                vuint32 		inActualDataType );

	// TO Methods:

		void				To_DataType(
								I_PacketSnd*	inPacket,
								vuint32			inValue ) const;

		void				To_AsPureData(
        						I_PacketSnd*  	inPacket,
                                int				inSizeOfType ) const;
    
		void				To_OnServerSide_AsSingleByteStringData(
                                I_PacketSnd*  	inPacket,
                                int				inSizeOfType ) const;


	protected://////////////////////////////////////////////////////////////////////////////////
	
		I_Database*			mpDb;
		bool				mIsSingleByte; 	// TRUE if this value keeps a single byte string.
};


/**********************************************************************************************/
class FBL_SHARED_EXP_CLASS Value_Variant :
    public Value_Variant_imp
{
		IMPLEMENT_UNKNOWN(Value_Variant)
		BEGIN_INTERFACE_TABLE(Value_Variant)
			IMPLEMENT_INTERFACE(I_Value)										
			IMPLEMENT_INTERFACE(I_ValueBinary)
			IMPLEMENT_INTERFACE(I_ValueVariant)
			IMPLEMENT_INTERFACE(I_Serializable)										
		END_INTERFACE_TABLE()
	

	public://///////////////////////////////////////////////////////////////////////////////////

	// --- Construction ---

							Value_Variant( I_Database_Ptr inpDb = nullptr );
							Value_Variant( const Value_Variant& inOther );
							
virtual 					~Value_Variant( void );

	
	public://///////////////////////////////////////////////////////////////////////////////////

// interface I_Value:

virtual	I_Value_Ptr			Clone( bool inCopyData = true ) const override;

							
	// --- Value access ---

virtual	int					Compare( 
								const I_Value& 	inOther,
								COMPARE_TYPE 	inCompareType = kNatureCompare ) const override 
							{
								argused1( inCompareType );
								FBL_CHECK( Value_Variant::get_Type() == inOther.get_Type() );
								
								if( get_Length() != inOther.get_Length() )
									return -1;

								return blob_traits::compare(
									(blob_traits::value_type*) begin(),
									(blob_traits::value_type*) inOther.begin(),
									get_Length() );
							}

virtual	int					CompareToIndexValue( 
								Const_I_Value_Ptr 	inTestValue,
								const void* 		inOther,
								vuint32				inParam,
								bool				inSwapBytes) const override 
							{
								argused2( inParam, inSwapBytes );
								return blob_traits::compare(
									(blob_traits::value_type*) inTestValue->begin(),
									(blob_traits::value_type*) inOther,
									get_Length() );
							}

virtual	int					CompareIndexValues( 
								const void* inLeft,
								const void* inRight,
								bool		inSwapBytes ) const override 
							{
								argused1(inSwapBytes);
								return blob_traits::compare(
									(blob_traits::value_type*) inLeft,
									(blob_traits::value_type*) inRight, 
									get_Length() );
							}
};


#pragma mark -


/**********************************************************************************************/
class FBL_SHARED_EXP_CLASS Value_Variant_null :
    public Value_Variant
{
	public://///////////////////////////////////////////////////////////////////////////////////

	// --- Construction ---

							Value_Variant_null( I_Database_Ptr inpDb = nullptr );
							Value_Variant_null( const Value_Variant_null& inOther );
							
virtual 					~Value_Variant_null( void );

	
	public://///////////////////////////////////////////////////////////////////////////////////

// interface I_Value:

virtual	I_Value_Ptr			Clone( bool inCopyData = true ) const override;


	// ---------------------
	// Properties:

virtual bool				get_IsNullable( void ) const override
								{ return true; }
	
virtual bool 				get_IsNull( void ) const override
								{ return mIsNull; }

virtual void 				put_IsNull( bool inValue ) override
							{
								if( (mIsNull = inValue) )
									Value_Variant::put_IsNull( inValue );
							}



	// ---------------------
	// Value access:

virtual	void				Assign( const I_Value& inValue ) override
							{
								if( inValue.get_IsNull() )
									put_IsNull( true );
								else
								{
									mIsNull = false;
									Value_Variant::Assign( inValue );
								}
							}

virtual	int					Compare( 
								const I_Value& 	inOther,
								COMPARE_TYPE 	inCompareType = kNatureCompare ) const override
							{
								argused1( inCompareType );
								FBL_CHECK( Value_Variant::get_Type() == inOther.get_Type() );

								bool otherIsNull = inOther.get_IsNull();

								/// If one of values (this or inOther) have null...
								if( otherIsNull || mIsNull )
								{
									/// ... we check only isNull flags.
									return mIsNull == otherIsNull ? 0 
																  : ( mIsNull == true ? -1 : 1 );		
								}
								else
								{
									/// ... have not null binary - compare chars.
									return Value_Variant::Compare( inOther );
								}
							}
};


/**********************************************************************************************/
FBL_SMART_PTR(Value_Variant);
FBL_SMART_PTR(Value_Variant_null);


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_Value_Variant_h
