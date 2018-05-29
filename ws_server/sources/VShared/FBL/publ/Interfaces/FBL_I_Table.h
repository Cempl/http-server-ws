/**********************************************************************************************/
/* FBL_I_Table.h 		                                                      				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_Table_h 
	#define _FBL_I_Table_h
#pragma once

//INCLUDE_H:  #include  <VShared/FBL/publ/Interfaces/FBL_I_Table.h>

// VSHARED:
#include <VShared/FBL/publ/Interfaces/FBL_I_Database.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Field.h>
#include <VShared/FBL/publ/Templates/FBL_Predicates.h>
#include <VShared/FBL/publ/Util_classes/FBL_SortOrder.h>
#include <VShared/FBL/publ/Util_classes/FBL_ImportResult.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>

 
/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// forward declarations:
//
SMART_INTERFACE(I_Link);
SMART_INTERFACE( I_KeyValue );

//
SMART_CLASS( ArraySet );
SMART_CLASS( BitSet );
SMART_CLASS( Set );


/**********************************************************************************************/
// this file declarations:
//
SMART_INTERFACE(I_Table);


/**********************************************************************************************/
SMART_ARRAY_OF_SMART_PTRS( ArrayOfTables, I_Table_Ptr );


/**********************************************************************************************/
// interface FBL_SHARED_EXP_CLASS class 
//
//StubInterface
interface FBL_SHARED_EXP_CLASS I_Table : public I_Unknown
{

virtual 						~I_Table( void );


	// ---------------------
	// Properties:

		// <ID> [r/o]
								/// ID of table. Tmp table have negative ID.
virtual	Table_ID				get_ID( void ) const = 0;

		// <Name>
								/// Returns the name of table.
virtual	const String&			get_Name( void ) const   = 0; //OLDKEEPAS: mName

								/// Changes the name of table.
								///	ERROR: ERR_NAME_EXISTS, ERR_BAD_NAME.
virtual	void					put_Name( const String& inName ) = 0;

		// <Database> [r/o]
								/// Returns Database of this Table.
virtual I_Database_Ptr			get_Database( void ) const = 0;
	
		// <IsSystem>
								/// Returns TRUE if this is system table.
virtual bool					get_IsSystem( void ) const = 0;

		// <IsTemporary>
								/// Returns TRUE if this table is temporary.
virtual bool					get_IsTemporary( void ) const = 0;

		// <RecordExists> [r/o]
virtual bool                	get_RecordExists( REC_ID inRecID ) = 0;

		// <Size> [r/o]
								/// Returns the size of table in bytes on disk.
virtual flength					get_Size( void ) const = 0;


	// ---------------------
	// Field properties:
								
		// <FieldCount> [r/o]
		
								/// Returns the number of fields in the table.
virtual	vuint16					get_FieldCount( void ) const = 0;

                                /** Returns a field by Index.
                                    Returns NULL if the field is not found. */
virtual I_Field_Ptr				get_Field( vuint16 inIndex ) const = 0;

                                /** Returns a field by Name.
                                    Returns NULL if the field is not found. */
virtual I_Field_Ptr				get_Field( const String& inName ) const = 0;
    
    							// For C++11 'range-based for loop'.
virtual const ArrayOfFields& 	get_Fields( void ) const = 0;


	// ---------------------
	// Properties of the Current Record:

		// <RecordCount> [r/o]
		
								/// Returns the number of logical records.
virtual vuint32 				get_RecordCount( void ) const = 0;

		// <PhysicalRecordCount> [r/o]
		
								/// Returns the number of physical records in this table.
virtual vuint32 				get_PhysicalRecordCount( void ) const = 0;


		// <EOF> [r/o]	
								/** Returns TRUE if we was on the last record
									and there was attempt to read the next record. */
virtual	bool					get_EOF( void ) const = 0;

		// <BOF> [r/o]	
                                /** Returns TRUE if we was on the first record
                                    and there was attempt to read the prev record. */
virtual	bool					get_BOF( void ) const = 0;

		// <RecID>
                                /** Returns RecID of the current record.
                                    Returns ZERO if current record is undefined. */
virtual REC_ID					get_RecID( void ) const = 0;

                                /** GoTo the specified record.
                                    In case this record is deleted, returns FALSE
                                    and do not change current record. */
virtual bool					put_RecID( REC_ID inRecID ) = 0;

		// <OID>
								/** Returns OID of the current record. */
virtual OID						get_OID( void ) const = 0;

                                /** GoTo the specified Object ID.
                                    Actually it just extracts Table_ID and RecID,
                                    then checks if the Table_ID is right for this Table
                                    and then do put_RecID().
                                    In case of wrong Table_ID, throw errWrongTableID. */ 
virtual bool					put_OID( OID inOID ) = 0;


	// ---------------------
	// ADVANCED Properties:

		// <ForceRead>
								/// When this parameter is TRUE then if during read() table meet
								/// some corrupted field, it continue read rest of record without
								/// exception. Corrupted field get blank value.
virtual	bool					get_ForceRead( void ) const = 0;
virtual	void					put_ForceRead( bool inValue )  = 0;

		// <MaxRecordCount>
                                /** This property allows you to optimize your database
                                    if you have some info about data it will keep.
                                    
                                    For example, if you know that Table will have only 200 records
                                    then Valentina can use this info to store RecID of such table
                                    in 1 byte instead of 4. This can reduce size of:
                                        * Indexed of this table
                                        * ObjectPtr fields that point to this table.
                                        * BinaryLinks that link to this table.
                                    
                                    This is a persistent property. On default it is vuint32_max. */
virtual	vuint32					get_MaxRecordCount( void ) const = 0;
virtual	void					put_MaxRecordCount( vuint32 inValue ) = 0;

		// <IndexChanges>

                                /** If inOnOff is FALSE then this function drop all indexes of this
                                    Table except UNIQUE indexes and put Table into the state when any
                                    future modifications will not be indexed (except UNIQUE indexes). 
                                    
                                    This function do not touch system tables and field flags.
                                    Fields still are marked as indexed. So in case of system failure
                                    the next open will just rebuild missing indexes.
                                    
                                    This function is very useful if you are going to do some batch
                                    operation, e.g. add/delete a lots of records. When you have finish
                                    you need call this functions again with inOnOff = TRUE,
                                    to rebuild indexed and return table in the normal state. */
virtual bool					get_IndexChanges( void ) const = 0;
virtual void					put_IndexChanges( bool inOnOff )  = 0;


	// ---------------------
	// Link Properties:

								/** Returns the number of links. */
virtual vuint16					get_LinkCount( void ) const = 0;	

								/** Returns link by index. */
virtual I_Link_Ptr				get_Link( vuint16 inIndex ) const = 0;	

								/** Returns link by name. */
virtual I_Link_Ptr				get_Link( const String& inName ) const = 0;	

    							// For C++11 'range-based for loop'.
virtual const ArrayOfLinks&		get_Links( void ) const = 0;

		// <KeyValuesCount> [r/o]

virtual vuint32					get_KeyValueCount( void ) const  = 0;


	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// DB Schema Methods:
 
                                /** Creates a new field in the Table.

                                Errors:
                                        xFieldError	{ 	ERR_FIELD_WRONG_TYPE, 
                                                        ERR_FIELD_BAD_NAME, 
                                                        ERR_FIELD_NAME_NOT_UNIQUE 	}*/
virtual I_Field_Ptr				CreateField(
                                    const String& 		inName,
                                    FIELD_TYPE			inType, 
                                    vuint16 			inFlags = fNone,
                                    Const_I_PropertyContainer_Ptr inProperties = nullptr ) = 0;
							
								/** Drop Field with all its info (e.g. indexes) */
virtual void					DropField(
									I_Field_Ptr			inField ) = 0;

virtual I_Field_Ptr 			ConvertField(
                                    I_Field_Ptr			inField,
                                    FIELD_TYPE			inType, 
                                    vuint16 			inFlags,
                                    Const_I_PropertyContainer_Ptr inProperties ) = 0;


	// ---------------------
	// Record Modification Methods:

			
                                /** Clears RAM-buffer of the current record.
                                    Makes all nullable fields to be NULL, 
                                    if a field have default value set field to default. 
                                    @param inAccessMode allow you optimize this function,
                                    if you exactly know that you use SetBlank() to prepare buffer
                                    for AddRecord(). It will be faster because in this case Valentina
                                    will not spend time on copy of the old values. 	*/
virtual void					SetBlank(
									EValueAccess inAccessMode = forUpdate )  = 0;
	
                                /** Adds a new record to the table.
                                    Returns its REC_ID on success, otherwise returns 0. 
                                    ERROR: errNotUniqueValue */
virtual REC_ID					AddRecord( void ) = 0;

		// Return TRUE if record was added. 
		// Return FALSE if operation fail, e.g. database is read only.

                                /** Deletes the current record of Table if inRecID is 0.
                                    After a deletion, the next record becomes the current one if it exists. 
                                    Otherwise the previous record becomes current. 
                                    If a Cursor becomes empty then the current record will be undefined.
                                
                                    If inRecID is not 0, then deletes the specified record, 
                                    but NOT change the current record of Table.
                                    We use this e.g. to implement inherited tables functionality.
                                    
                                    ERROR: errRecNotFound, errRecIsLocked */ 
virtual void					DeleteRecord( REC_ID inRecID = 0 ) = 0;

                                /** Updates the current record by new field values if inRecID is 0.
                                    IF inRecID is not 0, then updates the specified record, 
                                    but NOT changes the current record of Table.
                                    ERROR: errNotUniqueValue */
virtual void					UpdateRecord( REC_ID inRecID = 0 ) = 0;

                                /** Delete all records of this I_Table if inSet is NULL.
                                    Otherwise deletes only the specified selection of records. */
virtual	void					DeleteAllRecords( Set_Ptr inSet = nullptr ) = 0;

                                /** Update all records of this I_Table if inSet is NULL.
                                    Otherwise updates only the specified selection of records. */
virtual	void					UpdateAllRecords( Set_Ptr inSet = nullptr ) = 0;

								/** Truncate all records of this I_Table.*/
virtual	void					Truncate( void ) = 0;

								/** Flush to disk all changes of THIS table.
									More effective than flush the whole db. */
virtual void					Flush( void ) = 0;


	// ---------------------
	// Record Navigation Methods:

		// Return TRUE if operation have found the target record. 
		// EXAMPLE: if e.g. NextRecord() find the next record, it returns TRUE,
		// but if we was on the last record then next record record was not found,
		// so it returns FALSE and the current record is not changed.
		//
		// So this methods move record pointer in table, and LOAD record from disk
		// into RAM buffer of record. 

								/** Make the first logical record of table current. */
virtual bool					FirstRecord( void ) = 0;

								/** Make the last logical record of table current. */
virtual bool 					LastRecord( void ) = 0;

								/** Make the next logical record of table current. */
virtual bool 					NextRecord( void ) = 0;

								/** Make the prev logical record of table current. */
virtual bool 					PrevRecord( void ) = 0;

								/** Size of client record buffer. */
virtual void					put_RecordBufferSize( vuint32 inRecordCount )  = 0;
virtual vuint32					get_RecordBufferSize( void ) const = 0;


	// ---------------------
	// Selection methods:

								/** Build and returns BitSet that show all existed logical records. */
virtual BitSet_Ptr				SelectAllRecords( void ) const = 0;

								/** Make empty BitSet of size corresponded to the number
									of physical records in table. */
virtual BitSet_Ptr				SelectNoneRecords( void ) const = 0;

								/** Get result set without inSet. */
virtual BitSet_Ptr				InvertSelection( BitSet_Ptr inSet ) const = 0;


	// ---------------------
	// Record Sorting Methods:	

                                /** Execute non-physical sorting of Table.
                                    You get sorted ArraySet as result of sorting.
                                    You can use it later to iterate records in the specified sort order.
                                    @param inSet - selection of this Table to be sorted. 
                                            Can be NULL, which means sorting of the whole Table.			
                                    @result - Sorted ArraySet of selected records. Can return NULL.
                                    @errors - { ERR_FIELD_NOT_FOUND, ERR_FIELD_CANNOT_BE_SORTED, 
                                                ERR_OUT_OF_MEMORY }	*/	
virtual ArraySet_Ptr			Sort(
									Set_Ptr			inSet,
									I_Field_Ptr		inSortField,
									bool			inAsc = true ) = 0;


								/** The same as above, but can do sorting on several fields.
									@param inSortOrder - specify fields and direction of sorting. */
virtual ArraySet_Ptr			Sort(
									Set_Ptr			inSet,
									SortOrder_Ptr	inSortOrder ) = 0;


	// ---------------------
	// Utility Methods:

                                //! Swap segments of db volumes to remove fragmentation.
                                //! Do not affect size of database.
                                //! Should be used after Compact().
virtual void	 				Defragment( void ) = 0;

                                /**	Performs diagnose of the table. Writes
                                    diagnostic information to the stream accordingly 
                                    specified verbose level. 
                                    @param inOut - stream for output messages.
                                    @param inLevel - specifies the amount of diagnostic messages
                                        to be produced during operation. */
virtual bool 					Diagnose(
									I_OStream_Ptr inOut,
									EVerboseLevel inLevel = kVerbose_Normal ) = 0;

virtual bool 					Diagnose(
									I_Location_Ptr	inReportLocation,
									EVerboseLevel	inLevel = kVerbose_Normal ) = 0;

								/// Compact the table.
virtual void 					Compact( void ) = 0;

virtual void					Reindex( void )  = 0;

								/// Repaire the table.
virtual void 					Repair( void ) = 0;

								/// Callbasck method on database destruction.
virtual	void					OnDatabaseDestroy( void ) = 0;


	// ---------------------
	// Dump/LoadDump:

                                /**	Dumps the table to the specified disk location.
                                    Produce XML or SQL dump as specified.

                                    @param inDumpLocation - location of dump file that must be loaded.
                                    @param inDumpType - kXML or kSQL dump.
                                    @param inDumpData - specify if should be dumped only structure, records or both.
                                    @param inFormatDump - if TRUE then produce formated dump file, which can be easy read 
                                                by human, but such file has bigger size, because contains more spaces and TABs.
                                */
virtual	void					Dump(
                                    I_Location_Ptr	inDumpLocation,
                                    DUMP_TYPE		inDumpType,
                                    EDumpData		inDumpData = kStructureAndRecords,	
                                    bool			inFormatDump = true,
                                    OS_Type			inOSType = kOsDefault,
                                    const char*		inEncoding = "UTF-16" ) = 0;


                                /** Loads the specified dump file.
                                    If table has fields then its structure must be the same as specified 
                                    in dump otherwise error will be thrown. If the table has no fields then 
                                    all the fields specified in the dump will be created. Finally the data 
                                    will be loaded to that table.

                                    @param inTable - pointer to the existing table object.
                                    @param inDumpLocation - location of dump file that must be loaded.
                                    @param inDumpType - kXML or kSQL dump. */
virtual	void					LoadDump(
									I_Location_Ptr	inDumpLocation,
									DUMP_TYPE		inDumpType,
									const char*		inEncoding = "UTF-16" ) = 0;

	// ---------------------
	// Import/Export:

                                /**	Exports records from the given inTable to the specified location.
                                    @param inType - desired export type.
                                    @param inTable - exported table.
                                    @param inLocation - location of the file.
                                    @param inContainer - exporter-specific configuration options.
                                    @return the number of successfully exported records. */
virtual	vuint32					Export(
                                    EXPORTER_TYPE 		inType,
                                    I_Location_Ptr		inLocation,
                                    Const_I_PropertyContainer_Ptr inContainer = nullptr ) = 0;


                                /**	Imports records from the specified location into the table.
                                    @param inType	- type of imported source.
                                    @param inTable	- table that will be used as import target.
                                    @param inLocation - import source location.
                                    @param inContainer - import properties.
                                    @param inRecToImport - number of records to import. 
                                        Default value 0 means "import all records". */
virtual	vuint32					Import(
                                    IMPORTER_TYPE 	inType,
                                    I_Location_Ptr	inLocation,
                                    Const_I_PropertyContainer_Ptr inContainer = nullptr,
                                    vuint32			inRecToImport = 0 ) = 0;

                                /**	Imports records from the specified location into the table.
                                    @param inOptions	- init flags for ImportResult (kNone, kWarnings, kOutputs).
                                    @param inType	- type of imported source.
                                    @param inTable	- table that will be used as import target.
                                    @param inLocation - import source location.
                                    @param inContainer - import properties.
                                    @param inRecToImport - number of records to import. 
                                        Default value 0 means "import all records". */
virtual	ImportResult_Ptr		Import(
                                    const vuint16	inOptions,
                                    IMPORTER_TYPE 	inType,
                                    I_Location_Ptr	inLocation,
                                    Const_I_PropertyContainer_Ptr inContainer = nullptr,
                                    vuint32			inRecToImport = 0 ) = 0;
	
	// ---------------------
	// KeyValue:

virtual I_KeyValue_Ptr			get_KeyValue( vuint32 inIndex ) const = 0;
virtual I_KeyValue_Ptr			get_KeyValue( const String& inName ) const = 0;
virtual I_KeyValue_Ptr			GetKeyValueByID( vint32 inIndex ) const = 0;

virtual	I_KeyValue_Ptr			CreateKeyValue(
									const String&	inName,
									vuint64			inOptions = fKeyValueNone ) = 0;
									
virtual void					DropKeyValue( I_KeyValue_Ptr inKeyValue ) = 0;
	
};


#pragma mark -


/**********************************************************************************************/
template <class Function>
void ForEachField( I_Table_Ptr inTable, Function f )
{
	vuint16 Count = inTable->	get_FieldCount();
	for( vuint16 i = 1; i <= Count ; ++i )
	{
		I_Field_Ptr pField = inTable->	get_Field( i );
		try
		{
			f( pField.get() );
		}
		catch(...)
		{
			// Do nothing.
			// We need it just to be sure we have done the f for each field.
		}
	} 
}


/**********************************************************************************************/
I_Table_Ptr FindTable( 
	const ArrayOfTables_Ptr	inTables, 
	const String&			inTableName	);


/**********************************************************************************************/
// Util stack class to simplify disbale/enable indexing of table.
class StStopIndexChanges
{
	public://///////////////////////////////////////////////////////////////////////////////////
 
							StStopIndexChanges( I_Table_Ptr inTable )
							:
								mpTable( inTable )
							{
								if( mpTable )
									mpTable->put_IndexChanges( false );
							}	
								
							~StStopIndexChanges( void )
							{
								if( mpTable )
									mpTable->put_IndexChanges( true );
							}
							

	protected:///////////////////////////////////////////////////////////////////////////////	/// 

		I_Table_Ptr			mpTable;

};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_I_Table_h
