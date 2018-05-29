/**********************************************************************************************/
/* FBL_Thread_Posix.h	                                                   					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/
/**
 *	IMPORTANT NOTE: DO NOT include this file directly
 *		unless you know what you are doing!
 *	Include <VShared/FBL/publ/Threads/FBL_Synch.h> instead.
 */
	// and strong rule of this:
	#ifndef _FBL_Synch_h
		#error "Please include FBL_Sync.h instead of this header."
	#endif // _FBL_Synch_h


#ifndef __FBL_Thread_Posix_h
	#define __FBL_Thread_Posix_h
#pragma once

// POSIX:
#include <pthread.h>

// FBL:
 #include <VShared/FBL/publ/Interfaces/FBL_I_Task.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
#if !FBL_POSIX_API
#error This file should be used only for POSIX API builds.
#endif // !FBL_POSIX_API


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
#define PTHREAD_API_CALL(X)
/*
{\
	int error = 0;\
	if( (error = (X)) != 0 ) \
	{ \
		throw xExternalError(error); \
	}\
}*/


/**********************************************************************************************/
#define THREAD_API 
#define THREAD_RETURN void*

typedef THREAD_RETURN (THREAD_API * THREAD_FUNC) ( void* );
typedef pthread_t TThreadHandle;

enum EThreadFlags
{
	kCreateSuspended = 0x01,
	kThreadDefault = kCreateSuspended
};


/**********************************************************************************************/
/// Thread implementation using POSIX thread API.
///
class FBL_SHARED_EXP_CLASS Thread_Posix
{
	public://///////////////////////////////////////////////////////////////////////////////////

							/**	Constructor which creates the OS thread 
								with the given set of parameters
								and then runs the passed task. */
							Thread_Posix( 
								I_Task_Ptr inTask = nullptr,
								long inFlags = kThreadDefault,
								long inPriority = 0,
								size_t inStackSize = 0 );

							/// We allow derivation.
virtual						~Thread_Posix( void );

							/// Resume the thread.
		void				Resume( void );

							/// Suspend the thread.
		void				Suspend( void );

							/**	Wait for the thread to exit 
								and reap its exit status. */
		vuint32				Wait( void );

							/// Returns true if the thread is still running.
		bool				get_Alive( void );

							/**	Return the OS thread identifier */
		vuint32				get_ID( void );

							/**	Return the task associated with this thread. */
		Const_I_Task_Ptr	get_Task( void ) const throw();

							/**	Return the thread's exit code.
								Only has meaning after thread completing. */
		vuint32				get_ExitCode( void ) const throw();

							/**	Wait for all threads in the array to exit.
								@param inThreads - array of thread objects to wait for.
								@param inSize	 - elements count in the array.
								@param inTimeout - wait timeout in milliseconds. */
static	void				Wait( 
								Thread_Posix* inThreads[], 
								vuint32 inSize, 
								vuint32 inTimeout = 0 );

							/**	Resume all the threads in array.
								@param inThreads - array of thread objects to resume.
								@param inSize	 - elements count in the array. */
static void					Resume( 
								Thread_Posix* inThreads[], 
								vuint32 inSize );

							/**	Deletes all thread objects from the array.
								@param inThreads- array of thread objects to delete to.
								@param inSize	- threads count in the array. */
static void					Destroy( 
								Thread_Posix* inThreads[], 
								vuint32 inSize );

							/**	Spawn N new thread objects of type T, 
								each of which calls Run() method of 
								passed I_Task object.
								@param inN		  - number of threads to spawn.
								@param inTask	  - array of each thread's task.
								@param inThreads  - array to store created objects.
								@param inFlags	  - array of each thread's creation flags.
								@param inPriority - array of each thread's priorities.
								@param inStackSize- array of stack sizes for each thread. */
template<class T> 
static	void				Spawn( 
								size_t 			inN,
								I_Task_Ptr 		inTask[],
								Thread_Posix* 	inThreads[],
								long 			inFlags[] = nullptr,
								long 			inPriority[] = nullptr,
								size_t 			inStackSize[] = nullptr );

							/** Spawn \a inN new thread objects of type \a T, 
								with default set of parameters. 
								@param inN		- number of threads to spawn.
								@param ioThreads- array to store created objects. */
template<class T>
static void					Spawn( 
								size_t 		 	inN,
								Thread_Posix* 	ioThreads[] );

							/**	Spawn \a inN new thread with \a inFunc entry point.
								@param inN		  - number of threads to spawn.
								@param inFunc	  - thread entry point.
								@param inArg	  - array of thread arguments.
								@param ioThreadHandles - array to store created objects.
								@param inFlags	  - thread creation flags.
								@param inPriority - thread creation priority.
								@param inStackSize- array of stack sizes for each thread.
								@return			  - number of succesfully spawned threads. */
static	size_t				Spawn(
								size_t			inN,
								THREAD_FUNC		inFunc,
								void*			inArg[] = nullptr,
								TThreadHandle 	ioThreadHandles[] = nullptr,
								long			inFlags = 0,
								long			inPriority = 0,
								size_t			inStackSize[] = nullptr,
								vuint32* 			outSpawned = nullptr );

							/**	Wait for all thread handles in the array to exit.
								@param inThreadHandles - array of thread handles to wait for.
								@param inSize	- elements count in the array.
								@param inTimeout- wait timeout in milliseconds. */
static	void				Wait( 
								TThreadHandle* inThreadHandles, 
								vuint32 inSize, 
								vuint32 inTimeout = vuint32(-1) );

	protected://////////////////////////////////////////////////////////////////////////////////


							/**	Concurrent entry point into the thread.
								Default implementation executes the task 
								associated with thread (if any). */
virtual vuint32				ThreadMain( void ); 


	private:////////////////////////////////////////////////////////////////////////////////////

							/// POSIX thread identifier
		pthread_t			mThrID;

							/// To serialize access to the internal state.
		pthread_mutex_t		mLock;

							/// Thread's exit code.
		vuint32				mExitCode;

							/// Task associated with the thread.
		I_Task_Ptr			mTask;

							/// Set to true if the thread must be suspended.
		bool				mSuspended;


	private:////////////////////////////////////////////////////////////////////////////////////


static void*				thread_entry_point( void* inArg );

};


/**********************************************************************************************/
inline 
void Thread_Posix::Resume( void )
{
	PTHREAD_API_CALL(::pthread_continue(&mThrID));
}


/**********************************************************************************************/
inline 
void Thread_Posix::Suspend( void )
{
	PTHREAD_API_CALL(::pthread_suspend(&mThrID));
}


/**********************************************************************************************/
inline 
vuint32 Thread_Posix::Wait( void )
{
	vuint32 Result = 0;
     
	// there are no race conditions here, so...
	FBL_CHECK(Result == mExitCode);

	return vuint32( Result );
}


/**********************************************************************************************/
inline 
bool Thread_Posix::get_Alive( void )
{
	PTHREAD_API_CALL( ::pthread_mutex_lock(&mLock) );
	
		bool Alive = (mThrID == 0);
		
	PTHREAD_API_CALL( ::pthread_mutex_unlock(&mLock) );
	
	return Alive;
}


/**********************************************************************************************/
inline 
vuint32 Thread_Posix::get_ID( void )
{
	PTHREAD_API_CALL(::pthread_mutex_lock(&mLock));
	
	// Is this right ???
	vuint32 ID = 0;// = static_cast<vuint32>(mThrID);
	
	PTHREAD_API_CALL(::pthread_mutex_unlock(&mLock));
	return ID;
}


/**********************************************************************************************/
inline 
Const_I_Task_Ptr Thread_Posix::get_Task( void ) const throw()
{
	return mTask;
}

/**********************************************************************************************/
inline 
vuint32 Thread_Posix::get_ExitCode( void ) const throw()
{
	PTHREAD_API_CALL( ::pthread_mutex_lock( (pthread_mutex_t*) &mLock) );
	
		vuint32 ExitCode = mExitCode;
		
	PTHREAD_API_CALL( ::pthread_mutex_unlock( (pthread_mutex_t*)  &mLock) );

	return ExitCode;
}


/**********************************************************************************************/
template<class T>
void Thread_Posix::Spawn( 
	size_t 			inN,
	I_Task_Ptr 		inTask[],
	Thread_Posix* 	inThreads[],
	long 			inFlags[],
	long 			inPriority[],
	size_t 			inStackSize[] )
{
	FBL_CHECK(inThreads);
	if( !inThreads )
		return;

	size_t i = 0;
	for( ; i < inN; ++i )
	{
		inThreads[i] = new T(
			inTask[i], 
			inFlags ? inFlags[i] : 0, 
			inPriority ? inPriority[i] : 0 , 
			inStackSize ? inStackSize[i] : 0 );
		
		FBL_CHECK(inThreads[i]);
	}
}


/**********************************************************************************************/
template<class T>
void Thread_Posix::Spawn( size_t inN, Thread_Posix* ioThreads[] )
{
	FBL_CHECK(ioThreads);
	if( !ioThreads )
		return;

	size_t i = 0;
	for( ; i < inN; ++i )
	{
		ioThreads[i] = new T;
		FBL_CHECK(ioThreads[i]);
	}
}


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // __FBL_Thread_Posix_h
