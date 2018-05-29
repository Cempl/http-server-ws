/**********************************************************************************************/
/* FBL_Globals.cpp	                                                     					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Headers/FBL_Globals.h>
#include <VShared/FBL/publ/Headers/FBL_Version.h>
#include <VShared/FBL/publ/Util_classes/FBL_String.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_Location.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Localizable.h>
#include <VShared/FBL/publ/Interfaces/fbl_i_valueformater.h>
#include <VShared/FBL/publ/Threads/FBL_Synch.h>
#include <VShared/FBL/publ/Util_classes/FBL_ObjectPool.h>

#if FBL_MAC
#include <sys/types.h>
#include <sys/sysctl.h>
#endif //FBL_MAC

#if FBL_UNIX
#include <unistd.h>
#endif //FBL_UNIX


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
static CriticalSection			gUtilitiesMutex;
#define FBL_UTILITIES_LOCK		StCriticalSectionLocker gUtilitiesLock(gUtilitiesMutex)


/**********************************************************************************************/
True_Own_Thread_Mutex_Recursive* sGlobalEngineMutex		= nullptr;
CriticalSection*				sGlobalDiskMutex		= nullptr;
True_Thread_Mutex_Recursive*	sLogFolderCreationMutex	= nullptr;
True_Thread_Mutex_Recursive*	sLogTimeStampMutex		= nullptr;
True_Thread_Mutex_Recursive*	sWarningLogMutex		= nullptr;
True_Thread_Mutex_Recursive*	sTunesLogMutex			= nullptr;
True_Thread_Mutex_Recursive*	sReportsLogMutex		= nullptr;

#if __CHECK
True_Thread_Mutex_Recursive*	sAssertLogMutex			= nullptr;
True_Thread_Mutex_Recursive*	sTestStatisticMutex		= nullptr;
#endif // __CHECK

True_Thread_Mutex_Recursive*	sTaskManagerMutex		= nullptr;
True_Thread_Mutex_Recursive*	sNotificationCenterMutex = nullptr;


/**********************************************************************************************/
True_Own_Thread_Mutex_Recursive& GetGlobalEngineLock( void )
{
	if( sGlobalEngineMutex == nullptr )
	{
		FBL_UTILITIES_LOCK;
		if( sGlobalEngineMutex == nullptr )
			sGlobalEngineMutex = new True_Own_Thread_Mutex_Recursive;
	}
	
	return *sGlobalEngineMutex;
}


/**********************************************************************************************/
CriticalSection& GetGlobalDiskLock( void )
{
	if( sGlobalDiskMutex == nullptr )
	{
		FBL_UTILITIES_LOCK;
		if( sGlobalDiskMutex == nullptr )
			sGlobalDiskMutex = new CriticalSection;
	}
	
	return *sGlobalDiskMutex;
}


/**********************************************************************************************/
True_Thread_Mutex_Recursive& GetLogFolderCreationLock( void )
{
	if( sLogFolderCreationMutex == nullptr )
	{
		FBL_UTILITIES_LOCK;
		if( sLogFolderCreationMutex == nullptr )
			sLogFolderCreationMutex = new True_Thread_Mutex_Recursive;
	}

	return *sLogFolderCreationMutex;
}


/**********************************************************************************************/
True_Thread_Mutex_Recursive& GetLogTimeStampLock( void )
{
	if( sLogTimeStampMutex == nullptr )
	{
		FBL_UTILITIES_LOCK;
		if( sLogTimeStampMutex == nullptr )
			sLogTimeStampMutex = new True_Thread_Mutex_Recursive;
	}

	return *sLogTimeStampMutex;
}


/**********************************************************************************************/
True_Thread_Mutex_Recursive& GetWarningLogLock( void )
{
	if( sWarningLogMutex == nullptr )
	{
		FBL_UTILITIES_LOCK;
		if( sWarningLogMutex == nullptr )
			sWarningLogMutex = new True_Thread_Mutex_Recursive;
	}

	return *sWarningLogMutex;
}


/**********************************************************************************************/
True_Thread_Mutex_Recursive& GetTunesLogLock( void )
{
	if( sTunesLogMutex == nullptr )
	{
		FBL_UTILITIES_LOCK;
		if( sTunesLogMutex == nullptr )
			sTunesLogMutex = new True_Thread_Mutex_Recursive;
	}

	return *sTunesLogMutex;
}


/**********************************************************************************************/
True_Thread_Mutex_Recursive& GetReportsLogLock( void )
{
	if( sReportsLogMutex == nullptr )
	{
		FBL_UTILITIES_LOCK;
		if( sReportsLogMutex == nullptr )
			sReportsLogMutex = new True_Thread_Mutex_Recursive;
	}

	return *sReportsLogMutex;
}


/**********************************************************************************************/
#if __CHECK

/**********************************************************************************************/
True_Thread_Mutex_Recursive& GetAssertLogLock( void )
{
	if( sAssertLogMutex == nullptr )
	{
		FBL_UTILITIES_LOCK;
		if( sAssertLogMutex == nullptr )
			sAssertLogMutex = new True_Thread_Mutex_Recursive;
	}

	return *sAssertLogMutex;
}


/**********************************************************************************************/
True_Thread_Mutex_Recursive& GetTestStatisticLock( void )
{
	if( sTestStatisticMutex == nullptr )
	{
		FBL_UTILITIES_LOCK;
		if( sTestStatisticMutex == nullptr )
			sTestStatisticMutex = new True_Thread_Mutex_Recursive;
	}

	return *sTestStatisticMutex;
}


/**********************************************************************************************/
#endif // __CHECK


/**********************************************************************************************/
True_Thread_Mutex_Recursive& GetTaskManagerLock( void )
{
	if( sTaskManagerMutex == nullptr )
	{
		FBL_UTILITIES_LOCK;
		if( sTaskManagerMutex == nullptr )
			sTaskManagerMutex = new True_Thread_Mutex_Recursive;
	}

	return *sTaskManagerMutex;
}


/**********************************************************************************************/
True_Thread_Mutex_Recursive& GetNotificationCenterLock( void )
{
	if( sNotificationCenterMutex == nullptr )
	{
		FBL_UTILITIES_LOCK;
		if( sNotificationCenterMutex == nullptr )
			sNotificationCenterMutex = new True_Thread_Mutex_Recursive;
	}

	return *sNotificationCenterMutex;
}



#pragma mark -


/**********************************************************************************************/
// VSERVER sets this to be TRUE.
// VREPORT checks this flag to see if it needs to check "report serials" for ReportInit(),
// 	in vserver mode we do NOT need check serials, because vserver always have them enabled.
// Race-conditions are impossible - it is inited in ValentinaInit() and never changed.
//
bool gKernelInServerMode = false;


/**********************************************************************************************/
const vuint32 gKernelVersionMajor				= kKernelVersionMajor;
const vuint32 gKernelVersionMinor				= kKernelVersionMinor;
const vuint32 gKernelVersionRevision			= kKernelVersionRevision;
const vuint32 gKernelVersionMinorAndRevision	= kKernelVersionMinorAndRevision;

// See <MacTypes.h> for definitions of Stage
const vuint32 gKernelVersionStage				= kKernelVersionStage;
const vuint32 gKernelVersionInternalRelease		= kKernelVersionInternalRelease;
const char*	  gKernelVersionString				= kKernelVersionString;

const vuint32 gKernelVersion					= kKernelVersion;


/**********************************************************************************************/
const vuint16 gFloatDefaultPrecision			= 6;
const vuint16 gFloatDefaultScale				= 5;
const vuint16 gDoubleDefaultPrecision			= 15;
const vuint16 gDoubleDefaultScale				= 6;


/**********************************************************************************************/
const vint64 gSequenceDefaultMinValue			= 1;
const vint64 gSequenceDefaultMaxValue			= LLONG_MAX-1;
const vint64 gSequenceDefaultIncrementValue		= 1;
const bool   gSequenceDefaultIsCycled			= false;


/**********************************************************************************************/
// This flag specify if Valentina will fill by zero place of deleted records.
// If you set this true then it will take some time on erasing of info.
//
const bool gEraseInformation = true;


/**********************************************************************************************/
const char* GetKernelVersionString( void )
{
	return gKernelVersionString;
}


/**********************************************************************************************/
vint16 StringCompare( 
	const String& 	inStr1, 
    const String& 	inStr2, 
    bool 			inCaseSensitive )
{
	return ( inCaseSensitive )	? (vint16) inStr1.compare( inStr2 ) 
								: (vint16) inStr1.caseCompare( inStr2 , 0 );
}


/**********************************************************************************************/
vint16 StringCompare( 
	const String&	inStr1, 
	const UChar*	inStr2, 
	tslen 			inLength,
	bool			inCaseSensitive )
{
	return ( inCaseSensitive )	? (vint16) inStr1.compare( inStr2, inLength ) 
								: (vint16) inStr1.caseCompare( inStr2 , inLength, 0 );
}


#pragma mark -


/**********************************************************************************************/
CreateValueFormater_PTR gpCreateValueFormater = nullptr;


/**********************************************************************************************/
void SetCreateValueFormaterFunction( CreateValueFormater_PTR inpFnc )
{
	gpCreateValueFormater = inpFnc;
}


/**********************************************************************************************/
I_ValueFormater_Ptr CreateLocalValueFormater(
	const String*		inPattern,
	const EFormat		inFormat,
	const String*		inpLocale )
{
	return (gpCreateValueFormater) ? gpCreateValueFormater(inPattern, inFormat, inpLocale) : nullptr;
}


#pragma mark -

/**********************************************************************************************/
const vuint16 gOptimalThreadCountPerProcessor		= 16;


/**********************************************************************************************/
int GetProcessorCount( void )
{
#if FBL_WIN

  SYSTEM_INFO sys_info;
  ::GetSystemInfo( &sys_info );
  return sys_info.dwNumberOfProcessors;

#elif FBL_MAC

  int num_processors = 0;
  int mib[2] = { CTL_HW, HW_NCPU };
  size_t len = sizeof( num_processors );
  return ( ::sysctl( mib, 2, &num_processors, &len, 0, 0 ) != -1 ) ? num_processors : -1;

#elif FBL_UNIX
  return sysconf( _SC_NPROCESSORS_ONLN );
#else

  return -1;	// Unknown result

#endif //FBL_WIN
}


/**********************************************************************************************/
int GetOptimalThreadCount( void )
{
	int result = gOptimalThreadCountPerProcessor;
	
	int processorCount = GetProcessorCount();
	if( processorCount > 0 )
	{
		result *= processorCount;
	}
	
	return result;
}


#pragma mark -

/**********************************************************************************************/
#if FBL_INTERNAL_THREADSAFE || FBL_EXTERNAL_THREADSAFE

	#if FBL_POOLED_LOCALIZABLE_SYS
		TLS_Pooled_Var<I_Localizable>			tlsSysLocalizable;
	#else
		TLS_I_Unknown_Var<I_Localizable>		tlsSysLocalizable;
	#endif // FBL_POOLED_LOCALIZABLE_SYS

#endif // FBL_INTERNAL_THREADSAFE || FBL_EXTERNAL_THREADSAFE


/**********************************************************************************************/
// This flag specify if Valentina kernel will be thread-safe calling stub-interfaces in runtime.
// We believe that only proper way to change this flag is BEFORE starting any additional thread,
// so it is safe to be simple boolean.
//
bool gGlobalThreadSafe = false;


#pragma mark -

/**********************************************************************************************/
// Callbacks:
//
YieldFuncPtr pYieldFunction = nullptr;


#pragma mark -

/**********************************************************************************************/
// vSQLite
//
const char* gvSqlite_journal_sfx	= "-journal";
const char* gvSqlite_wal_sfx		= "-wal";
const char* gvSqlite_shm_sfx		= "-shm";


/**********************************************************************************************/
FBL_End_Namespace
