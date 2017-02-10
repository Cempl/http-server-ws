/**********************************************************************************************/
/* FBL_DatabaseManager.h                                                   					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_DatabaseManager_h
	#define _FBL_DatabaseManager_h
#pragma once

#include <VShared/FBL/publ/Interfaces/FBL_I_Database.h>


/**********************************************************************************************/
FBL_Begin_Namespace

/**********************************************************************************************/
// Helper
//
FBL_SHARED_EXP bool IsProject( I_Database_Ptr inpDatabase );


/**********************************************************************************************/
// This is singleton which keeps references to the databases.
// ( As kernel-dll's dbs so VClient-dll dbs )
//
class FBL_SHARED_EXP_CLASS DatabaseManager
{
	protected://////////////////////////////////////////////////////////////////////////////////
	
							DatabaseManager();
							
	public://///////////////////////////////////////////////////////////////////////////////////
	
							// Access to singleton.
static DatabaseManager* 	get_DatabaseManager( void );
			
		vuint32				get_DatabaseCount( void );

		I_Database*			get_Database( vuint32 inIndex );
		I_Database*			get_Database( const UChar* inName );
		I_Database*			get_NotProjectDatabase( const UChar* inName );
		I_Database*			get_MasterDb( void );

		vuint32				FindIndex(I_Database* inDB );

	ArrayOfDatabases_Ptr 	get_Databases( void )  { return mDatabases; }


	public://///////////////////////////////////////////////////////////////////////////////////
	
							// This method is for cleanup purpose. 
							// Must be called from ValentinaShutDown() .
		void				Destroy_Databases( bool inIsRemoteOnly );

		void				RegisterDatabase( I_Database* inDB );
		void				UnRegisterDatabase( I_Database* inDB );

		void				RegisterDumbDatabase( I_Database_Ptr inDB );
		
	private:////////////////////////////////////////////////////////////////////////////////////

static	True_Thread_Mutex			sSingletonMutex;
#if STD_THREADING_SAFE
static	std::atomic<DatabaseManager*> mInstance;
#else
static	DatabaseManager* volatile	mInstance;
#endif // STD_THREADING_SAFE

								// This is array of just I_Database*, i.e. this is not smart 
								// pointers. We need this because we want be that Database object 
								// get destroyed when the last smart pointer of become ZERO. 
								// So we cannot keep in this array smart ptrs, because they will 
								// keep objects forever.

	ArrayOfDatabases_Ptr 	mDatabases;

							// Special case for LocalConnection.
							// If no active db we create dumb db on ram once and use it
							// But LocalConnection is singleton (static object) and destroyed
							// after ValentinaShutDown() - we can not store I_Database_Ptr there!
							// Instead we register it here and ValentinaShutDown() close and destroy
							// that db in Destroy_Databases()
							//
	I_Database_Ptr			mpDumbDbForLocalConnection;	
};


/**********************************************************************************************/
FBL_End_Namespace

#endif // _FBL_DatabaseManager_h
