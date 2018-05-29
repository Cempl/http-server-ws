/**********************************************************************************************/
/* FBL_File_Mac.cpp                                                          				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017 											   			  	  */
/**********************************************************************************************/

#include <VShared/FBL/publ/File_OS/mac/FBL_File_Mac.h>

#include <VShared/FBL/publ/Utilities/FBL_Util_Path.h>

#include <VShared/FBL/publ/Location/FBL_Location_Disk_FSSpec.h>
#include <VShared/FBL/publ/Location/FBL_Location_FSRef.h>


/**********************************************************************************************/
#if FBL_MAC


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
File_Mac::File_Mac( 
	OSType 			inCreator,
	OSType 			inFileType )
:
	mNeedFlush(false)
{
	mMacCreator		= inCreator;
	mMacType		= inFileType;
		
	mRefNum			= 0;  
	mReadOnly		= fWritable;
}


/**********************************************************************************************/
File_Mac::~File_Mac( void ) 
{	
}


#pragma mark -
#pragma mark ===== I_Storage =====


/**********************************************************************************************/
// Forse to write on disk all cached information from this file.	
//
void File_Mac::Flush( void )
{
	if( mNeedFlush )
	{
		#if FBL_LENGTH <= 32
			ParamBlockRec pb;
			pb.ioParam.ioRefNum = mRefNum;
			PBFlushFile( &pb, false );
		#else
			::FSFlushFork( mRefNum );
		#endif	
		
		mNeedFlush = false;
	}
}


#pragma mark -
#pragma mark ===== File =====


/**********************************************************************************************/
// Make direct reading from the disk file of inHowMuch bytes to the inBuffer.
// Returns inHowMuch bytes was actually readed to the buffer.
//
vuint32 File_Mac::DoRead( 
	char* 	inBuffer, 
	flength inFrom, 
	vuint32 inHowMuch ) 
{ 
	OSErr err = 0;

#if FBL_LENGTH <= 32	

	/* Set file marker to right position */
	err = SetFPos(mRefNum, fsFromStart, (long)inFrom);
	err = FSRead(mRefNum,(long*) &inHowMuch, inBuffer);

#else

	ByteCount bytes = 0;
	err = ::FSReadFork(mRefNum, fsFromStart, (vint64)inFrom, inHowMuch, inBuffer, &bytes );	
	
	inHowMuch = bytes;	
				
#endif	
	
	if( err )
		throw xOSFileError( (ERROR_TYPE)  err );

	return inHowMuch;	
}


/**********************************************************************************************/
// Write from inBuffer to the disk file inHowMuch bytes starting at inFrom position.
// Returns inHowMuch bytes was actually writen. 
//
vuint32 File_Mac::DoWrite( 
	const char* inBuffer, 
	flength 	inFrom, 
	vuint32		inHowMuch ) 
{ 
	if( mReadOnly )
		FBL_Throw(xOSFileError(ERR_OS_FILE_READ_ONLY));

	mNeedFlush = true;
	OSErr err = 0;

#if FBL_LENGTH <= 32	
	err = ::SetFPos(mRefNum, fsFromStart, (long) inFrom);
	if( err )
		throw xOSFileError( (ERROR_TYPE)  err );

	err = ::FSWrite(mRefNum, (long*)&inHowMuch, inBuffer);
	if( err )
		throw xOSFileError( (ERROR_TYPE)  err );

#else	
	err = ::FSWriteFork(mRefNum, fsFromStart, (vint64)inFrom, inHowMuch, inBuffer, NULL);
	if( err )
		throw xOSFileError( (ERROR_TYPE)  err );
#endif	
	
	// was no error, so we have write down all required bytes.
	return inHowMuch;	
}


#pragma mark -


/**********************************************************************************************/
void File_Mac::DoCreate( void ) 
{
	mNeedFlush = true;

#if !__LP64__
	const FSSpec* pSpec = GetSpec();
	if( pSpec )
	{
		OSErr err = ::FSpCreate( pSpec, mMacCreator, mMacType, smSystemScript );
		if( err )
			throw xOSFileError( (ERROR_TYPE)  err );
	}
	else
#endif // __LP64__
	{
		Location_Disk_FSRef_Ptr pLoc = fbl_dynamic_cast<Location_Disk_FSRef>(mpLocation);	
		if( pLoc )
		{
			const String& fname = pLoc->get_FileName();
			const UChar* pname = fname.c_str();
			const FSRef* parentRef = pLoc->get_ParentFSRef();

			// set creator/type:
			FInfo finfo = {0};
			finfo.fdCreator = mMacCreator;
			finfo.fdType    = mMacType;
					
			FSCatalogInfo catInfo = {0};
			*(FInfo*)(&catInfo.finderInfo) = finfo; 

			FSRef newRef;
			OSStatus serr = FSCreateFileUnicode( 
								parentRef, 
								(vuint32) fname.length(), (const UniChar *) pname, 
								kFSCatInfoFinderInfo, &catInfo, &newRef, NULL );
			
			if( serr )
				throw xOSFileError( (ERROR_TYPE)  serr );
			else
				pLoc->put_ObjectFSRef( &newRef );			
		}
	}

	// OS do not open file on create.
	Open();
}


/**********************************************************************************************/
void File_Mac::DoClose( void ) 
{ 
	// -----------------------------
	// in case this file is cached we need remove pages from cache.
	I_AspectHost* pHost = dcast<I_AspectHost*>( this );
	if( pHost )
		pHost->SendMessageToAll( 1 );

#if FBL_LENGTH <= 32	
	OSErr err = ::FSClose(mRefNum);
#else	
	OSErr err = ::FSCloseFork(mRefNum);
#endif	

	mRefNum = 0;
	if( err )
		throw xOSFileError( (ERROR_TYPE)  err );
}


/**********************************************************************************************/
// Get length of file from MacOS. File can be closed.
// 
flength File_Mac::DoGetLengthOfClosed( void ) const
{
	flength res = 0;

#if !__LP64__
	CInfoPBRec cInfo;

	const FSSpec* ps = GetSpec();

	cInfo.hFileInfo.ioNamePtr 	= (StringPtr) ps->name;
	cInfo.hFileInfo.ioVRefNum 	= ps->vRefNum;
	cInfo.hFileInfo.ioFDirIndex = 0;
	cInfo.hFileInfo.ioDirID 	= ps->parID;
	
	OSErr err = PBGetCatInfoSync( &cInfo );
	argused1( err );

	res = (flength) cInfo.hFileInfo.ioFlLgLen;
#endif // __LP64__

	return res;
}


/**********************************************************************************************/
// Get length of file from MacOS, we know that file is opened. 
// 
flength File_Mac::DoGetLengthOfOpened( void ) const
{
	flength Length = 0;

#if FBL_LENGTH <= 32	
	OSErr err = ::GetEOF( mRefNum, (long*) &Length );
#else	
	OSErr err = ::FSGetForkSize( mRefNum, (vint64*) &Length );
#endif	

	argused1( err );

	return Length;
}


/**********************************************************************************************/
void File_Mac::DoOpen( void )
{ 
	OSErr 		err;
	OSStatus 	errs;
	
#if __LP64__
	FSIORefNum newRefNum = 0;	
#else
	short newRefNum = 0;	
#endif // __LP64__


// ---------------------------------------------------------------------------------------------
// DEFINE fsRef for file.
// 	a) converting FSSpec -- Old carbon way
//	b) extracting it from our Location class.

	FSRef fsRef;

#if FBL_SUPPORT_FSSPEC
	const FSSpec* pSpec = GetSpec();
	if( pSpec ) // we have Location_FSSpec
	{
		err = ::FSpMakeFSRef( pSpec, &fsRef );
		if( err )
			throw xOSFileError( ERR_OS_FILE_NOT_FOUND, get_Name().c_str() );
	}
	else
#endif // FBL_SUPPORT_FSSPEC
	{
		Location_Disk_FSRef_Ptr pLoc = fbl_dynamic_cast<Location_Disk_FSRef>( mpLocation );
		
		if( pLoc == NULL )
			throw xInternalError( ERR_INTERNAL_INVALID_PARAMETER, "open" );
		
		errs = pLoc->get_ObjectFSRef( &fsRef );
	}


// ---------------------------------------------------------------------------------------------
// DEFINE mReadOnly flag.

	// ----------------------------------
	// If we want to open file for writing we have to check it is possible.
	// And open for reading in case of such check fails.
	//
	if( mReadOnly == fWritable && (FSCheckLock( &fsRef ) != 0) )
		mReadOnly |= fExternalRO;

	SInt8 perm = SInt8( (mReadOnly & fExternalRO) ? fsRdPerm : fsRdWrPerm/*fsCurPerm*/ );


// ---------------------------------------------------------------------------------------------
// DO open of the file

	HFSUniStr255 dfName;
	FSGetDataForkName( &dfName );

	err = ::FSOpenFork( &fsRef, dfName.length, dfName.unicode, perm, &newRefNum );
	
	// IS: 2015/07/17
	// At least for sqlite databases - open already "opened file for writing" cause permErr -
	// not opWrErr. So we check both states here.
	if( err == opWrErr || err == permErr || err == afpAccessDenied )
	{
		// Can not open file for RW, so let's try open it for RO
		mReadOnly |= fExternalRO;
		SInt8 perm_RO = SInt8( fsRdPerm );
		err = ::FSOpenFork( &fsRef, dfName.length, dfName.unicode, perm_RO, &newRefNum );
	}

// ---------------------------------------------------------------------------------------------
// CHECK ERRORS

	if( err )
	{
		if( err == permErr )
		{
			const UChar* pName = get_Name().c_str();
			throw xOSFileError( ERR_OS_FILE_BUSY, pName );
		}
		else
		{
			const UChar* pName = get_Name().c_str();
			throw xOSFileError( ERR_OS_FILE_NOT_FOUND, pName );
		}
	}
	else
	{
// ---------------------------------------------------------------------------------------------
// STORE RESULT handle of opened file into data member mRefNum.

		mRefNum = newRefNum;
    }
}


/**********************************************************************************************/
void File_Mac::DoPutLength( flength inLength )
{
	mNeedFlush = true;

#if FBL_LENGTH <= 32	
	OSErr err = ::SetEOF( mRefNum, long(inLength) );	
#else	
	OSErr err = ::FSSetForkSize( mRefNum, fsFromStart, (vint64) inLength );	
#endif	

	if( err )
		throw xOSFileError( (ERROR_TYPE)  err );
}


/**********************************************************************************************/
void File_Mac::DoThrowOut( void )
{
#if !__LP64__
	const FSSpec* pSpec = GetSpec();

	if( pSpec )
	{
		OSErr err = ::FSpDelete( pSpec );
		if( err )
			throw xOSFileError( (ERROR_TYPE)  err );
	}
	else
#endif // __LP64__
	{
		FSRef fsRef;
		
		Location_Disk_FSRef_Ptr pLoc = fbl_dynamic_cast<Location_Disk_FSRef>(mpLocation);	
		
		OSStatus errs = pLoc->get_ObjectFSRef( &fsRef );
		argused1( errs );
	
		OSStatus serr = FSDeleteObject( &fsRef );	
		if( serr == 0 )
		{
			pLoc->put_ObjectFSRef( NULL );
		}
		else 
		{
			throw xOSFileError( (ERROR_TYPE)  serr );
		}
	}
}


#pragma mark -
#pragma mark ===== File_Mac =====

 
/**********************************************************************************************/
#if FBL_SUPPORT_FSSPEC
const FSSpec* File_Mac::GetSpec( void ) const 
{ 
	if( mpLocation == NULL )
		return NULL;
	
	return (FSSpec*) mpLocation->get_FSSpec(); 
}
#endif // FBL_SUPPORT_FSSPEC


/**********************************************************************************************/
flength	File_Mac::GetPos( void ) const	
{ 
	flength pos;

	OSErr err =  FSGetForkPosition( mRefNum, (vint64*) &pos );
	argused1( err );

	return pos;
}


/**********************************************************************************************/
void File_Mac::SetPos( flength pos )	
{ 
	OSErr err = FSSetForkPosition( mRefNum, fsFromStart, (vint64) pos );
	argused1( err );
}


/**********************************************************************************************/
FBL_End_Namespace

#endif // FBL_MAC



