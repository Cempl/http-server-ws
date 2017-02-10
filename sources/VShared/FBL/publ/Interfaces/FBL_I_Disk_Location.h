/**********************************************************************************************/
/* FBL_I_Disk_Location.h 	                                                      			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_Disk_Location_h 
	#define _FBL_I_Disk_Location_h
#pragma once


// FBL:
#include <VShared/FBL/publ/Interfaces/FBL_I_Location.h>
#include <VShared/FBL/publ/Util_classes/FBL_String.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE(I_Disk_Location);
class String;


/**********************************************************************************************/
// Factories of I_Disk_Location.
//

FBL_SHARED_EXP	I_Disk_Location*	CreateRawLocation(
										const UChar* inStr );

FBL_SHARED_EXP	I_Disk_Location*	CreateRawLocation(
										const char* inStr );


FBL_SHARED_EXP	I_Disk_Location*	CreateDiskLocation( 
										const UChar* inStr );

FBL_SHARED_EXP	I_Disk_Location*	CreateDiskLocation( 
										const char* inStr );
		
        
/**********************************************************************************************/
#if FBL_MAC 

//-----------------------
#if FBL_SUPPORT_FSSPEC

FBL_SHARED_EXP	I_Disk_Location*	CreateDiskLocationFromFSSpec( 
										const FSSpec* inSpec );

FBL_SHARED_EXP	I_Disk_Location*	CreateDiskLocationFromFSSpec( 
										const char* inStr );
#endif // FBL_SUPPORT_FSSPEC


//-----------------------
#if FBL_SUPPORT_FSREF
									// This form should be used for FSRef of existed object.
FBL_SHARED_EXP	I_Disk_Location*	CreateDiskLocationFromFSRef( 
										const UChar* 	inFullPath );

FBL_SHARED_EXP	I_Disk_Location*	CreateDiskLocationFromFSRef( 
										const char* 	inFullPath );

									// This form should be used for FSRef of existed object.
FBL_SHARED_EXP	I_Disk_Location*	CreateDiskLocationFromFSRef( 
										HFSUniStr255* 	inName,
										const FSRef* 	inExistedRef );

									// This form should be used for non-existed object.
									// You should specify FSRef of parent folder and name of object.
FBL_SHARED_EXP	I_Disk_Location*	CreateDiskLocationFromFSRef( 
										const FSRef* 	inParentRef,
										HFSUniStr255& 	inName);

FBL_SHARED_EXP I_Disk_Location* 	CreateDiskLocationFromFSRef( 
										CFURLRef 		inUrlRef );

#endif // FBL_SUPPORT_FSREF.
		
#endif // FBL_MAC


/**********************************************************************************************/
interface FBL_SHARED_EXP_CLASS I_Disk_Location : public I_Location
{
virtual						~I_Disk_Location( void );

							// Static public constant char that represents 
							// the OS-specific path delimiter. 
static const UChar			sPathDelimiterU;
static const char			sPathDelimiterA;

static I_Disk_Location_Ptr	GetAppLocation( void );		
static void					SetAppLocation( I_Disk_Location_Ptr inAppLocation );

 							// Builds and returns a location of plugin folder inside of APP folder.
 							// You need just provide the name of plugin folder.		
static 	I_Disk_Location_Ptr	sGetAppRelative( const String& inName );
static 	I_Disk_Location_Ptr	sGetAppRelative( const UChar* inName );

							// Returns a location of system temporary directory.
static 	I_Disk_Location_Ptr	GetTempDir( void );

							// Returns path to the uniquely named temporary file.
static 	I_Disk_Location_Ptr	GetTempFile( const char* inBaseName );


	public://///////////////////////////////////////////////////////////////////////////////////

	//------------------
	// Properties:

		// <Name> 
							/// The name of the file.
virtual	const String&		get_FileName( void ) const  = 0;	//OLDKEEPAS: mFileName

		// <IsDirectory> 
							/// Returns TRUE if location is Directory. 
virtual	bool				get_IsDirectory( void ) const  = 0;
virtual	void				put_IsDirectory( bool inValue )  = 0;
		
		// <Exists> 
							/// Returns TRUE if this location exists on disk.
virtual	bool				get_Exists( void ) const  = 0;


	public://///////////////////////////////////////////////////////////////////////////////////

// For MacOS FSSpec:

#if FBL_SUPPORT_FSSPEC

virtual	const FSSpec*		get_FSSpec( void ) const = 0;  
virtual	void				put_FSSpec( const FSSpec* inSpec ) = 0;

#endif // FBL_SUPPORT_FSSPEC



	protected:////////////////////////////////////////////////////////////////////////////////// 

static 	I_Disk_Location_Ptr		spAppLocation;

//static	UChar				sAppPath[256];
};


/**********************************************************************************************/
FBL_End_Namespace


#include <VShared/FBL/publ/Headers/FBL_post_header.h>


#endif // _FBL_I_Database_h
