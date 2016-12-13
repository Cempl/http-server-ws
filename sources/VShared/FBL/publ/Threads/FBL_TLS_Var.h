/**********************************************************************************************/
/* FBL_TLS_Var.h			                                             					  */
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


#ifndef _FBL_TLS_Var_h
	#define _FBL_TLS_Var_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL_Macros.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
#if FBL_WIN
#include <windows.h>
#else
#include <pthread.h>
#endif // FBL_WIN


/**********************************************************************************************/
FBL_Begin_Namespace

/**********************************************************************************************/
//	TLS implementation.
//
class TLS_Var
{
	public://///////////////////////////////////////////////////////////////////////////////////

							TLS_Var()
							{
#if FBL_WIN	
								tls_index = TlsAlloc();
								//FBL_CHECK( tls_index != TLS_OUT_OF_INDEXES );
#else
								pthread_key_create( &key, nullptr );
#endif // FBL_WIN
							}


virtual						~TLS_Var()
							{
#if FBL_WIN	
								TlsFree( tls_index );
#else
								pthread_key_delete( key );
#endif // FBL_WIN
							}

	private:////////////////////////////////////////////////////////////////////////////////////

							// not copyable!
							TLS_Var( const TLS_Var& inExc );
        const TLS_Var&		operator=( const TLS_Var& inOther );


	public://///////////////////////////////////////////////////////////////////////////////////

		void				set( void* obj )
							{
#if FBL_WIN	
								TlsSetValue( tls_index, obj );
#else
								pthread_setspecific( key, obj );
#endif // FBL_WIN
							}

		void*				get() 
							{ 
#if FBL_WIN	
								return TlsGetValue( tls_index );
#else
								return pthread_getspecific( key );
#endif // FBL_WIN
							}


	private:////////////////////////////////////////////////////////////////////////////////////

							// TLS slot index
#if FBL_WIN	
		int					tls_index; 
#else
		pthread_key_t		key;
#endif // FBL_WIN
		
};


/**********************************************************************************************/
// TLS implementation for I_Unknown object.
// Keep the object alive self calling AddRef() in set().
// SafeRelease() is to free I_Unknown object.
//
template<class T>
class TLS_I_Unknown_Var
{
	public://///////////////////////////////////////////////////////////////////////////////////

							TLS_I_Unknown_Var()
							{
							}


virtual						~TLS_I_Unknown_Var()
							{
								SafeRelease();
							}


	public://///////////////////////////////////////////////////////////////////////////////////

		void				SafeRelease( void )
							{
								T* pObj = (T*)mTLSVar.get();
								if( pObj )
									pObj->Release();
							}

		void				set( T* obj )
							{
								SafeRelease();
								obj->AddRef();
								mTLSVar.set( obj );
							}

		T*					get()
							{
								return 	(T*)mTLSVar.get();
							}


	private:////////////////////////////////////////////////////////////////////////////////////

		TLS_Var				mTLSVar;
};


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // _FBL_TLS_Var_h
