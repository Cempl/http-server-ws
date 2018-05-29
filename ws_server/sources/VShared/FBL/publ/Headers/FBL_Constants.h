/**********************************************************************************************/
/* FBL_Constants.h 	                                                      					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_Constants_h 
	#define _FBL_Constants_h
#pragma once


/**********************************************************************************************/
FBL_Begin_Namespace


//----------------------------------------------------------------------------------------------
// VOLUME CONSTANTS:
//----------------------------------------------------------------------------------------------
//
// Constants, which are stored in the volume header to define native platform of database file.
//
enum OS_Type
{	
	kOsDefault	= 0,
	kOsMac		= 1,
	kOsWindows	= 2,	
	kOsUnix		= 3	
};

#if FBL_MAC
#	define kCurrentOS 	kOsMac
#elif FBL_WIN
#	define kCurrentOS 	kOsWindows
#else
#	define kCurrentOS 	kOsUnix
#endif


//----------------------------------------------------------------------------------------------
// Define the size of a page for disk file of an OS.
//
const vuint32 kPageSizeBits 	= 12;
const vuint32 kPageSize 		= 1 << kPageSizeBits;		// 1 << 12 = 4Kb


/**********************************************************************************************/
enum EDumpSortOrder
{
	kSchemaDefined		= 0,
	kSorted				= 1
};


//----------------------------------------------------------------------------------------------
// DATABASE CONSTANTS:
//----------------------------------------------------------------------------------------------
//
//// Storage types for create database and tables.
//
enum EStorageType
{
	kStorage_Default = 0,	// default for current database.
	kStorage_Disk,
	kStorage_RAM			
};


//----------------------------------------------------------------------------------------------
/// Constants for parameter MODE of method Database::Create():
//
enum DbMode
{
	kDscDatBlbInd		= 1,	///< (description,data,BLOB,indexes)
	kDsc_DatBlbInd 		= 2,	///< description + (data,BLOB,indexes)
	kDsc_DatBlb_Ind		= 3,	///< description + (data,BLOB) + indexes
	kDsc_Dat_Blb_Ind	= 4,	///< description + data + BLOB + indexes
	kDscDatBlb_Ind		= 5,	///< (description,data,BLOB) + indexes
	kDscDat_Blb_Ind		= 6,	///< (description,data) + BLOB + indexes
	kDscDatInd_Blb		= 7,	///< (description,data,indexes) + BLOB 
	kDsc_DatInd_Blb		= 8 	///< description + (data,indexes) + BLOB 
};


//----------------------------------------------------------------------------------------------
//
// ATTENTION!!! Segment size must be multiple of kMinSizeOfSegment!
//	Because of kSizePage which is a minimal data size for RB journal. 
//
//

//----------------------------------------------------------------------------------------------
/// The default size of volume segment is 32 Kb (8 pages):
//
const vuint32 kSizeOfSegment = 8 * kPageSize;	

//----------------------------------------------------------------------------------------------
/// The maximum segment size for description volume if it is not kDscDatBlbInd mode (single volume)
//
const vuint32 kMaxSizeOfSegment_DescVolume = kPageSize;	

//----------------------------------------------------------------------------------------------
/// The minimum segment size for volume. 
//
const vuint32 kMinSizeOfSegment = kPageSize; 
 
//----------------------------------------------------------------------------------------------
// TABLE CONSTANTS:
//----------------------------------------------------------------------------------------------

/// This enum is used in the Database.CreateTable() method.
/// Specifies the kind of table.
/// Permanent table is stored on the .dat volume. This is regular table of user.
/// Temporary table is stored on .tmp volume.
/// System table table is stored on .vdb volume. For internal use.
//
enum ETableKind		
{
	kTblPermanent = 0,		
	kTblTemporary = 1,		
	kTblSystem	  = 2,
	kTblResultTable = 3			// It works like kTblTemporary but has NO service fields (RecID, OID) 
								// For internal use in SQL.
};


/**********************************************************************************************/
const TableStructure	kTable_Separate		 = 1; ///< Fields are stored in the separate files.
const TableStructure	kTable_Solid	     = 2; ///< Fields as solid record. Not implemented.
const TableStructure	kTable_BitSet 		 = 3; ///< @internal 
const TableStructure	kTable_ArraySet		 = 4; ///< @internal
const TableStructure	kTable_Join_ArraySet = 5; ///< @internal
const TableStructure	kTable_Join_File	 = 6; ///< @internal


//----------------------------------------------------------------------------------------------
// Constants for RECORD LOCKs
//
const LOCK_TYPE kLock_None			= 0;	///< Record not locked.
const LOCK_TYPE kLock_Shared		= 1;	///< Shared lock. Used for read operations.
const LOCK_TYPE kLock_Exclusive 	= 2;	///< Exclusive lock. Used for change operations.
const LOCK_TYPE kLock_Update		= 3;	///< Update lock. Informs on intention to set up exclusive lock.


//----------------------------------------------------------------------------------------------
// Types of JOIN:
//
const JOIN_TYPE	kJoin_LeftOuter		= 0;
const JOIN_TYPE	kJoin_RightOuter	= 1;
const JOIN_TYPE	kJoin_FullOuter		= 2;
const JOIN_TYPE	kJoin_Inner			= 3;


/**********************************************************************************************/
// Possible kind of constraint. Order of this enum items - is sort order of priority to check.
//
enum EConstraintKind
{
	kConstraint_PrimaryKey,
	kConstraint_Unique,
	kConstraint_Check,
	kConstraint_ForeignKey,
	kConstraint_Index,
	kConstraintKindCount		// Special value - just to know how many items in the enum!
};


/**********************************************************************************************/
//	Level definition constraints. 
//
enum EConstraintLevel
{
	kFieldLevel,
	kTableLevel
};


//----------------------------------------------------------------------------------------------
// FIELD CONSTANTS:
//----------------------------------------------------------------------------------------------

// Types of database values
// All types of values less than 1000 are reserved by a KERNEL for future use.
// All user types should have the identifier more than 1000.
//
const VALUE_TYPE	kTypeEmpty		= 0;	// just a named constant.

	// integer types
const VALUE_TYPE	kTypeEnum		= 1;	// 1 or 2 bytes as specified by user.
const VALUE_TYPE	kTypeBoolean	= 2;	// 1 bit boolean.
const VALUE_TYPE	kTypeByte		= 3;	// 1 byte, unsigned integer
const VALUE_TYPE	kTypeShort		= 4;	// 2 bytes signed integer
const VALUE_TYPE	kTypeUShort		= 5;	// 2 bytes unsigned integer
const VALUE_TYPE	kTypeMedium		= 6;	// 3 bytes signed integer
const VALUE_TYPE	kTypeUMedium	= 7;	// 3 bytes unsigned integer
const VALUE_TYPE	kTypeLong		= 8;	// 4 bytes signed integer
const VALUE_TYPE	kTypeULong		= 9;	// 4 bytes unsigned integer
const VALUE_TYPE	kTypeLLong		= 10;	// 8 bytes signed integer
const VALUE_TYPE	kTypeULLong		= 11;	// 8 bytes unsigned integer

	// real number types
const VALUE_TYPE	kTypeFloat		= 12;	// 4 bytes real number
const VALUE_TYPE	kTypeDouble		= 13;	// 8 bytes real number
const VALUE_TYPE	kTypeLDouble	= 14;	// NOT IMPLEMENTED, 10 bytes real number.
const VALUE_TYPE	kTypeDecimal	= 15;	// Alias for kTypeLDouble

	// date time types
const VALUE_TYPE	kTypeDate		= 16;	// 4 bytes, date.
const VALUE_TYPE	kTypeTime		= 17;	// 4 bytes, time.	
const VALUE_TYPE	kTypeDateTime	= 18;	// 8 bytes, date_time.

	// string types
const VALUE_TYPE	kTypeString		= 19;	// fixed sring, up to 64KB.
const VALUE_TYPE	kTypeVarChar	= 20;	// variable string, ut to 64KB.

	// small binary types
const VALUE_TYPE	kTypeFixedBinary= 21;	// fixed binary, up to 64KB.
const VALUE_TYPE	kTypeVarBinary	= 22;	// variable binary, up to 64KB.
	
	// large binary types
const VALUE_TYPE	kTypeBLOB		= 23;	// BLOB, unlimited.
const VALUE_TYPE	kTypeText		= 24;	// BLOB that store text, can be indexed.
const VALUE_TYPE	kTypePicture	= 25;	// BLOB, that store pictures.
const VALUE_TYPE	kTypeSound		= 26;	// NOT IMPLEMENTED
const VALUE_TYPE	kTypeMovie		= 27;	// NOT IMPLEMENTED


	// Valentina specific types
const VALUE_TYPE	kTypeRecID		= 28; 	// 4 bytes. Virtual field, returns RecID of the current record.
const VALUE_TYPE	kTypeOID		= 29; 	// 8 bytes. Virtual field, returns OID of the current record.

const VALUE_TYPE	kTypeObjectPtr	= 30;	// 4 bytes. Pointer on RecID values.
const VALUE_TYPE	kTypeObjectsPtr	= 31;	// 8 bytes. Pointer on OID values.

const VALUE_TYPE	kTypeTimeStamp	= 32;	// 8 bytes. Store unsigned integer value of internal
											// database "clocks". This is not real time.

const VALUE_TYPE	kTypeEnum8		= 33;	// 1 byte. Enum on byte. Added in v5.0.
const VALUE_TYPE	kTypeEnum16		= 34;	// 2 bytes. Enum on ushort. Added in v5.0.

const VALUE_TYPE	kTypeSerial32	= 35;	// 4 bytes. Pseudo-type for VSQL. DO NOT USE directly!! Added in v5.0.
const VALUE_TYPE	kTypeSerial64	= 36;	// 8 bytes. Pseudo-type for VSQL. DO NOT USE directly!!. Added in v5.0.

const VALUE_TYPE	kTypeMoney		= 37;	// 8 bytes. Stored as kTypeLLong. Added in v6.0.

const VALUE_TYPE	kTypeVariant	= 38;	// variant type. Added in v.7.0

const VALUE_TYPE	kTypeCompound	 = 99;	// Used for Compound Values. Added in v5.0.
	
const VALUE_TYPE	kTypeUserDefined = 100;	// Marks a User Defined Type. 
											// It is clear that DB ENGINE nothing knows about 
											// these possible types.

const VALUE_TYPE	kPseudoTypeSegmentNumber = 101;	// There is NO value for this pseudo-type
											// We use it in Variant varchar-storage to differ real
											// variant-value and segment number (where real
											// variant-value stored in segment file).




//----------------------------------------------------------------------------------------------
// The following constants can be used for SetFlags() and GetFlags().
// You can use combination of this flags for single call.
//
enum FieldAttributes	
{
	// r - you can use this flag in CheckFlags() method. Actually you can read any flag.
	// w - you can use this flag in SetFlags()/ClearFlags() methods.

	fNone					= 0,		// (r/w) There is no any flags.
	
	fNullable				= 1 << 0,   // (r/w) True if the field must keep NULL values.

	fIndexed      			= 1 << 1,	// (r/w) Field has secondary index.
	fIndexUnique   			= 1 << 2,	// (r/w) Field is unique.
	fIndexByWords			= 1 << 3,	// (r/w) For string and text fields only.
	
	fCompressed				= 1 << 4,	// (r/w) For BLOBs, specify if must be compressed.	

	// Read only flags:
		
	fMethod 				= 1 << 5,	// (r/ ) Value of field is counted using values of other fields  
										// of the same record of BaseObject, it is not saved in the table,
										// but can have index. You can use this flag only for CheckFlags().
	
	fIdentity				= 1 << 6,	// (r/) Method dependednt on RecID (AutoIncrement).
	fCursorRecID			= 1 << 7,	// (r/w) RecID field for Cursor. (Temporary till we have lazy
										// method evaluating).
	fTemporary				= 1 << 8,	// (r/ ) On Creation of field allow specify field to be TEMPORARY.
										// Such field is not stored into sys tables, and its data go to .tmp volume!
				
	fIndexHashed			= 1 << 9	// (r/w) For string/varchar and binary/varbinary fields only.
};


/**********************************************************************************************/
enum EIndexType
{
	IndexType_kUnknown,
	IndexType_kDefault,
	IndexType_kHash
};


//----------------------------------------------------------------------------------------------
const vuint32 kMaxLengthOfVarChar_Bytes	= 4088;	// maximal size in bytes of string we can store 
//
const vuint32 kMaxLengthOfVarChar_UTF16	= 2044;	// UTF16 eats 2 bytes per character.
const vuint32 kMaxLengthOfVarChar_UTF8	= 4088;	//


//----------------------------------------------------------------------------------------------
const vuint32 kMaxLengthOfIndexValue = 127;		// maximal size of value in the index (symbols)


//----------------------------------------------------------------------------------------------
/// This enum specify constants that can be used to define sort order of a field. 
//
enum EOrder 
{
	kDesc	= 0,		///< Descending sort order.
	kAsc 	= 1			///< Ascending sort order.
};


//----------------------------------------------------------------------------------------------
/// This enum specify constants that can be used to define sort and null order. 
//
enum EOrderNulls
{
	kNullsFirst	= 0,		///< order: nulls first.
	kNullsLast 	= 1			///< order: nulls last.		(by default)
};


//----------------------------------------------------------------------------------------------
// Constants for a Picture field (only kJPG and kWinDIB are used for now).
//
enum PictType
{
	kUnknown 			= 0,
	kMacPict 			= 1,
	kMacCICN			= 2,
	kMacIconSuite		= 3,
	kMacGWorld			= 4,
	
	kWinDIB				= 10,
	
	kJPG				= 20,
	kTIFF				= 21,
	kPNG				= 22,
	kBMP				= 23,
	kGIF				= 24,
	kPCX				= 25,
	kPNM				= 26,
	kXBM				= 27,
	kXPM				= 28,
	kSVG				= 29,
	kTGA				= 30
};


//----------------------------------------------------------------------------------------------
// Constants for specify report printing format
//
enum ReportPrintType
{
	kToUnknown 			= 0,
	kToMetafile			= 1,

	kToPostscript		= 2,
	kToPDF				= 3,
	kToHTML				= 4,
	kToLatex			= 5,
	kToSVG				= 6,
	
	kToPicture_GIF		= 100,
	kToPicture_JPG		= 101,
	kToPicture_PNG		= 102,	
	kToPicture_TIFF		= 103,	
	kToPicture_BMP		= 104	
};


//----------------------------------------------------------------------------------------------
// For methods BaseObject.get_FieldCount() 
//
enum FieldKind						
{
	kAll 		= 0,	///< Return as Real Fields so Methods of BaseObject.
	kFields		= 1,	///< Return only Real fields of BaseObject.
	kMethods	= 2		///< Return only Methods of BaseObject.
};


//----------------------------------------------------------------------------------------------
// Define Language of Method Text (we use this when create calculated fields).
//
const TMETHOD_LANGUAGE kMethod_SQL 			= 1;
const TMETHOD_LANGUAGE kMethod_Java 		= 2;	// NOT IMPLEMENTED
const TMETHOD_LANGUAGE kMethod_Python 		= 3;	// NOT IMPLEMENTED


//----------------------------------------------------------------------------------------------
// LINK CONSTANTS:
//----------------------------------------------------------------------------------------------
/// This enum is used in the Create_Binary_Link_Properties() method.
/// Specifies the type of link.
/// * System link is stored on .vdb volume. For internal use.
/// * Permanent link is stored:
/// 	* on the .dat volume. This is a regular link of user.
/// 	* on the .tmp volume. This is a temporary link created by user.
//
enum ELinkStorageKind
{
	kLnkPermanent = 0,		
	kLnkSystem	  = 1
};

enum ELinkKind
{
	kObjectPtr,					///< ObjectPtr field.
	kObjectsPtr,				///< ObjectsPtr field.
	kBinaryRelation,			///< Valentina specific Binary Link between 2 tables.
	kRdbRelation,				///< RDB-link between 2 Tables, FOREIGN KEY.
	kPredicateRelation,			///< Predicate-link between 2 Tables.
	kBinaryWithOrderRelation	///< Valentina specific Binary Link With Order between 2 tables.
};


enum EDropBehavior
{
	kdbCascade,
	kdbRestrict	
};


enum ELinkDirection
{
    kFrom1To1,
    kFrom1ToMany,
    kFromManyTo1,
    kFromManyToMany,
    kSelfRecursion,
	kInvalidLinkDirection
};


enum ERecursionDirection
{
    kFromParentToChild,
	kFromChildToParent
};


//----------------------------------------------------------------------------------------------
enum ESearch
{
    kPreferIndexed,
	kPreferNonIndexed
};


//----------------------------------------------------------------------------------------------
// Set of constants that specify behavior of deletion of records in related MANY table
// when a parent record in ONE table is deleted. 
//
enum EOnDeletion
{
	kOnDelete_NoAction	= 3, ///< Synonym for kOnDelete_Restrict.
	kOnDelete_SetNull	= 1, ///< Allow delete "key" record with setting ptr in child table to NULL.
	kOnDelete_Cascade	= 2, ///< Allow delete "key" record with deletion of all linked ptrs.
	kOnDelete_Restrict	= 3, ///< Refuse to delete parent record until it have at least one child record.
	kOnDelete_Default	= 4, ///< Allow delete "key" record setting value of Pointer field to its default value.
	kOnDelete_NoCascade	= 1  ///< Allow delete "key" record just unlinked ptrs from parent (Specific to BinaryLink).	
};

enum EOnUpdate
{
	kOnUpdate_NoAction	= 3, ///< Synonym for kOnUpdate_Restrict.
	kOnUpdate_SetNull	= 1, ///< Pointer in child table must be zeroed.
	kOnUpdate_Cascade	= 2, ///< Child record also must be updated.
	kOnUpdate_Restrict	= 3, ///< Refuse to update parent record until it have at least one child record.
	kOnUpdate_Default	= 4  ///< Set value of Pointer field to its default value.
};


//----------------------------------------------------------------------------------------------
// Named Constants for LinkType parameters:
//
const LinkType kMany	= 0;	
const LinkType kOne		= 1;

//----------------------------------------------------------------------------------------------
// KeyValue CONSTANTS:
//----------------------------------------------------------------------------------------------
enum EKeyValueKind
{
	kKeyValueUnknown,
	kKeyValueDefault,			///< KeyValue for database.
    kKeyValueWithKey,			///< KeyValue WITH KEY.
	kKeyValueForTables,			///< KeyValue for tables.
	kKeyValueForTable,			///< KeyValue for table.
	kKeyValueForLink			///< KeyValue for link.
};



//----------------------------------------------------------------------------------------------
// DUMP CONSTANTS:
//----------------------------------------------------------------------------------------------

/// Supported by the kernel dump types
//
const DUMP_TYPE kSQL		= 1; ///< Dump in SQL-format.
const DUMP_TYPE kXML		= 2; ///< Dump in XML-format.


/// Specify if we need dump structure only, structure and records or records only. 
//
enum EDumpData	
{
	kStructureOnly 			= 1,	
	kStructureAndRecords 	= 2,	
	kRecordsOnly		 	= 3
};


//----------------------------------------------------------------------------------------------
// LOG / DIAGNOSE CONSTANTS:
//----------------------------------------------------------------------------------------------

/// This enum specify constants to be used with LogFiles and Diagnose().
/// It specifies 5 verbose levels.
//  
enum EVerboseLevel
{
	kVerbose_None		= 0,		// Only errors.
	kVerbose_Low		= 1,		// Implementation depends. 
	kVerbose_Normal		= 2,		// As 1.x version.
	kVerbose_High		= 3,		// Implementation depends. 
	kVerbose_VeryHigh	= 4			// Implementation depends.
};

/// This enum specify constants to be used with LogFiles and Diagnose().
/// It specifies 5 objects levels.
//
enum EObjectLevel
{
	kObject_LowLevel		= 4,		// Storages, Files, ...
	kObject_MiddleLevel		= 3,		// Fields, Indexes, Field-level Constraints
	kObject_HighLevel		= 2,		// Tables, Links, Table-level Constraints
	kObject_VeryHighLevel	= 1,		// Database
	kObject_UnknownLevel	= 0			// Unknown level
};


//----------------------------------------------------------------------------------------------
// Warning Log modes
// 
enum EWarningMode
{
	kLOGOFF		= 0,
	kLOGONFile	= 1 << 0,	// Logging option for log file
	kLOGONMsg	= 1 << 1	// Logging option for log message (SqlQueryEx)
};


//----------------------------------------------------------------------------------------------
enum IndexSearchLimit
{
	kNoLimit			= 0,
	kLimitMaxLength		= 1 << 0,	// if max_length < 256 - no index search
	kLimitIndexByWords	= 1 << 1	// if index_by_words - no index search
};


//----------------------------------------------------------------------------------------------
// IMPORT/EXPORT CONSTANTS:
//----------------------------------------------------------------------------------------------
/// Types of exporters
/// NOTE: All types of exporters less than 100 are reserved by a KERNEL for future use.
/// All user types should have the identifier more than 100.
//
const EXPORTER_TYPE	kExp_SeparatedText	= 0;	///< Fields are separated by delimiters.
const EXPORTER_TYPE	kExp_FixedText		= 1;	///< Fields are of fixed len.
const EXPORTER_TYPE	kExp_DBFBase		= 2;	///< File in DBF format.
const EXPORTER_TYPE	kExp_ValentinaBase	= 3;	///< File is in Valentina db format.		

//----------------------------------------------------------------------------------------------
// PRIVILEGE CONSTANTS:
//----------------------------------------------------------------------------------------------
// 
const OBJECT_PRIVILEGES	privNone	= 0;
const OBJECT_PRIVILEGES	privRead	= 1 << 0;
const OBJECT_PRIVILEGES	privInsert	= 1 << 1;
const OBJECT_PRIVILEGES	privUpdate	= 1 << 2;
const OBJECT_PRIVILEGES	privDelete  = 1 << 3;
const OBJECT_PRIVILEGES	privAlter	= 1 << 4;

const OBJECT_PRIVILEGES	privAll		= OBJECT_PRIVILEGES(-1);  // all bits are 1.


//----------------------------------------------------------------------------------------------
// OTHER CONSTANTS:
//----------------------------------------------------------------------------------------------

const int kBothAreNulls = -100;


/**********************************************************************************************/
// LIMITS, we define own limits to be sure we do not depend on libs and compilers.
//
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning( disable : 4305 )
#pragma warning( disable : 4309 )
#endif

const vuint16		vuint16_max	= 0xffffU;

#ifdef _MSC_VER
#pragma warning(pop)
#endif

const vuint16		vuint16_min	= 0;

const vuint32		vuint32_max	= 0xffffffffUL;
const vuint32		vuint32_min	= 0;

const vint32		vint32_max	= 2147483647L;
const vint32		vint32_min	= (-2147483647L - 1);

const vuint64		vuint64_max	= 0xFFFFFFFFFFFFFFFFULL;
const vuint64		vuint64_min	= 0;


/**********************************************************************************************/
// FormatValue modes
//
enum EFormat
{
	kUndefinedFormat,
	kNumberFormat,
	kCurrencyFormat,
	kPercentageFormat,
	kScientificFormat,
	kDateFormat,
	kRuleBasedFormat_SpellOut,	// spells out a value in words in the desired language
	kRuleBasedFormat_Ordinal,	// attaches an ordinal suffix from the desired language to the end of a number (e.g. "123rd")
	kRuleBasedFormat_Duration,  // formats a duration in seconds as hours, minutes, and seconds
	kRuleBasedFormat_Pattern,	// rule-based format defined by pattern
	kDateFormatFull,			// is pretty completely specified, such as
								// Tuesday, April 12, 1952 AD or 3:30:42pm PST.
	kDateFormatLong,			// such as January 12, 1952 or 3:30:32pm
	kDateFormatMedium,			// such as Jan 12, 1952
	kDateFormatShort,			// completely numeric, such as 12/13/52 or 3:30pm
	kDateFormat_Pattern
};


/**********************************************************************************************/
// The following constants can be used in SqlQueryEx().
// You can use combination of this flags for single call.
//
enum QueryOptions
{
	kEmpty					= 0,		// There is no flags.
	kWarnings				= 1 << 0,   // Warnings info is ON.
	kTunes      			= 1 << 1,	// Tunes info is ON.
	kOutputs      			= 1 << 2,	// Output info is ON (Print statement).
	kNoServerLog      		= 1 << 3	// NoServerLogging is ON (no log for the particular SqlQueryEx()).
};


/**********************************************************************************************/
const UChar				kLocalTmpTablePrefix = '#';


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // _FBL_Constants_h
