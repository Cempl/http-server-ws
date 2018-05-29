/**********************************************************************************************/
/* FBL_Atomic_Op_Posix.h	                                           						  */
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


#ifndef _FBL_Atomic_Op_Posix_h
	#define _FBL_Atomic_Op_Posix_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL_Macros.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
#if FBL_POSIX_API == 0
#error This file can only be used for Posix builds!
#endif // FBL_POSIX_API


/**********************************************************************************************/
FBL_Begin_Namespace

/**********************************************************************************************/
#if FBL_INTEL_CPU

/**********************************************************************************************/
// Helper functions
/**********************************************************************************************/
FBL_SHARED_EXP long threadsafe_increment( volatile long* value );
FBL_SHARED_EXP long threadsafe_decrement( volatile long *value );
FBL_SHARED_EXP long threadsafe_exchange( volatile long *value, long rhs );
FBL_SHARED_EXP long threadsafe_exchange_add( volatile long *value, long rhs );


/**********************************************************************************************/
#endif // FBL_INTEL_CPU




/**********************************************************************************************/
/// Specialization of Atomic_Op that support atomic integer operations for Posix.
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
							// long (not vint32) - because algorithms require long type
							// 32bit length for 32bit-process and 64bit long for 64bit-process.
							// Meanwhile this class looks as vint32 for callers, just internal
							// storage differs.
		volatile long		mValue;
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
	#if FBL_MAC
		return (vint32) threadsafe_increment( const_cast<long*>(reinterpret_cast<volatile long*>(&mValue)) );
	#else
		return  __sync_add_and_fetch( (long*) &mValue, 1);
	#endif // FBL_MAC
}


/**********************************************************************************************/
inline vint32 Atomic_Op<vint32>::operator++( int )
{
	return ++*this - 1;
}


/**********************************************************************************************/
inline vint32 Atomic_Op<vint32>::operator--( void )
{
	#if FBL_MAC
		return (vint32) threadsafe_decrement( const_cast<long*>(reinterpret_cast<volatile long*>(&mValue)) );
	#else
		return __sync_sub_and_fetch( (long*) &mValue, 1 );
	#endif // FBL_MAC
}


/**********************************************************************************************/
inline vint32 Atomic_Op<vint32>::operator--( int )
{
	return --*this + 1;
}


/**********************************************************************************************/
inline vint32 Atomic_Op<vint32>::operator+=( vint32 rhs )
{
	#if FBL_MAC
		// Trick: threadsafe_exchange_add() returns "old" value - so we add rhs one more to return
		// proper result BUT we do it against local copies of variables - so it is thread-safe.
		return static_cast<vint32>(
							threadsafe_exchange_add(
								const_cast<long*>(reinterpret_cast<volatile long*>(&mValue)),
								(long)rhs )
							+ (long)rhs );
	#else
		return __sync_add_and_fetch( (long*) &mValue, rhs );
	#endif // FBL_MAC		
}


/**********************************************************************************************/
inline vint32 Atomic_Op<vint32>::operator-=( vint32 rhs )
{
	#if FBL_MAC
		// Trick: threadsafe_exchange_add() returns "old" value - so we add rhs one more to return
		// proper result BUT we do it against local copies of variables - so it is thread-safe.
		return static_cast<vint32>(
							threadsafe_exchange_add(
								const_cast<long*>(reinterpret_cast<volatile long*>(&mValue)),
								(long)-rhs )
							- (long)rhs );
	#else
		return __sync_sub_and_fetch( (long*) &mValue, rhs );
	#endif // FBL_MAC		
}

/**********************************************************************************************/
inline bool Atomic_Op<vint32>::operator==( vint32 rhs ) const
{
	return static_cast<vint32>(mValue) == rhs;
}

/**********************************************************************************************/
inline bool Atomic_Op<vint32>::operator!=( vint32 rhs ) const
{
	return static_cast<vint32>(mValue) != rhs;
}


/**********************************************************************************************/
inline bool Atomic_Op<vint32>::operator>= ( vint32 rhs ) const
{
	return static_cast<vint32>(mValue) >= rhs;
}


/**********************************************************************************************/
inline bool Atomic_Op<vint32>::operator> ( vint32 rhs ) const
{
	return static_cast<vint32>(mValue) > rhs;
}


/**********************************************************************************************/
inline bool Atomic_Op<vint32>::operator<= ( vint32 rhs ) const
{
	return static_cast<vint32>(mValue) <= rhs;
}


/**********************************************************************************************/
inline bool Atomic_Op<vint32>::operator< ( vint32 rhs ) const
{
	return static_cast<vint32>(mValue) < rhs;
}


/**********************************************************************************************/
inline Atomic_Op<vint32>& Atomic_Op<vint32>::operator= ( vint32 rhs )
{
	threadsafe_exchange( const_cast<long*>(reinterpret_cast<volatile long*>(&mValue)), rhs );
	return *this;
}


/**********************************************************************************************/
inline Atomic_Op<vint32>& Atomic_Op<vint32>::operator= ( const Atomic_Op<vint32> &rhs )
{
	threadsafe_exchange( const_cast<long*>(reinterpret_cast<volatile long*>(&mValue)), rhs.mValue );	
	return *this;
}


/**********************************************************************************************/
inline vint32 Atomic_Op<vint32>::load( void ) const
{
	return static_cast<vint32>(mValue);
}


#pragma mark -

/**********************************************************************************************/
/// Specialization of Atomic_Op that support atomic unsigned integer operations for Posix.
//
template<> 
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
							// long (not vuint32) - because algorithms require long type
							// 32bit length for 32bit-process and 64bit long for 64bit-process.
							// Meanwhile this class looks as vuint32 for callers, just internal
							// storage differs.
		volatile long		mValue;
};


/**********************************************************************************************/
inline Atomic_Op<vuint32>::Atomic_Op( void )
: 
	mValue(0)
{}


/**********************************************************************************************/
inline Atomic_Op<vuint32>::Atomic_Op( vuint32 inValue )
: 
	mValue( (volatile long)inValue )
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
	#if FBL_MAC
		return static_cast<vuint32>( threadsafe_increment( reinterpret_cast<volatile long*>(&mValue) ) );
	#else
		return  __sync_add_and_fetch( (unsigned long*) &mValue, 1);
	#endif // FBL_MAC
}


/**********************************************************************************************/
inline vuint32 Atomic_Op<vuint32>::operator++( int )
{
	return ++*this - 1;
}


/**********************************************************************************************/
inline vuint32 Atomic_Op<vuint32>::operator--( void )
{
	#if FBL_MAC
		return static_cast<vuint32>( threadsafe_decrement( reinterpret_cast<volatile long*>(&mValue) ) );
	#else
		return __sync_sub_and_fetch( (unsigned long*) &mValue, 1 );
	#endif // FBL_MAC
}


/**********************************************************************************************/
inline vuint32 Atomic_Op<vuint32>::operator--( int )
{
	return --*this + 1;
}


/**********************************************************************************************/
inline vuint32 Atomic_Op<vuint32>::operator+=( vuint32 rhs )
{
	#if FBL_MAC
		// Trick: threadsafe_exchange_add() returns "old" value - so we add rhs one more to return
		// proper result BUT we do it against local copies of variables - so it is thread-safe.
		return static_cast<vuint32>(
							threadsafe_exchange_add(
								reinterpret_cast<volatile long*>(&mValue),
								(long)rhs )
							+ (long)rhs );
	#else
		return __sync_add_and_fetch( (unsigned long*) &mValue, rhs );
	#endif // FBL_MAC		
}


/**********************************************************************************************/
inline vuint32 Atomic_Op<vuint32>::operator-=( vuint32 rhs )
{
	#if FBL_MAC
		// Trick: threadsafe_exchange_add() returns "old" value - so we add rhs one more to return
		// proper result BUT we do it against local copies of variables - so it is thread-safe.
		return static_cast<vuint32>(
							threadsafe_exchange_add(
								reinterpret_cast<volatile long*>(&mValue),
								(long)-rhs )
							- (long)rhs );
	#else
		return __sync_sub_and_fetch( (unsigned long*) &mValue, rhs );
	#endif // FBL_MAC		
}

/**********************************************************************************************/
inline bool Atomic_Op<vuint32>::operator==( vuint32 rhs ) const
{
	return static_cast<vuint32>(mValue) == rhs;
}

/**********************************************************************************************/
inline bool Atomic_Op<vuint32>::operator!=( vuint32 rhs ) const
{
	return static_cast<vuint32>(mValue) != rhs;
}


/**********************************************************************************************/
inline bool Atomic_Op<vuint32>::operator>= ( vuint32 rhs ) const
{
	return static_cast<vuint32>(mValue) >= rhs;
}


/**********************************************************************************************/
inline bool Atomic_Op<vuint32>::operator> ( vuint32 rhs ) const
{
	return static_cast<vuint32>(mValue) > rhs;
}


/**********************************************************************************************/
inline bool Atomic_Op<vuint32>::operator<= ( vuint32 rhs ) const
{
	return static_cast<vuint32>(mValue) <= rhs;
}


/**********************************************************************************************/
inline bool Atomic_Op<vuint32>::operator< ( vuint32 rhs ) const
{
	return static_cast<vuint32>(mValue) < rhs;
}


/**********************************************************************************************/
inline Atomic_Op<vuint32>& Atomic_Op<vuint32>::operator= ( vuint32 rhs )
{
	threadsafe_exchange( reinterpret_cast<volatile long*>(&mValue), (long)rhs );
	return *this;
}


/**********************************************************************************************/
inline Atomic_Op<vuint32>& Atomic_Op<vuint32>::operator= ( const Atomic_Op<vuint32> &rhs )
{
	threadsafe_exchange( reinterpret_cast<volatile long*>(&mValue), (long)rhs.mValue );
	return *this;
}


/**********************************************************************************************/
inline vuint32 Atomic_Op<vuint32>::load( void ) const
{
	return static_cast<vuint32>(mValue);
}


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // _FBL_Atomic_Op_Posix_h
