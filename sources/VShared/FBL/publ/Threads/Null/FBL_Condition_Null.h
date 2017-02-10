/**********************************************************************************************/
/* FBL_Condition_Null.h	                                                   					  */
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


#ifndef _FBL_Condition_Null_h
	#define _FBL_Condition_Null_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Threads/Null/FBL_Thread_Mutex_Null.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
/// No-op condition variable implementation.
///
class FBL_SHARED_EXP_CLASS Condition_Null
{
	public://///////////////////////////////////////////////////////////////////////////////////

							/// Initialize the condition variable.
							Condition_Null();

							/// Implicitly destroy the condition variable.
							~Condition_Null( void );

	// ----------------
	// Lock accessors
		bool				wait_for( StLockGuard<Mutex_Null>& inLock, vuint32 inTimeOutMS );

							/// Block on condition.
		void				wait( StLockGuard<Mutex_Null>& inLock );

							/// Signal one waiting thread.
		void				notify_one( void );

							/// Signal *all* waiting threads.
		void				notify_all( void );



	private:////////////////////////////////////////////////////////////////////////////////////
  
							/// Prevent assignment and initialization.
		Condition_Null&		operator=( const Condition_Null& );
							Condition_Null( const Condition_Null& );
};


/**********************************************************************************************/
inline
Condition_Null::Condition_Null()
{
}


/**********************************************************************************************/
inline
Condition_Null::~Condition_Null( void )
{
}


/**********************************************************************************************/
inline
bool Condition_Null::wait_for( StLockGuard<Mutex_Null>& inLock, vuint32 inTimeOutMS )
{
	argused2(inLock, inTimeOutMS);
	return true;
}


/**********************************************************************************************/
inline
void Condition_Null::wait( StLockGuard<Mutex_Null>& inLock )
{
	argused1( inLock );
	/* No-op */
}


/**********************************************************************************************/
inline
void Condition_Null::notify_one( void )
{
	/* No-op */
}


/**********************************************************************************************/
inline
void Condition_Null::notify_all( void )
{
	/* No-op */
}


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // _FBL_Condition_Null_h
