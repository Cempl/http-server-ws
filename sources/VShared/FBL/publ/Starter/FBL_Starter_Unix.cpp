/**********************************************************************************************/
/* FBL_Starter_Unix.cpp                                                						  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Starter/FBL_Starter.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_Disk_Location.h>
#include <VShared/FBL/publ/File_OS/FBL_DynamicLibrary.h>
#include <VShared/FBL/publ/Util_classes/FBL_String.h>


/**********************************************************************************************/
#if FBL_UNIX


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
UnixHandle gVReportDllHandle;

/**********************************************************************************************/
typedef void (*InitBackpointers_PTR)( void );

/**********************************************************************************************/
extern "C" void InitBackpointers( void );


/**********************************************************************************************/
void Init_ValentinaDlls( bool inUseClientStrings )
{
#if FBL_STATIC

	argused1( inUseClientStrings );
	InitBackpointers();

#else // FBL_STATIC

	// Kernel-dll string-factory has priority.
	if( String::sFactory && inUseClientStrings )
		return;

	// At first search for already loaded symbols
	// to avoid conflicts (different betas of vshared/vclient/vkernel) on dlopen
	InitBackpointers_PTR pfn_app_client = nullptr;
	InitBackpointers_PTR pfn_app_kernel = nullptr;
	
	UnixHandle hDLL = ::dlopen( nullptr, RTLD_NOW | RTLD_GLOBAL );
	if( hDLL )
	{
		pfn_app_kernel = (InitBackpointers_PTR) ::dlsym( hDLL, "InitBackpointersKernel" );
		if( !pfn_app_kernel )
			pfn_app_client = (InitBackpointers_PTR) ::dlsym( hDLL, "InitBackpointersClient" );
	}
	
	InitBackpointers_PTR pfn = pfn_app_kernel;
	if( !pfn && inUseClientStrings )
		pfn = pfn_app_client;

	// Need to dynamically load libraries
	if( !pfn )
	{
		// VKERNEL
		if( !inUseClientStrings )
		{
			hDLL = ::dlopen( GetKernelDllName(), RTLD_NOW | RTLD_LOCAL );
			if( hDLL )
			{
				pfn = (InitBackpointers_PTR) ::dlsym( hDLL, "InitBackpointersKernel" );
				if( !pfn )
					pfn = (InitBackpointers_PTR) ::dlsym( hDLL, "InitBackpointers" );
			}
		}
		
		// VCLIENT
		if( !pfn )
		{
			hDLL = ::dlopen( GetClientDllName(), RTLD_NOW | RTLD_LOCAL );
			if( hDLL )
			{
				pfn = (InitBackpointers_PTR) ::dlsym( hDLL, "InitBackpointersClient" );
				if( !pfn )
					pfn = (InitBackpointers_PTR) ::dlsym( hDLL, "InitBackpointers" );
			}
		}
	}

	if( pfn )
		pfn();

#endif // FBL_STATIC
}


/**********************************************************************************************/
bool LoadVProjectDll( void )
{
	gVReportDllHandle = ::dlopen(GetVReportDllName(), RTLD_NOW | RTLD_LOCAL );

	return (gVReportDllHandle) ? true : false;
}


/**********************************************************************************************/
void*  FindSymbolInVProjectDLL( const char* inSymbolName )
{
	 void* pointer = nullptr;

	 // reset errors
	 dlerror();

	 if( !gVReportDllHandle )
	 {
		 printf( "Critical Error: VProject DLL was not loaded!\n" );
		 exit(1);
	 }

	 pointer = ::dlsym( gVReportDllHandle, inSymbolName );

	 const char* dlsym_error = ::dlerror();
	 if (dlsym_error)
	 {
		 printf( "Critical Error: Cannot load symbol create: %s\n", dlsym_error );
		 exit(1);
	 }

	 return pointer;
}


/**********************************************************************************************/
FBL_End_Namespace

#endif // FBL_UNIX
