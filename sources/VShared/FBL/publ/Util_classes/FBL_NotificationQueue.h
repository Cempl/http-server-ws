/**********************************************************************************************/
/* FBL_NotificationQueue.h																	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_NotificationQueue_h
	#define _FBL_NotificationQueue_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Util_classes/FBL_Notification.h>
#include <VShared/FBL/publ/Util_classes/FBL_String.h>
#include <VShared/FBL/publ/Util_classes/FBL_ForeignPlugin_Imp.h>

#include <queue>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace

/**********************************************************************************************/
typedef True_Own_Thread_Mutex		NQ_MUTEX;
typedef True_Own_ConditionVar		NQ_CONDITION_VAR;

/**********************************************************************************************/
SMART_CLASS( NotificationQueue );

/**********************************************************************************************/
FBL_SHARED_EXP NotificationQueue_Ptr GetLocalNotificationQueue( void );


/**********************************************************************************************/
// Customer app (vClient or vKernel based) should call these
// in order to get received notifications from the channels the app subscribed to.
//
class FBL_SHARED_EXP_CLASS NotificationQueue:
	public ForeignPlugin_imp
{	
		IMPLEMENT_UNKNOWN(NotificationQueue)
		BEGIN_INTERFACE_TABLE(NotificationQueue)
			IMPLEMENT_INTERFACE(I_ForeignPlugin)
		END_INTERFACE_TABLE()

	public://///////////////////////////////////////////////////////////////////////////////////

									NotificationQueue( size_t inMaxNQSize );
				
virtual								~NotificationQueue();


	public://///////////////////////////////////////////////////////////////////////////////////

									// Make NQ available to get/set notifications.
									// (NQ is in open state normally).
		void						Open( void );

									// Make NQ unavailable to get/set notifications.
									// (Mainly for interrupting  WaitForNotification() runned
									//  in another thread).
		void						Close( void );
		

		bool						isOpen( void );
		
									// Get pending Notification from the queue or return NULL
									// immediately.
									//
		Notification_Ptr			GetNotification( void );

									// Blocks current thread until either timeout reached
									// (0 - no timeout) or some notification comes to the queue
									// or NQ closed from some thread.
									//
									// !!! Don't call this method from the sinle-threaded app !!!
									// because of endless waiting there.
									//
		Notification_Ptr			WaitForNotification( vuint32 in_TimeOut_ms = 0 );

		size_t						get_Capacity( void )
									{
										return mMaxNQSize;
									}
				
				
	private:///////////////////////////////////////////////////////////////////////////////////

friend	class NotificationHandler;
friend	class NotificationCenter;
friend	class VC_NotificationCenter;

									// Add new notification (called by NotificationHandler)
		void						AddNotification( Notification_Ptr inpNotification );

		Notification_Ptr			GetNotificationEx( void );


	private:///////////////////////////////////////////////////////////////////////////////////

static	const size_t				sMinNQSize;

		Atomic_vint32				mIsOpen;
		size_t						mMaxNQSize;
		std::queue<Notification_Ptr> mQueue;				// Fixed size (mMaxNQSize)
		
mutable	NQ_MUTEX					mNQMutex;				// To synch access to the mQueue
mutable	NQ_MUTEX					mNQConditionVarMutex;	// To use for mNotificationAvailable
		NQ_CONDITION_VAR			mNotificationAvailable;	// To wake up WaitForNotification()
};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_NotificationQueue_h

