/**********************************************************************************************/
/* FBL_NotificationQueue.cpp																  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>
#include "FBL_NotificationQueue.h"

#if FBL_UNIX
#include <limits.h>
#endif // FBL_UNIX

/**********************************************************************************************/
FBL_Begin_Namespace

/**********************************************************************************************/
const size_t NotificationQueue::sMinNQSize 	= gDefaultNQSize;

/**********************************************************************************************/
// NOT visible outside of this file:
// Access via singlenton methods only.
//
static NotificationQueue_Ptr gNotificationQueuePtr	= nullptr;


/**********************************************************************************************/
NotificationQueue_Ptr GetLocalNotificationQueue( void )
{
	FBL_NOTIFICATION_CENTER_LOCK;

	if( gNotificationQueuePtr == nullptr )
		gNotificationQueuePtr = new NotificationQueue( gDefaultNQSize );

	return gNotificationQueuePtr;
}


/**********************************************************************************************/
NotificationQueue::NotificationQueue( size_t inMaxNQSize )
:
	mMaxNQSize( (inMaxNQSize < sMinNQSize) ? sMinNQSize : inMaxNQSize)
{
	Open();
}


/**********************************************************************************************/
NotificationQueue::~NotificationQueue()
{
}


/**********************************************************************************************/
void NotificationQueue::Open( void )
{
	mIsOpen = true;
}


/**********************************************************************************************/
void NotificationQueue::Close( void )
{
	mIsOpen = false;

	// Notify waiters:
	mNotificationAvailable.notify_one();
}


/**********************************************************************************************/
bool NotificationQueue::isOpen( void )
{
	return (bool)mIsOpen.load();
}


/**********************************************************************************************/
Notification_Ptr NotificationQueue::GetNotification( void )
{
	Notification_Ptr pRes;
	
	if( mIsOpen.load() )
	{
		StLockGuard<NQ_MUTEX> g( mNQMutex );
		pRes = GetNotificationEx();
	}
	
	return pRes;
}


/**********************************************************************************************/
Notification_Ptr NotificationQueue::WaitForNotification( vuint32 in_TimeOut_ms )
{
	Notification_Ptr pRes = GetNotification();
	
	// WaitForNotification() returns NULL in 2 cases:
	// - The queue was "closed" (the way to finish the thread which is running this function);
	// - timeout reached.
	//
	vuint32 timeOut = (in_TimeOut_ms) ? in_TimeOut_ms : ULONG_MAX;
	while( mIsOpen.load() && pRes == nullptr )
	{
		StLockGuard_T<NQ_MUTEX> guard( mNQConditionVarMutex );
		
		if( mNotificationAvailable.wait_for( guard, timeOut ) == false )
			break;
			
		pRes = GetNotification();
	}

	return pRes;
}


#pragma mark -

/**********************************************************************************************/
void NotificationQueue::AddNotification( Notification_Ptr inpNotification )
{
	if( mIsOpen.load() )
	{
		{
			StLockGuard<NQ_MUTEX> g( mNQMutex );

			// Remove oldest element from the queue in case of mMaxNQSize reached.
			if( mQueue.size() == mMaxNQSize )
				GetNotificationEx();
				
			mQueue.push( inpNotification );
		}
		
		// No lock at this time.
		mNotificationAvailable.notify_one();
	}
}


/**********************************************************************************************/
Notification_Ptr NotificationQueue::GetNotificationEx( void )
{
	Notification_Ptr pRes;
	
	if( mQueue.empty() == false )
	{
		pRes = mQueue.front();
		mQueue.pop();
	}
	
	return pRes;
}


/**********************************************************************************************/
FBL_End_Namespace

