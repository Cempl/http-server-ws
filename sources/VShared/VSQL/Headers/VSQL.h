/**********************************************************************************************/
/* VSQL.h																		 	  		  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _VSQL_h
   #define _VSQL_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
// Define VSQL namespace
// 
FBL_Begin_Namespace

#if Uses_Namespace
#define VSQL						vsql
#define VSQL_Begin_Namespace		namespace FBL { namespace VSQL {
#define VSQL_End_Namespace			} }
#define VSQL_Using_Namespace		using namespace FBL::VSQL;
#else
#define VSQL						
#define VSQL_Begin_Namespace		
#define VSQL_End_Namespace			
#define VSQL_Using_Namespace		
#endif

FBL_End_Namespace


/**********************************************************************************************/
VSQL_Begin_Namespace


/**********************************************************************************************/
// Don't use QueryResult::get_Type() (deprecated) to define the type of result.
// Use QueryResult::hasType( EQueryResult inResultType ) instead.
// http://www.valentina-db.com/bt/view.php?id=5516
//
// Specifies the SqlQuery() result type.
//
enum EQueryResult
{
	kQueryResult_Empty	= 0,
	kQueryResult_Cursor = 1,		///< Cursor result
	kQueryResult_Boolean,
	kQueryResult_ULong,				///< Affected rows result
	kQueryResult_String				///< String result.
};


/**********************************************************************************************/
// Specifies the SqlQuery() result type (bit mask for internal QueryResult usage to keep
// more than one state).
//
enum EQueryResultEx
{
	kQueryResultEx_Empty	= 0,
	kQueryResultEx_Cursor	= 1 << 0,	///< Cursor result
	kQueryResultEx_Boolean	= 1 << 1,
	kQueryResultEx_ULong	= 1 << 2,	///< Affected rows result
	kQueryResultEx_String	= 1 << 3	///< String result.
};


/**********************************************************************************************/
// Specifies additional inforamtion about SqlQuery execution. 
//
enum EQueryFlags
{
	kNone					= 0,		// (r/w) There is no any flags.	
	kSchemaWasModified		= 1 << 0,   // (r/w) The schema was modified by the query.
	kDatabaseWasChanged		= 1 << 1,   // (r/w) The database was changed by the query.
	
	
	kXMLResult				= 1 << 2    // (r/w) Query contains result as XML. (For FUTURE use)
										//
										// RZ 07/20/11 ????
										// may be this should go into EQueryResult
										// as kQueryResult_String_XML ????
};


/**********************************************************************************************/
// SQL Cursor CONSTANTS: 
/**********************************************************************************************/

// Specifies cursor location. 
enum ECursorLocation 
{
	kClient = 1,		///< Client side cursor
	kServer,			///< Server side cursor
	kServerBulk			///< Server side read-only cursor that can load records in batch mode.
};


enum ELockType
{
	kNoLocks = 1,		///< Cursor must not lock records. Also ignore locks of other cursors.
	kReadOnly = 2,		///< Cursor must lock records as ReadOnly.
	kReadWrite = 3		///< Cursor must lock records as ReadWrite.
};

// Specifies cursor direction.
enum ECursorDirection
{
	kForwardOnly = 1,	///< Navigation can be only from first to last records. ONCE.
	kRandom				///< Navigation can be in both directions. many times.
};


/**********************************************************************************************/
VSQL_End_Namespace


/**********************************************************************************************/
#include <VShared/VSQL/Errors/VSQL_Errors.h>
#include <VShared/VSQL/Errors/VSQL_Exceptions.h>


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _VSQL_h
