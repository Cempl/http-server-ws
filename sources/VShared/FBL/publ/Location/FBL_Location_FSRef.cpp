/**********************************************************************************************/
/* FBL_Location_FSRef.cpp              		                                  				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Location/FBL_Location_FSRef.h>

#include <VShared/FBL/publ/Util_classes/FBL_String.h>
#include <VShared/FBL/publ/Utilities/FBL_Util_Path.h>
#include <VShared/FBL/publ/Utilities/FBL_Util_PathA.h>
#include <VShared/FBL/publ/Starter/FBL_Starter.h>
#include <VShared/FBL/publ/Util_classes/FBL_StringConversion.h>


/**********************************************************************************************/
#if FBL_SUPPORT_FSREF

#if FBL_MAC && !FBL_MAC_MACHO		
#include <StandardFile.h>
#endif // FBL_MAC && if !FBL_MAC_MACHO


/**********************************************************************************************/
FBL_Begin_Namespace 


/**********************************************************************************************/
// RZ 2010-04-29: we make here COPY of FBL_Location.cpp factory.
// to be able use it in fake mode for fbl::vsql::LocalConnection::UseDumbDatabase() 
// ONLY for MAC 64 bit 
//
//#if !FBL_SUPPORT_FSSPEC 

I_Disk_Location* CreateDiskLocation( const char* inStr )
{
	String s( inStr );
	return new Location_Disk_FSRef( &s );
}
//#endif // !FBL_SUPPORT_FSSPEC 


/**********************************************************************************************/
#if !FBL_SUPPORT_FSSPEC
I_Disk_Location* CreateDiskLocation( const UChar* inStr )
{
	String s( inStr );
	return new Location_Disk_FSRef( &s );
}
#endif // !FBL_SUPPORT_FSSPEC


/**********************************************************************************************/
// Note, that Location object makes a COPY of the given string !!!
//
I_Disk_Location* CreateDiskLocationFromFSRef( const UChar* inStr )
{
	String s( inStr );
	return new Location_Disk_FSRef( &s );
}

/**********************************************************************************************/
// Note, that Location object makes a COPY of the given string !!!
//
I_Disk_Location* CreateDiskLocationFromFSRef( const char* inStr )
{
	String s( inStr );
	return new Location_Disk_FSRef( &s );
}


/**********************************************************************************************/
I_Disk_Location* CreateDiskLocationFromFSRef( 
	HFSUniStr255* 	inName,
	const FSRef* 	inExistedRef )
{
	return new Location_Disk_FSRef( inName, inExistedRef );
}
		
/**********************************************************************************************/
I_Disk_Location* CreateDiskLocationFromFSRef( 
	const FSRef* 	inParentRef,
	HFSUniStr255& 	inName )
{
	return new Location_Disk_FSRef( inParentRef, inName );
}
		

/**********************************************************************************************/
// RZ 10/22/09 
//
I_Disk_Location* CreateDiskLocationFromFSRef( CFURLRef inUrlRef )
{
/*
	 FSRef ref;
	 Boolean res = CFURLGetFSRef( inUrlRef, &ref );
	 
	 return new Location_Disk_FSRef( ref ); 
	 // but oops: we do not have such ctor in this class.
	 // and it not looks simple ... may be possible
	 // so for now dirty force solution. URL => UTF8 posix path => existed ctor
 */	
	CFStringRef path_ref = CFURLCopyFileSystemPath( inUrlRef, kCFURLPOSIXPathStyle );
	
	char cpath_utf8[PATH_MAX];
	Boolean cfres = CFStringGetCString( path_ref, cpath_utf8, sizeof(cpath_utf8)-1, kCFStringEncodingUTF8 );
	argused1( cfres );

	CFRelease( path_ref );
	
	String s( cpath_utf8, -1, GetConverter_UTF8() );
	Location_Disk_FSRef* ploc = new Location_Disk_FSRef( &s );

	return ploc;
}


#pragma mark -

 
/**********************************************************************************************/
// Note, that Location object makes a COPY of the given string !!!
//
Location_Disk_FSRef::Location_Disk_FSRef( 
	const String* inFullPath )
:
	Location( inFullPath )
{
	memset( &mParentFSRef, 0, sizeof(mParentFSRef) );
	memset( &mObjectFSRef, 0, sizeof(mParentFSRef) );

	mParentRefExists = false;
	mObjectRefExists = false;
	
	mName.length = 0;
	
	if( inFullPath == NULL )
		return;
	
	// Convert path to FSRef:
	StFromUTF16 path( inFullPath->c_str(), inFullPath->length(), GetConverter_UTF8() );
	char* pPathUTF8 = (char*) path.c_str();
	
	OSStatus res = FSPathMakeRef( (UInt8*) pPathUTF8, &mObjectFSRef, NULL );
	if( res == 0 )
	{
		// We have found existed object on disk.
		mObjectRefExists = true;	

		res = FSGetCatalogInfo( 
					&mObjectFSRef, kFSCatInfoNone, NULL, &mName, NULL, &mParentFSRef );

		mParentRefExists = bool(res == 0);
	}
	else if( res == fnfErr )
	{
		// then we check if this is a full path. Delimiter is POSIX.
		const UChar* pName = ExtractNameFromFullPath( inFullPath->c_str(), UChar('/') );
		if( pName )
		{
			// In this case we split path into pair: folder + name
			mName.length = (vuint16) vu_strlen( pName );
			memcpy( mName.unicode, pName, (mName.length + 1) * sizeof(UChar) );
			
			String pathUtf16( path.c_str() );
			vuint32 FolderLen = vuint32( pathUtf16.length() - mName.length );
			
			if( FolderLen == 0 ) // we have only file name on hands. Then we make this file in the APP folder.
			{
				I_Disk_Location_Ptr pAppLocation = I_Disk_Location::GetAppLocation();
				Location_Disk_FSRef_Ptr pAppLocation2 = fbl_dynamic_cast<Location_Disk_FSRef>( pAppLocation ); 
				
				pAppLocation2->get_ObjectFSRef( &mParentFSRef );
				mParentRefExists = true;
			}	
			else // FolderLen > 0
			{
				// can produce strings as "/" or "/Volumes/v1/folder/"
				// i.e. have end backslash. Needed to work with "/" case.
				pPathUTF8[ FolderLen ] = 0;
			
				res = FSPathMakeRef( (UInt8*) pPathUTF8, &mParentFSRef, NULL );
				mParentRefExists = (res == 0) ? true : false; 
			}
		}
	}
}


/**********************************************************************************************/
/*
Location_Disk_FSRef::Location_Disk_FSRef( 
	const char* inFullPath_UTF8 )
:
	Location( new String(inFullPath_UTF8) )
{
	memset( &mParentFSRef, 0, sizeof(mParentFSRef) );
	memset( &mObjectFSRef, 0, sizeof(mParentFSRef) );

	mParentRefExists = false;
	mObjectRefExists = false;
	
	mName.length = 0;
	
	if( inFullPath_UTF8 == NULL )
		return;
		
	OSStatus res = FSPathMakeRef( (UInt8*) inFullPath_UTF8, &mObjectFSRef, NULL );
	if( res == 0 )
	{
		mObjectRefExists = true;	

		res = FSGetCatalogInfo( 
					&mObjectFSRef, kFSCatInfoNone, NULL, &mName, NULL, &mParentFSRef );

		mParentRefExists = bool(res == 0);
	}
	else if( res == fnfErr )
	{
		// RZ: inFullPath_UTF8 is CONST str. We need make copy to be able modify it.
		vuint32 pathLen = strlen( inFullPath_UTF8 );
		MemPtr<char> PathBuff( pathLen + 1 );
		memcpy( PathBuff.begin(), inFullPath_UTF8, pathLen + 1 );
	
		// We check if this is a full path. We work on carbon, but delimiter is POSIX.
		const char* pName = ExtractNameFromFullPath( PathBuff.begin(), '/' );
		if( pName )
		{
			// In this case we split it into folder + name
			mName.length = (vuint16) strlen( pName );
			
			vuint32 FolderLen = vuint32( pathLen - mName.length ); 
			
			// Convert UTF8 to UTF16 for NAME of file.
			{
				CFStringRef u16_name;
				u16_name = CFStringCreateWithCString( NULL, pName, kCFStringEncodingUTF8 );						
				
				CFRange rangeToProcess = CFRangeMake(0, CFStringGetLength(u16_name));
				CFStringGetCharacters( u16_name, rangeToProcess, mName.unicode );
				CFRelease( u16_name );
			}
			
			// can produce strings as "/" or "/Volumes/v1/folder/"
			// i.e. have end backslash. Needed to work with "/" case.
			if( FolderLen > 0 )
			{
				char* p = (char*) PathBuff.begin();
				p[ FolderLen ] = 0;
			}
			
			res = FSPathMakeRef( (UInt8*) PathBuff.begin(), &mParentFSRef, NULL );
			if( res == 0 )
			{
				mParentRefExists = true;
			}
			
		}
	}
}*/


/**********************************************************************************************/
// Ctor for case when we have FSRef of an _EXISTED_ object. 
// We put the given inObjectRef into mObjectFSRef and extract name of object from OS.
// We need still have a copy of name to be able Create() the same object AFTER ThrowOut(). 
//
Location_Disk_FSRef::Location_Disk_FSRef( 
	HFSUniStr255* 	inName,
	const FSRef* 	inObjectRef ) 
: 
	Location( (UChar*) NULL )
{ 
	mObjectFSRef = *inObjectRef;
	mObjectRefExists = true;

	// If we get name then store it, otherwise we will ask OS about name.
	HFSUniStr255 tmpName;	
	HFSUniStr255* pName = NULL;

	if( inName == NULL || inName->length == 0 )
	{
		pName = &mName;
	}
	else
	{
		mName = *inName;
		pName = &tmpName;
	}

	OSStatus res = FSGetCatalogInfo( 
		inObjectRef, kFSCatInfoNone, NULL, pName, NULL, &mParentFSRef );

	mParentRefExists = bool(res == 0);
}	


/**********************************************************************************************/
// Ctor for case of NOT EXISTED object. We get the parent folder FSRef and the name of object.
// In the same time this ctor can be used for EXISTED object, so we check this point
// trying create FSref of object. 
// 
//
Location_Disk_FSRef::Location_Disk_FSRef( 
	const FSRef* 	inParentRef,
	HFSUniStr255& 	inName ) 
: 
	Location( (UChar*) NULL )
{ 
	// we store the name of object in any case.
	mName = inName; // copy of structure. 

	mParentFSRef = *inParentRef;
	mParentRefExists = true;

	// -----------------------
	// We try first of all build FSref for specified pair.
	// May happens that object exist.
	OSStatus err = FSMakeFSRefUnicode( 
						inParentRef, inName.length, (UniChar*) inName.unicode, 0, &mObjectFSRef );

	mObjectRefExists = bool( err == 0 );
}	


/**********************************************************************************************/
Location_Disk_FSRef::Location_Disk_FSRef( 
	const Location_Disk_FSRef& inOther )
:
	Location( inOther )
{
	mParentRefExists = inOther.mParentRefExists;	
	mObjectRefExists = inOther.mObjectRefExists;	
	
	if( mParentRefExists )
		mParentFSRef = inOther.mParentFSRef;	

	if( mObjectRefExists )
		mObjectFSRef = inOther.mObjectFSRef;
	
	mName = inOther.mName;
}


/**********************************************************************************************/
Location_Disk_FSRef::~Location_Disk_FSRef( void ) 
{	
}


/**********************************************************************************************/
Location_Disk_FSRef& Location_Disk_FSRef::operator=( const Location_Disk_FSRef& inOther )
{
	mParentRefExists = inOther.mParentRefExists;	
	mObjectRefExists = inOther.mObjectRefExists;	
	
	if( mParentRefExists )
		mParentFSRef = inOther.mParentFSRef;	

	if( mObjectRefExists )
		mObjectFSRef = inOther.mObjectFSRef;
	
	mName = inOther.mName;

	// also we check that may be inOther have path?
	ForgetObject( mpFullPath );

	if( inOther.mpFullPath )
		mpFullPath = new String( *inOther.mpFullPath );
		
	return *this;	
}


#pragma mark -


/**********************************************************************************************/
const String& Location_Disk_FSRef::get_Path( void ) const 
{ 
	if( mpFullPath )
		return *mpFullPath;
	
	UInt8 Path[1024];
	OSStatus err = 0;
		
	String* ps = NULL;	
		
	if( mObjectRefExists )	
	{
		// we have FSref of an existed object.
		err = FSRefMakePath( &mObjectFSRef,	Path, 1024 );
		
		ps = new String( (char*)Path, -1, GetConverter_UTF8() );
	}
	else
	{
		// To get the full page of NOT-existed object we at first get path for parent folder
		// then append the name of item.
		err = FSRefMakePath( &mParentFSRef, Path, 1024 );
		
		ps = new String( (char*)Path, -1, GetConverter_UTF8() );

		*ps += UChar('/'); // always POSIX delimiter here
		ps->append( (UChar*) mName.unicode, mName.length );
	}

	mpFullPath = ps;
	return *mpFullPath;
}


/**********************************************************************************************/
void Location_Disk_FSRef::put_Path( const UChar* inFullPath )  
{
	Location::put_Path( inFullPath );
		
	mParentRefExists = false;
}		


/**********************************************************************************************/
void Location_Disk_FSRef::put_Path( const String& inFullPath )  
{
	ForgetObject( mpFullPath ); 
	
	if( !inFullPath.isEmpty() ) 
		mpFullPath = new String( inFullPath );
		
	mParentRefExists = false;
}		


/**********************************************************************************************/
bool Location_Disk_FSRef::get_IsUndefined( void ) const 
{
	if( mpFullPath == NULL && !mParentRefExists )
		return true;
	else
		return false;
}


#pragma mark -


/**********************************************************************************************/
const String& Location_Disk_FSRef::get_FileName( void ) const 
{
static String sName;

	sName.setTo( (UChar*) mName.unicode, mName.length );

	return sName;
}


/**********************************************************************************************/
bool Location_Disk_FSRef::get_Exists( void ) const 
{ 
	OSStatus err;

	if( mObjectRefExists )
	{
		FSRef dummy;
		err = FSMakeFSRefUnicode( &mParentFSRef, mName.length, (UniChar*) mName.unicode, 0, &dummy );
		
		return (err == fnfErr) ? false : true;
	}
	else
	{
		// This file was absent on crete-this-object moment:
		const char* pathA = get_Path().getBufferA();
		return (access(pathA, F_OK) != -1);
	}
}


/**********************************************************************************************/
bool Location_Disk_FSRef::get_IsDirectory( void ) const 
{
	bool res = false;

	if( mIsDirectory == kUnknown ) // we need to use OS to define the result.
	{
		FSRef ref;
		get_ObjectFSRef( &ref );
	
        FSCatalogInfo catalogInfo;

        OSStatus err = FSGetCatalogInfo(
							&ref,
							kFSCatInfoNodeFlags,
							&catalogInfo,
							NULL,
							NULL,
							NULL );
		
        if( (err == noErr) && (catalogInfo.nodeFlags & kFSNodeIsDirectoryMask) )
            res = true;
	}
	else
	{
		res = bool( mIsDirectory == kDirectory );
	}
	
	return res;
}


/**********************************************************************************************/
I_Location_Ptr Location_Disk_FSRef::get_ChildLocation( const char* inName ) const
{
	String uname( inName );

	return get_ChildLocation( uname );	
}


/**********************************************************************************************/
I_Location_Ptr Location_Disk_FSRef::get_ChildLocation( const UChar* inName ) const
{
	String uname( inName );

	return get_ChildLocation( uname );	
}


/**********************************************************************************************/
I_Location_Ptr Location_Disk_FSRef::get_ChildLocation( const String& inName ) const
{
	if( !mObjectRefExists ) 
		throw xOSFileError( (ERROR_TYPE)  fnfErr );

	Location_Disk_FSRef_Ptr pChildLocation;

	HFSUniStr255 name;
	
	name.length = (vuint16) inName.length();
	memcpy( name.unicode, inName.c_str(), name.length * sizeof(UChar) );
	
	// we can get child location of only existed object. so
	pChildLocation = new Location_Disk_FSRef( &mObjectFSRef, name );	
	
	// RZ 11/28/09: inName can be relative path from THIS location.
	// We will use CFURL features to TRY resolve this.
	if( pChildLocation->mObjectRefExists == false )
	{		
		CFURLRef this_url = CFURLCreateFromFSRef( kCFAllocatorDefault, &mObjectFSRef );

		CFStringRef addStrRef = CFStringCreateWithCharacters( 
									kCFAllocatorDefault, inName.c_str(), inName.length() );
		
		// RZ: last parameter is FALSE for now, i.e. NOT a directory.
		// I think for directory this func adds the ending slash...
		CFURLRef file_url = CFURLCreateCopyAppendingPathComponent(
								kCFAllocatorDefault, this_url, addStrRef, false );
			
		FSRef fref;
		Boolean bres = CFURLGetFSRef( file_url, &fref );
		
		CFRelease( this_url );
		CFRelease( file_url );
		CFRelease( addStrRef );
		
		if( bres == true ) // ONLY if we was able find this file we create another Location_Disk_FSRef object.
			pChildLocation = new Location_Disk_FSRef( NULL, &fref );	// location of existed object
	}

	return pChildLocation;
}


/**********************************************************************************************/
I_Location_Ptr Location_Disk_FSRef::get_ParentDirectory( void ) const
{
	if( !mParentRefExists ) 
		throw xOSFileError( (ERROR_TYPE)  fnfErr );

	return new Location_Disk_FSRef( NULL, &mParentFSRef );
}


#pragma mark -


/**********************************************************************************************/
// Overrdien to convert FSRef into FSSPec.
//
#if !__LP64__
const FSSpec* Location_Disk_FSRef::get_FSSpec( void ) const
{
/*	OSStatus result = FSGetCatalogInfo( &mParentFSRef, kFSCatInfoNone, NULL, NULL, &mSpec, NULL );	

	if( mName.length != 0 ) // we have parent + name of non-existed object
	{
		// so we must get child object of that prarent FSSPec;
		StFromUTF16 fnameA( (UChar*)mName.unicode, mName.length );
		
		FSSpec child;
		MakeChildSpec( &mSpec, fnameA.c_str(), &child );
		
		mSpec = child;
	}	
	
	return &mSpec;*/
	
	return NULL;
} 
#endif // __LP64__


/**********************************************************************************************/
#if FBL_SUPPORT_FSSPEC
void Location_Disk_FSRef::put_FSSpec( const FSSpec* inSpec )
{
	OSStatus err; 
	
	err= FSpMakeFSRef( inSpec, &mObjectFSRef );

	if( err == 0 )	
	{
		put_ObjectFSRef( &mObjectFSRef );
	}
	else // we have not existing object:
	{
		mObjectRefExists = false;
	
		// so we need obtain FSRef of parent folder, then use name of FSSepc.
		FSSpec parentSpec;
		MakeParentSpec( inSpec, &parentSpec );
		
		err = FSpMakeFSRef( &parentSpec, &mParentFSRef );
		mParentRefExists = bool( err == 0 );
		
		mName.length = inSpec->name[0];
		vu_a2u( (char*) inSpec->name + 1, (UChar*) mName.unicode, mName.length );
	}
}	
#endif // FBL_SUPPORT_FSSPEC


#pragma mark -


/**********************************************************************************************/
const FSRef* Location_Disk_FSRef::get_ParentFSRef( void ) const
{
	// If Location was created by Path, we must CONVERT Path to FSRef.
	if( !mParentRefExists && mpFullPath ) 
	{
		// mParentRefExists = true;
	}

	return &mParentFSRef;
} 


/**********************************************************************************************/
OSStatus Location_Disk_FSRef::get_ObjectFSRef( FSRef* outRef ) const
{
	if( mObjectRefExists )	// we have FSref of existed object
	{
		*outRef = mObjectFSRef;	
		return 0;
	}
	else
	{
		OSStatus err = FSMakeFSRefUnicode( 
			&mParentFSRef, mName.length, (UniChar*) mName.unicode, 0, outRef );
		
		if( err == 0 )
		{
			// save result into 'this' also. ala-caching of result.
			mObjectFSRef = *outRef;	
			mObjectRefExists = true;
		}
		
		return err;
	}
}


/**********************************************************************************************/
void Location_Disk_FSRef::put_ObjectFSRef( const FSRef* inObjectRef )
{
	if( inObjectRef == NULL )
	{
		// when file/folder is deleted, we are informed about this, 
		// to corret mObjectRefExists flag.
		mObjectRefExists = false;
	}
	else
	{
		mObjectFSRef 		= *inObjectRef;	// copy structures here.
		mObjectRefExists 	= true;

		// If we have no yet name of this object
		HFSUniStr255* pName = NULL;
		if( mName.length == 0 ) 
			pName = &mName;
			
		OSStatus res = FSGetCatalogInfo( 
							inObjectRef, kFSCatInfoNone, NULL, pName, NULL, &mParentFSRef );

		mParentRefExists = bool( res == 0 );
	}

	ForgetObject( mpFullPath );
}	


/**********************************************************************************************/
FBL_End_Namespace

#endif // FBL_SUPPORT_FSREF
