/**********************************************************************************************/
/* FBL_BitSetIterator.cpp																	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                       					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Sets/FBL_BitSetIterator.h>
#include <VShared/FBL/publ/Sets/FBL_BitSet.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
BitSetIterator::BitSetIterator( const BitSet& inBitSet ) :
	mBitSet( &inBitSet )
{
	pCurrentByte 		= mBitSet->mArray;
	currBit 			= 0;
	mCurrentPosition	= 1;

	pEndByte = pCurrentByte + mBitSet->mBytesInArray - 1;
}


/**********************************************************************************************/
BitSetIterator::~BitSetIterator( void )
{
}

#pragma mark -


/**********************************************************************************************/
bool BitSetIterator::GoToBit( vuint32 inIndex )
{
	FBL_CHECK( inIndex > 0 ); 	

	if( inIndex > mBitSet->get_MaxValue() )
		return false;

	mCurrentPosition = inIndex;
	
	pCurrentByte = mBitSet->mArray + mBitSet->Loc( inIndex - 1 );
	currBit = vuint8( (inIndex - 1) % 8 );
	
	return true;	
}


#pragma mark -
 

/**********************************************************************************************/
bool BitSetIterator::NextBit( void )
{
	FBL_CHECK( 1 <= mCurrentPosition && mCurrentPosition <= mBitSet->mMaxValue ); 

	// Work out whether we are at the real end of our array
	if( mCurrentPosition >= mBitSet->mMaxValue )
		return false;
	
	// We are going to move on one index whatever
	++mCurrentPosition;

	// at first try find next bit in current byte
	if( currBit == BITS_IN_CHAR - 1 )
	{
		// now try to find next byte
		if( pCurrentByte < pEndByte )
		{
			++pCurrentByte;
			currBit = 0;			
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		++currBit;
		return true;
	}
}


/**********************************************************************************************/
// Move iterator to the prev bit. Returns true if such bit found.
//
bool BitSetIterator::PrevBit( void )
{
	FBL_CHECK( 1 <= mCurrentPosition && mCurrentPosition <= mBitSet->mMaxValue ); 

	if( mCurrentPosition <= 1 )
		return false;
	
	// We are going to move on one index whatever
	--mCurrentPosition;

	// at first try to find the prev bit in the current byte
	if( currBit == 0 )
	{
		// now try to find prev byte
		if( mBitSet->mArray < pCurrentByte )
		{
			--pCurrentByte;
			currBit = BITS_IN_CHAR - 1;			
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		--currBit;
		return true;
	}
}


#pragma mark -
 

/**********************************************************************************************/
bool BitSetIterator::FirstBit_1( void )
{
	if( mBitSet->mBytesInArray == 0 ) // No bits in the array
		return false;
	
	// Move to the beginning of the array
	FirstBit();
	
	// Return the next set bit
	if( (*pCurrentByte) & 0x01 )
	{
		// First bit is set
		return true;
	}
	else
	{
		// Return the next bit set
		return NextBit_1();
	}
}


/**********************************************************************************************/
bool BitSetIterator::FirstBit_0( void )
{
	if( mBitSet->mBytesInArray == 0 ) // No bits in the array
		return false;
	
	// Move to the beginning of the array
	FirstBit();
	
	// Return the next zero bit
	if( !((*pCurrentByte) & 0x01) )
	{
		// First bit is zero
		return true;
	}
	else
	{
		// Return the next zero bit
		return NextBit_0();
	}
}


#pragma mark -
 

/**********************************************************************************************/
bool BitSetIterator::LastBit_1( void )
{
	if( mBitSet->mBytesInArray == 0 ) // No bits in the array
		return false;
	
	// Move to the end of the array
	LastBit();
	
	// Return the next set bit
	if( (*pCurrentByte) & (0x01 << currBit) )
	{
		// Last bit is set
		return true;
	}
	else
	{
		// Return the next bit set
		return PrevBit_1();
	}
}


/**********************************************************************************************/
bool BitSetIterator::LastBit_0( void )
{
	if( mBitSet->mBytesInArray == 0 ) // No bits in the array
		return false;
	
	// Move to the end of the array
	LastBit();
	
	// Return the next set bit
	if( !((*pCurrentByte) & (0x01 << currBit)) )
	{
		// Last bit is set
		return true;
	}
	else
	{
		// Return the next bit set
		return PrevBit_0();
	}
}


#pragma mark -
 

/**********************************************************************************************/
// Move iterator to the next nonzero bit. Returns true if such bit found.
//
bool BitSetIterator::NextBit_1( void )
{
	FBL_CHECK( mBitSet->mBytesInArray > 0 ); 

	// We need to move to the next bit
	if( NextBit() )
	{	
		// Get the current word and adjust for the current bit
		vuint8 currentByte = *pCurrentByte;
		currentByte >>= currBit;
		
		// TODO optimisation like this (must be tested)
/*		
		if( currentByte == 0x00  ) 		
		{
			while( ++pCurrentByte < pEndByte && *pCurrentByte == 0x00 )
				;
		
			if( pCurrentByte < pEndByte ) // we have found next not zero byte
			{
				currentByte = *pCurrentByte;
				currBit = 0;
				mCurrentPosition = (pCurrentByte - mBitSet->mArray) * 8 + 1;	
			} 	
			else // no more 1 bits
			{
				mCurrentPosition = mBitSet->mMaxValue;
				currBit = vuint8((mCurrentPosition-1) % 8);
				return false;
			}
		}
*/
		
		// Coarse search forward until we find a byte that has at least one set bit
		while( currentByte == 0x00 )
		{
			// Work out whether we are at the real end of our array
			if( mCurrentPosition >= mBitSet->mMaxValue )
				return false;
			
			// Move to the next byte
			currentByte = *(++pCurrentByte);
			mCurrentPosition += vuint32( BITS_IN_CHAR - currBit );
			currBit = 0;
		}
		
		// OK, we must have a byte here that has at least one set bit.  
		// The question is, which bit is it?
		while( currBit < BITS_IN_CHAR )
		{
			if( (currentByte & 0x01) )
			{
				break;// Found our 1 bit
			}
			else
			{
				// Work out whether we are at the real end of our array
				if( mCurrentPosition >= mBitSet->mMaxValue )
					return false;

				// Move to the next bit
				currBit++;
				mCurrentPosition++;
				currentByte >>= 1;		
			}
		}
		
		return true;
	}
	else
	{
		return false;	// No next bit
	}
}


/**********************************************************************************************/
bool BitSetIterator::NextBit_0( void )
{
	FBL_CHECK( mBitSet->mBytesInArray > 0 ); 
	
	// We need to move to the next bit
	if( NextBit() )
	{	
		// Get the current word and adjust for the current bit
		vuint8 currentByte = *pCurrentByte;
		currentByte >>= currBit;
		
		// Coarse search forward until we find a byte that has at least one zero bit
		vuint8 value = vuint8(0xFF >> currBit);
		while( currentByte == value )
		{
			// Work out whether we are at the real end of our array
			if( mCurrentPosition >= mBitSet->mMaxValue )
				return false;
			
			// Move to the next byte
			currentByte = *(++pCurrentByte);
			mCurrentPosition += vuint32( BITS_IN_CHAR - currBit );
			currBit = 0;
		}
		
		// OK, we must have a byte here that has at least one set bit.  
		// The question is, which bit is it?
		while( currBit < BITS_IN_CHAR )
		{
			if( !(currentByte & 0x01) )
			{
				break;// Found our 0 bit
			}
			else
			{
				// Work out whether we are at the real end of our array
				if( mCurrentPosition >= mBitSet->mMaxValue )
					return false;

				// Move to the next bit
				currBit++;
				mCurrentPosition++;
				currentByte >>= 1;		
			}
		}
		
		return true;
	}
	else
	{
		return false;	// No next bit
	}
}


#pragma mark -
 

/**********************************************************************************************/
bool BitSetIterator::PrevBit_1( void )
{
	FBL_CHECK( mBitSet->mBytesInArray > 0 ); 
	
	// We need to move to the prev bit
	if( PrevBit() )
	{	
		// Get the current byte and adjust for the current bit
		vuint8 currentByte = *pCurrentByte;
		currentByte <<= vuint8( BITS_IN_CHAR - (currBit + 1) );
		
		// Coarse search forward until we find a byte that has at least one set bit
		while( currentByte == 0x00 )
		{
			// Work out whether we are at the real beginning of our array
			if( mCurrentPosition <= 1 )
				return false;
			
			// Move to the prev byte
			currentByte = *(--pCurrentByte);
			mCurrentPosition -= vuint32(currBit + 1);
			currBit = BITS_IN_CHAR - 1;
		}
		
		// OK, we must have a byte here that has at least one set bit.  
		// The question is, which bit is it?
		while( currBit != vuint8(-1) ) 
		{
			if( (currentByte & 0x80) )
			{
				break;// Found our 1 bit
			}
			else
			{
				// Work out whether we are at the real beginning of our array
				if( mCurrentPosition <= 1 )
					return false;

				// Move to the next bit
				currBit--;
				mCurrentPosition--;
				currentByte <<= 1;		
			}
		}
		
		return true;
	}
	else
	{
		return false;	// No prev bit
	}
}


/**********************************************************************************************/
bool BitSetIterator::PrevBit_0( void )
{
	FBL_CHECK( mBitSet->mBytesInArray > 0 ); 
	
	// We need to move to the prev bit
	if( PrevBit() )
	{	
		// Get the current byte and adjust for the current bit
		vuint8 currentByte = *pCurrentByte;
		currentByte <<= vuint8( BITS_IN_CHAR - (currBit + 1) );
		
		// Coarse search forward until we find a byte that has at least one set bit
		while( currentByte == 0xFF )
		{
			// Work out whether we are at the real beginning of our array
			if( mCurrentPosition <= 1 )
				return false;
			
			// Move to the prev byte
			currentByte = *(--pCurrentByte);
			mCurrentPosition -= vuint32( currBit + 1 );
			currBit = BITS_IN_CHAR - 1;
		}
		
		// OK, we must have a byte here that has at least one set bit.  
		// The question is, which bit is it?
		while( currBit != vuint8(-1) )
		{
			if( !(currentByte & 0x80) )
			{
				break;// Found our 1 bit
			}
			else
			{
				// Work out whether we are at the real beginning of our array
				if( mCurrentPosition <= 1 )
					return false;

				// Move to the next bit
				currBit--;
				mCurrentPosition--;
				currentByte <<= 1;		
			}
		}
		
		return true;
	}
	else
	{
		return false;	// No prev bit
	}
}


#pragma mark ===== BitSetIterator_01 =====


/**********************************************************************************************/
BitSetIterator_01::BitSetIterator_01( const BitSet_01& inBitSet ) :
	BitSetIterator( inBitSet )
{
	mType = inBitSet.mType;
}


#pragma mark -


/**********************************************************************************************/
bool BitSetIterator_01::GoToBit( vuint32 inIndex )
{
	FBL_CHECK( inIndex > 0 ); 	

	if( inIndex > mBitSet->get_MaxValue() )
		return false;

	mCurrentPosition = inIndex;
	
	return true;	
}


/**********************************************************************************************/
FBL_End_Namespace
