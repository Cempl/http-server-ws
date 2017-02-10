/**********************************************************************************************/
/* FBL_I_KeyValue.h		                                                      				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_KeyValue_h
	#define _FBL_I_KeyValue_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Interfaces/FBL_I_Storage.h>
#include <VShared/FBL/publ/Values/FBL_Value_Binary.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// this file declarations:
//
SMART_INTERFACE(I_KeyValue);
//
SMART_ARRAY_OF_SMART_PTRS( ArrayOfKeyValues, I_KeyValue_Ptr );


/**********************************************************************************************/
// Forward declarations:
//
SMART_INTERFACE(I_Database);


/**********************************************************************************************/
/*
	To define structure of KEY, we will use Array of FIELD_TYPE.
    The KEY for 
		- kKeyValueDefault type is just { VARCHAR }.
		- kKeyValueWithKey type is any combination of allowed types like this one - { LONG, VARCHAR }
		- kKeyValueForTable type is predefined and can not be changed - { ULONG, VARCHAR } where
		  ULONG is for RecID of table's records.
		- kKeyValueForLink type is predefined and can not be changed - { ULONG, ULONG, VARCHAR } where
		  ULONGs are for RecID-pairs of link's records.
 
    Currently we allow these types to be used in the KEY structure
		* INTEGER TYPES: BYTE, SHORT, USHORT, LONG, ULONG, LLONG, ULLONG
		* BINARY TYPES:  VARBINARY
		* STRING TYPES:	 VARCHAR (uses binary compare method - no collators)
        
	All other types will throw error: xFeatureError.
*/
SMART_ARRAY_OF_SIMPLES( KeyStructure, FIELD_TYPE );


/****************************************************x*****************************************/
// The following constants can be used in CreateKeyValue() methods, Set/Get/Clear Flags() methods.
// You can use combination of this flags in a single call.
//
enum KeyValueAttributes
{
	fKeyValueNone					= 0,		// (r/w) There is no flags.
	
	fKeyValueCompressed				= 1 << 1,	// (r/w) If true "big" data will be compressed internally.

	//
	// RO flags (could be set before KeyValue create only)
	//
	
	fKeyValueTemporary				= 1 << 2,	// (r/ ) Such KeyValue's data goes to .tmp volume.
	
	fKeyValueRAM					= 1 << 3	// (r/) Such KeyValue is not stored on the disk.
};


/****************************************************x*****************************************/
//StubInterface
interface FBL_SHARED_EXP_CLASS I_KeyValue : public I_Storage
{
	// ---------------------
	// Construction:

virtual 						~I_KeyValue( void );


	// ---------------------
	// Properties:
    
    							// Returns the Database to which belongs this KeyValue object.
virtual I_Database_Ptr			get_Database( void ) const = 0;

								// Returns the unique ID of this KeyValue object.
virtual KeyValue_ID				get_ID( void ) const = 0;

	// <Compressed>	[r/w]
								/** TRUE if the field must use compression for its data. */
virtual	bool					get_Compressed( void ) const  = 0;
virtual	void					put_Compressed( bool inValue ) = 0;
	
virtual	bool					get_IsTemporary( void ) const = 0;
	
								// Returns the KeyStructure of this KeyValue object.
                                // It is actually an array of allowed Field Types.
                                // See comment of this class above.
virtual KeyStructure_Ptr		get_KeyStructure( void ) const = 0;

                                // Returns the kind of this KeyValue object.
virtual EKeyValueKind			get_Kind( void ) const = 0;

                            	// Returns the name of this KeyValue object.
virtual const String			get_Name( void ) const = 0;

                                // Returns the count of KEY-VALUE pairs in this store.
virtual vuint64					get_PairCount( void ) const = 0;


	// ---------------------
	// Key-Value Methods:
								// Creates a new I_Value following KeyStructure rules
                                // (I_Value_Compound for complex keys).
                                // Use this method to get a copy of correct "key as I_Value"
								// instead of creating such I_Value yourself.
virtual I_Value_Ptr				MakeNewKeyByStructure( void ) const = 0;

								// Returns VALUE associated with the given inKey.
                                // Returns NULL if the inKey is not found.
virtual	I_Value_Ptr				Get( Const_I_Value_Ptr inKey ) = 0;
virtual	I_Value_Ptr				Get( const String&     inKey ) = 0;

								// INSERT KEY-VALUE pair into KeyValue store.
                                // If Key already exists throws an ERR_KEYVALUE_NOT_UNIQUE_KEY exception.
virtual	void					Insert( Const_I_Value_Ptr inKey, Const_I_Value_Ptr inValue ) = 0;
virtual	void					Insert( const String&     inKey, Const_I_Value_Ptr inValue ) = 0;

								// UPDATE VALUE for KEY in KeyValue store.
                                // If Key is not found throws an ERR_KEYVALUE_KEY_NOT_FOUND exception.
virtual	void					Update( Const_I_Value_Ptr inKey, Const_I_Value_Ptr inValue ) = 0;
virtual	void					Update( const String&     inKey, Const_I_Value_Ptr inValue ) = 0;

								// INSERT KEY-VALUE pair or UPDATE VALUE for KEY in KeyValue store if KEY exist.
                                // This method never throws exception related to KEY.
virtual	void					Upsert( Const_I_Value_Ptr inKey, Const_I_Value_Ptr inValue ) = 0;
virtual	void					Upsert( const String&     inKey, Const_I_Value_Ptr inValue ) = 0;

								// DELETE the KEY-VALUE pair specified by the inKey.
                                // If KEY is not found, nothing happens.
virtual bool					Delete( Const_I_Value_Ptr inKey ) = 0;
virtual bool					Delete( const String&     inKey ) = 0;


	// ---------------------
	// Diagnose Methods:
                                /**	Performs diagnose of the KeyValue. Writes
                                    diagnostic information to the stream accordingly 
                                    specified verbose level. 
                                    @param inOut - stream for output messages.
                                    @param inLevel - specifies the amount of diagnostic messages
                                        to be produced during operation. */
virtual bool 					Diagnose(
									I_OStream_Ptr	inOut,
									EVerboseLevel 	inLevel = kVerbose_Normal ) = 0;

virtual bool 					Diagnose(
									I_Location_Ptr	inReportLocation,
									EVerboseLevel	inLevel = kVerbose_Normal ) = 0;

};



/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_I_KeyValue_h
