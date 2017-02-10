/**********************************************************************************************/
/* FBL_Location.cpp                    		                                  				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Location/FBL_Location.h>

#include <VShared/FBL/publ/Starter/FBL_Starter.h>
#include <VShared/FBL/publ/Util_classes/FBL_String.h>
#include <VShared/FBL/publ/Util_Strings/FBL_ustdio.h>
#include <VShared/FBL/publ/Util_Strings/FBL_ustdio.h>
#include <VShared/FBL/publ/Utilities/FBL_Util_Path.h>

// RZ -- TMP -- here! -- GetAppPAth must be redone
#include <VShared/FBL/publ/Location/FBL_Location_FSRef.h>


#if FBL_MAC && !FBL_MAC_MACHO
	#include <StandardFile.h>
	#include <Folders.h>
#endif // FBL_MAC && if !FBL_MAC_MACHO


#if FBL_WIN
	#pragma warning(push)
	#pragma warning(disable: 4005)
	#include <shlwapi.h>
	#pragma warning(pop)
#endif //FBL_WIN


#if FBL_UNIX
	//#include <sys/types.h>
	#include <sys/stat.h>
	#include <fcntl.h>	// open() for the get_isDirectory()
	#include <unistd.h>  // access() for the get_Exists()
#endif // FBL_UNIX

#ifdef _MSC_VER
#pragma warning(disable: 4702)
#endif //_MSC_VER


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
#if FBL_WIN
	const UChar I_Disk_Location::sPathDelimiterU = UChar('\\');
	const char I_Disk_Location::sPathDelimiterA = '\\';
#endif // FBL_WIN

#if FBL_POSIX_API
	const UChar I_Disk_Location::sPathDelimiterU = UChar('/');
	const char I_Disk_Location::sPathDelimiterA = '/';
#endif // FBL_POSIX_API

#if FBL_MAC	&& !FBL_POSIX_API
	const UChar I_Disk_Location::sPathDelimiterU = UChar(':');
	const char I_Disk_Location::sPathDelimiterA = ':';
#endif // FBL_MAC


/**********************************************************************************************/
I_Disk_Location_Ptr 	I_Disk_Location::spAppLocation = nullptr;


/**********************************************************************************************/
I_Disk_Location* CreateRawLocation( const UChar* inStr )
{
	String s( inStr );
	return new Location( &s );
}


/**********************************************************************************************/
I_Disk_Location* CreateRawLocation( const char* inStr )
{
	String s( inStr );
	return new Location( &s );
}


/**********************************************************************************************/
#if FBL_WIN || FBL_UNIX

/** For Windows and Unix we can specify location of a file only by Path.
	For MacOS we can do this using FSSPec and FSRef also.
	For MacOS build this function will have other implementation.
	See FBL_Location_Disk_FSSpec.cpp */
I_Disk_Location* CreateDiskLocation( const UChar* inStr )
{
	String str(inStr);
	return new Location( &str );
}


/**********************************************************************************************/
I_Disk_Location* CreateDiskLocation( const char* inStr )
{
	String str(inStr);
	return new Location( &str );
}

#endif // FBL_WIN || FBL_UNIX


/**********************************************************************************************/
#if FBL_WIN || FBL_POSIX_API


/**********************************************************************************************/
// Exctract from a full path name of file. For each platform this function uses its delimiter.
//
FBL_SHARED_EXP const char*  ExtractNameFromFullPath( const char*  inFullPath, char  inDelim = 0 );

#endif // FBL_WIN || FBL_POSIX_API


/**********************************************************************************************/
// STATIC: works as singlenton:
//
I_Disk_Location_Ptr I_Disk_Location::GetAppLocation( void )
{
	if( ! I_Disk_Location::spAppLocation )
	{
		#if !TARGET_OS_IPHONE
			Location::MakeNewAppLocation();
		#endif // !TARGET_OS_IPHONE
	}

	return I_Disk_Location::spAppLocation;
}


/**********************************************************************************************/
// STATIC:
//
void I_Disk_Location::SetAppLocation( I_Disk_Location_Ptr inAppLocation )
{
	spAppLocation = inAppLocation;
}


#pragma mark -


/**********************************************************************************************/
I_Disk_Location_Ptr	I_Disk_Location::GetTempDir( void )
{
	I_Disk_Location_Ptr tempLoc;

#if FBL_WIN

	char sysTmpFolder[MAX_PATH];
	::GetTempPath( SIZE_OF_ARRAY(sysTmpFolder), sysTmpFolder );
	tempLoc = CreateDiskLocation(sysTmpFolder);

#elif FBL_MAC

	#if FBL_SUPPORT_FSSPEC

		// Ruslan, please check this code.
		FSSpec sysTmpSpec;
		memset(&sysTmpSpec, 0, sizeof(sysTmpSpec));

		OSErr err = ::FindFolder(kOnSystemDisk,
								 kTemporaryFolderType,
								 kCreateFolder,
								 &sysTmpSpec.vRefNum,
								 &sysTmpSpec.parID );
		if( err == 0 )
		{
			// TODO: throw error here...
		}
		tempLoc = CreateDiskLocationFromFSSpec( &sysTmpSpec );

	#elif FBL_SUPPORT_FSREF
	
		FSRef tmpFolderRef;
		OSErr err = FSFindFolder( kOnSystemDisk, kTemporaryFolderType, kCreateFolder, &tmpFolderRef );
		argused1( err );

		tempLoc = CreateDiskLocationFromFSRef( NULL, &tmpFolderRef );
	
	#endif // FBL_SUPPORT_FSREF

#elif FBL_UNIX

	// KP According to the "Filesystem Hierarchy Standard"
	// http://www.pathname.com/fhs/pub/fhs-2.3.html
	tempLoc = CreateDiskLocation("/tmp");

#endif //FBL_WIN
	
	return tempLoc;
}


/**********************************************************************************************/
//
I_Disk_Location_Ptr	I_Disk_Location::GetTempFile( const char* inBaseName )
{
	I_Disk_Location_Ptr tempLoc;

// -------------------------
#if FBL_WIN

	char sysTmpFolder[MAX_PATH];	
	::GetTempPath( SIZE_OF_ARRAY(sysTmpFolder), sysTmpFolder );

	// IS: 02.02.2010
	// ::GetTempFileName(..., ..., 0, ...) creates a file on the disk but we want to do it later
	// (It will be done on create storage). Because this file may not be used (RAM-based
	// storage for example) and will never be deleted.
	// So we call ::GetTempFileName(..., ..., 1, ...) which just build an unique name (for a moment)
	// and hope this one will not be used by another process.
	// On the other hand it is too risky - so let's do it with tickcount+counter - seems to be enough.
	//
	// char tmpFile[MAX_PATH];
	// ::GetTempFileName( sysTmpFolder, (inBaseName == NULL ? "temp" : inBaseName), 0, tmpFile);
	// ::GetTempFileName( sysTmpFolder, (inBaseName == NULL ? "temp" : inBaseName), 1, tmpFile);
	// tempLoc  = CreateDiskLocation(tmpFile);

	static vuint32 Counter = 0;
	String tmpName(sysTmpFolder);
	tmpName << inBaseName;
	tmpName.addUInt( FBL::TickCount() );	// as tmp name we use result of TickCount().
	tmpName.addUInt( ++Counter );
	tempLoc = CreateDiskLocation( tmpName.getBufferA() );


// -------------------------
#elif FBL_MAC
	// for MacOS we need self generate unique name for tmp file.
	// for this we use now just Name = TickCount + Counter.
	// This algorithm can be changed always

	argused1(inBaseName);

	static vuint32 Counter = 0;

	String tmpName;
	tmpName.setUInt( FBL::TickCount() );	// as tmp name we use result of TickCount().
	tmpName.addUInt( ++Counter );

	#if FBL_SUPPORT_FSSPEC

		const char* p = tmpName.getBufferA();
		//
		// Attention! 62 symbols available in sysTmpSpec.name only!!!
		// Actually it is char[64], but first symbol we uses for length (Pascal string) and last
		// symbol is '\0'.
		//
		vuint8 l = (vuint8)STD::min( tmpName.length(), (tslen)62 );
	
		// we make pascal string for FSSpec:
		FSSpec sysTmpSpec;
	
		memcpy( (char*)sysTmpSpec.name + 1, p, l );
		sysTmpSpec.name[0] = l;
		
		// Now we find system TMP folder:
		OSErr err = ::FindFolder(	kOnSystemDisk, kTemporaryFolderType, kCreateFolder,
									&sysTmpSpec.vRefNum, &sysTmpSpec.parID );
		if( err != 0 )
		{
			// TODO: throw error here...
		}
		
		tempLoc = CreateDiskLocationFromFSSpec( &sysTmpSpec );

	#elif FBL_SUPPORT_FSREF
		
		FSRef tmpFolderRef;
		OSErr err = FSFindFolder( kOnSystemDisk, kTemporaryFolderType, kCreateFolder, &tmpFolderRef );
		argused1( err );

		I_Disk_Location_Ptr pTmpFolder = CreateDiskLocationFromFSRef( NULL, &tmpFolderRef );


		I_Location_Ptr pTmpFile = pTmpFolder->get_ChildLocation( tmpName ); 
		tempLoc = fbl_dynamic_cast<I_Disk_Location>( pTmpFile ); 

	#endif // FBL_SUPPORT_FSREF


// -------------------------
#elif FBL_UNIX

	// For UNIX also we must generate unique filename
	static vuint32 Counter = 0;

	String tmpName = "/tmp/val_tmp_";
	tmpName.addUInt( FBL::TickCount() );	// as tmp name we use result of TickCount().
	tmpName.addUInt( ++Counter );

	tempLoc = CreateDiskLocation( tmpName.getBufferA() );

#else

	#error "NOT IMPLEMENTED"

#endif // FBL_MAC

	return tempLoc;
}


#pragma mark -

/**********************************************************************************************/
void CheckLocationExists( I_Disk_Location_Ptr inpLocation )
{
	FBL_CHECK( inpLocation );
	if( inpLocation->get_Exists() == false  )
		FBL_Throw( xOSFileError( ERR_OS_FILE_NOT_FOUND, inpLocation->get_Path().c_str() ) );
}


#pragma mark -

/**********************************************************************************************/
Location::Location( const String* inFullPath )
{
	mpFullPath = nullptr;
	mIsDirectory = kUnknown;

	if( inFullPath != nullptr )
		put_Path( *inFullPath );
}


/**********************************************************************************************/
Location::Location( const UChar* inFullPath )
{
	mpFullPath = nullptr;
	mIsDirectory = kUnknown;

	if( inFullPath != nullptr )
		put_Path( inFullPath );
}


/**********************************************************************************************/
Location::Location( const Location& inOther )
{
	if( inOther.mpFullPath )
		mpFullPath = new String( *inOther.mpFullPath );
	else
		mpFullPath = nullptr;

	mIsDirectory = kUnknown;
}


/**********************************************************************************************/
Location::~Location( void )
{
	if( mpFullPath )
		delete mpFullPath;
}


/**********************************************************************************************/
Location& Location::operator=( const Location& inOther )
{
	ForgetObject( mpFullPath );

	if( inOther.mpFullPath )
		mpFullPath = new String( *inOther.mpFullPath );
	else
		mpFullPath = nullptr;

	return *this;
}


#pragma mark -


/**********************************************************************************************/
// FBL_WIN || FBL_POSIX
//
bool Location::get_Exists( void ) const 
{
#if FBL_WIN

	const UChar* pathU = get_Path().c_str();
	if( ::PathIsDirectoryW(pathU) )
		return true;
   else
		return (bool)::PathFileExistsW(get_Path().c_str());

#elif FBL_UNIX

	const char* pathA = get_Path().getBufferA();
	return (access(pathA, F_OK) != -1);

#endif // FBL_WIN

	return false;
}


/**********************************************************************************************/
bool Location::get_IsDirectory( void ) const 
{
	if( mIsDirectory == kUnknown ) // we need to use OS to define result.
	{
		#if FBL_WIN

			DWORD dw= (mpFullPath->isSingleByte()) ?
						::GetFileAttributesA( get_Path().getBufferA() ) :
						::GetFileAttributesW( get_Path().c_str() );

			if( dw == INVALID_FILE_ATTRIBUTES )
				return false;

			return (dw & FILE_ATTRIBUTE_DIRECTORY) != 0;

		#endif // FBL_WIN

		#if FBL_MAC
			// FIXME!!!
			return false;
		#endif // FBL_MAC

		#if FBL_UNIX

			const char* pathA = get_Path().getBufferA();

			bool isDir = false;
			struct stat sb;
			if( stat( pathA, &sb ) != -1 )
				isDir = S_ISDIR( sb.st_mode );
			
			return isDir;

		#endif // FBL_UNIX
	}
	else
	{
		return bool(mIsDirectory == kDirectory);
	}
}


/**********************************************************************************************/
// Allow us fast assign of flag for Location.
// we can do this usually, when do iteration of directory using class Directory.
//
void Location::put_IsDirectory( bool inValue ) 
{
	mIsDirectory = inValue ? kDirectory : kFile;
}


/**********************************************************************************************/
bool Location::get_IsUndefined( void ) const 
{
	if( mpFullPath == nullptr )
		return true;
	else
		return false;
}


/**********************************************************************************************/
I_Location_Ptr Location::get_ChildLocation( const UChar* inName ) const
{
	// if this Location do not have mpFullPath we force to build it.
	if( mpFullPath == nullptr )
		get_Path();

	if( mpFullPath ) // just in case protection.
	{
		I_Location_Ptr childLoc = this->Clone();
		if( childLoc )
		{
			String childPath( *mpFullPath + inName );
			childLoc->put_Path( childPath );

//			I_Disk_Location_Ptr pDiskLocation = fbl_dynamic_cast<I_Disk_Location>( childLoc );
//			if( pDiskLocation->get_Exists() )
				return childLoc;
		}
	}

	return nullptr;
}


/**********************************************************************************************/
I_Location_Ptr Location::get_ChildLocation( const char* inName ) const
{
	String s(inName);
	return get_ChildLocation(s.c_str());
}


/**********************************************************************************************/
I_Location_Ptr Location::Clone( void ) const 
{
	return new Location(*this);
}


#pragma mark ====== I_Disk_Location API =======
#pragma mark -


/**********************************************************************************************/
I_Location_Ptr Location::get_ChildLocation( const String& inName ) const
{
	return get_ChildLocation(inName.c_str());
}


/**********************************************************************************************/
I_Location_Ptr Location::get_ParentDirectory( void ) const
{
	FBL_CHECK( mpFullPath != nullptr );

	// Build new Location for folder:
	I_Location_Ptr parentLoc = this->Clone();

	// Looking for last delimiter:
	vint32 pos = mpFullPath->reverseFind( I_Disk_Location::sPathDelimiterU );

	if( pos == -1 )	// Not found delimiter - there is current directory
	{
		parentLoc->put_Path( "" );
	}
	else
	{
		String path = mpFullPath->left( pos + 1 );
		parentLoc->put_Path( path );
	}

	return parentLoc;
}


/**********************************************************************************************/
const String& Location::get_FileName( void ) const 
{
	// Static string to return to the client
	static String Name;

#if FBL_WIN || FBL_POSIX_API
	if( mpFullPath->isSingleByte() )
		Name = ExtractNameFromFullPath( mpFullPath->getBufferA() );
	else
		Name = ExtractNameFromFullPath( mpFullPath->c_str() );
#endif // FBL_WIN

	return Name;
}


/**********************************************************************************************/
const String& Location::get_Path( void ) const 
{
	FBL_CHECK( mpFullPath );

	return *mpFullPath;
}


/**********************************************************************************************/
void Location::put_Path( const UChar* inFullPath ) 
{
	String s(inFullPath);
	put_Path(s);
}


/**********************************************************************************************/
void Location::put_Path( const String& inFullPath ) 
{
	ForgetObject( mpFullPath );

	mpFullPath = new String( inFullPath ); // this makes COPY of the given string !!!
	FBL_CHECK(mpFullPath);

#if FBL_WIN
	// http://www.valentina-db.com/bt/view.php?id=7075
	// For Windows we have to change all '/' occurances to native '\' symbols.
	if( mpFullPath->isSingleByte() )
	{
		char* p = mpFullPath->getBufferA(mpFullPath->length());
		p = strchr(p, '/');
		while (p)
		{
			*p = I_Disk_Location::sPathDelimiterA;
			p = strchr(p + 1, '/');
		}
		mpFullPath->releaseBufferA();
	}
	else
	{
		UChar* p = mpFullPath->getBuffer(mpFullPath->length());
		p = vu_strchr(p, '/');
		while (p)
		{
			*p = I_Disk_Location::sPathDelimiterU;
			p = vu_strchr(p + 1, '/');
		}
		mpFullPath->releaseBuffer();
	}
#endif //FBL_WIN

	if( get_IsDirectory() )
	{
		// Check for trailing backslash.
		UChar32 c = (*mpFullPath)[(mpFullPath->length() - 1)];
		if( c != UChar('\\') && c != UChar('/') )
		{
			*mpFullPath += I_Disk_Location::sPathDelimiterU;
		}
	}
}


#pragma mark -


/**********************************************************************************************/
// It is called if spAppLocation is NULL;
// We must define the sAppPath if it is empty.
//
#if FBL_WIN
void Location::MakeNewAppLocation( void )
{
	DWORD len = GetCurrentDirectory( 0, reinterpret_cast< TCHAR* >( NULL ) );
	if ( len != 0 )
	{
		MemPtr< UChar > buffer( len );
		GetCurrentDirectoryW( len, buffer );

		spAppLocation = FBL::CreateDiskLocation( buffer.begin() );
	}
}
#endif // FBL_WIN


// --------------------------------------------------------------------------------------
#if FBL_MAC
void Location::MakeNewAppLocation( void )
{
	#if FBL_SUPPORT_FSREF

		CFURLRef appURL = FindAppFolderCopy();
			
			spAppLocation = CreateDiskLocationFromFSRef( appURL );
		
		CFRelease( appURL );		
		
	#endif // FBL_SUPPORT_FSREF
}
#endif // FBL_MAC


// --------------------------------------------------------------------------------------
#if FBL_UNIX
void Location::MakeNewAppLocation( void )
{
	char buf[ 1024 ];

	int len = readlink( "/proc/self/exe", buf, 1023 );
	if( len != -1 )
	{
		if( *buf != '(' )
		{
			char* pc = buf + len;
			while( --pc > buf )
			{
				if( *pc == '/' )
				{
					*pc = 0; // get path only

					spAppLocation = FBL::CreateDiskLocation( buf );
					break;
				}
			}
		}
	}	
	
	if( !spAppLocation )
	{
		if( getcwd (buf, 1023) )
			spAppLocation = FBL::CreateDiskLocation( buf );
	}
}
#endif // FBL_UNIX


/**********************************************************************************************/
FBL_End_Namespace
