/**********************************************************************************************/
/* FBL_Lock_Guard_T.h	                                                   					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/
/**
 *	IMPORTANT NOTE: DO NOT include this file directly
 *		unless you know what you are doing!
 *	Include <VShared/FBL/publ/Threads/FBL_ThreadSafe.h> instead.
 */
	// and strong rule of this:
	#ifndef _FBL_ThreadSafe_h
		#error "Please include FBL_ThreadSafe.h instead of this header."
	#endif // _FBL_ThreadSafe_h


#ifndef _FBL_Lock_Guard_T_h
	#define _FBL_Lock_Guard_T_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL_Macros.h>
#include <VShared/FBL/publ/Threads/thread_safety.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
enum StLockPolicy
{
	kLPDefered,
	kLPImmediate,
	kLPImmediateForWrite,
	kLPImmediateForRead,
	kLPImmediateTry,
	kLPImmediateTryForWrite,
	kLPImmediateTryForRead
};


/**********************************************************************************************/
/**	This class shuold be used as stack variable.  
	It performs automatic aquisition and release of a parameterized synchronization object <LOCK>.
	
	The <LOCK> class given as an actual parameter must provide at
	the very least the <Acquire>, <TryAcquire>, <Release>, and <Remove> methods. 
 */
template <class LOCK>
class SCOPED_CAPABILITY StLockGuard_T
{
	public://///////////////////////////////////////////////////////////////////////////////////

							StLockGuard_T( LOCK &inLock, bool inLockRequired = true );

							/**	Acquire, "try to acquire" the lock or "do nothing"
								(depends on inBlockPolicy) */
							StLockGuard_T( LOCK &inLock, StLockPolicy inBlockPolicy );

							/// Release the lock if it is locked.
							~StLockGuard_T( void );


	// Lock accessors.
	// -------------------

							/// Explicitly acquire the lock.
		void				Acquire( void ) ACQUIRE();

							/// Explicitly acquires the write lock.
		void				AcquireWrite( void );

							/// Explicitly acquire the read lock.
		void				AcquireRead( void );


							/// Conditionally acquire the lock (i.e., won't block).
		bool				TryAcquire( void );

							/// Conditionally acquires the write lock (i.e., won't block).
		bool				TryAcquireWrite( void );

							/// Conditionally acquire the read lock (i.e., won't block).
		bool				TryAcquireRead( void );


							/// Explicitly release the lock, but only if it is held!
		void				Release( void ) RELEASE_();

		LOCK*				get_Lock( void );
	

	protected://////////////////////////////////////////////////////////////////////////////////


							/// Pointer to the LOCK we're guarding.
		LOCK*				mLock;

							/// Flag indicating whether guard should 
							/// release the lock.
		bool				mIsLocked;
		
		vuint8				mPad[3];	// to kill PAD warning.
		

	private:////////////////////////////////////////////////////////////////////////////////////
  

		StLockGuard_T<LOCK>&		operator= (const StLockGuard_T<LOCK> &);
							StLockGuard_T( const StLockGuard_T<LOCK> & );
};


/**********************************************************************************************/
template <class LOCK> inline
StLockGuard_T<LOCK>::StLockGuard_T( LOCK &inLock, bool inLockRequired )
:
	mLock(&inLock),
	mIsLocked(false)
{
	// Notes:
	// If you need some dynamic ON/OFF for threadsafe
	// You can do something like this
	// StLockGuard<Thread_Mutex> guard( mMutex, false );
	//
	if( inLockRequired )
		Acquire();
}


/**********************************************************************************************/
template <class LOCK> inline
StLockGuard_T<LOCK>::StLockGuard_T( LOCK &inLock, StLockPolicy inBlockPolicy )
:
	mLock(&inLock),
	mIsLocked(false)
{
	switch( inBlockPolicy )
	{
		case kLPDefered:										break;
		case kLPImmediate:					Acquire();			break;
		case kLPImmediateForWrite:			AcquireWrite();		break;
		case kLPImmediateForRead:			AcquireRead();		break;
		case kLPImmediateTry:				TryAcquire();		break;
		case kLPImmediateTryForWrite:		TryAcquireWrite();	break;
		case kLPImmediateTryForRead:		TryAcquireRead();	break;
		
		default:												break;
	}
}


/**********************************************************************************************/
template <class LOCK> inline
StLockGuard_T<LOCK>::~StLockGuard_T( void )
{
	Release();
}


/**********************************************************************************************/
template <class LOCK> inline
void StLockGuard_T<LOCK>::Acquire( void ) ACQUIRE()
{
	if( mIsLocked == false )
	{
		mLock->lock();
		mIsLocked = true;
	}
}


/**********************************************************************************************/
template <class LOCK> inline 
void StLockGuard_T<LOCK>::AcquireWrite( void )
{
	if( mIsLocked == false )
	{
		mLock->lock();
		mIsLocked = true;
	}
}


/**********************************************************************************************/
template <class LOCK> inline 
void StLockGuard_T<LOCK>::AcquireRead (void)
{
	if( mIsLocked == false )
	{
		mLock->lock();
		mIsLocked = true;
	}		
}


/**********************************************************************************************/
template <class LOCK> inline
bool StLockGuard_T<LOCK>::TryAcquire( void )
{
	if( mIsLocked == false )
		mIsLocked = mLock->try_lock();
	return mIsLocked;
}


/**********************************************************************************************/
template <class LOCK> inline 
bool StLockGuard_T<LOCK>::TryAcquireWrite( void )
{
	if( mIsLocked == false )
		mIsLocked = mLock->try_lock();
	return mIsLocked;
}


/**********************************************************************************************/
template <class LOCK> inline 
bool StLockGuard_T<LOCK>::TryAcquireRead( void )
{
	if( mIsLocked == false )
		mIsLocked = mLock->try_lock();
	return mIsLocked;		
}


/**********************************************************************************************/
template <class LOCK> inline
void StLockGuard_T<LOCK>::Release( void ) RELEASE_()
{
	if( mIsLocked )
	{
		mLock->unlock();
		mIsLocked = false;
	}
}


/**********************************************************************************************/
template <class LOCK> inline
LOCK* StLockGuard_T<LOCK>::get_Lock( void )
{
	return mLock;
}


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // _FBL_Lock_Guard_T_h
