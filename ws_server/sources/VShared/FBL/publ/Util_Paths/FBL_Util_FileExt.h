/**********************************************************************************************/
/* FBL_Util_FileExt.h																		  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_Util_FileExt_h
	#define _FBL_Util_FileExt_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
class Location;


/**********************************************************************************************/
// Database FILE extensions and types:

FBL_SHARED_EXP void SetDefaultExtentions( void );

FBL_SHARED_EXP void SetExtensions( 
						const UChar* inVDB, 
						const UChar* inDAT, 
						const UChar* inBLB, 
						const UChar* inIND );
						
FBL_SHARED_EXP void SetExtensions( 
						const char*  inVDB, 
						const char*  inDAT, 
						const char*  inBLB, 
						const char*  inIND );


//----------------------------------------------------------------------------------------------
#if FBL_MAC

	// Global constants which you can change to have own Creator and File types.
	FBL_SHARED_EXP extern const OSType gSignature;			// on default 'VALA'
	FBL_SHARED_EXP extern const OSType gDescription;		// on default 'Vdsc'
	FBL_SHARED_EXP extern const OSType gMainDataFile;		// on default 'Vdat'
	FBL_SHARED_EXP extern const OSType gBlobsDataFile;		// on default 'Vblb'
	FBL_SHARED_EXP extern const OSType gIndexDataFile;		// on default 'Vind'
	FBL_SHARED_EXP extern const OSType gTmpDataFile;		// on default 'Vtmp'
	
	FBL_SHARED_EXP void GetCreator( char*& outStr );
	FBL_SHARED_EXP void SetCreator( const char* inStr );
	
#endif // FBL_MAC


/**********************************************************************************************/
// Returns true if current OS requires an extension for files.
// Windows, MacOS X, Linux require it,
// MacOS Classis and Carbon don't.
// 
FBL_SHARED_EXP bool ExtRequired( void );


/**********************************************************************************************/
// Generate C-string names with correct extensions for 4 files of Valentina database.
// 
FBL_SHARED_EXP void MakeDbFileNames(
	const String&	inSpecName,
	String*			outVdbName,
	String*			outDatName,
	String*			outBlbName,
	String*			outIndName,
	String*			outTmpName,
	String*			outLockName = nullptr );


/**********************************************************************************************/
// Returns true if file has an extension
// 
FBL_SHARED_EXP bool HasExtension( const String&	inFileName );



/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_Util_FileExt_h


