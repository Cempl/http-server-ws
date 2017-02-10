/**********************************************************************************************/
/* FBL_Util_Path.h																			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_Util_Path_h
	#define _FBL_Util_Path_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace

/**********************************************************************************************/
// Extract from a full path name of file. For each platform this function uses its delimiter.
//
FBL_SHARED_EXP const UChar*	ExtractNameFromFullPath( const UChar* inFullPath, UChar inDelim = 0 );


/**********************************************************************************************/
#if FBL_MAC

#if FBL_SUPPORT_FSSPEC

FBL_SHARED_EXP	void 		FindAppFolder( 
								FSSpec* 		outSpec );

FBL_SHARED_EXP	void 		FindVcomponents( 
								FSSpec* 		outCompSpec );


FBL_SHARED_EXP	OSErr 		Path2Spec_Carbon( 
								const char*		inFullPath, 
								FSSpec*			inSpec );

FBL_SHARED_EXP	OSErr 		Path2Spec_FSRef( 
								const char*		inFullPath, 
								FSSpec*			inSpec );


FBL_SHARED_EXP	OSErr 		FSMakePath_Carbon(
								SInt16 			volRefNum,
								SInt32 			dirID,
								ConstStr255Param name,
								char*			outPath,
								vuint32 		inmaxPathSize );


	// ---------------------
	// "More Files" code:

FBL_SHARED_EXP	OSErr 		GetCatInfoNoName( 
								vint16			vRefNum, 
								vint32			dirID,  
								ConstStr255Param name, 
								CInfoPBPtr		pb);

FBL_SHARED_EXP	OSErr 		GetDirectoryID(
								vint16			vRefNum,
								vint32			dirID,
								ConstStr255Param name,
								vint32*			theDirID,
								bool*			isDirectory);

FBL_SHARED_EXP	OSErr 		FSpGetDirectoryID(
								const FSSpec*	inSpec,
								vint32*			theDirID,
								bool*			isDirectory);

FBL_SHARED_EXP	OSErr 		FSpGetFullPath(
								const FSSpec*	spec,
								vint16*			fullPathLength,
								Handle*			fullPath );

#endif // FBL_SUPPORT_FSSPEC


#if FBL_SUPPORT_FSREF

FBL_SHARED_EXP OSErr 		FSCheckLock(
								const FSRef*	ref );

FBL_SHARED_EXP	OSErr 		FSMakePath_FSRef(
								SInt16 				volRefNum,
								SInt32 				dirID,
								ConstStr255Param 	name,
								char*				outPath,
								vuint32 			inmaxPathSize );
								
FBL_SHARED_EXP	OSErr 		FSMakeFSRef(
								FSVolumeRefNum		volRefNum,
								SInt32				dirID,
								ConstStr255Param	name,
								FSRef*				ref );

#endif // FBL_SUPPORT_FSREF

#endif // FBL_MAC


/**********************************************************************************************/
#if FBL_MAC_MACHO																// MACHO

#if FBL_64
	#define VCOMPONENTS "vcomponents_x64"
#else
	#define VCOMPONENTS "vcomponents"
#endif // FBL_64

#endif // FBL_MAC_MACHO


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_Util_Path_h


