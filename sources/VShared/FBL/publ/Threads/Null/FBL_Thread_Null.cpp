/**********************************************************************************************/
/* FBL_Thread_Null.cpp																		  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#define _FBL_Synch_h // trick to enable the following header:
#include "FBL_Thread_Null.h"


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
Thread_Null::~Thread_Null( void )
{
}


/**********************************************************************************************/
void Thread_Null::Wait( Thread_Null* inThreads[], vuint32 inSize, vuint32 inTimeout )
{
	argused3(inThreads, inSize, inTimeout);
}


/**********************************************************************************************/
void Thread_Null::Resume( Thread_Null* inThreads[], vuint32 inSize )
{
	FBL_CHECK(inThreads);
	if( !inThreads )
	{
		return;
	}

	// spawn it
	for( vuint32 i = 0; i < inSize; ++i )
	{ 
		inThreads[i]->Resume();
	}
}


/**********************************************************************************************/
void Thread_Null::Destroy( Thread_Null* inThreads[], vuint32 inSize )
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
size_t Thread_Null::Spawn( size_t		inN,
						  THREAD_FUNC	inFunc,
						  void*			inArg[],
						  TThreadHandle	inThreadHandles[],
						  vint32			inFlags[],
						  vint32			inPriority[],
						  size_t		inStackSize[] )
{
	argused1(inN);
	argused1(inFunc);
	argused1(inArg);
	argused1(inThreadHandles);
	argused1(inFlags);
	argused1(inPriority);
	argused1(inStackSize);

	// Not (yet ???) implemented
	return 0;
}


/**********************************************************************************************/
void Thread_Null::Wait( TThreadHandle inThreadHandles[], vuint32 inSize, vuint32 inTimeout )
{
	argused3(inThreadHandles, inSize, inTimeout);
}



/**********************************************************************************************/
vuint32 Thread_Null::ThreadMain( void )
{
	if( mTask )
	{
		mTask->Run();
		return (vuint32)mTask->get_Error();
	}

	return 0UL;
}


/**********************************************************************************************/
void Thread_Null::Run( void )
{
	mExitCode = ThreadMain();
	mAlreadyDone = true;
}


/**********************************************************************************************/
FBL_End_Namespace








