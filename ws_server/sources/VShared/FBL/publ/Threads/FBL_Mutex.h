/**********************************************************************************************/
/* FBL_Mutex.h	                                                   							  */
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

#ifndef _FBL_Mutex_h
	#define _FBL_Mutex_h
#pragma once


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_Macros.h>
#include <VShared/FBL/publ/Threads/FBL_Lock_Guard_T.h>
#include <VShared/FBL/publ/Threads/thread_safety.h>

/**********************************************************************************************/
#if FBL_WIN
	#include <VShared/FBL/publ/Threads/Win/FBL_Condition_Win.h>
	#include <VShared/FBL/publ/Threads/Win/FBL_Thread_Mutex_Win.h>
#elif FBL_POSIX_API
	#include <VShared/FBL/publ/Threads/Posix/FBL_Condition_Posix.h>
	#include <VShared/FBL/publ/Threads/Posix/FBL_Thread_Mutex_Posix.h>
#else
	#error Platform not supported!
#endif // FBL_WIN

#include <VShared/FBL/publ/Threads/Null/FBL_Condition_Null.h>
#include <VShared/FBL/publ/Threads/Null/FBL_Thread_Mutex_Null.h>

#if STD_THREADING_SAFE
	// STD:
	#include <mutex>
	#include <condition_variable>
#endif // STD_THREADING_SAFE


/**********************************************************************************************/
FBL_Begin_Namespace

/**********************************************************************************************/
// "True_" means that algorithm can not proper work using "fake" mutex like Mutex_Null.
// (for example - TaskManager)
//

/**********************************************************************************************/
#if FBL_WIN
	typedef Thread_Mutex_Win					True_Own_Thread_Mutex;
	typedef Thread_Mutex_Win					True_Own_Thread_Mutex_Recursive;
	typedef Condition_Win<Thread_Mutex_Win>		True_Own_ConditionVar;
#elif FBL_POSIX_API
	typedef Thread_Mutex_Posix					True_Own_Thread_Mutex;
	typedef Thread_Mutex_Recursive_Posix		True_Own_Thread_Mutex_Recursive;
	typedef Condition_Posix<Thread_Mutex_Posix>	True_Own_ConditionVar;
#else
	#error Platform not supported!
#endif // FBL_WIN



/**********************************************************************************************/
#if STD_THREADING_SAFE

	typedef std::mutex							True_Thread_Mutex;
	typedef std::recursive_mutex				True_Thread_Mutex_Recursive;
	typedef std::condition_variable				True_ConditionVar;

#else

	typedef True_Own_Thread_Mutex				True_Thread_Mutex;
	typedef True_Own_Thread_Mutex_Recursive		True_Thread_Mutex_Recursive;
	typedef True_Own_ConditionVar				True_ConditionVar;

#endif // STD_THREADING_SAFE


/**********************************************************************************************/
#if FBL_INTERNAL_THREADSAFE

	#if STD_THREADING_SAFE
		typedef std::mutex							Thread_Mutex;
		typedef std::recursive_mutex				Thread_Mutex_Recursive;
		typedef std::condition_variable				ConditionVar;
	#else
		typedef True_Thread_Mutex					Thread_Mutex;
		typedef True_Thread_Mutex_Recursive			Thread_Mutex_Recursive;
		typedef True_ConditionVar					ConditionVar;
	#endif // STD_THREADING_SAFE

#else

	typedef Mutex_Null							Thread_Mutex;
	typedef Mutex_Null							Thread_Mutex_Recursive;
	typedef Condition_Null						ConditionVar;

#endif // FBL_INTERNAL_THREADSAFE



/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif //_FBL_Mutex_h
