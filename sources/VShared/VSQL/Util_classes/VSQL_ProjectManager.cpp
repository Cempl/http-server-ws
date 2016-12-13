/**********************************************************************************************/
/* VSQL_ProjectManager.cpp                                                 					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Util_classes/FBL_String.h>
#include <VShared/FBL/publ/Util_Strings/FBL_ustdio.h>

#include <VShared/VSQL/Util_classes/VSQL_ProjectManager.h>

/**********************************************************************************************/
VSQL_Begin_Namespace


/**********************************************************************************************/
ProjectManager*	ProjectManager::mInstance = nullptr;


/**********************************************************************************************/
ProjectManager* ProjectManager::get_ProjectManager( void ) 
{
	if( mInstance == nullptr )
	{
		mInstance = new ProjectManager;
	}
		
	return 	mInstance;	
}

/**********************************************************************************************/
void ProjectManager::Destroy_Projects( bool inIsRemoteOnly ) 
{
	vuint32 Count = mProjects->get_Count();
	
	for( vuint32 i = 1; i <= Count ; ++i )
	{
		I_VProject* pProject = mProjects->get_ItemAt( i );
		
		try
		{
			// Close and UnRegister some projects - depends on inIsRemoteOnly.
			bool isRemote = pProject->get_IsRemote();

			if( (isRemote && inIsRemoteOnly) ||
				(isRemote == false && inIsRemoteOnly == false ) )
			{	
				if( pProject->get_IsOpen() )
					pProject->Close();
			}
		}
		catch(...)
		{
		}
			
		UnRegisterProject( pProject );
	} 

	// We could not throw away this array.
	// It will die along with ProjectManager instance.
	//mProjects = NULL;
}


/**********************************************************************************************/
ProjectManager::ProjectManager()
{
	mProjects = new ArrayOfProjects( kNotOwnItems );
}


/**********************************************************************************************/
void ProjectManager::RegisterProject( I_VProject* inProject ) 
{
	if( mProjects->FindIndex(inProject) == 0 )
		mProjects->AddItem( inProject );
}


/**********************************************************************************************/
void ProjectManager::UnRegisterProject( I_VProject* inProject ) 
{
	mProjects->RemoveItem( inProject );
}


/**********************************************************************************************/
vuint32 ProjectManager::get_ProjectCount( void ) 
{
	return mProjects->get_Count();
}


/**********************************************************************************************/
vuint32 ProjectManager::FindIndex( I_VProject* inProject ) 
{
	return mProjects->FindIndex( inProject );
}


/**********************************************************************************************/
I_VProject* ProjectManager::get_Project( vuint32 inIndex ) 
{
	if( inIndex > mProjects->get_Count() )
		return nullptr;
	else
		return mProjects->get_ItemAt( inIndex );
}


/**********************************************************************************************/
I_VProject* ProjectManager::get_Project( const UChar* inName ) 
{
	vuint32 Count = mProjects->get_Count();
	for( vuint32 i = 1; i <= Count ; ++i )
	{
		I_VProject* pProject = mProjects->get_ItemAt( i );

		String projectName = pProject->get_Name();
		
		// remove extension if any.
		tslen DotIndex = projectName.find('.'); 
		if( DotIndex != -1 )
			projectName.remove( DotIndex );

		if( projectName.caseCompare( inName, 0 ) == 0 )
			return pProject;

	} 
	
	return nullptr;
}


/**********************************************************************************************/
VSQL_End_Namespace
