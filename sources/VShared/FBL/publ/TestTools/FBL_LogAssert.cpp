/**********************************************************************************************/
/* FBL_LogAssert.cpp                 	                                     				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

// VSHARED:
#include <VShared/FBL/publ/Interfaces/FBL_I_LogFile.h>
//
#include <VShared/FBL/publ/Util_classes/FBL_String.h>
#include <VShared/FBL/publ/Utilities/FBL_Util_PathA.h>
//
#include <VShared/FBL/publ/Threads/FBL_Synch.h>


/**********************************************************************************************/
#if __CHECK


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
void LogAssert( 	
	const char* inFile, 
	const char* inFunc, 
	vuint32 	inLine,
	const char* inMsg );


/**********************************************************************************************/
// Singleton of "FBL_Asserts.txt" assert log file.
// Is used by test system of Valentina.
//
I_LogFile* GetAssertsLog( void )
{
	FBL_ASSERT_LOG_LOCK;

static I_LogFile_Ptr sLogFile = nullptr;

#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
	return NULL;
#else

	if( sLogFile == nullptr )
	{
		// Strictly create FBL_Asserts.txt in app folder (not vlogs folder)
		// to prevent unintendent recursion ( CreateLogFile( "V4RB..." )
		// can cause some assert before vlogs is actually created ...).
		//
		sLogFile = CreateLogFileInAppFolder( "FBL_Asserts.txt" );

	}

#endif 
	
	return sLogFile.get();
}


#pragma mark -


/**********************************************************************************************/
// Do log of some failed assert into log file.
//
void LogAssert( 	
	const char* inFile, 
	const char* inFunc, 
	vuint32 	inLine,
	const char* inMsg )
{
	FBL_ASSERT_LOG_LOCK;

	++gAssertFailCount;

	I_LogFile& log = *GetAssertsLog();
	
	if( log.get_VerboseLevel() == 0 )
		return;

	if( log.get_VerboseLevel() >= 1 )	// show all:
	{
		// Visual produce __FILE__ as ful path. We want extract only file name.
		inFile = ExtractNameFromFullPath( inFile );

		log << inFile << ":" << inFunc << ":" << (vuint32) inLine << " " << inMsg << "\n";
	}
	
	//log.Flush();
}


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // __CHECK
