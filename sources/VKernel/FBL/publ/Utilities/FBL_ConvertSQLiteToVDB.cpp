/**********************************************************************************************/
/* VSQL_ConvertSQLiteToVDB.cpp                                            					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017 															  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/


#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VKernel/FBL/prot/Interfaces/FBL_I_DatabaseEx.h>

#include <VKernel/FBL/prot/Interfaces/FBL_I_TableEx.h>
#include <VKernel/FBL/prot/Structure/Constraints/FBL_Constraint_PrimaryKey.h>
#include <VKernel/FBL/prot/Nodes/FBL_Node.h>
#include <VKernel/FBL/publ/Utilities/FBL_ConvertSQLiteToVDB.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_Link.h>
#include <VShared/FBL/publ/Properties/FBL_FieldProperties.h>
#include <VShared/FBL/publ/Values/FBL_Value_Date.h>
#include <VShared/FBL/publ/Values/FBL_Value_DateTime.h>

#include <VShared/VSQL/Interfaces/VSQL_I_SqlDatabase.h>

#include <VKernel/VSQL/Parser/VLexer/VSQL_VLexer.h>
#include <VKernel/VSQL/Parser/VLexer/VSQL_Keywords_Vdb.hpp>

#include <VShared/FBL/publ/Interfaces/FBL_I_LogFile.h>

#include <VShared/VSQL/Interfaces/VSQL_I_SqliteStatement.h>
#include <VShared/VSQL/Interfaces/VSQL_I_SqliteCursor.h>

#include <VKernel/FBL/publ/Headers/VKernel.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
static VALUE_TYPE ConvertSQLiteToVDB_get_Value_Type( String inType )
{
	VALUE_TYPE res = 0;

	if( !inType.isEmpty() )
		inType = inType.toUpper(); // need a string uppercase

	if( inType.find( String("REAL").c_str() ) != -1 || inType.find( String("DOUB").c_str() ) != -1 || inType.find( String("FLOA").c_str() ) != -1 )
		res = kTypeDouble;

	if( inType.find( String("BLOB").c_str() ) != -1 || inType.isEmpty() )
		res = kTypeBLOB;

	if( inType.find( String("CHAR").c_str() ) != -1 || inType.find( String("TEXT").c_str() ) != -1 || inType.find( String("CLOB").c_str() ) != -1 )
		res = kTypeVarChar;

	if( inType.find( String("INT").c_str() ) != -1 )
		res = kTypeLLong;

	if( inType == "DATE" )
		res = kTypeDate;

	if( inType == "DATETIME" )
		res = kTypeDateTime;

	if( res == 0 )
		res = kTypeDecimal;

	return res;
}

/**********************************************************************************************/
static ArrayOfFields_Ptr ConvertSQLiteToVDB_get_Field_With_Index(
	VSQL::I_SqliteDatabase_Ptr	inLiteDB,
	ArrayOfFields_Ptr			inArrayFields,
	String						indexName)
{
	VSQL::I_SqliteStatement_Ptr pLiteStatement;
	VSQL::I_SqliteCursor_Ptr pLiteCursor;

	try
	{
		pLiteStatement = inLiteDB->CreateStatement(
										"PRAGMA index_info('" + indexName + "')");
		pLiteStatement->Prepare();
		pLiteCursor = pLiteStatement->SqlSelect();

		ArrayOfFields_Ptr pArrayFields = new ArrayOfFields();
		
		if( pLiteCursor->FirstRecord() )
		{
			vuint32 count = inArrayFields->get_Count();
			String FieldName;

			do
			{
				FieldName = pLiteCursor->get_Value(2)->get_String();

				for( vuint32 i = 1; i <= count; ++i )
					if( inArrayFields->get_ItemAt( i )->get_Name() == FieldName )
					{
						pArrayFields->AddItem( inArrayFields->get_ItemAt(i) );
						break;
					}
			}
			while(pLiteCursor->NextRecord());
		}

		inLiteDB->CloseStatement( pLiteStatement );

		return pArrayFields;
	}
	catch( VSQL::xSqliteError& )
	{
		inLiteDB->CloseStatement( pLiteStatement );

		throw;
	}
}


/**********************************************************************************************/
static void ConvertSQLiteToVDB_CopyIndex(
	VSQL::I_SqliteDatabase_Ptr	inLiteDB, 
	VSQL::I_SqlDatabase_Ptr		pSqlVDB, 
	I_Table_Ptr					inTable, 
	ArrayOfFields_Ptr			pArrayFields,
	I_LogFile*					pLogFile)
{
	*pLogFile << "\t\t\t\tCopying index...";

	VSQL::I_SqliteStatement_Ptr pLiteStatement;
	VSQL::I_SqliteCursor_Ptr pLiteCursor;

	try
	{
		pLiteStatement = inLiteDB->CreateStatement(
									"PRAGMA index_list('" + inTable->get_Name() + "')");
		pLiteStatement->Prepare();
		pLiteCursor = pLiteStatement->SqlSelect();

		if( pLiteCursor->FirstRecord() )
		{
			ArrayOfFields_Ptr pArrayFieldWithIndex = new ArrayOfFields();
			String origin;
			String IndexName;
			do
			{
				IndexName = pLiteCursor->get_Value(1)->get_String();
				origin = pLiteCursor->get_Value( 3 )->get_String();
				if( IndexName.left(7) != "sqlite_" && (origin == "c" || origin == "u") )
				{
					pArrayFieldWithIndex = ConvertSQLiteToVDB_get_Field_With_Index(inLiteDB, pArrayFields, IndexName);

					String sqlQuery("CREATE ");

					if( pLiteCursor->get_Value(2)->get_Boolean() )
						sqlQuery = (sqlQuery + "UNIQUE ");

					sqlQuery = (sqlQuery + "INDEX " + IndexName + " ON " + inTable->get_Name() + " (");
					
					vuint32 FieldCount = pArrayFieldWithIndex->get_Count();
					for( vuint32 i = 1; i < FieldCount; ++i )
					{
						sqlQuery = (sqlQuery + pArrayFieldWithIndex->get_ItemAt(i)->get_Name() + ",");
					}

					sqlQuery = (sqlQuery + pArrayFieldWithIndex->get_ItemAt(FieldCount)->get_Name() + ");");

					pSqlVDB->SqlExecute(sqlQuery);
				}
			}
			while( pLiteCursor->NextRecord() );
		}

		inLiteDB->CloseStatement( pLiteStatement );

		*pLogFile << "Done\n";
	}
	catch(...)
	{
		inLiteDB->CloseStatement( pLiteStatement );

		*pLogFile << "Fail\n";

		throw;
	}
}


/**********************************************************************************************/
static I_Value_Ptr ConvertSQLiteToVDB_getDateString(
	VSQL::I_SqliteDatabase_Ptr	inLiteDB, 
	I_Value_Ptr					inValue,
	VALUE_TYPE					inFieldType,
	VALUE_TYPE					inDataType)
{
	VSQL::I_SqliteStatement_Ptr pLiteStatement;
	VSQL::I_SqliteCursor_Ptr pLiteCursor;

	try
	{
		I_Value_Ptr pResValue;

		// Copy INT date and time, if date and time given a unix timestamp
		if( inFieldType == kTypeDate )
			if( inDataType == kTypeDouble )
				pLiteStatement = inLiteDB->CreateStatement( "SELECT date( " + inValue->get_String() + ")" );
			else
				pLiteStatement = inLiteDB->CreateStatement( "SELECT date( " + inValue->get_String() + " , 'unixepoch')" );
		else
			if( inDataType == kTypeDouble )
				pLiteStatement = inLiteDB->CreateStatement( "SELECT datetime( " + inValue->get_String() + ")" );
			else
				pLiteStatement = inLiteDB->CreateStatement( "SELECT datetime( " + inValue->get_String() + " , 'unixepoch')" );

		pLiteStatement->Prepare();
		pLiteCursor = pLiteStatement->SqlSelect();

		// If function don't gives result, we return NULL
		if( pLiteCursor->FirstRecord() )
			pResValue = pLiteCursor->get_Value(0);
		else
			pResValue = NULL;

		return pResValue;
	}
	catch( ... )
	{
		inLiteDB->CloseStatement( pLiteStatement );

		throw;
	}
}


/**********************************************************************************************/
static void ConvertSQLiteToVDB_CopyRecords(
	VSQL::I_SqliteDatabase_Ptr	inLiteDB,
	I_Table_Ptr					inTable,
	I_LogFile*					pLogFile)
{
	*pLogFile << "\t\t\t\tCopying records...";

	VSQL::I_SqliteStatement_Ptr pLiteStatement;
	VSQL::I_SqliteCursor_Ptr pLiteCursor;

	try
	{
		pLiteStatement = inLiteDB->CreateStatement( "SELECT * FROM " + inTable->get_Name() );
		pLiteStatement->Prepare();
		pLiteCursor = pLiteStatement->SqlSelect();

		bool flagFirstRecrod =  pLiteCursor->FirstRecord();
		ArrayOfFields_Ptr pArrayFields = new ArrayOfFields();
		vuint32 count = pLiteCursor->get_ColumnCount();

		// I must set fields in order like in db
		for( vuint32 i = 0; i < count; ++i )
		{
			pArrayFields->AddItem( inTable->get_Field( pLiteCursor->get_ColumnName(i) ) );
		}

		// Copy current row and after this go to next row
		if( flagFirstRecrod )
		{
			I_Field_Ptr pField;

			do
			{
				for( vuint32 i = 0; i < count; ++i )
				{
					pField = pArrayFields->get_ItemAt(i+1);
					VALUE_TYPE FieldType = pField->get_Type();

					switch( FieldType )
					{
						case kTypeDate:
						{
							I_Value_Ptr pDateValue = new Value_date;
							I_Value_Ptr pValue = pLiteCursor->get_Value(i);
							VALUE_TYPE DateType = pValue->get_Type();

							DTFormat format;
							format.mDateFormat = kYMD;
							format.mDateSep    = '-';

							if( DateType == kTypeDouble || DateType == kTypeLLong )
							{
								pValue = ConvertSQLiteToVDB_getDateString(inLiteDB, pValue, FieldType, DateType);
							}

							Convert_str_date_aa_fast(pValue->get_String().getBufferA(), &format, *((DateEncoded*) pDateValue->begin()));
							pField->put_Value( pDateValue );

							break;
						}

						case kTypeDateTime:
						{
							I_Value_Ptr pDateTimeValue = new Value_datetime;
							I_Value_Ptr pValue = pLiteCursor->get_Value(i);
							VALUE_TYPE DateType = pValue->get_Type();

							DTFormat format;
							format.mDateFormat = kYMD;
							format.mDateSep    = '-';

							if( DateType == kTypeDouble || DateType == kTypeLLong )
							{
								pValue = ConvertSQLiteToVDB_getDateString(inLiteDB, pValue, FieldType, DateType);
							}

							Convert_str_datetime_aa_fast(pValue->get_String().getBufferA(), &format, *((DateTimeEncoded*) pDateTimeValue->begin()));
							pField->put_Value( pDateTimeValue );

							break;
						}

						default:
						{
							pField->put_Value( pLiteCursor->get_Value(i) );
							break;
						}
					}
				}

				inTable->AddRecord();
			}
			while( pLiteCursor->NextRecord() );
		}

		*pLogFile << "Done\n";
	}
	catch( ... )
	{
		inLiteDB->CloseStatement( pLiteStatement );

		*pLogFile << "Fail\n";

		throw;
	}
}


/**********************************************************************************************/
static void ConvertSQLiteToVDB_Create_Fields(
	VSQL::I_SqliteDatabase_Ptr	inLiteDB, 
	VSQL::I_SqlDatabase_Ptr		pSqlVDB,
	I_Table_Ptr					inTable,
	I_LogFile*					pLogFile)
{
	VSQL::I_SqliteStatement_Ptr pLiteStatement;
	VSQL::I_SqliteCursor_Ptr pLiteCursor;	

	try
	{
		pLiteStatement = inLiteDB->CreateStatement(
										"PRAGMA table_info('" + inTable->get_Name() + "')");
		pLiteStatement->Prepare();
		pLiteCursor = pLiteStatement->SqlSelect();

		ArrayOfFields_Ptr pArrayFields = new ArrayOfFields;
		ArrayOfFields_Ptr pArrayKeyFields = new ArrayOfFields;

		if( pLiteCursor->FirstRecord() )
		{
			I_Value_Ptr pragmaValue;
			I_Field_Ptr pField;
			VALUE_TYPE type = 0;

			do
			{
				// get/set type
				pragmaValue = pLiteCursor->get_Value(2);
				type = ConvertSQLiteToVDB_get_Value_Type( pragmaValue->get_String() );

				// get/set field name
				pragmaValue = pLiteCursor->get_Value(1);

				pField = inTable->CreateField(pragmaValue->get_String(), type);

				// get/set nullable
				pragmaValue = pLiteCursor->get_Value(3);
				if( !pragmaValue->get_Boolean() )
					pField->put_Nullable(true);

				// get/set default name
				pragmaValue = pLiteCursor->get_Value(4);
				pField->put_DefaultValue(pragmaValue);

				// If field PK we remember this
				if( pLiteCursor->get_Value( 5 )->get_LLong() != 0)
					pArrayKeyFields->AddItem(pField);

				pArrayFields->AddItem( pField );			
			}
			while( pLiteCursor->NextRecord() );
		}

		//PK
		vuint32 CountOfKeyField = pArrayKeyFields->get_Count();
		if( CountOfKeyField )
		{
			I_Constrainable_Ptr	pConstrainable = fbl_dynamic_cast<I_Constrainable>( inTable );

			if( CountOfKeyField == 1 )
			{
				I_Field_Ptr pField = pArrayKeyFields->get_ItemAt(1);
				I_PropertyContainer_Ptr props = new PropertyContainer();
				props->Add( new Prop_PrimaryKey( pField.get() ) );
				props->Add( new Prop_IndexType( IndexType_kDefault ) );

				I_Constraint_Ptr constraint = FBL::CreateConstraint( inTable, "pk_" + pField->get_Name(), kConstraint_PrimaryKey, kTableLevel, false, props );
				pConstrainable->AddConstraint( constraint );
			}
			else
			{
				// This feature currently not working
				FBL_Throw(xConstraintError(ERR_CONSTRAINT_VIOLATION, "Do not support compound PK"));

				I_PropertyContainer_Ptr props = new PropertyContainer();
				props->Add( new Prop_PrimaryKey( pArrayKeyFields ) );
				props->Add( new Prop_IndexType( IndexType_kDefault ) );

				String pkName("pk_");
				for( vuint32 i = 0; i < CountOfKeyField; ++i )
				{
					pkName += ( pArrayKeyFields->get_ItemAt(i + 1)->get_Name() + "_" );
				}

				I_Constraint_Ptr constraint = FBL::CreateConstraint( inTable, pkName, kConstraint_PrimaryKey, kTableLevel, false, props );
				pConstrainable->AddConstraint( constraint );
			}
		}

		ConvertSQLiteToVDB_CopyIndex(inLiteDB, pSqlVDB, inTable, pArrayFields, pLogFile);
		ConvertSQLiteToVDB_CopyRecords(inLiteDB, inTable, pLogFile);

		inLiteDB->CloseStatement( pLiteStatement );	

		*pLogFile << "\t\t\tCopy fields completed successfully\n";
	}
	catch(...)
	{
		inLiteDB->CloseStatement( pLiteStatement );

		*pLogFile << "\t\t\tCopy fields completed unsuccessfully\n";

		throw;
	}
}


/**********************************************************************************************/
static void ConvertSQLiteToVDB_Create_Check(
	VSQL::I_SqliteDatabase_Ptr	inLiteDB, 
	I_Table_Ptr					inTable,
	I_LogFile*					pLogFile)
{
	VSQL::I_SqliteStatement_Ptr pLiteStatement;
	VSQL::I_SqliteCursor_Ptr pLiteCursor;

	try
	{
		pLiteStatement = inLiteDB->CreateStatement(
										"SELECT sql FROM sqlite_master WHERE name = '" + inTable->get_Name() + "'");
		pLiteStatement->Prepare();
		pLiteCursor = pLiteStatement->SqlSelect();

		if( pLiteCursor->FirstRecord() )
		{
			// Get query for create table and put this string in lexer
			String sqlQuery = pLiteCursor->get_Value(0)->get_String();
			VSQL::VLexerWithAllocator lexer;
			
			lexer.put_SqlString(sqlQuery.getBufferA());
			lexer.GetAllTokens(); 
 
			// Trying to get information about CHECK from a query
			VSQL::VLexer::Token pCurrToken;
			vuint32 countTokens = lexer.mTokens.get_Count();

			for( vuint32 i = 1; i <= countTokens; ++i )
			{
				// Trying to get the the name of the CHECK 
				pCurrToken = *(lexer.mTokens.get_ItemAt( i ));

				aptr<char> strTokenConstraint(new char [pCurrToken.mLen+1]);
				strncpy( strTokenConstraint.get(), pCurrToken.ps, (size_t) pCurrToken.mLen);
				strTokenConstraint.get()[pCurrToken.mLen] = '\0';

				String nameCheck;
				if( stricmp( strTokenConstraint.get(), "CONSTRAINT") == 0 )
				{
					++i;
					pCurrToken = *(lexer.mTokens.get_ItemAt( i ));

					nameCheck = String( pCurrToken.ps, pCurrToken.mLen );
					++i;
				}

				// Trying to get the text inside the CHECK
				bool firstBracket = true;
				vuint32 openBrackets = 0;
				pCurrToken = *(lexer.mTokens.get_ItemAt( i ));

				aptr<char> strTokenCHECK(new char [pCurrToken.mLen+1]);
				strncpy( strTokenCHECK.get(), pCurrToken.ps, (size_t) pCurrToken.mLen);
				strTokenCHECK.get()[pCurrToken.mLen] = '\0';

				if( stricmp(strTokenCHECK.get(), "CHECK") == 0 )
				{
					String conditionCheck;

					do
					{
						++i;
						pCurrToken = *(lexer.mTokens.get_ItemAt( i ));

						if( *pCurrToken.ps == '(')
						{
							++openBrackets;

							if( firstBracket )
							{
								firstBracket = false;
								continue; // get next token
							}
						}

						if( *pCurrToken.ps == ')' )
						{
							--openBrackets;
							if( !openBrackets )
								break; // exit the loop
						}

						conditionCheck = (conditionCheck + String(pCurrToken.ps, pCurrToken.mLen) + " ");
					}
					while( true );

					// Create CHECK
					PropertyContainer_Ptr props = new PropertyContainer();
					I_TableEx_Ptr pTableEx = QI(inTable, I_TableEx);
					props->Add( new PropertyString( "METHOD_TEXT", conditionCheck.c_str() ) );

					// If not find the name of the CHECK we we write in name text of the CHECK
					if( nameCheck.isEmpty() )
						pTableEx->CreateConstraint(conditionCheck.c_str(), kConstraint_Check, false, props);
					else
						pTableEx->CreateConstraint(nameCheck.c_str(), kConstraint_Check, false, props);
				}
			}
		}

		inLiteDB->CloseStatement( pLiteStatement );	

		*pLogFile << "Done\n";
	}
	catch( ... )
	{
		inLiteDB->CloseStatement( pLiteStatement );

		*pLogFile << "Fail\n";

		throw;
	}	
}


/**********************************************************************************************/
static ArrayOfStringsObj_Ptr ConvertSQLiteToVDB_get_Table_Info( VSQL::I_SqliteDatabase_Ptr inLiteDB )
{
	VSQL::I_SqliteStatement_Ptr pLiteStatement;
	VSQL::I_SqliteCursor_Ptr pLiteCursor;

	try
	{
		// Get the name of the table
		pLiteStatement = inLiteDB->CreateStatement(
										"SELECT name FROM sqlite_master WHERE type = 'table'");
		pLiteStatement->Prepare();
		pLiteCursor = pLiteStatement->SqlSelect();
	
		// Add the table name in array
		ArrayOfStringsObj_Ptr pTables_Name = new ArrayOfStringsObj();

		if( pLiteCursor->FirstRecord() )
		{
			String strTableName;

			do
			{
				strTableName = pLiteCursor->get_Value(0)->get_String();

				if( strTableName.left(7) != "sqlite_")
					pTables_Name->AddItem( strTableName );
			}
			while( pLiteCursor->NextRecord() );
		}
		
		// Cleanup
		inLiteDB->CloseStatement( pLiteStatement );
		
		return pTables_Name;
	}
	catch( VSQL::xSqliteError& )
	{
		inLiteDB->CloseStatement( pLiteStatement );

		throw;
	}		
}


/**********************************************************************************************/
static void ConvertSQLiteToVDB_CopyTables(
	VSQL::I_SqliteDatabase_Ptr	inLiteDB, 
	I_Database_Ptr				outVDB, 
	VSQL::I_SqlDatabase_Ptr		pSqlVDB,
	I_LogFile*					pLogFile)
{
	ArrayOfStringsObj_Ptr pTables_Name = new ArrayOfStringsObj();

	*pLogFile << "\tStart copying tables...\n";
		
	pTables_Name = ConvertSQLiteToVDB_get_Table_Info( inLiteDB );

	// Create table in Valentina DB
	I_Table_Ptr pTable;
	vuint32 table_count = pTables_Name->get_Count();

	try
	{
		for( vuint32 i = 0; i < table_count; ++i )
		{
			pTable = outVDB->CreateTable(pTables_Name->get_ItemAt(i+1));

			*pLogFile << "\t\tTable " << pTable->get_Name().c_str() << " copied\n";
			*pLogFile << "\t\t\tCopying fields...\n";

			ConvertSQLiteToVDB_Create_Fields(inLiteDB, pSqlVDB, pTable, pLogFile); 

			*pLogFile << "\t\t\tCopying CHECK...";

			ConvertSQLiteToVDB_Create_Check( inLiteDB, pTable, pLogFile );	
		}

		*pLogFile << "\tCopy tables completed successfully\n";
	}
	catch(...)
	{
		*pLogFile << "\tCopy tables completed unsuccessfully\n";

		throw;
	}
	
}


/**********************************************************************************************/
static EOnUpdate ConvertSQLiteToVDB_getOnUpdate( VSQL::I_SqliteCursor_Ptr inLiteCursor )
{
	EOnUpdate OnUpdate = kOnUpdate_NoAction; // Default value
	const char* Action = inLiteCursor->get_Value(6)->get_String().getBufferA();

	if( stricmp(Action, "RESTRICT") == 0 )
		OnUpdate = kOnUpdate_Restrict;

	if( stricmp(Action, "SET NULL") == 0 )
		OnUpdate = kOnUpdate_SetNull;

	if( stricmp(Action, "CASCADE") == 0 )
		OnUpdate = kOnUpdate_Cascade;

	if( stricmp(Action, "DEFAULT") == 0 )
		OnUpdate = kOnUpdate_Default;

	return OnUpdate;
}


/**********************************************************************************************/
static EOnDeletion ConvertSQLiteToVDB_getOnDelete( VSQL::I_SqliteCursor_Ptr inLiteCursor )
{
	EOnDeletion OnDelete = kOnDelete_NoAction; // Default value
	const char* Action = inLiteCursor->get_Value(6)->get_String().getBufferA();

	if( stricmp(Action, "RESTRICT") == 0 )
		OnDelete = kOnDelete_Restrict;

	if( stricmp(Action, "SET NULL") == 0 )
		OnDelete = kOnDelete_SetNull;

	if( stricmp(Action, "CASCADE") == 0 )
		OnDelete = kOnDelete_Cascade;

	if( stricmp(Action, "DEFAULT") == 0 )
		OnDelete = kOnDelete_Default;

	return OnDelete;
}


/**********************************************************************************************/
static void ConvertSQLiteToVDB_CopyFKLinks(
	VSQL::I_SqliteDatabase_Ptr	inLiteDB, 
	I_Database_Ptr				outVDB,
	I_LogFile*					pLogFile)
{
	*pLogFile << "\tCopying FK...";

	VSQL::I_SqliteStatement_Ptr pLiteStatement;
	VSQL::I_SqliteCursor_Ptr pLiteCursor;
	I_Table_Ptr pTable;
	vuint32 tables_count = outVDB->get_TableCount();

	try
	{
		for( vuint32 i = 0; i < tables_count; ++i )
		{
			pTable = outVDB->get_Table(i+1);

			pLiteStatement = inLiteDB->CreateStatement(
											"PRAGMA foreign_key_list('" + pTable->get_Name() + "')");
			pLiteStatement->Prepare();
			pLiteCursor = pLiteStatement->SqlSelect();

			if( pLiteCursor->FirstRecord() )
			{
				I_Field_Ptr key;
				I_Field_Ptr ptr;
				I_PropertyContainer_Ptr props;
				I_Table_Ptr keyTable;
				I_Value_Ptr pragmaValue;
				String LinkName;

				do
				{
					// Prepare 
					pragmaValue = pLiteCursor->get_Value(2);
					keyTable = outVDB->get_Table(pragmaValue->get_String());

					// Set ptr-field
					pragmaValue = pLiteCursor->get_Value(3);
					ptr = pTable->get_Field(pragmaValue->get_String());

					// ptr must be indexed
					if( !ptr->get_Indexed() )
						ptr->put_Indexed(true);

					// Set key-field
					pragmaValue = pLiteCursor->get_Value(4);
					key = keyTable->get_Field(pragmaValue->get_String());
					key->put_Unique(true);

					EOnUpdate OnUpdate = ConvertSQLiteToVDB_getOnUpdate(pLiteCursor);
					EOnDeletion OnDelete = ConvertSQLiteToVDB_getOnDelete(pLiteCursor);

					LinkName = "fk_" + pTable->get_Name() + ":" + ptr->get_Name() + "_" + keyTable->get_Name();

					props = Create_ForeignKey_Properties(
								key, ptr, OnDelete, OnUpdate );

					// Create link
					outVDB->CreateLink(LinkName, kRdbRelation, props);
				}
				while( pLiteCursor->NextRecord() );
			}

			inLiteDB->CloseStatement( pLiteStatement );
		}

		*pLogFile << "Done\n";
	}
	catch( ... )
	{
		inLiteDB->CloseStatement( pLiteStatement );

		*pLogFile << "Fail\n";

		throw;
	}	
}


/**********************************************************************************************/
static void ConvertSQLiteToVDB_CopyView(
	VSQL::I_SqliteDatabase_Ptr	inLiteDB, 
	I_Database_Ptr				outVDB,
	I_LogFile*					pLogFile)
{
	*pLogFile << "\tCopying view...";

	VSQL::I_SqliteStatement_Ptr pLiteStatement;
	VSQL::I_SqliteCursor_Ptr pLiteCursor;

	try
	{
		// Get information about view
		pLiteStatement = inLiteDB->CreateStatement(
										"SELECT name, sql  FROM sqlite_master WHERE type = 'view'");
		pLiteStatement->Prepare();
		pLiteCursor = pLiteStatement->SqlSelect();
	
		if( pLiteCursor->FirstRecord() )
		{
			String viewName;
			String viewText;

			do
			{
				viewName = pLiteCursor->get_Value(0)->get_String();
				viewText = pLiteCursor->get_Value(1)->get_String();

				I_DatabaseEx_Ptr exVDB = QI(outVDB, I_DatabaseEx);
				exVDB->CreateView(
								viewName,
								viewText,
								nullptr,
								nullptr,
								false);
			}
			while( pLiteCursor->NextRecord() );
		}

		*pLogFile << "Done\n";
	}
	catch( ... )
	{
		inLiteDB->CloseStatement( pLiteStatement );

		*pLogFile << "Fail\n";

		throw;
	}
}


/**********************************************************************************************/
static void ConvertSQLiteToVDB_getActionTimeAndEvent(
	String				triggerText, 
	ETriggerActionTime& outAction, 
	ETriggerEvent&		outEvent)
{
	// if we don't find Action Time or Event we must return zero
	outAction = kTriggerUnknownTime;
	outEvent = kTriggerUnknownEvent;

	VSQL::VLexerWithAllocator lexer;
			
	lexer.put_SqlString(triggerText.getBufferA());
	lexer.GetAllTokens();

	VSQL::VLexer::Token pCurrToken;
	vuint32 countTokens = lexer.mTokens.get_Count();

	for( vuint32 i = 1; i <= countTokens; ++i )
	{
		pCurrToken = *(lexer.mTokens.get_ItemAt( i ));

		aptr<char> strToken(new char [pCurrToken.mLen+1]);
		strncpy( strToken.get(), pCurrToken.ps, (size_t) pCurrToken.mLen);
		strToken.get()[pCurrToken.mLen] = '\0';

		if( stricmp(strToken.get(), "Before") == 0 )
			outAction = kTriggerBefore;

		if( stricmp(strToken.get(), "After") == 0 )
			outAction = kTriggerAfter;

		if( stricmp(strToken.get(), "Instead") == 0 )
			outAction = kTriggerInsteadOf;

		if( stricmp( strToken.get(), "Insert" ) == 0 )
		{
			outEvent = kTriggerInsert;
			break;
		}

		if( stricmp( strToken.get(), "Delete" ) == 0 )
		{
			outEvent = kTriggerDelete;
			break; 
		}

		if( stricmp( strToken.get(), "Update" ) == 0 )
		{
			outEvent = kTriggerUpdate;
			break;
		}
	}
}


/**********************************************************************************************/
static void ConvertSQLiteToVDB_CopyTrigger(
	VSQL::I_SqliteDatabase_Ptr	inLiteDB,
	I_Database_Ptr				outVDB,
	I_LogFile*					pLogFile)
{
	*pLogFile << "\tCopying trigger...";

	VSQL::I_SqliteStatement_Ptr pLiteStatement;
	VSQL::I_SqliteCursor_Ptr pLiteCursor;

	try
	{
		// Get information about view
		pLiteStatement = inLiteDB->CreateStatement(
										"SELECT name, tbl_name, sql  FROM sqlite_master WHERE type = 'trigger'");
		pLiteStatement->Prepare();
		pLiteCursor = pLiteStatement->SqlSelect();
	
		if( pLiteCursor->FirstRecord() )
		{
			String triggerName;
			String triggerText;
			String tableName;
			ETriggerActionTime action; 
			ETriggerEvent event;

			do
			{
				triggerName = pLiteCursor->get_Value(0)->get_String();
				tableName = pLiteCursor->get_Value(1)->get_String();
				triggerText = pLiteCursor->get_Value(2)->get_String();

				ConvertSQLiteToVDB_getActionTimeAndEvent(triggerText, action, event);

				I_DatabaseEx_Ptr exVDB = QI(outVDB, I_DatabaseEx);
				I_TableEx_Ptr pTableEx = exVDB->get_TableEx(tableName);
				if( pTableEx != NULL )
				{
					exVDB->CreateTrigger(
									triggerName,
									pTableEx,
									true,
									triggerText,
									false,
									action,
									event);
				}
				else
				{
					I_View_Ptr pView = exVDB->get_View(tableName);
					exVDB->CreateTrigger(
									triggerName,
									pView,
									true,
									triggerText,
									false,
									action,
									event);
				}
			}
			while( pLiteCursor->NextRecord() );
		}

		*pLogFile << "Done\n";
	}
	catch( ... )
	{
		inLiteDB->CloseStatement( pLiteStatement );

		*pLogFile << "Fail\n";

		throw;
	}
}


/**********************************************************************************************/
void ConvertSQLiteToVDB( 
	VSQL::I_SqliteDatabase_Ptr	inLiteDb, 
	I_Location_Ptr				inLocationVDB)
{
	I_LogFile* pLogFile = CreateLogFile(inLocationVDB->get_ParentDirectory()->get_Path(), "Log_ConvertSQLiteToVDB.txt");

	*pLogFile << "Convert Sqlite database to Valentina database\n";
	*pLogFile << "Status:\n";

	// Open db's
	VSQL::I_SqlDatabase_Ptr pSqlVDB = VSQL::CreateSqlDatabase();
	I_Database_Ptr pVDB = pSqlVDB->get_BaseDatabase();
	pVDB->put_Location(inLocationVDB);

	bool openLiteDB = false;
	pVDB->Open();
	if( !inLiteDb->get_IsOpen() )
	{
		inLiteDb->Open();
		openLiteDB = true;
	}

	try
	{
		ConvertSQLiteToVDB_CopyTables( inLiteDb, pVDB, pSqlVDB, pLogFile );

		// This feature is currently not working if we have compound PK
		ConvertSQLiteToVDB_CopyFKLinks( inLiteDb, pVDB, pLogFile );

		ConvertSQLiteToVDB_CopyView( inLiteDb, pVDB, pLogFile );
		ConvertSQLiteToVDB_CopyTrigger( inLiteDb, pVDB, pLogFile );
	}
	catch( xException& e )
	{
		inLiteDb->Close();
		pVDB->Close();

		*pLogFile << "Conversion not completed\n";
		*pLogFile <<"Errors:\n";
		*pLogFile <<e.get_ErrorString().c_str();

		throw e;
	}

	pVDB->Close();
	if( openLiteDB )
		inLiteDb->Close();
	
	*pLogFile << "Conversion completed successfully";
}


/**********************************************************************************************/
FBL_End_Namespace
