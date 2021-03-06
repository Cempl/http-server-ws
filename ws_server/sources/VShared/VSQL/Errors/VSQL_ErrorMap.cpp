/**********************************************************************************************/
/* VSQL_ErrorMap.cpp                                                      					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/
//
//
//	! ATTENTION!
//	! This file is automaticaly generated by transform.php
//	! Edit sql_errors.xml instead of edit this file.
//

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/VSQL/Errors/VSQL_ErrorMap.h>


/**********************************************************************************************/
VSQL_Begin_Namespace


/**********************************************************************************************/
#define ENTRY_ERROR(c) { c, #c, NULL}
#undef new


/**********************************************************************************************/
FBL::ErrorEntry gErrorMap[] =
{

	ENTRY_ERROR(  ERR_DYNAMIC_SQL_ERROR                                       ),
	ENTRY_ERROR(  ERR_CLAUSE_DOESNOT_MATCH_DYNAMIC_PARAM_SPECIFICATION        ),
	ENTRY_ERROR(  ERR_CLAUSE_DOESNOT_MATCH_TARGET_SPECIFICATION               ),
	ENTRY_ERROR(  ERR_CLAUSE_REQUIRED_FOR_DYNAMIC_PARAMETERS                  ),
	ENTRY_ERROR(  ERR_CLAUSE_REQUIRED_FOR_RESULT_FIELDS                       ),
	ENTRY_ERROR(  ERR_CARDINALITY_VIOLATION                                   ),
	ENTRY_ERROR(  ERR_TOO_FEW_VALUES                                          ),
	ENTRY_ERROR(  ERR_TOO_MUCH_VALUES                                         ),
	ENTRY_ERROR(  ERR_FIELD_COUNT                                             ),
	ENTRY_ERROR(  ERR_TOO_FEW_JOIN_CONDITIONS                                 ),
	ENTRY_ERROR(  ERR_DUPLICATED_JOIN_CONDITION                               ),
	ENTRY_ERROR(  ERR_SUBQUERY_RETURN_NO_CURSOR                               ),
	ENTRY_ERROR(  ERR_DATA_EXCEPTION                                          ),
	ENTRY_ERROR(  ERR_STRING_DATA_RIGHT_TRUNCATION                            ),
	ENTRY_ERROR(  ERR_NUMERIC_VALUE_OUT_OF_RANGE                              ),
	ENTRY_ERROR(  ERR_NULL_VALUE_NOT_ALLOWED                                  ),
	ENTRY_ERROR(  ERR_ERROR_IN_ASSIGNMENT                                     ),
	ENTRY_ERROR(  ERR_INVALID_DATETIME_FORMAT                                 ),
	ENTRY_ERROR(  ERR_DATETIME_FIELD_OVERFLOW                                 ),
	ENTRY_ERROR(  ERR_SUBSTRING_ERROR                                         ),
	ENTRY_ERROR(  ERR_DIVISION_BY_ZERO                                        ),
	ENTRY_ERROR(  ERR_INVALID_ESCAPE_CHARACTER                                ),
	ENTRY_ERROR(  ERR_INVALID_PARAMETER_VALUE                                 ),
	ENTRY_ERROR(  ERR_INVALID_ESCAPE_SEQUENCE                                 ),
	ENTRY_ERROR(  ERR_TRIM_ERROR                                              ),
	ENTRY_ERROR(  ERR_CONVERT_NOT_ALLOWED                                     ),
	ENTRY_ERROR(  ERR_SYNTAX_ERROR_OR_ACCESS_RULE_VIOLATION_DYNAMIC_SQL       ),
	ENTRY_ERROR(  ERR_SYNTAX_ERROR_OR_ACCESS_RULE_VIOLATION                   ),
	ENTRY_ERROR(  ERR_SYNTAX_ERROR                                            ),
	ENTRY_ERROR(  ERR_AMBIGUOUS_COLUMN                                        ),
	ENTRY_ERROR(  ERR_DUPLICATE_ALIAS                                         ),
	ENTRY_ERROR(  ERR_UNDEFINED_FUNCTION                                      ),
	ENTRY_ERROR(  ERR_RESERVED_NAME                                           ),
	ENTRY_ERROR(  ERR_MIX_AGREGATIVE_AND_NORMAL                               ),
	ENTRY_ERROR(  ERR_AGGR_FUNC_WITH_AGGR_PARAM                               ),
	ENTRY_ERROR(  ERR_AGGR_FUNC_IN_WHERE_OR_HAVING                            ),
	ENTRY_ERROR(  ERR_DATETIME_NOT_ALLOWED_IN_SUM_FUNC                        ),
	ENTRY_ERROR(  ERR_AGGR_FUNC_IMPROPER_USAGE                                ),
	ENTRY_ERROR(  ERR_USER_EXCEPTION                                          ),
	ENTRY_ERROR(  ERR_SQL_SEMANTIC_ERROR                                      ),
	ENTRY_ERROR(  ERR_OBJECTPTR_BAD_SPECIFICATION                             ),
	ENTRY_ERROR(  ERR_COMMAND_NOT_ALLOWED                                     ),
	ENTRY_ERROR(  ERR_INVALID_OBJECT                                          ),
	ENTRY_ERROR(  ERR_CURSOR_EXCEPTION                                        ),
	ENTRY_ERROR(  ERR_CURSOR_READONLY                                         ),
	ENTRY_ERROR(  ERR_CURSOR_ALREADY_OPENED                                   ),
	ENTRY_ERROR(  ERR_CURSOR_ALREADY_CLOSED                                   ),
	ENTRY_ERROR(  ERR_CURSOR_WRONG_POSITION                                   ),
	ENTRY_ERROR(  ERR_CURSOR_RW_CLIENT_CURSOR                                 ),
	ENTRY_ERROR(  ERR_BACKUP_EXCEPTION                                        ),
	ENTRY_ERROR(  ERR_INVALID_PATH                                            ),
	ENTRY_ERROR(  ERR_RAM_DATABASE                                            ),
	ENTRY_ERROR(  ERR_DATABASE_FAIL                                           ),
	ENTRY_ERROR(  ERR_FILECOPY_FAIL                                           ),
	ENTRY_ERROR(  ERR_SCHEDULE_EXCEPTION                                      ),
	ENTRY_ERROR(  ERR_ENDSTAMP_LESS_STARTSTAMP                                ),
	ENTRY_ERROR(  ERR_DATABASE_NOT_REGISTERED                                 ),
	ENTRY_ERROR(  ERR_EMPTY_EVENT_NAME                                        ),
	ENTRY_ERROR(  ERR_EMPTY_EVENT_ACTION                                      ),
	ENTRY_ERROR(  ERR_INTERVAL_ERROR                                          ),
	ENTRY_ERROR(  ERR_EVENT_NOT_FOUND                                         ),
	ENTRY_ERROR(  ERR_EVENT_NAME_NOT_UNIQUE                                   ),
	ENTRY_ERROR(  ERR_SQLITE_EXCEPTION                                        ),
	ENTRY_ERROR(  ERR_SQLITE_DATABASE                                         ),
	ENTRY_ERROR(  ERR_SQLITE_UNEXPECTED_SQLEXECUTE                            ),
	ENTRY_ERROR(  ERR_SQLITE_ENGINE                                           ),
	ENTRY_ERROR(  ERR_SQL_PARSER_ERROR                                        )
};


/**********************************************************************************************/
vuint32 gErrorMapSize = sizeof(gErrorMap) / sizeof(ErrorEntry);


/**********************************************************************************************/
FBL::ErrorEntry* GetErrorMapEntry( vuint32 i )
{
	if( i > (gErrorMapSize - 1) )
		return nullptr;

	return &gErrorMap[i];
}


/**********************************************************************************************/
vuint32 GetErrorMapSize( void )
{
	return gErrorMapSize;
}


/**********************************************************************************************/
VSQL_End_Namespace

