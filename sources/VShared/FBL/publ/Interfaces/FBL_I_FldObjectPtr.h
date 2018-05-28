/**********************************************************************************************/
/* FBL_I_FldObjectPtr.h	                                                      				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_FldObjectPtr_h 
	#define _FBL_I_FldObjectPtr_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Field.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE(I_FldObjectPtr);
SMART_INTERFACE(I_Table);


/**********************************************************************************************/
// Interface of ObjectPtr field.
// This fields have 2 mains parameters: Target table and deletion control.
// 
// ObjectsPtr is kind of a I_Link. So it register self in both tables as Link.
//
// This field is 4 bytes ULONG field that simply stores REC_ID values of Target table.
// ObjectPtr field cannot be Nullable.
//
// It is easy to see that ObjectPtr field play role of DIRECT POINTER, 
// something close to Navigational Database Model. 
// ObjectPtr field is 4 times faster of analaog RDB link based on 2 ULONG fields!
//
// This field create M : 1 link between tables.
// If mark this field as unique then it will specify 1 : 1 link.
//
//StubInterface
interface FBL_SHARED_EXP_CLASS I_FldObjectPtr : public I_Unknown
{
virtual						~I_FldObjectPtr( void );


	//----------------------

virtual void 				RegisterAsLink( bool inCheckRequired )	= 0;


	//----------------------
	// Properties:

		// <Target> [r/w]

							/// Returns the Table pointed by this ObjectPtr field.
virtual	I_Table_Ptr			get_Target( void ) const  = 0; 

							/** Change Target of ObjectPtr. In fact this operations must be used
								mainly for initialisation of ObjectPtr. 
								If Target is really changed then values in all records must become
							 	ZERO, because old values will not have sense with new target. */							
virtual	void				put_Target( I_Table_Ptr inTarget )  = 0; 

		// <OnDeletion> [r/w]
		
							/// Returns the deletion control flag value.
virtual	EOnDeletion			get_OnDelete( void ) const  = 0; 
virtual	void				put_OnDelete( EOnDeletion inValue ) = 0; 

		// <Value> [r/w]
							/** Returns current value of ObjectPtr field.
								You still can use I_Field::get_Value() method. */
virtual REC_ID				get_TargetValue( void ) const  = 0; 		
					
							/** Store in the ObjectPtr the RecID of a parent record.
							 	You still can use I_Field::put_Value() method.
								ERRROS: errTable_RecordNotFound */ 
virtual void				put_TargetValue( REC_ID inValue ) = 0; 		


		// RDB - conversion: 
	
							/// This is method is for convert existing RDB link to the ObjectPtr.
virtual	void				ConvertFromRDB( I_Field_Ptr inRDB_KEY,
											I_Field_Ptr inRDB_PTR  ) = 0;


}; 
 

/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_I_FldObjectPtr_h
