/**********************************************************************************************/
/* FBL_I_Field.h 		                                                      				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_Field_h 
	#define _FBL_I_Field_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_PropertyContainer.h>
#include <VShared/FBL/publ/Util_classes/FBL_Range.h>

// FINAL: 
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE( I_Field );
SMART_INTERFACE( I_IndexStyle );
SMART_INTERFACE( I_Table );
SMART_INTERFACE( I_Value );
SMART_INTERFACE( I_OStream );
SMART_INTERFACE( I_ForeignPlugin );


/**********************************************************************************************/
SMART_CLASS( Set );
SMART_CLASS( BitSet );
SMART_CLASS( ArraySet );


/**********************************************************************************************/
SMART_ARRAY_OF_SMART_PTRS( ArrayOfFields, I_Field_Ptr );


/**********************************************************************************************/
// Specifies 2 constant to be used with get_Value() that return non-const I_Value object.
// get_Value( forAdd ) -- should be used if you will assign a new value and do AddRecord().
// get_Value( forUpdate ) -- if you will assign a new value and do UpdateRecord().
// 
enum EValueAccess
{
	Undefined	= 0,
	forAdd 		= 1,
	forUpdate 	= 2
};


/**********************************************************************************************/
// interface FBL_SHARED_EXP_CLASS class
//
//StubInterface
interface FBL_SHARED_EXP_CLASS I_Field : public I_Unknown
{
virtual 					~I_Field( void );

							//! All virtual calls that cannot be placed in ctor must be here.
							//! It is called just after ctor.
virtual	void 				FinishCreate(
								I_Table_Ptr			inTable,
								VALUE_TYPE			inType,
								vuint16 			inFlags,
								Const_I_PropertyContainer_Ptr inProperties ) = 0;


virtual	void 				FinishConvert(
								I_Table_Ptr			inTable,
								VALUE_TYPE			inType,
								vuint16 			inFlags,
								Const_I_PropertyContainer_Ptr inProperties ) = 0;

virtual	void				InitCloned( 
								Const_I_Field_Ptr 	inOriginalField,
								I_Table_Ptr			inTableForCloned ) = 0;


	// ---------------------
	// Properties:	

 		// <IndexHashed>  [r/w]		
							/// Returns TRUE if index must be hashed.	
virtual	bool				get_IndexHashed( void ) const  = 0;

							/** Set ON/OFF the flag fIndexHashed. 
								If index exists, then this operation require rebuilt index.
								If index is absent on disk, then simply changes flag. */
virtual	void				put_IndexHashed( bool inOnOff ) = 0;


		// <DefaultValue>
							/** Returns the default value of the field. */
virtual Const_I_Value_Ptr	get_DefaultValue( void ) const  = 0;

							/** Assigns a new default value for this field. 
								Makes own copy of inValue.
								IF it is NULL, then field do not have default value. */
virtual	void				put_DefaultValue( Const_I_Value_Ptr inValue )  = 0;


		// <DefaultExpression>
							/** Returns the default expression of the field. */
virtual const String&		get_DefaultExpression( void ) const  = 0;

							/** Assigns a new default expresion for this field.
								IF it is empty, then field do not have default expression. */
virtual	void				put_DefaultExpression( const String& inValue )  = 0;

		// <ID>
							/// Unique ID of this field.
virtual	Field_ID			get_ID( void ) const  = 0;
virtual	void				put_ID( Field_ID inID )  = 0;	// Deprecated - moved to I_FieldEx::put_ID_Ex()

		// <IndexStyle> [r/o]

virtual I_IndexStyle_Ptr	get_IndexStyle( void )  = 0;
virtual void				put_IndexStyle( I_IndexStyle_Ptr inStyle )  = 0;

		// <IsMethod> [r/o]		
							/// Returns TRUE if this field is method (calculation field).
virtual	bool				get_IsMethod( void ) const  = 0;

		// <IsTemporary> [r/o]		
							/// Returns TRUE if this field has flag temporary.
virtual	bool				get_IsTemporary( void ) const  = 0;

		// <MaxLength>
							/** Returns the number of bytes that take this field in the table. 
								For numeric fields this is size of type, e.g. Long = 4 bytes. */
virtual vuint32				get_MaxLength( void ) const  = 0;

							/** Change the size of Table field. This operation have sense 
								only for such fields as String, VarChar, Binary.
								You cannot change size of e.g. ULONG field, which is always 4 bytes.
								Returns FALSE if size was not changed. This is just for info. */
virtual bool				put_MaxLength( vuint32 inSize ) = 0;
 
		// <MethodText>		
virtual const String&		get_MethodText( void ) const  = 0; //OLDKEEPAS: mMethodText
virtual void				put_MethodText( const String& inText ) = 0;

		// <Name>
							/// The name of a field.
virtual const String&		get_Name( void ) const  = 0;	//OLDKEEPAS: mName

							/** Change name of field in db structure.
								ERROR: errBadFieldName, errNameExists. */
virtual void				put_Name( const String& inName ) = 0;

		// <REC_ID> [r/o]
							/// Returns RecID of the current record in table.
virtual REC_ID				get_RecID( void ) const  = 0; 

		//<Size> [r/p]		
							///Returns size of field in bytes.
virtual flength				get_Size( void ) const  = 0;

		// <Table> [r/o]
							/** Returns Table to which belong this field. */
virtual	I_Table_Ptr			get_Table( void ) const  = 0;

		// <Type> [r/o]	
							/** The type of a field. You cannot change type of field,
								because this require change of C++ class type.
								But you can change type of Table Column in I_Table. */
virtual vuint32				get_Type( void ) const  = 0;

		// <TypeName> [r/o]	
							/** The type string of a field. */
virtual String				get_TypeString( const char inLocale[] = "en_US" ) const  = 0;

							/**	Returns specified properties for this field. This method used for
								clone fields. */
virtual I_PropertyContainer_Ptr	get_Properties( void ) const = 0;


	// ---------------------
	// Properties - flags:

 		// <Nullable>		
 							//! TRUE if the field can contain NULL value.
virtual	bool				get_Nullable( void ) const  = 0; 

							/** Set ON/OFF the flag fNullable. 
								This operation require chnages in the structure of table. */
virtual	void				put_Nullable( bool inValue )  = 0; 
 
 		// <Indexed>		
 							//! TRUE if the field have index.
virtual	bool				get_Indexed( void ) const  = 0; 

							/** Set ON/OFF the flag fIndexed. 
								If flag is set OFF then index file is removed from disk.
								If flag is ON, then index file is bult. 
								Also drop constraint. */
virtual	void				put_Indexed( bool inValue ) = 0; 
 
 		// <Unique> 		
 							//! TRUE if the field must have unique values.
virtual	bool				get_Unique( void ) const  = 0; 

							/** Set ON/OFF the flag fIndexUnique. 
								If index exists, then this operation requires to rebuilt index.
								If index is absent on disk, then it simply changes the flag.
								Also creates constraint in the Table of this field. */
virtual	void				put_Unique( bool inValue ) = 0; 

 		// <Identity>		
 							//! TRUE if the field is autoincremental.
virtual	bool				get_Identity( void ) const  = 0; 


		// <FlagsAlwaysON> [r/o] 
							//! Flags that must be always ON for this type of field.
							//! ZERO if there is no such flags.
virtual vuint16				get_FlagsAlwaysON( void ) const  = 0;

		// <FlagsAlwaysOFF>[r/o]
							//! Flags that must be always OFF for this type of field.
							//! ZERO if there is no such flags.
virtual vuint16				get_FlagsAlwaysOFF( void ) const  = 0;
 
 
	// ---------------------
 	// Properties of Field Value:
 	// This properties depend on the *current* record of Table.
 
		// <IsNull>
							/** Returns TRUE if the field value is NULL. 
								For not Nullable field always returns false. */
virtual bool				get_IsNull( void ) const  = 0;

							/** Clears the value of field: 
									-- For a numeric field zero it.	
									-- For a String field make it empty string.
									-- For a BLOB field .... ???
								For Nullable fields make it a NULL also. */
virtual void				put_IsNull( 
								bool 			inValue, 
								EValueAccess 	inAccessMode = forUpdate ) = 0;
	
		// <Value>
							/** Returns the value of field. 
								You can use returned value to read it. */
virtual Const_I_Value_Ptr	get_Value( void ) const  = 0;

							/** This method MUST be used when you want to assign
								a new value before Add/Update record.						
								@return non-const I_Value object which you can modify.							
								Use get_Value(forAdd)    if you get value todo AddRecord().								
								Use get_Value(forUpdate) if you get value toto UpdateRecord(). 
								In the last case it makes copy of the prev value
								to correctly update later the index of field.									
								In both cases it sets the flag IsDirty = ON. */
virtual I_Value_Ptr			get_Value( EValueAccess inValue )  = 0;

							/** Assigns a new value to a field. 
								Set flag IsDirty ON. Makes copy of the old value. */
virtual	void				put_Value( Const_I_Value_Ptr inValue ) = 0;

	
		// <Dirty>
							//! Returns TRUE if value is dirty.
virtual bool				get_IsDirty( void ) const  = 0; 
virtual void				put_IsDirty( bool inValue )  = 0; 
		

	// ---------------------
	// Default expression methods:

							/** 
								Return true if default expression is correct. */
virtual bool				get_IsDefaultExpressionCorrect( void ) const = 0;


	// ---------------------
	// Value methods:

							/** Assign value of inField to this field.
								Converts the value type if needed. 
								If convertion is not possible then value of field becomes NULL. */
virtual void				Assign( const I_Field& inField )  = 0;	

							/** Set Value of Field to be equal to default value of Field.
								In case there is no default value, do nothing. */
virtual void				AssignDefault( void )  = 0;	

							/** Makes nullable field to be NULL, 
								if a field have default value set field to default. 
								@param inAccessMode allow you optimize this function,
								if you exactly know that you use SetBlank() to prepare buffer
								for AddRecord(). It will be faster because in this case Valentina
								will not spend time on copy of the old values. 	*/
virtual void				SetBlank( EValueAccess inAccessMode = forUpdate )  = 0;


	// ---------------------  
	// Search Methods:
	//
	// The following set of methods allows do simple searches on a single column.
	// You can search as the whole table, so some its selection.
	// This methods works for both indexed and not indexed fields.
	// Using results of searches you can get AND/OR searches for few columns.
	// 
	// NOTE:
	// -- If inSelection is NULL then all records of Table are searched. 
	// 		Otherwise in case we do not-indexed scan of table, we use inSelection to iterate records.
	//		In case we do indexed search, inSelection do not help be faster,
	// 		we just do intersect of inSelection with index result, so it looks like we have made
	//		search on selection.			
	// -- Resulted BitSet has the number of bits equal to the number of physical records 
	//		in the Table we search.
	// -- Returns NULL in case no records found, because there is no big sense to allocate
	// 		object for empty BitSet.


							/** Returns a result of search for a given inValue using
								LIKE syntax. If inValue is NULL then result will be NULL. */
virtual BitSet_Ptr			FindLike( 
								Const_I_Value_Ptr 	inValue,
								UChar				inEscape = (UChar)'\\',
								Const_BitSet_Ptr  	inSelection = nullptr, 
								ESearch				inSearchPref = kPreferIndexed ) const = 0;

							/** Returns a result of search for a given inValue using
								ILIKE syntax. If inValue is NULL then result will be NULL. 
                                ILIKE respects collator of this field. 
                                It can work x2-x3 times slower of FindLike(). */
virtual BitSet_Ptr			FindILike(
								Const_I_Value_Ptr 	inValue,
								UChar				inEscape = (UChar)'\\',
								Const_BitSet_Ptr  	inSelection = nullptr, 
								ESearch				inSearchPref = kPreferIndexed ) const = 0;

							/** Returns a result of search for a given inValue using
								REGEX syntax. If inValue is NULL then result will be NULL. */
virtual BitSet_Ptr			FindRegEx( 
								Const_I_Value_Ptr inValue,
								Const_BitSet_Ptr  inSelection = nullptr, 
								ESearch				inSearchPref = kPreferIndexed ) const = 0;


		// -----------------
		// Search Value: 	
							/** @return TRUE if it finds inValue in the specified Table selection.
								@param outCount -- if specified then it get the number 
								of records that have this value. */
virtual	bool				ValueExists(
								Const_I_Value_Ptr 	inValue, 
								BitSet_Ptr			inSelection = nullptr,
								vuint32* 				outCount = nullptr, 
								ESearch				inSearchPref = kPreferIndexed  ) const = 0;

							/** Returns a result of search for a given inValue.
								If inValue is NULL then the current field value is used. */
virtual BitSet_Ptr			FindValue( 
								Const_I_Value_Ptr 	inValue = nullptr,
								Const_BitSet_Ptr 	inSelection = nullptr, 
								ESearch				inSearchPref = kPreferIndexed  ) const = 0;
	
							/** Returns set of records that have values != inValue.
								@param inSelection - if specified then search only records
								in this set. */
virtual BitSet_Ptr			FindNotValue( 
								Const_I_Value_Ptr 	inValue = nullptr,
								Const_BitSet_Ptr 	inSelection = nullptr, 
								ESearch				inSearchPref = kPreferIndexed  ) const = 0;

							/** Returns a result of search for a given inValue.
								@param inValue if NULL then use for search the current field value. 
								@param ioLimitCount - allows to specify the limit of found records.
									Since index is sorted, it acts as "SELECT first N records of result". */
virtual	ArraySet_Ptr		FindValueAsArraySet( 
								Const_I_Value_Ptr 	inValue = nullptr,
								Const_BitSet_Ptr 	inSelection = nullptr,
								vuint32				ioLimitCount = vuint32_max, 
								ESearch				inSearchPref = kPreferIndexed  ) const = 0;

							/** This function is comfortable for case when you know that result 
								of search will be just one record. also this will work for few
								found records, but only the first found will be returned. */
virtual REC_ID				FindSingleValue(
								Const_I_Value_Ptr 	inValue,
								Const_BitSet_Ptr 	inSelection = nullptr, 
								ESearch				inSearchPref = kPreferIndexed  ) const = 0;
																			  

	// -----------------
	// Search Ranges: 							

							/** Returns a result of search in Range {inLeftValue,inRightValue}. */
virtual	BitSet_Ptr			FindRange( 
								Const_Range_Ptr 	inRange,
								Const_BitSet_Ptr 	inSelection = nullptr, 
								ESearch				inSearchPref = kPreferIndexed  ) const = 0;
	
							/** Returns a result of search in Range {inLeftValue,inRightValue}
							 	as ArraySet. Usefull if it is known that we will find not many
							 	records. */
virtual	ArraySet_Ptr		FindRangeAsArraySet( 
								Const_Range_Ptr 	inRange,
								Const_BitSet_Ptr 	inSelection = nullptr,
								vuint32				ioLimitCount = vuint32_max, 
								ESearch				inSearchPref = kPreferIndexed  ) const = 0;
								
	// -----------------
	// OR Searches: 							

							/** Returns a result of search for a given array of values.
								This is very useful for searches of kind
										fld in (v1, v2, v3, v4)
										fld = v1 or fld = v2 or fld = v3 or ...
								@param inValues - array of values. */
virtual BitSet_Ptr			FindValues( 
								ArrayOfValues_Ptr 	inValues = nullptr,
								Const_BitSet_Ptr 	inSelection = nullptr, 
								ESearch				inSearchPref = kPreferIndexed  ) const = 0;

							/** Returns a result of search for array of Ranges.
								This is useful for queries as 
									(f < 0) or (f > 5 and f < 7) or (f > 9).	*/
virtual	BitSet_Ptr			FindRanges( 
								ArrayOfRanges_Ptr 	inRanges,
								Const_BitSet_Ptr 	inSelection = nullptr, 
								ESearch				inSearchPref = kPreferIndexed  ) const = 0;

							/** Another special function for OR searches. Should be used in case
								we have MIXED -- values and ranges joined by OR.
									f1 = 5 or f1 = 7 or (f1 > 10 and f1 < 20) */ 
virtual	BitSet_Ptr			FindValuesAndRanges( 
								ArrayOfValues_Ptr 	inValues,
								ArrayOfRanges_Ptr 	inRanges,
								Const_BitSet_Ptr 	inSelection = nullptr, 
								ESearch				inSearchPref = kPreferIndexed  ) const = 0;

	// -----------------
	// Search Nulls: 	
		
							/** @return BitSet of records that are NULLs. 
								Result can be intersected with inSelection. */
virtual BitSet_Ptr 			FindNulls( 
								Const_BitSet_Ptr 	inSelection = nullptr, 
								ESearch				inSearchPref = kPreferIndexed  ) const = 0;	

							/** @return BitSet of records that are NOT NULLs. 								
								Result can be intersected with inSelection. */
virtual BitSet_Ptr			FindNotNulls( 
								Const_BitSet_Ptr 	inSelection = nullptr, 
								ESearch				inSearchPref = kPreferIndexed ) const = 0;	

							/** @return BitSet of records that are NOT NULLs
								and not in inSet. */
virtual	BitSet_Ptr 			InvertSelection( BitSet_Ptr inSet ) const = 0;


	// -----------------
	// Special Searches:

virtual	BitSet_Ptr			FindDistinct( 
								Const_BitSet_Ptr 	inSelection = nullptr, 
								ESearch			 	inSearchPref = kPreferIndexed  ) const = 0;


	// ---------------------
	// Utility methods:

							//! Swap segments of db volumes to remove fragmentation.
							//! Do not affect size of database.
							//! Should be used after Compact().
virtual void 				Defragment( void ) = 0;

virtual bool 				Diagnose( 
								I_OStream_Ptr 	inOut,
								EVerboseLevel 	inLevel = kVerbose_Normal ) = 0;

virtual bool 				Diagnose( 
								I_Location_Ptr	inReportLocation,
								EVerboseLevel	inLevel = kVerbose_Normal ) = 0;

virtual void 				Compact( void ) = 0;

virtual void				Reindex( void )  = 0;

virtual void 				Repair( void ) = 0;

virtual	void				OnDatabaseDestroy( void ) = 0;

}; 


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_I_Field_h







