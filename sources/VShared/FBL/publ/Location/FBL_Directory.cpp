/**********************************************************************************************/
/* FBL_Directory.cpp                    		                               				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Location/FBL_Directory.h>

// VShared/FBL/publ:
#include <VShared/FBL/publ/Location/FBL_Location_Disk_FSSpec.h>
#include <VShared/FBL/publ/Util_classes/FBL_String.h>
#include <VShared/FBL/publ/Util_classes/FBL_StringConversion.h>
#include <VShared/FBL/publ/Util_Strings/FBL_ustdio.h>
#include <VShared/FBL/publ/Utilities/FBL_Util_Path.h>

     
/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
#if FBL_MAC && !FBL_MAC_MACHO


/**********************************************************************************************/
enum
{
    DIR_FILES     = 0x0001,       // include files
    DIR_DIRS      = 0x0002,       // include directories
    DIR_HIDDEN    = 0x0004,       // include hidden files
    DIR_DOTDOT    = 0x0008,       // include '.' and '..'

    // by default, enumerate everything except '.' and '..'
    DIR_DEFAULT   = DIR_FILES | DIR_DIRS | DIR_HIDDEN
};


/**********************************************************************************************/
class DirData
{
public:
    DirData(Const_I_Disk_Location_Ptr inDirLocation);
    ~DirData() {}

    bool Read( StringPtr outFilename, bool& outIsDirectory ); // reads the next 
    void Rewind() { m_index = 0; }

	vint32 GetDirID( void ) const { return m_dirId; }		

private:
	CInfoPBRec			m_CPB;
	vint16				m_index;
	vint32				m_dirId;	// IF of THIS directory.
	Str255				m_name;
	bool				m_isDir;

    int      m_flags;
};


/**********************************************************************************************/
DirData::DirData( Const_I_Disk_Location_Ptr inDirLocation )
{
	Const_Location_Disk_FSSpec_Ptr pLocSpec = fbl_dynamic_cast<const Location_Disk_FSSpec>(inDirLocation);

	const FSSpec* pSpec = pLocSpec->get_FSSpec();
	
	m_CPB.hFileInfo.ioVRefNum = pSpec->vRefNum;
	m_CPB.hFileInfo.ioNamePtr = m_name;
	m_index = 0;
	
	m_flags = DIR_FILES | DIR_DIRS;

	OSErr err = FBL::FSpGetDirectoryID( pSpec, &m_dirId , &m_isDir );
	FBL_CHECK( err == noErr );
}

/**********************************************************************************************/
bool DirData::Read( StringPtr outFilename, bool& outIsDirectory )
{
    if ( !m_isDir )
        return false;
		
	outIsDirectory = false;	
		
	vint16 err = noErr ;
	
	while ( err == noErr )
	{
		m_index++ ;
		m_CPB.dirInfo.ioFDirIndex = m_index;
		m_CPB.dirInfo.ioDrDirID = m_dirId;	/* we need to do this every time */

		err = PBGetCatInfoSync((CInfoPBPtr)&m_CPB);
		if ( err != noErr )
			break ;

#if TARGET_CARBON
        // under X thats the way the mounting points look like
        if ( ( m_CPB.dirInfo.ioDrDirID == 0 ) && ( m_flags & DIR_DIRS) )
        {
			outIsDirectory = true;		
            break ;
        }
#endif

		if ( ( m_CPB.dirInfo.ioFlAttrib & ioDirMask) != 0 && (m_flags & DIR_DIRS) ) //  we have a directory
		{
			outIsDirectory = true;		
			break ;
		}
			
		if ( ( m_CPB.dirInfo.ioFlAttrib & ioDirMask) == 0 && !(m_flags & DIR_FILES ) ) // its a file but we don't want it
			continue ;
			 
        if ( ( m_CPB.hFileInfo.ioFlFndrInfo.fdFlags & kIsInvisible ) && !(m_flags & DIR_HIDDEN) ) // its hidden but we don't want it
			continue ;

		// COMMENTED, because this is code from wxWindows, it do pattern search of file names. We do not need this feature.
/*		String file( m_name );
		if ( m_filespec.IsEmpty() || m_filespec == "*.*" || m_filespec == "*" )
		{
		}
		else if ( m_filespec.GetLength() > 1 && m_filespec.Left(1) =="*" )
		{
			if ( file.Right( m_filespec.GetLength() - 1 ).MakeUpper() != m_filespec.Mid(1).MakeUpper() )
			{
				continue ;
			}
		}
		else if ( m_filespec.GetLength() > 1 && m_filespec.Right(1) == "*" )
		{
			if ( file.Left( m_filespec.GetLength() - 1 ).MakeUpper() != m_filespec.Left( m_filespec.GetLength() - 1 ).MakeUpper() )
			{
				continue ;
			}
		}
		else if ( file.MakeUpper() != m_filespec.MakeUpper() )
		{
			continue ;
		}*/

		break;
	}

	if ( err != noErr )
		return false;

	memcpy( outFilename, m_name, vuint32(m_name[0] + 1) );
    return true;
}


#endif // FBL_MAC && !FBL_MAC_MACHO


#pragma mark -

 

/**********************************************************************************************/
Directory::Directory( I_Disk_Location_Ptr inLocation ) 
:
	mpLocation( inLocation )
#if FBL_WIN
	, mStarted( false )
#endif // FBL_WIN
{
	mDirent = nullptr;
}


/**********************************************************************************************/
Directory::~Directory( void )
{
	StopIteration();
}


/**********************************************************************************************/
I_Disk_Location_Ptr Directory::NextItem( void ) 
{
	I_Disk_Location_Ptr pLocation = nullptr;

#if FBL_WIN
	WIN32_FIND_DATAW FileData;
	if ( mStarted )
	{
		if ( !::FindNextFileW( mDirent, &FileData) )
			StopIteration();
	}
	else
	{
		if ( mDirent )
			StopIteration();

		String file_path( mpLocation->get_Path());
		if( file_path.charAt( file_path.length()-1 ) != I_Disk_Location::sPathDelimiterU )
			file_path += I_Disk_Location::sPathDelimiterU;
		file_path += "*";

		mDirent = ::FindFirstFileW( file_path.c_str(), &FileData ); 
		if ( mDirent == INVALID_HANDLE_VALUE )
			mDirent = NULL;
		else
			mStarted = true;
	}

	if ( mDirent )
	{
		String file_path( mpLocation->get_Path() );
		if( file_path.charAt( file_path.length()-1 ) != I_Disk_Location::sPathDelimiterU )
			file_path += I_Disk_Location::sPathDelimiterU;
		file_path += FileData.cFileName;
		pLocation = fbl_dynamic_cast<I_Disk_Location>(CreateDiskLocation( file_path.c_str() ) );
	}
#endif // FBL_WIN


#if FBL_POSIX_API
	dirent* pDirent = ::readdir( mDirent );
	
	if( pDirent )
	{	
		String file_path( mpLocation->get_Path().c_str() );
		if( file_path.charAt( file_path.length()-1 ) != I_Disk_Location::sPathDelimiterU )
			file_path += Location::sPathDelimiterU;
		file_path += pDirent->d_name;

		pLocation = new Location( file_path.c_str() );
		pLocation->put_IsDirectory( bool(pDirent->d_type == DT_DIR) );
	}	
#endif // FBL_POSIX_API		


#if FBL_MAC && !FBL_MAC_MACHO
	bool IsDirectory;
	Str255 pasName;

	((DirData*) mDirent)->Read( pasName, IsDirectory );
	
	if( pasName[0] > 0 )
	{
		Location_Disk_FSSpec_Ptr pLocSpec = fbl_dynamic_cast<Location_Disk_FSSpec>(mpLocation);
		const FSSpec* pThisDirectory = pLocSpec->get_FSSpec();

		FSSpec childSpec;
		FSMakeFSSpec( pThisDirectory->vRefNum, ((DirData*) mDirent)->GetDirID(), pasName, &childSpec );
		
		pLocation = CreateDiskLocationFromFSSpec( &childSpec );
		pLocation->put_IsDirectory( IsDirectory );
	}
#endif // FBL_MAC && !FBL_MAC_MACHO

	return pLocation; 
}		


/**********************************************************************************************/
bool Directory::StartIteration( void ) 
{
#if FBL_WIN
	return true;
#else

#if FBL_POSIX_API
    String thePath = mpLocation->get_Path();
    StFromUTF16 charPath( thePath.c_str(), thePath.end() );
	mDirent = ::opendir( charPath.c_str() );
#endif // FBL_POSIX_API	


#if FBL_MAC && !FBL_MAC_MACHO
	mDirent = new DirData( mpLocation );
#endif // FBL_MAC && !FBL_MAC_MACHO

	return bool( mDirent != nullptr );

#endif // FBL_WIN

}		


/**********************************************************************************************/
void Directory::StopIteration( void ) 
{
#if FBL_WIN
	if ( mDirent )
	{
		::FindClose( mDirent );
		mStarted = false;
	}
#endif // FBL_WIN

#if FBL_POSIX_API
 	if( mDirent ) 
    	closedir( mDirent );
#endif 		

#if FBL_MAC && !FBL_MAC_MACHO
	delete (DirData*) mDirent;
#endif // FBL_MAC && !FBL_MAC_MACHO

   	mDirent = nullptr;
}		


/**********************************************************************************************/
FBL_End_Namespace
