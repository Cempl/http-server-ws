/**********************************************************************************************/
/* FBL_Thread_Win.h	                                                   						  */
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


#ifndef _FBL_Thread_Win_h 
	#define _FBL_Thread_Win_h 
#pragma once


// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Task.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
#if FBL_WIN 


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
#define THREAD_API __stdcall
#define THREAD_RETURN vuint32

typedef THREAD_RETURN (THREAD_API * THREAD_FUNC) ( void* );
typedef HANDLE TThreadHandle;


/**********************************************************************************************/
enum EThreadFlags
{
	kCreateSuspended = CREATE_SUSPENDED,
	kThreadDefault = kCreateSuspended
};


/**********************************************************************************************/
/// Thread implementation for Windows.
///
class FBL_SHARED_EXP_CLASS Thread_Win 
{
	public://///////////////////////////////////////////////////////////////////////////////////

							
							/**	Constructor which creates the OS thread 
								with the given set of parameters
								and then runs the passed task.*/
							Thread_Win( 
								I_Task_Ptr inTask = 0,
								vuint32 inFlags = kThreadDefault,
								vint32 inPriority = 0,
								size_t inStackSize = 0 );

							/// We allow derivation.
virtual						~Thread_Win( void );

							/// Resume the thread.
		void				Resume( void );

							/// Suspend the thread.
		void				Suspend( void );

							/**	Wait for the thread to exit 
								and reap its exit status. */
		vuint32				Wait( void );

							/// Returns true if the thread is still running.
		bool				get_Alive( void ) const;

							///	Return the OS thread identifier
		vuint32				get_ID( void ) const;

							/**	Return the thread's exit code.
								Only has meaning after thread completing. */
		vuint32				get_ExitCode( void ) const;

							/**	Return the thread's exit code.
								Only has meaning after thread completing. */
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
								Thread_Win*	ioThreads[],
								vuint32 		inFlags[] = 0,
								vint32 		inPriority[] = 0,
								size_t		inStackSize[] = 0 );


							/**	Spawn N new thread objects of type T, 
								with default set of parameters. 
								@param inN		- number of threads to spawn;
								@param ioThreads- array to store created objects; */
template<class T>
static void					Spawn( 
								size_t 		inN,
								Thread_Win*	ioThreads[] );

							/**	Wait for all threads in the array to exit.
								NOTE: the inSize value must not exceeds 
								MAXIMUM_WAIT_OBJECTS value, otherwise it 
								will be truncated.
								@param inThreads - array of thread objects to wait for.
								@param inSize	 - elements count in the array.
								@param inTimeout - wait timeout in milliseconds. */
static void					Wait( 
								Thread_Win* inThreads[], 
								vuint32 inSize, 
								vuint32 inTimeout = INFINITE );

							/**	Resume all the threads in array.
								@param inThreads- array of thread objects to resume.
								@param inSize	- elements count in the array. */
static void					Resume( 
								Thread_Win* inThreads[], 
								vuint32 inSize );

							/**	Deletes all thread objects from the array.
								@param inThreads- array of thread objects to delete to.
								@param inSize	- threads count in the array. */
static void					Destroy( 
								Thread_Win* inThreads[], 
								vuint32 inSize );

							/**	Spawn \a inN new thread with \a inFunc entry point.
								@param inN		  - number of threads to spawn.
								@param inFunc	  - thread entry point.
								@param inArg	  - array of thread arguments.
								@param ioThreadHandles - array to store created objects.
								@param inFlags	  - thread creation flags.
								@param inPriority - thread creation priority.
								@param inStackSize- array of stack sizes for each thread.
								@return			  - number of succesfully spawned threads. */
static size_t				Spawn(
								size_t		inN,
								THREAD_FUNC	inFunc,
								void*		inArg[] = 0,
								HANDLE		ioThreadHandles[] = 0,
								vuint32		inFlags[] = 0,
								vint32		inPriority[] = 0,
								size_t		inStackSize[] = 0 );

							/**	Wait for all thread handles in the array to exit.
								NOTE: the inSize value must not exceeds 
								MAXIMUM_WAIT_OBJECTS value, otherwise it 
								will be truncated.
								@param inThreadHandles - array of thread handles to wait for.
								@param inSize	- elements count in the array.
								@param inTimeout- wait timeout in milliseconds. */
static void					Wait( 
								HANDLE* inThreadHandles, 
								vuint32 inSize, 
								vuint32 inTimeout = INFINITE );


	protected://////////////////////////////////////////////////////////////////////////////////


							/**	Concurrent entry point into the thread.
								Default implementation executes the task 
								associated with thread (if any). */
virtual vuint32					ThreadMain( void ); 


	private:///////////////////////////////////////////////////////////////////////////////////


		HANDLE				mHandle;		/**< OS Thread's handle */
		I_Task_Ptr			mTask;			/**< Task associated with the thread */
		vuint32				mID;			/**< OS Thread's ID */
							
	
	
	private:///////////////////////////////////////////////////////////////////////////////////
		

static DWORD WINAPI			thread_entry_point( LPVOID lpParameter );

};



/**********************************************************************************************/
inline 
void Thread_Win::Resume( void )
{
	if( ::ResumeThread(mHandle) == -1 )
	{
		throw xExternalError(::GetLastError());
	}
}


/**********************************************************************************************/
inline 
void Thread_Win::Suspend( void )
{
	if( ::SuspendThread(mHandle) == -1 )
	{
		throw xExternalError(::GetLastError());
	}
}


/**********************************************************************************************/
inline 
vuint32 Thread_Win::Wait( void )
{
	DWORD ExitCode;

	if( ::WaitForSingleObject(mHandle, INFINITE) == WAIT_OBJECT_0
		&& ::GetExitCodeThread(mHandle, &ExitCode) != FALSE )
	{
		; /* NO-OP */
	}
	else
	{
		throw xExternalError(::GetLastError());
	}

	return ExitCode;
}


/**********************************************************************************************/
inline 
bool Thread_Win::get_Alive( void ) const
{
	DWORD ExitCode;
	::GetExitCodeThread(mHandle, &ExitCode);
	return ExitCode == STILL_ACTIVE;
}


/**********************************************************************************************/
inline 
vuint32 Thread_Win::get_ID( void ) const
{
	return mID;
}


/**********************************************************************************************/
inline 
vuint32 Thread_Win::get_ExitCode( void ) const 
{
	DWORD ExitCode;
	::GetExitCodeThread(mHandle, &ExitCode);
	return static_cast<vuint32>(ExitCode);
}


/**********************************************************************************************/
inline
Const_I_Task_Ptr Thread_Win::get_Task( void ) const 
{
	return mTask;
}


/**********************************************************************************************/
template<class T> inline
void Thread_Win::Spawn(	size_t inN,
						I_Task_Ptr inTasks[],
						Thread_Win* inThreads[],
						vuint32 inFlags[],
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
			inTasks ? inTasks[i] : 0,
			inFlags ? inFlags[i] : kCreateSuspended, 
			inPriority ? inPriority[i] : 0 , 
			inStackSize ? inStackSize[i] : 0
			);
		
		FBL_CHECK(inThreads[i]);
	}
}


/**********************************************************************************************/
template<class T>
void Thread_Win::Spawn( size_t inN, Thread_Win* ioThreads[] )
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
#endif // _FBL_Thread_Win_h

#endif // FBL_WIN
