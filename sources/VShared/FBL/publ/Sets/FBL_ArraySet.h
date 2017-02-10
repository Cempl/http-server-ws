/**********************************************************************************************/
/* FBL_ArraySet.h																			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                       					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_ArraySet_h 
	#define _FBL_ArraySet_h
#pragma once

#include <VShared/FBL/publ/Sets/FBL_Set.h>
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>

#include <memory>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_CLASS( BitSet );
SMART_CLASS( ArraySet );
SMART_CLASS( SetIterator );


/**********************************************************************************************/
/*
	General purpose class.
	Implements Set of ULONG numbers as array.	
	Can contain up to MAX_ULONG items.
	ArraySet can change its size in both directions.
*/
class FBL_SHARED_EXP_CLASS ArraySet : public Set
{ 	
		friend class ArraySetIterator;

    public://///////////////////////////////////////////////////////////////////////////////////

							// Allocate ArraySet 
							ArraySet( 
								vuint32				inLimitCount );
							
							// Copy Ctor.
							ArraySet( 
								const ArraySet&		inOtherArraySet );
							
							// BitSet to ArraySet converter.
							// You can set inLimitCount e.g. to 10, 
							// then only 10 first items will be copied into ArraySet.
							ArraySet( 
								const BitSet&		inSet, 
								vuint32				inLimitCount = vuint32_max );

virtual			  			~ArraySet( void );


    public://///////////////////////////////////////////////////////////////////////////////////

// Set API:
		
	//----------------------
	// Properties:

virtual vuint32				get_Count( void ) const override;

virtual bool				get_IsEmpty( void ) const override  
								{ return bool( mpFinish == mpStart ); } 

virtual SetIterator_Ptr		get_Iterator( void ) const override;

virtual	bool				get_IsSorted( void ) const override;
virtual	void				put_IsSorted( bool inSorted ) override;


	// ---------------------
	// Construction Methods:

virtual Set*				Clone( bool inCopy = true ) const override;


	// ---------------------
	// Item Methods:

							/// ATTENTION: it just appends items to the array. It not checks that 
							/// item already exists. This works for now. 	
virtual	void				Append( ID_TYPE inItem ) override;

virtual	bool				Include( ID_TYPE inItem ) const override;

							/// Finds the specified inItem in the array and removes it.
							/// Items after removed one are moved forward.
virtual	void				Remove( ID_TYPE inItem ) override;	


	// ---------------------
	// BATCH Item Methods:

virtual void 				AppendSorted_123( 
								ID_TYPE*	inLeftIncrementedPtr, 
								ID_TYPE* 	inRightFixedPtr ) override;

virtual void 				AppendSorted_321( 
								ID_TYPE*	inLeftFixedPtr, 
								ID_TYPE* 	inRightDecrementedPtr ) override;


	// ---------------------
	// Sorting Methods:
				
							/// Sort items by values.
virtual	void				Sort( void ) override;

							
	//----------------------
	// Unary Operators:

virtual Set_Ptr				Union 		 		( const Set_Ptr inSet ) override; // OR
virtual Set_Ptr				Intersection		( const Set_Ptr inSet ) override; // AND
virtual Set_Ptr				Difference   		( const Set_Ptr inSet ) override; // AND NOT
virtual Set_Ptr				SymmetricDifference	( const Set_Ptr inSet ) override; // XOR

	
	public://///////////////////////////////////////////////////////////////////////////////////

// I_Serializable API:

    	// ----------------
		// Read Methods:

virtual void				From( I_IStream_Ptr 	inStream,
								  bool 				inBlock = true ) override;

virtual void				From( I_PacketRcv*	 	inPacket,
								  bool 				inBlock = true ) override;


    	// ----------------
		// Write Methods:
	
virtual void				To( I_OStream_Ptr 		inStream,
								bool 				inBlock = true ) const override;


virtual void				To( I_PacketSnd*		inPacket,
								bool 				inBlock = true ) const override;


    public://///////////////////////////////////////////////////////////////////////////////////

// THIS CLASS API:

	//-----------------
	// Properties:

		vuint32				get_MaxCount( void ) const;
    
                            // Returns allocated size in bytes.
		vuint32				get_Size( void ) const
								{ return (ID_TYPE)(mpStorageEnd - mpStart) * sizeof(ID_TYPE); }

							/// Returns reference on item at the specified position. 1..
		ID_TYPE&	 		operator()( vuint32 inPos ) const;
				
    						// Returns how much bytes use all existed ArraySet objects.
static	vuint32				get_TotalSize( void ) { return sTotalSize; }


	//-----------------
    // Set Operations:

		// Note: for much better performance use BitSet Methods (if your task allows this).

		ArraySet_Ptr		Intersection 		( ArraySet_Ptr inArraySet ); // AND
		ArraySet_Ptr		Union		 		( ArraySet_Ptr inArraySet ); // OR
		ArraySet_Ptr		Difference			( ArraySet_Ptr inArraySet ); // AND NOT
		ArraySet_Ptr		SymmetricDifference	( ArraySet_Ptr inArraySet ); // XOR
		
							// This method is dispatcher between above 4 Set Operations. 
static ArraySet_Ptr			MakeOperation( 		
								ArraySet_Ptr 	inLeftSet, 
								SetOperation	inOperation,
								ArraySet_Ptr 	inRightSet );						

	// ---------------------
	// Item Methods:
	
							/// Append several items at once.
		void 				AppendItems(
        						ID_TYPE* 	pArray,
                                vuint32 	inCount );

							/// Append several items in reverse order at once.
		void 				AppendItemsReverse(
        						ID_TYPE* 	pArray,
                                vuint32 	inCount );

							/// Insert item to the specified position.
							/// Next items moved back. 
		void				InsertItem(
        						vuint32 	inPos,
                                ID_TYPE 	inItem );
		
							/// Remove item at specified position, 0..
							/// Next items is moved forward.
		void				RemoveAt(
        						vuint32 	inPos,
                                vuint32 	inCount = 1 );
		
							/// Returns index (1..) of given inItem in set.
							/// Returns 0 if not found.
		vuint32	 			FindIndex(
        						ID_TYPE 	inItem,
                                vuint32 	inFrom = 0,
                                vuint32 	inTo = 0 );

    
	public://///////////////////////////////////////////////////////////////////////////////////
 
	// ---------------------
	// Advanced Methods:

		ID_TYPE*			begin( void ) const;	///< to have direct access.
		ID_TYPE*			end( void ) const;		///< to have direct access.
 
							/// If array was changed directly throw pointers (for speed) 
							/// then you must set right position of mpFinish marker.
		void 				SetItemsCount( ID_TYPE inNewCount );

							/// Can as grow, so and reduce size of set.Can truncate existing data.
			 				/// If NewItemsCount is 0 then free memory.
		void				Resize( vuint32 inNewItemsCount );

							/// Check space before adding new data.
							/// If there is no more space, resize array, but
							/// not more than inMaxPossible.
		void				Tune( vuint32 inMaxPossible );


	protected://////////////////////////////////////////////////////////////////////////////////

		void				InitSelf( ID_TYPE ioLimitCount );
		void				InitSelfWithArraySet(const ArraySet& inSet );


    protected://////////////////////////////////////////////////////////////////////////////////

		ID_TYPE*			mpStart;			///< points to the first item.
		ID_TYPE*			mpFinish;		///< points AFTER the last item.
		ID_TYPE*			mpStorageEnd;	

		bool				mIsSorted;
    
static 	vuint32				sTotalSize;		// Total Size of all existed ArraySet objects.
};


/**********************************************************************************************/
// HELPER ALGORITHM:
//
// RETURNS new ArraySet object, which has only items of inSrcArraySet, specified by inItemsToCopy bitset.
//
FBL_SHARED_EXP ArraySet_Ptr CopyItemsExcept(
								ArraySet_Ptr	inSrcArraySet,
								Set_Ptr			inItemsToCopy,
								vuint32			inColumns = 1,
								vuint32			inCopyCount = vuint32_max );


/**********************************************************************************************/
// HELPER ALGORITHM:
//
// Do the same as above method but before checks 
//		a) if we have zero items to copy - then fast return
//		b) if we have items to copy the same count as inSrcArraySet have.
//				then nothing to remove and fast return again.
//
// Otherwise we call CopyItemsExcept(), which create new array and copy required items.
// And we get back ArraySet with REMOVED items that was not specified in the inItemsToCopy set.
//
// so ATTENTION: this method can return as the same inSrcArraySet object so a new object.
//
FBL_SHARED_EXP ArraySet_Ptr RemoveItemsExcept(
								ArraySet_Ptr	inSrcArraySet,
								Set_Ptr			inItemsToCopy,
								vuint32			inColumns = 1 );


/**********************************************************************************************/
// HELPER ALGORITHM: appends inSet to the end of ioSet.
//
FBL_SHARED_EXP void Append( ArraySet_Ptr ioSet, ArraySet_Ptr inSet );


/**********************************************************************************************/
struct I_OStream;
struct I_IStream;


/**********************************************************************************************/
I_IStream& operator>>( I_IStream& inStream, ArraySet*& outSet );
I_OStream& operator<<( I_OStream& inStream, const ArraySet* inSet );


/**********************************************************************************************/
inline vuint32 ArraySet::get_MaxCount( void ) const	
{ 
	return (vuint32)(mpStorageEnd - mpStart); 
}

/**********************************************************************************************/
inline vuint32 ArraySet::get_Count( void ) const		
{ 
	return (vuint32)(mpFinish - mpStart);		
}	

/**********************************************************************************************/
inline bool ArraySet::get_IsSorted( void ) const 		
{ 
	return mIsSorted; 				
}

/**********************************************************************************************/
inline void ArraySet::put_IsSorted( bool inSorted ) 	
{ 
	mIsSorted = inSorted; 			
}
		
/**********************************************************************************************/
inline ID_TYPE* ArraySet::begin( void ) const	
{ 
	return mpStart;	
}	

/**********************************************************************************************/
inline ID_TYPE* ArraySet::end( void ) const	
{ 
	return mpFinish;	
}	


/**********************************************************************************************/
inline void ArraySet::Append( ID_TYPE inItem )
{
	if( mpFinish >= mpStorageEnd )	// there is no place, reallocate.
	{
		// first time we allocate 100 items, then we double the size of the array.
		ID_TYPE NewSize = (mpStorageEnd - mpStart) ? vuint32( 2 * (mpStorageEnd - mpStart) ) : vuint32(100);
		Resize( NewSize ); 
	}
	
	// RZ 2010-04-15 
	// TODO if new item bigger of last item of sorted array, then it is still sorted !!!
	
	*mpFinish++ = inItem;
	mIsSorted = false;
}


/**********************************************************************************************/
inline ID_TYPE& ArraySet::operator()( vuint32 inIndex ) const
{
	FBL_CHECK( inIndex >= 1 ); 			// range 1..
	FBL_CHECK( inIndex <= get_Count() ); 	

	return *(mpStart + inIndex - 1L);
	
}


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


#endif // _FBL_ArraySet_h
