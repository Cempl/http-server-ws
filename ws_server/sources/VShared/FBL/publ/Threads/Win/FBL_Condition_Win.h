/**********************************************************************************************/
/* FBL_Condition_Win.h	                                                   					  */
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


#ifndef _FBL_Condition_Win_h
	#define _FBL_Condition_Win_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL_Macros.h>

#if STD_THREADING_SAFE
// STD:
#include <mutex>
#endif // STD_THREADING_SAFE

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
#if FBL_WIN == 0
#error This file can only be used for Win builds!
#endif // FBL_WIN == 0


/**
 *	IMPORTANT NOTE: DO NOT include this file directly
 *		unless you know what you are doing!
 *	Include <VShared/FBL/publ/Threads/FBL_Synch.h> instead.
 */

/**********************************************************************************************/
FBL_Begin_Namespace

/**********************************************************************************************/
/**	Condition variable implementation for Windows.
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
template<class MUTEX>
class Condition_Win
{
	public://///////////////////////////////////////////////////////////////////////////////////

							/// Initialize the condition variable.
							Condition_Win();

							/// Implicitly destroy the condition variable.
							~Condition_Win( void );

	// ----------------
	// Lock accessors
							/**	Block on condition, or until inTimeOutMS has passed.
								Return false in case of timeout reached.
							 */
		bool				wait_for( StLockGuard<MUTEX>& inLock, vuint32 inTimeOutMS );

							/// Block on condition.
		void				wait( StLockGuard<MUTEX>& inLock );

							// Signal one waiting thread.
		void				notify_one( void );

							// Signal *all* waiting threads.
		void				notify_all( void );


	private:////////////////////////////////////////////////////////////////////////////////////
  
							
							/// Number of waiting threads.
		vint32				mWaitersCount;

							/// Serialize access to the waiters count.
		CRITICAL_SECTION	mWaitersLock;

							/// Queue up threads waiting for the 
							/// condition to become signaled.
		HANDLE				mSemaphore;

							/**	An auto reset event used by the Broadcast/Signal 
								thread to wait for the waiting thread(s) to wake 
								up and get a chance at the semaphore. */
		HANDLE				mWaitersDoneEvent;

							///	Keeps track of whether we were broadcasting or just signaling.
		bool				mWasBroadcast;


	private:////////////////////////////////////////////////////////////////////////////////////
  
							/// Prevent assignment and initialization.
		Condition_Win<MUTEX>& operator=( const Condition_Win<MUTEX>& );
		Condition_Win( const Condition_Win<MUTEX> & );
};


/**********************************************************************************************/
template<class MUTEX> inline
Condition_Win<MUTEX>::Condition_Win()
:
	mWaitersCount(0),
	mWasBroadcast(false)
{
	// Initialize waiters lock
	::InitializeCriticalSection(&mWaitersLock);

	// Create the semafore which will be used 
	// as the underlying synchronization mechanism
	mSemaphore = ::CreateSemaphoreW(NULL, 0, LONG_MAX, 0x00);
	if( !mSemaphore )
	{
		throw xExternalError(::GetLastError());
	}

	mWaitersDoneEvent = ::CreateEventW(NULL, 0, 0, 0x00);
	if( !mWaitersDoneEvent )
	{
		throw xExternalError(::GetLastError());
	}
}


/**********************************************************************************************/
template<class MUTEX> inline
Condition_Win<MUTEX>::~Condition_Win( void )
{
	::CloseHandle(mWaitersDoneEvent);
	::CloseHandle(mSemaphore);
	::DeleteCriticalSection(&mWaitersLock);
}


/**********************************************************************************************/
template<class MUTEX> inline
bool Condition_Win<MUTEX>::wait_for( StLockGuard<MUTEX>& inLock, vuint32 inTimeOutMS )
{
	// Atomically increment the waiters count
	::EnterCriticalSection(&mWaitersLock);
	mWaitersCount++;
	::LeaveCriticalSection(&mWaitersLock);

	// Release the external lock
	inLock.get_Lock()->unlock();
	DWORD result = 0;

	try
	{
		// Wait to be awakened by a Signal() or Broadcast().
		result = ::WaitForSingleObject(mSemaphore, inTimeOutMS);

		// Reacquire lock to avoid race conditions.
		::EnterCriticalSection(&mWaitersLock);
		mWaitersCount--;
		bool LastWaiter = mWasBroadcast && mWaitersCount == 0;
		::LeaveCriticalSection(&mWaitersLock);

		if (result == WAIT_ABANDONED || result == WAIT_FAILED)
		{
			throw xExternalError(::GetLastError());
		}

		if (LastWaiter)
		{
			// Release the signaler/broadcaster if we're the last waiter.
			if (!::SetEvent(mWaitersDoneEvent))
			{
				throw xExternalError(::GetLastError());
			}
		}
	}
	catch (...)
	{
		result = WAIT_TIMEOUT;
	}

	// We must always regain the external lock, even when errors
	// occur because that's the guarantee that we give to our callers.
	inLock.get_Lock()->lock();
	
	return (result == WAIT_TIMEOUT) ? false : true;
}


/**********************************************************************************************/
template<class MUTEX> inline
void Condition_Win<MUTEX>::wait( StLockGuard<MUTEX>& inLock )
{
	wait_for(inLock, INFINITE);
}


/**********************************************************************************************/
template<class MUTEX> inline
void Condition_Win<MUTEX>::notify_one( void )
{
	// Do we have a waiter?
	::EnterCriticalSection(&mWaitersLock);
	bool HaveWaiters = mWaitersCount > 0;
	::LeaveCriticalSection(&mWaitersLock);

	if( HaveWaiters )
	{
		// Wake up a waiter
		if( !::ReleaseSemaphore(mSemaphore, 1, 0) )
		{
			throw xExternalError(::GetLastError());
		}
	}
	else
	{
	   ; /* No-op */
	}
}


/**********************************************************************************************/
template<class MUTEX> inline
void Condition_Win<MUTEX>::notify_all( void )
{
	// The <external_mutex> must be locked before this call is made.

	// This is needed to ensure that <waiters_> and <was_broadcast_> are
	// consistent relative to each other.
	::EnterCriticalSection(&mWaitersLock);
	bool HaveWaiters = false;
	if( mWaitersCount > 0 )
	{
		// We are broadcasting, even if there is just one waiter...
		// Be sure to set this with the <mWaitersLock> held.
		mWasBroadcast = true;
		HaveWaiters = true;
	}
	::LeaveCriticalSection(&mWaitersLock);
	
	if( HaveWaiters )
	{
		// Wake up all the waiters.
		if( !::ReleaseSemaphore(mSemaphore, mWaitersCount, 0) )
		{
			throw xExternalError(::GetLastError());
		}

		// Wait now for the waiters.
		if( ::WaitForSingleObject(mWaitersDoneEvent, INFINITE) != WAIT_OBJECT_0 )
		{
			throw xExternalError(::GetLastError());
		}

		// This is okay, even without the <mWaitersLock> held because
		// no other waiter threads can wake up to access it.
		mWasBroadcast = false;
	}
}


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // _FBL_Condition_Win_h
