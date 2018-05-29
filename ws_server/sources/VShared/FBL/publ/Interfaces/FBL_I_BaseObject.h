/**********************************************************************************************/
/* FBL_I_BaseObject.h	                                                      				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef __FBL_I_BaseObject_h 
	#define __FBL_I_BaseObject_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Interfaces/FBL_I_Table.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE(I_BaseObject);


/**********************************************************************************************/
SMART_ARRAY_OF_SMART_PTRS( ArrayOfBase, I_BaseObject_Ptr );


/**********************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
	
/*		I_BaseObject*		CreateBaseObject( 
								const UChar*		inName,
								I_BaseObject_Ptr	inParent,
//								EInheritanceType	inInheritanceType,	
								bool 				inIsAbstract = false,															
								ETableKind			inTemporary	= kTblPermanent,
								EStorageType		inStorage	= kStorage_Default,
								ETableMode			inTblMode 	= kTblSeparate );
*/

#ifdef __cplusplus
}
#endif


/**********************************************************************************************/
/**
	I_BaseObject -- Valentina specific abstraction of OR data model for implementation of 
	table inheritance on DBMS level.

	Instance of BaseObject is from one side I_Table, i.e. it can be considerd as usual Table.
	On the other hand it is something bigger than just a table. Because 
	-- BaseObject in general case manage several physical Tables of Database. 
	-- BaseObject have special knowlage about how to manage that tables.
	-- BaseObject manage internal fields of that Tables.
	-- BaseObject have additional inheritance parameters.

	I_BaseObject inherits I_Table interface, to force override of all functions of I_Table.
	so, BaseObject can be considered as special type of Table.

	Each BaseObject have its own Table (we will name TopTable) which store its Fields.
	TopTable DO NOT store fields of Parent BaseObjects. Parent fields are stored in their own tables.

	TopTable have hidden (aka system, aka internal) ObjectPtr "sysParentPtr1" with fUnique flag. 
	We need flag fUnique to establish 1 : 1 link between Parent and Child tables. 
	Such kind of link (1 : 1) is inheritance requirement. 
	
	If BaseObject have several parents, then we need several ObjectPtr fields with names
	"sysParentPtr1", "sysParentPtr2", "sysParentPtr3", ... 	
*/
interface FBL_SHARED_EXP_CLASS I_BaseObject : public I_Table
{
virtual 					~I_BaseObject( void ) {}


	// ---------------------
	// Properties:

		// <ParentCount> [r/o]
		
							/// Returns the number of parent BaseObjects.
virtual vuint32				get_ParentCount( void ) const  = 0; 	

		// <Parent> [r/o]
		
							/// Returns the Parent of this BaseObject.
							/// Returns NULL if there is no parent, i.e. this BaseObject is root.
							/// If we have multiple inheritance, we can access parents by index.
virtual I_BaseObject_Ptr	get_Parent( vuint32 inIndex = 1 ) const  = 0;

		// <Abstract> [r/o]
		
							/// Returns TRUE if this BaseObject is abstract, 
							/// i.e. cannot have own instances of records, and it MUST have subclasses.									
virtual bool				get_IsAbstract( void ) const  = 0; 

		// <InheritanceType> [r/o]		
		
							/// Returns the type of inheritance for this BaseObject.
virtual EInheritanceType	get_InheritanceType( void ) const  = 0; 

virtual I_Table_Ptr			get_TopTable( void ) const  = 0; 


	// ---------------------
	// Methods:



	public://///////////////////////////////////////////////////////////////////////////////////
 
// I_Table interface:
//
// We repeat here this interface with additional comments on sense
// of this functions from the BaseObject point of view.

	// ---------------------
	// Properties:

							/// BaseObject: use ID of its TopTable.
virtual	Table_ID			get_ID( void ) const  = 0; 

							/// BaseObject: returns the name of TopTable.
virtual	const String&		get_Name( void ) const   = 0;

							/// BaseObject: Changes name of TopTable.
virtual	void				put_Name( const UChar* inName ) = 0;
virtual	void				put_Name( const String& inName ) = 0;

							/// Returns FALSE always, because system do not use BaseObjects.
virtual bool				get_IsSystem( void ) const  = 0; 

							/// Returns TRUE if this BaseObject is temporary.
							/// In the same time, temporary BaseObject can be inherited from
							/// Permanent BaseObject.
virtual bool				get_IsTemporary( void ) const  = 0; 

							/// BaseObject: Returns Storage object of TopTable.		
virtual I_TableStorage_Ptr	get_Storage( void ) const  = 0;


	// ---------------------
	// Field methods:

							/// BaseObject: Returns the TOTAL field count of fields
							/// including inherited and own fields.
							/// You can get own field count from TopTable. 	
virtual	vuint16				get_FieldCount( void ) const  = 0;

							/** Returns a field by Index.
								Returns NULL if the field is not found. 
								BaseObject: works on array of all fields. */
virtual I_Field_Ptr			get_Field( vuint16 inIndex ) const  = 0;

							/** Returns a field by Name.
								Returns NULL if the field is not found.
								BaseObject: works on array of all fields. */
virtual I_Field_Ptr			get_Field( const UChar* inName ) const  = 0;

								
	// ---------------------
	// Properties of the current record:

							/// BaseObject: Returns the number of logical records.
							/// This is number of records in the TopTable.
virtual vuint32 				get_RecordCount( void ) const  = 0;

							/// BaseObject: Returns the number of physical records in this table.
							/// This is the number of physical records  in the TopTable. 
virtual vuint32 				get_PhysicalRecordCount( void ) const  = 0;

							/// Returns RecID of the current record of TopTable.  	
virtual REC_ID				get_RecID( void ) const  = 0;

							/// GoTo the specified record in the TopTable.  
virtual bool				put_RecID( REC_ID inRecID ) = 0;

							/// Returns OID of the current record of TopTable.  	
virtual OID					get_OID( void ) const  = 0;
virtual bool				put_OID( OID inOID ) = 0;

							/// BaseObject: Returns I_Record which keep ALL fields of BaseObject,
							/// including inherited. If you need Record with only own fields,
							/// you can obtain it from TopTable. 
virtual I_Record_Ptr		get_Record( void ) const  = 0;


	// ---------------------
	// DB Schema Methods:
 
							/// BaseObject: Creates a new field in the TopTable. 
virtual I_Field_Ptr			CreateField( 
								const UChar* inName,
								FIELD_TYPE	inType, 
								vuint16 		inFlags = fNone,
								Const_I_PropertyContainer_Ptr inProperties = NULL ) = 0;

virtual I_Field_Ptr			CreateField( 
								const String& inName,
								FIELD_TYPE	inType, 
								vuint16 		inFlags = fNone,
								Const_I_PropertyContainer_Ptr inProperties = NULL ) = 0;


							/// BaseObject: Drop Field with all its info (e.g. indeex) from TopTable.
							/// BaseObject can drop only its own field. 
							/// ERROR: ERR_CANNOT_MODIFY_PARENT_FIELD.  
virtual void				DropField(
								I_Field_Ptr	inField ) = 0;


	// ---------------------
	// Record Modification methods:

							/// BaseObject: modify ALL fields.	
virtual void				SetBlank( void )  = 0;

							/// BaseObject: Add new record into Parent and TopTable tables.
							/// Automatically establish Pointers.
virtual REC_ID				AddRecord( void ) = 0;

							/// BaseObject: Update all tables of BaseObject.	
virtual void				UpdateRecord( REC_ID inRecID = 0 ) = 0;
virtual	void				UpdateAllRecords( Set_Ptr inSet = NULL ) = 0;

virtual void				DeleteRecord( REC_ID inRecID = 0 ) = 0;
virtual	void				DeleteAllRecords( Set_Ptr inSet = NULL ) = 0;


	// ---------------------
	// Record Navigation methods:

							/// BaseObject: Moves the current record of TopTable as specified,
							/// Automatically moves the current record pointer(s) 
							// in all parent Tables also, so all fields get correct values.

virtual bool				FirstRecord( void ) = 0;
virtual bool 				LastRecord( void ) = 0;
virtual bool 				NextRecord( void ) = 0;
virtual bool 				PrevRecord( void ) = 0; 


	// ---------------------
	// Selection methods:

							/// BaseObject: build selections of size corresponded to 
							/// the size of TopTable. 	

virtual BitSet_Ptr			SelectAllRecords( void ) const = 0;	
virtual BitSet_Ptr			SelectNoneRecords( void ) const = 0;	

	// ---------------------
	// Link properties:

							/// BaseObject: Work with the total count of Links 
							/// including inherited. You can get only own links
							/// from TopTable.  

virtual vuint16				get_LinkCount( void ) const  = 0;	
virtual I_Link_Ptr			get_Link( vuint16 inIndex ) const  = 0;	
virtual I_Link_Ptr			get_Link( const UChar* inName ) const  = 0;	


	// ---------------------
	// Record Locking methods:
		
		/**
			Record locking of BaseObject method is more hard operation than on single table.
			This is obviouse because we must set up locks in several tables.
				
			If Parent BaseObject is not abstract, i.e. it can have own Record Instances,
			then it also have own Lock flags. If we set LOCK on a Record of Child BaseObject 
			then we get success only if Parent record also can be locked as asked.
			
			If Parent BaseObject is abstract, then we can do small optimisation!
			Such BaseObject have no need for own LOCK flags, so we can do lock
			only on Child BaseObject's TopTable.   
		*/
}; 




/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // __FBL_I_BaseObject_h
