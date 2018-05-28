/**********************************************************************************************/
/* VSQL_I_SqlStatement.h                                                       				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _VSQL_I_Statement_h
	#define _VSQL_I_Statement_h
#pragma once

#include <VShared/VSQL/Headers/VSQL.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_Value.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
VSQL_Begin_Namespace


/**********************************************************************************************/
// forward declarations:
//
SMART_INTERFACE(I_SqlDatabase);
SMART_INTERFACE(I_Cursor);
SMART_CLASS(QueryResult);


/**********************************************************************************************/
// This module forwards:
//
SMART_INTERFACE(I_SqlStatement);
SMART_ARRAY_OF_CLASS_PTRS( ArrayOfSqlStatements, I_SqlStatement* );


/**********************************************************************************************/
//StubInterface
/*
	This class is the result of call of I_SqlDatabase::CreateStatement( inSqlCmd ).
    
    It contains prepared SQL Statement.
    You can use bind_xxxx() methods to specify specific bound values,
    then call SqlExecute() / SqlSelect() methods of this class.
*/
interface FBL_SHARED_EXP_CLASS I_SqlStatement : public I_Unknown
{
	 	
virtual 					~I_SqlStatement( void );


	// -------------------------
	// Properties:

virtual	const String&		get_Query( void ) const = 0;

							/// Returns SqlDatabase this SqlStatement belongs to.
virtual	I_SqlDatabase_Ptr	get_SqlDatabase( void ) const = 0;



	// ---------------------
	// Bind Methods - Batch Style:
    // User self have prepared an array of values or even matrix of values.

                            // Single-Row binding.
virtual void				Bind( const ArrayOfValues_Ptr inBindings )  = 0;

							// Multi-Row binding.
virtual void				Bind( const ArrayOfValueArrays_Ptr inBindings ) = 0;


	// -------------------------
	// Bind Methods - Dictation Style:
    //
    //	These methods allows you to dictate bind-values one by one
    //	directly into SqlStatement object.
    //	Internally some I_Value object will be created.

virtual void				Bind_null( vuint32 inIndex )  = 0;

virtual void				Bind_bool( vuint32 inIndex, bool inValue )  = 0;
virtual void				Bind_uint8( vuint32 inIndex, vuint8 inValue )  = 0;

virtual void				Bind_uint16( vuint32 inIndex, vuint16 inValue )  = 0;
virtual void				Bind_int16( vuint32 inIndex, vint16 inValue )  = 0;

virtual void				Bind_uint32( vuint32 inIndex, vuint32 inValue )  = 0;
virtual void				Bind_int32( vuint32 inIndex, vint32 inValue )  = 0;

virtual void				Bind_uint64(vuint32 inIndex, vuint64 inValue) = 0;
virtual void				Bind_int64(vuint32 inIndex, vint64 inValue) = 0;

virtual void				Bind_float( vuint32 inIndex, float inValue )  = 0;
virtual void				Bind_double( vuint32 inIndex, double inValue )  = 0;

virtual void				Bind_string( vuint32 inIndex, const String& inValue )  = 0;

virtual void				Bind_binary( vuint32 inIndex, const char* inValue, vuint32 inLength ) = 0;

virtual void				Bind_value( vuint32 inIndex, const I_Value_Ptr inValue ) = 0;


	// Additionaly for Multi-Row Dictation:

                            // Call it in loop on start to clear prev matrix of values,
                            // then start to dictate the first bind-row.
virtual void				Init( void ) = 0;

							// This method allows to specify bound values for several rows.
                            // This is used e.g. to make INSERT with N rows.
                            // After you have specify N values of the first bind-row,
                            // call this method and specify N values for the second row, and so on.
virtual void				AddBindRow( void ) = 0;


	// ---------------------
	// SQL Methods:

							/** Executes SQL SELECT or SHOW commands for database.
								Returns I_Cursor as result of the query.
								Throws VSQL::xException on error. */
virtual	I_Cursor_Ptr		SqlSelect( 
								ECursorLocation 	inCursorLocation = kClient,
								ELockType 			inLockType = kReadOnly,
								ECursorDirection 	inCursorDirection = kForwardOnly ) = 0;

							/** Executes any SQL command except SELECT.
								Returns the number of affected records.
								Throws VSQL::xException on error. */
virtual	vuint32				SqlExecute( void ) = 0;
	
							/** Executes any SQL command.
								Returns the object contained cursor or affected records'
								count depends on query matter.
								Throws VSQL::xException on error. */
virtual	QueryResult_Ptr		SqlQuery(	
								ECursorLocation 		inCursorLocation = kClient,
								ELockType 				inLockType = kReadOnly,
								ECursorDirection 		inCursorDirection = kForwardOnly ) = 0;

							/** Executes any SQL command.
								Returns the object contained cursor or affected records'
								count depends on query matter.

								inFlags (see QueryOptions) extends SqlQuery() functionality.
                             	You can specify if produce warning, tune info, output info, etc.

								Throws VSQL::xException on error. */
virtual	QueryResult_Ptr		SqlQueryEx(	
								const vuint16			inFlags,
								ECursorLocation 		inCursorLocation = kClient,
								ELockType 				inLockType = kReadOnly,
								ECursorDirection 		inCursorDirection = kForwardOnly ) = 0;

};


/**********************************************************************************************/
VSQL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _VSQL_I_Statement_h
