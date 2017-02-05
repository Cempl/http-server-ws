/**********************************************************************************************/
/* FBL_ArraySet.cpp																			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                       					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

// STD:
#include <algorithm>

// VSAHRED:
#include <VShared/FBL/publ/Errors/FBL_Exceptions.h>

#include <VShared/FBL/publ/Sets/FBL_ArraySet.h>
#include <VShared/FBL/publ/Sets/FBL_BitSet.h>

#include <VShared/FBL/publ/Sets/FBL_BitSetIterator.h>
#include <VShared/FBL/publ/Sets/FBL_ArraySetIterator.h>
#include <VShared/FBL/publ/Utilities/FBL_Sort.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
//void qsort_long( vuint32* first, vuint32* last );


/**********************************************************************************************/
vuint32 ArraySet::sTotalSize = 0;


/**********************************************************************************************/
ArraySet::ArraySet( vuint32 ioLimitCount ) 
{
	InitSelf( ioLimitCount );
}


/**********************************************************************************************/
// Class of list of records IDs. 
//
ArraySet::ArraySet( const ArraySet& inOtherArraySet )
{	
	InitSelfWithArraySet( inOtherArraySet );
}


/**********************************************************************************************/
// Convert bit vector to the ArraySet.
// Note, Array is sorted automatically!
//
ArraySet::ArraySet( const BitSet& inSet, vuint32 ioLimitCount )
{
	InitSelf( inSet.get_Bits_1_Count() );
	
	SetIterator_Ptr iter = inSet.get_Iterator();
	SetIterator& refIter = *iter;

	ID_TYPE id;
	if( (id = refIter.FirstItem()) != 0 )
	{			
		do
		{
			Append( id );
		}
		while( (id = refIter.NextItem()) && ioLimitCount-- ); 
	
		put_IsSorted( true );
	}		
}


/**********************************************************************************************/
ArraySet::~ArraySet( void )
{
	sTotalSize -= get_Size();

	if( mpStart )
		delete [] mpStart;
		
	mpStart 		= nullptr;
	mpFinish 		= nullptr;
	mpStorageEnd 	= nullptr;
}


/**********************************************************************************************/
void ArraySet::InitSelf( vuint32 inMaxCount )
{	
	if( inMaxCount )
	{
		mpStart 		= new ID_TYPE[inMaxCount];	
		mpFinish 		= mpStart;
		mpStorageEnd 	= mpStart + inMaxCount;
        
        sTotalSize += get_Size();
	}
	else
	{
		mpStart 		= nullptr;	
		mpFinish 		= nullptr;
		mpStorageEnd 	= nullptr;
	}
	
	mIsSorted = false;
}


/**********************************************************************************************/
void ArraySet::InitSelfWithArraySet( const ArraySet& inSet )
{	
	// A new array will get the size EXACTLY as the number of items in the inSet.
	// i.e. we do not copy empty space. 
	vuint32 inItems = inSet.get_Count();

	InitSelf( inItems );

	memcpy( mpStart, inSet.begin(), inItems * sizeof(ID_TYPE) );
	mpFinish = mpStart + inItems;

	mIsSorted = inSet.mIsSorted;
}


#pragma mark -
#pragma mark ====== Set API ======= 


#pragma mark -
#pragma mark ====== Construction Methods ======= 


/**********************************************************************************************/
Set* ArraySet::Clone( bool inCopy ) const 
{ 
	return inCopy ? new ArraySet(*this) : new ArraySet(get_MaxCount());
}


#pragma mark -
#pragma mark ====== Properties ======= 


/**********************************************************************************************/
SetIterator_Ptr ArraySet::get_Iterator( void ) const
{
	return new ArraySetIterator( *this );
}


#pragma mark -
#pragma mark ====== Item Methods ======= 


/**********************************************************************************************/
// Returns true if given Number is in list. The top bound of list is limited by Count.
//
bool ArraySet::Include( ID_TYPE inItem ) const
{
	bool res = false;

	if( mpStart < mpFinish )
	{
        if( mIsSorted )
        {
            res = STD::binary_search( mpStart, mpFinish, inItem );
        }
        else // perform linear search:
        {
            ID_TYPE* p = mpStart;
            while( (p < mpFinish) && (*p != inItem) )
                p++;
            
            res = bool( p < mpFinish );
        }	
	}

	return res;
}


/**********************************************************************************************/
// Remove inItem from the array.
//
void ArraySet::Remove( ID_TYPE inItem )
{
	ID_TYPE* p;

	/* At first try to find inItem */
	if( mIsSorted )
	{
		p = STD::lower_bound( mpStart, mpFinish, inItem );	
		if( p != mpFinish && *p > inItem )
			p = mpFinish;
	}
	else 
	{
		p = mpStart;
		while( p < mpFinish && *p != inItem )
			p++;
	}	

	/* If found remove it */
	if( p < mpFinish )
		 RemoveAt( (vuint32)(p - mpStart + 1 ) );
}


#pragma mark -
#pragma mark ====== Batch Item Methods ======= 


/**********************************************************************************************/
void ArraySet::AppendSorted_123( 
	ID_TYPE*	inLeftIncrementedPtr, 
	ID_TYPE* 	inRightFixedPtr )
{
	ID_TYPE ItemsToAppend = ID_TYPE(inRightFixedPtr - inLeftIncrementedPtr);
	ID_TYPE ItemsFree 	  = ID_TYPE(mpStorageEnd - mpFinish);

	if( ItemsToAppend > ItemsFree )	// there is no place, reallocate.
	{
						  // we have                		  // we need more of that
		ID_TYPE NewSize = ID_TYPE((mpStorageEnd - mpStart) + (ItemsToAppend - ItemsFree));

		Resize( NewSize ); 
	}	
	
	// COPY data:
	memcpy( mpFinish, inLeftIncrementedPtr, ItemsToAppend );
	
	// correct pointers of this ArraySet.
	mpFinish += ItemsToAppend;
	FBL_CHECK( mpFinish <= mpStorageEnd );
}


/**********************************************************************************************/
void ArraySet::AppendSorted_321( 
	ID_TYPE*	inLeftFixedPtr, 
	ID_TYPE* 	inRightDecrementedPtr )
{
	ID_TYPE ItemsToAppend = ID_TYPE(inRightDecrementedPtr - inLeftFixedPtr);
	ID_TYPE ItemsFree 	  = ID_TYPE(mpStorageEnd - mpFinish);
	
	if( ItemsToAppend > ItemsFree )	// there is no place, reallocate.
	{
		// we have                // we need more of that
		ID_TYPE NewSize = ID_TYPE((mpStorageEnd - mpStart) + (ItemsToAppend - ItemsFree));
		
		Resize( NewSize ); 
	}
    
    
	// COPY data:
	while( inLeftFixedPtr < inRightDecrementedPtr )
	{

#if 0
	// TODO FIXME. RZ 215-03-03 I have try to add this check, but it is not such simple... Oops
    // PROBLEM is that we appending SORTED items, but ArraySet can be not sorted itself, so we cannot just compare.
    //
    // If we will try do this only if ArraySet is sorted it will work, but another PROBLEM.
    // I see that Empty ArraySet must have mIsSorted = true. Why not?  Now it is false.
    // So this check will not work anyway if we starting AppendSorted() into empty ArraySet. Ops.
    //
    // And when it have one item it is still sorted.
    // But when we appending OTHER itmes using OTHER not AppendSorted_ algs, then may be
    // we can check if array is still sorted...

    #if _FBL_CHECK
        // in DEBUG MODE, we checking that the next value to append is bigger of prev.
        if( mIsSorted && mpFinish > mpStart ) // if we have at least one item in ArraySet
        {
            FBL_CHECK( *(mpFinish - 1) < *inRightDecrementedPtr );
        }
    #endif // _FBL_CHECK
#endif // 0
       
		*mpFinish++ = *inRightDecrementedPtr--;
	}

	FBL_CHECK( mpFinish <= mpStorageEnd );
}


#pragma mark -
#pragma mark ====== Sorting Methods ======= 


/**********************************************************************************************/
// Sorts items in array in ascending order by values.
//
void ArraySet::Sort( void ) 
{
	if( mIsSorted == false && mpFinish > mpStart )
	{
		FBL::sort( mpStart, mpFinish );
		mIsSorted = true;
	}
}


#pragma mark -
#pragma mark ====== Set Operations with other Set ======= 


/**********************************************************************************************/
Set_Ptr ArraySet::Union( const Set_Ptr inSet )
{
	Set_Ptr pRes;

	if( inSet == nullptr )
    {
		pRes = this-> Clone();
	}
    else
    {
        ArraySet_Ptr pArraySet = fbl_dynamic_cast<ArraySet>( inSet.get() );
        if( pArraySet )
        {
            pRes = this->Union( pArraySet );
        }
        else
        {
            FBL_CHECK( false ); // FIXME.
        }
	}

	return pRes;
}


/**********************************************************************************************/
Set_Ptr ArraySet::Intersection( const Set_Ptr inSet )
{
	Set_Ptr pRes;

	if( inSet )
	{
        if( ArraySet_Ptr pArraySet = fbl_dynamic_cast<ArraySet>(inSet.get()) )
        {
            pRes = Intersection( pArraySet );
        }
        else if( BitSet_Ptr pBitSet = fbl_dynamic_cast<BitSet>(inSet.get()) )
        {
            ArraySet_Ptr arraySet = new ArraySet( *pBitSet.get() );
            pRes = Intersection( arraySet );
        }
	}

	return pRes;
}


/**********************************************************************************************/
Set_Ptr ArraySet::Difference( const Set_Ptr inSet )
{
	Set_Ptr pRes;

	if( inSet == nullptr )
    {
		pRes = this->Clone();
	}
    else
    {
        ArraySet_Ptr pArraySet = fbl_dynamic_cast<ArraySet>( inSet.get() );
        if( pArraySet )
        {
            pRes = this->Difference( pArraySet );
        }
        else
        {
            FBL_CHECK( false ); // FIXME.
        }
    }

	return pRes;
}


/**********************************************************************************************/
Set_Ptr ArraySet::SymmetricDifference	( const Set_Ptr inSet )
{
	Set_Ptr pRes;

	if( inSet == nullptr )
    {
		pRes = this->Clone();
	}
    else
    {
        ArraySet_Ptr pArraySet = fbl_dynamic_cast<ArraySet>( inSet.get() );
        if( pArraySet )
        {
            pRes = this->SymmetricDifference( pArraySet );
        }
        else
        {
            FBL_CHECK( false ); // FIXME.
        }
	}
    
    return pRes;
}
 

#pragma mark -
#pragma mark ====== I_Serializable API ======= 


/**********************************************************************************************/
void ArraySet::From( 
	I_IStream_Ptr inStream,
	bool		  inBlock )
{
	argused1( inBlock );

	// Destroy existing content first.
	InitSelf( 0 );

	// 1) Allocated len.
	vuint32 allocLen;
	inStream->get( allocLen );

	if( allocLen > 0 )
	{
		InitSelf( allocLen / sizeof(REC_ID) );

		// 2) Actual len.
		vuint32 actualLen;
		inStream->get(actualLen);
		if( actualLen > 0)
		{
			// 3) BitSet's data.
			inStream->get((vuint8*)mpStart, actualLen);
			mpFinish = mpStart + (actualLen / sizeof(REC_ID));

			// 4) 'Sorted' flag.
			inStream->get( mIsSorted );
		}
	}
}


/**********************************************************************************************/
void ArraySet::From( 
	I_PacketRcv* 	inPacket,
	bool		  	inBlock )
{
	argused1( inBlock );

	// Destroy existing content first.
	InitSelf( 0 );

	// 1) Allocated len.
	vuint32 allocLen = inPacket->get_ULongParam();

	if( allocLen > 0 )
	{
		InitSelf( allocLen / sizeof(REC_ID) );

		// 2) Actual len.
		vuint32 actualLen = inPacket->get_ULongParam();
		if( actualLen > 0)
		{
			// 3) BitSet's data.
			inPacket->get_BinaryParam( mpStart, actualLen );
			
			mpFinish = mpStart + (actualLen / sizeof(REC_ID));

			// 4) 'Sorted' flag.
			mIsSorted = inPacket->get_BoolParam();
		}
	}
}


/**********************************************************************************************/
void ArraySet::To( 
	I_OStream_Ptr inStream,
	bool		  inBlock ) const
{
	argused1(inBlock);
	
	// Type of set.
	inStream->put(vuint8(1));

	if( mpStart )
	{
		vuint32 allocLen  = static_cast<vuint32>((mpStorageEnd - mpStart) * sizeof(vuint32));
		vuint32 actualLen = static_cast<vuint32>((mpFinish - mpStart)     * sizeof(vuint32));

		// 1) Allocated len (in bytes).
		inStream->put(allocLen);

		if( actualLen > 0 )
		{
			// 2) Actual len (in bytes).
			inStream->put(actualLen);
			
			// 3) Data of actual len.
			inStream->put((vuint8*)mpStart, actualLen);

			// 4) 'Sorted' flag.
			inStream->put(mIsSorted);
		}
		else
		{
			inStream->put( vuint32(0) );
		}
	}
	else
	{
		inStream->put( vuint32(0) );
	}
}


/**********************************************************************************************/
void ArraySet::To( 
	I_PacketSnd*	inPacket,
	bool		  	inBlock ) const
{
	argused1(inBlock);
	
	FixParamCount fixParamCount( inPacket );
	
	// Type of set.
	inPacket->put_UCharParam(vuint8(1));

	if( mpStart )
	{
		vuint32 allocLen  = static_cast<vuint32>((mpStorageEnd - mpStart) * sizeof(vuint32));
		vuint32 actualLen = static_cast<vuint32>((mpFinish - mpStart)     * sizeof(vuint32));

		// 1) Allocated len (in bytes).
		inPacket->put_ULongParam(allocLen);

		if( actualLen > 0 )
		{
			// 2) Actual len (in bytes).
			inPacket->put_ULongParam(actualLen);
			
			// 3) Data of actual len.
			inPacket->put_BinaryParam( mpStart, actualLen );

			// 4) 'Sorted' flag.
			inPacket->put_BoolParam(mIsSorted);
		}
		else
		{
			inPacket->put_ULongParam(0UL);
		}
	}
	else
	{
		inPacket->put_ULongParam(0UL);
	}
}


#pragma mark -
#pragma mark ====== THIS CLASS API: ======= 

#pragma mark -
#pragma mark ====== Set Methods ======= 


/**********************************************************************************************/
// Make new list of adresses - intersection of pA and pB.
//
ArraySet_Ptr ArraySet::Intersection( ArraySet_Ptr inListPtr ) 
{
	ArraySet_Ptr pRes;

	if( inListPtr )
	{
        ArraySet_Ptr pA = this;
        ArraySet_Ptr pB = inListPtr;
            
        ID_TYPE CountC = pA->get_Count();

        if( pB->get_Count() < CountC )
            CountC = pB->get_Count();

        ArraySet_Ptr pC = new ArraySet( CountC );	
        
        if( CountC )
        {
            pA->Sort();
            pB->Sort();
            
            pC->mpFinish = STD::set_intersection( 
                                    pA->mpStart, pA->mpFinish, 
                                    pB->mpStart, pB->mpFinish, 
                                    pC->mpStart );
        
        }
        
        pC->mIsSorted = true;
            
        pRes = pC->get_IsEmpty() ? nullptr : pC;
	}
    
    return pRes;
}


/**********************************************************************************************/
// Make new list of adresses - union of pA and pB.
//
ArraySet_Ptr ArraySet::Union( ArraySet_Ptr inListPtr ) 
{
	ArraySet_Ptr pRes;

	if( inListPtr == nullptr )
	{
		pRes = (ArraySet*) this->Clone();
	}
    else
    {
        ArraySet_Ptr pA = this;
        ArraySet_Ptr pB = inListPtr;
            
        pA->Sort();
        pB->Sort();
        
        ID_TYPE CountC = pA->get_Count() + pB->get_Count();
        ArraySet_Ptr pC = new ArraySet( CountC );	
        
        if( CountC )
        {
            pC->mpFinish = STD::set_union( 
                                    pA->mpStart, pA->mpFinish, 
                                    pB->mpStart, pB->mpFinish, 
                                    pC->mpStart );	
        }
        
        pC->mIsSorted = true;

        pRes = pC->get_IsEmpty() ? nullptr : pC;
	}

    return pRes;
}


/**********************************************************************************************/
// Make new list of adresses - difference of pA and pB.
//
ArraySet_Ptr ArraySet::Difference( ArraySet_Ptr inListPtr ) 
{
	ArraySet_Ptr pRes;

	if( inListPtr == nullptr )
	{
		pRes = (ArraySet*) this->Clone();
	}
    else
    {
        ArraySet_Ptr pA = this;
        ArraySet_Ptr pB = inListPtr;
            
        pA->Sort();
        pB->Sort();
        
        ID_TYPE CountC = pA->get_Count();
        ArraySet_Ptr pC = new ArraySet( CountC );	
        
        if( CountC )
        {
            pC->mpFinish =STD:: set_difference(	
                                    pA->mpStart, pA->mpFinish, 
                                    pB->mpStart, pB->mpFinish, 
                                    pC->mpStart );	
        }
        
        pC->mIsSorted = true;

        pRes = pC->get_IsEmpty() ? nullptr : pC;
	}
    
    return pRes;
}


/**********************************************************************************************/
// Make new list of adresses - difference of pA and pB.
//
ArraySet_Ptr ArraySet::SymmetricDifference( ArraySet_Ptr inListPtr ) 
{
	ArraySet_Ptr pRes;

	if( inListPtr == nullptr )
	{
		pRes = (ArraySet*) this->Clone();
	}
    else
    {
        ArraySet_Ptr pA = this;
        ArraySet_Ptr pB = inListPtr;
            
        pA->Sort();
        pB->Sort();
        
        ID_TYPE CountC = pA->get_Count() + pB->get_Count();
        ArraySet_Ptr pC = new ArraySet( CountC );	
        
        if( CountC )
        {
            pC->mpFinish = STD::set_symmetric_difference( 
                                    pA->mpStart, pA->mpFinish, 
                                    pB->mpStart, pB->mpFinish, 
                                    pC->mpStart );
        }
                
        pC->mIsSorted = true;

        pRes = pC->get_IsEmpty() ? nullptr : pC;
	}
    
    return pRes;
}


/**********************************************************************************************/
// Execute current operation on lists pResList and inListPtr.
// It make (pResList op= inListPtr), i.e it don't create additional ArraySet.
//
ArraySet_Ptr ArraySet::MakeOperation( 
		ArraySet_Ptr 		inLeftSet, 
		SetOperation	inOperation,
		ArraySet_Ptr 		inRightSet ) 
{
	FBL_CHECK( inLeftSet && inRightSet );

	ArraySet_Ptr pNewRes;

	switch( inOperation )
	{
		case kIntersection:		
				pNewRes = inLeftSet->Intersection( inRightSet );
				break;
		
		case kUnion:
				pNewRes = inLeftSet->Union( inRightSet );
				break;
		
		case kMinus:		
				pNewRes = inLeftSet->Difference( inRightSet );
				break;
				
		case kXOR:		
				pNewRes = inLeftSet->SymmetricDifference( inRightSet );
				break;
	}

	return pNewRes;	
}


#pragma mark -
#pragma mark ====== Item Methods: ======= 


/**********************************************************************************************/
void ArraySet::AppendItems( ID_TYPE* pArray, vuint32 inCount )
{
	ID_TYPE* pNext = pArray;
	 
	if( mpFinish + inCount >= mpStorageEnd )	// must be free place
	{
		ID_TYPE NewSize = (mpStorageEnd - mpStart) ? (ID_TYPE)(2 * (mpStorageEnd - mpStart)) 
												 : ID_TYPE(100);
		Resize( NewSize ); 
	}
	
	while( inCount-- )
		*mpFinish++ = *pNext++;		

	mIsSorted = false;
}


/**********************************************************************************************/
void ArraySet::AppendItemsReverse( ID_TYPE* pArray, vuint32 inCount )
{
	ID_TYPE* pNext = pArray;
	 
	if( mpFinish + inCount >= mpStorageEnd )	// must be free place
	{
		ID_TYPE NewSize = (mpStorageEnd - mpStart) ? ID_TYPE( 2 * (mpStorageEnd - mpStart) ) 
												 : ID_TYPE( 100 );
		Resize( NewSize ); 
	}
	
	while( inCount-- )
		*mpFinish++ = *pNext--;		

	mIsSorted = false;
}


/**********************************************************************************************/
void ArraySet::InsertItem ( vuint32 inPos, ID_TYPE inItem )
{
	/*if( mpFinish + 1 >= mpStorageEnd )	// must be place
	{
		ID_TYPE NewSize = (mpStorageEnd - mpStart) ? (ID_TYPE)2 * (mpStorageEnd - mpStart) : (ID_TYPE)100;
		Resize( NewSize ); 
	}*/

	vuint32 theCount = get_Count();

	FBL_CHECK( mpFinish < mpStorageEnd );	// must be place
	FBL_CHECK( inPos <= theCount );
	
	ID_TYPE* p = mpStart + inPos;
	memmove( p + 1, p, (theCount - inPos) * sizeof(ID_TYPE) );
	
	*p = inItem;
	
	mpFinish++;
	mIsSorted = false;
}


/**********************************************************************************************/
void ArraySet::RemoveAt ( vuint32 inIndex, vuint32 inCount )
{
	FBL_CHECK( 1 <= inIndex && inIndex <= get_Count() );	// range is 1..
	
	ID_TYPE* p = mpStart + inIndex - 1;
	memmove( p, p + inCount, (get_Count() - inIndex - inCount + 1) * sizeof(ID_TYPE) );
	
	mpFinish -= inCount;
}


/**********************************************************************************************/
// Returns index of the given inItem in the array. 
// Returns 0 if inItem is not found.
// If there is inFrom and inTo then use them as bounds for search (1..Count)
//
vuint32 ArraySet::FindIndex( ID_TYPE inItem, vuint32 inFrom, vuint32 inTo ) 
{
	vuint32 res = 0;

	if( mpStart < mpFinish )
	{
        FBL_CHECK( inFrom <= inTo );
            
        ID_TYPE*	theStart 	= inFrom ? mpStart + (inFrom - 1) : mpStart;
        ID_TYPE*	theFinish	= inTo 	 ? mpStart + inTo 		: mpFinish;
        
        if( theFinish > mpFinish )
            theFinish = mpFinish;
        
        if( mIsSorted )
        {
            ID_TYPE* i = STD::lower_bound(theStart, theFinish, inItem);
            
            if( i != theFinish && ! (inItem < *i) )
                    res = (vuint32)(i - theStart + 1);
            else 	res = (vuint32)0;

        }
        else
        {
            // simply iterate array:
            ID_TYPE* p = theStart;
            while( (p < theFinish) && (*p != inItem) )
                p++;
                
            if( p == theFinish )
                    res = (vuint32)0;
            else 	res = (vuint32)(p - theStart + 1);
        }
	}
    
    return res;
}


#pragma mark -
#pragma mark ====== Advanced Methods ======= 


/**********************************************************************************************/
// Can both grow and reduse the size of ArraySet.
// Can truncate exisitng data !!
// If NewItemsCount is 0 then free memory.
// Note, that it requires additional memory buffer during work.
//
void ArraySet::Resize( vuint32 inNewItemsCount )
{
	if( inNewItemsCount == 0 && mpStart )	// we want to free memory
	{
    	FBL_CHECK( get_Size() <= sTotalSize );
    	sTotalSize -= get_Size();
    
		delete [] mpStart;
		
		mpStart 		= nullptr;
		mpFinish		= nullptr;
		mpStorageEnd = nullptr;
	}
    else
	{
		ID_TYPE* pTmp = new ID_TYPE[inNewItemsCount];

//    	vuint32 MaxCount = get_MaxCount();
		vuint32	Count 	 = get_Count();
		
		// Choose a min value:
		// IF NewItemsCount is less THEN we trucate existing data.
		Count = (Count < inNewItemsCount) ? Count : inNewItemsCount;
		
		if( mpStart )
		{
			memcpy( (char*)pTmp, (char*)mpStart, Count * sizeof(ID_TYPE) ); 
			delete [] mpStart;
		}
		
		
		// fill by zero rest items if any.
		memset( pTmp + Count, 0, inNewItemsCount - Count ); 
		
		mpStart 		= pTmp;
		mpFinish		= mpStart + Count;
		mpStorageEnd = mpStart + inNewItemsCount;
        
        // CORRECT counter using +/- diff value.
        vint32 diff = (inNewItemsCount > Count) ? + (vint32)(inNewItemsCount - Count)
        										: - (vint32)(Count - inNewItemsCount);
        sTotalSize += diff * sizeof(ID_TYPE);
	}
}


/**********************************************************************************************/
// If array was changed directly throw pointers (for speed) then you must set the right 
// position of mpFinish marker.
//
// It is assumed that allocation size is not changed!
//
void ArraySet::SetItemsCount( ID_TYPE inNewCount )
{
	ID_TYPE theCount = get_MaxCount();
	
	if( inNewCount < theCount )	// min().
		theCount = inNewCount;
	
	mpFinish = mpStart + theCount;
}


/**********************************************************************************************/
// Check space before adding new data.
// If there is no more space, resize array, but not more than inMaxPossible.
//
void ArraySet::Tune( vuint32 inMaxPossible )
{
	if( get_Count() == get_MaxCount() )
	{
		ID_TYPE NewCount = 2 * get_MaxCount();
		
		if( NewCount == 0 )
			NewCount = 10;
		
		Resize( NewCount < inMaxPossible ? NewCount : inMaxPossible );
	}		
}



#pragma mark -
#pragma mark ====== HELPER ALGORITHMS ======


/**********************************************************************************************/
ArraySet_Ptr CopyItemsExcept(
   const ArraySet_Ptr	inSrcArraySet,
   const Set_Ptr		inItemsToCopy,
   vuint32				inColumns,
   vuint32				inCopyCount )
{
	if( inCopyCount == vuint32_max ) // i.e. not given us and we need calcualte it.
		inCopyCount = inItemsToCopy->get_Count();

	// We need create a new object of ArraySet and copy required items.	
	ArraySet_Ptr pNewArray  = new ArraySet( inCopyCount * inColumns );

	REC_ID* recIDPtr	= inSrcArraySet->begin();
	REC_ID* pDestRow	= pNewArray->begin();

	SetIterator_Ptr pIter = inItemsToCopy->get_Iterator();
	if( vuint32 pos = pIter->FirstItem() )
	{
		do
		{
			REC_ID* p = recIDPtr + (pos - 1) * inColumns;

			if( inColumns == 1 ) // simple case - do fast.
			{
				*pDestRow++ = *p;
			}
			else // copy the whole "record" - few columns at once.
			{
				vuint32 i = inColumns;
				while( i-- )
					*pDestRow++ = *p++;
			}
		}
		while( (pos = pIter->NextItem()) );

		pNewArray->SetItemsCount( inCopyCount * inColumns );
	}

	return pNewArray;
}


/**********************************************************************************************/
ArraySet_Ptr RemoveItemsExcept(
   const ArraySet_Ptr	inSrcArraySet,
   const Set_Ptr		inItemsToCopy,
   vuint32				inColumns )
{
	ArraySet_Ptr pRes;

	vuint32 copyCount = inItemsToCopy->get_Count();

	if( copyCount == 0 )
	{
		inSrcArraySet->Resize(0); // Returns the same array object, just make it empty.
		pRes = inSrcArraySet;
	}
	else
    {
        vuint32 recCount = inSrcArraySet->get_Count();
        if( copyCount == recCount ) // All records already distinct. Return the same array.
        {
            pRes = inSrcArraySet;
        }
		else // We already know Copycount, so pass it to the next func to avoid second calculation.
        {
            pRes = CopyItemsExcept( inSrcArraySet, inItemsToCopy, inColumns, copyCount );
		}
	}
    
    return pRes;
}


/**********************************************************************************************/
void SimpleUnion( ArraySet_Ptr ioSet, ArraySet_Ptr inSet )
{
	FBL_CHECK( ioSet );
	FBL_CHECK( inSet );

	vuint32 count = inSet->get_Count();
	for( vuint32 i = 0; i < count; ++i )
	{
		REC_ID* p = (REC_ID*) (inSet->begin()+i);
		ioSet->Append( *p );
	}
}


/**********************************************************************************************/
FBL_End_Namespace
