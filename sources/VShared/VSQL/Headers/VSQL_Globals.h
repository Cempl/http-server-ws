/**********************************************************************************************/
/* VSQL_Globals.h																			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _VSQL_Globals_h
	#define _VSQL_Globals_h
#pragma once

#include <VShared/VSQL/Headers/VSQL.h>


/**********************************************************************************************/
VSQL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE(I_SqlDatabase);


/**********************************************************************************************/
// Trick
// We need to registry some callbacks in A.dll (vShared.dll)
// but that callbacks are implemented in B.dll (vKernel.dll)
// in order to make possible call it from C.dll (vReport.dll)
// without introducing B.dll - C.dll dependence.
//
// In other words C.dll (vReport.dll) depends on A.dll (vShared.dll)
// but not stricktly required B.dll (vKernel.dll).


#pragma mark -


/**********************************************************************************************/
//
// Local sql-database factory
//
/**********************************************************************************************/
typedef I_SqlDatabase_Ptr (*CreateSqlDatabase_PTR)( EStorageType inStorage, bool inUseOwnCache );
extern CreateSqlDatabase_PTR gpCreateSqlDatabase;


/**********************************************************************************************/
// vKernel's callback register
//
FBL_SHARED_EXP void SetCreateDatabaseFunction( CreateSqlDatabase_PTR inpFnc );


/**********************************************************************************************/
// Actual call of registered vKernel callback
//
FBL_SHARED_EXP I_SqlDatabase_Ptr CreateLocalSQLDatabase( EStorageType inStorage, bool inUseOwnCache = false );


#pragma mark -


/**********************************************************************************************/
//
// Remote sql-database factory
//
/**********************************************************************************************/
// Not implemented because it is not required yet.
// Call vClient::CreateSqlDatabase( I_Connection_Ptr inConnection ) directly
//


#pragma mark -


/**********************************************************************************************/
//
// Local valentina shutdown callback (actually used by vKernel only - so no need in this callback)
//
/**********************************************************************************************/
typedef void (*OnValentinaShutDown_PTR)( void );
extern OnValentinaShutDown_PTR gpOnValentinaShutDown_PTR;


/**********************************************************************************************/
// vKernel's callback register
//
FBL_SHARED_EXP void SetOnValentinaShutDownFunction( OnValentinaShutDown_PTR inpFnc );


/**********************************************************************************************/
// Actual call of the registered vKernel callback
//
FBL_SHARED_EXP void OnValentinaShutDown( void );


/**********************************************************************************************/
VSQL_End_Namespace


/**********************************************************************************************/
#endif // _VSQL_Globals_h


