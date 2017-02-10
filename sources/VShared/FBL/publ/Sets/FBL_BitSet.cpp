/**********************************************************************************************/
/* FBL_BitSet.cpp																		 	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                       					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Sets/FBL_BitSet.h>
#include <VShared/FBL/publ/Sets/FBL_ArraySet.h>
#include <VShared/FBL/publ/Sets/FBL_BitSetIterator.h>
#include <VShared/FBL/publ/TestTools/FBL_TestSystem_Globals.h>

#include <string.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
vuint32 BitSet::sTotalSize = 0;


/**********************************************************************************************/
const vuint8 BitSet::sMasks[8] =
{
    0x0001,
    0x0002,
    0x0004,
    0x0008,
    0x0010,
    0x0020,
    0x0040,
    0x0080
};


/**********************************************************************************************/
const vuint32 BitSet::sBitsCount[256] =
{
	0, 	1,	1,	2,	1,	2,	2,	3,		//   0 -   7
	1,	2,	2, 	3,	2,	3,	3,	4,		//   8 -  15
	1,	2, 	2, 	3, 	2,  3,  3,	4,		//  16 -  23
	2,	3, 	3,	4,	3,	4,	4,	5,		//  24 -  31
	1,	2,	2,	3,	2,	3,	3,	4,		//  32 -  39
	2,	3,	3,	4,	3,	4,	4,	5,		//  40 -  47
	2, 	3,	3,	4,	3,	4,	4,	5,		//  48 -  55
	3, 	4,	4,	5,	4,	5,	5,	6,		//  56 -  63
	1,	2,	2,	3,	2,	3,	3,	4,		//  64 -  71
	2,	3,	3,	4,	3,	4,	4,	5,		//  72 -  79
	2,	3,	3,	4,	3,	4,	4,	5,		//  80 -  87
	3,	4,	4,	5,	4,	5,	5,	6,		//  88 -  95
	2,  3,	3,	4,	3, 	4,	4,	5,		//  96 - 103
	3,	4,	4,	5,	4,	5,	5,	6,		// 104 - 111
	3,	4,	4,	5,	4,	5,	5,	6,		// 112 - 119
	4, 	5, 	5,	6,	5,	6,	6,	7,		// 120 - 127
	1,	2,	2,	3,	2,	3,	3,	4,		// 128 - 135
	2, 	3,	3,	4,	3, 	4,	4,	5,		// 136 - 143
	2,  3,	3,	4,	3, 	4,	4,	5,		// 144 - 151
	3,	4,	4,	5,	4,	5,	5,	6,		// 152 - 159
	2,  3,	3,	4,	3, 	4,	4,	5,		// 160 - 167
	3,	4,	4,	5,	4,	5,	5,	6,		// 168 - 175
	3,	4,	4,	5,	4,	5,	5,	6,		// 176 - 183
	4, 	5, 	5,	6,	5,	6,	6,	7, 		// 184 - 191
	2,	3,	3,	4,	3, 	4,	4,	5,		// 192 - 199
	3,	4,	4,	5,	4,	5,	5,	6,		// 200 - 207
	3,	4,	4,	5,	4,	5,	5,	6,		// 208 - 215
	4,  5, 	5,	6,	5,	6,	6,	7,		// 216 - 223
	3,  4,	4,	5,	4,	5,	5,	6,		// 224 - 231
	4,  5, 	5,	6,	5,	6,	6,	7,		// 232 - 239
	4,  5, 	5,	6,	5,	6,	6,	7,		// 240 - 247
	6,  6,	6,	7,	6,	7,	7,	8		// 248 - 255
};


/**********************************************************************************************/
BitSet_Ptr Union( 
	const BitSet& inSet1,
	const BitSet& inSet2 );

BitSet_Ptr Intersection( 
	const BitSet& inSet1,
	const BitSet& inSet2 );

BitSet_Ptr Difference( 
	const BitSet& inSet1,
	const BitSet& inSet2 );

BitSet_Ptr SymmetricDifference( 
	const BitSet& inSet1,
	const BitSet& inSet2 );


#pragma mark -


/**********************************************************************************************/
inline vuint32 CalcAlignBytes( vuint32 inBytes )
{
	return ( inBytes / FBL_WORD_SIZE + 1 ) * FBL_WORD_SIZE;
}


#pragma mark -


/**********************************************************************************************/
// Copy ctor
//
BitSet::BitSet( 
	ID_TYPE	inMaxValue )
{
	InitSelf( inMaxValue );
}


/**********************************************************************************************/
// Copy ctor
//
BitSet::BitSet( const BitSet& inSrc )
{
	mMaxValue 		= inSrc.mMaxValue;
	mBytesInArray	= inSrc.mBytesInArray;
    mCount_1		= inSrc.mCount_1;
    
	if( mMaxValue == 0 )
    {
		mArray = nullptr;
	}		
	else
    {
        mArray = new vuint8 [mBytesInArray];
        memcpy( (char*)mArray, (char*)inSrc.mArray, mBytesInArray );
	}
    
    sTotalSize += mBytesInArray;
}


/**********************************************************************************************/
// Constructs vector of bits from ArraySet.
//
BitSet::BitSet( 
	ID_TYPE 			inMaxValue,
	Const_ArraySet_Ptr	inSet )
{
	InitSelf( inMaxValue );

	if( inSet )
    {
        vuint32 	Count = inSet->get_Count();
        vuint32*	theRecID = inSet->begin();
        
        while( Count-- )
            Append( *theRecID++ );
	}
}


/**********************************************************************************************/
// common ctor.
//
void BitSet::InitSelf( vuint32 inMaxValue )
{
	mCount_1 	= 0;
	mMaxValue 	= inMaxValue;

	if( mMaxValue == 0 )
    {
		mArray 			= nullptr;
		mBytesInArray 	= 0;
	}	
	else // mMaxValue > 0:
    {
        mBytesInArray = (mMaxValue - 1) / 8 + 1;
        
        // Make aligment to WORD. Additional bits don't used.
        // Thanks to aligment we can make faster union,itersection...
        
        if( mBytesInArray )
            mBytesInArray = CalcAlignBytes( mBytesInArray );
        
        mArray = new vuint8 [mBytesInArray];
        
        if( mBytesInArray )
            memset( (char*)mArray, 0, mBytesInArray );
	}

    sTotalSize += mBytesInArray;
}


/**********************************************************************************************/
BitSet::~BitSet( void )
{
    sTotalSize -= mBytesInArray;

	if( mArray )
    {	
		delete [] mArray;
		mArray = nullptr;	
	}
}


#pragma mark-
#pragma mark ====== Set API =======  


/**********************************************************************************************/
vuint32 BitSet::get_Count( void ) const
{ 
	return get_Bits_1_Count(); 
}


/**********************************************************************************************/
// Returns true if all bits are zero.
//
bool BitSet::get_IsEmpty( void ) const 
{
	return (mCount_1 == 0);

/*
	bool res = true; 

	if( mBytesInArray )
	{
		BitSetIterator iter( *this );
        res = !iter.FirstBit_1();	
	}

	return res;
*/
}


/**********************************************************************************************/
SetIterator_Ptr BitSet::get_Iterator( void ) const
{
	return new BitSetIterator( *this );
}


#pragma mark -
#pragma mark ====== Construction Methods ======= 


/**********************************************************************************************/
Set* BitSet::Clone(	bool inCopy ) const  
{ 
	return inCopy 	? new BitSet( *this ) 
    				: new BitSet( get_MaxValue() ); 
}


#pragma mark -
#pragma mark ====== Item Methods ======= 


/**********************************************************************************************/
bool BitSet::Include( vuint32 inItem ) const
{
	bool res = false;
	
	if( 	mCount_1 				// we have at least one bit = 1.
    	&&  inItem 					// and inItem > 0
        &&  inItem <= mMaxValue )	// and inItem fit range of this bitset
	{
		--inItem;	// make inItem zero based.
		res = bool( (mArray[ Loc(inItem) ] & Mask(inItem) ) != 0 );
	}	
	
	return res;
}


/**********************************************************************************************/
void BitSet::Append( vuint32 inItem )
{
	if( inItem && inItem <= mMaxValue )
	{
		--inItem; // make inItem zero based.
        
        vuint32 location = Loc( inItem );
        vuint8  mask 	 = Mask( inItem ); 
        
        vuint8* p = mArray + location;
        
        // If bit which we now set to be 1 was 0, then 
        if( (*p & mask) == 0 )
        	++mCount_1;
        
		*p |= mask;
	}	
}


/**********************************************************************************************/
void BitSet::Remove( vuint32 inItem )
{
	if( inItem && inItem <= mMaxValue )
	{
		--inItem; // make inItem zero based.

        vuint32 location = Loc( inItem );
        vuint8  mask 	 = Mask( inItem ); 
        
        vuint8* p = mArray + location;

        // If bit which we now set to be 1 was 0, then 
        if( (*p & mask) )
        	--mCount_1;
        
		*p &= (vuint8) ~Mask( inItem );
	}	
}


#pragma mark -
#pragma mark ====== Batch Item Methods ======= 


/**********************************************************************************************/
void BitSet::AppendSorted_123( 
	ID_TYPE*	inLeftIncrementedPtr, 
	ID_TYPE* 	inRightFixedPtr )
{
	while( inLeftIncrementedPtr < inRightFixedPtr )
	{
		BitSet::Append( *inLeftIncrementedPtr++ );
	}
}


/**********************************************************************************************/
void BitSet::AppendSorted_321( 
	ID_TYPE*	inLeftFixedPtr, 
	ID_TYPE* 	inRightDecrementedPtr )
{
	while( inLeftFixedPtr < inRightDecrementedPtr )
	{ 
		BitSet::Append( *inRightDecrementedPtr-- );
	}
}


#pragma mark -
#pragma mark ====== Unary operators ======= 


/**********************************************************************************************/
Set_Ptr BitSet::Union( const Set_Ptr inSet )
{
	if( inSet == nullptr )
		return this->Clone();
		
	BitSet* pBitSet = dcast<BitSet*>( inSet.get() ); 

	if( pBitSet )
	{
		return FBL::Union( *this, *pBitSet );
	}
	else
	{
		FBL_CHECK( false ); // FIXME.
		return nullptr;
	}
}


/**********************************************************************************************/
Set_Ptr BitSet::Intersection( const Set_Ptr inSet )
{
	if( inSet == nullptr )
		return nullptr;
		
	BitSet* pBitSet = dcast<BitSet*>( inSet.get() ); 

	if( pBitSet )
	{
		return FBL::Intersection( *this, *pBitSet );
	}
	else
	{
		FBL_CHECK( false ); // FIXME.
		return nullptr;
	}
}


/**********************************************************************************************/
Set_Ptr BitSet::Difference( const Set_Ptr inSet )
{
	if( inSet == nullptr )
		return Clone();

	BitSet* pBitSet = dcast<BitSet*>( inSet.get() ); 

	if( pBitSet )
	{
		return FBL::Difference( *this, *pBitSet );
	}
	else
	{
		FBL_CHECK( false ); // FIXME.
		return nullptr;
	}
}


/**********************************************************************************************/
Set_Ptr BitSet::SymmetricDifference	( const Set_Ptr inSet )
{
	if( inSet == nullptr )
		return Clone();

	BitSet* pBitSet = dcast<BitSet*>( inSet.get() ); 

	if( pBitSet )
	{
		return FBL::SymmetricDifference( *this, *pBitSet );
	}
	else
	{
		FBL_CHECK( false ); // FIXME.
		return nullptr;
	}
}


#pragma mark-
#pragma mark ====== I_Serializable API ======= 


/**********************************************************************************************/
void BitSet::From( 
	I_IStream_Ptr inStream,
	bool		  inBlock )
{
	argused1( inBlock );

	// Destroy existing content first.
	InitSelf( 0 );

	// 1) Bytes in array.
	inStream->get( mBytesInArray );
	if( mBytesInArray > 0 )
	{
		// No alignment is needed - it is already aligned.
		mArray = new vuint8[mBytesInArray];

		// 2) Data of actual len.
		inStream->get( (vuint8*)mArray, mBytesInArray );

		// 3) Maximum bit position.
		inStream->get( mMaxValue );
        
		// RZ 04/09/12. 
        // I have added mCount_1 - counter of 1 bits.
        // For now it not go to stream/packet, so we need calculate it.
		RefreshCount_1();
        
        sTotalSize += mBytesInArray;
	}
}


/**********************************************************************************************/
void BitSet::From( 
	I_PacketRcv* 	inPacket,
	bool		  	inBlock )
{
	argused1( inBlock );

	// Destroy existing content first.
	InitSelf( 0 );

	// 1) Bytes in array.
	mBytesInArray = inPacket->get_ULongParam();
	
	if( mBytesInArray > 0 )
	{
		// No alignment is needed - it is already aligned.
		mArray = new vuint8[mBytesInArray];

		// 2) Data of actual len.
		inPacket->get_BinaryParam( mArray, mBytesInArray );

		// 3) Maximal bit position.
		mMaxValue = inPacket->get_ULongParam();

		// RZ 04/09/12. 
        // I have added mCount_1 - counter of 1 bits.
        // For now it not go to stream/packet, so we need to calculate it.
		RefreshCount_1();

        sTotalSize += mBytesInArray;
	}
}


/**********************************************************************************************/
void BitSet::To( 
	I_OStream_Ptr inStream,
	bool		  inBlock ) const
{
	argused1(inBlock);
	
	// Type of set.
	inStream->put( vuint8(2) );

	if( mArray && mBytesInArray > 0 )
	{
		// 1) Array len (in bytes).
		inStream->put(mBytesInArray);

		// 2) Data of actual len.
		inStream->put((vuint8*)mArray, mBytesInArray);

		// 3) Maximum bit position.
		inStream->put(mMaxValue);
	}
	else
	{
		inStream->put( vuint32(0) );
	}
}


/**********************************************************************************************/
void BitSet::To( 
	I_PacketSnd*	inPacket,
	bool		  	inBlock ) const
{
	argused1(inBlock);
	
	FixParamCount fixParamCount( inPacket );
	
	// Type of set.
	inPacket->put_UCharParam(vuint8(2));

	if( mArray && mBytesInArray > 0 )
	{
		// 1) Array len (in bytes).
		inPacket->put_ULongParam(mBytesInArray);

		// 2) Data of actual len.
		inPacket->put_BinaryParam( mArray, mBytesInArray );

		// 3) Maximum bit position.
		inPacket->put_ULongParam(mMaxValue);
	}
	else
	{
		inPacket->put_ULongParam(0UL);
	}
}


#pragma mark-
#pragma mark ====== BitSet API  ======= 


/**********************************************************************************************/
// Finds in the BitSet a continuous block of inCount bits. 
// 
// EXAMPLE: FindContinuous(5)
//
// 		IF there are several blocks that satisfy this, then returns smallest of them.
//				src:	{ 100111111110011111100011111111111 }	
//				result:	{ 000000000000011111000000000000000 }	
//
// IF there is no such block, but exists few smaller, then returns that small blocks
//		and choose biggest of them.
//				src:	{ 100111011110011101100011110111101 }	
//				result:	{ 100000011110000000000000000000000 }	
//
// In the worse case -- there is no even 2 continuous bits.
//				src:	{ 100101010010010101000010100101001 }	
//				result:	{ 100101010010000000000000000000000 }	
//
// Also may happens that inCount is bigger than the number of 1 bits. In this case error.  
//				src:	{ 100000010000000000000000100100000 }	
//				result:	{ 000000000000000000000000000000000 } + FALSE;
//
/*bool BitSet::FindContinuous(
	vuint32		inCount,
	BitSet&		ioResult ) const
{
	BitSet_Ptr copySet = new BitSet( *this );

	do
	{	
		vuint32 found = copySet->FindContinuous_Core( inCount, ioResult );
		FBL_CHECK( found <= inCount ); 
		
		if( found == 0 ) // we do not have so many bits.
		{
			ioResult.ClearAll();
			return false;
		}
			
		inCount -= found;
		
	}while( inCount );

	return true;
}*/


/**********************************************************************************************/
/*
vuint32 BitSet::FindContinuous_Core(
	vuint32		inCount,
	BitSet&		ioResult ) 
{
		struct group{
			vuint32 start;
			vuint32 count;
		}; 
		
		group SmallerGroup	= { 0, 0 };
		group BiggerGroup 	= { 0, 0xFFFFFFFF }; 

	BitSetIterator it( *this );
	
	vuint32 group_start = it.FirstItem();	
	while( group_start )
	{
		vuint32 group_end;
		
		if( it.NextBit_0() )	
				group_end = *it;
		else	group_end = mMaxValue + 1; 

		vuint32 group_size = group_end - group_start;
		
		if( group_size == inCount ) // we have found exact group!
		{
			ioResult.SetBits( group_start, group_end - 1 );
			return inCount;
		}
		
		if( group_size > inCount ) // we have found group bigger then we need.
		{
			// we have found group which is smaller of other big group. Store it.
			if( group_size < BiggerGroup.count ) 
			{
				BiggerGroup.start = group_start;
				BiggerGroup.count = group_size;
			}	
		}
		else
		{
			if( group_size > SmallerGroup.count ) 
			{
				SmallerGroup.start = group_start;
				SmallerGroup.count = group_size;
			}	
		}
		
		group_start = it.NextItem();
	}
	
	// IF exists bigger group simply use it:
	if( BiggerGroup.start != 0 )
	{
		ioResult.SetBits( BiggerGroup.start, BiggerGroup.start + inCount - 1 );
		return inCount;
	}
	
	// we have only smaller group. We already know a biggest one.	
	// so we add it to result, and now we must search for rest number of bits:
	if( SmallerGroup.start != 0 ) 
	{
		ioResult.SetBits( SmallerGroup.start, SmallerGroup.start + SmallerGroup.count - 1 );
		this->ClearBits ( SmallerGroup.start, SmallerGroup.start + SmallerGroup.count - 1 );
	}
	
	return SmallerGroup.count;
}*/


/**********************************************************************************************/
void BitSet::SetAll( void )
{ 
	memset( mArray, 0xFF, mBytesInArray );

    mCount_1 = mMaxValue; 
}


/**********************************************************************************************/
void BitSet::ClearAll( void )
{ 
	memset( mArray, 0x00, mBytesInArray); 

    mCount_1 = 0; 
}


/**********************************************************************************************/
void BitSet::put_MaxValue( ID_TYPE inMaxValue )
{
	// RZ 04/09/12 
    // TODO:  may be here must be ERROR if inMAxValue is bigger ???

	FBL_CHECK( inMaxValue <= mMaxValue );
    
	mMaxValue = inMaxValue;
}


/**********************************************************************************************/
// Returns in range 0..(N-1) 
//
void BitSet::get_NonZeroBounds( vuint32& outFirst, vuint32& outLast ) const
{
	int* beg = (int*) mArray;
	int* end = (int*) (mArray + mBytesInArray);

	int* first 	= beg;
	int* last	= end - 1;

	while( first < end && *first == 0 )
		++first;
		
	while( last > first && *last == 0 )
		--last;

	outFirst 	= (vuint32) (first - (int*) mArray);
	outLast		= (vuint32) (last  - (int*) mArray);
	
}


/**********************************************************************************************/
void BitSet::operator = ( const BitSet& inSet )
{
	vuint32 MinBytes = (mBytesInArray < inSet.mBytesInArray) ? mBytesInArray : inSet.mBytesInArray;
	
	memcpy( mArray, inSet.mArray, MinBytes );		

	// We need re-count 1 bits:
	RefreshCount_1();
}


/**********************************************************************************************/
void BitSet::operator = ( const char* inBitString )
{
	const char* p = inBitString;
	vuint32 value = 1;

	while( *p )
	{
		if( *p != '0' )
			this->Append( value );
			
		++value;
		++p;
	}
}


/**********************************************************************************************/
// Set all bits in mArray by 0 in range [inLeft .. inRight]
//
void BitSet::ClearBits( ID_TYPE inLeft, ID_TYPE inRight )
{
	// values must be in range 1..
	FBL_CHECK( inLeft > 0 && inRight > 0 ); 

	if( inLeft > inRight )	// empty range
		return;
		
	if( inLeft > mMaxValue )	// empty range
		return;
		
	if( inRight > mMaxValue )
		inRight = mMaxValue;	

	// we transform them to zero base.
	if( inLeft )  --inLeft;
	if( inRight ) --inRight;
	
	vuint8* p1 = mArray + Loc(inLeft);
	vuint8* p2 = mArray + Loc(inRight);

	vuint8 mask1 = (vuint8) (inLeft  & 0x07);
	vuint8 mask2 = (vuint8) (inRight & 0x07);

	// set bits in the first byte.
	if( p1 == p2 )
	{
		while( mask1 <= mask2 )
			*p1 &= (vuint8) ~(sMasks[mask1++]);		//  better bits 
	
        return;	
	}
    else
    {
		while( mask1 <= 7 )
			*p1 &= (vuint8) ~(sMasks[mask1++]);		//  better bits 
	}
	
	p1++;

	// set bytes between p1 and p2. 	
	memset( p1, 0x00, vuint32(p2 - p1) );	

	// set bits of last byte p2
	mask1 = 0;
	while( mask1 <= mask2 )	
		*p2 &= (vuint8) ~(sMasks[mask1++]);			//  better bits 
        
        
    // RZ 04/09/12 
    // FAST and STUPID solution -- re-count all. 
    // It seems this method is not used so far ...
    RefreshCount_1();
}


#pragma mark -
#pragma mark ====== Unary Operators for BitSet ======= 


/**********************************************************************************************/
// Invert all bits.
//
bool BitSet::IsEqual( const BitSet& inSet ) const
{
	// FAST CHECK: may be MaxValueCount is different?
	if( mMaxValue != inSet.mMaxValue )
		return false; 

	// FAST CHECK: may be 1 bits count is different?
	if( mCount_1 != inSet.mCount_1 )
		return false; 

	// Okay, and now we need still compare byte to byte
	int* p1 = (int*) mArray;
	int* e1 = (int*) (mArray + mBytesInArray);
	
	int* p2 = (int*) inSet.mArray;
	int* e2 = (int*) (inSet.mArray + inSet.mBytesInArray);
	argused1(e2);
	
	while( p1 < e1 )
	{
		if( *p1++ != *p2++ )
			return false;
	}
	
	return true;
}


/**********************************************************************************************/
// Invert all bits.
//
void BitSet::Invert( void )
{
	int* p   = (int*) mArray;
	int* end = (int*) (mArray + mBytesInArray);
	
	while( p < end )
	{
		*p = ~(*p);	
		++p;
	}
    
    // And now we need to correct mCount_1
    // All 1 have become 0, and reverse.
    // So counting is simple:
	mCount_1 = mMaxValue - mCount_1;
}


/**********************************************************************************************/
// Make union (OR) of this vector with inSet.
//
void BitSet::Union( const BitSet& inSet )
{
	int diff = int(mBytesInArray - inSet.mBytesInArray);
	vuint32 MinBytes = diff < 0 ? mBytesInArray : inSet.mBytesInArray;
	
	int* p1  = (int*) mArray;
	int* end = (int*) (mArray + MinBytes);

	int* p2  = (int*) inSet.mArray;
	
	while( p1 < end )
		*p1++ |= *p2++;	
	
	// IS: 11.06.2007
	// http://valentina-db.com/bt/view.php?id=2382
	// Do nothing.

   	RefreshCount_1();
}


/**********************************************************************************************/
// Make intersection (AND) of this vector with inSet.
//
void BitSet::Intersection( const BitSet& inSet )
{
	int diff = int(mBytesInArray - inSet.mBytesInArray);
	vuint32 MinBytes = diff < 0 ? mBytesInArray : inSet.mBytesInArray;
	
	int* p1  	= (int*) mArray;
	int* end 	= (int*) (mArray + MinBytes);

	int* p2 	= (int*) inSet.mArray;
	
	while( p1 < end )
		*p1++ &= *p2++;	

	// IS: 11.06.2007
	// http://valentina-db.com/bt/view.php?id=2382
	// If this BitSet is longer than inSet then we need to zero the "tail".
	//
	if( diff > 0 )
		memset( end, 0, (vuint32) diff );

   	RefreshCount_1();
}


/**********************************************************************************************/
// Build difference of this set with inSet.
//
void BitSet::Difference( const BitSet& inSet )
{
	int diff = int(mBytesInArray - inSet.mBytesInArray);
	vuint32 MinBytes = diff < 0 ? mBytesInArray : inSet.mBytesInArray;
	
	int* p1  	= (int*) mArray;
	int* end 	= (int*) (mArray + MinBytes);

	int* p2 	= (int*) inSet.mArray;
	
	while( p1 < end )
		*p1++ &= ~(*p2++);	
	
	// IS: 11.06.2007
	// http://valentina-db.com/bt/view.php?id=2382
	// If this BitSet is longer than inSet then we need to zero the "tail".
	// Do nothing.
       
   	RefreshCount_1();
}


/**********************************************************************************************/
// Build symmetric difference of this set with inSet.
//
void BitSet::SymmetricDifference( const BitSet& inSet )
{
	int diff = int(mBytesInArray - inSet.mBytesInArray);
	vuint32 MinBytes = diff < 0 ? mBytesInArray : inSet.mBytesInArray;
	
	int* p1  	= (int*) mArray;
	int* end 	= (int*) (mArray + MinBytes);

	int* p2 	= (int*) inSet.mArray;
	
	while( p1 < end )
		*p1++ ^= *p2++;	
	
	// IS: 11.06.2007
	// http://valentina-db.com/bt/view.php?id=2382
	// If this BitSet is longer than inSet then we need to modify the "tail".
	//	
	if( diff > 0 )
	{
		int zero = 0;
		
        int* currentPos	= (int*) end;
		int* tailEnd  	= (int*) (end + diff);
        
		while( currentPos < tailEnd )
			*currentPos++ ^= zero;	
	}

   	RefreshCount_1();
}


#pragma mark -
#pragma mark ====== Binary Operations for BitSet ======= 


/**********************************************************************************************/
// Union of two vectors.
// Create and returns new bit vector.
// You must to destroy it. 
//
BitSet_Ptr Union( 
		const BitSet& inSet1,
		const BitSet& inSet2 )
{
	// RZ: For binary OPERATION() we take the MAX of sizes of the given BitSets,
	// while for unary we did take MIN().
	vuint32 mMaxValue = FBL::Max( inSet1.mMaxValue, inSet2.mMaxValue );
	
	BitSet_Ptr theResult = new BitSet( mMaxValue );
	
	vuint32 first1, last1;
	inSet1.get_NonZeroBounds( first1, last1 );
		
	vuint32 first2, last2;
	inSet2.get_NonZeroBounds( first2, last2 );
	
	vuint32 startWord	= first1 < first2 ? first1 : first2;
	vuint32 finishWord	= last1 > last2 ? last1 : last2;
		
	int* p1 	= ((int*) inSet1.mArray) + startWord;
	int* end 	= ((int*) inSet1.mArray) + finishWord;

	int* p2 	= ((int*) inSet2.mArray) + startWord;
	int* res 	= ((int*) theResult->mArray) + startWord;

	while( p1 <= end )
		*res++ = (*p1++) | (*p2++);	
	
    theResult->RefreshCount_1();
    
	return theResult;
}


/**********************************************************************************************/
// Intersection of two vectors.
// Create and returns new bit vector. You must destroy it. 
//
BitSet_Ptr Intersection( 
		const BitSet& inSet1,
		const BitSet& inSet2 )
{
	// RZ: For binary OPERATION() we take the MAX of sizes of the given BitSets,
	// while for unary we did take MIN().
	vuint32 mMaxValue = FBL::Max(inSet1.mMaxValue, inSet2.mMaxValue);
	
	BitSet_Ptr theResult = new BitSet( mMaxValue );
	
	vuint32 first1, last1;
	inSet1.get_NonZeroBounds( first1, last1 );
		
	vuint32 first2, last2;
	inSet2.get_NonZeroBounds( first2, last2 );
	
	vuint32 startWord	= (first1 > first2) ? first1 : first2;
	vuint32 finishWord	= ( last1 <  last2) ?  last1 :  last2;
		
	int* p1 	= ((int*) inSet1.mArray) + startWord;
	int* end 	= ((int*) inSet1.mArray) + finishWord;

	int* p2 	= ((int*) inSet2.mArray) + startWord;
	int* res 	= ((int*) theResult->mArray) + startWord;

	while( p1 <= end )
		*res++ = (*p1++) & (*p2++);	
	
	// Way for optimisation - 
	// scan backward and search the first intersected byte.
	// allocate a new bitset with new buffer and scan other data

    theResult->RefreshCount_1();

	return theResult;
}


/**********************************************************************************************/
// Difference of two vectors.
// Create and returns new bit vector. You must destroy it. 
//
BitSet_Ptr Difference( 
		const BitSet& inSet1,
		const BitSet& inSet2 )
{
	if( inSet1.mMaxValue == 0 || inSet2.mMaxValue == 0 )
	{
		return nullptr;
	}

	// RZ: For binary OPERATION() we take the MAX of sizes of the given BitSets,
	// while for unary we did take MIN().
	vuint32 mMaxValue = FBL::Max(inSet1.mMaxValue, inSet2.mMaxValue);
	
	BitSet_Ptr theResult = new BitSet( mMaxValue );
	
	vuint32 first1, last1;
	inSet1.get_NonZeroBounds( first1, last1 );
		
	int* p1 	= ((int*) inSet1.mArray) + first1;
	int* end 	= ((int*) inSet1.mArray) + last1;

	int* p2 	= ((int*) inSet2.mArray) + first1;
	int* res 	= ((int*) theResult->mArray) + first1;
	
	while( p1 <= end )
		*res++ = (*p1++) & ~(*p2++);	
	
    theResult->RefreshCount_1();

	return theResult;
}


/**********************************************************************************************/
// Difference of two vectors.
// Create and returns new bit vector. You must destroy it. 
//
BitSet_Ptr SymmetricDifference( 
		const BitSet& inSet1,
		const BitSet& inSet2 )
{
	if( inSet1.mMaxValue == 0 && inSet2.mMaxValue == 0 )
	{
		return nullptr;
	}

	if( inSet1.mMaxValue == 0 )
	{
		return new BitSet( inSet2 );
	}

	if( inSet2.mMaxValue == 0 )
	{
		return new BitSet( inSet1 );
	}

	// RZ: For binary OPERATION() we take the MAX of sizes of the given BitSets,
	// while for unary we did take MIN().
	vuint32 mMaxValue = FBL::Max(inSet1.mMaxValue, inSet2.mMaxValue);
	
	BitSet_Ptr theResult = new BitSet( mMaxValue );
	
	vuint32 first1, last1;
	inSet1.get_NonZeroBounds( first1, last1 );
		
	int* p1 	= ((int*) inSet1.mArray) + first1;
	int* end 	= ((int*) inSet1.mArray) + last1;

	int* p2 	= ((int*) inSet2.mArray) + first1;
	int* res 	= ((int*) theResult->mArray) + first1;
	
	while( p1 <= end )
		*res++ = (*p1++) ^ (*p2++);	
	
    theResult->RefreshCount_1();

	return theResult;	
}



#pragma mark -
#pragma mark ====== Bit Methods: ======= 
 

/**********************************************************************************************/
// Find Nth non-zero bit and returns the corresponded Value.
//
ID_TYPE BitSet::FindNth_1_Bit( vuint32 inNth ) const
{
	FBL_CHECK( inNth > 0 ); 

	// at first we must find byte which contains Nth non zero bit.
	// for this we will count bits in each byte.
	vuint32 Count = 0;
	
	vuint8* p 	= mArray;
	vuint8* end = p + mBytesInArray;
	
	// for all bytes including the last we count bits using look up table.
	while( p < end )
	{
		Count += sBitsCount[ *p ]; 
		
		if( inNth <= Count )
			break;

		p++;
	}
	
	if( p == end )
		return 0;

	vuint32 prevCount = Count - sBitsCount[ *p ];
	FBL_CHECK( inNth > prevCount ); 
	
	//vuint8 BitInByte = vuint8(inNth - prevCount);
	//FBL_CHECK( 1 <= BitInByte && BitInByte <= 8 ); 
	//vuint32 Value = (vuint32(p - mArray) << 3) + 1;  // x << 3 is x * 8
	//
	//// search bits in this byte.
	//while( BitInByte )
	//{
	//	if( Include(Value) )
	//			--BitInByte;
	//	else 	++Value;			
	//}	

	vuint8 BitInByte = vuint8(inNth - prevCount);
	FBL_CHECK( 1 <= BitInByte && BitInByte <= 8 ); 
	vuint32 Value = (vuint32(p - mArray) << 3);  // x << 3 is x * 8
	
	char c = (char)*p;
	while( BitInByte )
	{
		++Value;
		if( c & 1 )
		{
			--BitInByte;
		}
		c = char(c >> 1);
	}	
	
	return Value > mMaxValue ? 0 : Value;
}


/**********************************************************************************************/
void BitSet::RefreshCount_1( void )
{
	mCount_1 = CountBits_1();
}


/**********************************************************************************************/
// Count in bit vector number of bits 1.
// In other words this is like FBL_ArraySet::get_Count(); 
//
vuint32 BitSet::CountBits_1( void ) const
{
	vuint32 Count = 0;
		
	// Do a quick scan of our bit array counting the number of set bits.  We can do this using
	// UInt32s for additional speed.  Our array is already aligned to 32 bits so this will
	// work safely.  Using an iterator is inefficient as it extracts more information than
	// we need.
	FBL_CHECK( sizeof(vuint32) == 4 );
	
	if( mArray )
	{
		vuint32 x;

#if 0
		// RZ 12/12/08 attempt.
		vuint32 first_word_with_1;
		vuint32 last_word_with_1; 
        
		get_NonZeroBounds( first_word_with_1, last_word_with_1 );
		
		vuint8* pStartByte = ((vuint8*) (mArray + first_word_with_1) );
		vuint8* pEndByte   = ((vuint8*) &last_word_with_1) + sizeof(vuint32);
	
		vuint8* ptr = pStartByte;
		vuint32 bytesLeft = vuint32(pEndByte - pStartByte);
#else
		vuint8 *ptr = mArray;
		vuint32 bytesLeft = mBytesInArray;
#endif		
		
		while( bytesLeft )
		{
			// OK, get the current set of 4 bytes
			x = * (vuint32*) ptr;
			
			// Use a cunning algorithm to count the bits
		    x = (x & 0x55555555) + ((x>>1) & 0x55555555);
		    x = (x & 0x33333333) + ((x>>2) & 0x33333333);
		    x += (x>>4);
		    x &= 0x0F0F0F0F;
		    x += (x>>8);
		    x += (x>>16);
		    x &= 0x000000FF;
			
			// x is the count of set bits
			Count += x;
			
			// Move to the next entry
			ptr += sizeof(vuint32);
			bytesLeft -= sizeof(vuint32);
		}
	}

	if( Count > mMaxValue )
		Count = mMaxValue;

	return Count;
}



/**********************************************************************************************/
// Set all bits in mArray by 1 in range [inLeft .. inRight]
//
void BitSet::SetBits( ID_TYPE inLeft, ID_TYPE inRight )
{
	// Added by IN, to fix build on Linux DebugMT_64 target
#if FBL_TEST_CODE	
	
	// values must be in range 1..
	FBL_CHECK_EX( inLeft > 0 && inRight > 0, gBitSetSetBits_Assert );
	
#endif // FBL_TEST_CODE

	if( inLeft > inRight )	// empty range
		return;
		
	if( inLeft > mMaxValue )	// empty range
		return;
		
	if( inRight > mMaxValue )
		inRight = mMaxValue;	

	// we transform them to zero base.
	if( inLeft )  inLeft--;
	if( inRight ) inRight--;
	
	vuint8* p1 = mArray + Loc(inLeft);
	vuint8* p2 = mArray + Loc(inRight);

	vuint8 mask1 = (vuint8) (inLeft  & 0x07);
	vuint8 mask2 = (vuint8) (inRight & 0x07);

	// set bits in the first byte.
	if( p1 == p2 )
	{
		while( mask1 <= mask2 )
        {
        	if( (*p1 & sMasks[mask1]) == 0 )
            	++mCount_1;
        
			*p1 |= sMasks[mask1++];		//  better bits 
        }
	
        return;	
	}
    else
    {
        if( (*p1 & sMasks[mask1]) == 0 )
            ++mCount_1;

		while( mask1 <= 7 )
			*p1 |= sMasks[mask1++];		//  better bits 
	}
	
	p1++;

	// set bytes between p1 and p2. 	
    vuint32 Bytes = vuint32(p2 - p1);
	memset( p1, 0xFF, Bytes );	
    mCount_1 += Bytes * 8;

	// set bits of last byte p2
	mask1 = 0;
	while( mask1 <= mask2 )
    {
        if( (*p2 & sMasks[mask1]) == 0 )
            ++mCount_1;

		*p2 |= sMasks[mask1++];			//  better bits 	     
    }
}


#pragma mark -
#pragma mark ===== CLASS BitSet_01 ======


/**********************************************************************************************/
BitSet_01::BitSet_01(
	bool	inType,
	ID_TYPE inMaxValue )
:
	mType( inType )
{ 
	InitSelf( inMaxValue ); 
}


/**********************************************************************************************/
BitSet_01::BitSet_01( 
	bool			inType,
	const BitSet&	inSrc )
:
	mType( inType )
{
	(*this) = inSrc;
}


/**********************************************************************************************/
// Copy ctor
//
BitSet_01::BitSet_01( const BitSet& inSrc )
{
	argused1( inSrc );
	THROW_ERROR_INTERNAL_1( ERR_INTERNAL_FUNC_MUST_NOT_BE_CALLED, "BitSet_01 copy constructor" ); 
}


/**********************************************************************************************/
SetIterator_Ptr	BitSet_01::get_Iterator( void ) const
{
	return new BitSetIterator_01( *this  );
}


#pragma mark -


/**********************************************************************************************/
void BitSet_01::operator= ( const BitSet& inSet )
{
	ID_TYPE first;
	ID_TYPE last;
	inSet.get_NonZeroBounds( first, last );

	ID_TYPE maxValue = 0;

	// We assign only if inSet is filled only 1 or 0 numbers.
	if( ( mType && first == 1 && last == inSet.get_MaxValue() ) || ( !mType && first == 0 && last == 0 ) )
	{
		if( !mType || ( mType && inSet.get_Iterator()->FirstItem() ) )
		{
			maxValue = last;
		}
	}

	InitSelf( maxValue );
}


/**********************************************************************************************/
void BitSet_01::operator= ( const char* inBitString )
{ 
	argused1( inBitString );
	THROW_ERROR_INTERNAL_1( ERR_INTERNAL_FUNC_MUST_NOT_BE_CALLED, "BitSet_01::operator=" ); 
}


#pragma mark -


/**********************************************************************************************/
bool BitSet_01::IsEqual( const BitSet& inSet ) const
{
	bool result;
    
	if( inSet.get_MaxValue() == mMaxValue )
	{
		vuint32 count_1 = inSet.get_Bits_1_Count();
        
		result = (  mType && count_1 == mMaxValue ) || 
		  		 ( !mType && count_1 == 0 );
	}
	else
	{
		result = false;
	}

	return result;
}


/**********************************************************************************************/
void BitSet_01::Invert( void )
{ 
	THROW_ERROR_INTERNAL_1( ERR_INTERNAL_FUNC_MUST_NOT_BE_CALLED, "BitSet_01::Invert" ); 
}


/**********************************************************************************************/
void BitSet_01::Union( const BitSet& inSet )
{ 
	argused1( inSet );
	THROW_ERROR_INTERNAL_1( ERR_INTERNAL_FUNC_MUST_NOT_BE_CALLED, "BitSet_01::Union" ); 
}


/**********************************************************************************************/
void BitSet_01::Intersection( const BitSet& inSet )
{ 
	argused1( inSet );
	THROW_ERROR_INTERNAL_1( ERR_INTERNAL_FUNC_MUST_NOT_BE_CALLED, "BitSet_01::Intersection" ); 
}


/**********************************************************************************************/
void BitSet_01::Difference ( const BitSet& inSet )
{ 
	argused1( inSet );
	THROW_ERROR_INTERNAL_1( ERR_INTERNAL_FUNC_MUST_NOT_BE_CALLED, "BitSet_01::Difference" ); 
}


/**********************************************************************************************/
void BitSet_01::SymmetricDifference( const BitSet& inSet )
{ 
	argused1( inSet );
	THROW_ERROR_INTERNAL_1( ERR_INTERNAL_FUNC_MUST_NOT_BE_CALLED, "BitSet_01::SymmetricDifference" ); 
}


#pragma mark -


/**********************************************************************************************/
bool BitSet_01::Include( ID_TYPE inItem ) const
{
	return mType && inItem > 0 && inItem <= mMaxValue;
}


/**********************************************************************************************/
void BitSet_01::Append( ID_TYPE inItem )
{ 
	argused1( inItem );
	THROW_ERROR_INTERNAL_1( ERR_INTERNAL_FUNC_MUST_NOT_BE_CALLED, "BitSet_01::Append" ); 
}


/**********************************************************************************************/
void BitSet_01::Remove( ID_TYPE inItem )
{ 
	argused1( inItem );
	THROW_ERROR_INTERNAL_1( ERR_INTERNAL_FUNC_MUST_NOT_BE_CALLED, "BitSet_01::Remove" ); 
}
	

/**********************************************************************************************/
bool BitSet_01::get_IsEmpty( void ) const 
{ 
	return ( mType && mMaxValue > 0 );
}


#pragma mark -


/**********************************************************************************************/
ID_TYPE BitSet_01::FindNth_1_Bit( vuint32 inNth ) const
{
	return mType ? ( inNth <= mMaxValue ? (ID_TYPE)inNth : 0 )
				 : (ID_TYPE) 0;
}


/**********************************************************************************************/
FBL_SHARED_EXP void BitSet_01::get_NonZeroBounds( 
	ID_TYPE& 	outFirst, 
	ID_TYPE& 	outLast ) const
{
	if( mType )
	{
		outFirst = 1;
		outLast = mMaxValue;
	}
	else
	{
		outFirst = outLast = 0;
	}
}


/**********************************************************************************************/
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning( disable : 4702 )
#endif
//
vuint8* BitSet_01::get_ArrayPtr( void ) const 
{ 
	THROW_ERROR_INTERNAL_1( ERR_INTERNAL_FUNC_MUST_NOT_BE_CALLED, "BitSet_01::get_ArrayPtr" ); 
	return nullptr;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif


/**********************************************************************************************/
vuint32 BitSet_01::CountBits_1( void ) const
{ 
	return mType ? mMaxValue : (ID_TYPE)0;
}


/**********************************************************************************************/
void BitSet_01::SetAll( void )
{ 
	THROW_ERROR_INTERNAL_1( ERR_INTERNAL_FUNC_MUST_NOT_BE_CALLED, "BitSet_01::SetAll" ); 
}


/**********************************************************************************************/
void BitSet_01::ClearAll( void )
{ 
	THROW_ERROR_INTERNAL_1( ERR_INTERNAL_FUNC_MUST_NOT_BE_CALLED, "BitSet_01::ClearAll" ); 
}


/**********************************************************************************************/
void BitSet_01::SetBits( 
	ID_TYPE inLeft, 
	ID_TYPE inRight)
{ 
	argused2( inLeft, inRight );
	THROW_ERROR_INTERNAL_1( ERR_INTERNAL_FUNC_MUST_NOT_BE_CALLED, "BitSet_01::SetBits" ); 
}


/**********************************************************************************************/
void BitSet_01::ClearBits(
	ID_TYPE inLeft, 
	ID_TYPE inRight)
{
	argused2( inLeft, inRight );
	THROW_ERROR_INTERNAL_1( ERR_INTERNAL_FUNC_MUST_NOT_BE_CALLED, "BitSet_01::ClearBits" ); 
}


/**********************************************************************************************/
/*
void BitSet_01::SetAllBytes( 
	vuint8	inFillValue, 
	ID_TYPE inStartByte)
{
	argused2( inFillValue, inStartByte );
	THROW_ERROR_INTERNAL_1( ERR_INTERNAL_FUNC_MUST_NOT_BE_CALLED, "BitSet_01::SetAllBytes" ); 
}*/


#pragma mark -


/**********************************************************************************************/
void BitSet_01::InitSelf( ID_TYPE inMaxValue )
{
	mMaxValue 		= inMaxValue;
	mArray 			= nullptr;
    mCount_1		= mType ? inMaxValue : 0;

	mBytesInArray 	= (mMaxValue - 1) / 8 + 1;
    
	// Make aligment to WORD. Additional bits are not used.
	// Thanks to aligment we can make faster union,itersection...
	mBytesInArray = CalcAlignBytes( mBytesInArray );
    
    sTotalSize += mBytesInArray;
}


/**********************************************************************************************/
FBL_End_Namespace
