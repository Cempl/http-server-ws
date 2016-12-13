/**********************************************************************************************/
/* VSQL_I_SqlConnection.h																	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef __VSQL_I_SqlConnection_h
	#define __VSQL_I_SqlConnection_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Util_classes/FBL_String.h>

// VSQL:
#include <VShared/VSQL/Headers/VSQL.h>
#include <VShared/VSQL/Database/VSQL_QueryResult.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Connection.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
VSQL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE(I_SqlConnection);


/**********************************************************************************************/
/**	Interface I_SqlConnection provides you with ability to make sql queries. 
 */

//StubInterface
interface FBL_SHARED_EXP_CLASS I_SqlConnection : public I_Unknown
{
virtual						~I_SqlConnection( void );

	// --------------------
	// SQL-Server

							/** Executes SQL SELECT command for a database.
								Returns Cursor as the result of query.
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
					

virtual	QueryResult_Ptr		SqlQuery(	
								const String& 			inQuery,								
								ECursorLocation 		inCursorLocation = kClient,
								ELockType 				inLockType = kReadOnly,
								ECursorDirection 		inCursorDirection = kForwardOnly,
								const ArrayOfValues_Ptr inBindings = nullptr ) = 0;

							/** Executes any SQL command.
								Returns the object contained cursor or affected records'
								count depends on query matter.
								inFlags (see QueryOptions) extends SqlQuery() functionality.
								Throws VSQL::xException on error. */
virtual	QueryResult_Ptr		 SqlQueryEx(	
								const String& 			inQuery,								
								const vuint16			inFlags,
								ECursorLocation 		inCursorLocation = kClient,
								ELockType 				inLockType = kReadOnly,
								ECursorDirection 		inCursorDirection = kForwardOnly,
								const ArrayOfValues_Ptr inBindings = nullptr ) = 0;

virtual void				put_ActiveDatabase( I_SqlDatabase_Ptr inpDatabase ) = 0;

virtual I_SqlDatabase_Ptr	get_ActiveDatabase( void ) = 0;

		// vSqlite specific methods.
		//

virtual String				get_SqliteLibVersion(void) const = 0;

virtual	vuint16				get_SqliteDatabaseCount( void ) const = 0;

virtual String				get_SqliteDatabaseName( vuint16 inIndex ) = 0;

virtual vint32 				get_CurrentStatus(vint32 inStatus) = 0;

virtual vint32 				get_HighestStatus(vint32 inStatus) = 0;

};


/**********************************************************************************************/
VSQL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif //__VSQL_I_SqlConnection_h

