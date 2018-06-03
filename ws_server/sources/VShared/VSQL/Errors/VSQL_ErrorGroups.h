/**********************************************************************************************/
/* VSQL_ErrorGroups.h																	 	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/
//
//
//	! ATTENTION!
//	! This file is automatically generated by transform.php
//	! Edit sql_errors.xml instead of edit this file.
//

#ifndef __VSQL_ErrorGroups_h
	#define __VSQL_ErrorGroups_h



/**********************************************************************************************/
VSQL_Begin_Namespace


/**********************************************************************************************/
// ATTENTION!
// Note, that some exception like xSuccess doesn't need to be thrown.
// Just create and store to the some environment set-of-SQLSTATEs
//


DECLARE_EXCEPTION( xDynamicSQLError,              xVSQLException, ERR_DYNAMIC_SQL_ERROR )
DECLARE_EXCEPTION( xCardinalitySQLError,          xVSQLException, ERR_CARDINALITY_VIOLATION )
DECLARE_EXCEPTION( xDataSQLError,                 xVSQLException, ERR_DATA_EXCEPTION )
DECLARE_EXCEPTION( xDynSyntaxOrAccessSQLError,    xVSQLException, ERR_SYNTAX_ERROR_OR_ACCESS_RULE_VIOLATION_DYNAMIC_SQL )
DECLARE_EXCEPTION( xSyntaxOrAccessSQLError,       xVSQLException, ERR_SYNTAX_ERROR_OR_ACCESS_RULE_VIOLATION )
DECLARE_EXCEPTION( xUserError,                    xVSQLException, ERR_USER_EXCEPTION )
DECLARE_EXCEPTION( xVSQL_Semantic,                xVSQLException, ERR_SQL_SEMANTIC_ERROR )
DECLARE_EXCEPTION( xCursorError,                  xVSQLException, ERR_CURSOR_EXCEPTION )
DECLARE_EXCEPTION( xBackupError,                  xVSQLException, ERR_BACKUP_EXCEPTION )
DECLARE_EXCEPTION( xScheduleError,                xVSQLException, ERR_SCHEDULE_EXCEPTION )
DECLARE_EXCEPTION( xSqliteError,                  xVSQLException, ERR_SQLITE_EXCEPTION )
DECLARE_EXCEPTION( xSQLParserError,               xVSQLException, ERR_SQL_PARSER_ERROR )


/**********************************************************************************************/
VSQL_End_Namespace

#endif // __VSQL_ErrorGroups_h
