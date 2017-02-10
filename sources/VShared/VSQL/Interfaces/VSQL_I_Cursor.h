/**********************************************************************************************/
/* VSQL_I_Cursor.h																			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _VSQL_I_Cursor_h
   #define _VSQL_I_Cursor_h
#pragma once

#include <VShared/VSQL/Headers/VSQL.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_Table.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>

 
/**********************************************************************************************/
VSQL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE(I_Cursor);
 

/**********************************************************************************************/
SMART_ARRAY_OF_SMART_PTRS( ArrayOfCursors, I_Cursor_Ptr );
SMART_ARRAY_OF_CLASS_PTRS( ArrayOfCursorsPointers, I_Cursor* );


/**********************************************************************************************/
// Interface I_Cursor -- result of SQL query.
//
// Result of SQL query always is a Table, so we inherit I_Cursor from I_Table to join 
// methods of both interfaces in one.
//
//StubInterface
interface FBL_SHARED_EXP_CLASS I_Cursor : public I_Table
{

virtual							~I_Cursor( void );

	// ---------------------
	// Properties:
								// Returns read only state.
virtual	bool					get_IsReadOnly( void ) const  = 0;	

								// Returns TRUE if this cursor is able to perform insert.
virtual	bool					CanInsert( void ) const  = 0;	

								// Returns TRUE if this cursor is able to perform update.
virtual	bool					CanUpdate( void ) const  = 0;	

								// Returns TRUE if this cursor is able to perform delete.
virtual	bool					CanDelete( void ) const  = 0;	

								// Returns the count of tables used by this cursor.
virtual vuint32					get_OriginalTablesCount(void) const = 0;

								// Return table name used by this cursor.
                                // If cursor is over JOIN, then returns empty string.
virtual const String&			get_OriginalTableName(vuint32 inIndex) const = 0;

								// Returns error number.
virtual	vint32					get_Error( void ) const  = 0;		

								// Return the table which cursor is built over.
virtual I_Table_Ptr				get_Table( void ) const  = 0;
virtual void					put_Table( I_Table_Ptr inTable )  = 0;

								// Returns the original query string that was used to build this cursor.
virtual const String&			get_Query( void ) const  = 0;


	// ---------------------
	// Record Navigation methods ODBC style:
	

virtual bool					FirstRecord_ODBC( void ) = 0;
virtual bool 					LastRecord_ODBC( void ) = 0;

virtual bool 					NextRecord_ODBC( void ) = 0;
virtual bool 					PrevRecord_ODBC( void ) = 0; 


	// ---------------------
	// SQL Methods:

								// Prepares an SQL query for execution: parse it, optimize, ...
								// As result a tree of nodes is built ready for execution.
virtual	vint32					Prepare( const String& inQuery ) = 0;
								
								// Executes the prepared sql query as many times as you need.
								// Actually have sense if you use '?' parameters in the query.
virtual	vint32					Execute( void ) = 0;


	// ---------------------
 	// Record methods:
 	
virtual CPOSITION				get_Position( void ) const  = 0;
virtual bool					put_Position( CPOSITION inPosition ) = 0;
 	
								// Is this cursor an owner of table.
virtual void					put_OwnsTable( I_Table_Ptr inTable )  = 0;
virtual void 					put_OwnsCursor( I_Cursor_Ptr inCursor )  = 0;

								// Delete current record.
virtual	void					DropRecord( void ) = 0;

								// Reduce the number of records in the Selection
virtual	void					ReduceSelection( vuint32 inLimitRows, vuint32 inLimitOffset  ) = 0;

								// ArraySet cursor's data representation.
								// Contains RecIDs of the original table.
								// Return NULL in join case.
								// There is always new ArraySet returned.
virtual ArraySet_Ptr 			ToArraySet( void ) = 0;

};


#pragma mark -
#pragma mark ===== Helper Methods ======


/**********************************************************************************************/
// RZ 05/29/11 for v 4.9 added.
//
// Problem is that if you are a C++ developer and use VSDK for development,
// then work with Values of Fields of cursor looks as:
//
// I_Cursor_Ptr pCursor = db->SqlSelect( ... );  
// ...
//		String str = pCursor->get_Field( "fldName"   )->get_Value()->get_String();
//		double vdd = pCursor->get_Field( "fldWeight" )->get_Value()->get_Double();
// ...
// 
// In contrast for such languages as REALbasic or VB syntax is more short:
//		String str = pCursor.StringField( "fldName" ).value
//
// RZ have decide that for C++ developer we can have set of simple macroses
// that will allow C++ developer syntax as:
//		String str = GET_STRING( pCursor, "fldName" );
//
//
// MORE COMPLEX VALUES as DateTime, Binary, still should be used in old way,
// because VSDK do not know format you goingto use. May be you can add own macroses in
// your project that looks similar and do job you need.


/**********************************************************************************************/
// HELPER macro, do not use directly. 
//
#define GET_VALUE_FROM_CURSOR(Type, curs, fname )										\
	curs->get_Field( fname )->get_Value()->get_##Type();		


/**********************************************************************************************/
#define GET_BOOLEAN( pCursor, fld_name )	GET_VALUE_FROM_CURSOR( Boolean, pCursor, fld_name )
#define GET_BYTE( pCursor, fld_name )		GET_VALUE_FROM_CURSOR( Byte,    pCursor, fld_name )

#define GET_SHORT( pCursor, fld_name )		GET_VALUE_FROM_CURSOR( Short,   pCursor, fld_name )
#define GET_USHORT( pCursor, fld_name )		GET_VALUE_FROM_CURSOR( UShort,  pCursor, fld_name )

#define GET_LONG( pCursor, fld_name )		GET_VALUE_FROM_CURSOR( Long,    pCursor, fld_name )
#define GET_ULONG( pCursor, fld_name )		GET_VALUE_FROM_CURSOR( ULong,   pCursor, fld_name )

#define GET_LLONG( pCursor, fld_name )		GET_VALUE_FROM_CURSOR( LLong,   pCursor, fld_name )
#define GET_ULLONG( pCursor, fld_name )		GET_VALUE_FROM_CURSOR( ULLong,  pCursor, fld_name )

#define GET_FLOAT( pCursor, fld_name )		GET_VALUE_FROM_CURSOR( Float,   pCursor, fld_name )
#define GET_DOUBLE( pCursor, fld_name )		GET_VALUE_FROM_CURSOR( Double,  pCursor, fld_name )

#define GET_STRING( pCursor, fld_name )		GET_VALUE_FROM_CURSOR( String,  pCursor, fld_name )


/**********************************************************************************************/
VSQL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _VSQL_I_Cursor_h



