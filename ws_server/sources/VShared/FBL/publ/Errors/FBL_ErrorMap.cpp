/**********************************************************************************************/
/* FBL_ErrorMap.cpp                                                      					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/
//
//
//	! ATTENTION!
//	! This file is automaticaly generated by transform.php
//	! Edit kernel_errors.xml instead of edit this file.
//

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Errors/FBL_ErrorMap.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
#define ENTRY_ERROR(c) { c, #c, NULL}
#undef new


/**********************************************************************************************/
ErrorEntry gErrorMap[] =
{

	ENTRY_ERROR(  ERR_OK                                                      ),
	ENTRY_ERROR(  ERR_WARNING                                                 ),
	ENTRY_ERROR(  ERR_WARNING_CURSOR_OPERATION_CONFLICT                       ),
	ENTRY_ERROR(  ERR_WARNING_DISCONNECT                                      ),
	ENTRY_ERROR(  ERR_WARNING_NULL_VALUE_ELIMINATED_IN_SET_FUNCTION           ),
	ENTRY_ERROR(  ERR_WARNING_STRING_DATA_RIGHT_TRUNCATION                    ),
	ENTRY_ERROR(  ERR_WARNING_NULL_COMPARE                                    ),
	ENTRY_ERROR(  ERR_FEATURE_NOT_SUPPORTED                                   ),
	ENTRY_ERROR(  ERR_FEATURE_NOT_SUPPORTED_EXPORT_FORMAT                     ),
	ENTRY_ERROR(  ERR_FEATURE_NOT_SUPPORTED_DUMP_FORMAT                       ),
	ENTRY_ERROR(  ERR_FEATURE_NOT_SUPPORTED_EMPTY_CONNECTION                  ),
	ENTRY_ERROR(  ERR_FEATURE_NOT_SUPPORTED_ADV                               ),
	ENTRY_ERROR(  ERR_CONSTRAINT_VIOLATION                                    ),
	ENTRY_ERROR(  ERR_CONSTRAINT_RESTRICT_VIOLATION                           ),
	ENTRY_ERROR(  ERR_CONSTRAINT_NOT_NULL_VIOLATION                           ),
	ENTRY_ERROR(  ERR_CONSTRAINT_FOREIGN_KEY_VIOLATION                        ),
	ENTRY_ERROR(  ERR_CONSTRAINT_UNIQUE_VIOLATION                             ),
	ENTRY_ERROR(  ERR_CONSTRAINT_CHECK_VIOLATION                              ),
	ENTRY_ERROR(  ERR_CONSTRAINT_DEFAULT_NOT_FOUND                            ),
	ENTRY_ERROR(  ERR_CONSTRAINT_EXPECTED_REQUIRED_PROPERTY                   ),
	ENTRY_ERROR(  ERR_CONSTRAINT_PRIMARY_KEY_VIOLATION                        ),
	ENTRY_ERROR(  ERR_CONSTRAINT_PRIMARY_KEY_ALREADY_DEFINED                  ),
	ENTRY_ERROR(  ERR_CONSTRAINT_NAME_NOT_UNIQUE                              ),
	ENTRY_ERROR(  ERR_CONSTRAINT_BAD_NAME                                     ),
	ENTRY_ERROR(  ERR_CONSTRAINT_NOT_FOUND                                    ),
	ENTRY_ERROR(  ERR_CONSTRAINT_WRONG_PROPERTY_TYPE                          ),
	ENTRY_ERROR(  ERR_CONSTRAINT_FIELD_ALREADY_UNIQUE                         ),
	ENTRY_ERROR(  ERR_CONSTRAINT_PRIMARY_KEY_HAVE_FOREIGN_KEY                 ),
	ENTRY_ERROR(  ERR_CONSTRAINT_FIELD_ALREADY_INDEXED                        ),
	ENTRY_ERROR(  ERR_CONSTRAINT_NOT_NULL_AND_NO_DEFAULT_VALUE_VIOLATION      ),
	ENTRY_ERROR(  ERR_TRANSACTION_STATE                                       ),
	ENTRY_ERROR(  ERR_TRANSACTION_STATE_ACTIVE                                ),
	ENTRY_ERROR(  ERR_TRANSACTION_STATE_READ_ONLY                             ),
	ENTRY_ERROR(  ERR_TRANSACTION_STATE_SCHEMA_AND_DATA_STATEMENT_MIXING      ),
	ENTRY_ERROR(  ERR_TRANSACTION_STATE_NOT_ACTIVE_FOR_CANCEL                 ),
	ENTRY_ERROR(  ERR_TRANSACTION_STATE_NOT_ACTIVE_FOR_COMMIT                 ),
	ENTRY_ERROR(  ERR_TRANSACTION_STATE_NO_CURRENT_TRANSACTION                ),
	ENTRY_ERROR(  ERR_INVALID_CHARACTER_SET                                   ),
	ENTRY_ERROR(  ERR_ROLLBACK                                                ),
	ENTRY_ERROR(  ERR_ROLLBACK_SERIALIZATION_FAILURE                          ),
	ENTRY_ERROR(  ERR_ROLLBACK_INTEGRITY_CONSTRAINT_VIOLATION                 ),
	ENTRY_ERROR(  ERR_ROLLBACK_STATEMENT_COMPLETION_UNKNOWN                   ),
	ENTRY_ERROR(  ERR_ROLLBACK_DEADLOCK_DETECTED                              ),
	ENTRY_ERROR(  ERR_DB_ERROR                                                ),
	ENTRY_ERROR(  ERR_DB_IDENTIFIER_NOT_FOUND                                 ),
	ENTRY_ERROR(  ERR_DB_LOCATION                                             ),
	ENTRY_ERROR(  ERR_DB_VERSION                                              ),
	ENTRY_ERROR(  ERR_DB_READ_ONLY                                            ),
	ENTRY_ERROR(  ERR_DB_NOT_FOUND                                            ),
	ENTRY_ERROR(  ERR_DB_NO_ACTIVE_DB                                         ),
	ENTRY_ERROR(  ERR_DATABASE_ALREADY_EXISTS                                 ),
	ENTRY_ERROR(  ERR_CONVERT_DATABASE_ERROR                                  ),
	ENTRY_ERROR(  ERR_DATABASE_AMBIGUOUS_IDENTIFIER                           ),
	ENTRY_ERROR(  ERR_SEQUENCE_ERROR                                          ),
	ENTRY_ERROR(  ERR_SEQUENCE_NOT_FOUND                                      ),
	ENTRY_ERROR(  ERR_SEQUENCE_NAME_NOT_UNIQUE                                ),
	ENTRY_ERROR(  ERR_SEQUENCE_BAD_NAME                                       ),
	ENTRY_ERROR(  ERR_SEQUENCE_LIMIT_REACHED                                  ),
	ENTRY_ERROR(  ERR_SEQUENCE_BOUNDARY_ERROR                                 ),
	ENTRY_ERROR(  ERR_SEQUENCE_BAD_START_VALUE                                ),
	ENTRY_ERROR(  ERR_SEQUENCE_BAD_INCREMENT_VALUE                            ),
	ENTRY_ERROR(  ERR_SEQUENCE_BAD_VALUE                                      ),
	ENTRY_ERROR(  ERR_SEQUENCE_IN_USE                                         ),
	ENTRY_ERROR(  ERR_INSUFFICIENT_RESOURCES                                  ),
	ENTRY_ERROR(  ERR_INSUFFICIENT_DISKSPACE                                  ),
	ENTRY_ERROR(  ERR_INSUFFICIENT_MEMORY                                     ),
	ENTRY_ERROR(  ERR_INSUFFICIENT_CACHE                                      ),
	ENTRY_ERROR(  ERR_EXTERNAL                                                ),
	ENTRY_ERROR(  ERR_EXTERNAL_INVALID_PARAMETER                              ),
	ENTRY_ERROR(  ERR_EXTERNAL_STD                                            ),
	ENTRY_ERROR(  ERR_INTERRUPT_OPERATION                                     ),
	ENTRY_ERROR(  ERR_INTERRUPT_QUERY                                         ),
	ENTRY_ERROR(  ERR_DUPLICATE_COLUMN                                        ),
	ENTRY_ERROR(  ERR_SHUTDOWN                                                ),
	ENTRY_ERROR(  ERR_SHUTDOWN_ADMIN                                          ),
	ENTRY_ERROR(  ERR_SHUTDOWN_CRASH                                          ),
	ENTRY_ERROR(  ERR_SHUTDOWN_CANNOT_CONNECT_NOW                             ),
	ENTRY_ERROR(  ERR_OS_FILE                                                 ),
	ENTRY_ERROR(  ERR_OS_FILE_BUSY                                            ),
	ENTRY_ERROR(  ERR_OS_FILE_NOT_FOUND                                       ),
	ENTRY_ERROR(  ERR_OS_FILE_NAME_NOT_UNIQUE                                 ),
	ENTRY_ERROR(  ERR_OS_FILE_OPEN                                            ),
	ENTRY_ERROR(  ERR_OS_FILE_NOT_OPEN                                        ),
	ENTRY_ERROR(  ERR_OS_FILE_EOF                                             ),
	ENTRY_ERROR(  ERR_OS_FILE_BOF                                             ),
	ENTRY_ERROR(  ERR_OS_FILE_READ_ONLY                                       ),
	ENTRY_ERROR(  ERR_OS_FILE_NOT_VALENTINA_DATABASE                          ),
	ENTRY_ERROR(  ERR_CREATE_FOLDER                                           ),
	ENTRY_ERROR(  ERR_OS_FOLDER_NOT_FOUND                                     ),
	ENTRY_ERROR(  ERR_STREAM_ERROR                                            ),
	ENTRY_ERROR(  ERR_STREAM_EOF                                              ),
	ENTRY_ERROR(  ERR_STREAM_BOF                                              ),
	ENTRY_ERROR(  ERR_STREAM_READ_ERROR                                       ),
	ENTRY_ERROR(  ERR_STREAM_WRITE_ERROR                                      ),
	ENTRY_ERROR(  ERR_STREAM_WOULD_BLOCK                                      ),
	ENTRY_ERROR(  ERR_STREAM_PARTIAL_DATA                                     ),
	ENTRY_ERROR(  ERR_ENCRYPTION_ERROR                                        ),
	ENTRY_ERROR(  ERR_ENCRYPTION_NOT_ENCRYPTED                                ),
	ENTRY_ERROR(  ERR_ENCRYPTION_WRONG_ENCRYPTION_KEY                         ),
	ENTRY_ERROR(  ERR_ENCRYPTION_KEY_OVERRIDING_NOT_ALLOWED                   ),
	ENTRY_ERROR(  ERR_ENCRYPTION_NOT_ROOT_OBJECT                              ),
	ENTRY_ERROR(  ERR_ENCRYPTION_NOT_EXCLUSIVE_ACCESS                         ),
	ENTRY_ERROR(  ERR_VALUE_INVALID                                           ),
	ENTRY_ERROR(  ERR_DATE_VALUE_NOT_MATCH_FORMAT                             ),
	ENTRY_ERROR(  ERR_DATE_VALUE_HAVE_WRONG_NUMBERS                           ),
	ENTRY_ERROR(  ERR_TIME_VALUE_NOT_MATCH_FORMAT                             ),
	ENTRY_ERROR(  ERR_TIME_VALUE_HAVE_WRONG_NUMBERS                           ),
	ENTRY_ERROR(  ERR_DATETIME_VALUE_NOT_MATCH_FORMAT                         ),
	ENTRY_ERROR(  ERR_DATETIME_VALUE_HAVE_WRONG_NUMBERS                       ),
	ENTRY_ERROR(  ERR_SQLSTATE_VALUE_INVALID                                  ),
	ENTRY_ERROR(  ERR_VALUE_BUFFER_OVERFLOW                                   ),
	ENTRY_ERROR(  ERR_ENUM_VALUE_INVALID                                      ),
	ENTRY_ERROR(  ERR_SSL_ERROR                                               ),
	ENTRY_ERROR(  ERR_SSL_CERTIFICATE_VALIDATION                              ),
	ENTRY_ERROR(  ERR_BONJOUR_ERROR                                           ),
	ENTRY_ERROR(  ERR_BONJOUR_SERVICE_ERROR                                   ),
	ENTRY_ERROR(  ERR_BONJOUR_SOCKET_ERROR                                    ),
	ENTRY_ERROR(  ERR_BONJOUR_SOCKET_TIMEOUT_ERROR                            ),
	ENTRY_ERROR(  ERR_BONJOUR_THREAD_ERROR                                    ),
	ENTRY_ERROR(  ERR_BONJOUR_NAME_IN_USE_ERROR                               ),
	ENTRY_ERROR(  ERR_BONJOUR_REGISTRATION_ERROR                              ),
	ENTRY_ERROR(  ERR_BONJOUR_SERIAL_NUMBER_ERROR                             ),
	ENTRY_ERROR(  ERR_PICTURE_ERROR                                           ),
	ENTRY_ERROR(  ERR_PICTURE_UNKNOWN_TYPE                                    ),
	ENTRY_ERROR(  ERR_PICTURE_UNKNOWN_ERROR                                   ),
	ENTRY_ERROR(  ERR_KEYVALUE_ERROR                                          ),
	ENTRY_ERROR(  ERR_KEYVALUE_BAD_NAME                                       ),
	ENTRY_ERROR(  ERR_KEYVALUE_NOT_FOUND                                      ),
	ENTRY_ERROR(  ERR_KEYVALUE_NAME_NOT_UNIQUE                                ),
	ENTRY_ERROR(  ERR_KEYVALUE_BAD_KEY                                        ),
	ENTRY_ERROR(  ERR_KEYVALUE_KEY_NOT_FOUND                                  ),
	ENTRY_ERROR(  ERR_KEYVALUE_NOT_UNIQUE_KEY                                 ),
	ENTRY_ERROR(  ERR_TABLE_ERROR                                             ),
	ENTRY_ERROR(  ERR_TABLE_EMPTY                                             ),
	ENTRY_ERROR(  ERR_TABLE_BAD_NAME                                          ),
	ENTRY_ERROR(  ERR_TABLE_NOT_FOUND                                         ),
	ENTRY_ERROR(  ERR_TABLE_RECORD_NOT_FOUND                                  ),
	ENTRY_ERROR(  ERR_TABLE_RECORD_LOCKED                                     ),
	ENTRY_ERROR(  ERR_TABLE_NAME_NOT_UNIQUE                                   ),
	ENTRY_ERROR(  ERR_WRONG_TABLENAME_USED                                    ),
	ENTRY_ERROR(  ERR_TABLE_IN_USE                                            ),
	ENTRY_ERROR(  ERR_TABLE_INCONSISTENT_FLAGS                                ),
	ENTRY_ERROR(  ERR_FIELD_ERROR                                             ),
	ENTRY_ERROR(  ERR_FIELD_NAME_NOT_UNIQUE                                   ),
	ENTRY_ERROR(  ERR_FIELD_BAD_NAME                                          ),
	ENTRY_ERROR(  ERR_FIELD_NOT_INDEXED                                       ),
	ENTRY_ERROR(  ERR_FIELD_IS_CALCULATED                                     ),
	ENTRY_ERROR(  ERR_FIELD_IS_COMPOSED                                       ),
	ENTRY_ERROR(  ERR_FIELD_UNIQUE                                            ),
	ENTRY_ERROR(  ERR_FIELD_IS_NOT_METHOD                                     ),
	ENTRY_ERROR(  ERR_FIELD_CANT_BE_CREATED                                   ),
	ENTRY_ERROR(  ERR_FIELD_LANGUAGE_NOT_FOUND                                ),
	ENTRY_ERROR(  ERR_FIELD_WRONG_TYPE                                        ),
	ENTRY_ERROR(  ERR_FIELD_NOT_FOUND                                         ),
	ENTRY_ERROR(  ERR_FIELD_METHOD_DEPENDS                                    ),
	ENTRY_ERROR(  ERR_FIELD_CANT_BE_METHOD                                    ),
	ENTRY_ERROR(  ERR_FIELD_VALUE_NOT_PREPARED_FOR_UPDATE                     ),
	ENTRY_ERROR(  ERR_FIELD_WRONG_ATTRIBUTE                                   ),
	ENTRY_ERROR(  ERR_FIELD_WRONG_TMP_FLAG                                    ),
	ENTRY_ERROR(  ERR_FIELD_MAX_LENGTH_TOO_BIG                                ),
	ENTRY_ERROR(  ERR_FIELD_IN_USE                                            ),
	ENTRY_ERROR(  ERR_FIELD_CANT_BE_DROPPED                                   ),
	ENTRY_ERROR(  ERR_FIELD_WRONG_DEFAULT_EXPRESSION                          ),
	ENTRY_ERROR(  ERR_LINK_ERROR                                              ),
	ENTRY_ERROR(  ERR_LINK_NAME_NOT_UNIQUE                                    ),
	ENTRY_ERROR(  ERR_LINK_NOT_FOUND                                          ),
	ENTRY_ERROR(  ERR_LINK_FIELDS_FROM_DIFFERENT_TABLES                       ),
	ENTRY_ERROR(  ERR_LINKED_RECORD_NOT_FOUND                                 ),
	ENTRY_ERROR(  ERR_LINK_WRONG_TABLE                                        ),
	ENTRY_ERROR(  ERR_LINK_WRONG_OWNER                                        ),
	ENTRY_ERROR(  ERR_LINK_BAD_NAME                                           ),
	ENTRY_ERROR(  ERR_LINK_BAD_PTR_TYPE                                       ),
	ENTRY_ERROR(  ERR_LINK_BAD_PTR_SIZE                                       ),
	ENTRY_ERROR(  ERR_LINK_PTR_NOT_NULLABLE                                   ),
	ENTRY_ERROR(  ERR_LINK_KEY_NOT_UNIQUE                                     ),
	ENTRY_ERROR(  ERR_LINK_KEY_NOT_INDEXED                                    ),
	ENTRY_ERROR(  ERR_LINK_PTR_NOT_INDEXED                                    ),
	ENTRY_ERROR(  ERR_LINK_TABLES_NOT_LINKED                                  ),
	ENTRY_ERROR(  ERR_LINK_LINKS_AMBIGUITY                                    ),
	ENTRY_ERROR(  ERR_LINK_WRONG_TYPE                                         ),
	ENTRY_ERROR(  ERR_LINKED_RECORDS_NOT_LINKED                               ),
	ENTRY_ERROR(  ERR_LINKED_RECORDS_ALREADY_LINKED                           ),
	ENTRY_ERROR(  ERR_LINK_POWER_VIOLATION                                    ),
	ENTRY_ERROR(  ERR_LINK_TO_NULL                                            ),
	ENTRY_ERROR(  ERR_LINK_RESTRICT_VIOLATION                                 ),
	ENTRY_ERROR(  ERR_LINK_WRONG_ON_DELETE_FLAG                               ),
	ENTRY_ERROR(  ERR_LINK_WRONG_ON_UPDATE_FLAG                               ),
	ENTRY_ERROR(  ERR_LINK_NO_KEY_VALUE_FOUND_RECORDS_COULD_NOT_BE_LINKED     ),
	ENTRY_ERROR(  ERR_LINK_CONVERT                                            ),
	ENTRY_ERROR(  ERR_LINK_CONVERT_TARGET_LINKS_TYPE                          ),
	ENTRY_ERROR(  ERR_LINK_CONVERT_SOURCE_LINK_TYPE                           ),
	ENTRY_ERROR(  ERR_LINK_CONVERT_DIFFERENT_TABLES                           ),
	ENTRY_ERROR(  ERR_LINK_CONVERT_TARGET_LINK_NOT_EMPTY                      ),
	ENTRY_ERROR(  ERR_LINK_CONVERT_TARGET_KEY_METHOD                          ),
	ENTRY_ERROR(  ERR_LINK_CONVERT_TARGET_PTR_METHOD                          ),
	ENTRY_ERROR(  ERR_LINK_CONVERT_TARGET_MUSTBE_MM                           ),
	ENTRY_ERROR(  ERR_LINK_CONVERT_SOURCE_MUSTBE_MM                           ),
	ENTRY_ERROR(  ERR_LINK_CONVERT_TARGET_LINKS_NOT_CORRESPONDED              ),
	ENTRY_ERROR(  ERR_LINK_CONVERT_SOURCE_LINKS_NOT_CORRESPONDED              ),
	ENTRY_ERROR(  ERR_LINK_TABLES_KIND_NOT_CORRESPONDED                       ),
	ENTRY_ERROR(  ERR_LINK_INCONSISTENT_FLAGS                                 ),
	ENTRY_ERROR(  ERR_LINK_INCONSISTENT_STORAGE_KIND                          ),
	ENTRY_ERROR(  ERR_LINK_MUST_BE_RECURSIVE                                  ),
	ENTRY_ERROR(  ERR_LINK_WITH_ORDER_CANNOT_BE_11                            ),
	ENTRY_ERROR(  ERR_CONNECTION_ERROR                                        ),
	ENTRY_ERROR(  ERR_CONNECTION_LIMIT                                        ),
	ENTRY_ERROR(  ERR_CONNECTION_PROTOCOL_VERSION_IS_TOO_NEW                  ),
	ENTRY_ERROR(  ERR_CONNECTION_INVALID_USER_NAME_OR_PASSWORD                ),
	ENTRY_ERROR(  ERR_CONNECTION_INVALID_DATABASE_NAME                        ),
	ENTRY_ERROR(  ERR_CONNECTION_COMMAND_OUT_OF_SYNC                          ),
	ENTRY_ERROR(  ERR_CONNECTION_NO_CURRENT_DB                                ),
	ENTRY_ERROR(  ERR_CONNECTION_TIMED_OUT                                    ),
	ENTRY_ERROR(  ERR_CONNECTION_WRONG_DEPLOYMENT_SERIAL                      ),
	ENTRY_ERROR(  ERR_CONNECTION_FORCEDLY_CLOSED                              ),
	ENTRY_ERROR(  ERR_CONNECTION_UNKNOWN                                      ),
	ENTRY_ERROR(  ERR_CONNECTION_INVALID_PROPERTY_NAME                        ),
	ENTRY_ERROR(  ERR_CONNECTION_ACCESS_DENIED                                ),
	ENTRY_ERROR(  ERR_CONNECTION_DATABASE_ALREADY_EXISTS                      ),
	ENTRY_ERROR(  ERR_CONNECTION_USER_ALREADY_EXISTS                          ),
	ENTRY_ERROR(  ERR_CONNECTION_INTERNAL_SERVER_ERROR                        ),
	ENTRY_ERROR(  ERR_CONNECTION_INVALID_IDENTIFIER                           ),
	ENTRY_ERROR(  ERR_CONNECTION_INCONSISTENT_RECEIVED_PACKET_VERSION         ),
	ENTRY_ERROR(  ERR_CONNECTION_BROKEN_RECEIVED_PACKET                       ),
	ENTRY_ERROR(  ERR_LOAD_SSL_LIBRARIES_ERROR                                ),
	ENTRY_ERROR(  ERR_CONNECTION_DATABASE_NOT_OPENED                          ),
	ENTRY_ERROR(  ERR_CONNECTION_USER_NOT_ALLOWED_REMOVE_SELF                 ),
	ENTRY_ERROR(  ERR_CONNECTION_OPERATION_REFUSED                            ),
	ENTRY_ERROR(  ERR_CONNECTION_USER_NOT_EXISTS                              ),
	ENTRY_ERROR(  ERR_CONNECTION_OPERATION_NOT_ALLOWED                        ),
	ENTRY_ERROR(  ERR_CONNECTION_OPERATION_REFUSED_ACL                        ),
	ENTRY_ERROR(  ERR_INDEX_ERROR                                             ),
	ENTRY_ERROR(  ERR_INDEX_CORRUPTED                                         ),
	ENTRY_ERROR(  ERR_INDEX_PAGE_INVENTORY_CORRUPTED                          ),
	ENTRY_ERROR(  ERR_INDEX_PAGE_CORRUPTED                                    ),
	ENTRY_ERROR(  ERR_INDEX_BYTE_RECLIST_CORRUPTED                            ),
	ENTRY_ERROR(  ERR_INDEX_UNIQUE_VIOLATION                                  ),
	ENTRY_ERROR(  ERR_INDEX_NOT_SORTED                                        ),
	ENTRY_ERROR(  ERR_INDEX_BUILD                                             ),
	ENTRY_ERROR(  ERR_INDEX_NOT_FOUND                                         ),
	ENTRY_ERROR(  ERR_INDEXPREFS_ERROR                                        ),
	ENTRY_ERROR(  ERR_INDEXPREFS_NAME_NOT_UNIQUE                              ),
	ENTRY_ERROR(  ERR_LICENCE_ERROR                                           ),
	ENTRY_ERROR(  ERR_LICENCE_INVALID                                         ),
	ENTRY_ERROR(  ERR_LICENCE_EXPIRED                                         ),
	ENTRY_ERROR(  ERR_COMPRESSOR_ERROR                                        ),
	ENTRY_ERROR(  ERR_ZIP_COMPRESSOR_ERROR                                    ),
	ENTRY_ERROR(  ERR_XML_PARSER_ERROR                                        ),
	ENTRY_ERROR(  ERR_XML_ATTRIBUTE_NAME_NOT_UNIQUE                           ),
	ENTRY_ERROR(  ERR_VIEW_ERROR                                              ),
	ENTRY_ERROR(  ERR_VIEW_NOT_FOUND                                          ),
	ENTRY_ERROR(  ERR_VIEW_NAME_NOT_UNIQUE                                    ),
	ENTRY_ERROR(  ERR_VIEW_BAD_NAME                                           ),
	ENTRY_ERROR(  ERR_VIEW_COLUMNS_TOO_FEW                                    ),
	ENTRY_ERROR(  ERR_VIEW_COLUMNS_TOO_MUCH                                   ),
	ENTRY_ERROR(  ERR_VIEW_READONLY                                           ),
	ENTRY_ERROR(  ERR_VIEW_COLUMN_NAME_NOT_UNIQUE                             ),
	ENTRY_ERROR(  ERR_SET_ERROR                                               ),
	ENTRY_ERROR(  ERR_BITSETS_SIZE_NOT_EQUAL_ERROR                            ),
	ENTRY_ERROR(  ERR_ARRAYSET_BIGGER_THAN_BITSET_ERROR                       ),
	ENTRY_ERROR(  ERR_INAPPROPRIATE_SET_OBJECT_ERROR                          ),
	ENTRY_ERROR(  ERR_METHOD_ERROR                                            ),
	ENTRY_ERROR(  ERR_METHOD_WRONG_LANGUAGE                                   ),
	ENTRY_ERROR(  ERR_METHOD_LEFT_OPERAND_MISSING                             ),
	ENTRY_ERROR(  ERR_METHOD_RIGHT_OPERAND_MISSING                            ),
	ENTRY_ERROR(  ERR_METHOD_BAD_EXPRESSION                                   ),
	ENTRY_ERROR(  ERR_METHOD_WRONG_DB_REF                                     ),
	ENTRY_ERROR(  ERR_METHOD_WRONG_BASEOBJECT_REF                             ),
	ENTRY_ERROR(  ERR_METHOD_WRONG_FIELD_REF                                  ),
	ENTRY_ERROR(  ERR_METHOD_WRONG_CURSOR_REF                                 ),
	ENTRY_ERROR(  ERR_METHOD_OUT_OF_RANGE                                     ),
	ENTRY_ERROR(  ERR_METHOD_FUNC_MUST_NOT_BE_CALLED                          ),
	ENTRY_ERROR(  ERR_METHOD_WRONG_PARAM_VALUE                                ),
	ENTRY_ERROR(  ERR_METHOD_PARAMS_TOO_FEW                                   ),
	ENTRY_ERROR(  ERR_METHOD_PARAMS_TOO_MUCH                                  ),
	ENTRY_ERROR(  ERR_METHOD_BAD_NAME                                         ),
	ENTRY_ERROR(  ERR_METHOD_NAME_NOT_UNIQUE                                  ),
	ENTRY_ERROR(  ERR_METHOD_AMBIGUOUS_IDENTIFIER                             ),
	ENTRY_ERROR(  ERR_TRIGGER_ERROR                                           ),
	ENTRY_ERROR(  ERR_TRIGGER_NOT_FOUND                                       ),
	ENTRY_ERROR(  ERR_TRIGGER_NAME_NOT_UNIQUE                                 ),
	ENTRY_ERROR(  ERR_TRIGGER_BAD_NAME                                        ),
	ENTRY_ERROR(  ERR_TRIGGER_TIME                                            ),
	ENTRY_ERROR(  ERR_TRIGGER_ACTION                                          ),
	ENTRY_ERROR(  ERR_TRIGGER_EXISTS                                          ),
	ENTRY_ERROR(  ERR_TRIGGER_ALIAS_ERROR                                     ),
	ENTRY_ERROR(  ERR_SP_ERROR                                                ),
	ENTRY_ERROR(  ERR_SP_NOT_FOUND                                            ),
	ENTRY_ERROR(  ERR_SP_NAME_NOT_UNIQUE                                      ),
	ENTRY_ERROR(  ERR_SP_BAD_NAME                                             ),
	ENTRY_ERROR(  ERR_SP_RAISE_ERROR                                          ),
	ENTRY_ERROR(  ERR_SP_NO_DEFAULT_ARGUMENT_VALUE_ERROR                      ),
	ENTRY_ERROR(  ERR_VALENTINA_ERROR                                         ),
	ENTRY_ERROR(  ERR_VALENTINA_PARAM_NAME_ALREADY_EXISTS                     ),
	ENTRY_ERROR(  ERR_VALENTINA_SERIAL_EXPIRED                                ),
	ENTRY_ERROR(  ERR_VALENTINA_ASSIGN_TO_SYSTEM_VARIABLE_ERROR               ),
	ENTRY_ERROR(  ERR_VALENTINA_ALREADY_INITIALIZED                           ),
	ENTRY_ERROR(  ERR_VALENTINA_FORMAT_ERROR                                  ),
	ENTRY_ERROR(  ERR_UDF_ERROR                                               ),
	ENTRY_ERROR(  ERR_UDF_NOT_SUPPORTED_RETURN_TYPE                           ),
	ENTRY_ERROR(  ERR_UDF_MASK_STANDARD_FUNCTION                              ),
	ENTRY_ERROR(  ERR_UDF_NAME_NOT_UNIQUE                                     ),
	ENTRY_ERROR(  ERR_REPORT_CURSOR_IS_NULL                                   ),
	ENTRY_ERROR(  ERR_REPORT_NOT_FOUND_IN_PROJECT                             ),
	ENTRY_ERROR(  ERR_REPORT_WRONG_PREVIEW_PAGE_INDEX                         ),
	ENTRY_ERROR(  ERR_REPORT_WRONG_SCALE_VALUE                                ),
	ENTRY_ERROR(  ERR_REPORT_PREVIEW_WIDTH                                    ),
	ENTRY_ERROR(  ERR_REPORT_WRONG_PREVIEW_HEIGHT                             ),
	ENTRY_ERROR(  ERR_REPORT_FILE_NOT_FOUND                                   ),
	ENTRY_ERROR(  ERR_PROJECT_NOT_FOUND                                       ),
	ENTRY_ERROR(  ERR_REPORT_UNKNOWN_DATASOURCE                               ),
	ENTRY_ERROR(  ERR_REPORT_SSH_TUNNEL                                       ),
	ENTRY_ERROR(  ERR_REPORT_GET_LIST                                         ),
	ENTRY_ERROR(  ERR_REPORT_BY_INDEX_NOT_FOUND_IN_PROJECT                    ),
	ENTRY_ERROR(  ERR_REPORT_STRUCTURE_NOT_LOADED                             ),
	ENTRY_ERROR(  ERR_REPORT_DEFAULT_SOURCE_QUERY_NOT_LOADED                  ),
	ENTRY_ERROR(  ERR_REPORT_DATASOURCE                                       ),
	ENTRY_ERROR(  ERR_TYPE_ERROR                                              ),
	ENTRY_ERROR(  ERR_TYPE_NOT_FOUND                                          ),
	ENTRY_ERROR(  ERR_TYPE_NAME_NOT_UNIQUE                                    ),
	ENTRY_ERROR(  ERR_TYPE_BAD_NAME                                           ),
	ENTRY_ERROR(  ERR_TYPE_UNKNOWN_CATEGORY                                   ),
	ENTRY_ERROR(  ERR_TYPE_BASETYPE_NOT_FOUND                                 ),
	ENTRY_ERROR(  ERR_TYPE_HAS_DESCENDANTS                                    ),
	ENTRY_ERROR(  ERR_TYPE_WRONG_BASETYPE                                     ),
	ENTRY_ERROR(  ERR_TYPE_HAS_INTERNAL_CATEGORY                              ),
	ENTRY_ERROR(  ERR_TYPE_IN_USE                                             ),
	ENTRY_ERROR(  ERR_TYPE_BAD_DEFAULT_VALUE                                  ),
	ENTRY_ERROR(  ERR_TYPE_WRONG_CATEGORY                                     ),
	ENTRY_ERROR(  ERR_TYPE_INSUFFICIENT_SPACE                                 ),
	ENTRY_ERROR(  ERR_TYPE_LOCALE_NAME_NOT_UNIQUE                             ),
	ENTRY_ERROR(  ERR_TYPE_LOCALE_WRONG_PARAMS_COUNT                          ),
	ENTRY_ERROR(  ERR_TYPE_LOCALE_NAME_NOT_FOUND                              ),
	ENTRY_ERROR(  ERR_VOLUME_ERROR                                            ),
	ENTRY_ERROR(  ERR_WRONG_SEGMENT                                           ),
	ENTRY_ERROR(  ERR_SEGMENT_LIMIT                                           ),
	ENTRY_ERROR(  ERR_ASPECT_ERROR                                            ),
	ENTRY_ERROR(  ERR_ASPECT_BAD_ID                                           ),
	ENTRY_ERROR(  ERR_ASPECT_ALREADY_EXIST                                    ),
	ENTRY_ERROR(  ERR_INTERNAL_ERROR                                          ),
	ENTRY_ERROR(  ERR_INTERNAL_DATA_CORRUPTED                                 ),
	ENTRY_ERROR(  ERR_INTERNAL_VARCHAR_CORRUPTED                              ),
	ENTRY_ERROR(  ERR_INTERNAL_EMBFILE_NOT_FOUND                              ),
	ENTRY_ERROR(  ERR_INTERNAL_VARCHAR_PAGE_CORRUPTED                         ),
	ENTRY_ERROR(  ERR_INTERNAL_OPEN_NEW_VERSION                               ),
	ENTRY_ERROR(  ERR_INTERNAL_READING_DOMAIN                                 ),
	ENTRY_ERROR(  ERR_INTERNAL_BASE_OBJECT_NOT_FOUND                          ),
	ENTRY_ERROR(  ERR_INTERNAL_INVALID_PARAMETER                              ),
	ENTRY_ERROR(  ERR_INTERNAL_FUNC_MUST_NOT_BE_CALLED                        ),
	ENTRY_ERROR(  ERR_INTERNAL_FUNC                                           ),
	ENTRY_ERROR(  ERR_INTERNAL_EMBFILE_IS_OPENED                              ),
	ENTRY_ERROR(  ERR_INTERNAL_EMBFILE_IS_NOT_OPENED                          ),
	ENTRY_ERROR(  ERR_INTERNAL_EMBFILE_EOF                                    ),
	ENTRY_ERROR(  ERR_INTERNAL_DUPLICATED_RECID_FOUND                         ),
	ENTRY_ERROR(  ERR_INTERNAL_DEMO_TIMEOUT                                   ),
	ENTRY_ERROR(  ERR_INTERNAL_SEGMENTMAP_CORRUPTED                           ),
	ENTRY_ERROR(  ERR_INTERNAL_WRONG_SEGMENT                                  ),
	ENTRY_ERROR(  ERR_INTERNAL_FIELD_WRONG_LENGTH                             ),
	ENTRY_ERROR(  ERR_JOURNALED_PAGE_NOT_UNIQUE                               ),
	ENTRY_ERROR(  ERR_INTERNAL_PAGEFILE_CORRUPTED                             ),
	ENTRY_ERROR(  ERR_CONFIG_FILE_ERROR                                       ), 

};


/**********************************************************************************************/
const vuint32 gErrorMapSize = sizeof(gErrorMap) / sizeof(ErrorEntry);

/**********************************************************************************************/
FBL_SHARED_EXP ErrorEntry* GetErrorMapEntry( vuint32 i )
{
	if( i > (gErrorMapSize - 1) )
		return NULL;

	return &gErrorMap[i];
}


/**********************************************************************************************/
FBL_SHARED_EXP vuint32 GetErrorMapSize( void )
{
	return gErrorMapSize;
}


/**********************************************************************************************/
FBL_End_Namespace

