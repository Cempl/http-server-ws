/**********************************************************************************************/
/* VSQL_Cursor_ImportExport.h																  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                         					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _VSQL_Cursor_ImportExport_h
	#define _VSQL_Cursor_ImportExport_h
#pragma once

// FBL:
#include <VShared/VSQL/Interfaces/VSQL_I_SqlDatabase.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Table.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Field.h>
#include <VShared/VSQL/Interfaces/VSQL_I_Cursor.h>

/**********************************************************************************************/
FBL_Begin_Namespace

/**********************************************************************************************/
// Clone (copy) current value from the source field to the target one.
//
FBL_SHARED_EXP void Clone_Current_Value( 
	VALUE_TYPE	inFldType, 
	I_Field_Ptr inSourceFld, 
	I_Field_Ptr inTargetFld );


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
VSQL_Begin_Namespace


/**********************************************************************************************/
// Creates and returns a new table named inTargetTableName in database pinTargetDatabase
// and copies structure and records from the inpSourceCursor.
// 
// ERRORS: Table with such name already exists.
//
FBL_SHARED_EXP I_Table_Ptr CopyCursorToNewTable( 
	VSQL::I_Cursor_Ptr		inpSourceCursor,
	VSQL::I_SqlDatabase_Ptr	inpTargetDatabase,
	const String&			inNewTableName,
	EStorageType			inNewTableStorage = kStorage_Default,
	bool					inCopyRecords = true );


// Helpers.

/**********************************************************************************************/
void CloneFieldsLight( 
	I_Table_Ptr 		inpTableSource, 
	I_Table_Ptr 		inpTableTarget );

/**********************************************************************************************/
void Clone_Current_Record_Light( 
	vuint16				inFieldCount,
	I_Table_Ptr 		inpSourceTable, 
	I_Table_Ptr			inpTargetTable );


/**********************************************************************************************/
VSQL_End_Namespace


/**********************************************************************************************/
#endif // _VSQL_Cursor_ImportExport_h
