/**********************************************************************************************/
/* FBL_Value_Double.h 	                                                   					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_Value_Double_h
	#define _FBL_Value_Double_h
#pragma once


// FBL:
#include <VShared/FBL/publ/Values/FBL_Value_Approximate_imp.h>
#include <VShared/FBL/publ/Values/FBL_Value_Numeric.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_CLASS(Value_double);
SMART_CLASS(Value_double_null);


/**********************************************************************************************/
// Implements Value double.
// 
class FBL_SHARED_EXP_CLASS Value_double_imp : 
	public Value_Approximate_Imp,
	public Value_Numeric< kTypeDouble, vcReal, policyInPlace_double >
{
		IMPLEMENT_UNKNOWN(Value_double_imp)											
		BEGIN_INTERFACE_TABLE(Value_double_imp)										
			IMPLEMENT_INTERFACE(I_Value)										
			IMPLEMENT_INTERFACE(I_Serializable)										
			IMPLEMENT_INTERFACE(I_ValueApproximate)										
		END_INTERFACE_TABLE()

	public://///////////////////////////////////////////////////////////////////////////////////

							Value_double_imp( void )  
							:
								Value_Approximate_Imp( gDoubleDefaultPrecision, gDoubleDefaultScale )
							{
							}

							Value_double_imp( 
								vuint16 inPrecision, 
								vuint16 inScale )  
							:
								Value_Approximate_Imp( inPrecision, inScale )
							{
							}
							
#if FBL_TEST_CODE							
							Value_double_imp( 
								double inValue,
								vuint16 inPrecision = gDoubleDefaultPrecision, 
								vuint16 inScale = gDoubleDefaultScale )  
							:
								Value_Approximate_Imp( inPrecision, inScale ),
								Value_Numeric< kTypeDouble, vcReal, policyInPlace_double >( inValue )
							{
							}
#endif // FBL_TEST_CODE

							Value_double_imp( const Value_double_imp& inOther )  
							:
								Value_Approximate_Imp( inOther ),
								Value_Numeric< kTypeDouble, vcReal, policyInPlace_double >( inOther )
							{
							}

	public://///////////////////////////////////////////////////////////////////////////////////

		using Value_Numeric< kTypeDouble, vcReal, policyInPlace_double >::Assign;

// I_Value API:

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

};


#pragma mark === Value_double ===


/**********************************************************************************************/
// Implements Value double.
// 
class FBL_SHARED_EXP_CLASS Value_double : 
	public Value_double_imp
{
	public://///////////////////////////////////////////////////////////////////////////////////

							Value_double( void )  
							:
								Value_double_imp( gDoubleDefaultPrecision, gDoubleDefaultScale )
							{
							}

							Value_double( 
								vuint16 inPrecision, 
								vuint16 inScale )  
							:
								Value_double_imp( inPrecision, inScale )
							{
							}
							
#if FBL_TEST_CODE							
							Value_double( 
								double inValue,
								vuint16 inPrecision = gDoubleDefaultPrecision, 
								vuint16 inScale = gDoubleDefaultScale )  
							:
								Value_double_imp( inValue, inPrecision, inScale )
							{
							}
#endif // FBL_TEST_CODE

							Value_double( const Value_double& inOther )  
							:
								Value_double_imp( inOther )
							{
							}


virtual						~Value_double( void )
								{}

	public:///////////////////////////////////////////////////////////////////////////////////

virtual	I_Value_Ptr			Clone( bool inCopyData = true ) const override;

};


#pragma mark === Value_double_null ===


/**********************************************************************************************/
// Implements nullable Value double.
// 
class FBL_SHARED_EXP_CLASS Value_double_null : 
	public Value_double,
	protected policyNullable< policyInPlace_double >
{
	public:///////////////////////////////////////////////////////////////////////////////////

		typedef policyNullable<policyInPlace_double>					PN;

	public:///////////////////////////////////////////////////////////////////////////////////

							Value_double_null( void )  
							:
								Value_double()
							{
							}

							Value_double_null( 								
								vuint16 inPrecision, 
								vuint16 inScale )  
							:
								Value_double( inPrecision, inScale )
							{
							}
							
#if FBL_TEST_CODE							
							Value_double_null( 
								double inValue,
								vuint16 inPrecision = gDoubleDefaultPrecision, 
								vuint16 inScale = gDoubleDefaultScale )  
							:
								Value_double( inValue, inPrecision, inScale )
							{
								put_IsNull(false);
							}
#endif // FBL_TEST_CODE

							Value_double_null( const Value_double& inOther )  
							:
								Value_double( inOther )
							{
								put_IsNull( inOther.get_IsNull() );
							}

virtual						~Value_double_null( void )
								{}

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
								Value_double::put_String( inBegin, inEnd );
							}

virtual void				put_String( const char* inBegin, const char* inEnd = nullptr ) override
							{
								put_IsNull( false );								
								Value_double::put_String( inBegin, inEnd );
							}


	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// Numeric properties:


virtual void				put_Boolean( bool inValue ) override
							{ 
								put_IsNull( false );
								Value_double::put_Boolean( inValue ); 
							}

virtual void				put_Byte( vuint8 inValue ) override
							{ 
								put_IsNull( false );
								Value_double::put_Byte( inValue ); 
							}

virtual void				put_Short( vint16 inValue ) override
							{ 
								put_IsNull( false );
								Value_double::put_Short( inValue ); 
							}

virtual void				put_UShort( vuint16 inValue ) override
							{ 
								put_IsNull( false );
								Value_double::put_UShort( inValue ); 
							}

virtual void				put_Medium( vint32 inValue ) override
							{ 
								put_IsNull( false );
								Value_double::put_Medium( inValue ); 
							}

virtual void				put_Long( vint32 inValue ) override
							{ 
								put_IsNull( false );
								Value_double::put_Long( inValue ); 
							}

virtual void				put_ULong( vuint32 inValue ) override
							{ 
								put_IsNull( false );
								Value_double::put_ULong( inValue ); 
							}

virtual void				put_LLong( vint64 inValue ) override
							{ 
								put_IsNull( false );
								Value_double::put_LLong( inValue ); 
							}

virtual void				put_ULLong( vuint64 inValue ) override
							{ 
								put_IsNull( false );
								Value_double::put_ULLong( inValue ); 
							}

virtual void				put_Float( float inValue ) override
							{ 
								put_IsNull( false );
								Value_double::put_Float( inValue ); 
							}

virtual void				put_Double( double inValue ) override
							{ 
								put_IsNull( false );
								Value_double::put_Double( inValue ); 
							}
};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_Value_Double_h
