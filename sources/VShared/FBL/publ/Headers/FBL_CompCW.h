/**********************************************************************************************/
/* FBL_CompCW.h 	                                                      					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_CompCW_h 
	#define _FBL_CompCW_h
#pragma once


/**********************************************************************************************/
#pragma warn_ptr_int_conv on


/**********************************************************************************************/
// FBL_STATIC 	  = 1 	if you want compile static library of kernel.
// FBL_DLL_EXPORT = 1 	if you want compile dynamic library of kernel.
// FBL_DLL_IMPORT = 1 	if you want compile some project that link to kernel DLL.
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
	
#else // STATIC

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

	// ------------------------------------
	// for KERNEL.DLL build:
	#if FBL_DLL_EXPORT
		#define FBL_EXP __declspec(dllexport) 
		#define FBL_EXP_CLASS __declspec(dllexport) 
	#endif // FBL_DLL_EXPORT	
	
	// ------------------------------------
	// for VSHARED + VKERNEL dlls usage:
	#if FBL_DLL_IMPORT
		#define FBL_EXP __declspec(dllimport)
		#define FBL_EXP_CLASS __declspec(dllimport)
	#endif // FBL_DLL_IMPORT
	
	
#endif // FBL_STATIC


/**********************************************************************************************/
// EXPAT: force it to use Unicode, UTF16, 2 bytes.
//
#if __MACH__
	#define XML_UNICODE 1
	#undef XML_UNICODE_WCHAR_T	// for macho EXPAT must not use whcar_t, because it is 4 bytes
#else
	#define XML_UNICODE_WCHAR_T	1	// for carbon EXPAT should use wchar_t
#endif // __MACHO__



/**********************************************************************************************/
#endif // _FBL_CompCW_h
