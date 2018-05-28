/**********************************************************************************************/
/* FBL_Notification.h																		  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_Notification_h
	#define _FBL_Notification_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Serializable.h>
#include <VShared/FBL/publ/Util_classes/FBL_String.h>
#include <VShared/FBL/publ/Util_classes/FBL_ForeignPlugin_Imp.h>


// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace

/**********************************************************************************************/
SMART_CLASS( Notification );

/**********************************************************************************************/
class FBL_SHARED_EXP_CLASS Notification :
	public I_Serializable,
	public ForeignPlugin_imp
{	
		IMPLEMENT_UNKNOWN(Notification)
			BEGIN_INTERFACE_TABLE(Notification)										
				IMPLEMENT_INTERFACE(I_Serializable)
				IMPLEMENT_INTERFACE(I_ForeignPlugin)
			END_INTERFACE_TABLE()


	public://///////////////////////////////////////////////////////////////////////////////////

								Notification( void )
								:
									mTimeStamp(0),
									mInitiatorClientID(0),
									mIsSystem(false),
									mPriority(NormalPriority)
								{
								}

								Notification(
									time_t					inTimeStamp,
									size_t					inInitiatorClientID,
									String					inChannel,
									String					inPayLoad,
									bool					inIsSystem,
									ENotificationPriority	inPriority
								)
								:
									mTimeStamp(inTimeStamp),
									mInitiatorClientID(inInitiatorClientID),
									mChannel(inChannel),
									mPayLoad(inPayLoad),
									mIsSystem(inIsSystem),
									mPriority(inPriority)
								{}

virtual							~Notification( void )
								{
								}


	public://///////////////////////////////////////////////////////////////////////////////////

	// interface I_Serializable:

virtual void					From( I_IStream_Ptr inStream, bool inBlock = true ) override;
virtual void					From( I_PacketRcv* 	inPacket, bool inBlock = true ) override;

virtual void					To( I_OStream_Ptr inStream, bool inBlock = true ) const override;
virtual void					To( I_PacketSnd*  inPacket, bool inBlock = true ) const override;


				
	public://///////////////////////////////////////////////////////////////////////////////////
				
		time_t					mTimeStamp;
		size_t					mInitiatorClientID;	// 0 - means "from vKernel".
		String					mChannel;			// kind of notification.
		String					mPayLoad;			// user can put here anything.
		bool					mIsSystem;			// to differ internal notifications from user ones.
		ENotificationPriority	mPriority;
};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_Notification_h

