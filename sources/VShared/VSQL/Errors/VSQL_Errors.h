/**********************************************************************************************/
/* VSQL_Errors.h				   															  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/
//
//
//	! ATTENTION!
//	! This file is automaticaly generated by transform.php
//	! Edit sql_errors.xml instead of edit this file.
//

#ifndef __VSQL_Errors_h
	#define __VSQL_Errors_h
#pragma once


#include <VShared/FBL/publ/Errors/FBL_Errors.h>

#include <VShared/VSQL/Headers/VSQL.h>


/**********************************************************************************************/
VSQL_Begin_Namespace


/**********************************************************************************************/


#pragma mark GROUPS


/**********************************************************************************************/
// GROUP:  Dynamic SQL
// 

//Dynamic SQL error.
DECLARE_ERROR( ERR_DYNAMIC_SQL_ERROR                                       , 0x07000);   // STD

//Syntax error or prevents access.
DECLARE_ERROR( ERR_SYNTAX_ERROR_OR_ACCESS_RULE_VIOLATION_DYNAMIC_SQL       , 0x37000);   // STD


/**********************************************************************************************/
// GROUP:  Common SQL
// 

//Subquery returned more than 1 value. This is illegal when th
//e subquery follows =, !=, <, <= , >, >=, or when the subquer
//y is used as an expression.
DECLARE_ERROR( ERR_CARDINALITY_VIOLATION                                   , 0x21000);   // STD

//Syntax error or access violation.
DECLARE_ERROR( ERR_SYNTAX_ERROR_OR_ACCESS_RULE_VIOLATION                   , 0x42000);   // STD

//Semantic error.
DECLARE_ERROR( ERR_SQL_SEMANTIC_ERROR                                      , 0x55000);

//Cursor exception.
DECLARE_ERROR( ERR_CURSOR_EXCEPTION                                        , 0x56000);


/**********************************************************************************************/
// GROUP:  Data errors
// 

//Data Exception.
DECLARE_ERROR( ERR_DATA_EXCEPTION                                          , 0x22000);   // STD


/**********************************************************************************************/
// GROUP:  User-defined errors
// 

//User exception.
DECLARE_ERROR( ERR_USER_EXCEPTION                                          , 0x50000);


/**********************************************************************************************/
// GROUP:  Backup errors
// 

//Backup exception.
DECLARE_ERROR( ERR_BACKUP_EXCEPTION                                        , 0x66000);


/**********************************************************************************************/
// GROUP:  Server errors
// 

//Schedule exception.
DECLARE_ERROR( ERR_SCHEDULE_EXCEPTION                                      , 0x67000);


/**********************************************************************************************/
// GROUP:  Sqlite errors
// 

//Sqlite exception.
DECLARE_ERROR( ERR_SQLITE_EXCEPTION                                        , 0x68000);


/**********************************************************************************************/
// GROUP:  Parser SQL
// 

//%S
DECLARE_ERROR( ERR_SQL_PARSER_ERROR                                        , 0x71000);


#pragma mark -


#pragma mark Dynamic SQL error

/**********************************************************************************************/
// CLASS:  07               Dynamic SQL error
// 

//Using clause does not match parameter specs.
DECLARE_ERROR( ERR_CLAUSE_DOESNOT_MATCH_DYNAMIC_PARAM_SPECIFICATION        , 0x07001);   // STD

//Using clause does not match target specs.
DECLARE_ERROR( ERR_CLAUSE_DOESNOT_MATCH_TARGET_SPECIFICATION               , 0x07002);   // STD

//Using clause required for dynamic parameters.
DECLARE_ERROR( ERR_CLAUSE_REQUIRED_FOR_DYNAMIC_PARAMETERS                  , 0x07004);   // STD

//Using clause required for result fields.
DECLARE_ERROR( ERR_CLAUSE_REQUIRED_FOR_RESULT_FIELDS                       , 0x07007);   // STD


#pragma mark Cardinality Violation

/**********************************************************************************************/
// CLASS:  21               Cardinality Violation
// 

//It's too few values.
DECLARE_ERROR( ERR_TOO_FEW_VALUES                                          , 0x21501);

//It's too much values.
DECLARE_ERROR( ERR_TOO_MUCH_VALUES                                         , 0x21502);

//Only one expression can be specified in the select list when
// the subquery is not introduced with EXISTS.
DECLARE_ERROR( ERR_FIELD_COUNT                                             , 0x21503);

//It's too few join conditions.
DECLARE_ERROR( ERR_TOO_FEW_JOIN_CONDITIONS                                 , 0x21504);

//Duplicated join condition occurred.
DECLARE_ERROR( ERR_DUPLICATED_JOIN_CONDITION                               , 0x21505);

//Subquery returned no cursor.
DECLARE_ERROR( ERR_SUBQUERY_RETURN_NO_CURSOR                               , 0x21506);


#pragma mark Data Exception

/**********************************************************************************************/
// CLASS:  22               Data Exception
// 

//Data exception - string data right truncated.
DECLARE_ERROR( ERR_STRING_DATA_RIGHT_TRUNCATION                            , 0x22001);   // STD

//Arithmetic overflow occurred.
DECLARE_ERROR( ERR_NUMERIC_VALUE_OUT_OF_RANGE                              , 0x22003);   // STD

//NULL value is not allowed here.
DECLARE_ERROR( ERR_NULL_VALUE_NOT_ALLOWED                                  , 0x22004);   // STD

//Error in assignment.
DECLARE_ERROR( ERR_ERROR_IN_ASSIGNMENT                                     , 0x22005);   // STD

//Invalid datetime format.
DECLARE_ERROR( ERR_INVALID_DATETIME_FORMAT                                 , 0x22007);   // STD

//Datetime field overflow.
DECLARE_ERROR( ERR_DATETIME_FIELD_OVERFLOW                                 , 0x22008);   // STD

//Substring error.
DECLARE_ERROR( ERR_SUBSTRING_ERROR                                         , 0x22011);   // STD

//Divide by zero occurred.
DECLARE_ERROR( ERR_DIVISION_BY_ZERO                                        , 0x22012);   // STD

//Illegal escape character found. There are fewer bytes than n
//ecessary to form a valid character.
DECLARE_ERROR( ERR_INVALID_ESCAPE_CHARACTER                                , 0x22019);   // STD

//Invalid parameter value.
DECLARE_ERROR( ERR_INVALID_PARAMETER_VALUE                                 , 0x22023);   // STD

//Invalid pattern string. The character following the escape c
//haracter must be percent sign, underscore, left square brack
//et, right square bracket, or the escape character.
DECLARE_ERROR( ERR_INVALID_ESCAPE_SEQUENCE                                 , 0x22025);   // STD

//Trim error.
DECLARE_ERROR( ERR_TRIM_ERROR                                              , 0x22027);   // STD

//Convert is not allowed.
DECLARE_ERROR( ERR_CONVERT_NOT_ALLOWED                                     , 0x22501);


#pragma mark Syntax Error or Access Rule Violation

/**********************************************************************************************/
// CLASS:  42               Syntax Error or Access Rule Violation
// 

//Row %S. Incorrect syntax near "%S".
DECLARE_ERROR( ERR_SYNTAX_ERROR                                            , 0x42501);

//Ambiguous column name "%S"
DECLARE_ERROR( ERR_AMBIGUOUS_COLUMN                                        , 0x42502);

//Tables "%S" have same exposed names. Use correlation names t
//o distinguish them.
DECLARE_ERROR( ERR_DUPLICATE_ALIAS                                         , 0x42504);

//"%S" is not a recognized function name.
DECLARE_ERROR( ERR_UNDEFINED_FUNCTION                                      , 0x42506);

//Inadmissible reserved word using.
DECLARE_ERROR( ERR_RESERVED_NAME                                           , 0x42507);

//In the SELECT clause you can use only fields listed in the G
//ROUP BY and aggregative functions.
DECLARE_ERROR( ERR_MIX_AGREGATIVE_AND_NORMAL                               , 0x42508);

//Aggregative function cannot accept an argument, which itself
// is aggregative function.
DECLARE_ERROR( ERR_AGGR_FUNC_WITH_AGGR_PARAM                               , 0x42509);

//Aggregative function cannot appear in the WHERE or HAVING cl
//ause.
DECLARE_ERROR( ERR_AGGR_FUNC_IN_WHERE_OR_HAVING                            , 0x42510);

//The sum or average aggregate function cannot take a date, ti
//me or datetime data type as an argument.
DECLARE_ERROR( ERR_DATETIME_NOT_ALLOWED_IN_SUM_FUNC                        , 0x42511);

//Aggregative function cannot be used this way.
DECLARE_ERROR( ERR_AGGR_FUNC_IMPROPER_USAGE                                , 0x42512);


#pragma mark SQL Semantic error

/**********************************************************************************************/
// CLASS:  55               SQL Semantic error
// 

//Bad specification for ObjectPtr "%S".
DECLARE_ERROR( ERR_OBJECTPTR_BAD_SPECIFICATION                             , 0x55501);

//"%S" is not allowed in current context.
DECLARE_ERROR( ERR_COMMAND_NOT_ALLOWED                                     , 0x55502);

//Object "%S" has invalid body "%S".
DECLARE_ERROR( ERR_INVALID_OBJECT                                          , 0x55503);


#pragma mark Cursor Exceptions

/**********************************************************************************************/
// CLASS:  56               Cursor Exceptions
// 

//Cursor "%S" is read only.
DECLARE_ERROR( ERR_CURSOR_READONLY                                         , 0x56500);

//Cursor "%S" is opened already.
DECLARE_ERROR( ERR_CURSOR_ALREADY_OPENED                                   , 0x56501);

//Cursor "%S" is closed already.
DECLARE_ERROR( ERR_CURSOR_ALREADY_CLOSED                                   , 0x56502);

//Cursor "%S" failed to do "%S".
DECLARE_ERROR( ERR_CURSOR_WRONG_POSITION                                   , 0x56503);

//Client-side cursor can not be writable.
DECLARE_ERROR( ERR_CURSOR_RW_CLIENT_CURSOR                                 , 0x56504);


#pragma mark Backup Exceptions

/**********************************************************************************************/
// CLASS:  66               Backup Exceptions
// 

//Path "%S" is not valid.
DECLARE_ERROR( ERR_INVALID_PATH                                            , 0x66501);

//Database "%S" is RAM-based.
DECLARE_ERROR( ERR_RAM_DATABASE                                            , 0x66502);

//Database "%S" fails on diagnose.
DECLARE_ERROR( ERR_DATABASE_FAIL                                           , 0x66503);

//File "%S" copy failed with "%S" error.
DECLARE_ERROR( ERR_FILECOPY_FAIL                                           , 0x66504);


#pragma mark Schedule Exceptions

/**********************************************************************************************/
// CLASS:  67               Schedule Exceptions
// 

//Event "%S" end time stamp is less then start one.
DECLARE_ERROR( ERR_ENDSTAMP_LESS_STARTSTAMP                                , 0x67500);

//Event "%S". Database "%S" is not registered to the server.
DECLARE_ERROR( ERR_DATABASE_NOT_REGISTERED                                 , 0x67501);

//Event name is empty.
DECLARE_ERROR( ERR_EMPTY_EVENT_NAME                                        , 0x67502);

//Event "%S". Action text is empty.
DECLARE_ERROR( ERR_EMPTY_EVENT_ACTION                                      , 0x67503);

//Event "%S". Interval definition error.
DECLARE_ERROR( ERR_INTERVAL_ERROR                                          , 0x67504);

//Event "%S" is not found.
DECLARE_ERROR( ERR_EVENT_NOT_FOUND                                         , 0x67505);

//Event "%S" - name is not unique.
DECLARE_ERROR( ERR_EVENT_NAME_NOT_UNIQUE                                   , 0x67506);


#pragma mark Sqlite Exceptions

/**********************************************************************************************/
// CLASS:  68               Sqlite Exceptions
// 

//Sqlite db "%S": Error : "%S".
DECLARE_ERROR( ERR_SQLITE_DATABASE                                         , 0x68500);

//Sqlite db "%S": Error: call SqlSelect instead of SqlExecute.
DECLARE_ERROR( ERR_SQLITE_UNEXPECTED_SQLEXECUTE                            , 0x68501);

//Sqlite error : "%S".
DECLARE_ERROR( ERR_SQLITE_ENGINE                                           , 0x68502);


/**********************************************************************************************/

VSQL_End_Namespace

#endif // __VSQL_Errors_h
