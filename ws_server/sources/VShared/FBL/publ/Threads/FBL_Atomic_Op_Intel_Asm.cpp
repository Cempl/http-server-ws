/**********************************************************************************************/
/* FBL_Atomic_Op_Intel_Asm.cpp	                                       						  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include "FBL_Atomic_Op_Intel_Asm.h"

/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
#if FBL_INTEL_CPU


/**********************************************************************************************/
long threadsafe_increment( volatile long* value )
{
	long tmp = 1;
	
    unsigned long addr = reinterpret_cast<unsigned long>( value );
	
    asm( "lock ; xadd %0, (%1)" : "+r"(tmp) : "r"(addr) );
	
    return tmp + 1;
}


/**********************************************************************************************/
long threadsafe_decrement( volatile long *value )
{
	long tmp = -1;
	
    unsigned long addr = reinterpret_cast<unsigned long>( value );
	
    asm( "lock ; xadd %0, (%1)" : "+r"(tmp) : "r"(addr) );
	
    return tmp - 1;
}


/**********************************************************************************************/
long threadsafe_exchange( volatile long *value, long rhs )
{
	unsigned long addr = reinterpret_cast<unsigned long>( value );
	
    // The XCHG instruction automatically follows LOCK semantics
	asm( "xchg %0, (%1)" : "+r"(rhs) : "r"(addr) );
	
    return rhs;
}


/**********************************************************************************************/
long threadsafe_exchange_add( volatile long *value, long rhs )
{
	unsigned long addr = reinterpret_cast<unsigned long>( value );
	
    asm( "lock ; xadd %0, (%1)" : "+r"(rhs) : "r"(addr) );
    
	return rhs;
}


/**********************************************************************************************/
#endif // FBL_INTEL_CPU


/**********************************************************************************************/
FBL_End_Namespace




