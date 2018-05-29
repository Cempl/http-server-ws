/**********************************************************************************************/
/* FBL_Util_Path_Mac.cpp 	                                               					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Util_classes/FBL_String.h>
#include <VShared/FBL/publ/Utilities/FBL_Util_Path.h>
#include <VShared/FBL/publ/Utilities/FBL_Util_PathA.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
#if FBL_MAC


/**********************************************************************************************/
// NOTE:  fullPath to item that EXISTS on disk. Otherwise file not found error.
//
#if FBL_SUPPORT_FSSPEC
//
OSErr Path2Spec_FSRef( const char *inPath, FSSpec *spec)
{
	// parse path on parent folder and file name. 
	const char* pFileName = ExtractNameFromFullPath(inPath);
	const char* pDelim = pFileName - 1;

//	String folder_path( inPath, vuint32(pDelim - inPath) );

	vuint32 len;
	if( pDelim >= inPath )
		len = vuint32(pDelim - inPath);
	else
		len = strlen( inPath );
	
	MemPtr<char> folder_path( inPath, len + 1 );
	folder_path[len] = 0;
	
	char* pFolderPath = folder_path.begin();
	
	// convert the POSIX path to an FSRef of parent folder 
	FSRef ref;
	OSStatus result = FSPathMakeRef( (vuint8*) pFolderPath, &ref, NULL);
	
	if( result != 0 )
		return (OSErr) result;

	// and then convert the FSRef to an FSSpec 
	FSSpec parent;
	result = FSGetCatalogInfo(&ref, kFSCatInfoNone, NULL, NULL, &parent, NULL);
	if( result != 0 )
		return (OSErr) result;

	// now we have parent folder FSSpec, get its ID. 
	vint32 theDirID;
	bool isDirectory;
	GetDirectoryID( parent.vRefNum, parent.parID, parent.name, &theDirID, &isDirectory );
	
	// and now we make File spec:
	Str255 pasFileName;
	FBL::c2pstrcpy( pFileName, pasFileName );
	result = FSMakeFSSpec( parent.vRefNum, theDirID, pasFileName, spec );
	
	return (OSErr) result;
}

#endif // FBL_SUPPORT_FSSPEC

#endif // FBL_MAC


/**********************************************************************************************/
#if FBL_MAC_MACHO


#pragma mark -
#pragma mark ====== Not for use in 64 bit ======= 


#pragma mark -
#pragma mark ====== Can be used in 64 bit ======= 



/**********************************************************************************************/
#if FBL_SUPPORT_FSREF
 

/**********************************************************************************************/
OSErr FSMakePath_FSRef(
	SInt16 				volRefNum,
	SInt32 				dirID,
	ConstStr255Param 	name,
	char*				path,
	vuint32 			maxPathSize)
{
	OSStatus	result;
	FSRef		ref;
	
	// convert the inputs to an FSRef
	result = FSMakeFSRef(volRefNum, dirID, name, &ref);
	
	// and then convert the FSRef to a path
	result = FSRefMakePath(&ref, (UInt8*) path, maxPathSize);
	
	return (OSErr) result;
}


/**********************************************************************************************/ 
OSErr FSMakeFSRef(
	FSVolumeRefNum volRefNum,
	SInt32 dirID,
	ConstStr255Param name,
	FSRef *ref)
{
	OSErr		result;
	FSRefParam	pb;
	
	pb.ioVRefNum = volRefNum;
	pb.ioDirID = dirID;
	pb.ioNamePtr = (StringPtr)name;
	pb.newRef = ref;

#if __LP64__
	result = PBMakeFSRefUnicodeSync(&pb);
#else
	result = PBMakeFSRefSync(&pb);
#endif // __LP64__
	
	return ( result );
}


/**********************************************************************************************/
#endif // FBL_SUPPORT_FSREF


/**********************************************************************************************/
#endif // #if FBL_MAC_MACHO


/**********************************************************************************************/
FBL_End_Namespace


