/**********************************************************************************************/
/* FBL_Thread_Win.cpp																	 	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#define _FBL_Synch_h // trick to enable the following header:
#include "FBL_Thread_Win.h"


/**********************************************************************************************/
#if FBL_WIN

#include <process.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
Thread_Win::Thread_Win( 
	I_Task_Ptr inTask,
	vuint32 inFlags, 
	vint32 inPriority, 
	size_t inStackSize )
:
	mTask(inTask)
{
	mHandle = (HANDLE) _beginthreadex(
		NULL, 
		(unsigned)inStackSize, 
		(THREAD_FUNC)&thread_entry_point, 
		(LPVOID)this, 
		inFlags, 
		&mID);

	if( mHandle != NULL )
	{
		BOOL b = ::SetThreadPriority(mHandle, inPriority);
		FBL_CHECK(b == TRUE);
		argused1(b);
	}
	else
	{
		throw xExternalError(::GetLastError());
	}
}


/**********************************************************************************************/
Thread_Win::~Thread_Win( void )
{
	if( get_Alive() )
	{
		// this is very dangerous way
		// so do not delete thread object
		// until it is runnning...
		try
		{
			::TerminateThread(mHandle, 0xFFFFFFFF);
		}
		catch(...)
		{}
	}

	::CloseHandle(mHandle);
}


/**********************************************************************************************/
void Thread_Win::Wait( Thread_Win* inThreads[], vuint32 inSize, vuint32 inTimeout )
{
	FBL_CHECK(inSize <= MAXIMUM_WAIT_OBJECTS);

	// at first build the array of thread descriptors
	vuint32 Size = inSize > MAXIMUM_WAIT_OBJECTS ? MAXIMUM_WAIT_OBJECTS : inSize;
	HANDLE Handles[MAXIMUM_WAIT_OBJECTS];
	memset(Handles, 0, sizeof(HANDLE) * Size);

	for( vuint32 i = 0; i < Size; ++i)
	{
		FBL_CHECK(inThreads[i]);
		Handles[i] = inThreads[i]->mHandle;
	}

	// second - call the dispatcher;
	DWORD dw = ::WaitForMultipleObjects(Size, 
										Handles,
										TRUE,
										inTimeout);
	if( dw == WAIT_FAILED )
	{
		throw xExternalError(::GetLastError());
	}
	
	// all handles should be signalled:
	FBL_CHECK(dw == WAIT_OBJECT_0);
}


/**********************************************************************************************/
void Thread_Win::Resume( Thread_Win* inThreads[], vuint32 inSize )
{
	FBL_CHECK(inThreads);
	if( !inThreads )
		return;

	// spawn it
	for( vuint32 i = 0; i < inSize; ++i )
	{ 
		inThreads[i]->Resume();
	}
}


/**********************************************************************************************/
size_t Thread_Win::Spawn( 
	size_t		inN,
	THREAD_FUNC	inFunc,
	void*		inArg[],
	HANDLE		ioThreadHandles[],
	vuint32		inFlags[],
	vint32		inPriority[],
	size_t		inStackSize[] )
{
	FBL_CHECK(ioThreadHandles);
	if( !ioThreadHandles )
		return 0;

	vuint32	ID;
	HANDLE  LocalH;
	vuint32	i = 0;

	// spawn it
	for(; i < inN; ++i )
	{ 
		HANDLE&	h = ioThreadHandles ? ioThreadHandles[i] : LocalH;

		h = (HANDLE) _beginthreadex(
			NULL, 
			(unsigned)(inStackSize ? inStackSize[i] : 0), 
			inFunc, 
			inArg ? inArg[i] : 0, 
			inFlags ? inFlags[i] : kThreadDefault, 
			&ID);

		if( h != NULL )
		{
			BOOL b = ::SetThreadPriority(h, inPriority ? inPriority[i] : THREAD_PRIORITY_NORMAL);
			FBL_CHECK(b == TRUE);
			argused1(b);
		}
		else
		{
			break;
		}
	}

	return i;
}


/**********************************************************************************************/
void Thread_Win::Wait( HANDLE inThreadHandles[], vuint32 inSize, vuint32 inTimeout )
{
	// check the array of descriptors
	FBL_CHECK(inThreadHandles);
	if( !inThreadHandles )
		return;

	// check the size
	vuint32 Size = inSize > MAXIMUM_WAIT_OBJECTS ? MAXIMUM_WAIT_OBJECTS : inSize;

	// call the dispatcher
	DWORD dw = ::WaitForMultipleObjects(Size, 
										inThreadHandles,
										TRUE,
										inTimeout);
	if( dw == WAIT_FAILED )
	{
		throw xExternalError(::GetLastError());
	}
	
	// all handles should be signalled:
	FBL_CHECK(dw == WAIT_OBJECT_0);
}


/**********************************************************************************************/
DWORD WINAPI Thread_Win::thread_entry_point( LPVOID lpParameter )
{
	Thread_Win* p = reinterpret_cast<Thread_Win*>(lpParameter);
	FBL_CHECK(p);

	DWORD Result = 0xFFFFFFFF;
	try 
	{
		Result = p->ThreadMain();
	} 
	catch(...)
	{
		// Something is going wrong :( ...
		FBL_CHECK(0);
	}

	// Thread identifier has no meaning 
	// from here since thread is exiting.
	// So null it.
	p->mID = 0;

	return Result;
}


/**********************************************************************************************/
vuint32 Thread_Win::ThreadMain( void )
{
	if( mTask )
	{
		mTask->Run();
		return (vuint32)mTask->get_Error();
	}

	return 0;
}


/**********************************************************************************************/
void Thread_Win::Destroy( Thread_Win* inThreads[], vuint32 inSize )
{
	FBL_CHECK(inThreads);
	if( !inThreads )
	{
		return;
	}

	// spawn it
	for( vuint32 i = 0; i < inSize; ++i )
	{ 
		delete inThreads[i];
	}
}


/**********************************************************************************************/
FBL_End_Namespace

#endif // FBL_WIN
