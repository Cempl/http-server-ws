/**********************************************************************************************/
/* FBL_ResourceMonitor.cpp																	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                       					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Threads/FBL_ResourceMonitor.h>

/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
void ResourceMonitor::Register( void )
{
	StLockGuard<True_Thread_Mutex> g(mMutex);
	++mCount;
	FBL_CHECK( mCount >= 0 );
	
	// First waitable resource switch semaphore to the blocked state.
	if( mCount == 1 )
		mSemaphore.Wait();
}


/**********************************************************************************************/
void ResourceMonitor::UnRegister( void )
{
	StLockGuard<True_Thread_Mutex> g(mMutex);
	--mCount;
	FBL_CHECK( mCount >= 0 );

	// If no more waitable tasks - switch the semaphore to signaled state (free).
	//
	if( mCount == 0 )
		mSemaphore.Post();		
}


/**********************************************************************************************/
void ResourceMonitor::Wait( void )
{
	// Waiting for a semaphore (no monitored resources state).
	//
	mSemaphore.Wait();	// Wait until last task Unregistered.
	FBL_CHECK( mCount == 0 );
	mSemaphore.Post();	// To restore state of monitor.
}


/**********************************************************************************************/
void ResourceMonitor::RegisterError( xException& inError )
{
	StLockGuard<True_Thread_Mutex> g(mMutex); 

	if( mpErrors == nullptr )
		mpErrors = new ArrayOfExceptions( 10, kOwnItems );
	// Actually copying inError.
	mpErrors->AddItem( inError );
}


/**********************************************************************************************/
FBL_End_Namespace
