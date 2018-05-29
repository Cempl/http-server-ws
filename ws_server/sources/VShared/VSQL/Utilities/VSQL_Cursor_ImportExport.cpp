/**********************************************************************************************/
/* VSQL_Cursor_ImportExport.cpp                                            					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017 															  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/VSQL/Utilities/VSQL_Cursor_ImportExport.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_IndexStyle.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_FldBlob.h>
#include <VShared/FBL/publ/Properties/FBL_PropertyContainer.h>
#include <VShared/FBL/publ/Properties/FBL_FieldProperties.h>

/**********************************************************************************************/
FBL_Begin_Namespace

/**********************************************************************************************/
void Clone_Current_Value( 
	VALUE_TYPE	inFldType, 
	I_Field_Ptr inSourceFld, 
	I_Field_Ptr inTargetFld )
{		
	switch( inFldType )
	{		
		case 	kTypeSound:
		case 	kTypeMovie:
		case 	kTypeBLOB:
// Commented because this way we must read/write TEXT as STRING to avoid
// double read/write (we use FirstRecord()/NextRecord() for source table and therefore 
// src TEXT field already has its value).		
//		case 	kTypeText:
		case	kTypePicture:
		{	
			try
			{
				if( inSourceFld->get_IsNull() == false )
				{
					I_FldBlob_Ptr sourceBlobFld = QI( inSourceFld, I_FldBlob );
					FBL_CHECK( sourceBlobFld );
					vuint32 datasize = sourceBlobFld->get_DataSize();

					if( datasize )
					{
						MemPtr<char> theData( datasize );
						sourceBlobFld->ReadData( theData, datasize );						
						I_FldBlob_Ptr targetBlobFld = QI( inTargetFld, I_FldBlob );
						targetBlobFld->WriteData( theData, datasize );				
					}
					else
					{
						// Zero data but not sql-NULL.
						I_FldBlob_Ptr targetBlobFld = QI( inTargetFld, I_FldBlob );
						targetBlobFld->WriteData( nullptr, 0 );
					}
				}
			}
			catch(...)
			{
			}			
		} break;

		default: 
		{
			// Common case:
			inTargetFld->put_Value( inSourceFld->get_Value() );

		} break;
	}
}


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
VSQL_Begin_Namespace


/**********************************************************************************************/
I_Table_Ptr CopyCursorToNewTable( 
	VSQL::I_Cursor_Ptr		inpSourceCursor,
	VSQL::I_SqlDatabase_Ptr	inpTargetDatabase,
	const String&			inNewTableName,
	EStorageType			inNewTableStorage,
	bool					inCopyRecords )
{
	if( inpSourceCursor == nullptr || inpTargetDatabase == nullptr )
		return nullptr;

	I_Database_Ptr pTargetDb = inpTargetDatabase->get_BaseDatabase();
	I_Table_Ptr pSourceTable = inpSourceCursor->get_Table();

	// Create table	
	I_Table_Ptr pTargetTable = pTargetDb->CreateTable( inNewTableName, kTblPermanent, inNewTableStorage );

	// create fields
	try
	{
		CloneFieldsLight( pSourceTable, pTargetTable );
	}
	catch(...)
	{
		// it could be "SELECT f1, f1 FROM t1" - in
		// this case we are throwing exceptions.
		//
		pTargetDb->DropTable( pTargetTable );
		throw;
	}

	if( inCopyRecords )
	{
		// Create records
		vuint32 recCount = pSourceTable->get_RecordCount();
		vuint16 fldCount = pTargetTable->get_FieldCount();


		// During reading the data we can meet corrupted fields,
		// we want ignore this corruptions and logging it.
		// So we ask table continue read even if it meet corruption in some field.
		pSourceTable->put_ForceRead( true );

		// for each record:
		pSourceTable->FirstRecord();			
		for( vuint32 k = 1; k <= recCount; ++k )
		{					
			pTargetTable->SetBlank();		
			Clone_Current_Record_Light( fldCount, pSourceTable, pTargetTable );			
			pTargetTable->AddRecord();		
			pSourceTable->NextRecord();		
		}				

		pSourceTable->put_ForceRead( false );
	}

	return pTargetTable;
}


/**********************************************************************************************/
void CloneFieldsLight( 
	I_Table_Ptr 		inpTableSource, 
	I_Table_Ptr 		inpTableTarget )
{
	// Light because of 
	// 1. inpTableSource assumed to be a cursor (no ObjectPtrs, RecID, OID).
	// 2. inpTableTarget will be just a "data-snapshot" of inpTableSource
	//    (most of the flags and some properties like method-text are ignored)
	// 

	vuint16 fldCount = inpTableSource->get_FieldCount();
	for( vuint16 i = 1; i <= fldCount; ++i )
	{
		I_Field_Ptr pSourceFld = inpTableSource->get_Field(i);

		// Flags
		vuint16 fldFlags = fNone;
		if( pSourceFld->get_Nullable() )
			fldFlags |= fNullable;

		// At least for now we will ignore any flag except nullable
		// because of who need to get indexed (especially indexBywords -
		// btw here is a problem - asking for that we create some default tmp
		// index) result.
		// But note that there are some fields for which some flags are always ON!
		// ( indexed for boolean for example)
/*
		if( pSourceFld->get_Indexed() )
			fldFlags |= fIndexed;
		if( pSourceFld->get_Unique() )
			fldFlags |= fIndexUnique;
		if( pSourceFld->get_IndexStyle() )
			fldFlags |= fIndexByWords;
		
*/
		// No sense to keep identity, because cursor's field keeps some particular values
		// and we have to store exactly that values
		//
		//if( pSourceFld->get_Identity() )
		//	fldFlags |= fIdentity;
		
		// Remove any method property. Because it could be the methods
		// which are based on fields which are not available in target table.
		// "SELECT f1+1 FROM t2"
		// So every field will be snapshot of original one.
		//
		//if( pSourceFld->get_IsMethod() )
		//	fldFlags |= fMethod;

		vuint32 fldType = pSourceFld->get_Type();
		// Could not be objectPtr, RecID, OID in the target table because it is impossible 
		// to have those field types in the cursor.
		FBL_CHECK( fldType != kTypeObjectPtr && fldType != kTypeRecID && fldType != kTypeOID );

		// Type and properties
		I_PropertyContainer_Ptr properties;
		// Properties
		properties = pSourceFld->get_Properties();
		if( pSourceFld->get_IsMethod() )
		{
			properties->Remove( "METHOD_TEXT" );
			properties->Remove( "METHOD_TREE" );
		}

		// In case of client-cursor field in vclient environment (no properties at all)
		// register max length property manually.
		if( properties == nullptr )
		{
			properties = new PropertyContainer();	
			properties->Add( new Prop_MaxLen( pSourceFld->get_MaxLength() ) );
		}
		

		// Create target field
		I_Field_Ptr pTargetFld = inpTableTarget->CreateField( 
			pSourceFld->get_Name(), 
			fldType, 
			fldFlags,														
			properties );
	}	
}


/**********************************************************************************************/
void Clone_Current_Record_Light( 
	vuint16				inFieldCount,
	I_Table_Ptr 		inpSourceTable, 
	I_Table_Ptr			inpTargetTable )
{
	// For each field in target:
	//

	for( vuint16 j = 1; j <= inFieldCount; ++j )
	{
		I_Field_Ptr pTargetFld = inpTargetTable->get_Field( j );
		FBL_CHECK( pTargetFld );
		FBL_CHECK( pTargetFld->get_IsMethod() == false );

		// Skip identity:
		if( pTargetFld->get_Identity() )
			continue;

		I_Field_Ptr pSourceFld = inpSourceTable->get_Field( j );
		FBL_CHECK( pSourceFld );

		//
		// Clone DATA:
		//
		VALUE_TYPE fldType = pTargetFld->get_Type();
		Clone_Current_Value( fldType, pSourceFld, pTargetFld );
	}
}


/**********************************************************************************************/
VSQL_End_Namespace
