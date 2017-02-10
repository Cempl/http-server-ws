/**********************************************************************************************/
/* FBL_File_OS.h																		 	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                       					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/
// Master file to include when using platform-independent files.

#ifndef _FBL_File_OS_h 
	#define _FBL_File_OS_h
#pragma once


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL.h>


/**********************************************************************************************/
#if FBL_WIN
	#include <VShared/FBL/publ/File_OS/win/FBL_File_Win.h>
#elif FBL_MAC
	#include <VShared/FBL/publ/File_OS/mac/FBL_File_Mac.h>
#elif FBL_UNIX
	#include <VShared/FBL/publ/File_OS/posix/FBL_File_Posix.h>
#endif //FBL_WIN


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
#if FBL_WIN
	typedef FileWin		File_OS;
#elif FBL_MAC
	typedef File_Mac	File_OS;
#elif FBL_UNIX
	typedef File_Posix	File_OS;
#endif //FBL_WIN


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // _FBL_File_OS_h
