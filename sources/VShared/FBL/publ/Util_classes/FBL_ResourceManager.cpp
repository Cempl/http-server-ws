/**********************************************************************************************/
/* FBL_ResourceManager.cpp                                                 					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Util_classes/FBL_ResourceManager.h>
#include <VShared/FBL/publ/Util_Paths/FBL_Util_FileExt.h>
#include <VShared/FBL/publ/Util_classes/FBL_String.h>
#include <VShared/FBL/publ/Util_classes/FBL_StringConversion.h>
#include <VShared/FBL/publ/Util_Strings/FBL_ustdio.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
#if STD_THREADING_SAFE
std::atomic<ResourceManager*>	ResourceManager::mInstance;
#else
ResourceManager* volatile	ResourceManager::mInstance = nullptr;
#endif // STD_THREADING_SAFE

static CriticalSection		gResourceManagerFactoryLock;


/**********************************************************************************************/
ResourceManager* ResourceManager::get_ResourceManager( void )
{
	// Double-Checked Locking Pattern
	// http://www.aristeia.com/Papers/DDJ_Jul_Aug_2004_revised.pdf
	// C++11 - http://preshing.com/20130930/double-checked-locking-is-fixed-in-cpp11/
	//

	#if STD_THREADING_SAFE
		ResourceManager* tmp = mInstance.load();
		if( tmp == nullptr )
		{
			StCriticalSectionLocker lock(gResourceManagerFactoryLock);
			tmp = mInstance.load();
			if( tmp == nullptr )
			{
				tmp = new ResourceManager;
				mInstance.store(tmp);
			}
		}
	
		return tmp;
	#else
		if( mInstance == nullptr )
		{
			StCriticalSectionLocker lock(gResourceManagerFactoryLock);
			
			if( mInstance == nullptr )
			{
				// http://www.drdobbs.com/cpp/c-and-the-perils-of-double-checked-locki/184405726
				// Use tmp var to avoid second thread thinking that sGlobalProperties is not null
				// before construction is really finished in the current thread.
				// It is because "var = new type()" is not atomic.
				// There are 3 steps:
				// Step 1. Allocate memory to hold a Singleton object.
				// Step 2. Construct a Singleton object in the allocated memory.
				// Step 3. Make var point to the allocated memory.
				// Compilers are sometimes allowed to swap Steps 2 and 3.
				//
				ResourceManager* volatile pTemp = new ResourceManager;
				mInstance = pTemp;
			}
		}
			
		return 	mInstance;
	#endif // STD_THREADING_SAFE
}


/**********************************************************************************************/
ResourceManager::ResourceManager()
	#if FBL_MAC
:
	mSignature( gSignature )
	#endif // FBL_MAC

	#if FBL_UNIX
:
	mVReportDllHandle( nullptr )
	#endif // FBL_UNIX
{
	memset( mDescriptionExt, 0, mExtSize );
	memset( mMainDataFileExt, 0, mExtSize );
	memset( mIndexDataFileExt, 0, mExtSize );
	memset( mBlobsDataFileExt, 0, mExtSize );
	memset( mTmpDataFileExt, 0, mExtSize );

	#if FBL_REQUIRE_LOCK_FILE
	memset( mLockDataFileExt, 0, mExtSize );
	#endif // FBL_REQUIRE_LOCK_FILE
}


#pragma mark -

#if FBL_MAC
/**********************************************************************************************/
OSType ResourceManager::get_Signature( void ) const
{
	StCriticalSectionLocker lock(mLock);
	return mSignature;
}


/**********************************************************************************************/
void ResourceManager::put_Signature( OSType inValue )
{
	StCriticalSectionLocker lock(mLock);
	mSignature = inValue;
}
#endif // FBL_MAC


/**********************************************************************************************/
const UChar* ResourceManager::get_DescriptionExt( void ) const
{
	StCriticalSectionLocker lock(mLock);
	return mDescriptionExt;
}


/**********************************************************************************************/
void ResourceManager::put_DescriptionExt( const UChar* inpValue )
{
	StCriticalSectionLocker lock(mLock);

	if( inpValue == nullptr || *inpValue == 0 )
	{
		String defExt = ".vdb";
		vu_strncpy( mDescriptionExt, defExt.c_str(), mExtSize );
	}
	else if( inpValue[0] != UChar('.') )
	{
		mDescriptionExt[0] = UChar('.');
		vu_strncpy( mDescriptionExt + 1, inpValue, mExtSize-1 );
	}
	else
	{
        vu_strncpy( mDescriptionExt, inpValue, mExtSize );
	}
}


/**********************************************************************************************/
const UChar* ResourceManager::get_MainDataFileExt( void ) const
{
	StCriticalSectionLocker lock(mLock);
	return mMainDataFileExt;
}


/**********************************************************************************************/
void ResourceManager::put_MainDataFileExt( const UChar* inpValue )
{
	StCriticalSectionLocker lock(mLock);

	if( inpValue == nullptr || *inpValue == 0 )
	{
		String defExt = ".dat";
		vu_strncpy( mMainDataFileExt, defExt.c_str(), mExtSize );
	}
	else if( inpValue[0] != UChar('.') )
	{
		mMainDataFileExt[0] = UChar('.');
		vu_strncpy( mMainDataFileExt + 1, inpValue, mExtSize-1 );
	}
	else
	{
        vu_strncpy( mMainDataFileExt, inpValue, mExtSize );
	}
}


/**********************************************************************************************/
const UChar* ResourceManager::get_IndexDataFileExt( void ) const
{
	StCriticalSectionLocker lock(mLock);
	return mIndexDataFileExt;
}


/**********************************************************************************************/
void ResourceManager::put_IndexDataFileExt( const UChar* inpValue )
{
	StCriticalSectionLocker lock(mLock);

	if( inpValue == nullptr || *inpValue == 0 )
	{
		String defExt = ".ind";
		vu_strncpy( mIndexDataFileExt, defExt.c_str(), mExtSize );
	}
	else if( inpValue[0] != UChar('.') )
	{
		mIndexDataFileExt[0] = UChar('.');
		vu_strncpy( mIndexDataFileExt + 1, inpValue, mExtSize-1 );
	}
	else
	{
        vu_strncpy( mIndexDataFileExt, inpValue, mExtSize );
	}
}


/**********************************************************************************************/
const UChar* ResourceManager::get_BlobsDataFileExt( void ) const
{
	StCriticalSectionLocker lock(mLock);
	return mBlobsDataFileExt;
}


/**********************************************************************************************/
void ResourceManager::put_BlobsDataFileExt( const UChar* inpValue )
{
	StCriticalSectionLocker lock(mLock);

	if( inpValue == nullptr || *inpValue == 0 )
	{
		String defExt = ".blb";
		vu_strncpy( mBlobsDataFileExt, defExt.c_str(), mExtSize );
	}
	else if( inpValue[0] != UChar('.') )
	{
		mBlobsDataFileExt[0] = UChar('.');
		vu_strncpy( mBlobsDataFileExt + 1, inpValue, mExtSize-1 );
	}
	else
	{
        vu_strncpy( mBlobsDataFileExt, inpValue, mExtSize );
	}
}


/**********************************************************************************************/
const UChar* ResourceManager::get_TmpDataFileExt( void ) const
{
	StCriticalSectionLocker lock(mLock);
	return mTmpDataFileExt;
}


/**********************************************************************************************/
void ResourceManager::put_TmpDataFileExt( const UChar* inpValue )
{
	StCriticalSectionLocker lock(mLock);

	if( inpValue == nullptr || *inpValue == 0 )
	{
		String defExt = ".tmp";
		vu_strncpy( mTmpDataFileExt, defExt.c_str(), mExtSize );
	}
	else if( inpValue[0] != UChar('.') )
	{
		mTmpDataFileExt[0] = UChar('.');
		vu_strncpy( mTmpDataFileExt + 1, inpValue, mExtSize-1 );
	}
	else
	{
        vu_strncpy( mTmpDataFileExt, inpValue, mExtSize );
	}
}


#if FBL_REQUIRE_LOCK_FILE
/**********************************************************************************************/
const UChar* ResourceManager::get_LockDataFileExt( void ) const
{
	StCriticalSectionLocker lock(mLock);
	return mLockDataFileExt;
}


/**********************************************************************************************/
void ResourceManager::put_LockDataFileExt( const UChar* inpValue )
{
	StCriticalSectionLocker lock(mLock);

	if( inpValue == NULL || *inpValue == 0 )
	{
		String defExt = ".lck";
		vu_strncpy( mLockDataFileExt, defExt.c_str(), mExtSize );
	}
	else if( inpValue[0] != UChar('.') )
	{
		mLockDataFileExt[0] = UChar('.');
		vu_strncpy( mLockDataFileExt + 1, inpValue, mExtSize-1 );
	}
	else
	{
        vu_strncpy( mLockDataFileExt, inpValue, mExtSize );
	}
}
#endif // FBL_REQUIRE_LOCK_FILE


#if FBL_UNIX
/**********************************************************************************************/
const UnixHandle ResourceManager::get_VReportDllHandle( void ) const
{
	StCriticalSectionLocker lock(mLock);
	return mVReportDllHandle;
}


/**********************************************************************************************/
void ResourceManager::put_VReportDllHandle( const UnixHandle inValue )
{
	StCriticalSectionLocker lock(mLock);
	mVReportDllHandle = inValue;
}


#endif // FBL_UNIX


/**********************************************************************************************/
FBL_End_Namespace
