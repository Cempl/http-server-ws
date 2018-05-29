/**********************************************************************************************/
/* FBL_Synch.h	                                                   							  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/
/**
 *	The main purpose of this file is to hide platform 
 *	differenses when working with multithreading.
 *
 *	Another purpose of this file is to include
 *	all minimum needed stuff to work with 
 *	multi-threading and synchronization with one step.
 *	
 *	So instead of writing:
 *		#if FBL_WIN
 *		#include <FBL_Thread_Win.h>
 *		...
 *		#else
 *		#include <FBL_Thread_Posix.h>
 *		...
 *		#endif // FBL_WIN
 *		...
 *	you should simply write:
 *		#include <FBL_Synch.h>
 */

#ifndef _FBL_Synch_h
	#define _FBL_Synch_h
#pragma once


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Threads/FBL_ThreadSafe.h>

/**********************************************************************************************/
#if FBL_MULTI_THREADED_ADVANCED
	#if FBL_WIN
		#	include <VShared/FBL/publ/Threads/Win/FBL_Thread_Win.h>
	#elif FBL_POSIX_API
		#	include <VShared/FBL/publ/Threads/Posix/FBL_Thread_Posix.h>
	#else
		#	error Platform not supported!
	#endif // FBL_WIN
#else
		#	include <VShared/FBL/publ/Threads/Null/FBL_Thread_Null.h>
#endif // FBL_MULTI_THREADED_ADVANCED



/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
#if FBL_MULTI_THREADED_ADVANCED
	#if FBL_WIN
		typedef Thread_Win							Thread_Advanced;
	#elif FBL_POSIX_API
		typedef Thread_Posix						Thread_Advanced;
	#else
		#	error Platform not supported!
	#endif // FBL_WIN
#else
		typedef Thread_Null							Thread_Advanced;
#endif // FBL_MULTI_THREADED_ADVANCED


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Threads/FBL_Lock_Guard_T.h>
#include <VShared/FBL/publ/Threads/FBL_Thread_Simple.h>


/**********************************************************************************************/
#endif //_FBL_Synch_h
