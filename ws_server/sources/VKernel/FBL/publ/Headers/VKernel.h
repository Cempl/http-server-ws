/**********************************************************************************************/
/* VKernel.h																	 			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _VKernel_h
	#define _VKernel_h
#pragma once

// STD
#include <string>

// VSHARED
#include <VShared/FBL/publ/Interfaces/FBL_I_Database.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Connection.h>

// VSHARED/VSQL:
#include <VShared/VSQL/Headers/VSQL.h>
#include <VShared/VSQL/Interfaces/VSQL_I_SqlConnection.h>
#include <VShared/VSQL/Interfaces/VSQL_I_SqliteDatabase.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// Must be called to init Valentina kernel: 
// it creates static members, load Valentina plugins and then create cache.
// If you specify the serial numbers as NULL, then kernel will work 10 minutes.
// Returns the actual size of cache it was able to allocate. 
// On the second potential call, it just returns the size of existed cache.
//
FBL_EXP vuint32				ValentinaInit( 
								vuint32			inCacheSize = 9 * 1024 * 1024, 	// in bytes.
								const char*		inMacSN = nullptr,		// your MacOS serial number.
								const char*		inWinSN = nullptr,		// your Windows serial number.
								const char*		inUnixSN = nullptr );	// your Unix serial number.

// Must be called to finish work with Valentina lernel.
//
FBL_EXP void				ValentinaShutDown( void );


/**********************************************************************************************/
// These functions are needed to resolve ambiguity when linking to both
// (client & local) DLLs at the same time. They are just wrappers to above ValentinaInit() 
// and ValentinaShutDown
//
// VClient.h have methods ValentinaInitC() and ValentinaShutDownC();
//
FBL_EXP vuint32				ValentinaInitL(
								vuint32			inCacheSize,
								const char*		inMacSN = nullptr,
								const char*		inWinSN = nullptr );

FBL_EXP void				ValentinaShutDownL( void );


/**********************************************************************************************/
// Helpers
void CreateCaches( vuint32 inCacheSize );

void ProcessSerial(
	const char* inMacSN,
	const char* inWinSN,
	const char* inUnixSN );

void RegisterAggregativeFunctions( void );


/**********************************************************************************************/
// Check serials

FBL_EXP bool				Valentina_CheckSerial(
								const char*		inKey );

FBL_EXP bool				Valentina_CheckSerial(
								const char*		inKey,
								std::string*	outUserName,
								std::string*	outPlatform, 
								vuint16*		outDay,
								vuint16*		outMonth,
								vuint16*		outYear );

FBL_EXP bool				Valentina_CheckSerial(
								const char*		inKey,
								FBL::String*	outUserName,
								FBL::String*	outPlatform, 
								vuint16*		outDay,
								vuint16*		outMonth,
								vuint16*		outYear );



/**********************************************************************************************/
// Factory of FBL::I_Database. 
//
FBL_EXP	FBL::I_Database_Ptr	CreateDatabase( 
								EStorageType 	inStorage = FBL::kStorage_Disk,
								bool			inUseOwnCache = false );


/**********************************************************************************************/
FBL_End_Namespace


#pragma mark -


/**********************************************************************************************/
VSQL_Begin_Namespace

SMART_INTERFACE(I_SqlDatabase);

// Factory of FBL::I_SqlDatabase.
//
FBL_EXP I_SqlDatabase_Ptr 	CreateSqlDatabase(	
								EStorageType 	inStorage = kStorage_Disk,
								bool			inUseOwnCache = false );

FBL_EXP I_SqliteDatabase_Ptr CreateSqliteDatabase(
								I_Disk_Location_Ptr inLocation,
								int					inMaxRetryCount = gvSqlite_default_MaxRetryCount,
								int					inBusySleepTime_ms = gvSqlite_default_BusySleepTime_ms );

FBL_EXP I_SqlConnection_Ptr	GetLocalConnection( void );


/**********************************************************************************************/
FBL_EXP void VSQL_OnValentinaShutDown( void );


/**********************************************************************************************/
VSQL_End_Namespace


/**********************************************************************************************/
#endif // _VKernel_h


