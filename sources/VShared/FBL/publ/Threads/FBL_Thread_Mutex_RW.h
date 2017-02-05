/**********************************************************************************************/
/* FBL_Thread_Mutex_RW.h	                                           						  */
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


#ifndef _FBL_Thread_Mutex_RW_h
	#define _FBL_Thread_Mutex_RW_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
/// Read-write thread mutex implementation.
//
//class FBL_SHARED_EXP_CLASS Thread_Mutex_RW
//{
//	public://///////////////////////////////////////////////////////////////////////////////////
//
//							Thread_Mutex_RW( void );
//
//							/// Implicitly destroy a readers/writer lock
//							~Thread_Mutex_RW (void);
//
//							/**	Explicitly destroy a readers/writer lock. Note that only one
//								thread should call this method since it doesn't protect against
//								race conditions. */
//		void				Remove( void );
//
//							/// Acquire a read lock, but block if a writer hold the lock.
//		void				AcquireRead( void );
//
//							/**	Acquire a write lock, but block if any readers or a
//								writer hold the lock. */
//		void				AcquireWrite( void );
//
//							/**	Conditionally acquire a read lock (i.e., won't block). 
//								Returns boolean indicating whether the lock has been 
//								acquired or not. */
//		bool				TryAcquireRead( void );
//
//							/// Conditionally acquire a write lock (i.e., won't block).
//		bool				TryAcquireWrite (void);
//
//							/**	Conditionally upgrade a read lock to a write lock.  This only
//								works if there are no other readers present, in which case the
//								method returns true. Otherwise, the method returns false.  
//								Note that the caller of this method *must* already possess this 
//								lock as a read lock (but this condition is not checked by the 
//								current implementation). */
//		bool				TryAcquireWriteUpgrade( void );
//        
//							/**	Note, for interface uniformity with other synchronization
//								wrappers we include the <Acquire> method.  This is implemented 
//								as a write-lock to safe... */
//		void				Acquire( void );
//
//							/**	Note, for interface uniformity with other synchronization
//								wrappers we include the <TryAcquire> method. This is implemented
//								as a write-lock to be safe...  Returns boolean indicating whether 
//								the lock has been acquired or not. */
//		bool				TryAcquire( void );
//
//							/// Unlock a readers/writer lock.
//		void				Release( void );
//
//
//	private:////////////////////////////////////////////////////////////////////////////////////
//
//
//							/// Reader threads waiting to acquire the lock.
//		ConditionVar		mWaitingReaders;
//		
//							/// Number of waiting readers.
//		int					mNumWaitingReaders;
//
//							/// Writer threads waiting to acquire the lock.
//		ConditionVar		mWaitingWriters;
//
//							/// Number of waiting writers.
//		int					mNumWaitingWriters;
//		
//							/**	Value is -1 if writer has the lock, else this keeps track of the
//								number of readers holding the lock. */
//		vint32				mRefCount;
//
//							/// Condition for the upgrading reader.
//		ConditionVar		mWaitingImportantWriter;
//
//							/// Serialize access to internal state.
//		Thread_Mutex		mLock;
//
//							/// Indicate that a reader is trying to upgrade.
//		bool				mImportantWriter;
//
//							/**	Keeps track of whether <Remove> has been called yet to avoid
//								multiple <Remove> calls, e.g., explicitly and implicitly in the
//								destructor.  This flag isn't protected by a lock, so make sure
//								that you don't have multiple threads simultaneously calling
//								<Remove> on the same object, which is a bad idea anyway... */
//		bool				mRemoved;
//
//		vuint8				mPadWarnKiller[1];
//		
//
//	private:////////////////////////////////////////////////////////////////////////////////////
//
//							/// Prevent assignment and initialization.
//		Thread_Mutex_RW&	operator= ( const Thread_Mutex_RW & );
//							Thread_Mutex_RW( const Thread_Mutex_RW & );
//
//
//	public://///////////////////////////////////////////////////////////////////////////////////
//
//
//#if FBL_TEST_CODE
//
//		vint32				get_RefCount( void ) 
//							{
//								mLock.lock();
//								vint32 RefCount = mRefCount;
//								mLock.unlock();
//								return RefCount;
//							}
//#endif // FBL_TEST_CODE
//
//
//};


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // _FBL_Thread_Mutex_RW_h
