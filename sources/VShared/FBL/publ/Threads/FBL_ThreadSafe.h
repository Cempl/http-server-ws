/**********************************************************************************************/
/* FBL_ThreadSafe.h	                                               							  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/
/**
 *	The main purpose of this file is to hide platform 
 *	differenses when working in thread-safe environment.
 *
 *	Another purpose of this file is to include
 *	all minimum needed stuff to work with 
 *	synchronization with one step.
 *	
 */

#ifndef _FBL_ThreadSafe_h
	#define _FBL_ThreadSafe_h
#pragma once


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_Macros.h>
#include <VShared/FBL/publ/Threads/FBL_Mutex.h>
#include <VShared/FBL/publ/Threads/FBL_Semaphore.h>
#include <VShared/FBL/publ/Threads/FBL_CriticalSection.h>
#include <VShared/FBL/publ/Threads/FBL_Atomic_Op_T.h>
#include <VShared/FBL/publ/Threads/FBL_TLS_Var.h>

#if STD_THREADING_SAFE
	// STD:
	#include <atomic>
#endif // STD_THREADING_SAFE

/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
//typedef Old_CriticalSection						CriticalSection;
//typedef Old_StCriticalSectionLocker				StCriticalSectionLocker;
typedef True_Thread_Mutex_Recursive				CriticalSection;
typedef StLockGuard<CriticalSection>			StCriticalSectionLocker;


/**********************************************************************************************/
#if STD_THREADING_SAFE
	typedef std::atomic<bool>						Atomic_bool;
	typedef std::atomic<vint16>						Atomic_vint16;
	typedef std::atomic<vuint16>					Atomic_vuint16;
	typedef std::atomic<vint32>						Atomic_vint32;
	typedef std::atomic<vuint32>					Atomic_vuint32;
	typedef std::atomic<vuint64>					Atomic_vuint64;
#else
	typedef Atomic_Op<bool>							Atomic_bool;	// TODO: implement template for bool
	typedef Atomic_Op<vint16>						Atomic_vint16;
	typedef Atomic_Op<vuint16>						Atomic_vuint16;
	typedef Atomic_Op<vint32>						Atomic_vint32;
	typedef Atomic_Op<vuint32>						Atomic_vuint32;
	typedef Atomic_Op<vuint64>						Atomic_vuint64;
#endif // STD_THREADING_SAFE


/**********************************************************************************************/
FBL_SHARED_EXP CriticalSection& GetGlobalDiskLock( void );

FBL_SHARED_EXP True_Own_Thread_Mutex_Recursive& GetGlobalEngineLock( void );
FBL_SHARED_EXP True_Thread_Mutex_Recursive& GetLogFolderCreationLock( void );
FBL_SHARED_EXP True_Thread_Mutex_Recursive& GetLogTimeStampLock( void );
FBL_SHARED_EXP True_Thread_Mutex_Recursive& GetWarningLogLock( void );
FBL_SHARED_EXP True_Thread_Mutex_Recursive& GetTunesLogLock( void );
FBL_SHARED_EXP True_Thread_Mutex_Recursive& GetReportsLogLock( void );

#if __CHECK
FBL_SHARED_EXP True_Thread_Mutex_Recursive& GetAssertLogLock( void );
FBL_SHARED_EXP True_Thread_Mutex_Recursive& GetTestStatisticLock( void );
#endif // __CHECK

FBL_SHARED_EXP True_Thread_Mutex_Recursive& GetTaskManagerLock( void );
FBL_SHARED_EXP True_Thread_Mutex_Recursive& GetNotificationCenterLock( void );


#pragma mark -


/**********************************************************************************************/
FBL_SHARED_EXP extern bool gKernelInServerMode;
FBL_SHARED_EXP extern bool gGlobalThreadSafe;

/**********************************************************************************************/
/**	This class should be used as stack variable.  
	It performs automatic aquisition and release globalEngineLock if needed.
	Sure, we can use StLocalGuard<>(mutex&, condition) for this purpose, but this special class
	seems to be a little more efficient, because if dynamically threadsafe is OFF - the only 
	pitfall is - this stack object + two "if" (no need for getting actual mutex).
 */
class StGlobalLockGuard
{
	public://///////////////////////////////////////////////////////////////////////////////////

							StGlobalLockGuard()
							:
								mpGlobalEngineMutex(nullptr)
							{
								if( gKernelInServerMode || gGlobalThreadSafe )
									mpGlobalEngineMutex = &GetGlobalEngineLock();

								Acquire();
							}

							// Release the lock if it is locked.
							~StGlobalLockGuard( void )
							{
								Release();
							}


	public://///////////////////////////////////////////////////////////////////////////////////

							///	Acquire the lock.
		void				Acquire( void )
							{
								if( mpGlobalEngineMutex )
									mpGlobalEngineMutex->lock();
							}

							///	Release the lock.  
		void				Release( void )
							{
								if( mpGlobalEngineMutex )
									mpGlobalEngineMutex->unlock();
							}
	
	
	protected://////////////////////////////////////////////////////////////////////////////////

		True_Own_Thread_Mutex_Recursive*	mpGlobalEngineMutex;


	private:////////////////////////////////////////////////////////////////////////////////////
  

		StGlobalLockGuard&	operator= ( const StGlobalLockGuard& );
							StGlobalLockGuard( const StGlobalLockGuard& );
};


/**********************************************************************************************/
/**	This class should be used as stack variable.  
	It performs automatic release and aquisition globalEngineLock if needed.
 */
class StGlobalUnlockGuard
{
	public://///////////////////////////////////////////////////////////////////////////////////

							// Release the lock.
							StGlobalUnlockGuard()
							:
								mpGlobalEngineMutex(nullptr),
								mRecursionLevel(0)
							{
								if( gKernelInServerMode || gGlobalThreadSafe )
								{
									mpGlobalEngineMutex = &GetGlobalEngineLock();
									ReleaseAll();
								}
							}

							// Take the lock back if it was unlocked.
							~StGlobalUnlockGuard( void )
							{
								AcquireAll();
							}


	protected://////////////////////////////////////////////////////////////////////////////////

							///	Release the lock completelly.
							/// Tricky method for unlock the mutex (temporary).
							/// You should call AcquireAll() to lock the mutex again!!!
		void				ReleaseAll( void )
							{
								if( mpGlobalEngineMutex )
								{
									// We are sure that mutex is taken by this thread.
									mpGlobalEngineMutex->lock();
									mRecursionLevel = mpGlobalEngineMutex->get_NestingLevel() - 1;
									for( vuint32 i = 1; i <= mRecursionLevel; ++i )
										mpGlobalEngineMutex->unlock();
									mpGlobalEngineMutex->unlock();
								}
							}
	
							///	Acquire the lock to the level on ReleaseAll() call moment.
							/// Must be called after ReleaseAll() only!
		void				AcquireAll( void )
							{
								if( mpGlobalEngineMutex )
								{
									for( vuint32 i = 1; i <= mRecursionLevel; ++i )
										mpGlobalEngineMutex->lock();
								}
							}
	
	
	protected://////////////////////////////////////////////////////////////////////////////////

		True_Own_Thread_Mutex_Recursive*	mpGlobalEngineMutex;
		vuint32								mRecursionLevel;


	private:////////////////////////////////////////////////////////////////////////////////////
  

		StGlobalUnlockGuard&	operator= ( const StGlobalUnlockGuard& );
							StGlobalUnlockGuard( const StGlobalUnlockGuard& );
};


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif //_FBL_ThreadSafe_h
