/**********************************************************************************************/
/* VSQL_Instances.h																			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/VSQL/Interfaces/VSQL_I_ClientInfo.h>
#include <VShared/VSQL/Interfaces/VSQL_I_Cursor.h>

#include <VShared/VSQL/Interfaces/VSQL_I_DatabaseInfo.h>
#include <VShared/VSQL/Interfaces/VSQL_I_DataSource.h>
#include <VShared/VSQL/Interfaces/VSQL_I_DataSourceCursor.h>

#include <VShared/VSQL/Interfaces/VSQL_I_VProject.h>
#include <VShared/VSQL/Interfaces/VSQL_I_VReport.h>

#include <VShared/VSQL/Interfaces/VSQL_I_Server.h>
#include <VShared/VSQL/Interfaces/VSQL_I_SqlConnection.h>
#include <VShared/VSQL/Interfaces/VSQL_I_SqlDatabase.h>

#include <VShared/VSQL/Interfaces/VSQL_I_SqliteDatabase.h>
#include <VShared/VSQL/Interfaces/VSQL_I_SqliteStatement.h>
#include <VShared/VSQL/Interfaces/VSQL_I_SqliteCursor.h>


/**********************************************************************************************/
VSQL_Begin_Namespace


/**********************************************************************************************/
I_ClientInfo::~I_ClientInfo( void )							{}
I_Cursor::~I_Cursor( void )									{}

I_DatabaseInfo::~I_DatabaseInfo( void )						{}
I_DataSource::~I_DataSource( void )							{}
I_DataSourceCursor::~I_DataSourceCursor( void )				{}

I_VProject::~I_VProject( void )								{}
I_VReport::~I_VReport( void )								{}

I_Server::~I_Server( void )									{}
I_SqlConnection::~I_SqlConnection( void )					{}
I_SqlDatabase::~I_SqlDatabase( void )						{}

I_SqliteDatabase::~I_SqliteDatabase( void )					{}
I_SqliteStatement::~I_SqliteStatement( void )				{}
I_SqliteCursor::~I_SqliteCursor( void )						{}


/**********************************************************************************************/
VSQL_End_Namespace


