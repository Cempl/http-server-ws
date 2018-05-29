/**********************************************************************************************/
/* FBL_Thread_Posix.cpp																	 	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

// STD:
#include <errno.h>
#include <signal.h>

// VSHARED:
#include <VShared/FBL/publ/Headers/StdAfx.h>

#define _FBL_Synch_h // trick to enable the following header:
#include "FBL_Thread_Posix.h"


/**********************************************************************************************/
#if FBL_POSIX_API

#include <limits.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// Helper routine to spawn pthread.
//
namespace {

void spawn_pthread(	pthread_t& outThread,
					THREAD_FUNC inFunc,
					void* inArg = nullptr,
					long inFlags = 0, 
					long inPriority = 0, 
					size_t inStackSize = 0 )
{
	// not yet implemented
	argused2(inFlags, inPriority);

	pthread_attr_t attr;
	PTHREAD_API_CALL( ::pthread_attr_init(&attr) );

	// Make the thread joinable by default.
	int error;
	if( (error = ::pthread_attr_setdetachstate(
		&attr, PTHREAD_CREATE_JOINABLE)) != 0 )
	{
		::pthread_attr_destroy(&attr);
		throw xOSFileError( (vuint32) error );
	}

	// Handle stack size.
	if( inStackSize != 0 )
    {
		size_t size = inStackSize;
		if( size < static_cast<size_t>(PTHREAD_STACK_MIN) )
		{
			size = PTHREAD_STACK_MIN;
		}

		if( (error = ::pthread_attr_setstacksize(&attr, size) != 0) )
		{
			::pthread_attr_destroy(&attr);
			throw xOSFileError( (vuint32) error );
		}
	}

	if( ::pthread_create( &outThread, &attr, inFunc, inArg) == 0 )
	{
  		::pthread_attr_destroy(&attr);
			throw xOSFileError( (vuint32) errno );
	}

	::pthread_attr_destroy(&attr);
}

}


#pragma mark -


/**********************************************************************************************/
Thread_Posix::Thread_Posix( 
	I_Task_Ptr inTask, 
	long inFlags, 
	long inPriority, 
	size_t inStackSize )
:
	mExitCode(0),
	mTask(inTask)
{
	argused1(inPriority);

	// determine whether we must suspend self on creation
	mSuspended = (inFlags & kCreateSuspended) != 0;

	// operation success code
//	int error = 0;

	// initialize the internal lock
	PTHREAD_API_CALL(::pthread_mutex_init(&mLock, NULL));

	// use helper to spawn the thread
	spawn_pthread( mThrID, &thread_entry_point, (void*)this, 0, 0, inStackSize );
}


/**********************************************************************************************/
Thread_Posix::~Thread_Posix( void )
{
	if(get_Alive())
	{
		::pthread_kill( mThrID, 0 );
	}

	::pthread_mutex_destroy(&mLock);
}


/**********************************************************************************************/
void Thread_Posix::Wait( Thread_Posix* inThreads[], vuint32 inSize, vuint32 inTimeout )
{
	argused2( inThreads, inTimeout );
	
	for( vuint32 i = 0; i < inSize; ++i)
	{
		PTHREAD_API_CALL( ::pthread_join(inThreads[i]->mThrID, &Result) );
	}
}


/**********************************************************************************************/
size_t Thread_Posix::Spawn( 
	size_t 			inN,
	THREAD_FUNC 	inFunc,
	void* 			inArg[],
	TThreadHandle 	ioThreadHandles[],
	long 			inFlags,
	long 			inPriority,
	size_t 			inStackSize[],
	vuint32* 		outSpawned )
{
	// check out the outHThreads:
	ioThreadHandles = ioThreadHandles ? ioThreadHandles : new TThreadHandle[inN];

	// correctly set the count of spawned
	vuint32 LocalSpawned = 0;
    vuint32& i = (outSpawned ? *outSpawned = 0 : LocalSpawned);

	// finally: spawn it
	for( ; i < inN; ++i )
	{
		spawn_pthread( ioThreadHandles[i],
					   inFunc,
					   inArg ? inArg[i] : nullptr, 
					   inFlags,
					   inPriority,
					   inStackSize[i] );
	}
	
	return inN;
}


/**********************************************************************************************/
void Thread_Posix::Wait( 
    TThreadHandle   inThreadHandles[], 
    vuint32         inSize, 
    vuint32         inTimeout )
{
	argused1(inTimeout);

	// check the array of descriptors
	FBL_CHECK(inThreadHandles);
	if( !inThreadHandles )
	{
		return;
	}

	for( vuint32 i = 0; i < inSize; ++i)
	{
		int Result = 0;
		PTHREAD_API_CALL( ::pthread_join(inThreadHandles[i], &Result) );
		
		argused1( Result );
	}
}


/**********************************************************************************************/
void* Thread_Posix::thread_entry_point( void* inArg )
{
	Thread_Posix* p = reinterpret_cast<Thread_Posix*>(inArg);
	FBL_CHECK(p);

	// Since POSIX threads API do not support
	// natively creation of suspended threads 
	// we need to suspend it by hand
	::pthread_mutex_lock(&p->mLock);
	bool suspend_self = p->mSuspended;
	::pthread_mutex_unlock(&p->mLock);
	if( suspend_self )
	{
		p->Suspend();
	}

	int LocalResult = 0xFFFFFFFF;

	try 
	{
		LocalResult = (int) p->ThreadMain();
	}		
	catch(...)
	{
		// Something is going wrong :( ...
		FBL_CHECK(0);
	}

	::pthread_mutex_lock(&p->mLock);
	
	// Store result
	p->mExitCode = (vuint32) LocalResult;
	
	// Thread identifier has no meaning 
	// from here since thread is exiting.
	// So null it.
	p->mThrID = 0;

	::pthread_mutex_unlock(&p->mLock);

	// exit the thread
	::pthread_exit( &LocalResult );
	
	return nullptr;
}


/**********************************************************************************************/
vuint32 Thread_Posix::ThreadMain( void )
{
	if( mTask )
	{
		mTask->Run();
		return (vuint32) mTask->get_Error();
	}

	return 0;
}


/**********************************************************************************************/
void Thread_Posix::Resume( 
	Thread_Posix*   inThreads[], 
	vuint32         inSize )
{
    fixme_argused2( inThreads, inSize );

	// TODO !!!!!!
}


/**********************************************************************************************/
void Thread_Posix::Destroy( Thread_Posix* inThreads[], vuint32 inSize )
{
	FBL_CHECK(inThreads);
	if( !inThreads )
		return;

	// spawn it
	for( vuint32 i = 0; i < inSize; ++i )
	{ 
		delete inThreads[i];
	}
}


/**********************************************************************************************/
FBL_End_Namespace

#endif // FBL_POSIX_API


