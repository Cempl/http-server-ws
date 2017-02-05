/**********************************************************************************************/
/* FBL_memory.cpp                                                          					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Utilities/FBL_memory.h>


#if FBL_UNIX
	#include <unistd.h>
#endif // FBL_UNIX

#if FBL_WIN
	#include <windows.h>
#endif // FBL_UNIX

#if FBL_MAC
#include <sys/sysctl.h>
#endif // FBL_MAC


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
UChar* vu_memcpy( UChar* inDest, const UChar* inSrc, tslen inSize )
{
	return (UChar*) memcpy( inDest, inSrc, inSize * sizeof(UChar) );
}


/**********************************************************************************************/
UChar* vu_memset( UChar* inDest, UChar c, tslen inCount )
{
	UChar* p = inDest;
	
	while( inCount-- )
		*p++ = c;
		
	return inDest;
}


#pragma mark -


/**********************************************************************************************/
#if FBL_UNIX

vuint64 getTotalSystemMemory( void )
{
    long pages = sysconf(_SC_PHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);
    
    vuint64 res = vuint64(pages) * page_size;
    
    return res;
}


/**********************************************************************************************/
vuint64 getMemoryUsedByThisApp( void )
{
	return 0;
}


#endif // FBL_UNIX


/**********************************************************************************************/
#if FBL_MAC

vuint64 getTotalSystemMemory()
{
    int mib[2] = { CTL_HW, HW_MEMSIZE };
    u_int namelen = sizeof(mib) / sizeof(mib[0]);

    uint64_t size;
    size_t len = sizeof(size);

	int status = sysctl(mib, namelen, &size, &len, NULL, 0);

	return (status >= 0) ? size : 0;
}


/**********************************************************************************************/
vuint64 getMemoryUsedByThisApp( void )
{
	return 0;
}


#endif // FBL_MAC


/**********************************************************************************************/
#if FBL_WIN

vuint64 getTotalSystemMemory( void )
{
	MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    return status.ullTotalPhys;
}


/**********************************************************************************************/
vuint64 getMemoryUsedByThisApp( void )
{
	return 0;
}


#endif // FBL_WIN


/**********************************************************************************************/
FBL_End_Namespace
