/**********************************************************************************************/
/* FBL_Atomic_Op_T.h	                                                   					  */
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


#ifndef _FBL_Atomic_Op_T_h
	#define _FBL_Atomic_Op_T_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Threads/FBL_Lock_Guard_T.h>
#include <VShared/FBL/publ/Threads/FBL_Mutex.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace

/**********************************************************************************************/
typedef True_Thread_Mutex	MUTEX;


/**********************************************************************************************/
/**	Atomic operations implementation.
	Defines the convenient and thread-safe 
	way to deal with operations on numbers 
	in multithreaded environment. */
template <class TYPE>
class Atomic_Op
{	
	public://///////////////////////////////////////////////////////////////////////////////////

							/// Initialize \a mValue to 0.
							Atomic_Op();

							/// Initialize \a mValue to c.
							Atomic_Op( const TYPE &inValue );
							
							/// Copy ctor.
							Atomic_Op( const Atomic_Op<TYPE>& );

							/// Atomically pre-increments \a mValue.
		TYPE				operator++( void );

							/// Atomically post-increments \a mValue.
		TYPE				operator++( int );

							/// Atomically increments \a mValue by rhs.
		TYPE				operator+=( const TYPE &rhs );

							/// Atomically pre-decrements \a mValue.
		TYPE				operator--( void );
		
							/// Atomically post-decrements \a mValue.
		TYPE				operator--( int );

							/// Atomically decrements \a mValue by rhs.
		TYPE				operator-=( const TYPE &rhs );

							/// Atomically compares \a mValue with rhs.
		bool				operator==( const TYPE &rhs ) const;

							/// Atomically compares \a mValue with rhs.
		bool				operator!=( const TYPE &rhs ) const;
		
							/// Atomically checks if \a mValue greater than or equal to rhs.
		bool				operator>=( const TYPE &rhs ) const;

							/// Atomically checks if \a mValue greater than rhs.
		bool				operator>( const TYPE &rhs ) const;

							/// Atomically checks if \a mValue less than or equal to rhs.
		bool				operator<=( const TYPE &rhs ) const;

							/// Atomically checks if \a mValue less than rhs.
		bool				operator<( const TYPE &rhs ) const;

							/// Atomically assigns rhs to \a mValue.
		Atomic_Op<TYPE>&	operator=( const TYPE &rhs );

							/// Atomically assigns <rhs> to \a mValue.
		Atomic_Op<TYPE>&	operator=( const Atomic_Op<TYPE> &rhs );

							/// Explicitly returns \a mValue.
		TYPE				load( void ) const;



	private:////////////////////////////////////////////////////////////////////////////////////


							/// Type of synchronization mechanism.
		mutable MUTEX		mMutex;

							/// Current object decorated by the atomic op.
		TYPE				mValue;
};


/**********************************************************************************************/
template <class TYPE> inline 
Atomic_Op<TYPE>::Atomic_Op()
: 
    mValue(0)
{}


/**********************************************************************************************/
template <class TYPE> inline
Atomic_Op<TYPE>::Atomic_Op( const TYPE &inValue )
: 
    mValue(inValue)
{}


/**********************************************************************************************/
template <class TYPE> inline
Atomic_Op<TYPE>::Atomic_Op( const Atomic_Op<TYPE> &rhs )
{
	*this = rhs; // Invoke the assignment operator.
}


/**********************************************************************************************/
template <class TYPE> inline 
TYPE Atomic_Op<TYPE>::operator++( void )
{
	StLockGuard<MUTEX> g(mMutex); 
	return ++mValue;
}


/**********************************************************************************************/
template <class TYPE> inline 
TYPE Atomic_Op<TYPE>::operator+=( const TYPE &rhs )
{
#ifdef _MSC_VER
#pragma warning( disable : 4244 )
#endif

	StLockGuard<MUTEX> g(mMutex);
	return static_cast<TYPE>(mValue += rhs);
}


/**********************************************************************************************/
template <class TYPE> inline 
TYPE Atomic_Op<TYPE>::operator--( void )
{
	StLockGuard<MUTEX> g(mMutex);
	return --mValue;
}


/**********************************************************************************************/
template <class TYPE> inline TYPE
Atomic_Op<TYPE>::operator-= (const TYPE &rhs)
{
#ifdef _MSC_VER
#pragma warning( disable : 4244 )
#endif

	StLockGuard<MUTEX> g(mMutex);
	return mValue -= rhs;
}


/**********************************************************************************************/
template <class TYPE> inline 
TYPE Atomic_Op<TYPE>::operator++ (int)
{
	StLockGuard<MUTEX> g(mMutex);
	return mValue++;
}


/**********************************************************************************************/
template <class TYPE> inline 
TYPE Atomic_Op<TYPE>::operator--( int )
{
	StLockGuard<MUTEX> g(mMutex);
	return mValue--;
}


/**********************************************************************************************/
template <class TYPE> inline 
bool Atomic_Op<TYPE>::operator==( const TYPE &rhs ) const
{
	StLockGuard<MUTEX> g(mMutex);
	return mValue == rhs;
}


/**********************************************************************************************/
template <class TYPE> inline 
bool Atomic_Op<TYPE>::operator!= (const TYPE &rhs) const
{
	return !(*this == rhs);
}


/**********************************************************************************************/
template <class TYPE> inline
bool Atomic_Op<TYPE>::operator>= (const TYPE &rhs) const
{
	StLockGuard<MUTEX> g(mMutex);
	return mValue >= rhs;
}


/**********************************************************************************************/
template <class TYPE> inline
bool Atomic_Op<TYPE>::operator> (const TYPE &rhs) const
{
	StLockGuard<MUTEX> g(mMutex);
	return mValue > rhs;
}


/**********************************************************************************************/
template <class TYPE> inline
bool Atomic_Op<TYPE>::operator<= (const TYPE &rhs) const
{
	StLockGuard<MUTEX> g(mMutex);
	return mValue <= rhs;
}


/**********************************************************************************************/
template <class TYPE> inline 
bool Atomic_Op<TYPE>::operator< (const TYPE &rhs) const
{
	StLockGuard<MUTEX> g(mMutex);
	return mValue < rhs;
}


/**********************************************************************************************/
template <class TYPE> inline 
Atomic_Op<TYPE>& Atomic_Op<TYPE>::operator=( const Atomic_Op<TYPE> &rhs )
{
	if (&rhs == this)
	{
		return *this; // Avoid deadlock...
	}

	StLockGuard<MUTEX> g(mMutex);
  
	// This will call Atomic_Op::TYPE(), which will ensure the value
	// of <rhs> is acquired atomically.
	mValue = rhs.load();

	return *this;
}


/**********************************************************************************************/
template <class TYPE> inline 
TYPE Atomic_Op<TYPE>::load( void ) const
{
	StLockGuard<MUTEX> g(mMutex);
	return mValue;
}


/**********************************************************************************************/
template <class TYPE> inline 
Atomic_Op<TYPE>& Atomic_Op<TYPE>::operator=( const TYPE &rhs )
{
	StLockGuard<MUTEX> g(mMutex);
	mValue = rhs;
	return *this;
}


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
// Include full specifications which are ignored mutexes but implement atomics in specific ways.
#if FBL_WIN 
	#include <VShared/FBL/publ/Threads/Win/FBL_Atomic_Op_Win.h>
# elif FBL_POSIX_API && FBL_INTEL_CPU
	#include <VShared/FBL/publ/Threads/Posix/FBL_Atomic_Op_Posix.h>
#endif //FBL_WIN


/**********************************************************************************************/
#endif // _FBL_Atomic_Op_T_h
