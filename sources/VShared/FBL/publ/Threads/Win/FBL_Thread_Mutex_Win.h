/**********************************************************************************************/
/* FBL_Thread_Mutex.h	                                                   					  */
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


#ifndef _FBL_Thread_Mutex_Win_h
	#define _FBL_Thread_Mutex_Win_h
#pragma once


// FBL:
#include <VShared/FBL/publ/Headers/FBL_Macros.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
#if FBL_WIN == 0
#error This file can only be used for Windows builds!
#endif // FBL_WIN


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
/// Thread-level mutex implementation for Windows.
///
class FBL_SHARED_EXP_CLASS Thread_Mutex_Win
{
	public://///////////////////////////////////////////////////////////////////////////////////

							/// Constructor, which intializes the mutex.
							Thread_Mutex_Win( void );


							/// Destructor destroys synch object.
							~Thread_Mutex_Win( void );


							/// Block the thread until the lock is acquired.  
							/// Throws an exception on failure.
		void				lock( void );


							/**	Conditionally acquire the lock (i.e., won't block).  
								Returns 0 if someone else already had the lock,
								and non-zero if succesfull. */
		bool 				try_lock( void );


							/// Release the lock.
		void				unlock( void );


//							/**	Block until the thread acquires a read lock.  
//								If the locking mechanism doesn't support 
//								read locks then this just calls \a Acquire. */
//		void				AcquireRead( void );
//
//
//							/**	Block until the thread acquires a write lock.  
//								If the locking mechanism doesn't support 
//								read locks then this just calls \a Acquire. */
//		void				AcquireWrite( void );
//	
//
//							/**	Conditionally acquire a read lock. If the locking mechanism
//								doesn't support read locks then this just calls \a Acquire.
//								Returns 0 if someone else already had the lock,
//								and non-zero if succesfull. */
//		bool				TryAcquireRead( void );
//
//
//							/**	Conditionally acquire a write lock. If the locking mechanism
//								doesn't support write locks then this just calls \a Acquire.
//								Returns false if someone else already had the lock,
//								and non-zero if succesfull. */
//		bool				TryAcquireWrite( void );


							/** Return the nesting level of the recursion.  When a thread has
								acquired the mutex for the first time, the nesting level == 1.
								The nesting level is incremented every time the thread acquires
								the mutex recursively. */
		vuint32				get_NestingLevel( void )
							{
								return mNestingLevel;
							}


	private:////////////////////////////////////////////////////////////////////////////////////

		
							/// On Windows we use CRITICAL_SECTION for thread-level mutex.
		CRITICAL_SECTION	mCS;
	
							/// Recusrion level
		vuint32				mNestingLevel;
};


/**********************************************************************************************/
inline 
Thread_Mutex_Win::Thread_Mutex_Win( void )
:
	mNestingLevel(0)
{
	::InitializeCriticalSection(&mCS);
}


/**********************************************************************************************/
inline 
Thread_Mutex_Win::~Thread_Mutex_Win( void )
{
	::DeleteCriticalSection(&mCS);
}


/**********************************************************************************************/
inline 
void Thread_Mutex_Win::lock( void )
{
	::EnterCriticalSection(&mCS);

	// Increase the nesting level
    mNestingLevel++;
}


/**********************************************************************************************/
inline 
bool Thread_Mutex_Win::try_lock( void )
{
#if _WIN32_WINNT >= 0x4000
	bool res = ::TryEnterCriticalSection(&mCS) == TRUE;
	if( res )
		mNestingLevel++;
	return res;
#else
	lock();
	return true;
#endif // _WIN32_WINNT >= 0x4000
}


/**********************************************************************************************/
inline 
void Thread_Mutex_Win::unlock( void )
{
	mNestingLevel--;
	::LeaveCriticalSection(&mCS);
}


///**********************************************************************************************/
//inline 
//void Thread_Mutex_Win::AcquireRead( void )
//{
//	Acquire();
//}
//
//
///**********************************************************************************************/
//inline 
//void Thread_Mutex_Win::AcquireWrite( void )
//{
//	Acquire();
//}
//
//
///**********************************************************************************************/
//inline 
//bool Thread_Mutex_Win::TryAcquireRead( void )
//{
//	return TryAcquire();
//}
//
//
///**********************************************************************************************/
//inline 
//bool Thread_Mutex_Win::TryAcquireWrite( void )
//{
//	return TryAcquire();
//}
//
/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // _FBL_Thread_Mutex_Win_h