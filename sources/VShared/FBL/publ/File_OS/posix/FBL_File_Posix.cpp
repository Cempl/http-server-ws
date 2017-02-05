/**********************************************************************************************/
/* FBL_File_Posix.cpp																		  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Headers/FBL.h>

#if FBL_POSIX_API && !FBL_MAC

#include <VShared/FBL/publ/File_OS/posix/FBL_File_Posix.h>

#include <VShared/FBL/publ/Errors/FBL_Exceptions.h>
#include <VShared/FBL/publ/Util_classes/FBL_StringConversion.h>
#include <VShared/FBL/publ/Util_Paths/FBL_Util_Dll_Win.h>
#include <VShared/FBL/publ/Utilities/FBL_Util_Path.h>

#include <errno.h>
#include <fcntl.h>/* open() and O_XXX flags */
#include <sys/stat.h>/* S_IXXX flags */
#include <sys/file.h>
//#include <sys/types.h>/* mode_t */
#include <unistd.h>/* close() read() */

#include <stdlib.h>
#include <stdio.h>


/**********************************************************************************************/
FBL_Begin_Namespace

/**********************************************************************************************/
File_Posix::File_Posix( void )
:
	mNeedFlush(false)
{
	mFd	  = -1;
	mReadOnly = fWritable;
}


/**********************************************************************************************/
File_Posix::~File_Posix( void )
{
}


#pragma mark -


/**********************************************************************************************/
// Forse to write on disk all cached information from this file.
//
void File_Posix::Flush( void )
{
	if (mFd == -1 )
		return;

	if( mNeedFlush )
	{
		if ( fsync( mFd ) == -1)
			throw xOSFileError( (ERROR_TYPE) errno );
		mNeedFlush = false;
	}
}


#pragma mark -


/**********************************************************************************************/
flength	File_Posix::Seek( flength inOff, ushort inFrom )
{
	flength res = lseek64( mFd, inOff, inFrom );

	if( res == -1  )
	{
		throw xOSFileError( (ERROR_TYPE) errno );
	}

	return res;
}


/**********************************************************************************************/
void File_Posix::DoClose( void )
{

	// -----------------------------
	// in case this file is cached we need remove pages from cache.
	I_AspectHost* pHost = dcast<I_AspectHost*>( this );
	if( pHost )
		pHost->SendMessageToAll( 1 );

	// Close file
	int res = close( mFd );

	// Check on error
	if( res == -1 )
	{
		throw xOSFileError( (ERROR_TYPE) errno );
	}

	// IS: 14.06.2011
	// Bug: #5566
	//
	flock( mFd, LOCK_UN | LOCK_NB );
	
	// clean handle
	mFd = -1;
}


/**********************************************************************************************/
void File_Posix::DoCreate( void )
{
	mNeedFlush = true;
	
	const char* myfile = get_Path().getBufferA();

	// Try create file
	mFd = open64( myfile, O_RDWR | O_CREAT|O_EXCL|O_LARGEFILE, S_IRUSR | S_IWUSR );

	// Check if error
	if( mFd  == -1)
	{
		throw xOSFileError( (ERROR_TYPE) errno );
	}
	
	// IS: 14.06.2011
	// Bug: #5566
	//
	if( flock( mFd, LOCK_EX | LOCK_NB )  == -1 )
	{
		DoClose();
		throw xOSFileError( ERR_OS_FILE_BUSY, myfile );
	}	
}


/**********************************************************************************************/
void File_Posix::DoOpen( void )
{
	const char* myfile =  get_Path().getBufferA();
	const UChar* fileNameU = get_Path().c_str();

	if( mReadOnly & fExternalRO )
	{
		// try open as RO
		mFd = open64( myfile, O_RDONLY|O_LARGEFILE );
		if( mFd == -1 )
			throw xOSFileError( ERR_OS_FILE_NOT_FOUND, fileNameU );
	}
	else
	{
		// Try open as RW
		mFd = open64( myfile, O_RDWR|O_LARGEFILE );

		// Check if error then try open it as RO
		if(  mFd == -1 )
		{
			// Check if file is not directory
			if( errno == EISDIR )
			{
				throw xOSFileError( ERR_OS_FILE_NOT_FOUND, fileNameU );
			}

			// try open as RO
			mFd = open64( myfile, O_RDONLY|O_LARGEFILE );
			if( mFd == -1 )
			{
				throw xOSFileError( ERR_OS_FILE_NOT_FOUND, fileNameU );
			}
			else
			{
				mReadOnly |= fExternalRO;
			}
		}
	}

	// IS: 14.06.2011
	// Bug: #5566
	//
	if( flock( mFd, LOCK_EX | LOCK_NB )  == -1 )
	{
		DoClose();
		throw xOSFileError( ERR_OS_FILE_BUSY, fileNameU );
	}
}


/**********************************************************************************************/
void File_Posix::DoPutLength( flength inLength )
{
	mNeedFlush = true;
	// KP it is don't need on Linux. File size is growed when do Write().
	// Seek( inLength, begin );

	// ! Warning. if inLength < current length the file will cutting to the inLength
	if ( ftruncate64( mFd, inLength ) )
		throw xOSFileError( (ERROR_TYPE) errno );
}


/**********************************************************************************************/
flength	File_Posix::DoGetLengthOfClosed( void ) const
{
	flength res = 0;
	struct stat statbuf;

	if( stat(get_Path().getBufferA(), &statbuf) == 0 )
		res = statbuf.st_size;

	return res ;
}


/**********************************************************************************************/
flength	File_Posix::DoGetLengthOfOpened( void ) const
{
	off64_t cur_pos   = 0;
	off64_t file_size = 0;

	cur_pos = lseek64(mFd, 0, SEEK_CUR);
	if( cur_pos == -1 )
		return 0;

	file_size = lseek64(mFd, 0, SEEK_END);
	lseek64(mFd, cur_pos, SEEK_SET);

	return (flength) file_size;
}


/**********************************************************************************************/
void File_Posix::DoThrowOut( void )
{
	// Just to check that file has no locks and we can drop it.
	// (Linux specific - yes, we set flock on Create/Open but some system functions like 
	// unlink() ignore that locks).
	//
	DoOpen();
	DoClose();
	
	const char* myfile = get_Path().getBufferA();

	// Close file
	int res = unlink( myfile );

	// Check on error
	if( res == -1 )
		throw xOSFileError( (ERROR_TYPE) errno );
}


/**********************************************************************************************/
vuint32 File_Posix::DoRead(
	char* 	inBuffer,
	flength	inFrom,
	vuint32	inHowMuch )
{
	FBL_CHECK(inBuffer);

	/* pre: read existing information */
	FBL_CHECK( inFrom + inHowMuch <= get_Length() );

	/* Set file marker to right position */
	//Seek(inFrom, begin);

	// read
	ssize_t res =  pread64( mFd, inBuffer, inHowMuch, inFrom );

	if( res == -1 )
	{
		throw xOSFileError( (ERROR_TYPE) errno );
	}

	return res == -1 ? 0: (vuint32) res;
}


/**********************************************************************************************/
// Finds the given position in the file, then writes a given buffer.
//
vuint32 File_Posix::DoWrite(
	const char* inBuffer,
	flength		inFrom,
	vuint32 	inHowMuch )
{
	mNeedFlush = true;
	
	// ulong writen = 0;
	FBL_CHECK(inBuffer);

	if( mReadOnly )
		FBL_Throw(xOSFileError(ERR_OS_FILE_READ_ONLY));

	// write
	ssize_t res = pwrite64( mFd, inBuffer, inHowMuch, inFrom );

	if( res == -1 )
	{
		throw xOSFileError( (ERROR_TYPE) errno );
	}

	return res == -1? 0: (vuint32) res;
}


/**********************************************************************************************/
FBL_End_Namespace

#endif // FBL_POSIX && !FBL_MAC
