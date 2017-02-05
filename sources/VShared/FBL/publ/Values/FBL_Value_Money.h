/**********************************************************************************************/
/* FBL_Value_Money.h 	                                                   					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_Value_Money_h
	#define _FBL_Value_Money_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Values/FBL_Value_Numeric.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_CLASS(Value_money);
SMART_CLASS(Value_money_null);


/**********************************************************************************************/
template <int Prec> struct DecimalFactor {
    static const vint64 value = 10 * DecimalFactor<Prec - 1>::value;
};

template <> struct DecimalFactor<0> {
    static const vint64 value = 1;
};

template <> struct DecimalFactor<1> {
    static const vint64 value = 10;
};


/**********************************************************************************************/
inline vint64 round( double inValue )
{
	return ( inValue < 0.0 ) ? (vint64)(inValue - 0.5) : (vint64)(inValue + 0.5);
}


/**********************************************************************************************/
#define MONEY_PRECISION		4
#define MONEY_SCALE			15


/**********************************************************************************************/
// Implements Value_money_imp.
// 
class FBL_SHARED_EXP_CLASS Value_money_imp :
	public Value_Numeric< kTypeMoney, vcMoney, policyInPlace_llong >
{
		IMPLEMENT_UNKNOWN(Value_money_imp)
		BEGIN_INTERFACE_TABLE(Value_money_imp)
			IMPLEMENT_INTERFACE(I_Value)										
			IMPLEMENT_INTERFACE(I_Serializable)										
		END_INTERFACE_TABLE()

	public://///////////////////////////////////////////////////////////////////////////////////
		
	public://///////////////////////////////////////////////////////////////////////////////////

							Value_money_imp( void )
							{
							}

#if FBL_TEST_CODE
							Value_money_imp( double inValue )
							:
								Value_Numeric< kTypeMoney, vcMoney, policyInPlace_llong >(
									round( static_cast<double>(DecimalFactor<MONEY_PRECISION>::value) * inValue ) )
							{
							}
#endif // FBL_TEST_CODE
							Value_money_imp( const Value_money_imp& inOther )
							:
								Value_Numeric< kTypeMoney, vcMoney, policyInPlace_llong >( inOther )
							{
							}

virtual						~Value_money_imp( void )
							{
							}


	public://///////////////////////////////////////////////////////////////////////////////////

// I_Value API:

		using Value_Numeric< kTypeMoney, vcMoney, policyInPlace_llong >::Assign;

	// ---------------------
	// Properties:

virtual vuint32				get_MaxChars( void ) const override;


	// ---------------------
	// get/put string:

virtual String				get_String( tslen inLimit = -1 ) const override;
virtual void				put_String( const String& inStr ) override;

virtual UChar*				get_String( 
								UChar* 			outString, 
								tslen 			inBufferChars ) const override;

virtual void				put_String( 
								const UChar* 	inStart,
								const UChar* 	inEnd = nullptr ) override;
								
virtual char*				get_String( 
								char* 			outString, 
								tslen 			inBufferChars ) const override;

virtual void				put_String( 
								const char* 	inStart,
								const char* 	inEnd = nullptr ) override;


	// ---------------------
	// Numeric properties:

virtual void				put_Boolean( bool inValue ) override;
virtual bool				get_Boolean( void ) const override;

virtual void				put_Byte( vuint8 inValue ) override;
virtual vuint8				get_Byte( void ) const override;

virtual void				put_Short( vint16 inValue ) override;
virtual vint16				get_Short( void ) const override;

virtual void				put_UShort( vuint16 inValue ) override;
virtual vuint16				get_UShort( void ) const override;

virtual void				put_Medium( vint32 inValue ) override;
virtual vint32				get_Medium( void ) const override;

virtual void				put_LLong( vint64 inValue ) override;
virtual vint64				get_LLong( void ) const override;

virtual void				put_ULLong( vuint64 inValue ) override;
virtual vuint64				get_ULLong( void ) const override;

virtual void				put_Float( float inValue ) override;
virtual float				get_Float( void ) const override;

virtual void				put_Double( double inValue ) override;
virtual double				get_Double( void ) const override;

};


#pragma mark === Value_money ===

/**********************************************************************************************/
// Implements Value money.
// 
class FBL_SHARED_EXP_CLASS Value_money : public Value_money_imp
{

public://///////////////////////////////////////////////////////////////////////////////////

							Value_money( void )
							{
							}

#if FBL_TEST_CODE
							Value_money( double inValue )
							:
								Value_money_imp( inValue )
							{
							}
#endif // FBL_TEST_CODE

							Value_money( const Value_money& inOther )
							:
								Value_money_imp( inOther )
							{
							}

virtual						~Value_money( void )
							{
							}


virtual	I_Value_Ptr			Clone( bool inCopyData = true ) const override;

};


#pragma mark === Value_money_null ===

/**********************************************************************************************/
// Implements nullable Value money.
// 
class FBL_SHARED_EXP_CLASS Value_money_null :
	public Value_money,
	protected policyNullable< policyInPlace_llong >

{
	public:///////////////////////////////////////////////////////////////////////////////////

		typedef policyNullable<policyInPlace_llong>					PN;


	public:///////////////////////////////////////////////////////////////////////////////////

							Value_money_null( void )
							:
								Value_money()
							{
							}

#if FBL_TEST_CODE
							Value_money_null( double inValue )
							:
								Value_money( inValue )
							{
								put_IsNull(false);
							}
#endif // FBL_TEST_CODE

							Value_money_null( const Value_money& inOther )
							:
								Value_money( inOther )
							{
								put_IsNull( inOther.get_IsNull() );
							}

virtual						~Value_money_null( void )
							{
							}

	public:///////////////////////////////////////////////////////////////////////////////////

virtual bool				get_IsNullable( void ) const override  { return true; }
virtual bool 				get_IsNull( void ) const override;
virtual void 				put_IsNull( bool inValue ) override;
							
virtual I_Value_Ptr			Clone( bool inCopyData ) const override;

virtual	int					Compare( 
								const I_Value& 	inOther, 
								COMPARE_TYPE 	inCompareType = kNatureCompare ) const override;							

virtual String				get_String( 
								tslen 		inLimit = -1 ) const override;

virtual UChar*				get_String( 
								UChar* 		outString, 
								tslen 		inBufferChars ) const override;

virtual char*				get_String( 
								char* 		outString, 
								tslen 		inBufferChars ) const override;

virtual void				put_String( const UChar* inBegin, const UChar* inEnd ) override
							{
								put_IsNull( false );								
								Value_money::put_String( inBegin, inEnd );
							}

virtual void				put_String( const char* inBegin, const char* inEnd = nullptr ) override
							{
								put_IsNull( false );								
								Value_money::put_String( inBegin, inEnd );
							}


	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// Numeric properties:


virtual void				put_Boolean( bool inValue ) override
							{ 
								put_IsNull( false );
								Value_money::put_Boolean( inValue );
							}

virtual void				put_Byte( vuint8 inValue ) override
							{ 
								put_IsNull( false );
								Value_money::put_Byte( inValue );
							}

virtual void				put_Short( vint16 inValue ) override
							{ 
								put_IsNull( false );
								Value_money::put_Short( inValue );
							}

virtual void				put_UShort( vuint16 inValue ) override
							{ 
								put_IsNull( false );
								Value_money::put_UShort( inValue );
							}

virtual void				put_Medium( vint32 inValue ) override
							{ 
								put_IsNull( false );
								Value_money::put_Medium( inValue );
							}

virtual void				put_Long( vint32 inValue ) override
							{ 
								put_IsNull( false );
								Value_money::put_Long( inValue );
							}

virtual void				put_ULong( vuint32 inValue ) override
							{ 
								put_IsNull( false );
								Value_money::put_ULong( inValue );
							}

virtual void				put_LLong( vint64 inValue ) override
							{ 
								put_IsNull( false );
								Value_money::put_LLong( inValue );
							}

virtual void				put_ULLong( vuint64 inValue ) override
							{ 
								put_IsNull( false );
								Value_money::put_ULLong( inValue );
							}

virtual void				put_Float( float inValue ) override
							{ 
								put_IsNull( false );
								Value_money::put_Float( inValue );
							}

virtual void				put_Double( double inValue ) override
							{ 
								put_IsNull( false );
								Value_money::put_Double( inValue );
							}
				
	protected://////////////////////////////////////////////////////////////////////////////////

		bool				mIsNull;		///< nullable flag.
							
};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_Value_Money_h
