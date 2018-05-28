/**********************************************************************************************/
/* FBL_JSON_Message.h																		  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_JSON_Message_h
	#define _FBL_JSON_Message_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
enum ActionType
{
	kCreateActionType				= 1,
	kDeleteActionType				= 2,
	kAlterActionType				= 3
};


/**********************************************************************************************/
enum ObjectType
{
	kDatabaseObjectType 			= 1,
	kTableObjectType	 			= 2,
	kViewObjectType					= 3,
	kLinkObjectType					= 4,
	kTriggerObjectType				= 5,
	kConstraintObjectType			= 6,
	kSPObjectType		 			= 7,
	kFieldObjectType	 			= 8,
	kKeyValueObjectType				= 9
};


/**********************************************************************************************/
FBL_SHARED_EXP String CreateMessage( 
	ActionType		inAction, 
	ObjectType		inObjectType, 
	const String&	inObjectName );


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_JSON_Message_h
