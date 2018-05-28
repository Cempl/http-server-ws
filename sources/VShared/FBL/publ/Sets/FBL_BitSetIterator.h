/**********************************************************************************************/
/* FBL_BitSetIterator.h																		  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_BitIterator_h
   #define _FBL_BitIterator_h
#pragma once

#include <VShared/FBL/publ/Sets/FBL_SetIterator.h>
#include <VShared/FBL/publ/Sets/FBL_BitSet.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_CLASS( BitSetIterator );
SMART_CLASS( SetIterator );


/**********************************************************************************************/
#define BITS_IN_CHAR	8


/**********************************************************************************************/
class FBL_SHARED_EXP_CLASS BitSetIterator : public SetIterator
{
	public://///////////////////////////////////////////////////////////////////////////////////

								BitSetIterator( const BitSet& inBitSet );

virtual							~BitSetIterator( void );


		/* Iteration methods */

virtual ID_TYPE	 				operator* ( void ) const override 	{ return mCurrentPosition; }
		ID_TYPE	 				GetPosition( void )			{ return mCurrentPosition; }

virtual bool					First( void ) override	{ return FirstBit_1(); 	}
virtual bool					Last( void ) override	{ return LastBit_1();	}

virtual bool					Next( void ) override	{ return NextBit_1(); 	}
virtual bool					Prev( void ) override	{ return PrevBit_1(); 	}

								// Locate Nth 1-bit. Not very fast function.
virtual bool					GoTo( vuint32 inIndex ) override;


		/* High level methods */

virtual ID_TYPE					FirstItem( void ) override;
virtual ID_TYPE					LastItem( void ) override;
							
virtual ID_TYPE					NextItem( void ) override;
virtual ID_TYPE					PrevItem( void ) override;


	public://///////////////////////////////////////////////////////////////////////////////////
	
		/* BitSet specific methods: */
		
								// Set iterator to the first 1/0 bit.
								// Returns true if such bit found.
virtual bool					FirstBit_1( void );
virtual bool					FirstBit_0( void );

								// Move iterator to the last 1/0 bit.
								// Returns true if such bit found.
virtual bool					LastBit_1( void );
virtual bool					LastBit_0( void );

								// Move iterator to the next 1/0 bit.
								// Returns true if such bit found.
virtual bool					NextBit_1( void );
virtual bool					NextBit_0( void );

								// Move iterator to the next 1/0 bit.
								// Returns true if such bit found.
virtual bool					PrevBit_1( void );
virtual bool					PrevBit_0( void );


	public://///////////////////////////////////////////////////////////////////////////////////
	
								// Move iterator to the first/last bit.
virtual void					FirstBit( void );
virtual void					LastBit( void );

								// Move iterator to the next/prev bit.
								// Returns true if such bit found.
virtual bool					NextBit( void );
virtual bool					PrevBit( void );

								// Set iterator to the Nth bit of iterated BitSet.
virtual bool					GoToBit( vuint32 inNth );
	
	
    protected://////////////////////////////////////////////////////////////////////////////////

Const_BitSet_Ptr				mBitSet;
		vuint8*					pEndByte;

		vuint8*					pCurrentByte;
		vuint8					currBit;			// range [0..31]
		ID_TYPE					mCurrentPosition;
};




/**********************************************************************************************/
inline void BitSetIterator::FirstBit( void )
{
	pCurrentByte 		= mBitSet->mArray;
	currBit 			= 0;
	mCurrentPosition   	= 1;
}


/**********************************************************************************************/
inline void BitSetIterator::LastBit( void )
{
	vuint32 byte = (mBitSet->mMaxValue-1) / 8;

	pCurrentByte 		= mBitSet->mArray + byte;
	currBit 			= (vuint8)((mBitSet->mMaxValue-1) % 8);
	mCurrentPosition   	= mBitSet->mMaxValue;
}


/**********************************************************************************************/
inline bool BitSetIterator::GoTo( vuint32 inIndex )
{
	FBL_CHECK( inIndex > 0 ); 	

	mCurrentPosition = mBitSet->FindNth_1_Bit( inIndex );
	return mCurrentPosition ? true : false;
}




#pragma mark -

 
/**********************************************************************************************/
inline ID_TYPE BitSetIterator::FirstItem( void )
	{ return BitSetIterator::First() ? mCurrentPosition : 0; }


/**********************************************************************************************/
inline ID_TYPE BitSetIterator::LastItem( void )
	{ return BitSetIterator::Last() ? mCurrentPosition : 0; }


/**********************************************************************************************/
inline ID_TYPE BitSetIterator::NextItem( void )
	{ return BitSetIterator::Next() ? mCurrentPosition : 0; }


/**********************************************************************************************/
inline ID_TYPE BitSetIterator::PrevItem( void )
	{ return BitSetIterator::Prev() ? mCurrentPosition : 0; }


#pragma mark -


/**********************************************************************************************/
// This class represent full (all rows selected withot deleted holes) bitset.
//
class FBL_SHARED_EXP_CLASS BitSetIterator_01 : public BitSetIterator
{
	public://///////////////////////////////////////////////////////////////////////////////////

								BitSetIterator_01( const BitSet_01& inBitSet );


	public://///////////////////////////////////////////////////////////////////////////////////
	
		/* BitSet specific methods: */
		
								// Set iterator to the first 1/0 bit.
								// Returns true if such bit found.
virtual bool					FirstBit_1( void ) override;
virtual bool					FirstBit_0( void ) override;

								// Move iterator to the last 1/0 bit.
								// Returns true if such bit found.
virtual bool					LastBit_1( void ) override;
virtual bool					LastBit_0( void ) override;

								// Move iterator to the next 1/0 bit.
								// Returns true if such bit found.
virtual bool					NextBit_1( void ) override;
virtual bool					NextBit_0( void ) override;

								// Move iterator to the next 1/0 bit.
								// Returns true if such bit found.
virtual bool					PrevBit_1( void ) override;
virtual	bool					PrevBit_0( void ) override;


	public://///////////////////////////////////////////////////////////////////////////////////
	
								// Move iterator to the first/last bit.
virtual void					FirstBit( void ) override;
virtual void					LastBit( void ) override;

								// Move iterator to the next/prev bit.
								// Returns true if such bit found.
virtual bool					NextBit( void ) override;
virtual bool					PrevBit( void ) override;

								// Set iterator to the Nth bit of iterated BitSet.
virtual bool					GoToBit( vuint32 inNth ) override;


	private:////////////////////////////////////////////////////////////////////////////////////

		bool					mType;	 // true - bitset filled 1, false - 0.

};


/**********************************************************************************************/
inline void BitSetIterator_01::FirstBit( void )
{
	mCurrentPosition = 1;
}


/**********************************************************************************************/
inline void BitSetIterator_01::LastBit( void )
{
	mCurrentPosition = mBitSet->mMaxValue;
}


#pragma mark -
 

/**********************************************************************************************/
inline bool BitSetIterator_01::NextBit( void )
{
	FBL_CHECK( 1 <= mCurrentPosition && mCurrentPosition <= mBitSet->mMaxValue ); 

	// Work out whether we are at the real end of our array
	if( mCurrentPosition >= mBitSet->mMaxValue )
		return false;
	
	return ++mCurrentPosition <=  mBitSet->mMaxValue;
}


/**********************************************************************************************/
// Move iterator to the prev bit. Returns true if such bit found.
//
inline bool BitSetIterator_01::PrevBit( void )
{
	FBL_CHECK( 1 <= mCurrentPosition && mCurrentPosition <= mBitSet->mMaxValue ); 

	if( mCurrentPosition <= 1 )
		return false;
	
	// We are going to move on one index whatever
	return --mCurrentPosition > 0;
}


#pragma mark -
 

/**********************************************************************************************/
inline bool BitSetIterator_01::FirstBit_1( void )
{
	if( mBitSet->mBytesInArray == 0 ) // No bits in the array
		return false;
	
	if( mType )
	{
		// Move to the beginning of the array
		FirstBit();
	}
	else
	{
		mCurrentPosition = 0;
	}
	
	return mType;
}


/**********************************************************************************************/
inline bool BitSetIterator_01::FirstBit_0( void )
{
	if( mBitSet->mBytesInArray == 0 ) // No bits in the array
		return false;
	
	if( !mType )
	{
		// Move to the beginning of the array
		FirstBit();
	}
	else
	{
		mCurrentPosition = 0;
	}
	
	return !mType;
}


#pragma mark -
 

/**********************************************************************************************/
inline bool BitSetIterator_01::LastBit_1( void )
{
	if( mBitSet->mBytesInArray == 0 ) // No bits in the array
		return false;
	
	if( mType )
	{
		// Move to the end of the array
		LastBit();
	}
	else
	{
		mCurrentPosition = 0;
	}
	
	return mType;
}


/**********************************************************************************************/
inline bool BitSetIterator_01::LastBit_0( void )
{
	if( mBitSet->mBytesInArray == 0 ) // No bits in the array
		return false;
	
	if( !mType )
	{
		// Move to the end of the array
		LastBit();
	}
	else
	{
		mCurrentPosition = 0;
	}
	
	return !mType;
}


#pragma mark -
 

/**********************************************************************************************/
// Move iterator to the next nonzero bit. Returns true if such bit found.
//
inline bool BitSetIterator_01::NextBit_1( void )
{
	FBL_CHECK( mBitSet->mBytesInArray > 0 ); 

	// We need to move to the next bit
	return mType ? NextBit() : false;

} /* BitSetIterator::NextBit_1 */


/**********************************************************************************************/
inline bool BitSetIterator_01::NextBit_0( void )
{
	FBL_CHECK( mBitSet->mBytesInArray > 0 ); 
	
	return !mType ? NextBit() : false;
}


#pragma mark -
 

/**********************************************************************************************/
inline bool BitSetIterator_01::PrevBit_1( void )
{
	FBL_CHECK( mBitSet->mBytesInArray > 0 ); 
	
	// We need to move to the prev bit
	return mType ? PrevBit() : false;
}


/**********************************************************************************************/
inline bool BitSetIterator_01::PrevBit_0( void )
{
	FBL_CHECK( mBitSet->mBytesInArray > 0 ); 

	return !mType ? PrevBit() : false;
}


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_BitIterator_h
