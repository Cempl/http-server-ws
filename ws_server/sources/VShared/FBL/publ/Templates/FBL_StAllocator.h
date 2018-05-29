/**********************************************************************************************/
/* FBL_StAllocator.h																		  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_StAllocator_h 
	#define _FBL_StAllocator_h
#pragma once

#define FBL_USE_MEMPOOL 0

#if FBL_USE_MEMPOOL
#include <boost/memory.hpp>
#endif // FBL_USE_MEMPOOL

#include <VShared/FBL/publ/Headers/FBL_Macros.h>
#include <VShared/FBL/publ/Headers/FBL_Debug_Macros.h>

// FINAL:	
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
#if FBL_USE_MEMPOOL


/**********************************************************************************************/
extern FBL_SHARED_EXP NS_BOOST_MEMORY::block_pool		gPoolRecycle;
extern FBL_SHARED_EXP NS_BOOST_MEMORY::scoped_alloc*	pAllocator;

/**********************************************************************************************/
class StAllocator
{
	public://///////////////////////////////////////////////////////////////////////////////////
		
							StAllocator( void ) : mPool( gPoolRecycle )
								{ Enable(); }
							
							~StAllocator( void )
								{ Disable(); }
		
inline	void				Enable( void )
							{
								FBL_CHECK( pAllocator == NULL );
								pAllocator = &mPool; 
							}
		
inline	void				Disable( void )
							{
								FBL_CHECK( pAllocator != NULL );
								pAllocator = NULL; 
							}
		
	protected:////////////////////////////////////////////////////////////////////////////////// 
		
		NS_BOOST_MEMORY::scoped_alloc	mPool;		
};


/**********************************************************************************************/
class StEnable
{
	public://///////////////////////////////////////////////////////////////////////////////////
		
							StEnable( StAllocator& inAllocator )
							:
								mStAllocator( inAllocator )
							{
								mStAllocator.Enable();
							}
							
							~StEnable( void )
							{
								mStAllocator.Disable();
							}
		
	protected:////////////////////////////////////////////////////////////////////////////////// 
		
		StAllocator&		mStAllocator;
};


/**********************************************************************************************/
#define NEW_FOR_THIS_CLASS_AND_CHILDS					\
void* operator new( size_t inSize )						\
{														\
	void* res;											\
														\
	res = pAllocator ? pAllocator->allocate(inSize)		\
	: malloc(inSize);									\
														\
	return res;											\
}


/**********************************************************************************************/
#define DO(x) x


#else // FBL_USE_MEMPOOL we define everything to be EMPTY and produce ZERO byte of code.


/**********************************************************************************************/
class StAllocator
{
	public://///////////////////////////////////////////////////////////////////////////////////
		
							StAllocator( void )	{}		
							~StAllocator( void ){}
		
inline	void				Enable( void )		{}
inline	void				Disable( void )		{}
};


/**********************************************************************************************/
class StEnable
{
	public://///////////////////////////////////////////////////////////////////////////////////
		
							StEnable( StAllocator& inAllocator ){ argused1(inAllocator); }
							~StEnable( void ){}
};


/**********************************************************************************************/
#define NEW_FOR_THIS_CLASS_AND_CHILDS 
#define DO(x)

#endif // FBL_USE_MEMPOOL


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_StAllocator_h

