/**********************************************************************************************/
/* FBL_TestSystem_Globals.cpp         		                                  				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Threads/FBL_Synch.h>
#include <VShared/FBL/publ/TestTools/FBL_TestSystem_Globals.h>


/**********************************************************************************************/
#if __CHECK


/**********************************************************************************************/
FBL_Begin_Namespace 


/**********************************************************************************************/
Atomic_vuint32			gTotalAssertCount 			{ 0 };
Atomic_vuint32			gAssertFailCount 			{ 0 };
Atomic_vuint32			gTotalTestCount 			{ 0 };
Atomic_vuint32			gTotalTestGroup 			{ 0 };
Atomic_vuint32			gTestFailCount 				{ 0 };
Atomic_vuint32			gTestFailOnExceptionCount 	{ 0 };
Atomic_vuint32			gSkippedAssertFailCount 	{ 0 };


#pragma mark -


/**********************************************************************************************/
#if FBL_TEST_CODE


#pragma mark -
#pragma mark ====== Special Build Manipulation =======


/**********************************************************************************************/
// Special assert flags to turn on/off "known" asserts in the tests.
//
bool gParserStartStmt_Assert				= true;
bool gTypeImpDatabase_Assert				= true;
bool gRangeInvariant_Assert					= true;
bool gBitSetSetBits_Assert					= true;
bool gSqlParserParseCommand_Assert			= true;
bool gRBTreeNodeGetRight_Assert				= true;
bool gRBTreeNodePutLeftNodePacked_Assert	= true;
bool gRBTreeNodePutRightNodePacked_Assert	= true;
bool gWrongConverter_Assert					= true;


/**********************************************************************************************/
/**
 	Helper variable for tests.
 	0 - use origial code with kSpecialBuildString
 	1 - test as RELEASE
 	2 - test as BETA
*/
vuint32 gTest_SpecialBuild = 0;


/**********************************************************************************************/
// If true - SegmentMap tree will be printed to the warning log.
//
bool gTest_DumpRBTree	= false;


/**********************************************************************************************/
StChangeSpecialBuildForTests::StChangeSpecialBuildForTests( bool inIsBeta )
{
	// Simplest -- no nested calls.
	FBL_CHECK( gTest_SpecialBuild == 0 );

	gTest_SpecialBuild = inIsBeta ? 2 : 1;
}


/**********************************************************************************************/
StChangeSpecialBuildForTests::~StChangeSpecialBuildForTests( void )
{
	gTest_SpecialBuild = 0;
}


#pragma mark -
#pragma mark ====== Build Date Manipulation =======


/**********************************************************************************************/
char gTest_BuildDate[11] = "";


/**********************************************************************************************/
StChangeBuildDateForTests::StChangeBuildDateForTests( const char* inNewBuildDate )
{
	// Simplest -- no nested calls.
	FBL_CHECK( *gTest_BuildDate == 0 );

	strncpy( gTest_BuildDate, inNewBuildDate, 11 );
}


/**********************************************************************************************/
StChangeBuildDateForTests::~StChangeBuildDateForTests( void )
{
	*gTest_BuildDate = 0;
}


#pragma mark -
#pragma mark ====== Current Date Manipulation =======


/**********************************************************************************************/
Atomic_vint32 	gTest_Year 	{ 0 };
Atomic_vuint16 	gTest_Month { 0 };
Atomic_vuint16 	gTest_Day 	{ 0 };


/**********************************************************************************************/
StChangeCurrentDateForTests::StChangeCurrentDateForTests( vint32 inY, vuint16 inM, vuint16 inD )
{
	// Simplest -- no nested calls.
    // But it is possible implement them if to use private data members.
	FBL_CHECK( gTest_Year == 0 );

	gTest_Year 	= inY;
	gTest_Month = inM;
	gTest_Day 	= inD;
}


/**********************************************************************************************/
StChangeCurrentDateForTests::~StChangeCurrentDateForTests( void )
{
	gTest_Year 	= 0;
	gTest_Month = 0;
	gTest_Day 	= 0;
}

/**********************************************************************************************/
#endif // FBL_TEST_CODE


#pragma mark -
#pragma mark ====== class TestStatisticStr =======


/**********************************************************************************************/
static TestStatisticStr* sTestStatisticStr = nullptr;


/**********************************************************************************************/
// Global string variables for TESTS statistic:
//
TestStatisticStr& GetTestStatisticStr( void )
{
	if( sTestStatisticStr == nullptr )
	{
		FBL_TEST_STATISTIC_LOCK;

		if( sTestStatisticStr == nullptr )
		{
			sTestStatisticStr = new TestStatisticStr;
		}
	}

	return *sTestStatisticStr;
}


#pragma mark -


/**********************************************************************************************/
// Global varaibles for TESTS string statistic:
//
TestStatisticStr::TestStatisticStr()
{
	memset( &mIndentStr, 0, gIdentStrLength );
}


/**********************************************************************************************/
void TestStatisticStr::PutIndentStr( int inIndex, char inValue )
{
	FBL_LOCK(mIndentStrMutex);
	mIndentStr[inIndex] = inValue;
}


/**********************************************************************************************/
const char* TestStatisticStr::GetIndentStr( void )
{
	FBL_LOCK(mIndentStrMutex);
	return mIndentStr;
}


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // __CHECK


