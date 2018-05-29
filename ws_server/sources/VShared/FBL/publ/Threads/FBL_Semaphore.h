/**********************************************************************************************/
/* FBL_Semaphore.h	                                               							  */
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

#ifndef _FBL_Semaphore_h
	#define _FBL_Semaphore_h
#pragma once


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_Macros.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>

#if FBL_WIN

	#include <limits.h>
	#define FBL_SEMAPHORE			HANDLE

#elif FBL_MAC

	#include <mach/mach.h>
	#include <mach/task.h>
	#include <mach/semaphore.h>

	#define FBL_SEMAPHORE			semaphore_t
	
#else

	#include <pthread.h>
	#include <semaphore.h>
	#define FBL_SEMAPHORE			sem_t

#endif // FBL_WIN


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
class FBL_SHARED_EXP_CLASS Semaphore
{
	public://///////////////////////////////////////////////////////////////////////////////////

							/// Initialize the semaphore.
							Semaphore( int inValue );

							~Semaphore( void );

	public://///////////////////////////////////////////////////////////////////////////////////

							/// Block semaphore.
		void				Wait( void );

							// Release semaphore.
		void				Post( void );

	private:////////////////////////////////////////////////////////////////////////////////////

		FBL_SEMAPHORE		mSemaphore;
};


/**********************************************************************************************/
inline Semaphore::Semaphore( int inValue )
{
	#if FBL_WIN
		mSemaphore = ::CreateSemaphoreW( NULL, inValue, LONG_MAX, 0x00 );
	#elif FBL_MAC
		int res = semaphore_create( mach_task_self(), &mSemaphore, SYNC_POLICY_FIFO, inValue );
		argused1(res);
	#else
		int res = sem_init( &mSemaphore, 0, (u_int)inValue );
		argused1(res);
	#endif // FBL_WIN
}


/**********************************************************************************************/
inline Semaphore::~Semaphore( void )
{
	#if FBL_WIN
		::CloseHandle( mSemaphore );
	#elif FBL_MAC
		semaphore_destroy( mach_task_self(), mSemaphore );
	#else
		int res = sem_destroy( &mSemaphore );
		argused1(res);
	#endif // FBL_WIN
}


/**********************************************************************************************/
inline void Semaphore::Wait( void )
{
	#if FBL_WIN
		DWORD result = ::WaitForSingleObject( mSemaphore, INFINITE );
		argused1(result);
	#elif FBL_MAC
		semaphore_wait( mSemaphore );
	#else
		sem_wait( &mSemaphore );
	#endif // FBL_WIN
}


/**********************************************************************************************/
inline void Semaphore::Post( void )
{
	#if FBL_WIN
		::ReleaseSemaphore( mSemaphore, 1, 0 );
	#elif FBL_MAC
		semaphore_signal( mSemaphore );
	#else
		int res = sem_post( &mSemaphore );
		argused1(res);
	#endif // FBL_WIN
}


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif //_FBL_Semaphore_h
