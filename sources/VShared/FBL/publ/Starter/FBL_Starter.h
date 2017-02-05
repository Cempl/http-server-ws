/**********************************************************************************************/
/* FBL_Starter.h		                                                   					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_Starter_h
	#define _FBL_Starter_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// inUseClientStrings should be TRUE if call is from VClient.dll
//
FBL_SHARED_EXP void Init_ValentinaDlls( bool inUseClientStrings = false );
FBL_SHARED_EXP bool LoadVProjectDll( void );


/**********************************************************************************************/
#if FBL_MAC

							// Returns App.app bundle itself
			   CFURLRef		FindAppBundleCopy( void );

							// Returns folder that contains App.app
FBL_SHARED_EXP CFURLRef		FindAppFolderCopy( void );

FBL_SHARED_EXP CFURLRef		FindVcomponents( void );


#if FBL_SUPPORT_FSSPEC

FBL_SHARED_EXP OSErr 		MakeParentSpec(
								const FSSpec* 	inSpec,
								FSSpec* 		outParentSpec );

FBL_SHARED_EXP OSErr 		MakeChildSpec(
								const FSSpec* 	inSpec,
								const char* 	inItemName,
								FSSpec* 		outChildSpec );

#endif // FBL_SUPPORT_FSSPEC

#endif // FBL_MAC


/**********************************************************************************************/
#if FBL_WIN

FBL_SHARED_EXP	void		FindComponentsFolder(
								char* outPath );

#endif // FBL_WIN


/**********************************************************************************************/
#if FBL_UNIX

FBL_SHARED_EXP	bool		CheckComponentsFolder( const char* inPath );

FBL_SHARED_EXP 	void*  		FindSymbolInVProjectDLL( const char* inSymbolName );

#endif // FBL_UNIX


/**********************************************************************************************/
const char* GetKernelDllName( void );
const char* GetClientDllName( void );
const char* GetVReportDllName( void );


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // _FBL_Starter_h
