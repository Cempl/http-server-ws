/**********************************************************************************************/
/* FBL_Macros.h 	                                                      					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_Macros_h 
	#define _FBL_Macros_h
#pragma once


/**********************************************************************************************/
#ifdef _MSC_VER
#pragma warning( disable: 4068 )
#endif

/**********************************************************************************************/
// [Under development].
// Transaction support.
//
#define TRANSACTION_SUPPORT				0


#pragma mark -
#pragma mark ======= ThreadSafe aspects ======

/**********************************************************************************************/
// ON ( at least vClient can be running in 2 threads now (notifications) ).
// If true -
//	- real mutexes, conditional var and so on will be used and "fake" ones otherwise;
//		Pay attention that algorithms and others who NEED the real mutexes
//		(or have no much sense without them) MUST use True_Mutexes.
//	- LocalizableSys, DebugNew, I:Unknown::mRef are thread-safe.
//
#define FBL_INTERNAL_THREADSAFE			1


/**********************************************************************************************/
// OFF for everything except V4NET, V4CC, VSDK, vServer_MT.
// If true - each interface method is thread safe.
// Note that rest our products like vServer_old, vStudio synch vKernel self and does not require
// thread-safe vKernel. The rest of ADKs can not use "external" multi-threading as well.
// Does not require FBL_INTERNAL_THREADSAFE because
//	- uses True_Mutexes;
//	- LocalizableSys, DebugNew, I:Unknown::mRef are thread-safe as well as for FBL_INTERNAL_THREADSAFE.
//
#define FBL_EXTERNAL_THREADSAFE			 FBL_MULTI_THREADED && 1


/**********************************************************************************************/
// OFF since only MT_Reindex uses it for now.
// Task manager is a special feature for running special MT tasks.
// Does not require FBL_INTERNAL_THREADSAFE because uses True_Mutexes.
// Does not require FBL_MULTI_THREADED_ADVANCED because uses Tread_Simple.
//
#define FBL_TASK_MANAGER				0
#define FBL_TASK_MANAGER_LOG			FBL_TASK_MANAGER && FBL_TEST_CODE


/**********************************************************************************************/
// OFF [Under development].
// Stuff required for MultiThreaded Reindex -- WHEN FEW indexes are built at the same time.
// (Current results even worst than single-threaded approach because of either cache
// or disk - which are very hot places for reindex algorithms)
//
#define FBL_MT_REINDEX					FBL_TASK_MANAGER && FBL_INTERNAL_THREADSAFE && 0


/**********************************************************************************************/
// OFF [Under development].
// Stuff required for sqlParser as ObjectPool object (Currently ANTLR uses a lot of "single" AST
// nodes, building different trees, so 2 parsers builds 2 trees which have common nodes).
//
#define FBL_POOLED_SQL_PARSER			(FBL_INTERNAL_THREADSAFE || FBL_EXTERNAL_THREADSAFE) && 0


/**********************************************************************************************/
// ON for MT environments.
// Stuff required for Localizable_Sys as ObjectPool object.
// If true - TLS_Pooled_Var<I_Localizable> will be used.
//		(Each thread gets own LocalizableSys, but from some global pool - so neither
//		real delete nor create (in case of enough "not in-use" objects in the pool)
//		for LocalizableSys objects.)
// If 0, but FBL_(IN/EX)TERNAL_THREADSAFE == 1 - TLS_I_Unknown_Var<I_Localizable> will be used.
//		(Each thread create/delete own LocalizableSys)
//
#define FBL_POOLED_LOCALIZABLE_SYS		(FBL_INTERNAL_THREADSAFE || FBL_EXTERNAL_THREADSAFE) && 1


/**********************************************************************************************/
// OFF since nobody use it for now.
// If true - fbl::Thread_Advanced is available (both internally and externally)
// If false - special fbl::Tread_Null does the same job in plain manner.
// Note that FBL_Thread is not affected by this param and always available.
//
#define FBL_MULTI_THREADED_ADVANCED		0


/**********************************************************************************************/
#define ADDREF(refs)	(++((refs).mValue))
#define RELEASE(refs)	(--((refs).mValue))

#define RETAIN ADDREF
#define Retain AddRef

#if __GNUC__
	#define FBL_INLINE(ret_type) static inline ret_type __attribute__((unused))
	#define FBL_INLINE_ATTR __attribute__((__always_inline__))
	#define FBL_ALIGNED(b) __attribute__((aligned(b)))
#else
	#define FBL_INLINE(ret_type) static inline ret_type
	#define FBL_INLINE_ATTR
	#define FBL_ALIGNED(b) /* FIX ME */
#endif // __GNUC__


#pragma mark -

/**********************************************************************************************/
#if __LP64__ || defined(_WIN64) // || LinuxGCCMacro
	#define FBL_64 1
#else
	#define FBL_64 0
#endif 


/**********************************************************************************************/
//	std Namespace

	// Macro for controlling use of "std" namespace for the C++ standard libraries.
	// Set to OFF only if you have explicitly disabled namespace support
	// in the MSL or are using another implementation of the C++
	// standard libraries that does not support namespaces.

#ifndef Uses_Std_Namespace
	#define	Uses_Std_Namespace			1
#endif // Uses_Std_Namespace

#if Uses_Std_Namespace
	#define Std_Begin_Namespace			namespace std {
	#define Std_End_Namespace			}
	#define Std_Using_Namespace			using namespace std;
	#define STD std
#else
	#define Std_Begin_Namespace			
	#define Std_End_Namespace			
	#define Std_Using_Namespace
	#define STD
#endif // Uses_Std_Namespace


// open namespaces for use in the sources of Kernel
//namespace std {}
//Std_Using_Namespace


/**********************************************************************************************/
//	FBL Namespaces.
// 	This is a namespace of kernel interfaces and kernel implementation of low (relational) level.
//
#ifndef Uses_Namespace
	#define Uses_Namespace				1
#endif

#if Uses_Namespace
	#define FBL_Begin_Namespace			namespace fbl {
	#define FBL_End_Namespace			}
	#define FBL_Using_Namespace			using namespace fbl;
	#define FBL							fbl
#else
	#define FBL_Begin_Namespace
	#define FBL_End_Namespace
	#define FBL_Using_Namespace
	#define FBL
#endif // Uses_Namespace


/**********************************************************************************************/
// Open-close namespaces, so compiler become awere about it.
//
FBL_Begin_Namespace		
FBL_End_Namespace


/**********************************************************************************************/
//									*** MACROS ***
/**********************************************************************************************/

#if  defined(_MSC_VER) 

	// Visual Studio C/C++ from Microsoft Inc.
	#if defined(_M_IX86) || defined(_M_X64)
		#define FBL_MAC 		0 
		#define FBL_MAC_MACHO 	0
		#define FBL_MAC_CARBON	0
		#define FBL_WIN 		1
		#define FBL_UNIX 		0
	#else
		#error "Not recognized target for Visual C++."
	#endif // _M_IX86

#endif //defined(_MSC_VER)


/**********************************************************************************************/
#if defined(__linux__)
    #define FBL_MAC 		0
    #define FBL_MAC_MACHO 	0
    #define FBL_MAC_CARBON	0
    #define FBL_WIN 		0
    #define FBL_UNIX 		1
#endif // defined(__linux__)
	
	
/**********************************************************************************************/
#if defined(__GNUC__) && (defined(__APPLE_CPP__) || defined(__APPLE_CC__) || defined(__NEXT_CPP__))
// gcc based compilers used on OpenStep -> Rhapsody -> Mac OS X
	#define FBL_MAC 		1 
	#define FBL_MAC_MACHO 	1
	#define FBL_MAC_CARBON	0
	#define FBL_WIN 		0
	#define FBL_UNIX 		0
#endif // defined(__APPLE_CPP__) || defined(__APPLE_CC__) || defined(__NEXT_CPP__)


#if defined(__GNUC__) && defined(__MINGW32__)
// Mingw gnu gcc/egcs compiler for Win32 systems (http://www.mingw.org).
	#define FBL_MAC 		0 
	#define FBL_MAC_MACHO 	0
	#define FBL_MAC_CARBON	0
	#define FBL_WIN 		1
	#define FBL_UNIX 		0
#endif // defined(__GNUC__) && defined(__MINGW32__)


/**********************************************************************************************/
// this is TRUE if target is MACHO or UNIX, so we use POSIX API for files, threads, ...
//
#define FBL_POSIX_API (FBL_MAC_MACHO || FBL_UNIX)


/**********************************************************************************************/
// For FBL_MAC we need to check if this is INTEL mac. 
//
#if FBL_MAC 
	#if TARGET_CPU_X86 || TARGET_CPU_X86_64
		#define FBL_INTEL_CPU 1
	#else 
		#define FBL_INTEL_CPU 0
	#endif //__LITTLE_ENDIAN__
#elif FBL_UNIX
    #ifdef __arm__
        #define FBL_INTEL_CPU 0
    #else
        #define FBL_INTEL_CPU 1
    #endif
#endif // FBL_MAC	


/**********************************************************************************************/
#if FBL_WIN
	#define FBL_BIG_ENDIAN		0
	#define FBL_LITTLE_ENDIAN	1

#elif FBL_MAC
   	#define FBL_BIG_ENDIAN		TARGET_RT_BIG_ENDIAN
    #define FBL_LITTLE_ENDIAN	TARGET_RT_LITTLE_ENDIAN

#elif FBL_UNIX		// KP: We work now just on i386 CPU on linux
	#define FBL_BIG_ENDIAN		0
	#define FBL_LITTLE_ENDIAN	1

#endif // FBL_WIN


#pragma mark -


/**********************************************************************************************/
#ifndef FBL_SUPPORT_FSSPEC
	#if FBL_MAC && !__LP64__
		#define FBL_SUPPORT_FSSPEC	1
	#else
		#define FBL_SUPPORT_FSSPEC	0
	#endif // __LP64__
#endif // FBL_SUPPORT_FSSPEC


/**********************************************************************************************/
#ifndef FBL_SUPPORT_FSREF
	#if FBL_MAC_MACHO
		#define FBL_SUPPORT_FSREF 1
	#endif // FBL_MAC_MACHO 
#endif // FBL_SUPPORT_FSREF


/**********************************************************************************************/
#ifndef FBL_USE_PAINTLIB
	#define FBL_USE_PAINTLIB 1
#endif // FBL_USE_PAINTLIB


#pragma mark -
#pragma mark ======= C++ "Extentions" ======


/**********************************************************************************************/
// C++ allow to developer silently ignore result of some function.
//	int res = foo( x, y );
//	foo( x, y );				// all right
// 
// YES, exists compiler warning, but it is always OFF, because then tons warnings everybody will get.
// In REALbasic, for example, compiler consider this as error in code, and developer must write
// CALL foo( x, y );
//
// In C++ we can close warning using syntax
//  (void) foo( x, y );
//
// RZ thinks that the following macro can be more correct by name (than CALL of RB),
// and catch eye of reader and be able close warning if it is ON.
//
// USAGE: 
//	IGNORE_RESULT foo(x, y);
//
#define IGNORE_RESULT (void)


/**********************************************************************************************/
// Macro to emulate "interface" keyword in the C++ language.
//
#ifdef interface
#	undef interface
#endif // interface

#define interface struct


/**********************************************************************************************/
// MS C++/CLI adds 'abstract' keyword just for style.
//
// USAGE:
//		virtual void foo() abstract;
//
#define abstract = 0



/**********************************************************************************************/
/*	RZ 2014-08-02:

	NEW IDEA - OBSERVATION:

virtual - standard modifier for all cases.

virtnew - class adds a new virtual method.
virtabs - class adds a new virtual pure function.

virtovr - overriden virtual method, when it not fits to below cases (rare?):
virtimp - the first implementation of abstract method from the parent class. Igor's idea.


	// VERY PROBLEMATIC but interesting:
	//
virtrep	- REPLACE parent alg. Cannot call it.
virtadd - ADDs something after parent alg. Parent alg MUST BE CALLED first near to start of func.
virtins - INSERTs something before parent alg. Parent alg MUST BE CALLED last near to end of func.
virtwrp - WRAPs parent alg before and after. Parent alg MUST BE CALLED in the middle of this.
*/


/**********************************************************************************************/
// RZ experimental. 2011-07-10
//
// USAGE:
//
// class A
// {
//	virtual void fx();
//	virtual void fy();
// };
//
// class B : public A
// {
//	virtovr void fx();		<<< dev clear see that this method have come from parent.
//
//	virtual void fz();		<<< this method was NOT in the parent and introduce in B.
//	virtovr void fe();		<<< in ideal - compiller error, like in MS C++/CLI
// }
//
//
// short alias with length same as 'virtual' = 7 chars
// can be good to find/replace in EXISTED sources to not break layout of existed code.
//


/**********************************************************************************************/
#define virtnew virtual
#define virtabs virtual

#define virtovr virtual
#define virtimp virtual


// PROBLEMATIC: adds info about HOW child method is implemented.
//
#define virtrep virtual
#define virtadd virtual
#define virtins virtual
#define virtwrp virtual


#pragma mark -


/**********************************************************************************************/
#ifndef FBL_LENGTH
#	define FBL_LENGTH 64 	
#endif	


/**********************************************************************************************/
// Smart pointer for Valentina kernel.
//
#define SMART_PTR(ptr_class, name)\
	typedef ptr_class<name> name##_Ptr 

#define CONST_SMART_PTR(ptr_class, name)\
	typedef ptr_class<const name> Const_##name##_Ptr 

#define FULL_SMART_PTR(ptr_class, name)	\
	SMART_PTR(ptr_class, name);\
	CONST_SMART_PTR(ptr_class, name)

#define FBL_SMART_PTR(name)	\
	FULL_SMART_PTR( FBL::smart_ptr, name )

#define SMART_INTERFACE(name) 	\
	interface name; 			\
	FBL_SMART_PTR(name)

#define SMART_CLASS(name) 		\
	class name; 				\
	FBL_SMART_PTR(name)

#define SMART_STRUCT(name) 		\
	struct name; 				\
	FBL_SMART_PTR(name)

#define SMART_VALUE_INTERFACE(name) 	\
	interface name; 			\
	FULL_SMART_PTR(fbl_smart_value_ptr, name)


/**********************************************************************************************/
// Smart Array  pointers.
//
#define SMART_ARRAY( name, item_type ) \
	typedef Array<item_type> name; \
	FULL_SMART_PTR(smart_ptr, name)

#define SMART_ARRAY_OF_SIMPLES( name, item_type ) \
	typedef ArrayOfSimples<item_type> name; \
	FULL_SMART_PTR(smart_ptr, name)

#define SMART_ARRAY_OF_SIMPLE_PTRS( name, item_type ) \
	typedef ArrayOfPtrs<item_type> name; \
	FBL_SMART_PTR(name)

#define SMART_ARRAY_OF_CLASSES( name, item_type ) \
	typedef ArrayOfClasses<item_type> name; \
	FBL_SMART_PTR(name)

#define SMART_ARRAY_OF_CLASS_PTRS( name, item_type ) \
	typedef ArrayOfPtrs<item_type> name; \
	FBL_SMART_PTR(name)

#define SMART_ARRAY_OF_SMART_PTRS( name, item_type ) \
	typedef ArrayOfSmartPtrs<item_type> name; \
	FBL_SMART_PTR(name)


/**********************************************************************************************/
// Smart stack pointers.
//
#define SMART_STACK_OF_SIMPLE_PTRS( name, item_type ) \
	typedef Stack<item_type> name; \
	FBL_SMART_PTR(name)


/**********************************************************************************************/
// Macro for std::unique_ptr
//
#if __cplusplus > 199711L
	#define aptr std::unique_ptr
#else
	#define aptr std::auto_ptr
#endif // __cplusplus > 199711L


/**********************************************************************************************/
// Plugin macro
// Each Valentina plugin must have extension ".plugin";
// Each Valentina plugin must have exported C function with the specified name.
//
#define FBL_PLUGIN_EXTENTION "plugin"

#if FBL_UNIX || FBL_MAC_MACHO
#	define FBL_PLUGIN_INIT_FUNCTION "_Initialize"
#else // FBL_WIN || FBL_MAC
#	define FBL_PLUGIN_INIT_FUNCTION "Initialize"
#endif // FBL_UNIX


/**********************************************************************************************/
#define SIZE_OF_ARRAY(a) \
	sizeof(a) / sizeof(a[0])


/**********************************************************************************************/
#define ZERO(x) \
	memset(&(x), 0, sizeof(x));


/**********************************************************************************************/
// IF we compile kernel for Application, e.g. Valentina Server,
// we do not need overhead from support of ForeignPlugin, 
// then for such project this macro can be set to 1.
//
#ifndef FBL_Skip_ForeignPlugin
#	define FBL_Skip_ForeignPlugin 0 
#endif // FBL_Skip_ForeignPlugin


/**********************************************************************************************/
// Define this to allow static linking with eXpat.
//
#ifndef XML_STATIC
#	define XML_STATIC
#endif // XML_STATIC


/**********************************************************************************************/
#ifdef __GNUC__ 
	#define FBL_FORCE_INLINE __attribute((always_inline)) inline 
#elif defined(_MSC_VER) 
	#define FBL_FORCE_INLINE __forceinline 
#else // includes __MWERKS__
	#define FBL_FORCE_INLINE inline
#endif


/**********************************************************************************************/
#ifndef FBL_USE_SSL
#	define FBL_USE_SSL 1 
#endif // FBL_USE_SSL


#pragma mark -


/**********************************************************************************************/
#if FBL_WIN && _MSC_VER < 1900
#define snprintf _snprintf
#endif // FBL_WIN


/**********************************************************************************************/
// 'Logs' folder location
//
#define	LOG_FOLDER_NAME "vlogs"

/**********************************************************************************************/
// Turn ON/OFF clang's "Thread Safety Analysis".
// Please note that it OFFs STD_THREADING_SAFE (since we can not add attributes to std...)
// Probably we need something better or just compile FBL_THREAD_SAFETY_ANALYSIS builds periodically
// just to make sure there are no new warnings.
//
#define FBL_THREAD_SAFETY_ANALYSIS 0


/**********************************************************************************************/
// - fatal error C1189: #error :  <atomic> is not supported when compiling with /clr or /clr:pure.
//   [C:\Jenkins\workspace\VDB_DEVELOP\V4NET\visual\V4NET.vcxproj]
// - Mac's V4MD
// - Mac's V4RB
// - Mac's VODBC
// - Mac's VPHP
//
#define STD_THREADING_SAFE (!FBL_THREAD_SAFETY_ANALYSIS && !FBL_WIN && !FBL_V4MD && !FBL_V4RB && !FBL_VODBC && !VPHP_DLL)

/**********************************************************************************************/
#if STD_THREADING_SAFE
	#define StLockGuard std::unique_lock
	#define WAIT_FOR( cond_var, guard, timeout ) \
		(cond_var.wait_for( guard, std::chrono::milliseconds(timeout) ) == std::cv_status::no_timeout)
#else
	#define StLockGuard StLockGuard_T
	#define WAIT_FOR( cond_var, guard, timeout ) \
		(cond_var.wait_for( guard, timeout ))
#endif // STD_THREADING_SAFE


/**********************************************************************************************/
// Macro, to simplify syntax of lock of mutex on Valentina Engine.
//
/*
#if FBL_EXTERNAL_THREADSAFE
#define FBL_ENGINE_LOCK\
	StLockGuard<True_Thread_Mutex_Recursive> gEngineLock(GetGlobalEngineLock(), (gKernelInServerMode || gGlobalThreadSafe) );
#else
#define FBL_ENGINE_LOCK
#endif // FBL_EXTERNAL_THREADSAFE
*/
        
#if FBL_EXTERNAL_THREADSAFE
#define FBL_ENGINE_LOCK				StGlobalLockGuard gEngineLock;
#define FBL_ENGINE_UNLOCK			StGlobalUnlockGuard gEngileUnlock;
#else
#define FBL_ENGINE_LOCK
#define FBL_ENGINE_UNLOCK
#endif // FBL_EXTERNAL_THREADSAFE

// Just to see that it is intentionally NO engine lock.
#define FBL_ENGINE_NO_LOCK

#if FBL_MT_REINDEX
#define FBL_GLOBAL_DISK_LOCK\
	StCriticalSectionLocker gDiskLock(GetGlobalDiskLock(), true);
#endif // FBL_MT_REINDEX

#define FBL_LOG_FOLDER_CREATION_LOCK\
	StLockGuard<True_Thread_Mutex_Recursive> gLogFolderCreationLock(GetLogFolderCreationLock());

#define FBL_LOG_TIMESTAMP_LOCK\
	StLockGuard<True_Thread_Mutex_Recursive> gLogTimeStampLock(GetLogTimeStampLock());

#define FBL_WARNING_LOG_LOCK\
	StLockGuard<True_Thread_Mutex_Recursive> gWarningLogLock(GetWarningLogLock());

#define FBL_TUNES_LOG_LOCK\
	StLockGuard<True_Thread_Mutex_Recursive> gTunesLogLock(GetTunesLogLock());

#define FBL_REPORTS_LOG_LOCK\
	StLockGuard<True_Thread_Mutex_Recursive> gReportsLogLock(GetReportsLogLock());

#if __CHECK
#define FBL_ASSERT_LOG_LOCK\
	StLockGuard<True_Thread_Mutex_Recursive> gAssertLog(GetAssertLogLock());
#define FBL_TEST_STATISTIC_LOCK\
	StLockGuard<True_Thread_Mutex_Recursive> gTestStatisticLog(GetTestStatisticLock());
#endif // __CHECK


#define FBL_TASK_MANAGER_LOCK\
	StLockGuard<True_Thread_Mutex_Recursive> gTaskManagerLock(GetTaskManagerLock());

#define FBL_NOTIFICATION_CENTER_LOCK\
	StLockGuard<True_Thread_Mutex_Recursive> gNotificationCenterLock(GetNotificationCenterLock());


/**********************************************************************************************/
// Macro to simplify syntax of lock of some named mutex.
//
#define FBL_LOCK(mutex)\
	StLockGuard<Thread_Mutex_Recursive> lock_##mutex(mutex);

/**********************************************************************************************/
// Swallow any exception in destructors to avoid terminating, since:
// 1. Destructors are not-throwing implicitely (C++ 2011);
// 2. Most (all) of Valentina objects are used under SmartPtr (stack object)
//    So destructor is called as result of stack unwinding.
//
#define SWALLOW_CATCH \
catch(...) {}

/**********************************************************************************************/
#endif // _FBL_Macros_h
