/**********************************************************************************************/
/* FBL_Condition_Posix.h	                                                   				  */
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


#ifndef __FBL_Condition_Posix_h
	#define __FBL_Condition_Posix_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL_Macros.h>


// POSIX:
#include <pthread.h>
#include <sys/time.h>

#if FBL_UNIX
#include <errno.h>
#endif // FBL_UNIX

#if STD_THREADING_SAFE
	// STD:
	#include <mutex>
#endif // STD_THREADING_SAFE

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
#if FBL_POSIX_API


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
/**	Condition variable implementation based on POSIX standard.
	Condition variables provide yet another way for threads to 
	synchronize. While mutexes implement synchronization by 
	controlling thread access to data, condition variables allow 
	threads to synchronize based upon the actual value of data. 
	Without condition variables, you would need to have threads 
	continually polling (possibly in a critical section), to check 
	if the condition is met. This can be very resource consuming 
	since the thread would be continuously busy in this activity. 
	A condition variable is a way to achieve the same goal without polling. 
	A condition variable is always used in conjunction with a mutex lock. 
 */
template <class MUTEX>
class Condition_Posix
{
	public://///////////////////////////////////////////////////////////////////////////////////

							/// Initialize the condition variable.
							Condition_Posix();

							/// Implicitly destroy the condition variable.
							~Condition_Posix( void );

	// ----------------
	// Lock accessors
							/**	Block on condition, or until inTimeOutMS has passed.
								Return false in case of timeout reached. */
		bool				wait_for( StLockGuard_T<MUTEX>& inLock, vuint32 inTimeOutMS );

							/// Block on condition.
		void				wait( StLockGuard_T<MUTEX>& inLock );

							/// Signal one waiting thread.
		void				notify_one( void );

							/// Signal *all* waiting threads.
		void				notify_all( void );


	private:////////////////////////////////////////////////////////////////////////////////////
  
							/// Condition variable.
		pthread_cond_t		mCondition;

	private:////////////////////////////////////////////////////////////////////////////////////
  
							/// Prevent assignment and initialization.
		Condition_Posix& operator=( const Condition_Posix& );
		Condition_Posix( const Condition_Posix& );
};



/**********************************************************************************************/
template<class MUTEX>
Condition_Posix<MUTEX>::Condition_Posix()
{
	int res = ::pthread_cond_init(&mCondition, nullptr);
	argused1( res );
}


/**********************************************************************************************/
template<class MUTEX>
Condition_Posix<MUTEX>::~Condition_Posix( void )
{
	try
	{
		int res = ::pthread_cond_destroy(&mCondition);
		argused1( res );
	}
	catch(...)
	{
		// something bad happened
		//FBL_CHECK(0);
	}
}


/**********************************************************************************************/
template<class MUTEX>
bool Condition_Posix<MUTEX>::wait_for( StLockGuard_T<MUTEX>& inLock, vuint32 inTimeOutMS )
{
	struct timeval now;
	struct timespec timeout;
	gettimeofday(&now, 0);
	timeout.tv_sec = now.tv_sec + (long)(inTimeOutMS/1000);				// sec
	timeout.tv_nsec = (now.tv_usec + (vint32)(inTimeOutMS%1000))*1000;  // nsec
	int res = ::pthread_cond_timedwait(&mCondition, (pthread_mutex_t*) inLock.get_Lock()->get(), &timeout);
	return (res == ETIMEDOUT) ? false : true;
}


/**********************************************************************************************/
template<class MUTEX>
void Condition_Posix<MUTEX>::wait( StLockGuard_T<MUTEX>& inLock )
{
	int res = ::pthread_cond_wait( &mCondition, (pthread_mutex_t*) inLock.get_Lock()->get() );
	argused1( res );
}


/**********************************************************************************************/
template<class MUTEX>
void Condition_Posix<MUTEX>::notify_one( void )
{
	int res = ::pthread_cond_signal(&mCondition);
	argused1( res );
}


/**********************************************************************************************/
template<class MUTEX>
void Condition_Posix<MUTEX>::notify_all( void )
{
	int res = ::pthread_cond_broadcast(&mCondition);
	argused1( res );
}


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // __FBL_Condition_Posix_h

#endif // FBL_POSIX_API
