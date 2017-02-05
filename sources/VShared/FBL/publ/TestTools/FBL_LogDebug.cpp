/**********************************************************************************************/
/* FBL_LogDebug.cpp                 	                                     				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include "FBL_LogDebug.h"

// VSHARED:
#include <VShared/FBL/publ/Interfaces/FBL_I_Disk_Location.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_LogFile.h>

#include <VShared/FBL/publ/Errors/FBL_Exceptions.h>
#include <VShared/FBL/publ/Streams/FBL_Stream_OnFile.h>
#include <VShared/FBL/publ/Values/FBL_type_traits.h>

#include <VShared/FBL/publ/Utilities/FBL_Util_Path.h>
#include <VShared/FBL/publ/Utilities/FBL_Util_PathA.h>

#include <VShared/FBL/publ/TestTools/FBL_TestSystem_Globals.h>


/**********************************************************************************************/
#if FBL_TEST_CODE


/**********************************************************************************************/
FBL_Begin_Namespace


#ifdef _MSC_VER
#pragma warning(disable: 4311)
#endif //_MSC_VER


/**********************************************************************************************/
// TMP UGLY solution to push some names into XML file of tests results.
//
const char* pCurrentClass = NULL;


/**********************************************************************************************/
void LogXml_Testsuites_start( void )
{
	I_LogFile& log = *GetTestsLogXml();

	log << "<testsuites>\n";
}


/**********************************************************************************************/
void LogXml_Testsuites_end( void )
{
	I_LogFile& log = *GetTestsLogXml();

	log << "</testsuites>\n";
}


/**********************************************************************************************/
void LogXml_Testsuite_start( const char* inTestSuiteName )
{
	I_LogFile& log = *GetTestsLogXml();

	log << "<testsuite name=\"" << inTestSuiteName << ">\n";
}


/**********************************************************************************************/
void LogXml_Testsuite_end( void )
{
	I_LogFile& log = *GetTestsLogXml();

	log << "</testsuite>\n";
}


/**********************************************************************************************/
void LogXml_TestCase_start( const char* inTestCaseName )
{
	I_LogFile& log = *GetTestsLogXml();

	String xmlStr( inTestCaseName );
    xmlStr.addXmlEncoding();

	log << "<testcase name=\"" << xmlStr.c_str()
        << "\" classname=\""   << (pCurrentClass ? pCurrentClass : "VKERNEL")
        << "\">\n";
    
}


/**********************************************************************************************/
void LogXml_Failure( const char* inMsg )
{
	I_LogFile& log = *GetTestsLogXml();

	String xmlStr( inMsg );
    xmlStr.addXmlEncoding();

	log << "<failure message=\"" << xmlStr.c_str() << "\" type=\"\"> </failure>\n";
    
}


/**********************************************************************************************/
void LogXml_TestCase_end( void )
{
	I_LogFile& log = *GetTestsLogXml();

	log << "</testcase>\n";
}



#pragma mark -


/**********************************************************************************************/
// Produce line as
//
// ......DONE: FBL_Test_I_Unknown.cpp:Test_I_Unknown:52 c1 == 100
// ......FAIL: FBL_Test_I_Unknown.cpp:Test_I_Unknown:52 c1 == 100
//
void LogTest( 	
	const char* inFile, 
	const char* inFunc, 
	vuint32		inLine,
	const char* inMsg,
	bool		inSucces,
	bool		inEndLines )
{
	if( !inSucces )
		++gTestFailCount;

	I_LogFile& log = *GetTestsLog();					
	
	vuint8 level = log.get_VerboseLevel();

	if( level == 0 )
		return;
	
	// Visual produce __FILE__ as full path. We want extract only file name.
	inFile = ExtractNameFromFullPath( inFile );

	if( level == 1 )	// show only erros:
	{
		if( inSucces == false )	
		{											
			log << GetTestStatisticStr().GetIndentStr() << "FAIL: " 
				<< inFile << ":" << inFunc << ":" << (vuint32) inLine << " " << inMsg;

			if( inEndLines )  
				log << "\n";
		}
	}

	if( level >= 2 )	// show all:
	{
		if( inSucces )												
				log << GetTestStatisticStr().GetIndentStr() << "DONE: ";				
		else	log << GetTestStatisticStr().GetIndentStr() << "FAIL: ";

		log << inFile << ":" << inFunc << ":" << (vuint32) inLine << " " << inMsg;

		if( inEndLines )  
			log << "\n";
	}
    
    
    // XML:
    if( !inSucces )
    {
    	String logMsg;
		logMsg << inFile << ":" << inFunc << ":" << (vuint32) inLine << " " << inMsg;
    
    	logMsg.addXmlEncoding();
    	
    	LogXml_Failure( logMsg.getBufferA() );
        
    }
    

//	log.Flush();	
}


/**********************************************************************************************/
// Extended version of log. 
// in case we fail, we write into log expression value and what is expected.
//
// ......FAIL: FBL_Test_I_Unknown.cpp:Test_I_Unknown:52 c1 == 100: left = 15, right = 100.
//
void LogTest( 	
	const char* inFile, const char* inFunc, vuint32 inLine, 
	const char* inLeftStr, char inOpStr, const char* inRightStr, bool inSucces,
	const char* inLeft,
	const char* inRight )
{
	vint32 l1 = (vint32)strlen( inLeftStr );
	vint32 l2 = 2;
	vint32 l3 = (vint32)strlen( inRightStr );

	vint32 len = l1 + l2 + l3 + 1;

	MemPtr<char> buff( len );

	strcpy( buff, inLeftStr );

	switch( inOpStr )
	{
		case '=':	strcpy( buff + l1, "==" );	break;
		default	:	break;
	}

	
	strcpy( buff + l1 + l2, inRightStr );

	LogTest( inFile, inFunc, inLine, buff, inSucces, false );	
		
	I_LogFile& log = *GetTestsLog();					
	log << ": left = " << inLeft << ", right = " << inRight;

	if( inSucces )
		log << '\n';
	else 
		log << "     <<<<<<< FAIL <<< \n";	
}


/**********************************************************************************************/
// ADAPTOR: inLeft/inRight converted from bool to str.
// 
void LogTest( 	
	const char* inFile, const char* inFunc, vuint32 inLine, 
	const char* inLeftStr, char inOpStr, const char* inRightStr,
	bool inLeft, bool inRight )
{
	char s1[kBoolStrMaxLen + 1];
	char s2[kBoolStrMaxLen + 1];

	type_traits<bool>::to_str( &inLeft , s1, kBoolStrMaxLen + 1 );
	type_traits<bool>::to_str( &inRight, s2, kBoolStrMaxLen + 1 );
	
	bool Succes = false;
	
	switch( inOpStr )
	{
		case '=':	Succes = bool(inLeft == inRight); break;
		default	: 	break;
	}
	 	
	LogTest( inFile, inFunc, inLine, inLeftStr, inOpStr, inRightStr, Succes, s1, s2 );		
}


/**********************************************************************************************/
// ADAPTOR: inLeft/inRight converted from long to str.
// 
void LogTest( 	
	const char* inFile, const char* inFunc, vuint32 inLine, 
	const char* inLeftStr, char inOpStr, const char* inRightStr,
	long inLeft, long inRight )
{
	char s1[kLongStrMaxLen + 1];
	char s2[kLongStrMaxLen + 1];

	type_traits<vint32>::to_str( (vint32*)  &inLeft, s1, kLongStrMaxLen + 1 );
	type_traits<vint32>::to_str( (vint32*) &inRight, s2, kLongStrMaxLen + 1 );
	
	bool Succes = false;
	
	switch( inOpStr )
	{
		case '=':	Succes = bool(inLeft == inRight); break;
		default	: 	break;
	}
	 	
	LogTest( inFile, inFunc, inLine, inLeftStr, inOpStr, inRightStr, Succes, s1, s2 );		
}


/**********************************************************************************************/
// ADAPTOR: inLeft/inRight converted from vuint32 to str.
// 
void LogTest( 	
	const char* inFile, const char* inFunc, vuint32 inLine, 
	const char* inLeftStr, char inOpStr, const char* inRightStr,
	vuint32 inLeft, vuint32 inRight )
{
	char s1[kULongStrMaxLen + 1];
	char s2[kULongStrMaxLen + 1];

	type_traits<vuint32>::to_str( (vuint32*)&inLeft , s1, kULongStrMaxLen + 1 );
	type_traits<vuint32>::to_str( (vuint32*)&inRight, s2, kULongStrMaxLen + 1 );
	
	bool Succes = false;
	
	switch( inOpStr )
	{
		case '=':	Succes = bool(inLeft == inRight); break;
		default	: 	break;
	}
	 
	LogTest( inFile, inFunc, inLine, inLeftStr, inOpStr, inRightStr, Succes, s1, s2 );		
}


/**********************************************************************************************/
// ADAPTOR: inLeft/inRight converted from double to str.
// 
void LogTest( 	
	const char* inFile, const char* inFunc, vuint32 inLine, 
	const char* inLeftStr, char inOpStr, const char* inRightStr,
	double inLeft, double inRight )
{
	char s1[kDoubleStrMaxLen + 1];
	char s2[kDoubleStrMaxLen + 1];

	type_traits<double>::to_str( &inLeft , s1, kDoubleStrMaxLen + 1 );
	type_traits<double>::to_str( &inRight, s2, kDoubleStrMaxLen + 1 );
	
	bool Succes = false;
	
	switch( inOpStr )
	{
		case '=':	Succes = bool(inLeft == inRight); break;
		default	: 	break;
	}
	 	
	LogTest( inFile, inFunc, inLine, inLeftStr, inOpStr, inRightStr, Succes, s1, s2 );		
}


/**********************************************************************************************/
void LogTest( 	
	const char* inFile, const char* inFunc, vuint32 inLine, 
	const char* inLeftStr, char inOpStr, const char* inRightStr, 
	void* inLeft, void* inRight )
{
    argused6( inFile, inFunc, inLine, inLeftStr, inOpStr, inRightStr );
    argused2( inLeft, inRight );
    
	// RZ 07/26/11 
	// may be here it needs to make printf()  0x  pointer kind ?
	// TODO !!!!

//	LogTest( inFile, inFunc, inLine, inLeftStr, inOpStr, inRightStr,  
//		(const char*) inLeft, (const char*) inRight );		
}


#pragma mark -
 

/**********************************************************************************************/
void LogException( 	
	const char* inFile, 
	const char* inFunc, 
	vuint32		inLine,
	xException*	inExc,
	const char*	inMsg )
{
	++gTestFailCount;
	++gTestFailOnExceptionCount;

	I_LogFile& log 		= *GetTestsLog();

	vuint8 level = log.get_VerboseLevel();
	if( level == 0 )
		return;

	
    FBL::String logMsg;
    
	if( level >= 1 )	// show only erros:
	{
		// Visual produce __FILE__ as ful path. We want extract only file name.
		inFile = ExtractNameFromFullPath( inFile );

		logMsg << GetTestStatisticStr().GetIndentStr() << "FAIL: " << inFile << ":" << inFunc << ":" << (vuint32) inLine;
		if( inMsg )
		{
			logMsg << " " << inMsg;
		}

		logMsg << " EXCEPTION: ";

		if( inExc )
			logMsg << inExc->get_ErrorCode() << " : " << inExc->get_ErrorString().c_str();
		else
			logMsg << "UNKNOWN";

		logMsg  << "\n";
	}
    
    
    log << logMsg.c_str();
    
    // XML log:
    logMsg.addXmlEncoding();
    LogXml_Failure( logMsg.getBufferA() );

	//log.Flush();	
}


/**********************************************************************************************/
void LogTestTitle( 	
	const char* inName )
{
	I_LogFile& log = *GetTestsLog();					
	
	log << '\n';

	log << GetTestStatisticStr().GetIndentStr() 
	<< "************************************************************************************************\n";


	// We can have very long names of kind "{Level}/{Level}/[Test]"
	// We will wrap this long name to string in 80 chars.
	while( 1 )
	{
		vuint32 len = (vuint32)strlen( inName );
		if( len <= 100 )	
		{
			log << GetTestStatisticStr().GetIndentStr() << "* " << inName << '\n';
			break; 
		}
		else
		{
			char* p = (char*) inName;
			
			// search the last '/' which is less of 80 bytes
			while(1)
			{
				char* p2 = strchr( p + 1, '/' );
				
				if( p2 == NULL )
					break;
				
				if( p2 - inName > 100 )
				{
					// now p points to the last '/' which is less of inName + 80
					break;
				}
				else
				{
					p = p2;
				}
			}
			
			*p = 0;	// truncate string;
			
			log << GetTestStatisticStr().GetIndentStr() << "* " << inName << '\n';
		
			*p = '/';	// restore string
		
			inName = p + 1; 
		}
	}
		
	log << GetTestStatisticStr().GetIndentStr() 
	<< "************************************************************************************************\n";
}


#pragma mark -
#pragma mark ====== Singlentones =======


/**********************************************************************************************/
I_LogFile* GetTestsLog( void )
{
static I_LogFile_Ptr sLogFile = NULL;

	if( sLogFile == NULL )
	{
		sLogFile = CreateLogFileInAppFolder( "FBL_Tests.txt" );
	}
	
	return sLogFile.get();
}


/**********************************************************************************************/
I_LogFile* GetTestsLogXml( void )
{
static I_LogFile_Ptr sLogFile = NULL;

	if( sLogFile == NULL )
	{
		sLogFile = CreateLogFileInAppFolder( "FBL_Tests.xml" );
	}
	
	return sLogFile.get();
}


/**********************************************************************************************/
I_OStream* GetEmbFilesLog( void )
{
static I_OStream_Ptr sLogFile = NULL;

	if( sLogFile == NULL )
	{
		I_Disk_Location_Ptr pLoc = CreateDiskLocation( "FBL_EmbeddedFiles.txt" );
		
		I_IStream_Ptr pIStream = CreateTextStream(pLoc);
		sLogFile = fbl_dynamic_cast<I_OStream>(pIStream);
	}
	
	return sLogFile.get();
}


/**********************************************************************************************/
I_LogFile* GetBenchLog( void )
{
static I_LogFile_Ptr sLogFile = NULL;

	if( sLogFile == NULL )
	{
		sLogFile = CreateLogFileInAppFolder( "FBL_Bench.txt" );
	}
	
	return sLogFile.get();
}


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // FBL_TEST_CODE
