/**********************************************************************************************/
/* FBL_Starter_Mac.cpp                                                 						  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Starter/FBL_Starter.h>

#include <VShared/FBL/publ/Util_classes/FBL_String.h>
#include <VShared/FBL/publ/Utilities/FBL_Util_Path.h>

#if FBL_MAC_MACHO
	#define DLOPEN_NO_WARN
	#include <dlfcn.h>	
#endif // FBL_MAC_MACHO 


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
void* LoadLibrary( CFURLRef inLibrary );
void* LoadLibrary( CFURLRef inLibrary )
{
	void* handle;
	
	CFStringRef path_ref = CFURLCopyFileSystemPath( inLibrary, kCFURLPOSIXPathStyle );

	char cpath[PATH_MAX];
	Boolean cfres = CFStringGetCString( path_ref, cpath, sizeof(cpath)-1, kCFStringEncodingUTF8 );
	argused1( cfres );

	handle = (void*) dlopen( cpath, 0 ); // 0 - is mode.

	// cleanup:
	CFRelease( path_ref );
	
	return handle;	
}
						
/**********************************************************************************************/
void* GetFunction( void* inLibrary, const char* inName );
void* GetFunction( void* inLibrary, const char* inName )
{
	void *symbol = NULL;    // return vwalue
	
	symbol = dlsym( inLibrary, inName );

	return symbol;
}
						

#pragma mark -


/**********************************************************************************************/
// returns .../Folder/App.app
//
CFURLRef FindAppBundleCopy( void )
{
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	
	CFURLRef mainURL = CFBundleCopyBundleURL( mainBundle ); 
	
	return mainURL; // should be destroyed later
}


/**********************************************************************************************/
// Starting with 4.3 - it returns folder, which contains APPL.app
//
CFURLRef FindAppFolderCopy( void )
{
	CFURLRef mainURL = FindAppBundleCopy(); 
	
	// We need to get the parent folder: .../Folder
	CFURLRef parentURL = CFURLCreateCopyDeletingLastPathComponent( NULL, mainURL );
	CFRelease( mainURL );
	
	return parentURL; // should be destroyed later
}


#pragma mark -


/**********************************************************************************************/
#if FBL_MAC_MACHO																// MACHO
// Macho version search the APP path.
// Macho version expect to see for now only this configuration:
//		my.app/Contents/vcomponents
// 		/usr/local/lib/vcomponents
//

#if FBL_64
	#define VCOMPONENTS "vcomponents_x64"
#else
	#define VCOMPONENTS "vcomponents"
#endif // FBL_64


CFURLRef FindVcomponents( void )
{
	// ---------------------------
	// APP.app/Contents/vcomponents 
	//
	CFURLRef app_URL = FindAppBundleCopy(); 
 
	CFURLRef app_vcomp_URL = CFURLCreateCopyAppendingPathComponent(
		kCFAllocatorDefault, app_URL, CFSTR( "Contents/" VCOMPONENTS ), true );  

// for iOS temporay disabled part of code.
// RZ: 2/11/10 TODO Hmm, it seems we need yet remove FSRef absolutly or make another branch for iphone
#if FBL_SUPPORT_FSREF

	FSRef app_vcomp_FSRef;
	Boolean bres = CFURLGetFSRef( app_vcomp_URL, &app_vcomp_FSRef );

	if( bres == false )
	{
		// We are working not in the APP.app mode,
		// e.g. we are working as plugin of Director/Rev/PHP IDE.

        CFRelease( app_vcomp_URL ); // this was wrong attempt.


		// OR this can be console_app with vcomponent items located in its folder.
        // So we can check 'vresources' folder to see if this is case
        CFURLRef app_vres_URL = CFURLCreateCopyAppendingPathComponent(
            kCFAllocatorDefault, app_URL, CFSTR( "vresources" ), true );

        FSRef app_vres_FSRef;
        Boolean exists_vresources = CFURLGetFSRef( app_vres_URL, &app_vres_FSRef );

        CFRelease( app_vres_URL ); // we do not need URL of vresources itself.

		if( exists_vresources )
		{
        	return app_URL;
		}
        else // THEN we check "/usr/local/lib/vcomponents"
        {
            app_vcomp_URL = CFURLCreateWithFileSystemPath(
                kCFAllocatorDefault, CFSTR( "/usr/local/lib/" VCOMPONENTS ), kCFURLPOSIXPathStyle, true );
		}
	}


#endif // FBL_SUPPORT_FSREF
	

	CFRelease( app_URL ); app_URL = nil;

	return app_vcomp_URL;
}
#endif // FBL_MAC_MACHO


#pragma mark -


/**********************************************************************************************/
#if !FBL_UNIX


/**********************************************************************************************/
extern "C" void InitBackpointers( void );


/**********************************************************************************************/
void Init_ValentinaDlls( bool inUseClientStrings )
{
#if FBL_STATIC 

	// This branch of code is used mainly by kernel_test project. And may be ONLY.
	// Here we just call GetStringFactory, because we do not have DLLs. 

	argused1( inUseClientStrings );
	InitBackpointers();

#else // FBL_STATIC 

	// KERNEL DLL string-factory has priority, i.e.
	// if sFactory is not NULL and call is from VCLIENT.DLL, we do not change pointers.
	// if sFactory is not NULL but call is from KERNEL.DLL, we will reset pointers to KERNEL strings. 
	// 
	if( String::sFactory && inUseClientStrings )
		return;

	CFURLRef vcomp_url = FindVcomponents();
	
	CFStringRef kernel_name_ref = CFStringCreateWithCString( 
		kCFAllocatorDefault, GetKernelDllName(), kCFStringEncodingUTF8 );
	
	CFURLRef kernel_URL = CFURLCreateCopyAppendingPathComponent(
		kCFAllocatorDefault, vcomp_url, kernel_name_ref, false );

	CFRelease( kernel_name_ref ); kernel_name_ref = NULL;

	// ---------------------------------------------
	// WE MUST load VKERNEL or VCLIENT dll at least.    
	void* dll = 0;
	
    // We must choose vclient if 
    // 	* inUseClientStrings is FALSE, i.e. call from Valentina.Init()
    // 	* VKERNEL.dll is not found. ???? RZ 04/24/12 ???? 
    bool use_vclient_dll = inUseClientStrings || (dll = LoadLibrary(kernel_URL)) == NULL; 
    
	if( use_vclient_dll )
	{
		CFStringRef vclient_name_ref = CFStringCreateWithCString( 
			kCFAllocatorDefault, GetClientDllName(), kCFStringEncodingUTF8 );

		CFURLRef client_URL = CFURLCreateCopyAppendingPathComponent(
			kCFAllocatorDefault, vcomp_url, vclient_name_ref, false );
	
		// Try to load VClient.DLL
		dll = LoadLibrary( client_URL ); 
		
		CFRelease( vclient_name_ref ); 	vclient_name_ref = NULL;
		CFRelease( client_URL ); 		client_URL = NULL; 
		
		if( !dll )
		{
			// CRITICAL SITUATION: neither kernel nor client dll was not found!
			// Valentina cannot continue to work.
			// FIXME: throw exception!

			CFRelease( kernel_URL ); 	kernel_URL = NULL;
			CFRelease( vcomp_url ); 	vcomp_url  = NULL;
			return;
		}
	}

	CFRelease( kernel_URL ); 	kernel_URL = NULL;
	CFRelease( vcomp_url ); 	vcomp_url  = NULL;

	// --------------------------------
	// Get init callbacks function.
	typedef void (*InitBackpointers_PTR)( void );
	InitBackpointers_PTR pfn;

	// Store reference to the function and call it:
	pfn = (InitBackpointers_PTR) GetFunction( dll, "InitBackpointers" );
	pfn();

#endif // FBL_STATIC 

}


/**********************************************************************************************/
bool LoadVProjectDll( void )
{
	void* dll = 0;
	
	CFURLRef vcomp_url = FindVcomponents();

	CFStringRef vproject_name_ref = CFStringCreateWithCString( 
		kCFAllocatorDefault, GetVReportDllName(), kCFStringEncodingUTF8 );
	
	CFURLRef vproject_URL = CFURLCreateCopyAppendingPathComponent(
		kCFAllocatorDefault, vcomp_url, vproject_name_ref, false );

	// Try to load.
	dll = LoadLibrary( vproject_URL );	

	CFRelease( vproject_URL );			vproject_URL 	 	= NULL;
	CFRelease( vproject_name_ref );		vproject_name_ref 	= NULL;
	CFRelease( vcomp_url ); 			vcomp_url  			= NULL;

	return (dll) ? true : false;
}


/**********************************************************************************************/
#endif  // !FBL_UNIX

/**********************************************************************************************/
FBL_End_Namespace

