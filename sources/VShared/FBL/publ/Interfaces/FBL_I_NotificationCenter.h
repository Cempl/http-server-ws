/**********************************************************************************************/
/* FBL_I_NotificationCenter.h                                                				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_NotificationCenter_h
	#define _FBL_I_NotificationCenter_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Util_classes/FBL_String.h>

// FINAL: 
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace

/**********************************************************************************************/
#define SCHEMA_CHANGES_CHANNEL "schemaChanges"


/**********************************************************************************************/
SMART_INTERFACE(I_NotificationCenter);

/**********************************************************************************************/
// interface FBL_SHARED_EXP_CLASS class
//
//StubInterface
interface FBL_SHARED_EXP_CLASS I_NotificationCenter : public I_Unknown
{
virtual							~I_NotificationCenter( void );

								// Return the list of all channels.
virtual ArrayOfStringsObj_Ptr	GetChannels( void ) = 0;

								// Return the list of system channels.
virtual ArrayOfStringsObj_Ptr	GetSystemChannels( void ) = 0;

								// Return the list of subscribed channels.
virtual ArrayOfStringsObj_Ptr	GetSubscribedChannels( void ) = 0;

								// Become listener of the inChannel. 
virtual void					Subscribe( const String& inChannel ) = 0;

								// Stop listen to inChannel.
virtual void					UnSubscribe( const String& inChannel ) = 0;

								// Stop listen to all subscribed channels.
virtual void					UnSubscribeAll( void ) = 0;

								// Send notification to all subscribers of inChannel.
virtual void					Notify(
									const String&			inChannel,
									const String&			inPayload,
									ENotificationPriority	inPriority = NormalPriority ) = 0;

};


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_I_NotificationCenter_h
