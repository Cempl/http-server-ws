/**********************************************************************************************/
/* FBL_OsWin.h 		                                                      					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_OsWin_h 
	#define _FBL_OsWin_h
#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN		/* Exclude rarely-used stuff from Windows headers */
#endif // WIN32_LEAN_AND_MEAN

#ifndef NOMINMAX
#define NOMINMAX
#endif // NOMINMAX

#include <windows.h>

//#include <winsock2.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// If your platform does not have the <inttypes.h> header, you may
// need to edit the typedefs below.

typedef __int8				vint8;
typedef unsigned __int8     vuint8;

typedef __int16				vint16;
typedef unsigned __int16    vuint16;

typedef __int32				vint32;
typedef unsigned __int32	vuint32;

typedef __int64				vint64;
typedef unsigned __int64	vuint64;


/**********************************************************************************************/
// GLOBAL FUNCTIONS:
//
inline vuint32 TickCount( void )
{
	return ::GetTickCount() / 16;
}


/**********************************************************************************************/
typedef vuint8* 		StringPtr;
typedef const vuint8* 	ConstStringPtr;
typedef vuint8 			Str255[256];
typedef vuint32 		OSType;

// Common system-independent and compiler-independent macro FBL_CHECK(x) 
#ifndef __CHECK
	#define __CHECK 0
#endif

#if __CHECK == 0
	#define CHECK(p) (void(0))
	#define CHECK_EX(p,cond) (void(0))
#else
        #define CHECK(p)  { \
            ++gTotalAssertCount;\
                if(!(p)) LogAssert(__FILE__,__FUNCTION__,__LINE__,#p); }	

		#define CHECK_EX(p,cond)  { ++gTotalAssertCount;\
							if(!(p)){\
								if((cond)){\
									LogAssert(__FILE__,__FUNCTION__,__LINE__,#p); }\
								else {\
									++gSkippedAssertFailCount;\
									}}}
#endif


/**********************************************************************************************/
// Windows do not have strcasecmp() func, instead exists strcmpi()
// So we specify own symbol fbl_strcasecmp, for each OS.
//
#define fbl_strcasecmp strcmpi


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // _FBL_OsWin_h
