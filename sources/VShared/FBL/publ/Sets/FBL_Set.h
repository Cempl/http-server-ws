/**********************************************************************************************/
/* FBL_Set.h																				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                       					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_Set_h
	#define _FBL_Set_h
#pragma once

// FBL:

#include <VShared/FBL/publ/Headers/FBL.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_Serializable.h>

#include <VShared/FBL/publ/Sets/FBL_SetIterator.h>

#include <VShared/FBL/publ/Util_classes/FBL_ForeignPlugin_Imp.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
enum SetOperation
{
		// Intersection of all sets. Logical AND.
		// Result set contains records which belongs each set.
		// Binary operation.
	kIntersection	= 1,

		// Union of all sets. Logical OR.
		// Result set contains records which belongs at least one set.
		// Binary operation.
	kUnion,

		// Difference of two set: S = S1 - S2
		// There is no logical
		// Result set contains records which belong to S1 but not belong to S2.
		// Binary operation.
	kMinus,

		// Symmetric difference of the sets.
		// Logical XOR (excluded OR).
		// Result set contains such records, which if it belong to one of sets
		// then it don't belong to any other.
		// Binary operation.
	kXOR
};


/**********************************************************************************************/
SMART_CLASS( SetIterator );
SMART_CLASS( Set );
//
SMART_ARRAY_OF_SMART_PTRS( ArrayOfSets, Set_Ptr );


/**********************************************************************************************/
/*
	Interface class (i.e abstract with no data memebers).
	Can be used in multiple inheritance.

	General purpose class.
	Implements set of elements, i.e it can contain only one copy of an element.
*/

class FBL_SHARED_EXP_CLASS Set :
	public I_Serializable,
	public ForeignPlugin_imp
{
		IMPLEMENT_UNKNOWN(Set)
		BEGIN_INTERFACE_TABLE(Set)
			IMPLEMENT_INTERFACE(I_ForeignPlugin)
			IMPLEMENT_INTERFACE(I_Serializable)
		END_INTERFACE_TABLE()

	protected://////////////////////////////////////////////////////////////////////////////////

virtual		  				~Set( void );


    public://///////////////////////////////////////////////////////////////////////////////////

							Set( void );

							// virtual ctor.
virtual Set*				Clone( bool inCopy = true ) const  = 0;


    public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// Properties:

		// <Count>
							/** Returns the count of items in the set. */
virtual vuint32				get_Count( void ) const	= 0;

							/// Returns TRUE if set is empty.
virtual bool				get_IsEmpty( void ) const  = 0;

		// <Iterator>
							/** Create instance of iterator and return it.
								This is virtual factory. */
virtual SetIterator_Ptr		get_Iterator( void ) const	= 0;

virtual	bool				get_IsSorted( void ) const  = 0;
virtual	void				put_IsSorted( bool inSorted )  = 0;


	// ---------------------
	// Item Methods:

							/// Adds value to the set. Valid range 1..mMaxValue.
virtual void				Append(	ID_TYPE inItem ) = 0;

							/// Returns TRUE if inItem is in the set.
virtual bool				Include( ID_TYPE inItem ) const = 0;

							/// Removes value from the set. Valid range 1..mMaxValue.
virtual void				Remove(	ID_TYPE inItem ) = 0;


	// ---------------------
	// BATCH Item Methods:

							// inLeftIncrementedPtr - points to the first/left of array.
							//		This pointer will be INCREMENTED by alg to move forward.
							// inRightFixedPtr - points AFTER the last/right item of array.		
							// The given array must be SORTED ASCENDING !!!
virtual void 				AppendSorted_123( 
								ID_TYPE*	inLeftIncrementedPtr, 
								ID_TYPE* 	inRightFixedPtr ) = 0;

							// inLeftFixedPtr - points BEFORE the first/left item of array.
							// inRightFixedPtr - points to the last/right item of array.
							//		This pointer will be DECREMENTED by alg to move backward.
							// The given array must be SORTED DESCENDING !!!
virtual void 				AppendSorted_321( 
								ID_TYPE*	inLeftFixedPtr, 
								ID_TYPE* 	inRightDecrementedPtr ) = 0;


	// ---------------------
	// Sorting Methods:

							/// Sort items by values.
virtual	void				Sort( void )  = 0;


	//----------------------
	// Unary Operators:
	//
	// The result is retured as a new Set of the same type as 'this'.
	// This operations can Sort() this Set to proceed.

virtual Set_Ptr				Union 		 		( const Set_Ptr inSet ) = 0; // OR
virtual Set_Ptr				Intersection		( const Set_Ptr inSet ) = 0; // AND
virtual Set_Ptr				Difference   		( const Set_Ptr inSet ) = 0; // AND NOT
virtual Set_Ptr				SymmetricDifference	( const Set_Ptr inSet ) = 0; // XOR

};


#pragma mark -


/**********************************************************************************************/
// RZ 2010-04-15 
// TODO AppendItems on pointers.


/**********************************************************************************************/
/**
	General algorithm:
	@param inSet - the set to add new items.
	@param inArray - Pointer on the first item of array of items.
	@param inHowMuch - specifies how much items of array we should append.
	@param inForwardDirection - if TRUE then we move in array forward. Else -- backward.
*/
FBL_SHARED_EXP void AppendItems(
	Set_Ptr		inSet,
	ID_TYPE* 	inArray,
	vuint32 	inHowMuch,
	bool		inForwardDirection = true );





/**********************************************************************************************/
// Tests if the RESULT is empty.
//
inline bool IsEmpty( Set_Ptr set )
{
	return !set || set->get_Count() == 0;
}


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_Set_h

