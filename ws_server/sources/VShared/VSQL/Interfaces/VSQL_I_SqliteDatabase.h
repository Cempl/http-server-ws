/**********************************************************************************************/
/* VSQL_I_SqliteDatabase.h 		                                                   			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _VSQL_I_SqliteDatabase_h
	#define _VSQL_I_SqliteDatabase_h
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
SMART_INTERFACE(I_SqliteStatement);


/**********************************************************************************************/
// THIS module forward declarations:
//
SMART_INTERFACE(I_SqliteDatabase);


/**********************************************************************************************/
// Sqlite event structure (see Sqlite Authorizer Action Codes).
//
struct SqliteEvent
{
	vuint32		mEventID;		// like SQLITE_ALTER_TABLE, SQLITE_CREATE_INDEX ...
	String		mParam1;		// meaning depends on mEventID
	String		mParam2;		// meaning depends on mEventID
};


/**********************************************************************************************/
SMART_ARRAY_OF_CLASSES( ArrayOfSqliteEvents, SqliteEvent );


/**********************************************************************************************/
//
//StubInterface
interface FBL_SHARED_EXP_CLASS I_SqliteDatabase : public I_Unknown
{
	public://////////////////////////////////////////////////////////////////////////

virtual						~I_SqliteDatabase( void );


	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// Properties:

		// <AffectedRecords> [r/o]
							/// Returns affected record count after the last INSERT/UPDATE/DELETE.
virtual vint64 				get_AffectedRecords( void ) = 0;

		// <CurrentStatus> [r/o]
							/// Returns the current status of inStatus param.
virtual vint32 				get_CurrentStatus( vint32 inStatus ) = 0;

		// <HighestStatus> [r/o]
							/// Returns the highest status of inStatus param.
virtual vint32 				get_HighestStatus( vint32 inStatus ) = 0;

		// <ConnectionCount> [r/o]
							/// Returns connection count to this db.
virtual	vuint32				get_ConnectionCount( void ) const = 0;

		// <StatementCount> [r/o]
							/// Returns statement count.
virtual vuint32				get_StatementCount( void ) = 0;

		// <LastInsertedRowID> [r/o]
							/// Returns the last inserted rowID.
virtual vint64 				get_LastInsertedRowID( void ) = 0;

		// <Exists> [r/o]
							/// Returns TRUE if the db exists.
virtual bool				get_Exists(void) const = 0;

		// <IsOpen> [r/o]
							/// Returns TRUE if the db is open.
virtual bool				get_IsOpen(void) = 0;

		// <IsReadOnly> [r/o]
							/// Returns TRUE if the db is read-only.
virtual bool				get_IsReadOnly( void ) = 0;

		// <Name> [r/o]
							/// Returns the name of database. In fact this is the name of db file.
virtual	const String&		get_Name( void ) const = 0;

		// <EnableSharedCache> [r/w]
							/// Set EnableSharedCache value.
virtual void				put_EnableSharedCache( bool inValue ) = 0;

							/// Returns EnableSharedCache value.
virtual bool				get_EnableSharedCache( void ) = 0;

		// <EnableSqliteEvents> [r/w]
							/// Set EnableSqliteEvents value.
virtual void				put_EnableSqliteEvents( bool inValue ) = 0;

							/// Returns EnableSqliteEvents value.
virtual bool				get_EnableSqliteEvents( void ) = 0;


	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
    // DB File Methods:

							/// Creates a new database.
virtual void				Create( void ) = 0;

							/// Opens an existed database.
virtual void				Open( void ) = 0;

							/// Closes the database.
virtual void				Close( void ) = 0;

							/// Drops the database.
                            /// Database should be in the closed state.
                            /// If this database is on VServer, any connection should not use it.
virtual void				ThrowOut( void ) = 0;

virtual void				Backup( I_Location_Ptr inpBackupFolder ) = 0;


	// ---------------------
    // Statement Methods:

virtual I_SqliteStatement_Ptr CreateStatement( const String& inQuery ) = 0;

virtual void				CloseStatement( I_SqliteStatement_Ptr inpStatement ) = 0;

							// Returns statement by index.
virtual I_SqliteStatement_Ptr get_Statement( vuint32 inStatementIndex ) = 0;

							// Execute all the commands in inQuery directly.
virtual void				SqlExecute( const String& inQuery ) = 0;

							// Return results of sql "PRAGMA ..." for each item in inPragmas.
virtual ArrayOfValues_Ptr	GetPragmaValues( const ArrayOfStrings_Ptr inPragmas ) = 0;

	// ---------------------
    // Sqlite Event Methods:

virtual ArrayOfSqliteEvents_Ptr	GetLastSqliteEvents( void ) = 0;

};


/**********************************************************************************************/
VSQL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _VSQL_I_SqliteDatabase_h
