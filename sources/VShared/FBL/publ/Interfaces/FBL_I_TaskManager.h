/**********************************************************************************************/
/* FBL_I_TaskManager.h                                                       				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_TaskManager_h 
	#define _FBL_I_TaskManager_h

#include <VShared/FBL/publ/Headers/FBL.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE(I_Task_MT);
SMART_INTERFACE(I_TaskManager);


/**********************************************************************************************/
interface FBL_SHARED_EXP_CLASS I_TaskManager : public I_Unknown
{
virtual						~I_TaskManager( void );

	// --- Properties ---
	
		// <TaskCount>
virtual vuint32				get_TaskCount( void ) const = 0;

virtual vuint32				get_IncompleteTaskCount( void ) const = 0;


	// --- Methods ---
							/// Assign new task to the TaskManager
virtual	void				AssignTask( I_Task_MT_Ptr inNewTask ) = 0;

							/// Returns a task by index.
virtual I_Task_MT_Ptr		get_Task( vuint32 inIndex ) const = 0;

							/// Returns a task by name.
virtual I_Task_MT_Ptr		get_Task( const UChar* inName ) const = 0;

							/// Remove all the task which are not Ready or Running.
virtual void				Compact( void ) = 0;

};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_I_TaskManager_h
