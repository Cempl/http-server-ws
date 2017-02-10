/**********************************************************************************************/
/* FBL_ObjectPool.h																			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_ObjectPool_h
	#define _FBL_ObjectPool_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Util_classes/FBL_ToUChar.h>

// STD
#include <vector>

// POSIX:
#ifdef __linux__
#include <unistd.h>
#endif // __linux__

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace

/**********************************************************************************************/
// Global array of registered (ValentinaThreadInit was called) ThreadIDs
// to check anywhere that current thread is registered.
//
#if FBL_INTERNAL_THREADSAFE || FBL_EXTERNAL_THREADSAFE
	#if FBL_TEST_CODE
		FBL_SHARED_EXP extern ArrayOfUllong_Ptr	gThreadIDArray;
		FBL_SHARED_EXP extern True_Thread_Mutex	gThreadIDArrayMutex;
	#endif // FBL_TEST_CODE
#endif // FBL_INTERNAL_THREADSAFE || FBL_EXTERNAL_THREADSAFE

#pragma mark -

/**********************************************************************************************/
// This class is a pool of generic objects.
// It keeps up to mMaxPoolSize elements and it knows - is particular Object currently in use or not.
//
// ATTENTION!!!
// You MUST specify a couple of methods for EACH TYPE!
//	CreateNewObject_imp()
//	FreeObject_imp()
//
template<class T>
class ObjectPool
{
	private://///////////////////////////////////////////////////////////////////////////////////

		struct PooledObjectDesc
		{
			T*				mpObject;
			bool			mInUse;
		};
		

	public://///////////////////////////////////////////////////////////////////////////////////

	// API of singlenton

							// Access to Pool.
static ObjectPool<T>*		GetObjectPool( size_t inMaxPoolSize = 2 )
							{
								// Double-Checked Locking Pattern
								// http://www.aristeia.com/Papers/DDJ_Jul_Aug_2004_revised.pdf
								// C++11 - http://preshing.com/20130930/double-checked-locking-is-fixed-in-cpp11/
								//

								#if STD_THREADING_SAFE
									ObjectPool<T>* tmp = sObjectPool.load();
									if( tmp == nullptr )
									{
										StLockGuard<True_Thread_Mutex> g(sSingletonMutex);
										tmp = sObjectPool.load();
										if( tmp == nullptr )
										{
											tmp = new ObjectPool<T>(inMaxPoolSize);
											sObjectPool.store(tmp);
										}
									}
								
									return tmp;
								#else
									if( sObjectPool == nullptr )
									{
										StLockGuard<True_Thread_Mutex> g(sSingletonMutex);

										if( sObjectPool == nullptr )
										{
											// http://www.drdobbs.com/cpp/c-and-the-perils-of-double-checked-locki/184405726
											// Use tmp var to avoid second thread thinking that sGlobalProperties is not null
											// before construction is really finished in the current thread.
											// It is because "var = new type()" is not atomic.
											// There are 3 steps:
											// Step 1. Allocate memory to hold a Singleton object.
											// Step 2. Construct a Singleton object in the allocated memory.
											// Step 3. Make var point to the allocated memory.
											// Compilers are sometimes allowed to swap Steps 2 and 3.
											//
											ObjectPool<T>* volatile pTemp = new ObjectPool<T>(inMaxPoolSize);
											sObjectPool = pTemp;
										}
									}
										
									return sObjectPool;
								#endif // STD_THREADING_SAFE
							}
 	 	
static void					Cleanup( void )
							{
								ForgetObject( sObjectPool );
							}


	protected://////////////////////////////////////////////////////////////////////////////////

							ObjectPool( size_t inMaxPoolSize )
							:
								mMaxPoolSize( (inMaxPoolSize < sMinPoolSize) ? sMinPoolSize : inMaxPoolSize)
							{
							}
				
virtual 					~ObjectPool( void )
							{
								// Delete all objects in the pool.
								//
								
								// Manualy free ALL PooledObjectDesc::mpObjects here
								// (not individually in PooledObjectDesc destructor
								// because container (vector) can reallocate own elements
								// when adding new ones. So PooledObjectDesc destructor must not free
								// the PooledObjectDesc::mpObject).
								//
								size_t poolSize = mPool.size();
								for( size_t i = 0; i < poolSize; ++i )
								{
									FreeObject_imp( mPool[i].mpObject );
								}							
								
								mPool.clear();
							}


				
	public://///////////////////////////////////////////////////////////////////////////////////

							// Getting "not in-use" object from the pool.
							// If no "free" object and mMaxPoolSize reached - waits until "free"
							// object appears (never return NULL).
							//
		T*					CheckOutObject( void )
							{
								T* pRes = nullptr;
								
								#if FBL_INTERNAL_THREADSAFE || FBL_EXTERNAL_THREADSAFE
									#if FBL_TEST_CODE
										// Check that current thread was properly inited.
										//
										#if FBL_WIN
											vuint64 currentThreadID = (fbl::vuint64)GetCurrentThreadId();
										#else
											vuint64 currentThreadID = (fbl::vuint64)( pthread_self() );
										#endif //FBL_WIN
										
										argused1(currentThreadID);
//										{
//											// The problem is - in case of pooled localizableSys objects
//											// FBL_CHECK requires such an object as well -
//											// so we are falling to recursion if we call FBL_CHECK here.
//											StLockGuard<True_Thread_Mutex> lock(gThreadIDArrayMutex);
//											FBL_CHECK( gThreadIDArray && gThreadIDArray->Include( currentThreadID ) );
//										}
									#endif //FBL_TEST_CODE
								#endif // FBL_INTERNAL_THREADSAFE || FBL_EXTERNAL_THREADSAFE
								
								do
								{
									pRes = CheckOutObject_imp();								
									if( pRes == nullptr )
									{
										// We are going to wait for a "free object" because pool
										// is limited (can not create one more object) and all its
										// objects are currently "in-use".
										// Important! mMutex MUST NOT BE locked here!!!
										StLockGuard<True_Thread_Mutex> g( mMutex );
										mFreeObjectAvailable.wait(g);
									}

								} while( pRes == nullptr );

								return pRes;
							}

							// "Returning" object back to the pool (mark it as "not in-use").
							//
		void				CheckInObject( T* inpObject )
							{
								StLockGuard<True_Thread_Mutex> g(mMutex);
								
								size_t poolSize = mPool.size();
								for( size_t i = 0; i < poolSize; ++i )
								{
									if( mPool[i].mpObject == inpObject)
									{
										mPool[i].mInUse = false; // Make object "free"
										mFreeObjectAvailable.notify_one();
										break;
									}
								}
							}


	protected://////////////////////////////////////////////////////////////////////////////////

		T*					CheckOutObject_imp( void )
							{
								T* pRes = nullptr;
							
								StLockGuard<True_Thread_Mutex> g(mMutex);

								size_t poolSize = mPool.size();
								for( size_t i = 0; i < poolSize; ++i )
								{
									if(! mPool[i].mInUse )
									{
										mPool[i].mInUse = true; // Mark object in-use
										pRes = mPool[i].mpObject;
									}
								}
								
								// If no free objects found:
								if( pRes == nullptr && poolSize < mMaxPoolSize )
								{
									PooledObjectDesc objectDesc;
									objectDesc.mpObject = CreateNewObject_imp();
									objectDesc.mInUse   = true; // Mark object in-use

									mPool.push_back(objectDesc);
									pRes = objectDesc.mpObject;
								}
								
								return pRes; // Could be NULL if mMaxPoolSize objects are in-use.
							}

	protected://////////////////////////////////////////////////////////////////////////////////

		T*					CreateNewObject_imp( void )
							{
								// #error "You must specify CreateNewObject_imp() for this type"
								THROW_ERROR_INTERNAL_1(ERR_INTERNAL_ERROR,
									"You must specify CreateNewObject_imp() for this type");
								return NULL;
							}

		void				FreeObject_imp( T* inpObject )
							{
								argused1( inpObject );
								// #error "You must specify FreeObject_imp() for this type"								
								THROW_ERROR_INTERNAL_1(ERR_INTERNAL_ERROR,
									"You must specify FreeObject_imp() for this type");
							}
				
				
	protected://////////////////////////////////////////////////////////////////////////////////

	// static data:
	//
	
static	const size_t					sMinPoolSize;

		// Single pool per each pooled-object type approach (enough for now).
		//
		// Note:
		//   Probably we will need few different pools for the same-type objects (Something like
		//	 pool of UTF8_Converters, pool of Latin1_Converters, ...).
		//	 In this case we should define and use few pool-factories instead of
		//	 singleton-per-type approach.
		//
#if STD_THREADING_SAFE
static	std::atomic<ObjectPool<T>*>		sObjectPool;	 // Single pool-of-T-objects instance.
#else
static	ObjectPool<T>*	volatile		sObjectPool;	 // Single pool-of-T-objects instance.
#endif // STD_THREADING_SAFE
	
static	True_Thread_Mutex				sSingletonMutex; // Mutex for Pool static instance.


	// instanse data:
	//

mutable	True_Thread_Mutex				mMutex;			// Synch access inside of particular pool.

		True_ConditionVar				mFreeObjectAvailable;
		
		size_t							mMaxPoolSize;	// Pool size is limited.
		std::vector<PooledObjectDesc>	mPool;			// Pool storage.
		
};


#pragma mark -

/**********************************************************************************************/
//initialize static instance
//
template<class T> const size_t				ObjectPool<T>::sMinPoolSize 	= 2;
#if STD_THREADING_SAFE
template<class T> std::atomic<ObjectPool<T>*> 	ObjectPool<T>::sObjectPool;
#else
template<class T> ObjectPool<T>* volatile	ObjectPool<T>::sObjectPool		= nullptr;
#endif // STD_THREADING_SAFE

template<class T> True_Thread_Mutex			ObjectPool<T>::sSingletonMutex;


#pragma mark -

/**********************************************************************************************/
// This class must be used as a holder for pooled object.
// The main purpose is - to ask for pooled object in constructor and return it back to the pool
// in destructor (RAII patern).
// Due to the overloaded operators PoolObjectHolder might be used as pointer to managed object.
// Example:
//	typedef ObjectPool<SqlParser> SqlParserPool;
//	PoolObjectHolder<SqlParser> pParser( SqlParserPool::GetObjectPool() );
//	pParser->ParseSingleCommand(....);
//
template<class T>
class PoolObjectHolder:	public noncopyable
{
	public:////////////////////////////////////////////////////////////////////////////////////

							PoolObjectHolder( ObjectPool<T>* p )
							:
								mpObjectPool(p),
								mpObject(nullptr)
							{
								mpObject = mpObjectPool->CheckOutObject();
							}
 
							~PoolObjectHolder()
							{
								mpObjectPool->CheckInObject( mpObject );
							}


	public:////////////////////////////////////////////////////////////////////////////////////

							// operators to check for non-empty managed object.
							//
							operator bool() const
								{	return mpObject != NULL;	}

		bool				operator!() const
								{	return !mpObject;	}

							// operators to access the managed object.
							//
		T*					operator->()
								{	return mpObject;	}
				
		const T*			operator->() const
								{	return mpObject;	}
    
				
				
	private://///////////////////////////////////////////////////////////////////////////////////

        ObjectPool<T>*		mpObjectPool;

		T*					mpObject;
};


#pragma mark -

/**********************************************************************************************/
// TLS implementation for PoolObjectHolder object.
//
template<class T>
class TLS_Pooled_Var
{
	public://///////////////////////////////////////////////////////////////////////////////////

							TLS_Pooled_Var()
							{
							}


virtual						~TLS_Pooled_Var()
							{
								SafeRelease();
							}


	public://///////////////////////////////////////////////////////////////////////////////////

		void				SafeRelease( void )
							{
								PoolObjectHolder<T>* pObj = (PoolObjectHolder<T>*)mTLSVar.get();
								// Delete PoolObjectHolder<T> object - not T !!! to return
								// T-object back to the pool.
								ForgetObject( pObj );
								// No more valid PoolObjectHolder for the current thread.
								mTLSVar.set( nullptr );
							}

		void				set( PoolObjectHolder<T>* obj )
							{
								SafeRelease();
								mTLSVar.set( obj );
							}

		T*					get()
							{
								PoolObjectHolder<T>* pObj = (PoolObjectHolder<T>*)mTLSVar.get();
								return 	(pObj) ? pObj->operator->() : nullptr;
							}


	private:////////////////////////////////////////////////////////////////////////////////////

		TLS_Var				mTLSVar;	// Actually PoolObjectHolder<T>* in current thread.
};



/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_ObjectPool_h


