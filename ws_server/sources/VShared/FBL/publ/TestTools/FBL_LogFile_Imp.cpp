/**********************************************************************************************/
/* FBL_LogFile_Imp.cpp                                                      				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/TestTools/FBL_LogFile_Imp.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_IStream.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Disk_Location.h>
#include <VShared/FBL/publ/Streams/FBL_Stream_OnFile.h>
#include <VShared/FBL/publ/Threads/FBL_Synch.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
static CriticalSection gLogMutex;

#define V_LOCK_LOG\
	StCriticalSectionLocker g(gLogMutex);


/**********************************************************************************************/
I_LogFile* CreateLogFileInAppFolder( 
	const String& 	inLogFileName, 
	vuint8 			inVerboseLevel, 
	bool 			inFlushEachStep )
{
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR

	argused3(inLogFileName, inVerboseLevel, inFlushEachStep );

	return NULL;
    
#else

	V_LOCK_LOG;
		
	// Special case for something like FBL_Asserts.txt which can be retrieved
	// during CreateLogFile( "V4RB..." ) call which is about to create a vlogs folder.
	// To prevent such a recursions we create FBL_ASsert.txt strictly in app folder.
	//
	return new FBL::LogFile_Imp( inLogFileName, inVerboseLevel, inFlushEachStep );

#endif // TARGET_OS_IPHONE	
}

 
/**********************************************************************************************/
I_LogFile* CreateLogFile( 
	I_Location_Ptr	inLocation, 
	vuint8 			inVerboseLevel, 
	bool 			inFlushEachStep )
{
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR

	argused3(inLocation, inVerboseLevel, inFlushEachStep );

	return NULL;

#else

	V_LOCK_LOG;

	I_LogFile* lf = new FBL::LogFile_Imp( inLocation, inVerboseLevel, inFlushEachStep );

	return lf;

#endif // TARGET_OS_IPHONE	
}

 
/**********************************************************************************************/
I_LogFile* CreateLogFile( 
	const String& 	inLogFileName, 
	vuint8 			inVerboseLevel, 
	bool 			inFlushEachStep )
{
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR

	argused3(inLogFileName, inVerboseLevel, inFlushEachStep );

	return NULL;
    
#else

	V_LOCK_LOG;
		
	#if FBL_MAC
		I_Disk_Location_Ptr pLogsCatalogLocation = FBL::CreateDiskLocationFromFSRef( LOG_FOLDER_NAME );
	#else
		I_Disk_Location_Ptr pLogsCatalogLocation = FBL::CreateDiskLocation( LOG_FOLDER_NAME );
	#endif // FBL_MAC				

	I_LogFile* lf = nullptr;

	if( pLogsCatalogLocation )
	{
		pLogsCatalogLocation = PrepareAppRelativeFolderLocation( pLogsCatalogLocation );
		I_Location_Ptr pLoc = pLogsCatalogLocation->get_ChildLocation( inLogFileName.begin() );
		lf = new FBL::LogFile_Imp( pLoc, inVerboseLevel, inFlushEachStep );
	}
	else
	{
		lf = new FBL::LogFile_Imp( inLogFileName, inVerboseLevel, inFlushEachStep );
	}

	return lf;

#endif // TARGET_OS_IPHONE	
}

 
/**********************************************************************************************/
I_LogFile* CreateLogFile( 
	const String& 	inLogFolderPath, 
	const String& 	inLogFileName, 
	vuint8 			inVerboseLevel, 
	bool 			inFlushEachStep )
{
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR

	argused4(inLogFolderPath, inLogFileName, inVerboseLevel, inFlushEachStep );

	return NULL;

#else

	V_LOCK_LOG;

	I_LogFile* lf = new FBL::LogFile_Imp( 
										inLogFolderPath, 
										inLogFileName, 
										inVerboseLevel, 
										inFlushEachStep );
	return lf;
	
#endif // TARGET_OS_IPHONE	
}


#pragma mark -

/**********************************************************************************************/
LogFile_Imp::LogFile_Imp( 
	I_Location_Ptr	inLocation, 
	vuint8 			inVerboseLevel,
	bool	 		inFlushEachStep )
{
	mVerboseLevel = inVerboseLevel;
	mFlushEachStep = inFlushEachStep;

	Init( inLocation );
}


/**********************************************************************************************/
LogFile_Imp::LogFile_Imp( 
	const String& 	inLogFileName, 
	vuint8 			inVerboseLevel,
	bool	 		inFlushEachStep )
{
	mVerboseLevel = inVerboseLevel;
	mFlushEachStep = inFlushEachStep;

	I_Location_Ptr pLoc = I_Disk_Location::GetAppLocation()->
							get_ChildLocation( inLogFileName.c_str() );

	Init( pLoc );
}


/**********************************************************************************************/
LogFile_Imp::LogFile_Imp( 
	const String& 	inLogFolderPath,
	const String& 	inLogFileName, 
	vuint8 			inVerboseLevel,
	bool	 		inFlushEachStep )
{
	mVerboseLevel = inVerboseLevel;
	mFlushEachStep = inFlushEachStep;

#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR 

	argused4( inLogFolderPath, inLogFileName, inVerboseLevel, inFlushEachStep );

    // cannot be log so far ...

#else // TARGET_OS_IPHONE

	#if FBL_MAC
		I_Location_Ptr pLoc = CreateDiskLocationFromFSRef( (inLogFolderPath + inLogFileName).c_str() );
	#else
		I_Location_Ptr pLoc = CreateDiskLocation( (inLogFolderPath + inLogFileName).c_str() );
	#endif // FBL_MAC	

	Init( pLoc );

#endif // TARGET_OS_IPHONE
}



/**********************************************************************************************/
void LogFile_Imp::Init( I_Location_Ptr	inLocation )
{
	V_LOCK_LOG;

	try // RZ: can throw e.g. if location is read only.
	{
		I_IStream_Ptr pIStream = CreateTextStream( 
								dynamic_cast<I_Disk_Location*>( inLocation.get() ) );
		 
		 // RZ: replaced on dynamic_cast to NOT log this dynamic_cast.
		 // Otheriwse we get loop.
		 //
		 // mpStream = fbl_dynamic_cast<I_OStream>( pIStream );
		 mpStream = dynamic_cast<I_OStream*>( pIStream.get() );
	}
	catch(...)
	{
	}
}

/**********************************************************************************************/
LogFile_Imp::~LogFile_Imp( void )
{
	//if( mpStream )
	//	delete mpStream;
}


#pragma mark -


/**********************************************************************************************/
void LogFile_Imp::put_VerboseLevel( vuint8 inLevel ) 	
{ 
	V_LOCK_LOG;
	mVerboseLevel = inLevel; 
}
 

/**********************************************************************************************/
void LogFile_Imp::Flush( void )
{
	V_LOCK_LOG;

	if( mpStream )
		mpStream->Flush();
}


/**********************************************************************************************/
void LogFile_Imp::Close( void )
{
	V_LOCK_LOG;

	if( mpStream )
	{
		Flush();
		mpStream->Close();
	}
}


#pragma mark -


/**********************************************************************************************/
LogFile_Imp& LogFile_Imp::operator<<( char s )
{
	V_LOCK_LOG;

	if( mpStream )
	{
		mpStream->put(s);
		
		if( mFlushEachStep )
			mpStream->Flush();
	}
	
	return *this;
}


/**********************************************************************************************/
LogFile_Imp& LogFile_Imp::operator<<( vuint8 s )
{
	V_LOCK_LOG;

	if( mpStream )
	{
		mpStream->put(s);
		
		if( mFlushEachStep )
			mpStream->Flush();
	}
	
	return *this;
}


/**********************************************************************************************/
LogFile_Imp& LogFile_Imp::operator<<( vint16 s )
{
	V_LOCK_LOG;

	if( mpStream )
	{
		mpStream->put(s);
		
		if( mFlushEachStep )
			mpStream->Flush();
	}

	return *this;
}


/**********************************************************************************************/
LogFile_Imp& LogFile_Imp::operator<<( vuint16 s )
{
	V_LOCK_LOG;

	if( mpStream )
	{
		mpStream->put(s);
		
		if( mFlushEachStep )
			mpStream->Flush();
	}

	return *this;
}


/**********************************************************************************************/
LogFile_Imp& LogFile_Imp::operator<<( vint32 s )
{
	V_LOCK_LOG;

	if( mpStream )
	{
		mpStream->put(s);
		
		if( mFlushEachStep )
			mpStream->Flush();
	}

	return *this;
}


/**********************************************************************************************/
LogFile_Imp& LogFile_Imp::operator<<( vuint32 s )
{
	V_LOCK_LOG;

	if( mpStream )
	{
		mpStream->put(s);
		
		if( mFlushEachStep )
			mpStream->Flush();
	}

	return *this;
}


/**********************************************************************************************/
LogFile_Imp& LogFile_Imp::operator<<( vint64 s )
{
	V_LOCK_LOG;

	if( mpStream )
	{
		mpStream->put(s);
		
		if( mFlushEachStep )
			mpStream->Flush();
	}

	return *this;
}


/**********************************************************************************************/
LogFile_Imp& LogFile_Imp::operator<<( vuint64 s )
{
	V_LOCK_LOG;

	if( mpStream )
	{
		mpStream->put(s);
		
		if( mFlushEachStep )
			mpStream->Flush();
	}

	return *this;
}


/**********************************************************************************************/
LogFile_Imp& LogFile_Imp::operator<<( float s )
{
	V_LOCK_LOG;

	if( mpStream )
	{
		mpStream->put(s);
		
		if( mFlushEachStep )
			mpStream->Flush();
	}

	return *this;
}


/**********************************************************************************************/
LogFile_Imp& LogFile_Imp::operator<<( double s )
{
	V_LOCK_LOG;

	if( mpStream )
	{
		mpStream->put(s);
		
		if( mFlushEachStep )
			mpStream->Flush();
	}

	return *this;
}


/**********************************************************************************************/
LogFile_Imp& LogFile_Imp::operator<<( const char* s )
{
	V_LOCK_LOG;

	if( mpStream )
	{
		mpStream->put( s );
		
		if( mFlushEachStep )
			mpStream->Flush();
	}

	return *this;
}


/**********************************************************************************************/
LogFile_Imp& LogFile_Imp::operator<<( const UChar* s )
{
	V_LOCK_LOG;

	if( mpStream )
	{
		mpStream->put( s );
			
		if( mFlushEachStep )
			mpStream->Flush();
	}

	return *this;
}


/**********************************************************************************************/
I_File_Ptr LogFile_Imp::get_File( void)
{
	V_LOCK_LOG;

	I_File_Ptr pRes;

	if( mFile )
	{
		pRes = mFile;
	}
	else if( mpStream )
	{
		Text_Stream_OnFile* pStreamOnFile = fbl_dynamic_cast<Text_Stream_OnFile>( mpStream ).get();
		if( pStreamOnFile )
			pRes = pStreamOnFile->get_File();
	}

	return pRes;
}


/**********************************************************************************************/
FBL_End_Namespace
