/**********************************************************************************************/
/* FBL_Value_Binary.h 		                                               					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_Value_Binary_h 
	#define _FBL_Value_Binary_h
#pragma once


// FBL:
#include <VShared/FBL/publ/Interfaces/FBL_I_Value.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_ValueBinary.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Serializable.h>

#include <VShared/FBL/publ/Values/FBL_Convert.h>
#include <VShared/FBL/publ/Values/FBL_type_traits.h>
#include <VShared/FBL/publ/Values/FBL_Value_StoragePolicy.h>
#include <VShared/FBL/publ/Values/FBL_Type2String.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// Implements Value that requires RAM buffer: string, blob, ... 
// This is the midle class that implements the most of the raw buffer functions.
// It will be used as a base class for Value_Raw_T where will be finished implementation
// for Compare(). 
// 
#pragma mark class Value_Raw

class FBL_SHARED_EXP_CLASS Value_Raw : 
	public I_Value, 
    public I_ValueBinary, 
    public I_Serializable
{
		IMPLEMENT_UNKNOWN(Value_Raw)											
		BEGIN_INTERFACE_TABLE(Value_Raw)										
			IMPLEMENT_INTERFACE(I_Value)										
			IMPLEMENT_INTERFACE(I_ValueBinary)										
			IMPLEMENT_INTERFACE(I_Serializable)										
		END_INTERFACE_TABLE()

	public://///////////////////////////////////////////////////////////////////////////////////

	// --- Construction ---
		
							Value_Raw( void );
							Value_Raw( vuint32 inSize, vuint32 inDeltaPlus = 0 );
							Value_Raw( const Value_Raw& inOther );
							
virtual 					~Value_Raw( void );
							

	public://///////////////////////////////////////////////////////////////////////////////////

// Interface I_Value:

	// ---------------------
	// Properties:
	
virtual vuint32 			get_Allocated( void ) const override   	
								{ return vuint32(m_pBufferEnd - m_pStart); }
								
virtual bool 				put_Allocated( vuint32 inSize ) override;

virtual vuint32 			get_ByteLength( void ) const override		{ return get_Length(); }
virtual void 				put_ByteLength( vuint32 inNewLength ) override;

virtual	vuint32				get_ByteLengthForIndex( void ) const override;

virtual bool				get_IsNullable( void ) const override		{ return false; }
virtual bool 				get_IsNull( void ) const override;
virtual void 				put_IsNull( bool inValue ) override;

virtual bool				get_IsRemote( void ) const override  		{ return mIsRemote; }
virtual void 				put_IsRemote( bool inValue ) override		{ mIsRemote = inValue; }

virtual vuint32 			get_Length( void ) const override			{ return vuint32(m_pEnd - m_pStart); }
virtual vuint32 			put_Length( vuint32 inNewLen );
							
virtual vuint32				get_MaxChars( void ) const override  		{ return vuint32_max; }

virtual vuint32				get_StorageSize( void ) const override  	
								{ return mDeltaPlus + get_Allocated(); }


	// ---------------------
	// Value access:

virtual const char*			begin( void ) const override 				{ return m_pStart; 	}
virtual const char*			end( void ) const override					{ return m_pEnd; }

virtual	void				Assign( const I_Value& inValue ) override;

virtual vuint32				CopyToIndex( void* outBuffer, bool inSwapBytes ) const override 
								{ argused2(outBuffer, inSwapBytes); return 0; }

virtual void 				CopyFromIndex( const void* inBuffer, bool inSwapBytes ) override 
								{ argused2(inBuffer, inSwapBytes); }


	// ---------------------
	// Value compare methods:

virtual	String				get_String( tslen inLimit = -1 ) const override;
virtual void				put_String( const String& inStr ) override;

virtual void				put_String( const UChar* inBegin, const UChar* inEnd = nullptr ) override;
virtual UChar*				get_String( UChar* outString, tslen inBufferChars ) const override;


virtual void				put_String( const char* inBegin, const char* inEnd = nullptr ) override;
virtual char*				get_String( char* outString, tslen inBufferChars ) const override;



	// ---------------------
	// Numeric properties:

virtual void				put_Boolean( bool inValue ) override				{ argused1( inValue ); }
virtual bool				get_Boolean( void ) const override				{ return 0; }

virtual void				put_Byte( vuint8 inValue ) override				{ argused1( inValue ); }
virtual vuint8				get_Byte( void ) const override					{ return 0; }

virtual void				put_Short( vint16 inValue ) override				{ argused1( inValue ); }
virtual vint16				get_Short( void ) const override					{ return 0; }

virtual void				put_UShort( vuint16 inValue ) override			{ argused1( inValue ); }
virtual vuint16				get_UShort( void ) const override				{ return 0; }

virtual void				put_Medium( vint32 inValue ) override			{ argused1( inValue ); }
virtual vint32				get_Medium( void ) const override				{ return 0; }

virtual void				put_Long( vint32 inValue ) override				{ argused1( inValue ); }
virtual vint32				get_Long( void ) const override					{ return 0; }

virtual void				put_ULong( vuint32 inValue ) override			{ argused1( inValue ); }
virtual vuint32				get_ULong( void ) const override					{ return 0; }

virtual void				put_LLong( vint64 inValue ) override				{ argused1( inValue ); }
virtual vint64				get_LLong( void ) const override					{ return 0; }

virtual void				put_ULLong( vuint64 inValue ) override			{ argused1( inValue ); }
virtual vuint64				get_ULLong( void ) const override				{ return 0; }

virtual void				put_Float( float inValue ) override				{ argused1( inValue ); }
virtual float				get_Float( void ) const override					{ return 0; }

virtual void				put_Double( double inValue ) override			{ argused1( inValue ); }
virtual double				get_Double( void ) const override				{ return 0; }


	// ---------------------
	// Byte swapping:

virtual void				SwapBytes( void ) override 							{}
virtual void				SwapBytes( void* inValue ) const override 			{ argused1(inValue); }							
virtual void				SwapBytes( const void* inSrc, void* inDest ) override{ argused2(inSrc, inDest); }


	public://///////////////////////////////////////////////////////////////////////////////////

// Interface I_Serializable:

virtual void				From( I_IStream_Ptr inStream, bool inBlock = true ) override;
virtual void				From( I_PacketRcv* 	inPacket, bool inBlock = true ) override;

virtual void				To( I_OStream_Ptr inStream, bool inBlock = true ) const override;
virtual void				To( I_PacketSnd*  inPacket, bool inBlock = true ) const override;



	public://///////////////////////////////////////////////////////////////////////////////////

// Interface I_ValueBinary:
virtual vuint32				get_Data( vuint8* outDataBuffer, vuint32 inBufferSize ) const override;
virtual void				put_Data( vuint8* inDataBuffer,  vuint32 inBufferSize ) override;


	protected:////////////////////////////////////////////////////////////////////////////////// 

		void 				Alloc( vuint32 inSize );
		const char*			AllocCopy( const char* inStart, const char* inEnd );

		void 				Clear( void );	
		void				InitSelf( void );


	private:////////////////////////////////////////////////////////////////////////////////////

							/**	Tries to truncate value size to inBySize bytes.
								Returns a new size. */
		vuint32				TruncateTo( vuint32 inNewSize );
							
							/**	Tries to grow value by inBySize bytes.
								Returns a new size. */
		vuint32				GrowBy( vuint32 inBySize );

							
	protected://////////////////////////////////////////////////////////////////////////////////

	// --- Data members ---
	
		char*				m_pStart;		///< The start of the buffer.
		char*				m_pBufferEnd;	///< The end of the buffer. 
		char*				m_pEnd;			///< The end of the value. 
		
		vuint8				mDeltaPlus;		///< To support FixedBinary value that have +2 for length
		
		bool				mIsRemote; 		// implements <IsRemote> property.


	public://///////////////////////////////////////////////////////////////////////////////////

//#if FBL_TEST_CODE

virtual void				Init( void ) override;
virtual void 				Increment( void ) override;

//#endif // FBL_TEST_CODE


}; 


#pragma mark -


/**********************************************************************************************/
// Template class that add type index
//
#pragma mark class Value_Raw_T<traits, kFldTypeIndex>

template<class traits, vuint32 kFldTypeIndex>
class Value_Raw_T : public Value_Raw
{
	public://///////////////////////////////////////////////////////////////////////////////////

		typedef Value_Raw_T<traits,kFldTypeIndex>				this_type;

		typedef smart_ptr<this_type>							pointer;
		typedef const pointer									const_pointer;
		typedef this_type&										reference;
		typedef const this_type&								const_reference;


	public://///////////////////////////////////////////////////////////////////////////////////

	// --- Construction ---

							Value_Raw_T( void )  {}	

							Value_Raw_T( vuint32 inSize, vuint32 inDeltaPlus = 0 )  
								:  Value_Raw(inSize, inDeltaPlus) {}	

							Value_Raw_T( const char* inValue, const char* inEnd )  :	
								Value_Raw()
							{
								AllocCopy( inValue, inEnd );	
							}
							
							Value_Raw_T( const_reference inOther )  :	
								Value_Raw( inOther )
							{
							}

							
	public://///////////////////////////////////////////////////////////////////////////////////

// interface I_Value:

virtual	I_Value_Ptr			Clone( bool inCopyData = true ) const override  
							{
								I_Value_Ptr result;

								if( inCopyData == true )
									result = new Value_Raw_T<traits,kFldTypeIndex>( *this );
								else
								{
									result = new Value_Raw_T<traits,kFldTypeIndex>( get_Allocated(), Value_Raw::mDeltaPlus );
									result->put_IsNull( true );
								}

								result->put_IsRemote( mIsRemote );

								return result;
							};
							
	// --- Attributes ---

virtual VALUE_TYPE			get_Type( void ) const override   { return kFldTypeIndex; }

virtual String				get_TypeString( const char inLocale[] = "en_US" ) const override  
								{ return TypeCode2String(kFldTypeIndex, inLocale); }
								
virtual VALUE_CATEGORY		get_Category( void ) const override  	{ return vcBinary; }


	// --- Value access ---

virtual	int					Compare( 
								const I_Value& 	inOther,
								COMPARE_TYPE 	inCompareType = kNatureCompare ) const override 
							{
								argused1( inCompareType );
								FBL_CHECK( get_Type() == inOther.get_Type() ); 
								
								if( get_Length() != inOther.get_Length() )
									return -1;

								return traits::compare( 
									(typename traits::value_type*) begin(), 
									(typename traits::value_type*) inOther.begin(), 
									get_Length() );
							}

virtual	int					CompareToIndexValue( 
								Const_I_Value_Ptr 	inTestValue,
								const void* 		inOther,
								vuint32				inParam,
								bool				inSwapBytes) const override 
							{
								argused2( inParam, inSwapBytes );
								return traits::compare( 
									(typename traits::value_type*) inTestValue->begin(), 
									(typename traits::value_type*) inOther, 
									get_Length() );
							}

virtual	int					CompareIndexValues( 
								const void* inLeft,
								const void* inRight,
								bool		inSwapBytes ) const override 
							{
								argused1(inSwapBytes);
								return traits::compare( 
									(typename traits::value_type*) inLeft, 
									(typename traits::value_type*) inRight, 
									get_Length() );
							}
};


/**********************************************************************************************/
///
#pragma mark class Value_Raw_Null

template<class traits, vuint32 kFldTypeIndex>
class Value_Raw_Null_T : public Value_Raw_T<traits, kFldTypeIndex>
{
	public://///////////////////////////////////////////////////////////////////////////////////

		typedef Value_Raw_T<traits, kFldTypeIndex>				inherited;

		typedef	Value_Raw_Null_T<traits, kFldTypeIndex>			this_type;
		typedef	Value_Raw_T<traits, kFldTypeIndex>				base_type;

		typedef       smart_ptr<this_type>						pointer;
		typedef const pointer									const_pointer;
		typedef       this_type&								reference;
		typedef const this_type&								const_reference;


	public://///////////////////////////////////////////////////////////////////////////////////

	// --- Construction ---

							Value_Raw_Null_T( void )  
							{}	

							Value_Raw_Null_T( vuint32 inSize, vuint32 inDeltaPlus = 0 )  
								: base_type(inSize, inDeltaPlus) 
							{}	

							Value_Raw_Null_T( const char* inValue, const char* inEnd )  
								: base_type(inValue, inEnd)
							{}
							
							Value_Raw_Null_T( const_reference inOther )  
								: base_type( inOther )
							{}

virtual						~Value_Raw_Null_T( void ) 
							{}
							
							
	public://///////////////////////////////////////////////////////////////////////////////////

// interface I_Value:

virtual	I_Value_Ptr			Clone( bool inCopyData = true ) const  
							{
								I_Value_Ptr result;

								if( inCopyData == true )
									result = new this_type( *this );
								else
								{
									result = new this_type( this->get_Allocated(), inherited::mDeltaPlus );
									result->put_IsNull( true );
								}

								result->put_IsRemote( inherited::mIsRemote );

								return result;
							}

	// ---------------------
	// Properties:

virtual bool				get_IsNullable( void ) const  { return true; }
	
virtual bool 				get_IsNull( void ) const 
								{ return mIsNull; }

virtual void 				put_IsNull( bool inValue ) 
							{
								if( (mIsNull = inValue) )
								{
									base_type::m_pEnd = base_type::m_pStart;
									if( base_type::m_pEnd )
										*base_type::m_pEnd = 0;
								}
							}



	// ---------------------
	// Value access:

virtual	void				Assign( const I_Value& inValue ) 
							{
								if( inValue.get_IsNull() )
									put_IsNull( true );
								else
								{
									mIsNull = false;
									base_type::Assign( inValue );
								}
							}

virtual	int					Compare( 
								const I_Value& 	inOther,
								COMPARE_TYPE 	inCompareType = kNatureCompare ) const 
							{
								argused1( inCompareType );
								FBL_CHECK( base_type::get_Type() == inOther.get_Type() );

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
									return base_type::Compare( inOther );
								}
							}


	protected://////////////////////////////////////////////////////////////////////////////////

		bool				mIsNull;		///< nullable flag.
};


/**********************************************************************************************/
typedef Value_Raw_T<blob_traits, kTypeFixedBinary>		Value_fixedbinary;
typedef Value_Raw_T<blob_traits, kTypeVarBinary>		Value_varbinary;
typedef Value_Raw_T<blob_traits, kTypePicture>			Value_picture;

//
typedef Value_Raw_Null_T<blob_traits, kTypeFixedBinary>	Value_fixedbinary_null;
typedef Value_Raw_Null_T<blob_traits, kTypeVarBinary>	Value_varbinary_null;
//typedef Value_Raw_Null_T<blob_traits, kTypePicture>		Value_picture_null;


/**********************************************************************************************/
FBL_SMART_PTR(Value_Raw);

FBL_SMART_PTR(Value_fixedbinary);
FBL_SMART_PTR(Value_fixedbinary_null);


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_Value_Binary_h
