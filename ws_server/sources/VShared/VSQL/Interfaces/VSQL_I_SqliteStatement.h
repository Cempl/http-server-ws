/**********************************************************************************************/
/* VSQL_I_SqliteStatement.h																	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _VSQL_I_SqliteStatement_h
	#define _VSQL_I_SqliteStatement_h
#pragma once

#include <VShared/VSQL/Headers/VSQL.h>

// FBL:
#include <VShared/FBL/publ/Interfaces/FBL_I_Disk_Location.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Value.h>
#include <VShared/FBL/publ/Util_classes/FBL_String.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
VSQL_Begin_Namespace


/**********************************************************************************************/
// forward declarations:
//
SMART_INTERFACE(I_SqliteDatabase);
SMART_INTERFACE(I_SqliteCursor);
//
SMART_CLASS(SqliteQueryResult);


/**********************************************************************************************/
// This file forward declarations:
//
SMART_INTERFACE(I_SqliteStatement);


/**********************************************************************************************/
// Interface, which is wrapper to SQLite statement object.
//
// Dependency:
//		I_SqliteDatabase -> I_SqliteStatement -> I_SqliteCursor
//
//StubInterface
interface FBL_SHARED_EXP_CLASS I_SqliteStatement : public I_Unknown
{
	public://////////////////////////////////////////////////////////////////////////

virtual							~I_SqliteStatement( void );


	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// Properties:

		// <OriginalQuery> [r/o]

								/// Returns the text of "original" sql query "as is".
virtual	const String&			get_OriginalQuery( void ) const = 0;

		// <Query> [r/o]
								/// Returns the text of "actual" sql query.
								/// Actual means - original one before Prepare() call and prepared one
								/// otherwise.
								/// Example:
								///		"INSERT INTO t1 VALUES (1); SELECT * FROM t1;"
								/// After Prepare():
								///		get_Query() returns "INSERT INTO t1 VALUES (1);"
								///		get_UnusedQuery() returns "SELECT * FROM t1;"
								///
virtual	const String&			get_Query( void ) const = 0;

		// <UnusedQuery> [r/o]
								/// Returns the text of unused sql query.
virtual	const String&			get_UnusedQuery( void ) const = 0;

		// <CurrentStatus> [r/o]
								/// Returns the current status of inStatus param.
virtual vint32 					get_CurrentStatus( vint32 inStatus ) = 0;

								/// Returns SqliteDatabase this SqliteStatement belongs to.
virtual	I_SqliteDatabase_Ptr	get_SqliteDatabase( void ) const = 0;


	public://///////////////////////////////////////////////////////////////////////////////////

	// -------------------------
    // Prepare Methods:
								/// Prepare statement.
virtual void					Prepare( void ) = 0;

								/// Prepares and Executes a single statement from the whoole query.
								///	Returns TRUE if unused part of the query remains.
virtual bool					PrepareAndExecuteOneCommand(void) = 0;

								/// Prepares and Executes all statements in the query.
virtual void					PrepareAndExecuteAllCommands(void) = 0;


	// -------------------------
    // Bind Methods:
								/// Bind values to the statement.
virtual void					SqlBind(
									const ArrayOfValues_Ptr inBindings = nullptr )  = 0;

								/// Bind multi-row values to the statement.
virtual void					SqlMultiRowBind(
									const ArrayOfValueArrays_Ptr inBindings = nullptr ) = 0;


	// -------------------------
    // Query Methods:
								/// Execute SQL statement of any kind except SELECT.
                                /// Returns the last inserted row id.
virtual vint64					SqlExecute( void )  = 0;

								/// Execute SQL statement of SELECT kind.
                                /// Returns a cursor.
                                /// You MUST CloseCursor() when finish to work with it.
virtual I_SqliteCursor_Ptr		SqlSelect( ECursorLocation inCursorLocation = kClient )  = 0;

								/// Execute SQL statement, of SELECT kind.
                                /// Returns result records as a JSON string.
virtual String					SqlSelectAsJSON( void ) = 0;


								/// Execute SQL statement of any kind.
                                /// This method is combination of SqlExecute() and SqlSelect().
                                /// Returns either cursor or the last inserted row id.
                                /// Effective to work with remote Valentina SQLite Server.
virtual SqliteQueryResult_Ptr	SqlQuery( ECursorLocation inCursorLocation = kClient ) = 0;



	// -------------------------
    // Cleanup Methods:
								/// Free cursor, which belongs to this statement.
virtual void					CloseCursor( I_SqliteCursor_Ptr inpCursor ) = 0;

};


/**********************************************************************************************/
VSQL_End_Namespace


#include <VShared/FBL/publ/Headers/FBL_post_header.h>


#endif // _VSQL_I_SqliteStatement_h
