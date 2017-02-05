/**********************************************************************************************/
/* FBL_StubRegistry.h	                                                    				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_StubRegistry_h 
	#define _FBL_StubRegistry_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_Unknown.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


// STD:
#include <map>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// FORWARD DECLARATIONS:
//
interface I_Connectable;
interface I_Connection;

SMART_INTERFACE(I_Connection);


/**********************************************************************************************/
// clients:
// Key		- ProxyPtr (proxy-object address)
// Value	- I_Connectable_Ptr (stub-object address)
//
// Server:
// Key		- KernelPtr (kernel-object address)
// Value	- ProxyPtr (proxy-object address)
//
template <class TKey, class TValue> class StubRegistry : public I_Unknown
{
		IMPLEMENT_UNKNOWN(StubRegistry)											
		BEGIN_INTERFACE_TABLE(StubRegistry)
		END_INTERFACE_TABLE()
		
	public://///////////////////////////////////////////////////////////////////////////////////

//	typedef std::map<TKey, TValue>			 TStubMap;
#ifdef V_SERVER
	typedef std::map<KernelPtr, ProxyPtr>			TStubMap;
#else	
	typedef std::map<ProxyPtr, I_Connectable*>		TStubMap;
#endif // V_SERVER

		

	public://///////////////////////////////////////////////////////////////////////////////////

							StubRegistry( void )
							{
							}

virtual		 				~StubRegistry( void )
							{
							}
				
					
	public://///////////////////////////////////////////////////////////////////////////////////

#ifdef V_SERVER
		void				Register( TKey inKey, TValue inStubPtr ) 
							{
								if( inKey )
									mStubMap.insert(
										std::make_pair( 
											inKey, 
											inStubPtr 
											) );
							}
#else
		void				Register( TValue inStubPtr ) 
							{
								if( inStubPtr )
								{
									// Lock StubRegistry.
									StCriticalSectionLocker lock_guard(mMutex);
								
									mStubMap.insert( 
										std::make_pair( 
											inStubPtr->get_RemoteID(), 
											inStubPtr ) );
								}
							}
#endif // V_SERVER

		void				Unregister( TKey inKey ) 
							{
								if( inKey )
								{
#ifdef V_SERVER
									mStubMap.erase( inKey );
#else
									// Lock StubRegistry.
									StCriticalSectionLocker lock_guard(mMutex);
									mStubMap.erase( inKey );
#endif // V_SERVER
								}
							}


		TValue				Find( TKey inStubName ) 
							{
								#ifndef V_SERVER
									// Lock StubRegistry.
									StCriticalSectionLocker lock_guard(mMutex);
								#endif // !V_SERVER

								TStubMap::iterator i = mStubMap.find(inStubName);
								if( i == mStubMap.end() )
									return (TValue) nullptr;

								return i->second;
							}

		int					GetCount( void )
							{
								#ifndef V_SERVER
									// Lock StubRegistry.
									StCriticalSectionLocker lock_guard(mMutex);
								#endif // !V_SERVER
							
								return mStubMap.size();
							}

		void				Erase( void ) 
							{
								#ifndef V_SERVER
									// Lock StubRegistry.
									StCriticalSectionLocker lock_guard(mMutex);
								#endif // !V_SERVER
							
								mStubMap.clear();
							}


	private:////////////////////////////////////////////////////////////////////////////////////

#ifndef V_SERVER
		CriticalSection		mMutex;
#endif // !V_SERVER
		TStubMap			mStubMap;
};


typedef StubRegistry<ProxyPtr, I_Connectable*> ClientStubRegistry;
typedef StubRegistry<KernelPtr, ProxyPtr> ServerStubRegistry;

FBL_SMART_PTR	( ClientStubRegistry );
FBL_SMART_PTR	( ServerStubRegistry );


/**********************************************************************************************/
ClientStubRegistry* GetClientStubRegistry( I_Connection_Ptr		inConnection );


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_StubRegistry_h
