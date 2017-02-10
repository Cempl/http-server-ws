/**********************************************************************************************/
/* FBL_Atomic_Op_Win.h	                                               						  */
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


#ifndef _FBL_Atomic_Op_Win_h
	#define _FBL_Atomic_Op_Win_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL_Macros.h>


// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
#if FBL_WIN == 0
#error This file can only be used for Windows builds!
#endif // FBL_WIN


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
/// Specialization of Atomic_Op that support atomic integer operations for Windows.
//
template <>
class Atomic_Op<vint32>
{
	public://///////////////////////////////////////////////////////////////////////////////////


							/// Initialize \a mValue to 0.
							Atomic_Op( void );

							/// Initialize \a mValue to c.
							Atomic_Op( vint32 c );

							/// Manage copying...
							Atomic_Op( const Atomic_Op<vint32> &c );

							/// Atomically pre-increment \a mValue.
		vint32				operator++ ( void );

							/// Atomically post-increment \a mValue.
		vint32				operator++ ( int );

							/// Atomically increment \a mValue by rhs.
		vint32				operator+= ( vint32 rhs );

							/// Atomically pre-decrement \a mValue.
		vint32				operator-- ( void );

							/// Atomically post-decrement \a mValue.
		vint32				operator-- ( int );

							/// Atomically decrement \a mValue by rhs.
		vint32				operator-= ( vint32 rhs );

							/// Atomically compare \a mValue with rhs.
		bool				operator== ( vint32 rhs ) const;

							/// Atomically compare \a mValue with rhs.
		bool				operator!= ( vint32 rhs ) const;

							/// Atomically check if \a mValue greater than or equal to rhs.
		bool				operator>= ( vint32 rhs ) const;

							/// Atomically check if \a mValue greater than rhs.
		bool				operator> ( vint32 rhs ) const;

							/// Atomically check if \a mValue less than or equal to rhs.
		bool				operator<= ( vint32 rhs ) const;


							/// Atomically check if \a mValue less than rhs.
		bool				operator< ( vint32 rhs ) const;

							/// Atomically assign \a rhs to \a mValue.
		 Atomic_Op<vint32>&
							operator=( vint32 rhs );


							/// Atomically assign \a rhs to \a mValue.
		 Atomic_Op<vint32>&
							operator=( const Atomic_Op<vint32> &rhs );


							/// Explicitly return <mValue>.
		vint32				load( void ) const;



	private:////////////////////////////////////////////////////////////////////////////////////

  
							// Current object decorated by the atomic op.
		volatile vint32		mValue;
};


/**********************************************************************************************/
inline Atomic_Op<vint32>::Atomic_Op( void )
: 
	mValue(0)
{}


/**********************************************************************************************/
inline Atomic_Op<vint32>::Atomic_Op( vint32 inValue )
: 
	mValue(inValue)
{
}


/**********************************************************************************************/
inline Atomic_Op<vint32>::Atomic_Op( const Atomic_Op<vint32> &rhs )
: 
	mValue (rhs.mValue)
{}


/**********************************************************************************************/
inline vint32 Atomic_Op<vint32>::operator++( void )
{
	return ::InterlockedIncrement( (long*) &mValue);
}


/**********************************************************************************************/
inline vint32 Atomic_Op<vint32>::operator++( int )
{
	return ::InterlockedIncrement( (long*) &mValue ) - 1;
}


/**********************************************************************************************/
inline vint32 Atomic_Op<vint32>::operator--( void )
{
	return ::InterlockedDecrement( (long*) &mValue );
}


/**********************************************************************************************/
inline vint32 Atomic_Op<vint32>::operator--( int )
{
	return ::InterlockedDecrement( (long*) &mValue ) + 1;
}


/**********************************************************************************************/
inline vint32 Atomic_Op<vint32>::operator+=( vint32 rhs )
{
	return ::InterlockedExchangeAdd( (long*) &mValue, rhs ) + rhs;
}


/**********************************************************************************************/
inline vint32 Atomic_Op<vint32>::operator-=( vint32 rhs )
{
	return ::InterlockedExchangeAdd( (long*) &mValue, -rhs ) - rhs;
}

/**********************************************************************************************/
inline bool Atomic_Op<vint32>::operator==( vint32 rhs ) const
{
	return mValue == rhs;
}

/**********************************************************************************************/
inline bool Atomic_Op<vint32>::operator!=( vint32 rhs ) const
{
	return mValue != rhs;
}


/**********************************************************************************************/
inline bool Atomic_Op<vint32>::operator>= ( vint32 rhs ) const
{
	return mValue >= rhs;
}


/**********************************************************************************************/
inline bool Atomic_Op<vint32>::operator> ( vint32 rhs ) const
{
	return mValue > rhs;
}


/**********************************************************************************************/
inline bool Atomic_Op<vint32>::operator<= ( vint32 rhs ) const
{
	return mValue <= rhs;
}


/**********************************************************************************************/
inline bool Atomic_Op<vint32>::operator< ( vint32 rhs ) const
{
	return mValue < rhs;
}


/**********************************************************************************************/
inline Atomic_Op<vint32>& Atomic_Op<vint32>::operator= ( vint32 rhs )
{
	::InterlockedExchange( const_cast<long*>(reinterpret_cast<volatile long*>(&mValue)), rhs );	
	return *this;
}


/**********************************************************************************************/
inline Atomic_Op<vint32>& Atomic_Op<vint32>::operator= ( const Atomic_Op<vint32> &rhs )
{
	::InterlockedExchange( const_cast<long*>(reinterpret_cast<volatile long*>(&mValue)), rhs.mValue);	
	return *this;
}


/**********************************************************************************************/
inline vint32 Atomic_Op<vint32>::load( void ) const
{
	return mValue;
}


#pragma mark -

/**********************************************************************************************/
/// Full specialization of Atomic_Op that support atomic unsigned integer operations for Windows.
//
template <>
class Atomic_Op<vuint32>
{
	public://///////////////////////////////////////////////////////////////////////////////////


							/// Initialize \a mValue to 0.
							Atomic_Op( void );

							/// Initialize \a mValue to c.
							Atomic_Op( vuint32 c );

							/// Manage copying...
							Atomic_Op( const Atomic_Op<vuint32> &c );

							/// Atomically pre-increment \a mValue.
		vuint32				operator++ ( void );

							/// Atomically post-increment \a mValue.
		vuint32				operator++ ( int );

							/// Atomically increment \a mValue by rhs.
		vuint32				operator+= ( vuint32 rhs );

							/// Atomically pre-decrement \a mValue.
		vuint32				operator-- ( void );

							/// Atomically post-decrement \a mValue.
		vuint32				operator-- ( int );

							/// Atomically decrement \a mValue by rhs.
		vuint32				operator-= ( vuint32 rhs );

							/// Atomically compare \a mValue with rhs.
		bool				operator== ( vuint32 rhs ) const;

							/// Atomically compare \a mValue with rhs.
		bool				operator!= ( vuint32 rhs ) const;

							/// Atomically check if \a mValue greater than or equal to rhs.
		bool				operator>= ( vuint32 rhs ) const;

							/// Atomically check if \a mValue greater than rhs.
		bool				operator> ( vuint32 rhs ) const;

							/// Atomically check if \a mValue less than or equal to rhs.
		bool				operator<= ( vuint32 rhs ) const;


							/// Atomically check if \a mValue less than rhs.
		bool				operator< ( vuint32 rhs ) const;

							/// Atomically assign \a rhs to \a mValue.
		 Atomic_Op<vuint32>&
							operator=( vuint32 rhs );


							/// Atomically assign \a rhs to \a mValue.
		 Atomic_Op<vuint32>&
							operator=( const Atomic_Op<vuint32> &rhs );


							/// Explicitly return <mValue>.
		vuint32				load( void ) const;



	private:////////////////////////////////////////////////////////////////////////////////////

  
							// Current object decorated by the atomic op.
		volatile vuint32		mValue;
};


/**********************************************************************************************/
inline Atomic_Op<vuint32>::Atomic_Op( void )
: 
	mValue(0)
{}


/**********************************************************************************************/
inline Atomic_Op<vuint32>::Atomic_Op( vuint32 inValue )
: 
	mValue(inValue)
{
}


/**********************************************************************************************/
inline Atomic_Op<vuint32>::Atomic_Op( const Atomic_Op<vuint32> &rhs )
: 
	mValue (rhs.mValue)
{}


/**********************************************************************************************/
inline vuint32 Atomic_Op<vuint32>::operator++( void )
{
	return static_cast<vuint32>(
				::InterlockedIncrement(
					const_cast<long*>(reinterpret_cast<volatile long*>(&mValue))));
}


/**********************************************************************************************/
inline vuint32 Atomic_Op<vuint32>::operator++( int )
{
	return ++*this - 1;
}


/**********************************************************************************************/
inline vuint32 Atomic_Op<vuint32>::operator--( void )
{
	return static_cast<vuint32>(
				::InterlockedDecrement(
					const_cast<long*>(reinterpret_cast<volatile long*>(&mValue))));
}


/**********************************************************************************************/
inline vuint32 Atomic_Op<vuint32>::operator--( int )
{
	return --*this + 1;
}


/**********************************************************************************************/
inline vuint32 Atomic_Op<vuint32>::operator+=( vuint32 rhs )
{
  return static_cast<vuint32>(
				::InterlockedExchangeAdd(
					const_cast<long*>(reinterpret_cast<volatile long*>(&mValue)), rhs)) + rhs;
}


/**********************************************************************************************/
inline vuint32 Atomic_Op<vuint32>::operator-=( vuint32 rhs )
{
	return static_cast<vuint32>(
				::InterlockedExchangeAdd(
					const_cast<long*>(reinterpret_cast<volatile long*>(&mValue)), 
					-static_cast<long>(rhs))) - rhs;
}

/**********************************************************************************************/
inline bool Atomic_Op<vuint32>::operator==( vuint32 rhs ) const
{
	return mValue == rhs;
}

/**********************************************************************************************/
inline bool Atomic_Op<vuint32>::operator!=( vuint32 rhs ) const
{
	return mValue != rhs;
}


/**********************************************************************************************/
inline bool Atomic_Op<vuint32>::operator>= ( vuint32 rhs ) const
{
	return mValue >= rhs;
}


/**********************************************************************************************/
inline bool Atomic_Op<vuint32>::operator> ( vuint32 rhs ) const
{
	return mValue > rhs;
}


/**********************************************************************************************/
inline bool Atomic_Op<vuint32>::operator<= ( vuint32 rhs ) const
{
	return mValue <= rhs;
}


/**********************************************************************************************/
inline bool Atomic_Op<vuint32>::operator< ( vuint32 rhs ) const
{
	return mValue < rhs;
}


/**********************************************************************************************/
inline Atomic_Op<vuint32>& Atomic_Op<vuint32>::operator= ( vuint32 rhs )
{
	::InterlockedExchange( const_cast<long*>( reinterpret_cast<volatile long*>(&mValue) ), rhs);
	return *this;
}


/**********************************************************************************************/
inline Atomic_Op<vuint32>& Atomic_Op<vuint32>::operator= ( const Atomic_Op<vuint32> &rhs )
{
	::InterlockedExchange( const_cast<long*>( reinterpret_cast<volatile long*>(&mValue) ), rhs.mValue);
	return *this;
}


/**********************************************************************************************/
inline vuint32 Atomic_Op<vuint32>::load( void ) const
{
	return mValue;
}


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // _FBL_Atomic_Op_Win_h
