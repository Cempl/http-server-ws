/**********************************************************************************************/
/* FBL.h	 		                                                      					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/
// The TOP HEADER of Valentina engine, which includes into self
// 	 Macros.h > OS.h > Types.h > Constants > Errors.h > CompilerXXX.h > FBL_new.h > Helpers
//
#ifndef _FBL_h
	#define _FBL_h
#pragma once


/**********************************************************************************************/
#if defined(_WIN64)
#include <stdio.h>
#endif // _WIN64


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_Macros.h>


/**********************************************************************************************/
#if FBL_MAC
	#if FBL_MAC_MACHO
		#include <VShared/FBL/publ/Headers/FBL_OsMacMacho.h>
	#else
		#include <VShared/FBL/publ/Headers/FBL_OsMac.h>
	#endif
#endif // FBL_MAC

#if FBL_WIN
	#include <VShared/FBL/publ/Headers/FBL_OsWin.h>
#endif // FBL_WIN

#if FBL_UNIX
    #include <stdio.h>
	#include <VShared/FBL/publ/Headers/FBL_OsLinux.h>
#endif // FBL_UNIX


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_Types.h>
#include <VShared/FBL/publ/Headers/FBL_Types_Unicode.h>
//
#include <VShared/FBL/publ/Headers/FBL_Constants.h>	// constants go after types, to use them.
//
#include <VShared/FBL/publ/Errors/FBL_Errors.h>		// just vuint32 constants actually.


/**********************************************************************************************/
// If not static defined and not DLL defined and we build not shared dll, then we set DLL mode.
// This is to simplify setup of projects for VSDK users.
// This section should go before #include <CompillerXXXX.h> section.
//
#if !defined(FBL_STATIC) && !defined(FBL_DLL_IMPORT) &&	!defined(FBL_SHARED_EXPORT) && !defined(FBL_DLL_EXPORT)

#define FBL_DLL_IMPORT 1

#endif // not static and not dll defined


/**********************************************************************************************/
// Compiler specific macros:
//
#ifdef __GNUC__
	#include <VShared/FBL/publ/Headers/FBL_CompGCC.h>
#endif // __GNUC__

#ifdef _MSC_VER
	#include <VShared/FBL/publ/Headers/FBL_CompVisual.h>
#endif // _MSC_VER


/**********************************************************************************************/
#include <VShared/FBL/publ/Templates/FBL_StAllocator.h>


/**********************************************************************************************/
// DEBUG Macros and tools for Valentina Tests. Used only in DEBUG builds of course.
//
#include <VShared/FBL/publ/Headers/FBL_Debug_Macros.h>
#include <VShared/FBL/publ/TestTools/FBL_TestSystem_Globals.h>


/**********************************************************************************************/
// We have own implementation of intrusive smart_ptrs inspired by BOOST.
//
#include <VShared/FBL/publ/Templates/FBL_Smart_Ptr.h>



/**********************************************************************************************/
#include <VShared/FBL/publ/Templates/FBL_StValueChanger.h>


/**********************************************************************************************/
// Some useful global functions, classes, templates that can be used in any file.
//
// independent:
//
#include <VShared/FBL/publ/Templates/FBL_MemPtr.h>
#include <VShared/FBL/publ/Headers/FBL_Globals.h>
#include <VShared/FBL/publ/Templates/FBL_Global_T.h>
//
// depends:
//
#include <VShared/FBL/publ/Interfaces/FBL_I_Unknown.h>
#include <VShared/FBL/publ/Templates/FBL_Array.h>
#include <VShared/FBL/publ/Errors/FBL_Exceptions.h>


/**********************************************************************************************/
#endif // _FBL_h


