/**********************************************************************************************/
/* VSQL_Globals.cpp                                                     					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/VSQL/Headers/VSQL_Globals.h>
#include <VShared/VSQL/Interfaces/VSQL_I_SqlDatabase.h>


/**********************************************************************************************/
VSQL_Begin_Namespace


/**********************************************************************************************/
CreateSqlDatabase_PTR	gpCreateSqlDatabase		  = nullptr;
OnValentinaShutDown_PTR gpOnValentinaShutDown_PTR = nullptr;


#pragma mark -

/**********************************************************************************************/
void SetCreateDatabaseFunction( CreateSqlDatabase_PTR inpFnc )
{
	gpCreateSqlDatabase = inpFnc;
}


/**********************************************************************************************/
I_SqlDatabase_Ptr CreateLocalSQLDatabase( EStorageType 	inStorage, bool	inUseOwnCache )
{
	return (gpCreateSqlDatabase) ? gpCreateSqlDatabase(inStorage, inUseOwnCache) : nullptr;
}


#pragma mark -

/**********************************************************************************************/
void SetOnValentinaShutDownFunction( OnValentinaShutDown_PTR inpFnc )
{
	gpOnValentinaShutDown_PTR = inpFnc;
}


/**********************************************************************************************/
void OnValentinaShutDown( void )
{
	if( gpOnValentinaShutDown_PTR )
		gpOnValentinaShutDown_PTR();
}


/**********************************************************************************************/
VSQL_End_Namespace
