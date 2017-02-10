/**********************************************************************************************/
/* FBL_Util_Dll_Win.cpp	 	                                               					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Util_Paths/FBL_Util_Dll_Win.h>


/**********************************************************************************************/
#if FBL_WIN


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
BOOL GetDllVersion( HINSTANCE hInstDLL, DLLVERSIONINFO* pDllVersionInfo )
{
	//FBL_CHECK(!::IsBadWritePtr(pDllVersionInfo, sizeof(DLLVERSIONINFO)));

	// We must get this function explicitly because some DLLs don't implement it.
	DLLGETVERSIONPROC pfnDllGetVersion = (DLLGETVERSIONPROC)::GetProcAddress(hInstDLL, "DllGetVersion");
	if (pfnDllGetVersion == NULL)
		return FALSE;

	return (*pfnDllGetVersion)(pDllVersionInfo) ? true : false;
}


/**********************************************************************************************/
BOOL GetDllVersion( LPCTSTR lpDLLPath, DLLVERSIONINFO* pDllVersionInfo )
{
	HINSTANCE hInstDLL = ::LoadLibrary(lpDLLPath);
	if( hInstDLL == NULL )
		return FALSE;

	BOOL bRet = GetDllVersion(hInstDLL, pDllVersionInfo) == NO_ERROR;
	::FreeLibrary(hInstDLL);
	return bRet;
}


/**********************************************************************************************/
// Common Control Versions:
//   Win95/WinNT 4.0    maj=4 min=00
//   IE 3.x             maj=4 min=70
//   IE 4.0             maj=4 min=71
//   IE 5.0             maj=5 min=80
//   Win2000            maj=5 min=81
BOOL GetCommCtrlVersion( LPDWORD pdwMajor, LPDWORD pdwMinor )
{
	//FBL_CHECK(!::IsBadWritePtr(pdwMajor, sizeof(DWORD)) && !::IsBadWritePtr(pdwMinor, sizeof(DWORD)));

	DLLVERSIONINFO dvi;
	memset(&dvi, 0, sizeof(dvi));
	dvi.cbSize = sizeof(dvi);
	BOOL b = GetDllVersion(TEXT("comctl32.dll"), &dvi);

	if( b )
	{
		*pdwMajor = dvi.dwMajorVersion;
		*pdwMinor = dvi.dwMinorVersion;
	}
	else if( b == -1 )
	{
		// If DllGetVersion is not there, then the DLL is a version
		// previous to the one shipped with IE 4.x
		*pdwMajor = 4;
		*pdwMinor = 0;
		b = TRUE;
	}

	return b;
}


/**********************************************************************************************/
LONG GetCommCtrlVersion( void )
{
	DWORD dwMajor = 0, dwMinor = 0;
	GetCommCtrlVersion(&dwMajor, &dwMinor);
	return MAKELONG(dwMinor, dwMajor);
}


/**********************************************************************************************/
// Shell Versions:
//   Win95/WinNT 4.0                                maj=4 min=00
//   IE 3.x, IE 4.0 without Web Integrated Desktop  maj=4 min=00
//   IE 4.0 with Web Integrated Desktop             maj=4 min=71
//   IE 4.01 with Web Integrated Desktop            maj=4 min=72
//   Win2000                                        maj=5 min=00
BOOL GetShellVersion( LPDWORD pdwMajor, LPDWORD pdwMinor )
{
	//FBL_CHECK(!::IsBadWritePtr(pdwMajor, sizeof(DWORD)) && !::IsBadWritePtr(pdwMinor, sizeof(DWORD)));

	DLLVERSIONINFO dvi;
	memset(&dvi, 0, sizeof(dvi));
	dvi.cbSize = sizeof(dvi);
	BOOL b = GetDllVersion(TEXT("shell32.dll"), &dvi);

	if( b )
	{
		*pdwMajor = dvi.dwMajorVersion;
		*pdwMinor = dvi.dwMinorVersion;
	}
	else if( b == -1 )
	{
		// If DllGetVersion is not there, then the DLL is a version
		// previous to the one shipped with IE 4.x
		*pdwMajor = 4;
		*pdwMinor = 0;
		b = TRUE;
	}

	return b;
}


/**********************************************************************************************/
LONG GetShellVersion( void )
{
	DWORD dwMajor = 0, dwMinor = 0;
	GetShellVersion(&dwMajor, &dwMinor);
	return MAKELONG(dwMajor, dwMinor);
}


/**********************************************************************************************/
bool IsWindowsNTFamilyOS( void )
{
	static bool first = true;
	static bool isNT = false;

	if( first )
	{
		OSVERSIONINFOEX osvi;
		BOOL bOsVersionInfoEx;

		// Try calling GetVersionEx using the OSVERSIONINFOEX structure.
		// If that fails, try using the OSVERSIONINFO structure.
		ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

		if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
		{
			osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
			if (! GetVersionEx( (OSVERSIONINFO *) &osvi) ) 
			{
				//FBL_Throw(xExternalError( ERR_EXTERNAL ));
				// Don't know what to do here - byt we can't generate exception
				// because of circular dependency.
				isNT = false;
				first = false;
				goto exit;
			}
		}

		isNT = (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT);
		first = false;
	}

exit:;
	return isNT;
}


/**********************************************************************************************/
FBL_End_Namespace

#endif // FBL_WIN
