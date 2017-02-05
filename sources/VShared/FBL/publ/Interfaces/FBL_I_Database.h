/**********************************************************************************************/
/* FBL_I_Database.h 	                                                      				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_Database_h 
	#define _FBL_I_Database_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Interfaces/FBL_I_Connection.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Type.h>

#include <VShared/FBL/publ/Util_classes/FBL_DTFormat.h>
#include <VShared/FBL/publ/Util_classes/FBL_DumpResult.h>


// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// forward declarations:
//
SMART_INTERFACE(I_Error);
SMART_INTERFACE(I_IndexStyle);
SMART_INTERFACE(I_Link);
SMART_INTERFACE(I_Location);
SMART_INTERFACE(I_OStream);
SMART_INTERFACE(I_PropertyContainer);
SMART_INTERFACE(I_Table);
SMART_INTERFACE(I_Value);
//
SMART_ARRAY_OF_SMART_PTRS( ArrayOfTables, I_Table_Ptr );
SMART_ARRAY_OF_SMART_PTRS( ArrayOfLinks, I_Link_Ptr );
SMART_ARRAY_OF_SMART_PTRS( ArrayOfTypes, I_Type_Ptr );


/**********************************************************************************************/
// This file forwards:
//
SMART_INTERFACE(I_Database);
SMART_ARRAY_OF_CLASS_PTRS( ArrayOfDatabases, I_Database* );


/**********************************************************************************************/
/// interface FBL_SHARED_EXP_CLASS class 
//
//StubInterface
interface FBL_SHARED_EXP_CLASS I_Database : public I_Unknown
{
virtual							~I_Database( void );


	// ---------------------
	// Properties:

		// <Error> [r/o]
								/** Returns the last error for this db.
									If no error, then returns NULL. */
virtual	const I_Error*			get_Error( void ) const = 0;


	// ---------------------
	// Database Schema properties:

		// <IndexCount>
								/// Returns the count of indexes in the database.
virtual vuint32					get_IndexCount( void ) const  = 0;

		// <LinkCount> [r/o]
								/// Returns the count of links in the database.
virtual	vuint32					get_LinkCount( void ) const = 0;

		// <SchemaVersion> 	
								/** Schema version of database. 1, 2, 3...
                            		It is stored in the header of DB. */
virtual	vuint16					get_SchemaVersion( void ) const = 0;
virtual	void					put_SchemaVersion( vuint16 inVersion ) = 0;

		// <TableCount> [r/o]
								/// Returns the count of tables in the database.
virtual	vuint32					get_TableCount( void ) const = 0;

		// <TypeCount> [r/o]
								/// Returns the count of types in the database.
virtual	vuint32					get_TypeCount( void ) const = 0;

	
	// ---------------------
	// DateTime properties:

		// <DateFormat> [r/w]
virtual	EDateFormat				get_DateFormat( void ) const = 0;
virtual	void					put_DateFormat( EDateFormat inValue ) = 0;

		// <DateSep> [r/w]  // On default '/'
virtual	UChar					get_DateSep	 ( void ) const = 0;
virtual	void					put_DateSep	 ( UChar inValue ) = 0;

		// <TimeSep> [r/w]	// On default ':'
virtual	UChar					get_TimeSep	 ( void ) const = 0;
virtual	void					put_TimeSep	 ( UChar inValue ) = 0;

		// <CenturyBound> [r/w]	// On default 20
		// This parameter allows to Valentina db engine covert string with year in format YY
        // to YYYY format. For example "79" => 1979, "14"=>2014.

virtual	vuint16					get_CenturyBound( void ) const = 0;
virtual	void					put_CenturyBound( vuint16 inValue ) = 0;

virtual	vuint32					get_Century1( void ) const = 0;
virtual	void					put_Century1( vuint32 inValue ) = 0;

virtual	vuint32					get_Century2( void ) const = 0;
virtual	void					put_Century2( vuint32 inValue ) = 0;

		// <DTFormat> [r/o]
virtual	const DTFormat*			get_DTFormat( void ) const = 0;


	// ---------------------
	// Database Storage Properties:

		// <BigEndian> 
								/// Returns endian of database files.
virtual bool					get_BigEndian( void ) const  = 0;

                               	/// Allow assign before db.Create() the endian for db files.
                               	/// If it is not called before db.Create() then current OS endian is used.
virtual void					put_BigEndian( bool inValue )  = 0;

		// <Exists> [r/o]
                                /** Return TRUE if this Storage already have files on device.
                                    After Create() must be TRUE.
									After ThrowOut() must be FALSE. */
virtual bool					get_Exists( void ) const  = 0;

		// <IsOpen> [r/o]
								/** Returns TRUE if storage is opened now.
									Must be TRUE after Open(), must be FALSE, after Close(). */
virtual	bool					get_IsOpen( void ) const = 0;
		
		// <IsReadOnly> [r/w]
								/** TRUE if Storage is located on locked volume,
									or its files marked as read-only. */
virtual	bool					get_IsReadOnly( void ) const = 0;

								/** Db can be RO even in case of RW-able storage. */
virtual	void					put_IsReadOnly( bool inValue ) = 0;

		// <IsRemote> [r/o]
								/** TRUE if DB is located on server. */
virtual bool					get_IsRemote( void ) const = 0;

		// <Location>
								/// Returns the Location of this db storage.
virtual	I_Location_Ptr			get_Location( void ) const  = 0;

								/** Assign location for this db storage. */
virtual void					put_Location( I_Location_Ptr inLocation ) = 0;

		// <Name> [r/o]
                                /** Returns the name of database. In fact this is the name of db file.
                                    This is not the same as Path of db file.
                                    Cannot be changed, because this may require changing of name
                                    for several disk files. If you need change db name, use OS. */
virtual	const String&			get_Name( void ) const = 0;	//OLDKEEPAS: mName

		// <RBJournal path>

virtual I_Location_Ptr			get_RBJournalFolder( void ) const = 0;

virtual void					put_RBJournalFolder( I_Location_Ptr inpFolder ) = 0;

		// <Mode>						
								/// Returns the Mode for this db.
virtual	DbMode					get_Mode( void ) const  = 0;

                                /** Assign the Mode for db volumes.
                                    Can be used only before Create().
                                    Cannot change mode of existed db (at least now). */
virtual void					put_Mode( DbMode inMode )  = 0;

		// <SegmentSize>
								/// Returns the segment size of Volume(s) for this db in bytes.
virtual	vuint32 				get_SegmentSize( void ) const  = 0;

                                /** Assign the segment size for db Volumes.
                                    Can be used only before Create().
                                    Cannot change segment size of existed db (at least now). */
virtual void					put_SegmentSize( vuint32 inSize ) = 0;

		// <Size> [r/o]
								/// Returns size of Database in bytes.
virtual flength					get_Size( void ) const  = 0;
    

	// ---------------------
	// Rollback Journal Properties:

								/// Returns true if db is journaled (db changes go to the rollback log).
virtual	bool	 				get_Journaled( void ) const  = 0;

								/// On/Off rollback journal to the database.
virtual void					put_Journaled( bool inValue ) = 0;

	
	// ---------------------
	// Rollback Journal Methods:

								/// Returns true if closed db will be rolled back on the db.Open() call.
								/// In the rest of cases returns false.
virtual bool					CheckRollback( void )  = 0;



	// ---------------------
	// Storage Methods:

								/// Creates files of this object on Device.
virtual	void					Create( void ) = 0;
		
								/// Opens files of this object on Device.
virtual void 					Open( void ) = 0;

								/// Closes files of this object.
virtual void 					Close( void ) = 0;

								/// Flushes files of object from Cache to Device if needed.
virtual	void					Flush( void ) = 0;

								/// Removes files of this object from Device.
virtual void 					ThrowOut( void ) = 0;

								/// Clone this db to the new one.
virtual void 					CloneDB(
									I_Database_Ptr 	inTargetDB,
									bool 			inLoadRecords ) = 0;
								
virtual void 					CloneDB(
									I_Location_Ptr 	inTargetLocation,
									bool 			inLoadRecords ) = 0;



	// ---------------------
	// Table Methods:
								/// Returns a table by index (1 - based) or by name.
								/// Returns NULL if table not found.
virtual I_Table_Ptr				get_Table( vuint32 inIndex ) const  = 0;
virtual I_Table_Ptr				get_Table( const String& inTable ) const  = 0;

								/** Returns a table using its unique ID.
									For a tmp table ID is negative.
									Returns NULL if table is not found. */
virtual I_Table_Ptr				GetTableByID( vint32 inIndex ) const  = 0;

    							// For C++11 'range-based for loop'.
virtual const ArrayOfTables& 	get_Tables( void ) const = 0;


	// ---------------------
	// Link Methods:

virtual I_Link_Ptr				get_Link( vuint32 inIndex ) const  = 0;
virtual I_Link_Ptr				get_Link( const String& inTableName ) const  = 0;

								/** Returns a link using its unique ID.
									Returns NULL if link is not found. */
virtual I_Link_Ptr				GetLinkByID( vint32 inIndex ) const  = 0;

    							// For C++11 'range-based for loop'.
virtual const ArrayOfLinks&		get_Links( void ) const = 0;


	// ---------------------
	// Type Methods:

virtual I_Type_Ptr				get_Type( vuint32 inIndex ) const = 0;
virtual I_Type_Ptr				get_Type( const String& inTypeName ) const = 0;

								/** Returns a type using its unique ID.
									Returns NULL if type is not found. */
virtual I_Type_Ptr				GetTypeByID( Type_ID inTypeID ) const = 0;

    							// For C++11 'range-based for loop'.
virtual const ArrayOfTypes&		get_Types( void ) const = 0;


	// ---------------------
	// Value Methods:

                                /**	This is a virtual factory of Values.
                                    This allows to create values, which know if they are from
                                    VKernel or from VClient DLL. */
virtual I_Value_Ptr 			CreateValue(
                                    VALUE_TYPE 	inType,
                                    vuint16		inFlags,
                                    void*		inData = nullptr) = 0;

	// ---------------------
	// DB Schema Methods:
	
                                /** Creates a new Table in the Database.
                                    You can ask for tmp table. Such Table will be automatically
                                    destroyed on the next open, in case of app failure. 
                                    Also you can specify the storage for this Table.
                                    Note, for RAM-based db, we cannot create Disk-based table. 
                                    
                                    Errors:
                                        xTableError{ ERR_TABLE_BAD_NAME, ERR_TABLE_ALREADY_EXISTS }	*/
virtual	I_Table_Ptr				CreateTable(
                                    const String& 		inName,
                                    ETableKind			inTableKind = kTblPermanent,
                                    EStorageType		inStorage = kStorage_Default,
                                    TableStructure		inTableStructure = kTable_Separate ) = 0;

								/* Removes table from database.
									Errors:
										xConstraintError{ ERR_CONSTRAINT_PRIMARY_KEY_HAVE_FOREIGN_KEY } */
virtual	void					DropTable( I_Table_Ptr inTable ) = 0;

                                /** Creates a new Link in the Database.
                                    Errors:
                                        xLinkError{ ERR_LINK_BAD_NAME, ERR_LINK_NAME_NOT_UNIQUE } */
virtual	I_Link_Ptr				CreateLink(
                                    const String&					inName,
                                    ELinkKind						inKind,
                                    Const_I_PropertyContainer_Ptr	inProperties,
                                    bool							inTemporary = false, 
                                    bool							inRegisterInTable = true) = 0;

								/// Remove link from database.
virtual	void					DropLink( I_Link_Ptr inLink ) = 0;

                                /** Creates a new Type in the Database.
                                    Errors:
                                        xTypeError{ ERR_TYPE_BAD_NAME, 
                                                    ERR_TYPE_UNKNOWN_CATEGORY,
                                                    ERR_TYPE_NAME_NOT_UNIQUE } */
virtual I_Type_Ptr				CreateType(
                                    const String&		inTypeName,
                                    TYPE_CATEGORY		inCategory ) = 0;


								/// Remove type from database.
virtual void	 				DropType( I_Type_Ptr inType ) = 0;


	// ---------------------
	// IndexStyle API methods:

virtual IndStyle_ID 			GetNextIndStyleID( bool inTemporary )  = 0;

virtual I_IndexStyle_Ptr		get_IndexStyle_ByIndex( vuint32 inIndex ) const  = 0;

                                /** Returns an object of IndexStyle with the specified name.
                                    If not found then returns NULL.	*/
virtual I_IndexStyle_Ptr		get_IndexStyle( IndStyle_ID inIndStyleID ) const  = 0;
virtual I_IndexStyle_Ptr		get_IndexStyle( const String& inName ) const  = 0;

virtual vuint32					get_IndexStyleCount( void ) const  = 0;

								/** Returns a new object of IndexStrPeefs.
									@error: ErrNameNotUnique. */
virtual	I_IndexStyle_Ptr		CreateIndexStyle(
									const String& 	inName,
									bool			inTmp = false ) = 0;

								/** Remove from database the specified Prefs */
virtual void					DropIndexStyle( I_IndexStyle_Ptr inPrefs ) = 0;
	
	// ---------------------
	// Utility methods:

								/// Compact the database.
virtual void 					Compact( void ) = 0;

								//! Swap segments of db volumes to remove fragmentation.
								//! Do not affect size of database.
								//! Should be used after Compact().
virtual void 					Defragment( void ) = 0;

                                /**	Performs diagnose of the database. Writes
                                    diagnostic information to the stream accordingly 
                                    specified verbose level. 
                                    @param inOut - stream for output messages.
                                    @param inLevel - specifies the amount of diagnostic messages
                                        to be produced during operation. */
virtual bool 					Diagnose(
                                    I_OStream_Ptr inOut,
                                    EVerboseLevel inLevel = kVerbose_Normal ) = 0;

                                /**	Performs diagnose of the database. Writes
                                    diagnostic information to the inReportLocation accordingly 
                                    specified verbose level. 
                                    @param inReportLocation - location for output messages.
                                    @param inLevel - specifies the amount of diagnostic messages
                                    to be produced during operation. */
virtual bool 					Diagnose(
                                    I_Location_Ptr	inReportLocation,
                                    EVerboseLevel	inLevel = kVerbose_Normal ) = 0;

virtual void					Reindex( void )  = 0;

								/// Repaire the database.
virtual void 					Repair( void ) = 0;

								// Assign 4-letters MAC types for db files.
virtual void					SetMacTypes(
                                    OSType inDescType,
                                    OSType inDatType, 
                                    OSType inBlbType, 
                                    OSType inIndType ) = 0; 

	// ---------------------
	// Dump/LoadDump:


                                /**	Dumps the database to the specified disk location.
                                    Produces XML or SQL dump as specified.

                                    @param inDumpLocation - location of dump file that must be created.
                                    @param inDumpType - kXML or kSQL dump.
                                    @param inDumpData - specify if should be dumped only structure, records or both.
                                    @param inFormatDump - if TRUE then produce formatted dump file, which can be easy read 
                                                by human, but such file has bigger size, because contains more spaces and TABs.
                                     @param inEncoding - dump file encoding. */
virtual void					Dump(
                                    I_Location_Ptr	inDumpLocation,
                                    DUMP_TYPE		inDumpType,
                                    EDumpData		inDumpData = kStructureAndRecords,	
                                    bool			inFormatDump = true,
                                    OS_Type			inOSType = kOsDefault,
                                    const char*		inEncoding = "UTF-16" ) = 0;


                                /**	Loads the specified dump file into a new fresh database.
                                    LoadDump() creates disk files of this db, creates its structure according to dump
                                    and imports records from dump into created tables.

                                    @param inDumpLocation - location of dump file that must be loaded.
                                    @param inNewDbLocation - location for new database.
                                    @param inDumpType - kXML or kSQL dump. 
                                    @param inEncoding - dump file encoding. */
virtual void					LoadDump(
                                    I_Location_Ptr	inDumpLocation,
                                    I_Location_Ptr	inNewDbLocation,
                                    DUMP_TYPE		inDumpType,
                                    const char*		inEncoding = "UTF-16" ) = 0;

virtual DumpResult_Ptr			LoadDump(
                                    const vuint16	inOptions,
                                    I_Location_Ptr	inDumpLocation,
                                    I_Location_Ptr	inNewDbLocation,
                                    DUMP_TYPE		inDumpType,
                                    const char*		inEncoding = "UTF-16") = 0;

	// ---------------------
	// Last inserted RecID:

virtual	REC_ID					get_LastInserted_RecID( void ) const = 0;

}; 


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_I_Database_h
