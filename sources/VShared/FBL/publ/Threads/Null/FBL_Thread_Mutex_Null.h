/**********************************************************************************************/
/* FBL_Mutex_Null.h	                                                   						  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/
/**
 *	IMPORTANT NOTE: DO NOT include this file directly
 *		unless you know what you are doing!
 *	Include <VShared/FBL/publ/Threads/FBL_ThreadSafe.h> instead.
 */
	// and strong rule of this:
	#ifndef _FBL_ThreadSafe_h
		#error "Please include FBL_ThreadSafe.h instead of this header."
	#endif // _FBL_ThreadSafe_h


#ifndef _FBL_Mutex_Null_h
	#define _FBL_Mutex_Null_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL_Macros.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
/// No-op mutex implementation
/// (can be used in generic programming)
///
class FBL_SHARED_EXP_CLASS Mutex_Null
{
	public://///////////////////////////////////////////////////////////////////////////////////

							/// Constructor, which intializes the mutex.
							Mutex_Null( void )
							{}

							/// Destructor destroys synch object.
							~Mutex_Null( void )
							{}

							/** Block the thread until the lock is acquired.  
								Throws an exception on failure. */
		void				lock( void )
							{}

							/** Conditionally acquire the lock (i.e., won't block).  
								Returns false if someone else already had the lock,
								and true if succesfull. */
		bool 				try_lock( void )
							{ return true; }

							/// Release the lock.  
		void				unlock( void )
							{}

//							/** Block until the thread acquires a read lock.  
//								If the locking mechanism doesn't support 
//								read locks then this just calls <Acquire>. */
//		void				AcquireRead( void )
//							{}
//
//							/** Block until the thread acquires a write lock.  
//								If the locking mechanism doesn't support 
//								read locks then this just calls <Acquire>. */
//		void				AcquireWrite( void )
//							{}
//	
//							/** Conditionally acquire a read lock. If the locking mechanism
//							 	doesn't support read locks then this just calls @a <Acquire>.
//							 	Returns false if someone else already had the lock,
//							 	and true if succesfull. */
//		bool				TryAcquireRead( void )
//							{ return true; }
//
//							/** Conditionally acquire a write lock. If the locking mechanism
//								doesn't support write locks then this just calls <Acquire>.
//								Returns false if someone else already had the lock,
//								and non-zero if succesfull. */
//		bool				TryAcquireWrite( void )
//							{ return true; }
};


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // _FBL_Mutex_Null_h
