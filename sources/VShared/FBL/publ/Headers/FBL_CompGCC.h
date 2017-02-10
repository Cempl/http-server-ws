/**********************************************************************************************/
/* FBL_CompGCC.h 		                                                   					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef __FBL_CompGCC_h
	#define __FBL_CompGCC_h
#pragma once


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

#else // FBL_STATIC

	// ------------------------------------
	// for VSHARED.dll build:
	#if FBL_SHARED_EXPORT
		#define FBL_SHARED_EXP __attribute__((visibility("default")))

		#if __GNUC__ >= 4
			#define FBL_SHARED_EXP_CLASS __attribute__((visibility("default")))
		#else
			#define FBL_SHARED_EXP_CLASS
		#endif //  __GNUC__ >= 4
	#else
		#define FBL_SHARED_EXP
		#define FBL_SHARED_EXP_CLASS
	#endif // FBL_SHARED_EXPORT

	// ------------------------------------
	// for KERNEL.DLL build:
	#if FBL_DLL_EXPORT
		#define FBL_EXP __attribute__((visibility("default")))

		#if __GNUC__ >= 4
			#define FBL_EXP_CLASS __attribute__((visibility("default")))
		#else
			#define FBL_EXP_CLASS
		#endif //  __GNUC__ >= 4
	#else
		#define FBL_EXP
		#define FBL_EXP_CLASS
	#endif // FBL_DLL_EXPORT

	// ------------------------------------
	// for VSHARED + VKERNEL dlls usage:
	#if FBL_DLL_IMPORT
		#ifdef FBL_EXP
			#undef FBL_EXP // KP Fixed GCC warning
		#endif // FBL_EXP

		#define FBL_EXP __attribute__ ((weak))
		#define FBL_EXP_CLASS
	#endif // FBL_DLL_IMPORT

#endif // FBL_STATIC


/**********************************************************************************************/
// EXPAT: force it to use Unicode, UTF16, 2 bytes.
//
#define HAVE_MEMMOVE 1
//
#define XML_UNICODE 1
#undef XML_UNICODE_WCHAR_T	// for macho EXPAT must not use whcar_t (4), but ushort (2 bytes)
//
#define KXML_UNICODE 1
#undef KXML_UNICODE_WCHAR_T	// for macho EXPAT must not use whcar_t (4), but ushort (2 bytes)


/**********************************************************************************************/
#endif // __FBL_CompGCC_h
