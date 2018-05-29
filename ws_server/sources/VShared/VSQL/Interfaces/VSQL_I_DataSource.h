/**********************************************************************************************/
/* VSQL_I_DataSource.h 				                                               			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _VSQL_I_DataSource_h
	#define _VSQL_I_DataSource_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Interfaces/FBL_I_Value.h>

// VSQL:
#include <VShared/VSQL/Headers/VSQL.h>

// STD:
#if FBL_UNIX
	#include <string>
#endif // FBL_UNIX

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
VSQL_Begin_Namespace


/**********************************************************************************************/
// forward declarations:
//
interface I_VReportEx;


/**********************************************************************************************/
// Currently it is implemented in vReport.dll (later it could be connectivity.dll) for
// 1. Valentina data-source
// 2. SqlLite data-source
// ....
//
// Reports don't use any db-specific terms like I_Database, I_Cursor, I_Field.
// Instead they knows only I_DataSources and ask data from them.
// 
/**********************************************************************************************/
SMART_INTERFACE(I_DataSource);
SMART_INTERFACE(I_DataSourceCursor);


/**********************************************************************************************/
// Interface I_DataSource uses by VReport to access various datasources -
// like Valentina, SQLite, ODBC etc.
//
interface FBL_SHARED_EXP_CLASS I_DataSource : public I_Unknown
{
	public://////////////////////////////////////////////////////////////////////////

virtual								~I_DataSource( void );

	public://////////////////////////////////////////////////////////////////////////

    // ---------------------
	// Properties:
    
									/** Returns database name if any. */
virtual	const std::wstring&			get_DatabaseName( void ) const = 0;
		
                                    /** Returns character used to quote names. */
virtual wchar_t                     get_QuoteChar( void ) const = 0;
    
                                    /** Returns TRUE, if Valentina engine is used to evaluate expressions */
virtual bool                        get_ExpressionEngineValentina( void ) const = 0;
    
    
    public://////////////////////////////////////////////////////////////////////////
    
	// -----------------------
	// Methods:

									/** Returns name for parameter's by index. */
virtual	std::wstring				GetBindName( size_t index ) const = 0;
		
									/** Parent report to log errors. */
virtual	I_VReportEx*                GetParentReport( void ) = 0;
virtual	void						SetParentReport( I_VReportEx* report ) = 0;

									// Executes query and returns result as cursor. 
virtual	I_DataSourceCursor_Ptr		SqlSelect(
										const std::wstring&		inQuery,
										ECursorLocation			inCursorLocation	= kClient,
										ELockType				inLockType			= kReadOnly,
										ECursorDirection		inCursorDirection	= kRandom,	
										ArrayOfConstValues_Ptr	inBinds				= nullptr ) = 0;
};


/**********************************************************************************************/
VSQL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _VSQL_I_DataSource_h
