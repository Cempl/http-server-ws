/**********************************************************************************************/
/* FBL_I_Task.h 		                                                      				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_Task_h 
	#define _FBL_I_Task_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Sets/FBL_Set.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// this file declarations:
//
SMART_INTERFACE(I_Task);
SMART_INTERFACE(I_Task_MT);


/**********************************************************************************************/
SMART_ARRAY_OF_SMART_PTRS( TaskArray, I_Task_Ptr );
SMART_ARRAY_OF_SMART_PTRS( TaskMtArray, I_Task_MT_Ptr );


/**********************************************************************************************/
// interface FBL_SHARED_EXP_CLASS class
//
interface FBL_SHARED_EXP_CLASS I_Task : public I_Unknown
{
	// ---------------------
	// Types:

	enum EState { 
		Ready, 			// The task is prepared but was not yet started.
		Running, 		// the task now execute Run() command.
		Finished, 		// The task have finish its work. We can obtain results.
		Canceled		// The task was canceled by user. Result not exists.
	};

	// ---------------------
	// Construction:

virtual 					~I_Task( void );

	// ---------------------
	// Properties:
	
		// <Name>
							/// The name of a task.
virtual const String&		get_Name( void ) const  = 0; //OLDKEEPAS: mName
virtual void				put_Name( const String& inName )  = 0;

		// <State>
virtual	EState				get_State( void ) const  = 0;
virtual void				put_State( EState inState )  = 0;

		// <Error>
							/// The last error during execution of this task. 
virtual vint32				get_Error( void ) const   = 0;		
virtual void				put_Error( vint32 inError )  = 0;

		// <AffectRows>
							/// If we want that task remember records it have affect,
							/// then we must specify Set to be used for this.
virtual Set_Ptr				get_AffectedRecords( void ) const  = 0;
virtual void				put_AffectedRecords( 
								Set_Ptr 	inSet ) = 0;

		// <Completion>
							/// Returns completion for this task in the range 0.0 - 1.0
virtual double				get_Completion( void ) const  = 0;


	// ---------------------
	// Methods:

							/// This function must execute work of this task.
virtual	void				Run( void )  = 0;	

							/// Task can be explicitly cancelled.
							/// Run() should check for Canceled state in appropriate places.
virtual	void				Cancel( void )  = 0;	

};


/**********************************************************************************************/
typedef True_Thread_Mutex_Recursive	TASK_MUTEX;


/**********************************************************************************************/
// MultiThreaded I_Task - knows own children.
//
interface FBL_SHARED_EXP_CLASS I_Task_MT : public I_Unknown
{
	// ---------------------
	// Construction:

virtual 					~I_Task_MT( void );

	// ---------------------
	// Properties:

		// <Child task count>
							/// Child tasks count.
virtual vuint32				get_ChildCount( void ) const = 0;

	// ---------------------
	// Methods:

							/// TaskManager get it for analyzing/changing task state.
							/// (TaskManager asks task for it's state and then decide what to do,
							/// it may take time, but task may change own state itself in different
							/// thread - that is why we need the mutex from Task in TaskManager).
virtual TASK_MUTEX*			GetTaskMutex( void ) = 0;

							/// On error during execution of this task.
virtual void				OnException( xException& inError )  = 0;

							/// Register task as a child of current task.
							/// Parent task MUST register all tasks produced in own Run()
							/// before register this task to the TaskManager.
virtual void				put_Child( I_Task_MT_Ptr inpTask ) = 0;

							/// Return child task
virtual I_Task_MT_Ptr		get_Child( vuint32 inPosition ) = 0;

virtual void				RemoveChild( I_Task_MT_Ptr inpTask ) = 0;

};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_I_Task_h
