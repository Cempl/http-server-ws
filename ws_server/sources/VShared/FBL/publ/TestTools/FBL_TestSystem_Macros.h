/**********************************************************************************************/
/* FBL_TestSystem_Macros.h                                                    				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_TestSystem_Macros_h 
	#define _FBL_TestSystem_Macros_h
#pragma once 

#include <VShared/FBL/publ/Headers/FBL_Macros.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// 1 if we want to generate code of self-testing functions.
//
#ifndef FBL_TEST_CODE
#	define FBL_TEST_CODE 0
#endif


/**********************************************************************************************/
#if FBL_TEST_CODE


/**********************************************************************************************/
// DO_TEST(t) macro should be used like ASSERT.
// 	It prints DONE/FAIL result into the test_log file. 
//
// DO_TEST_MSG(p,msg) macro allows display customised message instead of "p" string.
//
// LOG_EXC(e) macro allows to log information about catched FBL xException.
// LOG_EXC_MSG(e) macro do the same, but allows you to provide own message text.
//
#if FBL_MAC
		#define DO_TEST(p) { ++gTotalTestCount;\
							LogTest( __FILE__, __func__, __LINE__, #p, bool(p) ); }
		#define DO_TEST_MSG(p,msg) { ++gTotalTestCount;\
							LogTest( __FILE__, __func__, __LINE__, msg, bool(p) ); }
		#define LOG_EXC(pe) { ++gTotalTestCount;\
							LogException( __FILE__, __func__, __LINE__, pe ); }
		#define LOG_EXC_MSG(pe, msg) { ++gTotalTestCount;\
							LogException( __FILE__, __func__, __LINE__, pe, msg ); }
#elif FBL_WIN
		#define DO_TEST(p) { ++gTotalTestCount;\
							LogTest( __FILE__, __FUNCTION__, __LINE__, #p, bool(p) ); }
		#define DO_TEST_MSG(p,msg) { ++gTotalTestCount;\
							LogTest( __FILE__, __FUNCTION__, __LINE__, msg, bool(p) ); }
		#define LOG_EXC(pe) { ++gTotalTestCount;\
							LogException( __FILE__, __FUNCTION__, __LINE__, pe ); }
		#define LOG_EXC_MSG(pe,msg) { ++gTotalTestCount;\
							LogException( __FILE__, __FUNCTION__, __LINE__, pe, msg ); }
#elif FBL_UNIX
		#define DO_TEST(p) { ++gTotalTestCount;\
							LogTest( __FILE__, __func__, __LINE__, #p, bool(p) ); }
		#define DO_TEST_MSG(p,msg) { ++gTotalTestCount;\
							LogTest( __FILE__, __func__, __LINE__, msg, bool(p) ); }
		#define LOG_EXC(pe) { ++gTotalTestCount;\
							LogException( __FILE__, __func__, __LINE__, pe ); }
		#define LOG_EXC_MSG(pe, msg) { ++gTotalTestCount;\
							LogException( __FILE__, __func__, __LINE__, pe, msg ); }
#endif // FBL_MAC


//
// SKIPPED versions, to be able DO NOTHING for some long-time broken tests.
//
#define DO_TEST_SKIP(p)				{ ++gDoItFixMeZeroCount; }
#define DO_TEST_MSG_SKIP(p, msg)	{ ++gDoItFixMeZeroCount; }


/**********************************************************************************************/
// DO_TEST_EQ macro.
// 	It allows put into the log file the values of 2 compared numbers. 
//	NOTE: This macro do not work for strings! See next macro.
// 
#if FBL_MAC

	#define DO_TEST_EQ( expr, expectedValue ) { ++gTotalTestCount;\
		LogTest( __FILE__, __func__, __LINE__, #expr, '=', #expectedValue, expr, expectedValue ); }
		
#elif FBL_WIN

	#define DO_TEST_EQ( expr, expectedValue ) { ++gTotalTestCount;\
		LogTest( __FILE__, __FUNCTION__, __LINE__, #expr, '=', #expectedValue, expr, expectedValue ); }

#elif FBL_UNIX

	#define DO_TEST_EQ( expr, expectedValue ) { ++gTotalTestCount;\
		LogTest( __FILE__, __func__, __LINE__, #expr, '=', #expectedValue, expr, expectedValue ); }

#endif // FBL_MAC


//
// SKIPPED versions, to be able DO NOTHING for some long-time broken tests.
//
#define DO_TEST_EQ_SKIP( expr, expectedValue ) { ++gDoItFixMeZeroCount; }


/**********************************************************************************************/
// DO_TEST_EQ_ASTR macro allow you compare 2 char* strings and log result.
// 
#if FBL_MAC

	#define DO_TEST_EQ_ASTR( s1, s2 ) { ++gTotalTestCount;\
		LogTest( __FILE__, __func__, __LINE__, #s1, '=', #s2, strcmp(s1,s2) == 0, s1, s2 ); }
		
#elif FBL_WIN

	#define DO_TEST_EQ_ASTR( s1, s2 ) { ++gTotalTestCount;\
		LogTest( __FILE__, __FUNCTION__, __LINE__, #s1, '=', #s2, strcmp(s1,s2) == 0, s1, s2 ); }

#elif FBL_UNIX

	#define DO_TEST_EQ_ASTR( s1, s2 ) { ++gTotalTestCount;\
		LogTest( __FILE__, __func__, __LINE__, #s1, '=', #s2, strcmp(s1,s2) == 0, s1, s2 ); }

#endif // FBL_MAC


//
// SKIPPED versions, to be able DO NOTHING for some long-time broken tests.
//
#define DO_TEST_EQ_ASTR_SKIP( s1, s2 ) { ++gDoItFixMeZeroCount; }


/**********************************************************************************************/
#define DO_TEST_ILLEGAL(x, err)\
	try\
	{\
		(x);\
		DO_TEST(false);\
	}\
	catch( xException& e )\
	{\
		DO_TEST_EQ(e.get_ErrorCode(), (err));\
	}


/**********************************************************************************************/
#define DO_TEST_BREAK(x)\
	if( !(x) )\
	{\
		DO_TEST(0);\
		break;\
	}


/**********************************************************************************************/
#endif // FBL_TEST_CODE

	
/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_TestSystem_Macros_h
