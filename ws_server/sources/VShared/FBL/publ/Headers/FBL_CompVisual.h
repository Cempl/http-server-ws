/**********************************************************************************************/
/* FBL_CompVisual.h                                                      					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/
 
#ifndef __FBL_CompVisual_h 
	#define __FBL_CompVisual_h
#pragma once


/**********************************************************************************************/
#define __va_start(parm) (char*) ((vint32) (&parm) + ((((vint32) (&parm + 1) - (vint32) (&parm))+3)/4*4))  /*- mm 9708027 -*/


/**********************************************************************************************/
// FBL_STATIC 	  = 1 	if you want to compile static library of vkernel.
// FBL_DLL_EXPORT = 1 	if you want to compile dynamic library of vkernel.
// FBL_DLL_IMPORT = 1 	if you want to compile some project that links to VKERNEL.DLL
//
// Logic is next:
//
// * If you make STATIC library, then there is no exported symbols, so all macroses are empty.
// * If you build VKERNEL.DLL then FBL_SHARED_EXP macro works on Export. Otherwise it works on Import.
// * If you build VSHARED.DLL then FBL_SHARED_EXP macro works on export and FBL_SHARED_EXP is empty,
//		because vshared.dll do not depend on vkernel.dll
//
// See the same notes in all Compiller-specific FBL headers.
//
#if FBL_STATIC

	#define FBL_EXP
	#define FBL_EXP_CLASS

	#define FBL_SHARED_EXP
	#define FBL_SHARED_EXP_CLASS
	
#else // FBL_STATIC

	// ------------------------------------
	// for VSHARED.dll build:
	#if FBL_SHARED_EXPORT
		#define FBL_EXP
		#define FBL_EXP_CLASS

		#define FBL_SHARED_EXP __declspec(dllexport) 
		#define FBL_SHARED_EXP_CLASS __declspec(dllexport) 
	#else
		#define FBL_SHARED_EXP __declspec(dllimport) 
		#define FBL_SHARED_EXP_CLASS __declspec(dllimport) 
	#endif // FBL_SHARED_EXPORT
	
	#if FBL_DLL_EXPORT
		#define FBL_EXP __declspec(dllexport) 
		#define FBL_EXP_CLASS __declspec(dllexport) 
	#endif // FBL_DLL_EXPORT	
	
	#if FBL_DLL_IMPORT
		#define FBL_EXP __declspec(dllimport)
		#define FBL_EXP_CLASS __declspec(dllimport)
	#endif // FBL_DLL_IMPORT


#endif // FBL_STATIC



/**********************************************************************************************/
// EXPAT: force it to use Unicode, UTF16, 2 bytes.
//
#define XML_UNICODE_WCHAR_T	1


/**********************************************************************************************/
#endif // __FBL_CompVisual_h
