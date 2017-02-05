/**********************************************************************************************/
/* FBL_Value_Enum.h 	                                                   					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_Value_Enum_h 
	#define _FBL_Value_Enum_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Values/FBL_Value_Numeric.h>
#include <VShared/FBL/publ/Utilities/FBL_TypeUtilities.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// This class implements 
//
template 
<
	vuint32 		kFldTypeIndex, 
	class 			policyStorage
>
class FBL_SHARED_EXP_CLASS Value_enum : 
	public Value_Numeric< kFldTypeIndex, vcUInteger, policyStorage >
{
	public://///////////////////////////////////////////////////////////////////////////////////

		typedef policyStorage 									PS;
		typedef Value_Numeric< kFldTypeIndex, vcUInteger, PS >	super;

		typedef	Value_enum										this_type;

		typedef       smart_ptr<this_type>						pointer;
		typedef const pointer									const_pointer;
		typedef       this_type&								reference;
		typedef const this_type&								const_reference;


	public://///////////////////////////////////////////////////////////////////////////////////

	// Construction:

							Value_enum( I_Type_Enumerated_Ptr inpType )
							:
								mpEnumType( inpType )
							{
								mpType = QI( mpEnumType, I_Type );
								InternalInit();
							}


							/**	COPY-ctor.
								@param inOther - other Value_enum.*/
							Value_enum( 
								const_reference 	inOther )
							:
								super( inOther ),
								mpEnumType( inOther.mpEnumType ),
								mpType( inOther.mpType )	
							{
								PS::mValue = inOther.mValue;
							}


virtual						~Value_enum( void )
							{
							}
						

	public://///////////////////////////////////////////////////////////////////////////////////

		using Value_Numeric< kFldTypeIndex, vcUInteger, PS >::Assign;

		// I_Value API:

virtual	I_Value_Ptr			Clone( bool inCopyData = true ) const override
							{ 
								I_Value_Ptr result;

								if( inCopyData == true )
								{
									result = new Value_enum( *this );
								}
								else
								{
									result = new Value_enum( mpEnumType );
									result->put_IsNull( true );
								}

								result->put_IsRemote(super::mIsRemote);
								return result;
							}


virtual vuint32				get_MaxChars( void ) const override
							{
								return (mpType) ? mpType->get_MaxChars() : 0;
							}


virtual VALUE_CATEGORY		get_Category( void ) const override	{ return vcEnum; }

virtual VALUE_TYPE			get_Type( void ) const override { return kTypeEmpty; }	

virtual String				get_TypeString( const char inLocale[] = "en_US" ) const override
							{ 
								argused1( inLocale );
								return (mpType) ? mpType->get_TypeName() : "Unknown";
							}



		// ---------------------
		// get/put string:

virtual String				get_String( tslen inLimit = -1 ) const override
							{
								FBL_CHECK( mpType && mpEnumType );
								String str;

								if( inLimit != 0 )
								{		
									tslen MaxLen = static_cast<tslen>( mpType->get_MaxChars() );
									UChar* pBuffer = str.getBuffer(MaxLen + 1);

									UChar* pEnd = get_String( pBuffer, -1 );

									str.releaseBuffer(static_cast<tslen>(pEnd - pBuffer));

									// not -1 and less than maxLen
									if( inLimit > 0 && inLimit < MaxLen )
									{
										str.truncate( inLimit );
									}
								}

								return str;
							}

virtual void				put_String( const String& inStr ) override
							{
								FBL_CHECK( mpType && mpEnumType );
								PS::mValue = (typename this_type::value_type)GetEnumValueAsInt( inStr, mpEnumType );
								super::put_IsNull(false);
							}


virtual UChar*				get_String( 
								UChar* 			outString, 
								tslen 			inBufferChars ) const override
							{
								FBL_CHECK( mpType && mpEnumType );

								if( inBufferChars && PS::mValue )
								{
									const String& value_str = mpEnumType->get_IdentAsString( PS::mValue );
									if( inBufferChars == -1 )
										inBufferChars = value_str.length();
									else
										inBufferChars = Min(value_str.length(), inBufferChars);

									vu_strncpy( outString, value_str.c_str(), inBufferChars );
								}

								return outString + inBufferChars;
							}	


virtual void				put_String( 
								const UChar* 	inStart,
								const UChar* 	inEnd = nullptr ) override
							{
								if( inEnd == nullptr )
								{
									String value( inStart );
									put_String( value );
								}
								else
								{
									FBL_CHECK( inEnd > inStart );
									String value( inStart, inEnd );
									put_String( value );
								}
							}


virtual char*				get_String( 
								char* 			outString, 
								tslen 			inBufferChars ) const override
							{
								FBL_CHECK( mpType && mpEnumType );

								if( inBufferChars && PS::mValue )
								{
									const String& value_str = mpEnumType->get_IdentAsString( PS::mValue );
									if( inBufferChars == -1 )
										inBufferChars = value_str.length();
									else
										inBufferChars = Min(value_str.length(), inBufferChars);

									vu_u2a( value_str.c_str(), outString, inBufferChars );		
								}

								return outString + inBufferChars;
							}


virtual void				put_String( 
								const char* 	inStart,
								const char* 	inEnd = nullptr ) override
							{
								if( inEnd == nullptr )
								{
									String value( inStart );
									put_String( value );
								}
								else
								{
									FBL_CHECK( inEnd > inStart );
									String value( inStart, (tslen)(inEnd-inStart) );
									put_String( value );
								}
							}


	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// This class API

virtual	I_Type_Enumerated_Ptr get_EnumType( void ) const
								{	return mpEnumType;	}


	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// Numeric properties:


virtual void				put_Boolean( bool inValue ) override
							{ 
								CheckValue( (vuint16)inValue, mpEnumType );
								super::put_Boolean( inValue ); 
							}

virtual void				put_Byte( vuint8 inValue ) override
							{ 
								CheckValue( (vuint16)inValue, mpEnumType );
								super::put_Byte( inValue ); 
							}

virtual void				put_Short( vint16 inValue ) override
							{ 
								CheckValue( (vuint16)inValue, mpEnumType );
								super::put_Short( inValue ); 
							}

virtual void				put_UShort( vuint16 inValue ) override
							{ 
								CheckValue( (vuint16)inValue, mpEnumType );
								super::put_UShort( inValue ); 
							}

virtual void				put_Medium( vint32 inValue ) override
							{ 
								CheckValue( (vuint16)inValue, mpEnumType );
								super::put_Medium( inValue ); 
							}

virtual void				put_Long( vint32 inValue ) override
							{ 
								CheckValue( (vuint16)inValue, mpEnumType );
								super::put_Long( inValue ); 
							}

virtual void				put_ULong( vuint32 inValue ) override
							{ 
								CheckValue( (vuint16)inValue, mpEnumType );
								super::put_ULong( inValue ); 
							}

virtual void				put_LLong( vint64 inValue ) override
							{ 
								CheckValue( (vuint16)inValue, mpEnumType );
								super::put_LLong( inValue ); 
							}

virtual void				put_ULLong( vuint64 inValue ) override
							{ 
								CheckValue( (vuint16)inValue, mpEnumType );
								super::put_ULLong( inValue ); 
							}

virtual void				put_Float( float inValue ) override
							{ 
								CheckValue( (vuint16)inValue, mpEnumType );
								super::put_Float( inValue ); 
							}

virtual void				put_Double( double inValue ) override
							{ 
								CheckValue( (vuint16)inValue, mpEnumType );
								super::put_Double( inValue ); 
							}


	public://///////////////////////////////////////////////////////////////////////////////////

	// interface I_Serializable:

virtual void				From( I_IStream_Ptr inStream, bool inBlock = true ) override
							{
								// This type already knows own I_Type (you can not create this value without passing
								// I_Type). So we should only pass the actual value only.
								FBL_CHECK( mpType && mpEnumType );
								super::From( inStream, inBlock );
							}

virtual void				From( I_PacketRcv* inPacket, bool inBlock = true ) override
							{
								// This type already knows own I_Type (you can not create this value without passing
								// I_Type). So we should only pass the actual value only.
								FBL_CHECK( mpType && mpEnumType );
								super::From( inPacket, inBlock );
							}



virtual void				To( I_OStream_Ptr inStream, bool inBlock = true ) const override
							{
								// This type already knows own I_Type (you can not create this value without passing
								// I_Type). So we should only pass the actual value only.
								FBL_CHECK( mpType && mpEnumType );
								super::To( inStream, inBlock );
							}

virtual void				To( I_PacketSnd* inPacket, bool inBlock = true ) const override
							{
								// This type already knows own I_Type (you can not create this value without passing
								// I_Type). So we should only pass the actual value only.
								FBL_CHECK( mpType && mpEnumType );
								super::To( inPacket, inBlock );
							}


	protected://////////////////////////////////////////////////////////////////////////////////

		void				InternalInit( void )
							{
								FBL_CHECK( mpType && mpEnumType && mpEnumType->get_IdentCount() );
								const String& DefaultValue = mpType->get_DefaultValue();	
								PS::mValue = (typename this_type::value_type)(
												( DefaultValue.isEmpty() ) ? 
													0 : 
													(vuint16) mpEnumType->get_IdentAsLong( DefaultValue ));
							}



	protected://////////////////////////////////////////////////////////////////////////////////

	/// --- Data members ---
	
		I_Type_Enumerated_Ptr	mpEnumType;
		I_Type_Ptr				mpType;

	public://///////////////////////////////////////////////////////////////////////////////////

#if FBL_TEST_CODE

							Value_enum(
								I_Type_Enumerated_Ptr	inpType,
								vint32					inValue )
							:
								mpEnumType( inpType )
							{
								mpType = QI( mpEnumType, I_Type );
								InternalInit();
								CheckValue( (vuint16)inValue, mpEnumType );
								super::put_Long( inValue );
							}

#endif // FBL_TEST_CODE

	// Section of class with TEST - helper methods.
	// for internal use.

virtual void				Init( void ) override
							{ 
								// Intentionally not implemented
								FBL_CHECK(false);
							}

virtual void 				Increment( void ) override
							{ 
								// Intentionally not implemented
								FBL_CHECK(false);
							}

};


/**********************************************************************************************/
///
#pragma mark class Value_enum_null

template 
<
	vuint32 		kFldTypeIndex, 
	class 			policyStorage
>
class FBL_SHARED_EXP_CLASS Value_enum_null : 
	public Value_enum<kFldTypeIndex, policyStorage>,
	protected policyNullable< policyStorage >
{
	public:///////////////////////////////////////////////////////////////////////////////////

		typedef policyStorage 									PS;
		typedef Value_enum< kFldTypeIndex, PS >					super;		
		typedef policyNullable<policyStorage>					PN;

	public://///////////////////////////////////////////////////////////////////////////////////

		typedef	Value_enum_null									this_type;

		typedef       smart_ptr<this_type>						pointer;
		typedef const pointer									const_pointer;
		typedef       this_type&								reference;
		typedef const this_type&								const_reference;


	public://///////////////////////////////////////////////////////////////////////////////////

	// --- Construction ---

							Value_enum_null( I_Type_Enumerated_Ptr inpType )
							:
								super( inpType )								
							{
								if( PS::mValue )
									put_IsNull( false );
							}

							/**	This is are copy-ctor. 
								@param inOther - other Value_enum_null.*/
							Value_enum_null( const_reference inOther )
							:
								super( inOther )
							{
								if( inOther.get_IsNull() == false ) 
									this_type::put_IsNull( false );
							}

virtual						~Value_enum_null( void )
							{
							}
							

	public://///////////////////////////////////////////////////////////////////////////////////

// interface I_Value:
virtual bool				get_IsNullable( void ) const override { return true; }
virtual bool 				get_IsNull( void ) const override
							{
								return PN::get_IsNull(); 
							}

virtual void 				put_IsNull( bool inValue ) override
							{
								PN::put_IsNull( PS::get_ValuePtr(), inValue ); 
							}


virtual I_Value_Ptr			Clone( bool inCopyData ) const override
							{
								I_Value_Ptr result;

								if( inCopyData == true )
								{
									result = new Value_enum_null( *this );
								}
								else
								{
									result = new Value_enum_null( super::mpEnumType );
									result->put_IsNull( true );
								}

								result->put_IsRemote(super::mIsRemote);
								return result;
							}


virtual	int					Compare( 
								const I_Value& 	inOther, 
								COMPARE_TYPE 	inCompareType = kNatureCompare ) const override
							{
								argused1( inCompareType );
								FBL_CHECK( this->get_Type() == inOther.get_Type() ); 

								// If one of values (this or inOther) have null...
								if( get_IsNull() )
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
										return super::Compare( inOther );    	
									}
								}								
							}


virtual String				get_String( 
								tslen 		inLimit = -1 ) const override
							{
								if( get_IsNull() )
									return String::sEmpty();

								return super::get_String(inLimit);
							}


virtual UChar*				get_String( 
								UChar* 		outString, 
								tslen 		inBufferChars ) const override
							{
								if( get_IsNull() )
								{
									if( inBufferChars > 0 )
										outString[0] = 0;
									return outString;
								}

								return super::get_String(outString, inBufferChars);
							}


virtual char*				get_String( 
								char* 		outString, 
								tslen 		inBufferChars ) const override
							{
								if( get_IsNull() )
								{
									if( inBufferChars > 0 )
										outString[0] = 0;
									return outString;
								}

								return super::get_String(outString, inBufferChars);
							}

virtual void				put_String( const String& inStr ) override
							{
								put_IsNull( false );
								super::put_String( inStr );
							}

virtual void				put_String( const UChar* inBegin, const UChar* inEnd ) override
							{
								put_IsNull( false );								
								super::put_String( inBegin, inEnd );
							}

virtual void				put_String( const char* inBegin, const char* inEnd = nullptr ) override
							{
								put_IsNull( false );								
								super::put_String( inBegin, inEnd );
							}


	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// Numeric properties:


virtual void				put_Boolean( bool inValue ) override
							{ 
								put_IsNull( false );
								super::put_Boolean( inValue ); 
							}

virtual void				put_Byte( vuint8 inValue ) override
							{ 
								put_IsNull( false );
								super::put_Byte( inValue ); 
							}

virtual void				put_Short( vint16 inValue ) override
							{ 
								put_IsNull( false );
								super::put_Short( inValue ); 
							}

virtual void				put_UShort( vuint16 inValue ) override
							{ 
								put_IsNull( false );
								super::put_UShort( inValue ); 
							}

virtual void				put_Medium( vint32 inValue ) override
							{ 
								put_IsNull( false );
								super::put_Medium( inValue ); 
							}

virtual void				put_Long( vint32 inValue ) override
							{ 
								put_IsNull( false );
								super::put_Long( inValue ); 
							}

virtual void				put_ULong( vuint32 inValue ) override
							{ 
								put_IsNull( false );
								super::put_ULong( inValue ); 
							}

virtual void				put_LLong( vint64 inValue ) override
							{ 
								put_IsNull( false );
								super::put_LLong( inValue ); 
							}

virtual void				put_ULLong( vuint64 inValue ) override
							{ 
								put_IsNull( false );
								super::put_ULLong( inValue ); 
							}

virtual void				put_Float( float inValue ) override
							{ 
								put_IsNull( false );
								super::put_Float( inValue ); 
							}

virtual void				put_Double( double inValue ) override
							{ 
								put_IsNull( false );
								super::put_Double( inValue ); 
							}
};


/**********************************************************************************************/
typedef Value_enum<kTypeByte, policyInPlace_byte>				Value_enum8;
typedef Value_enum<kTypeUShort, policyInPlace_ushort>			Value_enum16;

typedef Value_enum_null<kTypeByte, policyInPlace_byte>			Value_enum_null8;
typedef Value_enum_null<kTypeUShort, policyInPlace_ushort>		Value_enum_null16;


/**********************************************************************************************/
template<>
inline
VALUE_TYPE Value_enum<kTypeByte, policyInPlace_byte>::get_Type( void ) const 
{ 
	return kTypeEnum8; 
}	

/**********************************************************************************************/
template<>
inline
VALUE_TYPE Value_enum<kTypeUShort, policyInPlace_ushort>::get_Type( void ) const 
{ 
	return kTypeEnum16; 
}	


/**********************************************************************************************/
FBL_SMART_PTR( Value_enum8 );
FBL_SMART_PTR( Value_enum16 );
FBL_SMART_PTR( Value_enum_null8 );
FBL_SMART_PTR( Value_enum_null16 );


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_Value_enum_h
