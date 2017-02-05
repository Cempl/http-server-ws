/**********************************************************************************************/
/* FBL_OsMac.h 		                                                      					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_OsMac_h 
	#define _FBL_OsMac_h
#pragma once
	
#include <MacMemory.h>
#include <stdint.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
typedef int8_t				vint8;
typedef uint8_t             vuint8;
	
typedef int16_t				vint16;
typedef uint16_t			vuint16;
	
typedef int32_t				vint32;
typedef uint32_t			vuint32;
	
typedef int64_t				vint64;
typedef uint64_t			vuint64;


/**********************************************************************************************/
//									GLOBAL FUNCTIONS:
/**********************************************************************************************/
inline vuint32 TickCount( void )
{
	return ::TickCount();
}

 
/**********************************************************************************************/
// This is basic macro for assert in kernel:
//
#ifndef __CHECK
	#define __CHECK 0
#endif

	// Common system-independent and compiler-independent macro FBL_CHECK(x) 
#if __CHECK == 0
	#define CHECK(p) (void(0))
	#define CHECK_EX(p,cond) (void(0))
#else
	#define CHECK(p)  { ++gTotalAssertCount;\
						if(!(p)) LogAssert(__FILE__,__func__,__LINE__,#p); }
	#define CHECK_EX(p,cond)  { ++gTotalAssertCount;\
						if(!(p)){\
							if((cond)){\
								LogAssert(__FILE__,__func__,__LINE__,#p); }\
							else {\
								++gSkippedAssertFailCount;\
								}}}						
#endif


/**********************************************************************************************/
// Windows do not have strcasecmp() func, instead exists strcmpi()
// So we specify own symbol fbl_strcasecmp, for each OS.
//
#define fbl_strcasecmp strcasecmp


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // _FBL_OsMac_h 
