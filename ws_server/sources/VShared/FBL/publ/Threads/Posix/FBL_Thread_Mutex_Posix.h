/**********************************************************************************************/
/* FBL_Thread_Mutex_Posix.h	                                                   				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
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


#ifndef __FBL_Thread_Mutex_Posix_h
	#define __FBL_Thread_Mutex_Posix_h
#pragma once

// POSIX:
#include <pthread.h>

// FBL:
#include <VShared/FBL/publ/Threads/Posix/FBL_Condition_Posix.h>
#include <VShared/FBL/publ/Threads/thread_safety.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
#if FBL_POSIX_API


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
/// Non-recursive thread-level mutex implementation for POSIX threads.
//
class FBL_SHARED_EXP_CLASS CAPABILITY( "mutex" ) Thread_Mutex_Posix
{
	public://///////////////////////////////////////////////////////////////////////////////////

							Thread_Mutex_Posix( void );
							~Thread_Mutex_Posix( void );

 							/**	Block the thread until the lock is acquired.  
								Throws an exception on failure. */
		void				lock( void ) ACQUIRE();

							/**	Conditionally acquire the lock (i.e., won't block).  
								Returns 0 if someone else already had the lock,
								and non-zero if succesfull.*/
		bool 				try_lock( void );

							///	Release the lock.  
		void				unlock( void ) RELEASE_();

//							/**	Block until the thread acquires a read lock.  
//								If the locking mechanism doesn't support 
//								read locks then this just calls \a Acquire. */
//		void				AcquireRead( void );
//
//							/**	Block until the thread acquires a write lock.  
//								If the locking mechanism doesn't support 
//								read locks then this just calls \a Acquire. */
//		void				AcquireWrite( void );
//
//							/**	Conditionally acquire a read lock. If the locking mechanism
//								doesn't support read locks then this just calls \a Acquire.
//								Returns true if succesfull and false otherwise */
//		bool				TryAcquireRead( void );
//
//							/**	Conditionally acquire a write lock. If the locking mechanism
//								doesn't support write locks then this just calls \a Acquire.
//								Returns true if succesfull and false otherwise. */
//		bool				TryAcquireWrite( void );
//
		const pthread_mutex_t*	get( void ) const throw() { return &mMutex; }	
	
	private:////////////////////////////////////////////////////////////////////////////////////

		pthread_mutex_t		mMutex; /// Thread mutex descriptor.
};


/**********************************************************************************************/
inline 
Thread_Mutex_Posix::Thread_Mutex_Posix( void )
{
	int res = ::pthread_mutex_init(&mMutex, nullptr);
	argused1( res );
}


/**********************************************************************************************/
inline 
Thread_Mutex_Posix::~Thread_Mutex_Posix( void )
{
	int res = ::pthread_mutex_destroy(&mMutex);
	argused1( res );
}


/**********************************************************************************************/
inline 
void Thread_Mutex_Posix::lock( void ) ACQUIRE()
{
	int res = ::pthread_mutex_lock(&mMutex);
	argused1( res );
}


/**********************************************************************************************/
inline 
bool Thread_Mutex_Posix::try_lock( void )
{
	int res = ::pthread_mutex_trylock(&mMutex);

	return res == 1 ? true : false;
}


/**********************************************************************************************/
inline 
void Thread_Mutex_Posix::unlock( void ) RELEASE_()
{
	int res = ::pthread_mutex_unlock(&mMutex);
	argused1( res );
}


#pragma mark -
 

///**********************************************************************************************/
//inline 
//void Thread_Mutex_Posix::AcquireRead( void )
//{
//	Acquire();
//}
//
//
///**********************************************************************************************/
//inline 
//void Thread_Mutex_Posix::AcquireWrite( void )
//{
//	Acquire();
//}
//
//
///**********************************************************************************************/
//inline 
//bool Thread_Mutex_Posix::TryAcquireRead( void )
//{
//	return TryAcquire();
//}
//
//
///**********************************************************************************************/
//inline 
//bool Thread_Mutex_Posix::TryAcquireWrite( void )
//{
//	return TryAcquire();
//}


#pragma mark -
 

/**********************************************************************************************/
/// Recursive thread-level mutex implementation for POSIX threads.
/// Since there is no built-in supprot for this in PTHREAD API we need to emulate it.
/// 
class FBL_SHARED_EXP_CLASS CAPABILITY( "mutex" ) Thread_Mutex_Recursive_Posix
{
	public://///////////////////////////////////////////////////////////////////////////////////

							/// Constructor, which intializes the mutex.
							Thread_Mutex_Recursive_Posix( void );


							/// Destructor destroys synch object.
							~Thread_Mutex_Recursive_Posix( void );


							/**	Block the thread until the lock is acquired.  
								Throws an exception on failure. */
		void				lock( void ) ACQUIRE();


							/**	Conditionally acquire the lock (i.e., won't block).  
								Returns 0 if someone else already had the lock,
								and non-zero if succesfull.*/
		bool 				try_lock( void );


							///	Release the lock.  
		void				unlock( void ) RELEASE_();


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
//								Returns true if succesfull and false otherwise */
//		bool				TryAcquireRead( void );
//
//
//							/**	Conditionally acquire a write lock. If the locking mechanism
//								doesn't support write locks then this just calls \a Acquire.
//								Returns true if succesfull and false otherwise. */
//		bool				TryAcquireWrite( void );


							/// Return the id of the thread that currently owns the mutex.
		vuint32				native_handle( void );


							/** Return the nesting level of the recursion.  When a thread has
								acquired the mutex for the first time, the nesting level == 1.
								The nesting level is incremented every time the thread acquires
								the mutex recursively. */
		vuint32				get_NestingLevel( void )
							{
								return mNestingLevel;
							}



	private:////////////////////////////////////////////////////////////////////////////////////

							/// Guards access to the mNestingLevel and mThreadID variables
		Thread_Mutex_Posix	mLock;

							/// This condition suspends other waiting threads until the
							/// mutex is available.
		Condition_Posix<Thread_Mutex_Posix>	mLockAvailable;

							/// Recusrion level
		vuint32				mNestingLevel;

							/// Owner thread's id.
		pthread_t			mOwnerID;
};


/**********************************************************************************************/
inline
Thread_Mutex_Recursive_Posix::Thread_Mutex_Recursive_Posix( void )
:
	mNestingLevel(0),
	mOwnerID(0)
{
}


/**********************************************************************************************/
inline
Thread_Mutex_Recursive_Posix::~Thread_Mutex_Recursive_Posix( void )
{}


///**********************************************************************************************/
//inline
//void Thread_Mutex_Recursive_Posix::AcquireRead( void )
//{
//	Acquire();
//}
//
//
///**********************************************************************************************/
//inline
//void Thread_Mutex_Recursive_Posix::AcquireWrite( void )
//{
//	Acquire();
//}
//
//
///**********************************************************************************************/
//inline
//bool Thread_Mutex_Recursive_Posix::TryAcquireRead( void )
//{
//	return TryAcquire();
//}
//
//
///**********************************************************************************************/
//inline
//bool Thread_Mutex_Recursive_Posix::TryAcquireWrite( void )
//{
//	return TryAcquire();
//}
//
//
/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // __FBL_Thread_Mutex_Posix_h

#endif // FBL_POSIX_API
