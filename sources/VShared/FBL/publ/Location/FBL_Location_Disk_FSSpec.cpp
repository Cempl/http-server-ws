/**********************************************************************************************/
/* FBL_Location_Disk_FSSpec.cpp            		                               				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Location/FBL_Location_Disk_FSSpec.h>

#include <VShared/FBL/publ/Util_classes/FBL_String.h>
#include <VShared/FBL/publ/Util_Strings/FBL_ustdio.h>
#include <VShared/FBL/publ/Utilities/FBL_Util_Path.h>
#include <VShared/FBL/publ/Starter/FBL_Starter.h>
#include <VShared/FBL/publ/Util_classes/FBL_StringConversion.h>

#include <VShared/FBL/publ/Utilities/FBL_Util_PathA.h>


/**********************************************************************************************/
#if FBL_SUPPORT_FSSPEC

#if FBL_MAC && !FBL_MAC_MACHO		// i.e. FBL_CARBON
#include <StandardFile.h>
#endif // FBL_MAC && if !FBL_MAC_MACHO


/**********************************************************************************************/
FBL_Begin_Namespace 


/**********************************************************************************************/
#if FBL_MAC_MACHO
bool gUseCarbon = false;
#else
bool gUseCarbon = true;
#endif // FBL_MAC_MACHO


/**********************************************************************************************/
// SWITCHER method to choose runtime between CARBON and POSIX style of paths.
// First was needed for V4MD UB, because fucking Director11 still use CARBON paths. 
// 
OSErr Path2Spec( const char *inPath, FSSpec *spec );
OSErr Path2Spec( const char *inPath, FSSpec *spec )
{
	if( gUseCarbon )
		return Path2Spec_Carbon( inPath, spec );
	
#if !FBL_MAC_CARBON	
	else
		return Path2Spec_FSRef( inPath, spec );
#endif // !FBL_MAC_CARBON		
}


/**********************************************************************************************/
// For MacOS CARBON we use other version of the CreateDiskLocation() factory.
//
I_Disk_Location* CreateDiskLocation( const UChar* inStr )
{
	if( gUseCarbon )
	{
		String fullPath(inStr);
		return new Location_Disk_FSSpec( &fullPath );
	}
	else // i.e. we have MACHO 
	{
		I_Disk_Location_Ptr pLocation = CreateDiskLocationFromFSRef( inStr );
		if( !pLocation->get_Exists() )
		{
			I_Location_Ptr pa = I_Disk_Location::GetAppLocation();
			I_Location_Ptr pl = pa->get_ChildLocation( inStr );
			
			I_Disk_Location* pdl = dcast<I_Disk_Location*>( pl.get() ); 
			pdl->AddRef();
			
			return pdl;
		}
		else
		{
			I_Disk_Location* pdl = dcast<I_Disk_Location*>( pLocation.get() ); 
			pdl->AddRef();

			return pdl;
		}
	}	
}


/**********************************************************************************************/
// We use FSSpec only if htere is no support of FSRef.
// Otherwise we always must prefer FSRef.
// 
//#if !FBL_SUPPORT_FSREF

I_Disk_Location* CreateDiskLocationFromFSSpec( const char* inStr )
{
	FSSpec spec;
	OSErr err = Path2Spec( inStr, &spec ); 

	if( gUseCarbon )
	{
		return new Location_Disk_FSSpec( &spec );
	}
	else // i.e. we have MACHO
	{
		if( err == fnfErr )	// so inStr is relative path, then we go relative to app path:
		{
			I_Location_Ptr pa = I_Disk_Location::GetAppLocation();
			I_Location_Ptr pl = pa->get_ChildLocation( inStr );
			
			I_Disk_Location* pdl = dcast<I_Disk_Location*>( pl.get() ); 
			pdl->AddRef();
			
			return pdl;
		}
		else
		{
			return new Location_Disk_FSSpec( &spec ); 
		}
	}	
}

//#endif // !FBL_SUPPORT_FSREF


#pragma mark -


/**********************************************************************************************/
I_Disk_Location* CreateDiskLocationFromFSSpec( const FSSpec* inSpec )
{
	return new Location_Disk_FSSpec(inSpec);
}


#pragma mark -

 
/**********************************************************************************************/
Location_Disk_FSSpec::Location_Disk_FSSpec( const String* inFullPath )
:
	Location( inFullPath )
{
	mSpecExists = false;	
	memset( &mFSSpec, 0, sizeof(mFSSpec) );
}


/**********************************************************************************************/
Location_Disk_FSSpec::Location_Disk_FSSpec( const Location_Disk_FSSpec& inOther )
:
	Location( inOther )
{
	mSpecExists = inOther.mSpecExists;	
	mFSSpec = inOther.mFSSpec;		
}


/**********************************************************************************************/
Location_Disk_FSSpec::~Location_Disk_FSSpec( void ) 
{	
	ForgetObject( mpFullPath ); 
}


/**********************************************************************************************/
Location_Disk_FSSpec& Location_Disk_FSSpec::operator=( const Location_Disk_FSSpec& inOther )
{
	mSpecExists = inOther.mSpecExists;	
	mFSSpec = inOther.mFSSpec;

	// also we check that may be inOther have path?
	ForgetObject( mpFullPath );

	if( inOther.mpFullPath )
		mpFullPath = new String( *inOther.mpFullPath );
		
	return *this;	
}


#pragma mark -


/**********************************************************************************************/
const String& Location_Disk_FSSpec::get_Path( void ) const 
{ 
	if( mpFullPath )
		return *mpFullPath;
		
	char Path[1024];

#if FBL_MAC_MACHO
	OSErr err = gUseCarbon
							? FSMakePath_Carbon( mFSSpec.vRefNum, mFSSpec.parID, mFSSpec.name, Path, 1024 )
							: FSMakePath_FSRef ( mFSSpec.vRefNum, mFSSpec.parID, mFSSpec.name, Path, 1024 );
#else
	OSErr err = FSMakePath_Carbon( mFSSpec.vRefNum, mFSSpec.parID, mFSSpec.name, Path, 1024 );
#endif // FBL_MAC_MACHO

	argused1(err);							
	
	String* ps = new String( Path );
	mpFullPath = ps;
	
	return *mpFullPath;
}


/**********************************************************************************************/
void Location_Disk_FSSpec::put_Path( const UChar* inFullPath )  
{
	Location::put_Path( inFullPath );
		
	mSpecExists = false;
}		


/**********************************************************************************************/
void Location_Disk_FSSpec::put_Path( const String& inFullPath )  
{
	ForgetObject( mpFullPath ); 
	
	if( !inFullPath.isEmpty() ) 
		mpFullPath = new String( inFullPath );
		
	mSpecExists = false;
}		


/**********************************************************************************************/
bool Location_Disk_FSSpec::get_IsUndefined( void ) const 
{
	if( mpFullPath == NULL && !mSpecExists )
		return true;
	else
		return false;
}


#pragma mark -


/**********************************************************************************************/
const String& Location_Disk_FSSpec::get_FileName( void ) const 
{
static String Name;

	const FSSpec* ps = get_FSSpec();
	
	char cName[65];
	p2cstrcpy( (StringPtr) ps->name, cName, sizeof(cName) );
	
	Name = cName;

	return Name;
}


/**********************************************************************************************/
bool Location_Disk_FSSpec::get_Exists( void ) const 
{ 
	const FSSpec* pSpec = get_FSSpec();

	FInfo fileInfo;
	OSErr err = ::FSpGetFInfo(pSpec, &fileInfo);
	
	if( err == fnfErr ) // there is no such file, may be exists such directory?
	{
		CInfoPBRec pb;

		pb.dirInfo.ioNamePtr = (StringPtr) &(pSpec->name);
		pb.dirInfo.ioVRefNum = pSpec->vRefNum;
		pb.dirInfo.ioDrDirID = pSpec->parID;
		pb.dirInfo.ioFDirIndex = 0;

		err = PBGetCatInfoSync(&pb);
	}
	
	return bool( err == noErr );
}


/**********************************************************************************************/
bool Location_Disk_FSSpec::get_IsDirectory( void ) const 
{
	bool res = false;
	
	if( mIsDirectory == kUnknown ) // we need to use OS to define the result.
	{
		const FSSpec* pSpec = get_FSSpec();
		
		CInfoPBRec pb;
		pb.dirInfo.ioNamePtr = (StringPtr) &(pSpec->name);
		pb.dirInfo.ioVRefNum = pSpec->vRefNum;
		pb.dirInfo.ioDrDirID = pSpec->parID;
		pb.dirInfo.ioFDirIndex = 0;

		OSErr err = PBGetCatInfoSync(&pb);
		
		if( (err == noErr) && (pb.hFileInfo.ioFlAttrib & kioFlAttribDirMask) != 0 )
			res = true;
	}
	else
	{
		res = bool( mIsDirectory == kDirectory );
	}
	
	return res;
}


/**********************************************************************************************/
I_Location_Ptr Location_Disk_FSSpec::get_ChildLocation( const char* inName ) const
{
	const FSSpec* curr = get_FSSpec();

	vint32 DirID;
	bool IsDir;
	GetDirectoryID( curr->vRefNum, curr->parID, curr->name, &DirID, &IsDir );

	Str255 pasName;
	c2pstrcpy(inName, pasName);
	
	FSSpec childSpec;
	OSErr err = FSMakeFSSpec( curr->vRefNum, DirID, pasName, &childSpec );
	argused1(err);

	return new Location_Disk_FSSpec( &childSpec );
}


/**********************************************************************************************/
I_Location_Ptr Location_Disk_FSSpec::get_ChildLocation( const String& inName ) const
{
	StFromUTF16 name( inName.c_str(), inName.length() );
	return get_ChildLocation( name.c_str() );
}

/**********************************************************************************************/
I_Location_Ptr Location_Disk_FSSpec::get_ParentDirectory( void ) const
{
	FSSpec parent;
	MakeParentSpec( &mFSSpec, &parent );
	
	return new Location_Disk_FSSpec( &parent );
}


#pragma mark -


/**********************************************************************************************/
const FSSpec* Location_Disk_FSSpec::get_FSSpec( void ) const
{
	// If Location was created by Path, we must CONVERT Path to FSSpec.
	if( !mSpecExists && mpFullPath ) 
	{
//		char cFullPath[1024];
//		u_FromUnicodeToDefaultCP( mpFullPath->c_str(), mpFullPath->length(), cFullPath, 1024 );

		const char* cFullPath = mpFullPath->getBufferA();

		OSErr err = gUseCarbon
								? Path2Spec_Carbon( cFullPath, &mFSSpec )
								: Path2Spec_FSRef ( cFullPath, &mFSSpec );

		// IF we get error then 
		// Check that may be this is a simple name of file, without full path?
		// if yes then user want create file in the current directory.
		if( err )
		{
			if( strchr( cFullPath, I_Disk_Location::sPathDelimiterA ) == NULL )
			{
				vuint8 pasName[256];
				FBL::c2pstrcpy( cFullPath, pasName, 256 );
				err = FSMakeFSSpec( 0, 0, pasName, &mFSSpec );
			}
		}

		if( err && err != fnfErr )
			throw xOSFileError( (ERROR_TYPE)  err );
		
		mSpecExists = true;
	}

	return &mFSSpec;
} 


/**********************************************************************************************/
void Location_Disk_FSSpec::put_FSSpec( const FSSpec* inSpec )
{
	FBL_CHECK( inSpec != NULL );
	
	mFSSpec 	= *inSpec;	// copy structures here.
	mSpecExists = true;

	ForgetObject( mpFullPath );
}	


/**********************************************************************************************/
FBL_End_Namespace

#endif // FBL_SUPPORT_FSSPEC

