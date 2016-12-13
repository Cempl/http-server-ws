/**********************************************************************************************/
/* VSQL_SqliteQueryResult.cpp			                                                   	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/
 
#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/Sqlite/VSQL_SqliteQueryResult.h>
#include <VShared/VSQL/Interfaces/VSQL_I_SqliteStatement.h>


/**********************************************************************************************/
VSQL_Begin_Namespace


/**********************************************************************************************/
SqliteQueryResult::SqliteQueryResult()
:
	mResultType( kSqliteQueryResult_Empty ),
	mpCursor( nullptr ),
	mLastInsertedRowID( 0 ),
	mAffectedRecords( 0 )
{
}


/**********************************************************************************************/
SqliteQueryResult::SqliteQueryResult( const SqliteQueryResult& inQueryResult )
:
	mResultType( inQueryResult.mResultType ),
	mpCursor( inQueryResult.get_Cursor() ),
	mLastInsertedRowID( inQueryResult.get_LastInsertedRowID() ),
	mAffectedRecords( inQueryResult.get_AffectedRecords() )
{
}


/**********************************************************************************************/
SqliteQueryResult::~SqliteQueryResult( void )
{
	// User MUST close cursor himself (using SqliteQueryResult just as container
	// to get "universal result").
	// Otherwise it will be alive until it's creator (SqlStatement) alive.

//	if( mpCursor )
//	{
//		try
//		{
//			I_SqliteStatement_Ptr pStmt = mpCursor->get_SqliteStatement();
//			if( pStmt )
//			{
//				pStmt->CloseCursor( mpCursor );
//			}
//		}
//		catch(...)
//		{
//		}
//	}
}


#pragma mark -

/**********************************************************************************************/
bool SqliteQueryResult::hasType( ESqliteQueryResult inResultType ) const
{
	bool res = false;
	
	switch( inResultType )
	{
		case kSqliteQueryResult_Empty:
		{
			res = (mResultType & kSqliteQueryResult_Empty);
		} break;

		case kSqliteQueryResult_Cursor:
		{
			res = (mResultType & kSqliteQueryResult_Cursor);		
		} break;

		case kSqliteQueryResult_AffectedRecords:
		{
			res = (mResultType & kSqliteQueryResult_AffectedRecords);		
		} break;

		case kSqliteQueryResult_LastInsertedRowID:
		{
			res = (mResultType & kSqliteQueryResult_LastInsertedRowID);
		} break;

		default:
			FBL_CHECK(false);
	}
	
	return res;
}


/**********************************************************************************************/
ESqliteQueryResult SqliteQueryResult::get_Type( void ) const
{
	return mResultType;
}


/**********************************************************************************************/
I_SqliteCursor_Ptr SqliteQueryResult::get_Cursor( void ) const
{
	return mpCursor;
}


/**********************************************************************************************/
void SqliteQueryResult::put_Cursor( I_SqliteCursor_Ptr inpCursor )
{
	mResultType = (ESqliteQueryResult)(mResultType | kSqliteQueryResult_Cursor);
	mpCursor = inpCursor;
}


/**********************************************************************************************/
vint64 SqliteQueryResult::get_LastInsertedRowID( void ) const
{
	return mLastInsertedRowID;
}


/**********************************************************************************************/
void SqliteQueryResult::put_LastInsertedRowID( vint64 inValue )
{
	mResultType = (ESqliteQueryResult)(mResultType | kSqliteQueryResult_LastInsertedRowID);
	mLastInsertedRowID = inValue;
}


/**********************************************************************************************/
vint64 SqliteQueryResult::get_AffectedRecords( void ) const
{
	return mAffectedRecords;
}


/**********************************************************************************************/
void SqliteQueryResult::put_AffectedRecords( vint64 inValue )
{
	mResultType = (ESqliteQueryResult)(mResultType | kSqliteQueryResult_AffectedRecords);
	mAffectedRecords = inValue;
}


/**********************************************************************************************/
VSQL_End_Namespace
