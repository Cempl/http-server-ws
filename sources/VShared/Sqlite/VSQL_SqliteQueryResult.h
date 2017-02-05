/**********************************************************************************************/
/* VSQL_SqliteQueryResult.h                                    	                 			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _VSQL_SqliteQueryResult_h
	#define _VSQL_SqliteQueryResult_h
#pragma once

#include <VShared/FBL/publ/Util_classes/FBL_ForeignPlugin_Imp.h>

#include <VShared/VSQL/Interfaces/VSQL_I_SqliteCursor.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
VSQL_Begin_Namespace


/**********************************************************************************************/
// THIS module declarations:
//
SMART_CLASS(SqliteQueryResult);


/**********************************************************************************************/
// Specifies the SqlQuery() result type (bit mask for internal SqliteQueryResult usage to keep
// more than one state).
//
enum ESqliteQueryResult
{
	kSqliteQueryResult_Empty				= 0,
	kSqliteQueryResult_Cursor				= 1 << 0,	///< Cursor result
	kSqliteQueryResult_AffectedRecords		= 1 << 1,	///< Affected rows result
	kSqliteQueryResult_LastInsertedRowID	= 1 << 2,	///< Last inserted RowID result
};


/**********************************************************************************************/
// This class is for SqliteDatabase::SqlQuery() method result.
// 
class FBL_SHARED_EXP_CLASS SqliteQueryResult :
	public ForeignPlugin_imp
{
		friend class SqliteStatement;
		friend class VC_SqliteStatement_imp;
	
		IMPLEMENT_UNKNOWN(SqliteQueryResult)
		BEGIN_INTERFACE_TABLE(SqliteQueryResult)
			IMPLEMENT_INTERFACE(I_ForeignPlugin)
		END_INTERFACE_TABLE()
 
	public://///////////////////////////////////////////////////////////////////////////////////
	
							SqliteQueryResult( void );
							SqliteQueryResult( const SqliteQueryResult& inQueryResult );

virtual						~SqliteQueryResult( void );

	
	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// Properties:

		vint64				get_AffectedRecords( void ) const;
		I_SqliteCursor_Ptr	get_Cursor( void ) const;
		vint64				get_LastInsertedRowID( void ) const;


	public://///////////////////////////////////////////////////////////////////////////////////

		bool				hasType( ESqliteQueryResult inResultType ) const;
		ESqliteQueryResult	get_Type( void ) const;


	protected://///////////////////////////////////////////////////////////////////////////////

		void				put_AffectedRecords( vint64 inValue );
		void				put_Cursor( I_SqliteCursor_Ptr	inpCursor );
		void				put_LastInsertedRowID( vint64 inValue );


	protected://////////////////////////////////////////////////////////////////////////////////

	// Result data:

		ESqliteQueryResult		mResultType;

		I_SqliteCursor_Ptr		mpCursor;
		vint64					mLastInsertedRowID;
		vint64					mAffectedRecords;
};


/**********************************************************************************************/
VSQL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _VSQL_SqliteQueryResult_h
