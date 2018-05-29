/**********************************************************************************************/
/* FBL_Types.h 		                                                      					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_Types_h 
	#define _FBL_Types_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL_Macros.h>

/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
#if !FBL_MAC
	typedef void*	voidPtr;
#endif // !FBL_MAC


/**********************************************************************************************/
// Useful shortcuts for fundamental C types. Left for C++ client and V1.
//
#ifndef FBL_USE_DEPRECATED_TYPES
	#define FBL_USE_DEPRECATED_TYPES 0
#endif // FBL_USE_DEPRECATED_TYPES

#if FBL_USE_DEPRECATED_TYPES

	typedef unsigned char   		uchar;
	typedef unsigned short  		ushort; 
	 
	#if __LP64__
 		typedef unsigned int		ulong;
	#else
 		typedef unsigned long   	ulong;
	#endif // __LP64__
	 
	#if defined(_MSC_VER)
		typedef __int64  			llong;
 		typedef unsigned __int64    ullong;
	#else
 		typedef long long  			llong;
 		typedef unsigned long long  ullong;
	#endif

#endif // FBL_USE_DEPRECATED_TYPES


/**********************************************************************************************/
// This types specify database parameters: 
//
#if FBL_LENGTH <= 32
	typedef vuint32 				flength;	// type for file length < 2GB.
#else 
	typedef vuint64 				flength;	// type for file length > 2GB.
#endif
  
  
/**********************************************************************************************/
// REC_ID and OID types.
//
typedef vuint32 REC_ID;		// type for RecID, so up to 4 billions records in one Table.	
 
typedef REC_ID 	CPOSITION;	// Cursor does not have rec_id. It always operates with positions.
 
typedef REC_ID 	ID_TYPE;	// Some algorithms require to work with numbers
							// that must be the same type as REC_ID, but they are not RecIDs
							// of some table, so we specify special type name.


typedef vint64	OID;			// "Object ID" = (Table_ID << 32) | REC_ID



/**********************************************************************************************/
typedef vuint16	Field_ID;			// ID of a Field. 
typedef vuint32	IndStyle_ID;		// ID of a Index style. 
typedef vint32	KeyValue_ID;		// ID of a KeyValue.
typedef vint32	Link_ID;			// ID of a Link. Tmp Link have negative ID.
typedef vint32	Index_ID;			// ID of an Index.
typedef vint32	Sequence_ID;		// ID of a Sequence.
typedef vint32	StoredProcedure_ID;	// ID of a Stored procedure.
typedef vint32	Table_ID;			// ID of a Table. Tmp Table have negative ID.
typedef vint32	Trigger_ID;			// ID of a Trigger.
typedef vint32	Type_ID;			// ID of a Type.
typedef vint32	View_ID;			// ID of a View.


/**********************************************************************************************/
typedef vuint32	ERROR_TYPE;


/**********************************************************************************************/
// Types of objects of a KERNEL which the user may develop and use as statically linked or
// dynamically loaded (as Plugins) libraries. This is why they are declared as typedef 
// but not as enums.
//
typedef vuint32		VALUE_TYPE;					
typedef vuint32		FIELD_TYPE;					

typedef vuint32		FUNCTION_TYPE;				
typedef vuint32		INDEX_TYPE;

typedef vuint32		IMPORTER_TYPE;
typedef vuint32		EXPORTER_TYPE;
typedef vuint32		PROGRAM_LANGUAGE_TYPE;
typedef vuint32		COMPRESSOR_TYPE;
typedef vuint32		ENCRYPTION_TYPE;
typedef vuint8   	LOCK_TYPE;
typedef vuint8   	DUMP_TYPE;
typedef vuint8		JOIN_TYPE;

typedef vuint32		TMETHOD_LANGUAGE;

typedef	vuint32		OBJECT_PRIVILEGES;


/**********************************************************************************************/
/// This type is used in the CreateTable(). 
/// Specifies how the Table should store its fields: 
///		* as separate columns;
/// 	* as single file with the fixed size records;
//
typedef vuint32 TableStructure;


/**********************************************************************************************/
// Specify link type, e.g. [1 : 1] [1 : M] [1 : N]  
// 0 - Many
// 1 - One
// N - not more than N, similiar to Many with the top limit.
//
typedef vuint16	LinkType;	


/**********************************************************************************************/
// Specifies the length of FBL::String:
//
typedef vint32 tslen;


/**********************************************************************************************/
// Header for storing of a disk file in the BLOB field.
// This header is needed to correctly store ALL info of MacOS files.
//
#ifdef WINDOWS
	#pragma pack(push, 2)
#endif

struct OSFileDescription // sizeof = 88 
{
//	char		mName[64]; 
	vuint32		mCreator;
	vuint32		mType;
	vuint32		mDataForkLength;
	vuint32		mResForkLength;
	vuint16		mFlags;
	vuint8		mReserved[6];
};


#ifdef WINDOWS
	#pragma pack(pop)
#endif


/**********************************************************************************************/
enum EStringImpType
{
	kICUImpl,
	kCustomImpl
};


/**********************************************************************************************/
// Typedef for server object identifier.
//
#if V_SERVER

	typedef size_t		RemoteID;		// Proxy address depends on build - 32 or 64bit integer
	typedef RemoteID	ProxyPtr;		// The same as RemoteID		
	typedef size_t		KernelPtr;		// Kernel address depends on build - 32 or 64bit integer

#elif FBL_SHARED

	// Special for vShared - we MUST NOT implement any code using these types there
	typedef void*		RemoteID;		// Proxy address out of Client or Server is not allowed
	typedef void*		ProxyPtr;		// The same as RemoteID		
	typedef void*		KernelPtr;		// Kernel address  out of Client or Server is not allowed

#else 

	// FBL_CLIENT
	// Clients always operate with RemoteID (proxy-addresses) as 64bit integers.
	// But only on Send/Receive packets Client operates with RemoteID regarding to 
	// the connection property (32 or 64 bit)
	typedef vuint64		RemoteID;		// Proxy address is always 64-bit integer even if 32bit-server
	typedef RemoteID	ProxyPtr;		// The same as RemoteID
	typedef size_t		StubPtr;		// Stub address depends on build - 32 or 64bit integer
	typedef void		KernelPtr;		// Not used on client side

#endif //V_SERVER


/**********************************************************************************************/
typedef vuint32 TIMESTAMP;				


/**********************************************************************************************/
enum EObjectKind
{
	kObjectUnknown		= 0,
	kObjectTable		= 1,
	kObjectCursor		= 2,
	kObjectView			= 3,
	kObjectTrigger		= 4,
	kObjectStoredProcedure	= 5,
	kObjectField		= 6,
	kObjectSequence		= 7,
	kObjectType			= 8,
	kObjectConstraint	= 9
};


/**********************************************************************************************/
enum EVGranteeObjectType
{
	kGrantee_Server,
	kGrantee_Database,
	kGrantee_Project,
	kGrantee_Table,
	kGrantee_Link,
	kGrantee_SP,
	kGrantee_Field
};


/**********************************************************************************************/
// Urgent notifications must be delivered first (top in NotificationQueue).
// 
enum ENotificationPriority
{
	NormalPriority		= 0,
	UrgentPriority 		= 1
};


/**********************************************************************************************/
// Kind of particular vServer connection.
//
enum EVClientHandlerKind
{
	kClientHandlerKind_Unknown = 0,
	kClientHandlerKind_Regular,
	kClientHandlerKind_Fake,
	kClientHandlerKind_REST,
	kClientHandlerKind_Sqlite
};


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // _FBL_Types_h
