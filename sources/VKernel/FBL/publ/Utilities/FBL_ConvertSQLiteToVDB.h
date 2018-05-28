/**********************************************************************************************/
/* VSQL_ConvertSQLiteToVDB.h                                            					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017 															  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_ConvertSQLiteToVDB_h 
	#define _FBL_ConvertSQLiteToVDB_h
#pragma once

#include <VShared/VSQL/Interfaces/VSQL_I_SqliteDatabase.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
FBL_EXP void ConvertSQLiteToVDB( VSQL::I_SqliteDatabase_Ptr	inLiteDb, I_Location_Ptr inVDB );


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_ConvertSQLiteToVDB_h
