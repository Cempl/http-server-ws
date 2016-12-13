/**********************************************************************************************/
/* FBL_Atomic_Op_Intel_Asm.h                                           						  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/
#ifndef _FBL_Atomic_Op_Intel_Asm_h
	#define _FBL_Atomic_Op_Intel_Asm_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
#if FBL_INTEL_CPU

/**********************************************************************************************/
// Helper functions
/**********************************************************************************************/
FBL_SHARED_EXP long threadsafe_increment( volatile long* value );
FBL_SHARED_EXP long threadsafe_decrement( volatile long *value );
FBL_SHARED_EXP long threadsafe_exchange( volatile long *value, long rhs );
FBL_SHARED_EXP long threadsafe_exchange_add( volatile long *value, long rhs );


/**********************************************************************************************/
#endif // FBL_INTEL_CPU


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // _FBL_Atomic_Op_Posix_h
