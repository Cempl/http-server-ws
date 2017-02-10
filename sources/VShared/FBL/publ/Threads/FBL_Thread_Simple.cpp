/**********************************************************************************************/
/* FBL_Thread_Simple.cpp																	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                       					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#define _FBL_Synch_h // trick to enable the following header:
#include "FBL_Thread_Simple.h"

#include <VShared/FBL/publ/Interfaces/FBL_I_Localizable.h>
#include <VShared/FBL/publ/Util_classes/FBL_ObjectPool.h>

#if !FBL_WIN 
#include <pthread.h>
#else
#include <process.h>
#endif //FBL_WIN 


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
ValentinaThreadInitEx_PTR		pValentinaThreadInitEx		= nullptr;
ValentinaThreadShutDownEx_PTR	pValentinaThreadShutDownEx	= nullptr;


/**********************************************************************************************/
// To be able to call ValentinaThreadInit/ValentinaThreadShutDown number of times
// but do the real init/shutdown only once.
// It is for QThreadPool, async calls and so on... (It could be pretty hard to get access to the
// thread start/finish moments - so we call ValentinaThreadInit/ValentinaThreadShutDown not in
// "thread-main" function but in "thread-worker" one )
//
#if FBL_INTERNAL_THREADSAFE || FBL_EXTERNAL_THREADSAFE
TLS_Var		tlsInitCounter;
#endif // FBL_INTERNAL_THREADSAFE || FBL_EXTERNAL_THREADSAFE


/**********************************************************************************************/
void ValentinaThreadInit( void )
{
	#if FBL_INTERNAL_THREADSAFE || FBL_EXTERNAL_THREADSAFE
	
		vuint64* threadInitCounter = (vuint64*)tlsInitCounter.get();
		if( threadInitCounter == nullptr )
		{
			threadInitCounter = new vuint64(1);

			#if FBL_TEST_CODE
			{
				StLockGuard<True_Thread_Mutex> lock(gThreadIDArrayMutex);
				
				if( gThreadIDArray == NULL )
					gThreadIDArray = new ArrayOfUllong();
				
				#if FBL_WIN
					gThreadIDArray->AddItem( (fbl::vuint64)GetCurrentThreadId() );
				#else
					gThreadIDArray->AddItem( (fbl::vuint64)( pthread_self() ) );
				#endif //FBL_WIN
			}
			#endif // FBL_TEST_CODE		

			// vKernel stuff.
			if( pValentinaThreadInitEx )
				pValentinaThreadInitEx();
		}
		else
		{
			// Just increment counter to use this knowledge in ValentinaThreadShutDown.
			(*threadInitCounter)++;
		}

		tlsInitCounter.set( threadInitCounter );
	
	#endif // FBL_INTERNAL_THREADSAFE || FBL_EXTERNAL_THREADSAFE		
}


/**********************************************************************************************/
void ValentinaThreadShutDown( void )
{
	#if FBL_INTERNAL_THREADSAFE || FBL_EXTERNAL_THREADSAFE

		vuint64* threadInitCounter = (vuint64*)tlsInitCounter.get();
		if( threadInitCounter )
		{
			if( (*threadInitCounter) > 1 )
			{
				// ValentinaThreadShutDown is called, but we just register this fact
				// instead of real job.
				(*threadInitCounter)--;
			}
			else
			{
				// (*threadInitCounter) == 1
				//

				#if FBL_TEST_CODE
				{
					StLockGuard<True_Thread_Mutex> lock(gThreadIDArrayMutex);
					
					FBL_CHECK( gThreadIDArray );
					if( gThreadIDArray )
					{
						#if FBL_WIN
							gThreadIDArray->RemoveItem( (fbl::vuint64)GetCurrentThreadId() );
						#else
							gThreadIDArray->RemoveItem( (fbl::vuint64)( pthread_self() ) );
						#endif //FBL_WIN		
					}
				}
				#endif // FBL_TEST_CODE

				// vKernel stuff.
				if( pValentinaThreadShutDownEx )
					pValentinaThreadShutDownEx();
				
				// "Free" TLS objects (safe even if this thread did not "create" tlsSysLocalizable ).
				tlsSysLocalizable.SafeRelease();
				
				// "Free" tlsInitCounter object
				ForgetObject( threadInitCounter );
			}
			
			tlsInitCounter.set( threadInitCounter );
		}
		else
		{
			// Silently ignore - ValentinaThreadShutDown is called before any ValentinaThreadInit.
		}
		
	#endif // FBL_INTERNAL_THREADSAFE || FBL_EXTERNAL_THREADSAFE
}


/**********************************************************************************************/
// Predefined "main" thread function.
// We need it to run some "pre" and "post" steps like free TLS objects.
//
FBL_THREAD_ROUTINE( FBL_Thread_Main_Function, inFBL_Thread_Args );
FBL_THREAD_ROUTINE( FBL_Thread_Main_Function, inFBL_Thread_Args )
{
	// Extract data from parameter:
	FBL_Thread_Args* pArgs = (FBL_Thread_Args*) inFBL_Thread_Args;
	FBL_CHECK( pArgs && pArgs->mpStartRoutine );
	
	// "Prepare" steps:
	//
	ValentinaThreadInit();
	
	// Run thread routine:
	//
	try
	{
		pArgs->mpStartRoutine( pArgs->mpArgs );
	}
	catch(...)
	{
		// Silently catch any exception since not cautgh exception in the thread cause
		// application abort.
	}
	
	// "Clearing" steps:
	//
	ValentinaThreadShutDown();
	
	return 0;
}


#pragma mark -


/**********************************************************************************************/
Thread_Simple::Thread_Simple( FBL_THREAD_ROUTINE_TYPE inpStartRoutine, void* inpArgs )
:	
	mHandle(0),
	mpThreadArgs( new FBL_Thread_Args() )
{
	Create( inpStartRoutine, inpArgs );
}


/**********************************************************************************************/
Thread_Simple::~Thread_Simple( void )
{
	#if FBL_WIN 
		if( mHandle )
			CloseHandle( mHandle );
	#endif //FBL_WIN 
	
	ForgetObject( mpThreadArgs );
}


/**********************************************************************************************/
void Thread_Simple::Create( FBL_THREAD_ROUTINE_TYPE inpStartRoutine, void* inpArgs )
{
	mpThreadArgs->mpStartRoutine	= inpStartRoutine;
	mpThreadArgs->mpArgs			= inpArgs;
	
	#if FBL_WIN 
		if( mHandle )
			CloseHandle( mHandle );
		// IS: 09.11.2011
		// _beginthreadex() is prefferable and safe for creating thread because of
		// 1. Implemented in MT std lib only.
		// 2. SEH in some calls like signal()
		//
		//mHandle = CreateThread( NULL, 0, inpStartRoutine, inpArgs, 0, NULL);
		mHandle = (HANDLE) _beginthreadex( 
									NULL,
									0,
									(PTHREAD_START)FBL_Thread_Main_Function,
									(void*)mpThreadArgs,
									0,
									NULL);
	#else
		pthread_create(
									&mHandle,
									nullptr,
									FBL_Thread_Main_Function,
									(void*)mpThreadArgs );
	#endif //FBL_WIN 
}


/**********************************************************************************************/
void Thread_Simple::join( void )
{
	if( mHandle )
	{
	#if FBL_WIN 
		WaitForSingleObject( mHandle, INFINITE );
	#else
		pthread_join( mHandle, nullptr );
	#endif //FBL_WIN 
	}
}


/**********************************************************************************************/
FBL_End_Namespace
