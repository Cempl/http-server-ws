/**********************************************************************************************/
/* FBL_Util_PathA.h																			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_Util_PathA_h
	#define _FBL_Util_PathA_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// Extracts file name from its full path. For each platform this function uses its delimiter.
//
FBL_SHARED_EXP const char* ExtractNameFromFullPath( const char* inFullPath, char inDelim = 0 );


/**********************************************************************************************/
// Extracts directory by the absolute path to file.
// @param outBase - buffer, where to copy directory path.
//
FBL_SHARED_EXP void ExtractDirectoryFromPath( const char inPath[], char* outDir );


/**********************************************************************************************/
// Returns pointer to the place in string where starts extension of file.
// If string does not contain file name with extension then returns NULL.
//
FBL_SHARED_EXP const char* ExtractExtFromFileName( const char* inNameWithExt );


/**********************************************************************************************/
// Extracts base portion of file name from string.
// Returns pointer to the base name.
// @param outBase - buffer, where to copy base name.
//
FBL_SHARED_EXP char* ExtractBaseFromFileName( const char* inFileName, char* outBase );


/**********************************************************************************************/
// Extracts base portion of file name from absolute path to file.
// Returns pointer to the base name.
// @param outBase - buffer, where to copy base name.
//
FBL_SHARED_EXP void ExtractBaseNameFromFullPath( const char inPath[], char* outBase );


/**********************************************************************************************/
// Extracts host and port from inpSrc.
// @param outpHost - buffer, where to copy host.
// @param outPort - buffer, where to copy port.
//
FBL_SHARED_EXP void SplitHostPort( char* inpSrc, MemPtr<char>* outpHost, vint32& outPort );


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_Util_PathA_h


