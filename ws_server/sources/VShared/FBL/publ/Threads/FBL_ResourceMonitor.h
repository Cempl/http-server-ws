/**********************************************************************************************/
/* FBL_ResourceMonitor.h	                                       							  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/
#ifndef _FBL_ResourceMonitor_h
	#define _FBL_ResourceMonitor_h
#pragma once


/**********************************************************************************************/
#include <VShared/FBL/publ/Threads/FBL_Synch.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace

/**********************************************************************************************/
SMART_ARRAY_OF_CLASSES( ArrayOfExceptions, xException );


/**********************************************************************************************/
class FBL_SHARED_EXP_CLASS ResourceMonitor : public I_Unknown
{
	IMPLEMENT_SINGLE_UNKNOWN(ResourceMonitor)

	public://///////////////////////////////////////////////////////////////////////////////////

							/// Initialize the monitor.
							ResourceMonitor( void )
							:
								mCount(0),
								mSemaphore(1)
							{
								// Initially, semaphore is in signalled state (free)
								// (no monitored resources)
							}

							~ResourceMonitor( void )
							{
							}


	public://///////////////////////////////////////////////////////////////////////////////////

							// Each resource call it to register itself as monitored one.
		void				Register( void );

							// Each resource call it to unregister itself 
							// from set of monitored resources.
		void				UnRegister( void );

		vint32				get_ResorceCount( void )
							{
								StLockGuard<True_Thread_Mutex> g(mMutex);
								return mCount;
							}
				
							// Caller will be waiting until monitored resources exist.
		void				Wait( void );

		void				RegisterError( xException& inError );
		
							// Errors accumulated during monitored resources execution/life.
		vuint32				get_ErrorCount( void )
							{
								return (mpErrors) ? mpErrors->get_Count() : 0;
							}
				
		const xException*	get_Error( vuint32 inIndex )
							{
								return ( mpErrors && inIndex <= mpErrors->get_Count() ) ?
															&(mpErrors->get_ItemAt(inIndex)) : nullptr;
							}
				

	private:////////////////////////////////////////////////////////////////////////////////////

		vint32						mCount;			// Resource count

									// Resource semaphore. We can not use a mutex instead of this
									// binary semaphor because of mutex is owned by some particular
									// thread - all the following leads to undefined behavior:
									// 1.	Mutex can not be locked in thread 1 and
									//		released in thread 2.
									// 2.	Mutex can not be locked and then twice released.
									//
		Semaphore					mSemaphore;

									// Resource mutex.
		True_Thread_Mutex			mMutex;
		
		ArrayOfExceptions_Ptr		mpErrors;

};


/**********************************************************************************************/
SMART_CLASS(ResourceMonitor);

/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif //_FBL_ResourceMonitor_h
