/**********************************************************************************************/
/* FBL_LogFile_Imp.h	                                                   					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_LogFile_Imp_h 
	#define _FBL_LogFile_Imp_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Interfaces/FBL_I_OStream.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_LogFile.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_File.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
class LogFile_Imp : public I_LogFile
{
		IMPLEMENT_UNKNOWN(LogFile_Imp)											
		BEGIN_INTERFACE_TABLE(LogFile_Imp)										
			IMPLEMENT_INTERFACE(I_LogFile)										
		END_INTERFACE_TABLE()

	public://///////////////////////////////////////////////////////////////////////////////////

	// --- Construction ---
	 	
							LogFile_Imp( 
								I_Location_Ptr	inLocation, 
								vuint8 			inVerboseLevel = 255,
								bool	 		inFlushEachStep = false );

							LogFile_Imp( 
								const String& 	inLogFileName, 
								vuint8 			inVerboseLevel = 255,
								bool	 		inFlushEachStep = false );

							LogFile_Imp(
								const String& 	inFolderPath, 
								const String& 	inLogFileName, 
								vuint8 			inVerboseLevel = 255,
								bool	 		inFlushEachStep = false );
							
virtual						~LogFile_Imp( void ); 
	
 							
	// --- Attributes ---

virtual	vuint8				get_VerboseLevel( void ) const override 		{ return mVerboseLevel; }	
virtual	void				put_VerboseLevel( vuint8 inLevel ) override;


	public://///////////////////////////////////////////////////////////////////////////////////

//	interface I_LogFile

virtual	LogFile_Imp&		operator<<( char s ) override;
virtual	LogFile_Imp&		operator<<( vuint8 s ) override;

virtual	LogFile_Imp&		operator<<( vint16 s ) override;
virtual	LogFile_Imp&		operator<<( vuint16 s ) override;

virtual	LogFile_Imp&		operator<<( vint32 s ) override;
virtual	LogFile_Imp&		operator<<( vuint32 s ) override;

virtual	LogFile_Imp&		operator<<( vint64 s ) override;
virtual	LogFile_Imp&		operator<<( vuint64 s ) override;

virtual	LogFile_Imp&		operator<<( float s ) override;
virtual	LogFile_Imp&		operator<<( double s ) override;

virtual	LogFile_Imp&		operator<<( const char* s ) override;
virtual	LogFile_Imp&		operator<<( const UChar* s ) override;

virtual void				Flush( void ) override;
virtual void				Close( void ) override;

virtual I_File_Ptr			get_File( void) override;


	private://///////////////////////////////////////////////////////////////////////////////////

		void				Init( I_Location_Ptr inLocation );

  	private://///////////////////////////////////////////////////////////////////////////////////

		vuint8				mVerboseLevel;
		bool				mFlushEachStep;

		I_File_Ptr			mFile;
		I_OStream_Ptr		mpStream;
};


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_LogFile_Imp_h
