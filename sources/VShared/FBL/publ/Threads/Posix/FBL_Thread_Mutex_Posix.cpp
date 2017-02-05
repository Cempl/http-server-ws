/**********************************************************************************************/
/* FBL_Thread_Mutex_Posix.cpp																  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Headers/FBL.h>
#define _FBL_ThreadSafe_h // trick to enable the following header:
#include "FBL_Thread_Mutex_Posix.h"


/**********************************************************************************************/
#if FBL_POSIX_API


/**********************************************************************************************/
FBL_Begin_Namespace

/**********************************************************************************************/
void Thread_Mutex_Recursive_Posix::lock( void )
{
	// Prevent race conditions
	
	StLockGuard_T<Thread_Mutex_Posix> guard( mLock );

	// Get this thread id
	pthread_t ID = ::pthread_self();

	if( mNestingLevel == 0 )
	{
        mOwnerID = ID;
	}
	else if( ID != mOwnerID )
	{
		// Wait until the nesting level has dropped to zero, at
        // which point we can acquire the lock.
		 while( mNestingLevel > 0 )
		 {
			 mLockAvailable.wait( guard );
		 }

		 // At this point we held the lock
         mOwnerID = ID;
	}

	// Increase the nesting level
    mNestingLevel++;
}


/**********************************************************************************************/
bool Thread_Mutex_Recursive_Posix::try_lock( void )
{
	// Prevent race conditions
	mLock.lock();

	// Get this thread id
	pthread_t ID = ::pthread_self();
	bool Result = true;

	// If there's no contention, just grab the lock immediately.
    if( mNestingLevel == 0 )
    {
        mOwnerID = ID;
        mNestingLevel = 1;
    }
    // If we already own the lock, then increment the nesting level
    // and proceed.
    else if( ID == mOwnerID )
	{
		mNestingLevel++;
	}
	else
	{
		Result = false;
	}

	mLock.unlock();

	return Result;
}


/**********************************************************************************************/
void Thread_Mutex_Recursive_Posix::unlock( void )
{
//	pthread_t ID = ::pthread_self();
	
	// Prevent race conditions
	StLockGuard_T<Thread_Mutex_Posix> guard( mLock );

	if( mNestingLevel )
		mNestingLevel--;
		
    if( mNestingLevel == 0 )
    {
        // This may not be strictly necessary, but it does put
        // the mutex into a known state...
        mOwnerID = 0;

        // Inform a waiter that the lock is free.
		mLockAvailable.notify_one();
    }
}


/**********************************************************************************************/
FBL_End_Namespace

#endif // FBL_POSIX_API
