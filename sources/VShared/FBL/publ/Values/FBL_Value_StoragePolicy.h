/**********************************************************************************************/
/* FBL_Value_StoragePolicy.h                                               					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_Value_StoragePolicy_h 
	#define _FBL_Value_StoragePolicy_h
#pragma once

#include <VShared/FBL/publ/Values/FBL_type_traits.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


#pragma mark -

 
/**********************************************************************************************/
// Storage policy for numeric types that keep value in place.
// 
template <class traits>
class policyInPlace 
{
	public://///////////////////////////////////////////////////////////////////////////////////
 
 		typedef traits						ps_traits;
		typedef typename traits::value_type	value_type;


	public://///////////////////////////////////////////////////////////////////////////////////
 
							policyInPlace( void ) :
 								mValue(0) 
 							{
 							}
 							
 							policyInPlace( value_type inValue ) :
 								mValue(inValue) 
 							{
 							}
 
 		value_type*			get_ValuePtr( void )  		{ return &mValue; }
 		const value_type*	get_ValuePtr( void ) const 	{ return &mValue; }

		void				put_ValuePtr( value_type* inValuePtr ) { argused1(inValuePtr); }

		void				SwapBytes( void ) {	ps_traits::swap_bytes(mValue); }

		void				SwapBytes( void* inValue ) const 
        						{ ps_traits::swap_bytes(*(value_type*)inValue); }
							
		void				SwapBytes( const void* inSrc, void* inDest ) 
							{
								*(value_type*)inDest = *(value_type*)inSrc;
								ps_traits::swap_bytes(*(value_type*)inDest);
							}

	public://///////////////////////////////////////////////////////////////////////////////////
 
		value_type			mValue;
};


/**********************************************************************************************/
// Storage policy for numeric types that keep only pointer on value, 
// that in fact is located inside of e.g. Record buffer.
// 
template <class traits>
class policyIndirect
{
	public://///////////////////////////////////////////////////////////////////////////////////

		typedef traits						ps_traits;
		typedef typename traits::value_type	value_type;


	public://///////////////////////////////////////////////////////////////////////////////////

 							policyIndirect( void ) :
 								m_pValue(0) 
 							{
 							}
 							
 							policyIndirect( value_type inValue )
 							{
 								argused1( inValue );
 							}

 		value_type*			get_ValuePtr( void ) 		{ return m_pValue; }
		const value_type*	get_ValuePtr( void ) const  { return m_pValue; }

		void				put_ValuePtr( value_type* inValuePtr )  
        						{ m_pValue = inValuePtr; }
                                

	public://///////////////////////////////////////////////////////////////////////////////////
 
		value_type*			m_pValue;		
};


/**********************************************************************************************/
// Provide general algorithms for Nullable fields.
// Can be used only with NUMERIC types, because use sizeof(value_type).
//
template<class policyStorage>
class policyNullable
{
	public://///////////////////////////////////////////////////////////////////////////////////

		typedef typename policyStorage::ps_traits::value_type		value_type;


	public://///////////////////////////////////////////////////////////////////////////////////
 
 							policyNullable( void )  { mIsNull = true; }
 
 							policyNullable( const policyNullable& inOther ) 
							{
								mIsNull = inOther.get_IsNull();
 							}

		bool 				get_IsNull( void ) const  { return mIsNull; }

		void 				put_IsNull( void* inValuePtr, bool inValue ) 
							{ 
								if( (mIsNull = inValue) ) 
								{
									if(inValue) 
										* (value_type*) inValuePtr = 0;
								}
							}	
							

	private:////////////////////////////////////////////////////////////////////////////////////
 	
		bool				mIsNull;		// TRUE if value is NULL. 
};


/**********************************************************************************************/
typedef policyInPlace<bool_traits> 		policyInPlace_bool;
typedef policyInPlace<byte_traits> 		policyInPlace_byte;

typedef policyInPlace<short_traits> 	policyInPlace_short;
typedef policyInPlace<ushort_traits>	policyInPlace_ushort;

typedef policyInPlace<long_traits> 		policyInPlace_long;
typedef policyInPlace<ulong_traits> 	policyInPlace_ulong;

typedef policyInPlace<llong_traits> 	policyInPlace_llong;
typedef policyInPlace<ullong_traits> 	policyInPlace_ullong;

typedef policyInPlace<float_traits> 	policyInPlace_float;
typedef policyInPlace<double_traits> 	policyInPlace_double;


/**********************************************************************************************/
typedef policyIndirect<bool_traits> 	policyIndirect_bool;
typedef policyIndirect<byte_traits> 	policyIndirect_byte;

typedef policyIndirect<short_traits> 	policyIndirect_short;
typedef policyIndirect<ushort_traits>	policyIndirect_ushort;

typedef policyIndirect<long_traits> 	policyIndirect_long;
typedef policyIndirect<ulong_traits> 	policyIndirect_ulong;

typedef policyIndirect<llong_traits> 	policyIndirect_llong;
typedef policyIndirect<ullong_traits> 	policyIndirect_ullong;

typedef policyIndirect<float_traits> 	policyIndirect_float;
typedef policyIndirect<double_traits> 	policyIndirect_double;


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_Value_StoragePolicy_h
