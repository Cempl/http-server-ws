/**********************************************************************************************/
/* FBL_Starter_Win.cpp                                                 						  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Starter/FBL_Starter.h>
#include <VShared/FBL/publ/Util_classes/FBL_String.h>


/**********************************************************************************************/
#if FBL_WIN

#pragma warning(push)
#pragma warning(disable: 4005)
#include <shlobj.h>
#pragma warning(pop)

/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
/** Returns packed version of Windows Shell DLL. */
FBL_SHARED_EXP LONG GetShellVersion ();


/**********************************************************************************************/
const char* GetKernelDllName( void );
const char* GetClientDllName( void );

typedef HRESULT (__stdcall *SHGetFolderPathPfn)(
	HWND hwndOwner,
    int nFolder,
    HANDLE hToken,
    DWORD dwFlags,
    LPTSTR pszPath
);



/**********************************************************************************************/
void FindComponentsFolder( char* outPath )
{
	// PATH's tail for vcomponents folder.
	const char* pVComponents_Tail = "\\Paradigma Software\\VComponents_Win_VC\\";

	// In case of any troubles with getting actual %ProgramFiles% variable we set
	// this path as path to vComponents folder.
	const char* pDefault_VComponents_Path = "C:\\Program Files\\Paradigma Software\\VComponents_Win_VC\\";

	LONG v = GetShellVersion();
	if( v >= MAKELONG(5, 0) )
	{
		HMODULE hShell = LoadLibrary("shell32.dll");
		if( hShell )
		{
			SHGetFolderPathPfn pfn = (SHGetFolderPathPfn)::GetProcAddress(hShell, "SHGetFolderPathA");
			if( pfn )
			{
				if( SUCCEEDED( pfn(NULL, CSIDL_PROGRAM_FILES, NULL, 0, outPath) ) )
				{
					// Now outPath is something like this one - "c:\Program Files"
					// or "c:\Program Files (x86)" depending on app and windows architecture.
					//
					::strcat( outPath, pVComponents_Tail );
				}
				else
				{
					// Error in SHGetFolderPathA() - fallback to the default path.
					::strcpy( outPath, pDefault_VComponents_Path );
				}
			}
			else
			{
				// No SHGetFolderPathA() in shell32.dll - fallback to the default path.
				::strcpy( outPath, pDefault_VComponents_Path );
			}

			FreeLibrary(hShell);
		}
		else
		{
			// Error loading shell32.dll - fallback to the default path.
			::strcpy( outPath, pDefault_VComponents_Path );
		}
	}
	else
	{
		// Ancient windows version - fallback to the default path.
		::strcpy( outPath, pDefault_VComponents_Path );
	}
}


/**********************************************************************************************/
extern "C" void InitBackpointers( void );


/**********************************************************************************************/
void Init_ValentinaDlls( bool inUseClientStrings )
{
#if FBL_STATIC

	argused1( inUseClientStrings );
	InitBackpointers();

#else

	// Kernel-dll string-factory has priority.
	// if sFactory is not NULL and call is from VCLIENT.DLL, we do not change pointers.
	// if sFactory is not NULL but call is from KERNEL.DLL, we will reset pointers to KERNEL strings. 
	// 
	if( String::sFactory && inUseClientStrings )
		return;

	HMODULE hDLL = 0;
	
    // We must choose vclient if 
    // 	* inUseClientStrings is TRUE, i.e. call from Valentina.InitClient()
    // OR
    // 	* VKERNEL.dll is not found, but call was from Valentina.Init()
    bool use_vclient_dll = inUseClientStrings
                        || (hDLL = LoadLibrary(GetKernelDllName())) == NULL;


	if( use_vclient_dll )
	{
		// Try to load VCLIENT implementation.
        hDLL = LoadLibrary( GetClientDllName() );
        
		if( !hDLL )
		{
			// CRITICAL SITUATION: neither kernel nor client dll was not found!
			// Valentina cannot continue to work.
			// FIXME: throw exception!
			return;
		}
	}

	// GET init callbacks function
	typedef void (*InitBackpointers_PTR)( void );
	InitBackpointers_PTR pfn;

	// STORE reference to the function and call it:
	pfn = (InitBackpointers_PTR) GetProcAddress( hDLL, "InitBackpointers" );

    // CALL that function:
	pfn();

#endif // FBL_STATIC
}


/**********************************************************************************************/
bool LoadVProjectDll( void )
{
	HMODULE hDLL;
	
	// Try to load.
	hDLL = LoadLibrary( GetVReportDllName() );

	return (hDLL) ? true : false;
}


/**********************************************************************************************/
FBL_End_Namespace

#endif // FBL_WIN
