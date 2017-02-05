/**********************************************************************************************/
/* FBL_Globals.h																			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_Globals_h
	#define _FBL_Globals_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// forward declarations: 
//
SMART_INTERFACE( I_Location );
SMART_INTERFACE( I_StringFactory );


/**********************************************************************************************/
//									GLOBAL FUNCTIONS:
/**********************************************************************************************/

/**********************************************************************************************/
FBL_SHARED_EXP bool IsKernelDLLAvailable( bool inTryAgain = false );
FBL_SHARED_EXP bool IsClientDLLAvailable( bool inTryAgain = false );


/**********************************************************************************************/
FBL_SHARED_EXP extern bool gKernelInServerMode;


/**********************************************************************************************/
//									Version constants:
/**********************************************************************************************/
FBL_SHARED_EXP extern const vuint32 gKernelVersionMajor;
FBL_SHARED_EXP extern const vuint32 gKernelVersionMinor;
FBL_SHARED_EXP extern const vuint32 gKernelVersionRevision;
FBL_SHARED_EXP extern const vuint32 gKernelVersionMinorAndRevision;

FBL_SHARED_EXP extern const vuint32 gKernelVersionStage; // See <MacTypes.h> for definitions of Stage
FBL_SHARED_EXP extern const vuint32 gKernelVersionInternalRelease;
FBL_SHARED_EXP extern const char* 	gKernelVersionString;

FBL_SHARED_EXP extern const vuint32 gKernelVersion;


//----------------------------------------------------------------------------------------------
FBL_SHARED_EXP extern const bool  gEraseInformation;


//----------------------------------------------------------------------------------------------
FBL_SHARED_EXP const char* GetKernelVersionString( void );


/**********************************************************************************************/
// Private copy constructor and copy assignment ensure classes derived from
// class noncopyable cannot be copied.
//
class FBL_SHARED_EXP_CLASS noncopyable
{
	protected:////////////////////////////////////////////////////////////////////////////////// 

					    	noncopyable(){}
virtual				    	~noncopyable(){}

	private:////////////////////////////////////////////////////////////////////////////////////
	 
						    noncopyable( const noncopyable& );
						    const noncopyable& operator=( const noncopyable& );
};


//----------------------------------------------------------------------------------------------
// Identificator's restrictions:
//----------------------------------------------------------------------------------------------
const tslen gNameLength = 128;	// Max length for names (tables, fields, and so on)


/**********************************************************************************************/
FBL_SHARED_EXP extern const vuint16 gFloatDefaultPrecision;
FBL_SHARED_EXP extern const vuint16 gFloatDefaultScale;
FBL_SHARED_EXP extern const vuint16 gDoubleDefaultPrecision;
FBL_SHARED_EXP extern const vuint16 gDoubleDefaultScale;

/**********************************************************************************************/
FBL_SHARED_EXP extern const vint64 gSequenceDefaultMinValue;
FBL_SHARED_EXP extern const vint64 gSequenceDefaultMaxValue;
FBL_SHARED_EXP extern const vint64 gSequenceDefaultIncrementValue;
FBL_SHARED_EXP extern const bool   gSequenceDefaultIsCycled;

/**********************************************************************************************/
class String;
FBL_SHARED_EXP vint16 StringCompare( 
	const String&	inStr1, 
	const String&	inStr2, 
	bool			inCaseSensitive );

FBL_SHARED_EXP vint16 StringCompare( 
   const String&	inStr1, 
   const UChar*		inStr2, 
   tslen 			inLength,
   bool				inCaseSensitive );


#pragma mark -

/**********************************************************************************************/
SMART_INTERFACE( I_ValueFormater );

/**********************************************************************************************/
typedef I_ValueFormater_Ptr (*CreateValueFormater_PTR)( 
	const String*		inPattern,
	const EFormat		inFormat,
	const String*		inpLocale );

extern CreateValueFormater_PTR gpCreateValueFormater;

/**********************************************************************************************/
FBL_SHARED_EXP void SetCreateValueFormaterFunction( CreateValueFormater_PTR inpFnc );


/**********************************************************************************************/
FBL_SHARED_EXP extern const vuint16 gOptimalThreadCountPerProcessor;

/**********************************************************************************************/
// Return count of available processors in the system. 
// -1 means unknown. 
FBL_SHARED_EXP int GetProcessorCount( void );

/**********************************************************************************************/
// Return optimal thread count for this system.
//
FBL_SHARED_EXP int GetOptimalThreadCount( void );


/**********************************************************************************************/
//									TLS Vars:
/**********************************************************************************************/
#if FBL_INTERNAL_THREADSAFE || FBL_EXTERNAL_THREADSAFE

	SMART_INTERFACE( I_Localizable );

	#if FBL_POOLED_LOCALIZABLE_SYS
		template<class T> class TLS_Pooled_Var;
		FBL_SHARED_EXP extern TLS_Pooled_Var<I_Localizable>			tlsSysLocalizable;
	#else
		FBL_SHARED_EXP extern TLS_I_Unknown_Var<I_Localizable>		tlsSysLocalizable;
	#endif // FBL_POOLED_LOCALIZABLE_SYS

#endif // FBL_INTERNAL_THREADSAFE || FBL_EXTERNAL_THREADSAFE


/**********************************************************************************************/
//								Dynamic ThreadSafe Vars:
/**********************************************************************************************/
FBL_SHARED_EXP extern bool   gGlobalThreadSafe;


/**********************************************************************************************/
//									Callbacks:
/**********************************************************************************************/
// Callback for a "Yield" function (required to be called from "heavy" kernel functions
// to make context switching within cooperative multitasling model like RealBasic)
//
typedef	void (*YieldFuncPtr)( void );
FBL_SHARED_EXP extern YieldFuncPtr pYieldFunction;


/**********************************************************************************************/
//									Notification Queue:
/**********************************************************************************************/
const size_t gDefaultNQSize					= 20;


/**********************************************************************************************/
//									vSqlite:
/**********************************************************************************************/
const int gvSqlite_default_MaxRetryCount		= 10;
const int gvSqlite_default_BusySleepTime_ms		= 50;

FBL_SHARED_EXP extern const char* gvSqlite_journal_sfx;
FBL_SHARED_EXP extern const char* gvSqlite_wal_sfx;
FBL_SHARED_EXP extern const char* gvSqlite_shm_sfx;



/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // _FBL_Globals_h


