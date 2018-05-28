/**********************************************************************************************/
/* FBL_ResourceManager.h                                                   					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_ResourceManager_h
	#define _FBL_ResourceManager_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>

/**********************************************************************************************/
FBL_Begin_Namespace

/**********************************************************************************************/
// This is singleton for synched get/set "global" variables.
//
class FBL_SHARED_EXP_CLASS ResourceManager
{
	protected://////////////////////////////////////////////////////////////////////////////////
	
							ResourceManager();
							
	public://///////////////////////////////////////////////////////////////////////////////////
	
							// Access to singleton.
static ResourceManager* 	get_ResourceManager( void );
			

	public://///////////////////////////////////////////////////////////////////////////////////

		#if FBL_MAC
		
		OSType				get_Signature( void ) const;
		void				put_Signature( OSType inValue );
		
		#endif // FBL_MAC
		
		
		const UChar*		get_DescriptionExt( void ) const;
		void				put_DescriptionExt( const UChar* inpValue );
		const UChar*		get_MainDataFileExt( void ) const;
		void				put_MainDataFileExt( const UChar* inpValue );
		const UChar*		get_IndexDataFileExt( void ) const;
		void				put_IndexDataFileExt( const UChar* inpValue );
		const UChar*		get_BlobsDataFileExt( void ) const;
		void				put_BlobsDataFileExt( const UChar* inpValue );
		const UChar*		get_TmpDataFileExt( void ) const;
		void				put_TmpDataFileExt( const UChar* inpValue );

		#if FBL_REQUIRE_LOCK_FILE
		const UChar*		get_LockDataFileExt( void ) const;
		void				put_LockDataFileExt( const UChar* inpValue );
		#endif // FBL_REQUIRE_LOCK_FILE
		
		#if FBL_UNIX
		const UnixHandle	get_VReportDllHandle( void ) const;
		void				put_VReportDllHandle( const UnixHandle inpValue );
		#endif // FBL_UNIX

		
	private:////////////////////////////////////////////////////////////////////////////////////

#if STD_THREADING_SAFE
static	std::atomic<ResourceManager*> mInstance;
#else
static	ResourceManager* volatile	mInstance;
#endif // STD_THREADING_SAFE


	private:////////////////////////////////////////////////////////////////////////////////////

mutable CriticalSection		mLock;
		
		#if FBL_MAC

		// Global constants which you can change to have own Creator and File types.
		//
		OSType				mSignature;				// on default 'VALA'
	
		#endif // FBL_MAC
		
static	const int			mExtSize = 32;
		UChar				mDescriptionExt[mExtSize];
		UChar				mMainDataFileExt[mExtSize];
		UChar				mIndexDataFileExt[mExtSize];
		UChar				mBlobsDataFileExt[mExtSize];
		UChar				mTmpDataFileExt[mExtSize];

		#if FBL_REQUIRE_LOCK_FILE
		UChar				mLockDataFileExt[mExtSize];
		#endif // FBL_REQUIRE_LOCK_FILE

		#if FBL_UNIX
		UnixHandle			mVReportDllHandle;	// TODO and check on linux
		#endif // FBL_UNIX
		

};


/**********************************************************************************************/
FBL_End_Namespace

#endif // _FBL_ResourceManager_h
