/**********************************************************************************************/
/* FBL_I_File.h			                                                      				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_File_h 
	#define _FBL_I_File_h
#pragma once

#include <VShared/FBL/publ/Interfaces/FBL_I_Storage.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Location.h>

#include <VShared/FBL/publ/Util_classes/FBL_String.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE(I_File);
SMART_INTERFACE(I_FileIterator);


/**********************************************************************************************/
// Factory functions
//
FBL_SHARED_EXP	I_File* 			CreateFile( const char*		inPath );
FBL_SHARED_EXP	I_File* 			CreateFile( const UChar*	inPath );
FBL_SHARED_EXP	I_File* 			CreateFile( I_Location_Ptr	inLoc );

FBL_SHARED_EXP	I_Location_Ptr		CreateFolder( 
										I_Location_Ptr	inParentLoc,  
										const char*		inFolderName );
										
FBL_SHARED_EXP	I_Location_Ptr		CreateFolder( 
										I_Location_Ptr	inParentLoc,   
										const UChar*	inFolderName );

FBL_SHARED_EXP	I_Location_Ptr		PrepareFolder(
										I_Location_Ptr	inParentLoc,  
										const char*		inFolderName );
										
FBL_SHARED_EXP	I_Location_Ptr		PrepareFolder( 
										I_Location_Ptr	inParentLoc,   
										const UChar*	inFolderName );

/**********************************************************************************************/
// Create Folder if not exists.
SMART_INTERFACE(I_Disk_Location);
FBL_SHARED_EXP	I_Disk_Location_Ptr PrepareAppRelativeFolderLocation(
										I_Disk_Location_Ptr inpFolderLocation );


/**********************************************************************************************/
// interface class
//
interface FBL_SHARED_EXP_CLASS I_File : public I_Storage
{
virtual						~I_File( void );

	// ---------------------
	// Properties:

		// <Location>
virtual I_Location_Ptr		get_Location( void ) const  = 0;	
virtual void				put_Location( I_Location_Ptr inLocation )	= 0;

		// <Length>			
							// Current length of the file.
virtual flength				get_Length( void ) const					= 0;				
virtual void				put_Length( 
								flength 	inValue, 
								bool 		inSaveDisk = true )			= 0;
	
		// <Pos>			
							// Current position in the file.
virtual flength				get_Pos( void ) const						= 0;
virtual void				put_Pos( flength pos )						= 0;

		// <EOF> [r/o] 			
							// Returns TRUE if we are at EOF.
virtual	bool				get_EOF( void ) const						= 0;

		// <IsEmpty> [r/o]
							/** Returns TRUE if the file is empty.
								This not means it must be of zero length. 
								For example file of Table can have header, but no records. */ 
virtual bool				get_IsEmpty( void ) const					= 0;


	// ---------------------
	// Read/Write
							
							// Reads bytes to the memory buffer at the current position.
virtual vuint32				Read( 
								char* 		inBuffer, 
								vuint32 	inHowMuch )					= 0;

							// Reads bytes to the memory buffer at the specified position.
virtual vuint32				Read( 
								char* 		inBuffer,
								flength		inFrom, 
								vuint32 	inHowMuch )					= 0;

							// Writes bytes from the memory buffer at the specified position.
virtual	vuint32				Write( 
								const char* inBuffer, 
								flength		inFrom, 
								vuint32 	inHowMuch ) 				= 0;

							// Writes bytes from the memory buffer at the current position.
virtual	vuint32				Write( 
								const char* inBuffer, 
								vuint32 	inHowMuch ) 				= 0;

virtual	void				EnsureLength(
								flength 	inValue, 
								bool 		inSaveDisk = true )			= 0;
								
};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_I_File_h
