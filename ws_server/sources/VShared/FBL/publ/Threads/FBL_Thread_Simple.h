/**********************************************************************************************/
/* FBL_Thread_Simple.h		                                               					  */
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


#ifndef _FBL_Thread_Simple_h
	#define _FBL_Thread_Simple_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>

#if STD_THREADING_SAFE
	// STD:
	#include <thread>
#endif // STD_THREADING_SAFE

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// If user does not use our Thread_Simple wrapper to create own threads
// He MUST call following functions himself in his thread-functions to avoid memory leaks.
//
FBL_SHARED_EXP_CLASS void ValentinaThreadInit( void );
FBL_SHARED_EXP_CLASS void ValentinaThreadShutDown( void );

// Pointers to the vKernel Thread Init/Finish functions
// Will be called from vShared if set.
typedef void (*ValentinaThreadInitEx_PTR)( void );
typedef void (*ValentinaThreadShutDownEx_PTR)( void );

extern FBL_SHARED_EXP ValentinaThreadInitEx_PTR			pValentinaThreadInitEx;
extern FBL_SHARED_EXP ValentinaThreadShutDownEx_PTR 	pValentinaThreadShutDownEx;


/**********************************************************************************************/
// Valentina thread macroses
//
#if FBL_WIN 

	typedef unsigned (__stdcall *PTHREAD_START) (void *);
	#define FBL_THREAD_ROUTINE_TYPE		LPTHREAD_START_ROUTINE
	#define FBL_THREAD_ROUTINE(n, a)	DWORD WINAPI n( LPVOID a )

#else // FBL_WIN 

	typedef void* (* THREAD_FUNC) ( void* );
	#define FBL_THREAD_ROUTINE_TYPE		THREAD_FUNC
	#define FBL_THREAD_ROUTINE(n, a)	void* n( void* a )

#endif // FBL_WIN 


/**********************************************************************************************/
struct FBL_Thread_Args
{
							FBL_Thread_Args( void )
							:
								mpStartRoutine( nullptr ),
								mpArgs( nullptr )
							{
							}
				
				
	FBL_THREAD_ROUTINE_TYPE mpStartRoutine;
	void*					mpArgs;
};


/**********************************************************************************************/
// Simple thread class.
//
class FBL_SHARED_EXP_CLASS Thread_Simple
{
	public://///////////////////////////////////////////////////////////////////////////////////

							Thread_Simple( FBL_THREAD_ROUTINE_TYPE inpStartRoutine, void* inpArgs );
virtual						~Thread_Simple( void );

							//
							// This class is very simple - not copy/move able.
							// So you can not use it this way:
							// vector<Thread_Simple>...
							//
							// Because of windows implementation, where we should finally
							// call CloseHandle(). Otherwise, we need some "owner" logic inside
							// and passing that ownership on copy/move operations (call
							// CloseHandle() if this instance is "owner" only).
							//
							// If you need copy/move - use std::thread instead.
							// Note:	In this case, you MUST call
							//			ValentinaThreadInit()/ValentinaThreadShutDown() yourself
							//			in your thread-function to avoid memory leaks.
							//
	
							// Not copyable:
							Thread_Simple( const Thread_Simple& inThread ) = delete;
							Thread_Simple& operator=( const Thread_Simple& inThread ) = delete;

							// Not moveable:
							Thread_Simple( Thread_Simple&& inThread ) = delete;
							Thread_Simple& operator=( Thread_Simple&& inThread ) = delete;
	

	public://///////////////////////////////////////////////////////////////////////////////////

virtual	void				join( void );

virtual size_t				get_id( void )
		#if FBL_WIN 
								{	return reinterpret_cast<size_t>(mHandle); }
		#else
									// IN, 2013: commented due build errors on mac and linux
								{	return 0; /*static_cast<size_t>(mHandle);*/ }
		#endif //FBL_WIN 


	protected:////////////////////////////////////////////////////////////////////////////////// 

		void				Create(
								FBL_THREAD_ROUTINE_TYPE inpStartRoutine, 
								void* inpArgs );


	protected:////////////////////////////////////////////////////////////////////////////////// 

		#if FBL_WIN 
			HANDLE	mHandle;
		#else
			pthread_t mHandle;
		#endif //FBL_WIN
		
		FBL_Thread_Args*		mpThreadArgs;
};


/**********************************************************************************************/
#if STD_THREADING_SAFE
	typedef std::thread						FBL_Thread;
#else
	typedef Thread_Simple					FBL_Thread;
#endif // STD_THREADING_SAFE


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // _FBL_Thread_Simple_h
