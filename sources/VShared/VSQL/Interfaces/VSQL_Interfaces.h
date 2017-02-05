/**********************************************************************************************/
/* VSQL_Interfaces.h															 			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015 															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

// This file include all VSQL/publ/Interfaces.
//
// This allow to developer inlude only this one file.
// This is a good idea because developer sources will not depend on possible changes in the 
// Intefaces folder.

#ifndef _VSQL_Interfaces_h 
	#define _VSQL_Interfaces_h
#pragma once


/**********************************************************************************************/
#include <VShared/VSQL/Interfaces/VSQL_I_ClientInfo.h>
#include <VShared/VSQL/Interfaces/VSQL_I_Cursor.h>

#include <VShared/VSQL/Interfaces/VSQL_I_DatabaseInfo.h>

#include <VShared/VSQL/Interfaces/VSQL_I_Server.h>
#include <VShared/VSQL/Interfaces/VSQL_I_SqlConnection.h>
#include <VShared/VSQL/Interfaces/VSQL_I_SqlDatabase.h>

#include <VShared/VSQL/Interfaces/VSQL_I_VProject.h>
#include <VShared/VSQL/Interfaces/VSQL_I_VReport.h>

#include <VShared/VSQL/Interfaces/VSQL_I_SqliteDatabase.h>
#include <VShared/VSQL/Interfaces/VSQL_I_SqliteStatement.h>
#include <VShared/VSQL/Interfaces/VSQL_I_SqliteCursor.h>


/**********************************************************************************************/
#endif // _VSQL_Interfaces_h
