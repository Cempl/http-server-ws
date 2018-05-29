/**********************************************************************************************/
/* FBL_Thread_Null.h	                                                   					  */
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


#ifndef _FBL_Thread_Null_h 
	#define _FBL_Thread_Null_h 
#pragma once

// FBL:
#include <VShared/FBL/publ/Interfaces/FBL_I_Task.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
#define THREAD_API
#define THREAD_RETURN void*

typedef THREAD_RETURN (THREAD_API * THREAD_FUNC) ( void* );
typedef int TThreadHandle;


/**********************************************************************************************/
enum EThreadFlags
{
	kCreateSuspended = 0x01,
	kThreadDefault = kCreateSuspended
};


/**********************************************************************************************/
/// Null thread implementation.
///
class FBL_SHARED_EXP_CLASS Thread_Null
{
	public://///////////////////////////////////////////////////////////////////////////////////

							
							/** Constructor which creates the OS thread 
								with the given set of parameters
								and then runs the passed task. */
							Thread_Null( 
								I_Task_Ptr inTask = nullptr,
								vint32 inFlags = kThreadDefault,
								vint32 inPriority = 0,
								size_t inStackSize = 0
								);

							/// Allows derivation.
virtual						~Thread_Null( void );

							/// Resume the thread.
							/// No-op.
		void				Resume( void );

							/// Suspend the thread.
							/// No-op.
		void				Suspend( void );

							/**	Wait for the thread to exit 
								and reap its exit status. */
							/// No-op.
		vuint32				Wait( void );

							/**	Returns true if the thread is still running.
								Always return false. */
		bool				get_Alive( void ) const;

							/** Return the OS thread identifier 
								Always return 0. */
		vuint32				get_ID( void ) const;

							/**	Return the thread's exit code.
								Only has meaning after thread completing. 
								Always return 0. */
		vuint32				get_ExitCode( void ) const;
        
							/**	Return the task associated with this thread. */
		Const_I_Task_Ptr	get_Task( void ) const;

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
								size_t 		inN,
								I_Task_Ptr	inTasks[],
								Thread_Null*ioThreads[],
								vint32 		inFlags[] = nullptr,
								vint32 		inPriority[] = nullptr,
								size_t		inStackSize[] = nullptr
								);


							/** Spawn \a inN new thread objects of type \a T, 
								with default set of parameters. 
								@param inN		- number of threads to spawn.
								@param ioThreads- array to store created objects. */
template<class T>
static void					Spawn( 
								size_t 		 inN,
								Thread_Null* ioThreads[]
								);


							/**	Wait for all threads in the array to exit.
								@param inThreads - array of thread objects to wait for.
								@param inSize	 - elements count in the array.
								@param inTimeout - wait timeout in milliseconds. */
static void					Wait( 
								Thread_Null* inThreads[], 
								vuint32 inSize, 
								vuint32 inTimeout = 0 );


							/**	Resume all the threads in array.
								@param inThreads - array of thread objects to resume.
								@param inSize	 - elements count in the array. */
static void					Resume( 
								Thread_Null* inThreads[], 
								vuint32 inSize );

							/**	Deletes all thread objects from the array.
								@param inThreads- array of thread objects to delete to.
								@param inSize	- threads count in the array. */
static void					Destroy( 
								Thread_Null* inThreads[], 
								vuint32 inSize );


							/**	Spawn \a inN new thread with \a inFunc entry point.
								@param inN		  - number of threads to spawn.
								@param inFunc	  - thread entry point.
								@param inArg	  - array of thread arguments.
								@param inThreadHandles - array to store created objects.
								@param inFlags	  - thread creation flags.
								@param inPriority - thread creation priority.
								@param inStackSize- array of stack sizes for each thread.
								@return			  - number of succesfully spawned threads. */
static size_t				Spawn(
								size_t		inN,
								THREAD_FUNC	inFunc,
								void*		inArg[] = nullptr,
								TThreadHandle inThreadHandles[] = nullptr,
								vint32		inFlags[] = nullptr,
								vint32		inPriority[] = nullptr,
								size_t		inStackSize[] = nullptr );


							/**	Wait for all thread handles in the array to exit.
								@param inThreadHandles - array of thread handles to wait for.
								@param inSize	 - elements count in the array.
								@param inTimeout - wait timeout in milliseconds. */
static void					Wait( 
								TThreadHandle* inThreadHandles, 
								vuint32 inSize, 
								vuint32 inTimeout = 0
								);

	protected://////////////////////////////////////////////////////////////////////////////////


							/**	Concurrent entry point into the thread.
								Default implementation executes the task 
								associated with thread (if any). */
virtual vuint32			ThreadMain( void ); 


	private:///////////////////////////////////////////////////////////////////////////////////


		void				Run( void );


	private:///////////////////////////////////////////////////////////////////////////////////


		I_Task_Ptr			mTask;			/**< Task associated with the thread */
							
							/** Keeps track of whether thread has been 
								executed already since creation. */
		bool				mAlreadyDone;	

							/** Keeps error code returned from the 
								task (if any) */
		vuint32			mExitCode;

};


/**********************************************************************************************/
inline 
Thread_Null::Thread_Null( I_Task_Ptr inTask,
						  vint32 inFlags,
						  vint32 inPriority,
						  size_t inStackSize )
:
	mTask(inTask),
	mAlreadyDone(false),
	mExitCode(0)
{
	argused1(inPriority);
	argused1(inStackSize);

	if( !(inFlags & kCreateSuspended) )
	{
		// This is not generally safe 
		// to do virtual call in the ctor
		// and so all I can expect that 
		// will be called this class's function
		Run();
	}
}


/**********************************************************************************************/
inline 
void Thread_Null::Resume( void )
{
	if( !mAlreadyDone )
	{
		Run();
	}
}


/**********************************************************************************************/
inline 
void Thread_Null::Suspend( void )
{
	/* No-op */
}


/**********************************************************************************************/
inline 
vuint32 Thread_Null::Wait( void )
{
	return static_cast<vuint32>(mExitCode);
}


/**********************************************************************************************/
inline 
bool Thread_Null::get_Alive( void ) const
{
	return mAlreadyDone != true;
}


/**********************************************************************************************/
inline 
vuint32 Thread_Null::get_ID( void ) const
{
	return mAlreadyDone ? 0UL : 1UL;
}


/**********************************************************************************************/
inline 
vuint32 Thread_Null::get_ExitCode( void ) const 
{
	return static_cast<vuint32>(mExitCode);
}


/**********************************************************************************************/
inline
Const_I_Task_Ptr Thread_Null::get_Task( void ) const 
{
	return mTask;
}


/**********************************************************************************************/
template<class T> inline
void Thread_Null::Spawn(size_t inN,
						I_Task_Ptr inTasks[],
						Thread_Null* inThreads[],
						vint32 inFlags[],
						vint32 inPriority[],
						size_t inStackSize[] )
{
	FBL_CHECK(inThreads);
	if( !inThreads )
	{
		return;
	}

	size_t i = 0;
	for(; i < inN; ++i )
	{
		inThreads[i] = new T(
			inTasks ? inTasks[i] : nullptr,
			inFlags ? inFlags[i] : kCreateSuspended, 
			inPriority ? inPriority[i] : 0 , 
			inStackSize ? inStackSize[i] : 0
			);
		
		FBL_CHECK(inThreads[i]);
	}
}


/**********************************************************************************************/
template<class T>
void Thread_Null::Spawn( size_t inN, Thread_Null* ioThreads[] )
{
	FBL_CHECK(ioThreads);
	if( !ioThreads )
	{
		return;
	}

	size_t i = 0;
	for(; i < inN; ++i )
	{
		ioThreads[i] = new T;
		FBL_CHECK(ioThreads[i]);
	}
}


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // _FBL_Thread_Null_h
