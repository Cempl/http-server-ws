/**********************************************************************************************/
/* FBL_Thread_Mutex_RW.cpp									 								  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Threads/FBL_Synch.h>


/**********************************************************************************************/
FBL_Begin_Namespace


///**********************************************************************************************/
//Thread_Mutex_RW::Thread_Mutex_RW( void )
//:
//	mWaitingReaders(mLock),
//	mNumWaitingReaders(0),
//	mWaitingWriters(mLock),
//	mNumWaitingWriters(0),
//	mRefCount(0),
//	mWaitingImportantWriter(mLock),
//	mImportantWriter(false),
//	mRemoved(false)
//{
//}
//
//
///**********************************************************************************************/
//Thread_Mutex_RW::~Thread_Mutex_RW( void )
//{
//	
//}
//
//
///**********************************************************************************************/
//void Thread_Mutex_RW::Remove( void )
//{
//
//}
//
//
///**********************************************************************************************/
//void Thread_Mutex_RW::AcquireRead( void )
//{
//	mLock.Acquire();
//
//	// Give preference to writers who are waiting.
//	while( mRefCount < 0 || mNumWaitingWriters > 0 )
//    {
//		// Join the waiting readers
//        mNumWaitingReaders++;
//		mWaitingReaders.Wait();
//        mNumWaitingReaders--;
//    }
//
//	// Add one more reader
//	mRefCount++;
//
//	// Release the lock to avoid deadlock
//	mLock.Release();
//}
//
//
///**********************************************************************************************/
//void Thread_Mutex_RW::AcquireWrite( void )
//{
//	mLock.Acquire();
//
//	while( mRefCount != 0 )
//    {
//		// Join the waiting writers
//        mNumWaitingWriters++;
//		mWaitingWriters.Wait();
//        mNumWaitingWriters--;
//    }
//
//	mRefCount = -1;
//	mLock.Release();
//}
//
//
///**********************************************************************************************/
//bool Thread_Mutex_RW::TryAcquireRead( void )
//{
//	bool Result = false;
//
//	mLock.Acquire();
//
//	if( mRefCount == -1 || 
//		mNumWaitingWriters > 0 )
//    {
//		// No way
//		Result = false;
//    }
//    else
//    {
//		// OK: Go on
//		mRefCount++;
//		Result = true;
//    }
//
//    mLock.Release();
//
//	return Result;
//}
//
//
///**********************************************************************************************/
//bool Thread_Mutex_RW::TryAcquireWrite( void )
//{
//	bool Result = false;
//
//	mLock.Acquire();
//
//    if( mRefCount != 0 )
//    {
//        Result = false;
//    }
//    else
//    {
//        mRefCount = -1;
//        Result = true;
//    }
//
//    mLock.Release();
//
//	return Result;
//}
//
//
///**********************************************************************************************/
//bool Thread_Mutex_RW::TryAcquireWriteUpgrade( void )
//{
//	bool Result = false;
//
//	mLock.Acquire();
//
//	if( mImportantWriter )
//    {
//		// an other reader upgrades already
//		Result = false;
//    }
//	else
//	{
//		while( mRefCount > 1 )		// Wait until only I am left
//        {
//			mNumWaitingWriters++;	// Prohibit any more readers
//			mImportantWriter = true;
//			mWaitingImportantWriter.Wait();
//			mImportantWriter = false;
//			mNumWaitingWriters--;
//        }
//		
//		// now I am a writer
//		// everything is O.K.
//		mRefCount = -1;
//		Result = true;
//	}
//
//	mLock.Release();
//
//	return Result;
//}
//
//
///**********************************************************************************************/
//void Thread_Mutex_RW::Acquire( void )
//{
//	AcquireWrite();
//}
//
//
///**********************************************************************************************/
//bool Thread_Mutex_RW::TryAcquire( void )
//{
//	return TryAcquireWrite();
//}
//
//
///**********************************************************************************************/
//void Thread_Mutex_RW::Release( void )
//{
//	mLock.Acquire();
//
//	if( mRefCount > 0 )			// Releasing a reader.
//	{
//		mRefCount--;
//	}
//	else if( mRefCount == -1 )	// Releasing a writer.
//	{
//		mRefCount = 0;
//	}
//	else
//	{
//		// Count should not be 0!
//		FBL_CHECK(0);
//	}
//
//	if( mImportantWriter && 
//		mRefCount == 1)
//	{
//		// Only the reader requesting to upgrade its lock is left over.
//		mWaitingImportantWriter.Signal();
//	}
//	else if( mNumWaitingWriters > 0 && 
//			 mRefCount == 0 )
//	{
//		// Give preference to writers over readers...
//		mWaitingWriters.Signal();
//	}
//	else if( mNumWaitingReaders > 0 && 
//			 mNumWaitingWriters == 0 )
//	{
//		// Wake up all readers...
//		mWaitingReaders.Broadcast();
//	}
//
//	mLock.Release();
//}


/**********************************************************************************************/
FBL_End_Namespace
