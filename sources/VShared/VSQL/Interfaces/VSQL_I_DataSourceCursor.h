/**********************************************************************************************/
/* VSQL_I_DataSourceCursor.h																  */
/*																							  */
/* Copyright Paradigma Software, 2001-2007													  */
/* All Rights Reserved.																	  	  */
/**********************************************************************************************/

#ifndef _VSQL_I_DataSourceCursor_h
	#define _VSQL_I_DataSourceCursor_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Interfaces/FBL_I_Value.h>

// VSQL:
#include <VShared/VSQL/Headers/VSQL.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
VSQL_Begin_Namespace

/**********************************************************************************************/
SMART_INTERFACE(I_DataSource);
SMART_INTERFACE(I_DataSourceCursor);


/**********************************************************************************************/
// Interface I_DataSourceCursor represents a wrapper to a table/cursor from a datasource
//
interface FBL_SHARED_EXP_CLASS I_DataSourceCursor : public I_Unknown
{
	public://////////////////////////////////////////////////////////////////////////

virtual							~I_DataSourceCursor( void );

	public://////////////////////////////////////////////////////////////////////////

	// -----------------------
	// Properties

		// <ColumnCount> [r/o]
								/** Returns count of columns in the cursor. */
virtual	vuint32					get_ColumnCount( void ) const = 0;

		// <ColumnDataSize> [r/o]
								/** Returns size in bytes of value for current row. */
virtual	vuint32					get_ColumnDataSize( vuint32 index ) const = 0;

		// <ColumnIndex> [r/o]
								/** Returns index of column. */
virtual	int						get_ColumnIndex( const std::wstring& name ) const = 0;

		// <ColumnName> [r/o]
								/** Returns name of the N-th column. */
virtual	const std::wstring&		get_ColumnName( vuint32 index ) const = 0;

		// <ColumnType> [r/o]
								/** Returns type of the N-th column. */
virtual	FIELD_TYPE				get_ColumnType( vuint32 index ) const = 0;

		// <ColumnValue> [r/o]
								/** Returns the value for the N-th column of the current row. */
virtual	Const_I_Value_Ptr		get_ColumnValue( vuint32 index ) const = 0;

		// <ColumnValueBool> [r/o]
								/** Returns the value for the N-th column of the current row. */
virtual	bool					get_ColumnValueBool( vuint32 index ) const = 0;

		// <ColumnValueDouble> [r/o]
								/** Returns the value for the N-th column of the current row. */
virtual	double					get_ColumnValueDouble( vuint32 index ) const = 0;

		// <ColumnValueString> [r/o]
								/** Returns the value for the N-th column of the current row. */
virtual	std::wstring			get_ColumnValueString( vuint32 index ) const = 0;

		// <CursorDirection> [r/o]
								/** Returns cursor's direction. */
virtual	ECursorDirection		get_CursorDirection( void ) const = 0;

		// <CursorLocation> [r/o]
								/** Returns cursor's location. */
virtual	ECursorLocation			get_CursorLocation( void ) const = 0;

		// <CursorLockType> [r/o]
								/** Returns cursor's lock type. */
virtual	ELockType				get_CursorLockType( void ) const = 0;

		// <DataSource> [r/o]
								/** Returns parent datasource. */
virtual	I_DataSource_Ptr		get_DataSource( void ) const = 0;

		// <Position>
								/** Returns current position in the cursor. */
virtual	vuint32					get_Position( void ) const = 0;
virtual	void					put_Position( vuint32 pos ) = 0;

		// <Query> [r/o]
								/** Returns source query for the cursor. */
virtual	std::wstring			get_Query( void ) const = 0;

		// <RecordCount>
								/** Returns count of record in the cursor. */
virtual	vuint32					get_RecordCount( void ) const = 0;


	public://////////////////////////////////////////////////////////////////////////

	// -----------------------
	// Methods

								/** Make the first logical record of table current. */
virtual	bool					FirstRecord( void ) = 0;

								/** Make the next logical record of table current. */
virtual	bool					NextRecord( void ) = 0;

								/** Read data from the BLOB field. */
virtual	vuint32					ReadColumnData(
									vuint32		index,
									char*		outData,
									vuint32		count ) = 0;
};


/**********************************************************************************************/
VSQL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _VSQL_I_DataSourceCursor_h
