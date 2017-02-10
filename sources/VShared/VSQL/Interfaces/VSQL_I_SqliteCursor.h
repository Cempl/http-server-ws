/**********************************************************************************************/
/* VSQL_I_SqliteCursor.h																	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _VSQL_I_SqliteCursor_h
	#define _VSQL_I_SqliteCursor_h
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
SMART_INTERFACE(I_SqliteCursor);
SMART_INTERFACE(I_SqliteStatement);


/**********************************************************************************************/
//
//StubInterface
interface FBL_SHARED_EXP_CLASS I_SqliteCursor : public I_Unknown
{
	public://////////////////////////////////////////////////////////////////////////

virtual							~I_SqliteCursor( void );


	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// Properties:

		// <ColumnCount> [r/o]
								/// Returns the column count.
virtual vuint32					get_ColumnCount( void ) const = 0;

		// <ColumnName> [r/o]
								/// Returns the column name.
                                /// inColumnIndex - zero-based index of the cursor column.
virtual const String&			get_ColumnName( vuint32 inColumnIndex ) const = 0;

		// <ColumnType> [r/o]
								/// Returns the column type: "text", "numeric", "integer", "real", "blob".
                                /// inColumnIndex - zero-based index of the cursor column.
virtual const String&			get_ColumnType( vuint32 inColumnIndex ) const = 0;

		// <ColumnDeclType> [r/o]
								/// Returns the column original type.
								/// inColumnIndex - zero-based index of the cursor column.
virtual const String&			get_ColumnDeclType(vuint32 inColumnIndex) const = 0;

		// <ColumnTableName> [r/o]
								/// Returns the column table name.
                                /// inColumnIndex - zero-based index of the cursor column.
virtual const String&           get_ColumnTableName( vuint32 inColumnIndex ) const = 0;

		// <RecordCount> [r/o]
		
								/// Returns the number of records.
virtual vuint32 				get_RecordCount( void ) const  = 0;

		// <CurrentRecord>
								/** Returns the current record number.
									Returns ZERO if the current record is undefined. */
virtual vuint32					get_CurrentRecord( void ) const  = 0;

								/** GoTo the specified record. 
									In case this record is absent, returns FALSE
									and do not change the current record. */
virtual bool					put_CurrentRecord( vuint32 inRecordNumber ) = 0;

		// <SqliteStatement> [r/o]
								/// Returns SqliteStatement this cursor belongs to.
virtual	I_SqliteStatement_Ptr	get_SqliteStatement( void ) const = 0;


	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// Navigation Methods:

								/// Makes the first record of cursor current. 
								/// Returns TRUE if there was successful move.
virtual bool					FirstRecord( void ) = 0;

								/// Makes the last record of cursor current.
								/// Returns TRUE if there was successful move.
virtual bool 					LastRecord( void ) = 0;

								/// Makes the next record of cursor current.
								/// Returns TRUE if there was successful move.
virtual bool 					NextRecord( void ) = 0;

								/// Makes the prev record of cursor current.
								/// Returns TRUE if there was successful move.
virtual bool 					PrevRecord( void ) = 0;

	// ---------------------
	// Value Methods:
								/// Returns the specified column's value for the current row.
                                /// inColumnIndex - zero-based index of the cursor column.
virtual I_Value_Ptr				get_Value( vuint32 inColumnIndex ) = 0;

};


/**********************************************************************************************/
VSQL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _VSQL_I_SqliteCursor_h
