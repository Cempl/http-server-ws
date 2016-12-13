/**********************************************************************************************/
/* FBL_DatabaseManager.cpp                                                 					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Util_classes/FBL_DatabaseManager.h>
#include <VShared/FBL/publ/Util_classes/FBL_String.h>
#include <VShared/FBL/publ/Util_Strings/FBL_ustdio.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_Table.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
#if STD_THREADING_SAFE
std::atomic<DatabaseManager*>	DatabaseManager::mInstance;
#else
DatabaseManager* volatile	DatabaseManager::mInstance = nullptr;
#endif // STD_THREADING_SAFE

True_Thread_Mutex			DatabaseManager::sSingletonMutex;

/**********************************************************************************************/
DatabaseManager* DatabaseManager::get_DatabaseManager( void ) 
{
	// Double-Checked Locking Pattern
	// http://www.aristeia.com/Papers/DDJ_Jul_Aug_2004_revised.pdf
	// C++11 - http://preshing.com/20130930/double-checked-locking-is-fixed-in-cpp11/
	//

	#if STD_THREADING_SAFE
		DatabaseManager* tmp = mInstance.load();
		if( tmp == nullptr )
		{
			StLockGuard<True_Thread_Mutex> lock(sSingletonMutex);
			tmp = mInstance.load();
			if( tmp == nullptr )
			{
				tmp = new DatabaseManager;
				mInstance.store(tmp);
			}
		}
	
		return tmp;
	#else
		if( mInstance == nullptr )
		{
			StLockGuard<True_Thread_Mutex> lock(sSingletonMutex);
			if( mInstance == nullptr )
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
				DatabaseManager* volatile pTemp = new DatabaseManager;
				mInstance = pTemp;
			}
		}
			
		return 	mInstance;
	#endif // STD_THREADING_SAFE
}

/**********************************************************************************************/
void DatabaseManager::Destroy_Databases( bool inIsRemoteOnly ) 
{
	FBL_ENGINE_LOCK
	if( mpDumbDbForLocalConnection )
	{
		// Destroy mpDumbDbForLocalConnection which DatabaseManager owns.
		mpDumbDbForLocalConnection->Close();
		mpDumbDbForLocalConnection = nullptr;
	}


	vuint32 Count = mDatabases->get_Count();
	
	for( vuint32 i = 1; i <= Count ; ++i )
	{
		I_Database* pdb = mDatabases->get_ItemAt( i );
		
		try
		{
			// Close and UnRegister some databases - depends on inIsRemoteOnly.
			bool isRemote = pdb->get_IsRemote();

			if( (isRemote && inIsRemoteOnly) ||
				(isRemote == false && inIsRemoteOnly == false ) )
			{	
				if( pdb->get_IsOpen() )
					pdb->Close();
			}
		}
		catch(...)
		{
		}
			
		UnRegisterDatabase( pdb );
	} 

	// We could not throw away this array.
	// It will die along with DatabaseManager instance.
	//mDatabases = NULL;
}


/**********************************************************************************************/
DatabaseManager::DatabaseManager()
{
	mDatabases = new ArrayOfDatabases( kNotOwnItems );
}


/**********************************************************************************************/
void DatabaseManager::RegisterDatabase( I_Database* inDB ) 
{
	FBL_ENGINE_LOCK
	if( mDatabases->FindIndex(inDB) == 0 )
		mDatabases->AddItem( inDB );
}


/**********************************************************************************************/
void DatabaseManager::UnRegisterDatabase( I_Database* inDB ) 
{
	FBL_ENGINE_LOCK
	mDatabases->RemoveItem( inDB );
}


/**********************************************************************************************/
void DatabaseManager::RegisterDumbDatabase( I_Database_Ptr inDB ) 
{
	FBL_ENGINE_LOCK
	mpDumbDbForLocalConnection = inDB;
}


/**********************************************************************************************/
vuint32 DatabaseManager::get_DatabaseCount( void ) 
{
	FBL_ENGINE_LOCK
	return mDatabases->get_Count();
}


/**********************************************************************************************/
vuint32 DatabaseManager::FindIndex( I_Database* inDB ) 
{
	return mDatabases->FindIndex( inDB );
}


/**********************************************************************************************/
I_Database* DatabaseManager::get_Database( vuint32 inIndex ) 
{
	FBL_ENGINE_LOCK
	if( inIndex > mDatabases->get_Count() )
		return nullptr;
	else
		return mDatabases->get_ItemAt( inIndex );
}


/**********************************************************************************************/
I_Database* DatabaseManager::get_Database( const UChar* inName ) 
{
	FBL_ENGINE_LOCK
	vuint32 Count = mDatabases->get_Count();
	for( vuint32 i = 1; i <= Count ; ++i )
	{
		I_Database* pdb = mDatabases->get_ItemAt( i );

		String dbName = pdb->get_Name();
		
		// remove extension if any.
		tslen DotIndex = dbName.find('.'); 
		if( DotIndex != -1 )
			dbName.remove( DotIndex );

		if( StringCompare( dbName, inName, false ) == 0 )
			return pdb;

/*
		if( vu_stricmp( inName, pdb->get_Name().c_str() ) == 0 )
			return pdb;
*/
	} 
	
	return nullptr;
}


/**********************************************************************************************/
I_Database* DatabaseManager::get_NotProjectDatabase( const UChar* inName ) 
{
	FBL_ENGINE_LOCK
	vuint32 Count = mDatabases->get_Count();
	for( vuint32 i = 1; i <= Count ; ++i )
	{
		I_Database* pdb = mDatabases->get_ItemAt( i );

		if( IsProject( pdb ) )
			continue;
			
		String dbName = pdb->get_Name();
		
		// remove extension if any.
		tslen DotIndex = dbName.find('.'); 
		if( DotIndex != -1 )
			dbName.remove( DotIndex );

		if( StringCompare( dbName, inName, false ) == 0 )
			return pdb;

/*
		if( vu_stricmp( inName, pdb->get_Name().c_str() ) == 0 )
			return pdb;
*/
	} 
	
	return nullptr;
}


/**********************************************************************************************/
I_Database* DatabaseManager::get_MasterDb( void ) 
{
	FBL_ENGINE_LOCK
	String masterDBName("master.vdb");

	vuint32 Count = mDatabases->get_Count();
	for( vuint32 i = 1; i <= Count ; ++i )
	{
		I_Database* pdb = mDatabases->get_ItemAt( i );
		if( masterDBName == pdb->get_Name() )
			return pdb;
	} 
	
	return nullptr;
}


#pragma mark -

/**********************************************************************************************/
bool IsProject( I_Database_Ptr inpDatabase )
{
	FBL_ENGINE_LOCK
	bool result = false;

	// We check here: It must be system table Project for database being project.
	FBL_CHECK( inpDatabase );
	I_Table_Ptr pTable = inpDatabase->get_Table( "Project" );
	if( pTable &&
		(pTable->get_IsSystem() || pTable->get_IsTemporary()) )
		result = true;

	return result;
}


/**********************************************************************************************/
FBL_End_Namespace
