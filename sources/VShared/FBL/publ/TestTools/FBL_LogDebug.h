/**********************************************************************************************/
/* FBL_LogDebug.h	                                                   						  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_LogDebug_h 
	#define _FBL_LogDebug_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>

#include <loki/TypeTraits.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE( I_LogFile );
SMART_INTERFACE( I_OStream );

class xException;


#pragma mark -
#pragma mark ====== XML Log =======


/**********************************************************************************************/
FBL_SHARED_EXP extern const char* pCurrentClass;


/**********************************************************************************************/
FBL_SHARED_EXP void LogXml_Testsuites_start( void );
FBL_SHARED_EXP void LogXml_Testsuites_end( void );


FBL_SHARED_EXP void LogXml_Testsuite_start( const char* inTestSuiteName );
FBL_SHARED_EXP void LogXml_Testsuite_end( void );


FBL_SHARED_EXP void LogXml_TestCase_start( const char* inTestCaseName );
FBL_SHARED_EXP void LogXml_Failure( const char* inMsg );
FBL_SHARED_EXP void LogXml_TestCase_end( void );


#pragma mark -


/**********************************************************************************************/
FBL_SHARED_EXP	void 		LogTest( 	
								const char* inFile, 
								const char* inFunc, 
								vuint32		inLine,
								const char* inMsg,
								bool		inSucces,
								bool		inEndLines = true );

FBL_SHARED_EXP void 		LogTest( 	
								const char* inFile, 
								const char* inFunc, 
								vuint32		inLine,
								const char* inLeftStr, 
								char 		inOpStr, 
								const char* inRightStr,
								bool		inSucces,
								const char* inLeft,
								const char* inRight );

/**********************************************************************************************/
FBL_SHARED_EXP void LogTest( 	
	const char* inFile, const char* inFunc, vuint32 inLine, 
	const char* inLeftStr, char inOpStr, const char* inRightStr, 
	bool inLeft, bool inRight );

FBL_SHARED_EXP void LogTest( 	
	const char* inFile, const char* inFunc, vuint32 inLine, 
	const char* inLeftStr, char inOpStr, const char* inRightStr, 
	long inLeft, long inRight );

FBL_SHARED_EXP void LogTest( 	
	const char* inFile, const char* inFunc, vuint32 inLine, 
	const char* inLeftStr, char inOpStr, const char* inRightStr,
	vuint32 inLeft, vuint32 inRight );

FBL_SHARED_EXP void LogTest( 	
	const char* inFile, const char* inFunc, vuint32 inLine, 
	const char* inLeftStr, char inOpStr, const char* inRightStr,
	double inLeft, double inRight );

FBL_SHARED_EXP void LogTest( 	
	const char* inFile, const char* inFunc, vuint32 inLine, 
	const char* inLeftStr, char inOpStr, const char* inRightStr,
	void* inLeft, void* inRight );


/**********************************************************************************************/
template <class T>
/*FBL_SHARED_EXP*/ void LogTest( 	
	const char* inFile, const char* inFunc, vuint32 inLine, 
	const char* inLeftStr, char inOpStr, const char* inRightStr,
	smart_ptr<T> inLeft, smart_ptr<T> inRight )
{
	void* p1 = inLeft.get();	
	void* p2 = inRight.get();	
	
	LogTest( inFile, inFunc, inLine, inLeftStr, inOpStr, inRightStr, p1, p2 );
}

/**********************************************************************************************/
template <class T>
/*FBL_SHARED_EXP*/ void LogTest( 	
	const char* inFile, const char* inFunc, vuint32 inLine, 
	const char* inLeftStr, char inOpStr, const char* inRightStr,
	smart_ptr<T> inLeft, void* inRight )
{
	void* p1 = inLeft.get();	
	
	LogTest( inFile, inFunc, inLine, inLeftStr, inOpStr, inRightStr, p1, inRight );
}


/**********************************************************************************************/
template <class T>
/*FBL_SHARED_EXP*/ void LogTest( 	
	const char* inFile, const char* inFunc, vuint32 inLine, 
	const char* inLeftStr, char inOpStr, const char* inRightStr,
	T inLeft, T inRight )
{
	if( Loki::TypeTraits<T>::isFloat )
	{
		LogTest( inFile, inFunc, inLine, inLeftStr, inOpStr, inRightStr, 
			(double)inLeft, (double) inRight );
	}
	else if( Loki::TypeTraits<T>::isSignedInt )
	{
		LogTest( inFile, inFunc, inLine, inLeftStr, inOpStr, inRightStr, 
			(long) inLeft, (long) inRight );
	}
	else if( Loki::TypeTraits<T>::isUnsignedInt )
	{
		LogTest( inFile, inFunc, inLine, inLeftStr, inOpStr, inRightStr, 
			(vuint32) inLeft, (vuint32) inRight );
	}
	else
	{
		LogTest( inFile, inFunc, inLine, inLeftStr, inOpStr, inRightStr, 
			(int) inLeft, (int) inRight );
	}
}


/**********************************************************************************************/
template <class T, class Y>
/*FBL_SHARED_EXP*/ void LogTest( 	
	const char* inFile, const char* inFunc, vuint32 inLine, 
	const char* inLeftStr, char inOpStr, const char* inRightStr,
	T inLeft, Y inRight )
{
	LogTest( inFile, inFunc, inLine, inLeftStr, inOpStr, inRightStr, 
		inLeft, (T) inRight );
}


/**********************************************************************************************/
FBL_SHARED_EXP	void 		LogException( 	
								const char* inFile, 
								const char* inFunc, 
								vuint32		inLine,
								xException*	inExc,
								const char*	inMessage = nullptr );	
								

/**********************************************************************************************/
FBL_SHARED_EXP	void 		LogTestTitle( 	
								const char* inName );

FBL_SHARED_EXP	void 		LogTestName( 	
								const char* inName );

FBL_SHARED_EXP	void 		LogTestLevelName( 	
								const char* inName );
			

/**********************************************************************************************/
// Access to standard debug log files of kernel.
//
FBL_SHARED_EXP	I_LogFile*			GetAssertsLog( void );
FBL_SHARED_EXP	I_LogFile*			GetTestsLog( void );
FBL_SHARED_EXP	I_LogFile*			GetTestsLogXml( void );
FBL_SHARED_EXP	I_LogFile* 			GetBenchLog( void );

FBL_SHARED_EXP	I_OStream*			GetEmbFilesLog( void );


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // _FBL_LogDebug_h
