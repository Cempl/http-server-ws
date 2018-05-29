/**********************************************************************************************/
/* FBL_Starter_Unix.cpp                                                						  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
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
extern "C" void InitBackpointers( void );


/**********************************************************************************************/
void Init_ValentinaDlls( bool inUseClientStrings )
{
#if FBL_STATIC

	argused1( inUseClientStrings );
	InitBackpointers();

#else // FBL_STATIC

	// KERNEL DLL string-factory has priority, i.e.
	// if sFactory is not NULL and call is from VCLIENT.DLL, we do not change pointers.
	// if sFactory is not NULL but call is from KERNEL.DLL, we will reset pointers to KERNEL strings. 
	// 
	if( String::sFactory && inUseClientStrings )
		return;


// --------------------
// This part is Linux-specific.
// We asking APP itself for 2 entry points, which for linux have different names.
// --------------------

	// Search for already loaded symbols to avoid conflicts
	// (different betas of vshared/vclient/vkernel) on dlopen
	typedef void (*InitBackpointers_PTR)( void );
    //
	InitBackpointers_PTR pfn_app_client = nullptr;
	InitBackpointers_PTR pfn_app_kernel = nullptr;
	
	UnixHandle hDLL = ::dlopen( nullptr, RTLD_NOW | RTLD_GLOBAL );
	if( hDLL )
	{
		pfn_app_kernel = (InitBackpointers_PTR) ::dlsym( hDLL, "InitBackpointersKernel" );
		pfn_app_client = (InitBackpointers_PTR) ::dlsym( hDLL, "InitBackpointersClient" );
	}
	
	// GET init callbacks function.
	InitBackpointers_PTR pfn = (inUseClientStrings || (pfn_app_kernel == NULL))
                            ? pfn_app_client
    						: pfn_app_kernel;


// --------------------
// This part more or less corresponds to Mac + Win version
// At 2016-08-22 Sergey and Team found that this part never works, as far as we see.
// So we not delete it for now ... and it not harms.
// --------------------
	if( !pfn ) // Need to dynamically load libraries
	{
	    bool use_vclient_dll = inUseClientStrings
        					|| (hDLL = ::dlopen( GetKernelDllName(), RTLD_NOW | RTLD_LOCAL )) == NULL;


		if( use_vclient_dll )
		{
			// Try to load VCLIENT implementation.
			hDLL = ::dlopen( GetClientDllName(), RTLD_NOW | RTLD_LOCAL );

            if( !hDLL )
            {
                // CRITICAL SITUATION: neither kernel nor client dll was not found!
                // Valentina cannot continue to work.
                // FIXME: throw exception!
                return;
            }
		}

	 	// GET init callbacks function
       pfn = use_vclient_dll
       		? (InitBackpointers_PTR) ::dlsym( hDLL, "InitBackpointersKernel" )
			: (InitBackpointers_PTR) ::dlsym( hDLL, "InitBackpointers" );
	}
// --------------------

    // CALL that function:
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
