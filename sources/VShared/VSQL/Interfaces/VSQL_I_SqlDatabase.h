/**********************************************************************************************/
/* VSQL_I_SqlDatabase.h                                                       				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _VSQL_I_Database_h 
	#define _VSQL_I_Database_h
#pragma once

#include <VShared/VSQL/Headers/VSQL.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_Database.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Value.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_Connection.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
VSQL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE(I_SqlDatabase);
SMART_INTERFACE(I_SqlStatement);
SMART_INTERFACE(I_Cursor);

SMART_CLASS(QueryResult);

class SqlPlanPool;


/**********************************************************************************************/
// Provides functions to execute SQL queries for a Valentina database.
//
// Implementation of this interface should be a Database object  
// that is inherited from FBL::I_Database or that aggregates it. 
// Important is that user must be able to obtain FBL::I_Database interface
// from the SQL database object.
//
//StubInterface
interface FBL_SHARED_EXP_CLASS I_SqlDatabase : public I_Unknown
{
	 	
virtual 					~I_SqlDatabase( void );


	// ---------------------
    // Statement Methods: (new in v7.0)
    //
    // The best way to work with SQL command that have SQL binding and batch execution.
    // It can be x3-x4 times faster than below methods, which use pool of SQL commands with binding.

virtual I_SqlStatement_Ptr	CreateStatement( const String& inQuery ) = 0;

virtual	void				RemoveStatement( I_SqlStatement& inStatement ) = 0;

virtual I_SqlStatement*		get_Statement( vuint32 inStatementIndex ) = 0;
virtual vuint32				get_StatementCount( void ) const = 0;


	// ---------------------
	// SQL Methods:
    //
    // Historically they are older than above methods with SqlStatement.
    // These methods also can work with SQL commands that has SQL binding.
    // Such commands are stored in the special internal pool.

							/** Executes SQL SELECT or SHOW commands for database.
								Returns I_Cursor as result of the query.
								Throws VSQL::xException on error. */
virtual	I_Cursor_Ptr		SqlSelect( 
								const String& 		inQuery,								
								ECursorLocation 	inCursorLocation = kClient,
								ELockType 			inLockType = kReadOnly,
								ECursorDirection 	inCursorDirection = kForwardOnly,
								const ArrayOfValues_Ptr inBindings = nullptr ) = 0;

							/** Executes any SQL command except SELECT.
								Returns the number of affected records.
								Throws VSQL::xException on error. */
virtual	vuint32				SqlExecute( 
								const String&			inCommand,
								const ArrayOfValues_Ptr inBindings = nullptr) = 0;
					
							/** SqlExecute with array of value arrays.  */
virtual	vuint32				SqlExecute( 
                            	const String&			inCommand,
                            	const ArrayOfValueArrays_Ptr inBindings ) = 0;

							/** Executes any SQL command.
								Returns the object contained cursor or affected records'
								count depends on query matter.
								Throws VSQL::xException on error. */
virtual	QueryResult_Ptr		 SqlQuery(	
								const String& 			inQuery,								
								ECursorLocation 		inCursorLocation = kClient,
								ELockType 				inLockType = kReadOnly,
								ECursorDirection 		inCursorDirection = kForwardOnly,
								const ArrayOfValues_Ptr inBindings = nullptr ) = 0;

							/** Executes any SQL command.
								Returns the object contained cursor or affected records'
								count depends on query matter.
								
                                inFlags (see QueryOptions) extends SqlQuery() functionality.
                             	You can specify if produce warning, tune info, output info, etc.
                             
								Throws VSQL::xException on error. */
virtual	QueryResult_Ptr		 SqlQueryEx(	
								const String& 			inQuery,								
								const vuint16			inFlags,
								ECursorLocation 		inCursorLocation = kClient,
								ELockType 				inLockType = kReadOnly,
								ECursorDirection 		inCursorDirection = kForwardOnly,
								const ArrayOfValues_Ptr inBindings = nullptr ) = 0;


	// ---------------------
	// Cursor Methods:

							// returns the count of cursors for this db.
virtual vuint32				get_CursorCount( void ) const = 0;
								
virtual I_Cursor*			get_Cursor( vuint32 inIndex ) const = 0;

							// Unregister the specified cursor.
							// Technically need to be here (Because VC_Cursor use it).
virtual	void				RemoveCursor( I_Cursor& inCursor ) = 0;


	// ---------------------
	// TEMPORARY, until we get QueryInterface().

virtual	FBL::I_Database_Ptr	get_BaseDatabase( void ) = 0;

}; 


/**********************************************************************************************/
VSQL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _VSQL_I_Database_h
