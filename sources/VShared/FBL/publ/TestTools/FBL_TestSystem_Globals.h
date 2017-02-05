/**********************************************************************************************/
/* FBL_TestSystem_Globals.h                                                    				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_TestSystem_Globals_h 
	#define _FBL_TestSystem_Globals_h
#pragma once 

// FBL:
//#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Threads/FBL_ThreadSafe.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
#if __CHECK


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// Global variables for TESTS statistic:
//
FBL_SHARED_EXP extern Atomic_vuint32			gTotalAssertCount;
FBL_SHARED_EXP extern Atomic_vuint32			gAssertFailCount;
FBL_SHARED_EXP extern Atomic_vuint32			gTotalTestCount;
FBL_SHARED_EXP extern Atomic_vuint32			gTotalTestGroup;
FBL_SHARED_EXP extern Atomic_vuint32			gTestFailCount;
FBL_SHARED_EXP extern Atomic_vuint32			gTestFailOnExceptionCount;
FBL_SHARED_EXP extern Atomic_vuint32			gSkippedAssertFailCount;


/**********************************************************************************************/
interface I_LogFile;

FBL_SHARED_EXP I_LogFile* GetAssertsLog( void );


/**********************************************************************************************/
FBL_SHARED_EXP void	LogAssert( 	
	const char* inFile, 
	const char* inFunc, 
	vuint32		inLine,
	const char* inMsg );


/**********************************************************************************************/
#if FBL_TEST_CODE


#pragma mark -
#pragma mark ====== Special Build Manipulation =======

/**********************************************************************************************/
FBL_SHARED_EXP extern bool gParserStartStmt_Assert;
FBL_SHARED_EXP extern bool gTypeImpDatabase_Assert;
FBL_SHARED_EXP extern bool gRangeInvariant_Assert;
FBL_SHARED_EXP extern bool gBitSetSetBits_Assert;
FBL_SHARED_EXP extern bool gSqlParserParseCommand_Assert;
FBL_SHARED_EXP extern bool gRBTreeNodeGetRight_Assert;
FBL_SHARED_EXP extern bool gRBTreeNodePutLeftNodePacked_Assert;
FBL_SHARED_EXP extern bool gRBTreeNodePutRightNodePacked_Assert;
FBL_SHARED_EXP extern bool gWrongConverter_Assert;


/**********************************************************************************************/
FBL_SHARED_EXP extern vuint32	gTest_SpecialBuild;
FBL_SHARED_EXP extern bool		gTest_DumpRBTree;


/**********************************************************************************************/
/**
	Stack-based class to change in one line of code global variables for tests,
    to affect result of GetCurrentDate() function located in the file 
    VShared/FBL/publ/Utilities/FBL_DateTimeUtilities.cpp
*/
class StChangeSpecialBuildForTests
{
	public://///////////////////////////////////////////////////////////////////////////////////

						StChangeSpecialBuildForTests( bool inIsBeta );
    					~StChangeSpecialBuildForTests( void );
};


#pragma mark -
#pragma mark ====== Build Date Manipulation =======


/**********************************************************************************************/
/** 
	Global var to affect result of GetBuildDate() function from
	VShared/FBL/publ/Utilities/FBL_DateTimeUtilities.cpp
*/
FBL_SHARED_EXP extern char gTest_BuildDate[11];


/**********************************************************************************************/
/**
	Stack-based class to change in one line of code global variables for tests,
    to affect result of GetCurrentDate() function located in the file 
    VShared/FBL/publ/Utilities/FBL_DateTimeUtilities.cpp
*/
class StChangeBuildDateForTests
{
	public://///////////////////////////////////////////////////////////////////////////////////

						StChangeBuildDateForTests( const char* inNewBuildDate );
    					~StChangeBuildDateForTests( void );
};


#pragma mark -
#pragma mark ====== Current Date Manipulation =======


/**********************************************************************************************/
FBL_SHARED_EXP extern Atomic_vint32 	gTest_Year;
FBL_SHARED_EXP extern Atomic_vuint16 	gTest_Month;
FBL_SHARED_EXP extern Atomic_vuint16 	gTest_Day;


/**********************************************************************************************/
/**
	Stack-based class to change in one line of code global variables for tests,
    to affect result of GetCurrentDate() function located in the file 
    VShared/FBL/publ/Utilities/FBL_DateTimeUtilities.cpp
*/
class StChangeCurrentDateForTests
{
	public://///////////////////////////////////////////////////////////////////////////////////

						StChangeCurrentDateForTests( vint32 inY, vuint16 inM, vuint16 inD );
    					~StChangeCurrentDateForTests( void );
};


/**********************************************************************************************/
#endif // FBL_TEST_CODE


#pragma mark -
#pragma mark ====== class TestStatisticStr =======


/**********************************************************************************************/
const int				gIdentStrLength = 100;


/**********************************************************************************************/
class FBL_SHARED_EXP_CLASS TestStatisticStr
{
	public://///////////////////////////////////////////////////////////////////////////////////

							TestStatisticStr( void );

		void				PutIndentStr( int inIndex, char inValue );
		const char*			GetIndentStr( void );


	private:////////////////////////////////////////////////////////////////////////////////////

		Thread_Mutex_Recursive	mIndentStrMutex;

		// We use this string buffer to build string with few spaces
		// to indent output of tests levels.
		char				mIndentStr[gIdentStrLength];

};


/**********************************************************************************************/
FBL_SHARED_EXP TestStatisticStr& GetTestStatisticStr( void );


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // __CHECK

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_TestSystem_Globals_h
