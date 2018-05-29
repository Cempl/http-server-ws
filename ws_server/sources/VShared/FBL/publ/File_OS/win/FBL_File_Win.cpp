/**********************************************************************************************/
/* FBL_FileWin.cpp                                                          				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                       					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifdef _MSC_VER
#pragma warning(disable: 4005)
#pragma warning(disable: 4701)
#endif //_MSC_VER

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/File_OS/win/FBL_File_Win.h>

#include <VShared/FBL/publ/Errors/FBL_Exceptions.h>
#include <VShared/FBL/publ/Util_classes/FBL_StringConversion.h>
#include <VShared/FBL/publ/Util_Paths/FBL_Util_Dll_Win.h>
#include <VShared/FBL/publ/Utilities/FBL_Util_Path.h>


/**********************************************************************************************/
#if FBL_WIN


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
FileWin::FileWin( void )
:
	mNeedFlush(false)
{
	mHandle	= INVALID_HANDLE_VALUE;
	mReadOnly = fWritable;
}


/**********************************************************************************************/
FileWin::~FileWin( void ) 
{
}


#pragma mark -


/**********************************************************************************************/
// Forse to write on disk all cached information from this file.	
//
void FileWin::Flush( void )
{
	if (mHandle == INVALID_HANDLE_VALUE)
		return;

	if( mNeedFlush )
	{
		if (!::FlushFileBuffers(mHandle))
			throw xOSFileError( (ERROR_TYPE) ::GetLastError() );
		mNeedFlush = false;
	}
}


#pragma mark -


/**********************************************************************************************/
flength	FileWin::Seek( flength inOff, vuint16 inFrom )
{ 
#if FBL_LENGTH <= 32	
	flength dwNew = (flength) ::SetFilePointer( (HANDLE)mHandle, (vint32)inOff, NULL, (vuint32)inFrom );

	if( dwNew == flength(-1) )
		throw xException( (ERROR_TYPE) ::GetLastError() );

	return dwNew;
	
#else	
	LARGE_INTEGER li;
	li.QuadPart = (vint64) inOff;
	li.LowPart = ::SetFilePointer( (HANDLE)mHandle, (vint32) li.LowPart, &li.HighPart, (DWORD)inFrom );

	if( li.LowPart == 0xFFFFFFFF && GetLastError() != NO_ERROR )
		throw xOSFileError( (ERROR_TYPE) ::GetLastError() );

	return flength(li.QuadPart);
#endif	
}


/**********************************************************************************************/
void FileWin::DoClose( void )
{
	// -----------------------------
	// in case this file is cached we need remove pages from cache.
	I_AspectHost* pHost = dcast<I_AspectHost*>( this );
	if( pHost )
		pHost->SendMessageToAll( 1 );

	BOOL bError = !::CloseHandle( mHandle );
	mHandle = INVALID_HANDLE_VALUE;
	
	if (bError)	
		throw xOSFileError( (ERROR_TYPE) ::GetLastError() );
}


/**********************************************************************************************/
void FileWin::DoCreate( void )
{
	mNeedFlush = true;
	
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	// IS: 24.04.2008
	// Use FILE_SHARE_READ shared access instead of 0.
	// It would be very helpful for Warnings and so on.
	// In other words - this process have RW access and allows RO for another processes.
	//

	HANDLE hFile;
	if( IsWindowsNTFamilyOS() )
	{
		const UChar* pathU = get_Path().c_str();
		hFile = ::CreateFileW( pathU, 
					GENERIC_READ | GENERIC_WRITE, 					
					FILE_SHARE_READ,
					&sa, 
					CREATE_ALWAYS, 
					FILE_ATTRIBUTE_NORMAL, 
					NULL);
	}
	else
	{
		const char* pathA = get_Path().getBufferA();
		hFile = ::CreateFileA( pathA, 
					GENERIC_READ | GENERIC_WRITE, 					
					FILE_SHARE_READ,
					&sa, 
					CREATE_ALWAYS, 
					FILE_ATTRIBUTE_NORMAL, 
					NULL);
	}
	
	if( hFile == INVALID_HANDLE_VALUE )
		throw xOSFileError( (ERROR_TYPE)::GetLastError() );

	mHandle = hFile;
}


/**********************************************************************************************/
void FileWin::DoOpen( void )
{
	const char*  fileNameA = get_Path().getBufferA();
	const UChar* fileNameU = get_Path().c_str();

	// map modeNoInherit flag
	SECURITY_ATTRIBUTES sa;
	sa.nLength 				= sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle 		= TRUE;
	
	// If we want to open file for writing we have to check it is possible.
	// And open for reading in case of such check fails.
	//	
	if( (mReadOnly & fExternalRO) == false )
	{
		// define attributes of file:
		DWORD attr;
		if( IsWindowsNTFamilyOS() )
			attr = GetFileAttributesW( fileNameU );
		else
			attr = GetFileAttributesA( fileNameA );

		if( attr & FILE_ATTRIBUTE_READONLY )
			mReadOnly |= fExternalRO;
	}
	
	//
	// mReadOnly == false	- this process have RW access and allows RO for another processes.
	// mReadOnly == true	- this process have RO access and allows RW for another processes.
	//
	DWORD mode 		= (mReadOnly & fExternalRO) ? GENERIC_READ : (GENERIC_READ | GENERIC_WRITE);
	DWORD shared	= (mReadOnly & fExternalRO) ? (FILE_SHARE_READ | FILE_SHARE_WRITE) : FILE_SHARE_READ;

	HANDLE hFile;
	if( IsWindowsNTFamilyOS() )
		hFile = ::CreateFileW( fileNameU, mode, shared, &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	else
		hFile = ::CreateFileA( fileNameA, mode, shared, &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if( hFile == INVALID_HANDLE_VALUE )
	{
		// IS: 03.03.2008
		// http://www.valentina-db.com/bt/view.php?id=3063
		//throw xOSFileError( ERR_OS_FILE_NOT_FOUND, fileNameU );
		ERROR_TYPE err = ::GetLastError();
		// If "The system cannot find the file specified. " we throw xOSFileError
		// we need it because of another files like posix which throws this error.
		if( err == ERROR_FILE_NOT_FOUND ||
			err == ERROR_PATH_NOT_FOUND )
		{
			throw xOSFileError( ERR_OS_FILE_NOT_FOUND, fileNameU );
		}
		else
		{
			// IS: 07.04.2010
			// If we are not able to open file for writing because of access denied
			// Probably we can open it for reading
			if( ((mReadOnly & fExternalRO) == false) && (
						err == ERROR_ACCESS_DENIED 
					||	err == ERROR_WRITE_PROTECT 
					||	err == ERROR_SHARING_VIOLATION ) )
			{
				mReadOnly |= fExternalRO;
				DWORD mode2 	= GENERIC_READ;
				DWORD shared2 = FILE_SHARE_READ | FILE_SHARE_WRITE;

				if( IsWindowsNTFamilyOS() )
					hFile = ::CreateFileW( fileNameU, mode2, shared2, &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				else
					hFile = ::CreateFileA( fileNameA, mode2, shared2, &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

				err =  (hFile == INVALID_HANDLE_VALUE) ? ::GetLastError() : 0;
			}

			if( err )
				throw xOSFileError( err );
		}
	}

	mHandle = hFile;
}


/**********************************************************************************************/
void FileWin::DoPutLength( flength inLength )
{
	mNeedFlush = true;
	
	Seek( inLength, begin );

	if (!::SetEndOfFile(mHandle))
		throw xOSFileError( (ERROR_TYPE) ::GetLastError() );
}


/**********************************************************************************************/
flength	FileWin::DoGetLengthOfClosed( void ) const
{
	WIN32_FIND_DATAA findFileDataA = {0};
	WIN32_FIND_DATAW findFileDataW = {0};

	HANDLE hFind;
	if( IsWindowsNTFamilyOS() )
		hFind = ::FindFirstFileW( (LPWSTR) get_Path().c_str(), &findFileDataW );
	else
		hFind = ::FindFirstFileA( (LPSTR) get_Path().getBufferA(), &findFileDataA );

	if( hFind == INVALID_HANDLE_VALUE )
		return 0;

	::FindClose(hFind);
	//FBL_CHECK(findFileData.nFileSizeHigh == 0);

#if FBL_LENGTH <= 32	
	if( IsWindowsNTFamilyOS() )
		return (flength)findFileDataW.nFileSizeLow;
	else
		return (flength)findFileDataA.nFileSizeLow;
#else	
	LARGE_INTEGER li;
	if( IsWindowsNTFamilyOS() )
	{
		li.LowPart 	= findFileDataW.nFileSizeLow;
		li.HighPart = (vint32) findFileDataW.nFileSizeHigh;
	}
	else
	{
		li.LowPart 	= findFileDataA.nFileSizeLow;
		li.HighPart = (vint32) findFileDataA.nFileSizeHigh;
	}

	return (flength) li.QuadPart;
#endif	
}


/**********************************************************************************************/
flength	FileWin::DoGetLengthOfOpened( void ) const
{
	FBL_CHECK(mHandle);

#if FBL_LENGTH <= 32
	DWORD dw;
	if(!::GetFileSize(mHandle, &dw))
		throw(xOSFileError( (ERROR_TYPE) ::GetLastError() ));

	return (static_cast<flength>(dw));
#else
	// IS: 03.03.2008
	// http://valentina-db.com/bt/view.php?id=3056
	// Commented because GetFileSize returns zero in case file is zero-length.
	//
/*
	DWORD dwh, dwl;
	if(!(dwl = ::GetFileSize(mHandle, &dwh)))
		throw(xOSFileError( (ERROR_TYPE) ::GetLastError() ));

	return (flength(dwh) << 32) | flength(dwl);
*/
	LARGE_INTEGER res;
	if( ::GetFileSizeEx(mHandle, &res) == false )
		throw(xOSFileError( (ERROR_TYPE) ::GetLastError() ));

	return (flength(res.HighPart) << 32) | flength(res.LowPart);

#endif 
}


/**********************************************************************************************/
void FileWin::DoThrowOut( void )
{
	const String& s = get_Path();
	if( !::DeleteFileA( s.getBufferA() ) )
		throw xOSFileError( (ERROR_TYPE) ::GetLastError() );	
}


/**********************************************************************************************/
vuint32 FileWin::DoRead(
	char* 	inBuffer,
	flength	inFrom,
	vuint32 	inHowMuch )
{
	FBL_CHECK(inBuffer);

	/* pre: read existing information */  
	FBL_CHECK( inFrom + inHowMuch <= get_Length() );

	/* Set file marker to right position */
	Seek(inFrom, begin);

	DWORD Read;

	if (!::ReadFile(mHandle, inBuffer, inHowMuch, &Read, NULL))
	{	
		ERROR_TYPE err = (ERROR_TYPE) ::GetLastError();
		throw xOSFileError( err );
	}

	return Read;
}


/**********************************************************************************************/
// Writes a given buffer starting from the current position, without Seek() overhead.
//
/*vuint32 FileWin::DoWrite( 
	const char* inBuffer, 
	vuint32 		inHowMuch )
{
	vuint32 Written;

	FBL_CHECK(inBuffer);

	if( mReadOnly )
		FBL_Throw(xOSFileError(ERR_OS_FILE_READ_ONLY));

	if (!::WriteFile(mHandle, inBuffer, inHowMuch, &Written, NULL))
		throw xOSFileError( (ERROR_TYPE) ::GetLastError() );

	return Written;
}*/


/**********************************************************************************************/
// Finds the given position in the file, then writes a given buffer.
//
vuint32 FileWin::DoWrite( 
	const char* inBuffer, 
	flength		inFrom, 
	vuint32 		inHowMuch )
{
	mNeedFlush = true;
	
	DWORD Written;

	FBL_CHECK(inBuffer);

	if( mReadOnly )
		FBL_Throw(xOSFileError(ERR_OS_FILE_READ_ONLY));

	Seek(inFrom, begin);

	if (!::WriteFile(mHandle, inBuffer, inHowMuch, &Written, NULL))
		throw xOSFileError( (ERROR_TYPE) ::GetLastError() );

	return Written;
}


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // FBL_WIN


