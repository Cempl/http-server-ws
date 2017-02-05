/**********************************************************************************************/
/* FBL_CreateFile.cpp 	                                                   					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/File_OS/FBL_File_OS.h>

#include <VShared/FBL/publ/Location/FBL_Location_Disk_FSSpec.h>
#include <VShared/FBL/publ/Location/FBL_Location_FSRef.h>

#include <VShared/FBL/publ/Util_classes/FBL_ToUChar.h>
#include <VShared/FBL/publ/Util_Strings/FBL_ustdio.h>

#if FBL_UNIX
#	include <sys/stat.h>
#endif // FBL_UNIX

/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
I_File* CreateFile( const char* inPath )
{
#if TARGET_OS_IPHONE
	argused1( inPath );

	return NULL;
#else
	File_OS* pFile = new File_OS();

	#if FBL_MAC
		I_Location_Ptr pLoc = CreateDiskLocationFromFSRef( inPath );
	#else
		I_Location_Ptr pLoc = CreateDiskLocation( inPath );
	#endif // FBL_MAC

	pFile->put_Location( pLoc );

	return pFile;
#endif // TARGET_OS_IPHONE
}


/**********************************************************************************************/
I_File* CreateFile( const UChar* inPath )
{
#if TARGET_OS_IPHONE
	argused1( inPath );

	return NULL;
#else
	File_OS* pFile = new File_OS();

	#if FBL_MAC
		I_Location_Ptr pLoc = CreateDiskLocationFromFSRef( inPath );	
	#else
		I_Location_Ptr pLoc = CreateDiskLocation( inPath );
	#endif // FBL_MAC
	
	pFile->put_Location( pLoc );

	return pFile;
#endif // TARGET_OS_IPHONE
}


/**********************************************************************************************/
I_File* CreateFile( I_Location_Ptr inLoc )
{
#if TARGET_OS_IPHONE
	argused1( inLoc );

	return NULL;
#else
	File_OS* pFile = new File_OS();

	pFile->put_Location( inLoc );

	return pFile;
#endif // TARGET_OS_IPHONE
}


#pragma mark -

/**********************************************************************************************/
// Create folder on the disk.
//
I_Location_Ptr CreateFolder( I_Location_Ptr inParentLoc,  const char* inFolderName )
{
	String newFolderName( inFolderName );

	return CreateFolder( inParentLoc, newFolderName.c_str() );
}


/**********************************************************************************************/
I_Location_Ptr CreateFolder( I_Location_Ptr inParentLoc,  const UChar* inFolderName )
{
	// Location to be returned.
	I_Location_Ptr pLoc = inParentLoc->get_ChildLocation( inFolderName );

	// Create folder on the disk.
	//
	#if FBL_WIN
		CreateDirectoryW( pLoc->get_Path().c_str(), NULL );
		/*
		if( CreateDirectoryW( pLoc->get_Path().c_str(), NULL ) == false )
		{
			FBL_Throw( xOSFileError(
							ERR_CREATE_FOLDER,
							pLoc->get_Path().c_str(),
							ToUCharPtr(GetLastError()).c_str() ) );
		}
		*/
	#elif FBL_MAC
		#if FBL_SUPPORT_FSREF
			Location_Disk_FSRef_Ptr pLocationRef = fbl_dynamic_cast<Location_Disk_FSRef>(inParentLoc);
			#if FBL_SUPPORT_FSSPEC
				if( !pLocationRef )
				{
					Location_Disk_FSSpec_Ptr pLocationSpec = fbl_dynamic_cast<Location_Disk_FSSpec>(inParentLoc);

					pLocationRef = new Location_Disk_FSRef();
					pLocationRef->put_FSSpec( pLocationSpec->get_FSSpec() );
				}
			#endif //FBL_SUPPORT_FSSPEC

			FSRef parentRef;
			OSStatus status = pLocationRef->get_ObjectFSRef( &parentRef );
			FBL_CHECK( status == 0 );
			argused1(status);

			OSErr err = FSCreateDirectoryUnicode(
							&parentRef,
							(vuint32) vu_strlen( inFolderName ),
							(UniChar*) inFolderName,
							kFSCatInfoNone, NULL, NULL, NULL, NULL );
			FBL_CHECK( err == 0 );
			argused1( err );

			// For MAC OS X its important to create ChildLocation AFTER
			// we have create disk folder, becuse then we get correct flag mObjectExists.
			pLoc = inParentLoc->get_ChildLocation( inFolderName );

		#endif // FBL_SUPPORT_FSREF
	#elif FBL_UNIX
		int err = mkdir( pLoc->get_Path().getBufferA(), S_IRWXU | S_IRGRP | S_IXGRP | S_IXOTH );
		FBL_CHECK( err == 0 );
		// Do it once more to set correct mIsDirectory flag and mFullPath.
		// Because Location::get_IsDirectory() called from Location::get_ChildLocation
		// for absent object returns false and treats the location as not-directory one
		// forever.
		//
		pLoc = inParentLoc->get_ChildLocation( inFolderName );
	#else // FBL_WIN
		#pragma error "Define CreateFolder() method for your platfrom."
	#endif // FBL_WIN

	return pLoc;
}


/**********************************************************************************************/
// Create folder if needed on the disk.
//
I_Location_Ptr PrepareFolder( I_Location_Ptr inParentLoc,  const char* inFolderName )
{
	String newFolderName( inFolderName );
	return PrepareFolder( inParentLoc, newFolderName.c_str() );
}


/**********************************************************************************************/
I_Location_Ptr PrepareFolder( I_Location_Ptr inParentLoc,  const UChar* inFolderName )
{
	// Location to be returned.
	I_Location_Ptr pLoc = inParentLoc->get_ChildLocation( inFolderName );
	I_Disk_Location_Ptr pDiskLoc = QI( pLoc, I_Disk_Location );
	if( pDiskLoc && pDiskLoc->get_Exists() == false )
	{
		pLoc = CreateFolder( inParentLoc, inFolderName );
	}
	
	return pLoc;
}


/**********************************************************************************************/
// Create folder if not exists.
//
I_Disk_Location_Ptr PrepareAppRelativeFolderLocation( I_Disk_Location_Ptr inpFolderLocation )
{
	I_Disk_Location_Ptr pRes = inpFolderLocation;
	
	if( inpFolderLocation && inpFolderLocation->get_Exists() == false )
	{
		// Create directory if needed.
		I_Location_Ptr pAppFolder = I_Disk_Location::GetAppLocation();
		String LogsCatalogPath( inpFolderLocation->get_Path() );

		#if FBL_MAC
			// nothing do
		#elif FBL_UNIX
			LogsCatalogPath << "/";
		#else
			LogsCatalogPath << "\\";
		#endif	// FBL_MAC
		
		I_Location_Ptr pNewFolder = CreateFolder( pAppFolder, LogsCatalogPath.c_str() );
		pRes = QI( pNewFolder, I_Disk_Location );
	}
	
	return pRes;
}


/**********************************************************************************************/
FBL_End_Namespace
