/**********************************************************************************************/
/* FBL_CriticalSection.h                                                  					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
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


#ifndef _FBL_CriticalSection_h 
	#define _FBL_CriticalSection_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>

#pragma mark -
#pragma mark ====== PTHREADS ======= 

#if __GNUC__

#include <pthread.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
class CriticalSection_Posix
{
	public://///////////////////////////////////////////////////////////////////////////////////
		
FBL_FORCE_INLINE			CriticalSection_Posix()
							{
								pthread_mutexattr_init(&mAttr);
								pthread_mutexattr_settype(&mAttr, PTHREAD_MUTEX_RECURSIVE);
								pthread_mutex_init( &mSection, &mAttr );					
							}
							
FBL_FORCE_INLINE			~CriticalSection_Posix()
							{
								pthread_mutex_destroy( &mSection );
								pthread_mutexattr_destroy(&mAttr);
							}
							
FBL_FORCE_INLINE void		Enter( void )
							{
								pthread_mutex_lock( &mSection );
							}
	
FBL_FORCE_INLINE void		Leave( void )
							{
								pthread_mutex_unlock( &mSection );
							}

FBL_FORCE_INLINE bool		TryEnter( void )
							{
								// POSIX returns ZERO on success.
								return pthread_mutex_trylock( &mSection ) == 0;
							}

	protected:////////////////////////////////////////////////////////////////////////////////// 
	
		pthread_mutex_t		mSection;
		pthread_mutexattr_t mAttr;
} ;

#endif // __GNUC__


#pragma mark -
#pragma mark ====== PTHREADS ======= 

#if FBL_WIN

/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
class CriticalSection_Win
{
	public://///////////////////////////////////////////////////////////////////////////////////
	
FBL_FORCE_INLINE			CriticalSection_Win()
							{
								InitializeCriticalSection( &mSection );					
							}

FBL_FORCE_INLINE			~CriticalSection_Win()
							{
								DeleteCriticalSection( &mSection );
							}

FBL_FORCE_INLINE void		Enter( void )
							{
								EnterCriticalSection( &mSection );
							}

FBL_FORCE_INLINE void		Leave( void )
							{
								LeaveCriticalSection( &mSection );
							}

FBL_FORCE_INLINE bool		TryEnter( void )
							{
								// WIN funcs return ZERO if not success, i.e. CS is busy by other thread
								// We convert ZERO to false.
								//return TryEnterCriticalSection( &mSection ) != 0;
								return false;
							}

	protected:////////////////////////////////////////////////////////////////////////////////// 
	
		CRITICAL_SECTION	mSection;
} ;

#endif // FBL_WIN


/**********************************************************************************************/
class CriticalSection_Null
{
	public://///////////////////////////////////////////////////////////////////////////////////
	
FBL_FORCE_INLINE			CriticalSection_Null()
							{
							}

FBL_FORCE_INLINE			~CriticalSection_Null()
							{
							}

FBL_FORCE_INLINE void		Enter( void )
							{
							}

FBL_FORCE_INLINE void		Leave( void )
							{
							}

FBL_FORCE_INLINE bool		TryEnter( void )
							{
								return false;
							}
} ;


#pragma mark -
#pragma mark ====== StCriticalSectionLocker ======= 


/**********************************************************************************************/
template<class T>
class StCriticalSectionLockerT
{	
	public://///////////////////////////////////////////////////////////////////////////////////

FBL_FORCE_INLINE			StCriticalSectionLockerT( T& inSection, bool inLockRequired = true )
							:
								mSection( inSection ),
								mIsLocked( false )
							{
								if( inLockRequired )
								{
									mSection.Enter();
									mIsLocked = true;
								}
							}
							
FBL_FORCE_INLINE			~StCriticalSectionLockerT( void )
							{
								if( mIsLocked )
									mSection.Leave();
							}

	protected:////////////////////////////////////////////////////////////////////////////////// 
		
		T&					mSection;
		bool				mIsLocked;
};


#pragma mark -

/**********************************************************************************************/
#if FBL_WIN
	typedef CriticalSection_Win					Old_CriticalSection;
#elif FBL_POSIX_API
	typedef CriticalSection_Posix				Old_CriticalSection;
#else
	#error Platform not supported!
#endif // FBL_WIN


typedef StCriticalSectionLockerT<Old_CriticalSection>	Old_StCriticalSectionLocker;


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // _FBL_CriticalSection_h
