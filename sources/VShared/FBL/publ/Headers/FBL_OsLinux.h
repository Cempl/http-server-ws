/**********************************************************************************************/
/* FBL_OsLinux.h	 		                                               					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef __FBL_OsLinux_h
#define __FBL_OsLinux_h

#ifdef FBL_UNIX

#include <sys/times.h>
#include <stdint.h>


/**********************************************************************************************/
// Used for work with big files
//#define _GNU_SOURCE
//#define _LARGEFILE_SOURCE
//#define _LARGEFILE64_SOURCE
//#define _FILE_OFFSET_BITS 64


/**********************************************************************************************/
// #define FBL_POSIX_API 1
// They need for build into the ASP linux
#ifndef ULLONG_MAX
    #define ULLONG_MAX     18446744073709551615ULL
#endif // ULLONG_MAX

#ifndef LLONG_MAX
    #define LLONG_MAX      9223372036854775807LL
#endif // LLONG_MAX

#ifndef NULL
#define NULL 0
#endif // NULL


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
extern "C"{


typedef vuint32       		OSType;
typedef vuint8 *     		StringPtr;


// GCC do not have stricmp, but it have strcasecmp
#define stricmp strcasecmp

}


/**********************************************************************************************/
//typedef vuint32  DWORD;
//typedef vuint16 uint16_t;
typedef void *PicHandle;
typedef void *UnixHandle;


//**********************************************************************************************/
//									GLOBAL FUNCTIONS:
//**********************************************************************************************
inline vuint32 TickCount( void )
{
	tms tm;
	return times(&tm);
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
#endif // FBL_UNIX
#endif // __FBL_OsLinux_h
