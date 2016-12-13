/**********************************************************************************************/
/* FBL_Util_Path_Mac.cpp 	                                               					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Utilities/FBL_Util_Path.h>
#include <VShared/FBL/publ/Util_classes/FBL_String.h>


/**********************************************************************************************/
FBL_Begin_Namespace

#if FBL_MAC 

#pragma mark -
#pragma mark ====== Not for use in 64 bit ======= 


#if FBL_SUPPORT_FSSPEC


/**********************************************************************************************/
/*void MakePathInTheSameFolder( FILE_PATH* inOtherFile, FILE_PATH* outPath, char* inFileName )
{
	*outPath = *inOtherFile;
	
	vuint8 pstr[256];
	FBL::c2pstrcpy( inFileName, pstr );
	
	memcpy( outPath->name, pstr, vuint32(pstr[0] + 1) );
}*/ 

#pragma mark -

//#if !FBL_MAC_MACHO 


/**********************************************************************************************/
OSErr MakeParentSpec( const FSSpec* inSpec, FSSpec* outParentSpec );
OSErr MakeParentSpec( const FSSpec* inSpec, FSSpec* outParentSpec )
{
	OSErr err = FSMakeFSSpec( inSpec->vRefNum, inSpec->parID, "\p", outParentSpec );
	
	return err;
}


/**********************************************************************************************/
OSErr MakeChildSpec( const FSSpec* inSpec, const char* inItemName, FSSpec* outChildSpec );
OSErr MakeChildSpec( const FSSpec* inSpec, const char* inItemName, FSSpec* outChildSpec )
{
	vint32 DirID;
	bool IsDir;
	GetDirectoryID( inSpec->vRefNum, inSpec->parID, inSpec->name, &DirID, &IsDir );
	
	Str255 pasName;
	c2pstrcpy(inItemName, pasName);
	
	OSErr err = FSMakeFSSpec( inSpec->vRefNum, DirID, pasName, outChildSpec );	
	
	return err;
}


/**********************************************************************************************/
// Define FSSpec for a given path folder.
//
bool GetFSpecOfFolder( const char* inPathToFind, FSSpec* outSpec );
bool GetFSpecOfFolder( const char* inPathToFind, FSSpec* outSpec )
{
	FSRef ref;
	Boolean isDir;
	
	OSStatus err = FSPathMakeRef( (UInt8*) inPathToFind, &ref, &isDir );
	
	if( err == 0 )
	{
		FSGetCatalogInfo( &ref, kFSCatInfoNone, NULL, NULL, outSpec, NULL );
		return true;
	}
	else
	{
		return false;
	}
}


#pragma mark -


/**********************************************************************************************/
// For CARBON this function returns spec of PARENT folder of app.
//
// FOR MACHO starting with 4.3 (no carbon) - it returns spec of APP.app folder.
//
void FindAppFolder( FSSpec* outSpec )
{
#if FBL_MAC_CARBON		
	
	ProcessSerialNumber	psn;
	ProcessInfoRec		info;
	
	info.processAppSpec = outSpec;
	info.processInfoLength = sizeof(info);
	info.processName = nil;
	
	::GetCurrentProcess(&psn);
	::GetProcessInformation(&psn,&info);  
	
	// now we have spec of app itself and go up to its directory.
	MakeParentSpec( outSpec, outSpec );
	
#else // FBL_MAC_CARBON
	
	FSRef BundleRef; 
	
	ProcessSerialNumber  currentProcess = { 0, kCurrentProcess }; 
	GetProcessBundleLocation( &currentProcess, &BundleRef );
	
	FSGetCatalogInfo( &BundleRef, kFSCatInfoNone, NULL, NULL, outSpec, NULL );
	
#endif // FBL_MAC_CARBON
}


/**********************************************************************************************/
#if FBL_MAC_MACHO																// MACHO
//
// Macho version search the APP path.
// Macho version expect to see for now only this configuration:
//		my.app/Contents/MacOS
//		my.app/Contents/vcomponents
//
void FindVcomponents( FSSpec* outCompSpec )
{
	OSErr err;
	
	// ---------------------------
	// APP/Contetns/vcomponents 
	//
	FSSpec appSpec;
	FindAppFolder( &appSpec );
	
	FSSpec contentsSpec;
	err = MakeChildSpec( &appSpec, "Contents", &contentsSpec );
	if( err == 0 )
	{
		// Components/vcomponents
		err = MakeChildSpec( &contentsSpec, VCOMPONENTS, outCompSpec );		
		
		// if we have found vcomponents inside of Contents folder, we return.
		// else we continue search for vcomponents in other places.
		if( err == 0 )
			return;
	}	
	
	// -------------------------
	// check "/usr/local/lib/vcomponents"
	// 
	GetFSpecOfFolder( "/usr/local/lib/" VCOMPONENTS, outCompSpec );
}
#endif // FBL_MAC_MACHO


/**********************************************************************************************/
#if FBL_MAC_CARBON																// CARBON
//
void FindVcomponents( FSSpec* outCompSpec )
{
	// 1) APP folders:
	FSSpec appSpec;
	FindAppFolder( &appSpec );
	
	FSSpec resSpec;	
	OSErr err = MakeChildSpec( &appSpec, "vresources", &resSpec );
	
	if( err == 0 )	
	{
		memcpy( outCompSpec, &appSpec, sizeof(FSSpec) );
		return;
	}
	
	// 2) HOME/Library/CFMSupport
	// todo
	
	
	// 3) /Library/CFMSupport/vcomponents
	GetFSpecOfFolder( "/Library/CFMSupport/vcomponents", outCompSpec );
}
#endif // FBL_MAC_CARBON


#pragma mark -


/**********************************************************************************************/
// This function can accept as FullPath, 
// so simple file name, then it build FSSpec relative to working directory.
//
OSErr Path2Spec_Carbon( const char *inPath, FSSpec *spec )
{
	AliasHandle		alias;
	OSErr			result;
	Boolean			wasChanged;
	Str32			nullString;

	Str255 pasStrPath;
    FBL::c2pstrcpy( inPath, pasStrPath, 255 );

	const char* pd = strchr( inPath, ':' );
	
	if( pd == NULL ) // we have simple name:
	{
		result = FSMakeFSSpec( 0, 0, pasStrPath, spec  );
	}
	else	
	{
		vint16 fullPathLength = *pasStrPath;
		
		/* Create a minimal alias from the full pathname */
		nullString[0] = 0;	/* null string to indicate no zone or server name */
		
		result = NewAliasMinimalFromFullPath(fullPathLength, pasStrPath + 1, nullString, nullString, &alias);

		if ( result == noErr )
		{
			/* Let the Alias Manager resolve the alias. */
			result = ResolveAlias(NULL, alias, spec, &wasChanged);
			
			DisposeHandle((Handle)alias);	/* Free up memory used */
		}
	}
	
	return ( result );
}


/**********************************************************************************************/
OSErr FSMakePath_Carbon(
	SInt16 			volRefNum,
	SInt32 			dirID,
	ConstStr255Param name,
	char*			path,
	vuint32 		maxPathSize)
{
	OSErr	result;
	FSSpec	spec;

	result = FSMakeFSSpec(volRefNum, dirID, name, &spec);
	if ( (result == noErr) || (result == fnfErr) )
	{
		vint16 fullPathLength = 0;
		Handle fullPath = NULL;
	
		result = FSpGetFullPath( &spec, &fullPathLength, &fullPath );
		
		vuint32 len = Min( vuint32(fullPathLength), maxPathSize - 1 );
		memcpy( path, *fullPath, len );	
		path[len] = 0; // add zero byte.
		
		DisposeHandle( fullPath );
	}

	return result;	
}


//#endif // FBL_MAC_MACHO



/**********************************************************************************************/
/**********************************************************************************************/
/**********************************************************************************************/
#pragma mark -
#pragma mark ==== MoreFiles code ====


/**********************************************************************************************/
OSErr GetCatInfoNoName(
	vint16 vRefNum,
	vint32 dirID,
	ConstStr255Param name,
	CInfoPBPtr pb)
{
	Str31 tempName;
	OSErr error;
	
	/* Protection against File Sharing problem */
	if ( (name == NULL) || (name[0] == 0) )
	{
		tempName[0] = 0;
		pb->dirInfo.ioNamePtr = tempName;
		pb->dirInfo.ioFDirIndex = -1;	/* use ioDirID */
	}
	else
	{
		pb->dirInfo.ioNamePtr = (StringPtr)name;
		pb->dirInfo.ioFDirIndex = 0;	/* use ioNamePtr and ioDirID */
	}
	pb->dirInfo.ioVRefNum = vRefNum;
	pb->dirInfo.ioDrDirID = dirID;
	error = PBGetCatInfoSync(pb);
	pb->dirInfo.ioNamePtr = NULL;
	return ( error );
}


/**********************************************************************************************/
OSErr GetDirectoryID(
	vint16 vRefNum,
	vint32 dirID,
	ConstStr255Param name,
	vint32 *theDirID,
	bool *isDirectory)
{
	CInfoPBRec pb;
	OSErr error;

	error = GetCatInfoNoName(vRefNum, dirID, name, &pb);
	if ( error == noErr )
	{
		*isDirectory = (pb.hFileInfo.ioFlAttrib & kioFlAttribDirMask) != 0;
		if ( *isDirectory )
		{
			*theDirID = pb.dirInfo.ioDrDirID;
		}
		else
		{
			*theDirID = pb.hFileInfo.ioFlParID;
		}
	}
	
	return ( error );
}


/**********************************************************************************************/
OSErr FSpGetDirectoryID(
	const FSSpec* inSpec,
	vint32 *theDirID,
	bool *isDirectory)
{
	return GetDirectoryID( inSpec->vRefNum, inSpec->parID, inSpec->name, theDirID, isDirectory );
}

 
/**********************************************************************************************/ 
OSErr FSpGetFullPath(
	const FSSpec *spec,
	vint16 *fullPathLength,
	Handle *fullPath )
{
	OSErr		result;
	OSErr		realResult;
	FSSpec		tempSpec;
	CInfoPBRec	pb;
	
	*fullPathLength = 0;
	*fullPath = NULL;
	
	
	/* Default to noErr */
	realResult = result = noErr;
	
	/* work around Nav Services "bug" (it returns invalid FSSpecs with empty names) */
	if ( spec->name[0] == 0 )
	{
		result = FSMakeFSSpec(spec->vRefNum, spec->parID, spec->name, &tempSpec);
	}
	else
	{
		/* Make a copy of the input FSSpec that can be modified */
		BlockMoveData(spec, &tempSpec, sizeof(FSSpec));
	}
	
	if ( result == noErr )
	{
		if ( tempSpec.parID == fsRtParID )
		{
			/* The object is a volume */
			
			/* Add a colon to make it a full pathname */
			++tempSpec.name[0];
			tempSpec.name[tempSpec.name[0]] = ':';
			
			/* We're done */
			result = PtrToHand(&tempSpec.name[1], fullPath, tempSpec.name[0]);
		}
		else
		{
			/* The object isn't a volume */
			
			/* Is the object a file or a directory? */
			pb.dirInfo.ioNamePtr = tempSpec.name;
			pb.dirInfo.ioVRefNum = tempSpec.vRefNum;
			pb.dirInfo.ioDrDirID = tempSpec.parID;
			pb.dirInfo.ioFDirIndex = 0;
			result = PBGetCatInfoSync(&pb);
			// Allow file/directory name at end of path to not exist.
			realResult = result;
			if ( (result == noErr) || (result == fnfErr) )
			{
				/* if the object is a directory, append a colon so full pathname ends with colon */
				if ( (result == noErr) && (pb.hFileInfo.ioFlAttrib & kioFlAttribDirMask) != 0 )
				{
					++tempSpec.name[0];
					tempSpec.name[tempSpec.name[0]] = ':';
				}
				
				/* Put the object name in first */
				result = PtrToHand(&tempSpec.name[1], fullPath, tempSpec.name[0]);
				if ( result == noErr )
				{
					/* Get the ancestor directory names */
					pb.dirInfo.ioNamePtr = tempSpec.name;
					pb.dirInfo.ioVRefNum = tempSpec.vRefNum;
					pb.dirInfo.ioDrParID = tempSpec.parID;
					do	/* loop until we have an error or find the root directory */
					{
						pb.dirInfo.ioFDirIndex = -1;
						pb.dirInfo.ioDrDirID = pb.dirInfo.ioDrParID;
						result = PBGetCatInfoSync(&pb);
						if ( result == noErr )
						{
							/* Append colon to directory name */
							++tempSpec.name[0];
							tempSpec.name[tempSpec.name[0]] = ':';
							
							/* Add directory name to beginning of fullPath */
							(void) Munger(*fullPath, 0, NULL, 0, &tempSpec.name[1], tempSpec.name[0]);
							result = MemError();
						}
					} while ( (result == noErr) && (pb.dirInfo.ioDrDirID != fsRtDirID) );
				}
			}
		}
	}
	
	if ( result == noErr )
	{
		/* Return the length */
		*fullPathLength = (vint16) GetHandleSize(*fullPath);
		result = realResult;	// return realResult in case it was fnfErr
	}
	else
	{
		/* Dispose of the handle and return NULL and zero length */
		if ( *fullPath != NULL )
		{
			DisposeHandle(*fullPath);
		}
		*fullPath = NULL;
		*fullPathLength = 0;
	}
	
	return ( result );
}

#endif // FBL_SUPPORT_FSSPEC


#pragma mark -
#pragma mark ====== Can be used in 64 bit ======= 


/**********************************************************************************************/
#if FBL_SUPPORT_FSREF


/**********************************************************************************************/
// Returns ZERO if file is not locked in any way, i.e. we can WRITE into such file.
//
OSErr FSCheckLock(
	const FSRef *ref)
{
	OSErr			result;
	FSCatalogInfo	catalogInfo;
	const FSPermissionInfo* pPermInfo;
	
	
	/* get nodeFlags and vRefNum for container */
	result = FSGetCatalogInfo( ref, 
				kFSCatInfoNodeFlags + kFSCatInfoVolume + kFSCatInfoPermissions, 
				&catalogInfo, NULL, NULL,NULL );
	
	pPermInfo = (const FSPermissionInfo*) &catalogInfo.permissions;
	
	if( (pPermInfo->mode & 0x80) == 0 ) // octal 200 = dec 128 = if owner is allowed to write?
	{
		result = fLckdErr;
	}
	else if ( 0 != (catalogInfo.nodeFlags & kFSNodeLockedMask) )		// is file locked?
	{		
		result = fLckdErr;	// file is locked
	}
	else
	{
		// file isn't locked, but is volume locked? 

		FSVolumeInfo	volumeInfo;
		
		/* get volume flags */
		result = FSGetVolumeInfo(catalogInfo.volume, 0, NULL, kFSVolInfoFlags, &volumeInfo, NULL, NULL);
		
		if ( 0 != (volumeInfo.flags & kFSVolFlagHardwareLockedMask) )
		{
			result = wPrErr;	/* volume locked by hardware */
		}
		else if ( 0 != (volumeInfo.flags & kFSVolFlagSoftwareLockedMask) )
		{
			result = vLckdErr;	/* volume locked by software */
		}
	}
	
	return result;
}
 
 
/**********************************************************************************************/
#endif // FBL_SUPPORT_FSREF
 
 
/**********************************************************************************************/
#endif // FBL_MAC


/**********************************************************************************************/
FBL_End_Namespace
