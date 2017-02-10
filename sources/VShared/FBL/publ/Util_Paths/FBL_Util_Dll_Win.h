/**********************************************************************************************/
/* FBL_Util_Dll_Win.h																		  */
/*																							  */
/* DLL utility functions																	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_Util_Dll_Win_h
	#define _FBL_Util_Dll_Win_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>

#if FBL_WIN

#pragma warning(push)
#pragma warning(disable: 4005)
#include <commctrl.h>
#include <shlwapi.h>
#pragma warning(pop)

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
/** Returns DLL version info by its handle. */
FBL_SHARED_EXP BOOL GetDllVersion( 
	HINSTANCE hInstDLL, 
	DLLVERSIONINFO* pDllVersionInfo );


/**********************************************************************************************/
/** Returns DLL version info by path to the DLL. */
FBL_SHARED_EXP BOOL GetDllVersion(
	LPCTSTR lpDLLPath, 
	DLLVERSIONINFO* lpVI );


/**********************************************************************************************/
/** Returns Common Control DLL version. */
FBL_SHARED_EXP BOOL GetCommCtrlVersion(
	LPDWORD pdwMajor, 
	LPDWORD pdwMinor);


/**********************************************************************************************/
/** Returns packed Common Control DLL version. */
FBL_SHARED_EXP LONG GetCommCtrlVersion ();


/**********************************************************************************************/
/** Returns version of Windows Shell DLL. */
FBL_SHARED_EXP BOOL GetShellVersion (LPDWORD pdwMajor, LPDWORD pdwMinor);


/**********************************************************************************************/
/** Returns packed version of Windows Shell DLL. */
FBL_SHARED_EXP LONG GetShellVersion ();


/**********************************************************************************************/
/** Returns true if running Windows version from Windows NT family.
	This can be used, for example, to choose between ANSI and Unicode 
	functions versions.	*/
FBL_SHARED_EXP bool IsWindowsNTFamilyOS( void );



/**********************************************************************************************/
FBL_End_Namespace


#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // FBL_WIN

#endif //_FBL_Util_Dll_Win_h
