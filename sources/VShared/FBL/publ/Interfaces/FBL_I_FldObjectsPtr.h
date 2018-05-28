/**********************************************************************************************/
/* FBL_I_FldObjectsPtr.h	                                                   				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef __FBL_I_FldObjectsPtr_h 
	#define __FBL_I_FldObjectsPtr_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Table.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE(I_FldObjectsPtr);


/**********************************************************************************************/
// Interface of ObjectsPtr field.
// 
// While ObjectPtr field always points to some table and store only its records,
// a ObjectsPtr field do not have fixed target and is able store records from ANY table 
// in Valentina database. 
//
// ObjectsPtr field stores 8 byte OID of a record. Having OID we easy can define Table 
// that have this record and record itself.
//
// Problem comes if we want move from PARENT record to CHILD record. We do not know what 
// table contain a ObjectPtr field which MAY point to parent record. As result we get very 
// uneffective process -- we need search EACH ObjectsPtr in the database.
//
// As optimisation of this problem we will use next trick. ObjectsPtr field will get 
// set of tables which records it can point. So we get extention of ObjectPtr field.
// Instead of one Target object we will have n Targets.
//
// ObjectsPtr is kind of a I_Link. So it register self in both tables as Link.
//
// This field creates "strange" M : 1 link between N tables.
// If mark this field as unique then it will specify "strange" 1 : 1 link.
//
interface FBL_SHARED_EXP_CLASS I_FldObjectsPtr : public I_Unknown
{

virtual						~I_FldObjectsPtr( void );

	//----------------------
	// Properties:

		// <Targets> [r/w]
		
							/// Returns the Table pointed by this ObjectPtr field.
virtual	ArrayOfTables_Ptr	get_Targets( void ) const  = 0; 

							/** Assign Target Tables for this ObjectsPtr. */							
virtual	void				put_Targets( ArrayOfTables_Ptr inTargets )  = 0; 

		// <OnDeletion> [r/w]
		
							/// Returns the Table pointed by this ObjectPtr field.
virtual	EOnDeletion			get_OnDeletion( void ) const  = 0; 

virtual	void				put_OnDeletion( EOnDeletion inValue ) = 0; 

		// <Value> [r/w]
							/** Returns the current value of ObjectsPtr field.
								You still can use I_Field::get_Value() method. */
virtual OID					get_TargetValue( void ) const  = 0; 		
					
							/** Store in the the ObjectsPtr the RecID of a parent record.
							 	You still can use I_Field::put_Value() method.
								ERRROS: errTable_RecordNotFound */ 
virtual void				put_TargetValue( OID inValue ) = 0; 		

}; 
 

/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // __FBL_I_FldObjectsPtr_h
