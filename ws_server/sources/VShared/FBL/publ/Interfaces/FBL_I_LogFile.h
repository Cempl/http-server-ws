/**********************************************************************************************/
/* FBL_I_LogFile.h	                                                      					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_LogFile_h
	#define _FBL_I_LogFile_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// forward declarations: 
//
class String;
//
SMART_INTERFACE(I_File);
SMART_INTERFACE(I_Location);


/**********************************************************************************************/
// this file declarations:
//
SMART_INTERFACE(I_LogFile);


/**********************************************************************************************/
FBL_SHARED_EXP	I_LogFile* 	CreateLogFileInAppFolder( 
								const String& inLogFileName, 
								vuint8 inVerboseLevel = 255,
								bool inFlushEachStep = false );


/**********************************************************************************************/
FBL_SHARED_EXP	I_LogFile* 	CreateLogFile( 
								I_Location_Ptr	inLocation, 
								vuint8 inVerboseLevel = 255,
								bool inFlushEachStep = false );	


/**********************************************************************************************/
FBL_SHARED_EXP	I_LogFile* 	CreateLogFile( 
								const String& inLogFileName, 
								vuint8 inVerboseLevel = 255,
								bool inFlushEachStep = false );


/**********************************************************************************************/
FBL_SHARED_EXP	I_LogFile* 	CreateLogFile(
								const String& inLogFolderPath,
								const String& inLogFileName, 
								vuint8 inVerboseLevel = 255,
								bool inFlushEachStep = false );	


/**********************************************************************************************/
// Interface class -- log file.
// 
interface FBL_SHARED_EXP_CLASS I_LogFile : public I_Unknown
{

virtual 					~I_LogFile( void );

	// ---------------------
	// Properties:

							// VerbosLevel of LogFile. 0 level - nothing write.
							// The higher level the more verbose is Log.	
virtual	vuint8				get_VerboseLevel( void ) const 	  	= 0;	
virtual	void				put_VerboseLevel( vuint8 inLevel ) 	= 0;


	// --------------------
	// Stream operators:
	
virtual	I_LogFile&			operator<<( char s ) 				= 0; //OLDKEEPAS: mLogFile

virtual	I_LogFile&			operator<<( vuint8 s ) 				= 0; //OLDKEEPAS: mLogFile

virtual	I_LogFile&			operator<<( vint16 s ) 				= 0; //OLDKEEPAS: mLogFile
virtual	I_LogFile&			operator<<( vuint16 s ) 				= 0; //OLDKEEPAS: mLogFile

virtual	I_LogFile&			operator<<( vint32 s ) 				= 0; //OLDKEEPAS: mLogFile
virtual	I_LogFile&			operator<<( vuint32 s ) 				= 0; //OLDKEEPAS: mLogFile

virtual	I_LogFile&			operator<<( vint64 s ) 				= 0; //OLDKEEPAS: mLogFile
virtual	I_LogFile&			operator<<( vuint64 s ) 				= 0; //OLDKEEPAS: mLogFile

virtual	I_LogFile&			operator<<( float s ) 				= 0; //OLDKEEPAS: mLogFile
virtual	I_LogFile&			operator<<( double s ) 				= 0; //OLDKEEPAS: mLogFile
 
virtual	I_LogFile&			operator<<( const char* s ) 		= 0; //OLDKEEPAS: mLogFile

virtual	I_LogFile&			operator<<( const UChar* s )		= 0; //OLDKEEPAS: mLogFile

virtual void				Flush( void )						= 0;
virtual void				Close( void )						= 0;

virtual I_File_Ptr			get_File( void )					= 0;

}; 


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_I_LogFile_h
