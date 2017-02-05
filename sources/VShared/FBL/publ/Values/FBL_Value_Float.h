/**********************************************************************************************/
/* FBL_Value_Float.h 	                                                   					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_Value_Float_h
	#define _FBL_Value_Float_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Values/FBL_Value_Approximate_imp.h>
#include <VShared/FBL/publ/Values/FBL_Value_Numeric.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_CLASS(Value_float);
SMART_CLASS(Value_float_null);


/**********************************************************************************************/
// Implements Value_float_imp.
// 
class FBL_SHARED_EXP_CLASS Value_float_imp : 
	public Value_Approximate_Imp,
	public Value_Numeric< kTypeFloat, vcReal, policyInPlace_float >
{
		IMPLEMENT_UNKNOWN(Value_float_imp)											
		BEGIN_INTERFACE_TABLE(Value_float_imp)										
			IMPLEMENT_INTERFACE(I_Value)										
			IMPLEMENT_INTERFACE(I_Serializable)										
			IMPLEMENT_INTERFACE(I_ValueApproximate)										
		END_INTERFACE_TABLE()

	public://///////////////////////////////////////////////////////////////////////////////////

							Value_float_imp( void )  
							:
								Value_Approximate_Imp( gFloatDefaultPrecision, gFloatDefaultScale )
							{
							}

							Value_float_imp( 
								vuint16 inPrecision, 
								vuint16 inScale )  
							:
								Value_Approximate_Imp( inPrecision, inScale )
							{
							}
#if FBL_TEST_CODE							
							Value_float_imp( 
								float inValue,
								vuint16 inPrecision = gFloatDefaultPrecision, 
								vuint16 inScale = gFloatDefaultScale )  
							:
								Value_Approximate_Imp( inPrecision, inScale ),
								Value_Numeric< kTypeFloat, vcReal, policyInPlace_float >( inValue )
							{
							}
#endif // FBL_TEST_CODE
							Value_float_imp( const Value_float_imp& inOther )  
							:
								Value_Approximate_Imp( inOther ),
								Value_Numeric< kTypeFloat, vcReal, policyInPlace_float >( inOther )
							{
							}

	public://///////////////////////////////////////////////////////////////////////////////////

		using Value_Numeric< kTypeFloat, vcReal, policyInPlace_float >::Assign;

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


#pragma mark === Value_float ===


/**********************************************************************************************/
// Implements Value float.
// 
class FBL_SHARED_EXP_CLASS Value_float : 
	public Value_float_imp
{
	public://///////////////////////////////////////////////////////////////////////////////////

							Value_float( void )  
							:
								Value_float_imp( gFloatDefaultPrecision, gFloatDefaultScale )
							{
							}

							Value_float( 
								vuint16 inPrecision, 
								vuint16 inScale )  
							:
								Value_float_imp( inPrecision, inScale )
							{
							}
							
#if FBL_TEST_CODE
							Value_float( 
								float inValue,
								vuint16 inPrecision = gFloatDefaultPrecision, 
								vuint16 inScale = gFloatDefaultScale )  
							:
								Value_float_imp( inValue, inPrecision, inScale )
							{
							}
#endif // FBL_TEST_CODE

							Value_float( const Value_float& inOther )  
							:
								Value_float_imp( inOther )
							{
							}


virtual						~Value_float( void ) {}


	public:///////////////////////////////////////////////////////////////////////////////////

virtual	I_Value_Ptr			Clone( bool inCopyData = true ) const override;

};


#pragma mark === Value_float_null ===


/**********************************************************************************************/
// Implements nullable Value float.
// 
class FBL_SHARED_EXP_CLASS Value_float_null : 
	public Value_float,
	protected policyNullable< policyInPlace_float >
{
	public:///////////////////////////////////////////////////////////////////////////////////

		typedef policyNullable<policyInPlace_float>					PN;

	public:///////////////////////////////////////////////////////////////////////////////////

							Value_float_null( void )  
							:
								Value_float()
							{
							}

							Value_float_null( 								
								vuint16 inPrecision, 
								vuint16 inScale )  
							:
								Value_float( inPrecision, inScale )
							{
							}

#if FBL_TEST_CODE
							Value_float_null( 
								float inValue,
								vuint16 inPrecision = gFloatDefaultPrecision, 
								vuint16 inScale = gFloatDefaultScale )  
							:
								Value_float( inValue, inPrecision, inScale )
							{
								put_IsNull(false);
							}
#endif // FBL_TEST_CODE

							Value_float_null( const Value_float& inOther )  
							:
								Value_float( inOther )
							{
								put_IsNull( inOther.get_IsNull() );
							}

virtual						~Value_float_null( void )
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
								Value_float::put_String( inBegin, inEnd );
							}

virtual void				put_String( const char* inBegin, const char* inEnd = nullptr ) override
							{
								put_IsNull( false );								
								Value_float::put_String( inBegin, inEnd );
							}


	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// Numeric properties:


virtual void				put_Boolean( bool inValue ) override
							{ 
								put_IsNull( false );
								Value_float::put_Boolean( inValue ); 
							}

virtual void				put_Byte( vuint8 inValue ) override
							{ 
								put_IsNull( false );
								Value_float::put_Byte( inValue ); 
							}

virtual void				put_Short( vint16 inValue ) override
							{ 
								put_IsNull( false );
								Value_float::put_Short( inValue ); 
							}

virtual void				put_UShort( vuint16 inValue ) override
							{ 
								put_IsNull( false );
								Value_float::put_UShort( inValue ); 
							}

virtual void				put_Medium( vint32 inValue ) override
							{ 
								put_IsNull( false );
								Value_float::put_Medium( inValue ); 
							}

virtual void				put_Long( vint32 inValue ) override
							{ 
								put_IsNull( false );
								Value_float::put_Long( inValue ); 
							}

virtual void				put_ULong( vuint32 inValue ) override
							{ 
								put_IsNull( false );
								Value_float::put_ULong( inValue ); 
							}

virtual void				put_LLong( vint64 inValue ) override
							{ 
								put_IsNull( false );
								Value_float::put_LLong( inValue ); 
							}

virtual void				put_ULLong( vuint64 inValue ) override
							{ 
								put_IsNull( false );
								Value_float::put_ULLong( inValue ); 
							}

virtual void				put_Float( float inValue ) override
							{ 
								put_IsNull( false );
								Value_float::put_Float( inValue ); 
							}

virtual void				put_Double( double inValue ) override
							{ 
								put_IsNull( false );
								Value_float::put_Double( inValue ); 
							}
};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_Value_Float_h
