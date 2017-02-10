/**********************************************************************************************/
/* VSQL_ProjectManager.h                                                   					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _VSQL_ProjectManager_h
	#define _VSQL_ProjectManager_h
#pragma once

#include <VShared/VSQL/Interfaces/VSQL_I_VProject.h>


/**********************************************************************************************/
VSQL_Begin_Namespace

/**********************************************************************************************/
// This is singleton which keeps references to the projects.
// ( As kernel-dll's dbs so VClient-dll dbs )
//
class FBL_SHARED_EXP_CLASS ProjectManager
{
	protected://////////////////////////////////////////////////////////////////////////////////
	
							ProjectManager();
							
	public://///////////////////////////////////////////////////////////////////////////////////
	
							// Access to singleton.
static ProjectManager* 		get_ProjectManager( void );
			
		vuint32				get_ProjectCount( void );

		I_VProject*			get_Project( vuint32 inIndex );
		I_VProject*			get_Project( const UChar* inName );

		vuint32				FindIndex( I_VProject* inProject );

	ArrayOfProjects_Ptr 	get_Projects( void )  { return mProjects; }


	public://///////////////////////////////////////////////////////////////////////////////////
	
							// This method is for cleanup purpose. 
							// Must be called from ValentinaShutDown() .
		void				Destroy_Projects( bool inIsRemoteOnly );

		void				RegisterProject( I_VProject* inProject );
		void				UnRegisterProject( I_VProject* inProject );

		
	private:////////////////////////////////////////////////////////////////////////////////////

static	ProjectManager*		mInstance;

								// This is array of just I_VProject*, i.e. this is not smart 
								// pointers. We need this because we want that a Project object 
								// is destroyed when the last its smart pointer becomes NULL. 
								// So we cannot keep in this array smart ptrs, because they will 
								// keep objects forever.

	ArrayOfProjects_Ptr 	mProjects;
};


/**********************************************************************************************/
VSQL_End_Namespace

#endif // _VSQL_ProjectManager_h
