/**********************************************************************************************/
/* FBL_BitSet.h																				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_BitSet_h
   #define _FBL_BitSet_h
#pragma once

#include <VShared/FBL/publ/Sets/FBL_Set.h>
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>

 
/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// Constants:
//
#define FBL_WORD_SIZE 	sizeof(int)
#define WORD_BITS_ON 	0xFFFFFFFF			// for sizeof(int) == 4



/**********************************************************************************************/
SMART_CLASS( BitSet );
SMART_CLASS( BitSet_01 );
SMART_CLASS( BitSetIterator );
SMART_CLASS( BitSetIterator_01 );

SMART_ARRAY_OF_SMART_PTRS( ArrayOfBitSets, BitSet_Ptr );


/**********************************************************************************************/
// forward declarations: 
//
SMART_CLASS( ArraySet );


/**********************************************************************************************/
/// General purpose class.
/// Implements Set of vuint32 numbers as BitSet.
/// On creation you must specify the top value.
//
class FBL_SHARED_EXP_CLASS BitSet : public Set
{
		friend class ArraySet;
		friend class BitSetIterator;
		friend class BitSetIterator_01;

	protected://////////////////////////////////////////////////////////////////////////////////
							
							BitSet( void ) {} // This constructor is needed for inheritance.							
virtual		    			~BitSet( void );	


	public://///////////////////////////////////////////////////////////////////////////////////

		//-----------------
		// Construction:
		
							BitSet( ID_TYPE	inMaxValue );						
							BitSet( const BitSet& inSrc );
							BitSet( 
								ID_TYPE				inMaxValue,	 
								Const_ArraySet_Ptr	inSet );
						

	public://///////////////////////////////////////////////////////////////////////////////////

// Set API:
		
	//----------------------
	// Properties:

virtual vuint32				get_Count( void ) const override;
virtual bool				get_IsEmpty( void ) const override;
virtual SetIterator_Ptr		get_Iterator( void ) const override;


	// ---------------------
	// Construction Methods:

virtual Set*				Clone( bool inCopy = true ) const override;


	// ---------------------
	// Item Methods:
		
virtual void				Append( ID_TYPE inItem ) override;
virtual void				Remove( ID_TYPE inItem ) override;
virtual bool				Include( ID_TYPE inItem ) const override;					


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
		
		bool				get_IsSorted( void ) const override  	{ return true; }
		void				put_IsSorted( bool inSorted ) override   { argused1( inSorted );	}
		
							// Sort items by values.
		void				Sort( void ) override  { /* do nothing. */ }


	//----------------------
	// Unary operators:

virtual Set_Ptr				Union 		 		( const Set_Ptr inSet ) override; // OR
virtual Set_Ptr				Intersection		( const Set_Ptr inSet ) override; // AND
virtual Set_Ptr				Difference   		( const Set_Ptr inSet ) override; // AND NOT
virtual Set_Ptr				SymmetricDifference	( const Set_Ptr inSet ) override; // XOR


	public://///////////////////////////////////////////////////////////////////////////////////

// I_Serializable API:

virtual void				From( I_IStream_Ptr inStream,
								  bool inBlock = true ) override;

virtual void				From( I_PacketRcv*	 	inPacket,
								  bool 				inBlock = true ) override;


virtual void				To( I_OStream_Ptr inStream,
								bool inBlock = true ) const override;
							
virtual void				To( I_PacketSnd*	inPacket,
								bool 			inBlock = true ) const override;
							
		
	public://///////////////////////////////////////////////////////////////////////////////////

// THIS CLASS API:

	//----------------------
	// Properties:

		virtual vuint8*		get_ArrayPtr	( void ) const	{ return mArray; }
		virtual vuint32		get_BytesInArray( void ) const	{ return mBytesInArray; }
		
		ID_TYPE				get_Bits_1_Count( void ) const	
                            {
                            	#if FBL_TEST_CODE
//                            		FBL_CHECK( mCount_1 == CountBits_1() );
	                            #endif // FBL_TEST_CODE
                            
                            	return mCount_1; 
                            }        
        
		ID_TYPE				get_Bits_0_Count( void ) const  { return get_MaxValue() - get_Bits_1_Count(); }

		ID_TYPE				get_MaxValue( void ) const		{ return mMaxValue; }
		void				put_MaxValue( ID_TYPE inMaxValue );
		
							/// Returns first and last nonzero bytes in set (range 0..N-1).
virtual void				get_NonZeroBounds( 
								ID_TYPE& 	outFirst, 
								ID_TYPE& 	outLast ) const;
																	
		vuint32				get_Size( void ) const { return mBytesInArray; }

static	vuint32				get_TotalSize( void ) { return sTotalSize; }


	//---------------------
	// Set Operations:

							/// Copy bytes from a given Set.
virtual void				operator= ( const BitSet& inSet );

							/// make this BitSet equal to the specified bit set string.
virtual void				operator= ( const char* inBitString );

	
	//----------------------
	// Unary Operations for BitSet:
	
virtual bool				IsEqual( const BitSet& inSet ) const;
					
virtual void				Invert( void );	/// invert all bits.

		// The folowing methods execute operation in place.
		// Only BitSet have such methods. 
		//		
virtual void				Union 		 		( const BitSet& inSet ); // OR
virtual void				Intersection		( const BitSet& inSet ); // AND
virtual void				Difference   		( const BitSet& inSet ); // AND NOT
virtual void				SymmetricDifference	( const BitSet& inSet ); // XOR


	//----------------------
	// Binary Operations for BitSet.
    // Create a new BitSet as result.
						
friend	FBL_SHARED_EXP 
		BitSet_Ptr			Union( 
								const BitSet& inSet1,
								const BitSet& inSet2 );
	
friend	FBL_SHARED_EXP 
		BitSet_Ptr			Intersection( 
								const BitSet& inSet1,
								const BitSet& inSet2 );
		
friend FBL_SHARED_EXP 
		BitSet_Ptr			Difference( 
								const BitSet& inSet1,
								const BitSet& inSet2 );
	

friend FBL_SHARED_EXP 
		BitSet_Ptr			SymmetricDifference( 
								const BitSet& inSet1,
								const BitSet& inSet2 );
	
	
	public://///////////////////////////////////////////////////////////////////////////////////
 
//virtual bool 				FindContinuous(
//								vuint32		inCount,
//								BitSet&		ioResult ) const;
		
virtual ID_TYPE 			FindNth_1_Bit( vuint32 inNth ) const;


	public://///////////////////////////////////////////////////////////////////////////////////
 
	//----------------------
	// Bit Methods:

virtual	void				RefreshCount_1( void );

virtual	vuint32	 			CountBits_1( void ) const;

							/// Set all bits in set to 1/0.
virtual void				ClearAll( void );
virtual void				SetAll( void );
							
							/// Fill all bits in mArray starting from inStartValue by inItem.
virtual void				SetBits  ( 
								ID_TYPE inLeft	= 1, 
								ID_TYPE inRight = vuint32_max );

virtual void				ClearBits( 
								ID_TYPE inLeft	= 1, 
								ID_TYPE inRight = vuint32_max );

							/// Fill all bytes in mArray with given BYTE value.
/*
virtual void				SetAllBytes( 
								vuint8	inFillValue, 
								ID_TYPE inStartByte = 0 );
*/								

							/// VERY CAREFULLY !!
                            /// This should be  called only if algorithm have info about count of 1.
                            /// For example, we load bitset from file or stream.
		void				put_Count_1( vuint32 inCount_1 ) 
        					{
                            	FBL_CHECK( inCount_1 == CountBits_1() );  
                            	mCount_1 = inCount_1; 
                            }


	protected://////////////////////////////////////////////////////////////////////////////////

//		vuint32 			FindContinuous_Core(
//								vuint32		inCount,
//								BitSet&		ioResult );

virtual void 				InitSelf( ID_TYPE inMaxValue );
							
							/// Calculates the byte in mArray, which contains bit of inItem.
							/// We do  x/8 for this.
		vuint32				Loc( ID_TYPE inItem ) const 
								{ return inItem >> 3; }
	
							
							/// Returns Mask. &7 - means cutting of low 3 bits from value.
							/// So we get 0..7 values. Then we shift left 1 on that value
							/// to get one of bytes: 00000001, 00000010, 00000100, 00001000 .. 10000000
		vuint8				Mask( ID_TYPE inItem ) const 
								{ return vuint8(1UL << (inItem & 0x07)); }


	protected://////////////////////////////////////////////////////////////////////////////////

		vuint8*  			mArray;
		vuint32				mBytesInArray;	// alligned to int.
							
		ID_TYPE				mMaxValue;
        vuint32				mCount_1;
							
static 	const vuint8     	sMasks[8];
static	const vuint32 		sBitsCount[256];

static 	vuint32				sTotalSize;		// Total Size of all existed BitSet objects.
};



/**********************************************************************************************/
/*
inline void BitSet::SetAllBytes( vuint8 inFillValue, ID_TYPE inStartByte )
{
	vuint8* p1  = mArray + inStartByte;
	memset( p1, inFillValue, mBytesInArray - inStartByte );	
}*/


#pragma mark -


/**********************************************************************************************/
/// General purpose class.
/// Implements Set of vuint32 numbers as BitSet that have ONLY 1 or 0 bits.
/// For such bitset we do NOT allocate array of N bits.
/// Instead we override methods so they always return value 1 or 0 for any bit.
/// We get practically ZERO memory overhead for ANY size if bitset.
//
class FBL_SHARED_EXP_CLASS BitSet_01 : public BitSet
{
		friend class BitSetIterator_01;

	public://///////////////////////////////////////////////////////////////////////////////////

	//----------------------
	// Constructors:
		
							BitSet_01(
								bool	inType,			///< type of bitset, true - filled 1, false - filled 0.
								ID_TYPE	inMaxValue );

							BitSet_01( const BitSet& inSrc );

							BitSet_01( 
								bool			inType,	///< type of bitset, true - filled 1, false - filled 0.
								const BitSet&	inSrc );
						
virtual						~BitSet_01( void ){};


	public://///////////////////////////////////////////////////////////////////////////////////

// Set API:
		
	//----------------------
	// Properties:
		
virtual SetIterator_Ptr		get_Iterator( void ) const override;
virtual bool				get_IsEmpty( void ) const override;

	// ---------------------
	// Item methods:
		
virtual void				Append 	( ID_TYPE inItem ) override;
virtual void				Remove	( ID_TYPE inItem ) override;
virtual bool				Include( ID_TYPE inItem ) const override;


	public://///////////////////////////////////////////////////////////////////////////////////

// BitSet API:
		
virtual vuint8*				get_ArrayPtr( void ) const override;

virtual void				get_NonZeroBounds( 
								ID_TYPE& 	outFirst, 
								ID_TYPE& 	outLast ) const override;
		

	//----------------------
	// Set operations:

							/// Copy bytes from a given Set.
virtual void				operator= ( const BitSet& inSet ) override;
									
							/// make this BitSet equal to specified bit set string.
virtual void				operator= ( const char* inBitString ) override;
	
		
	//----------------------
	// Unary operators. Modify this set.
	
virtual bool				IsEqual( const BitSet& inSet ) const override;
						
virtual void				Invert( void ) override;
	
virtual void				Union 				( const BitSet& inSet ) override;
virtual void				Intersection		( const BitSet& inSet ) override;
virtual void				Difference   		( const BitSet& inSet ) override;
virtual void				SymmetricDifference	( const BitSet& inSet ) override;

	
	public://///////////////////////////////////////////////////////////////////////////////////

virtual ID_TYPE 			FindNth_1_Bit( vuint32 inNth ) const override;


	public://///////////////////////////////////////////////////////////////////////////////////
 
	//----------------------
	// Bit methods:

virtual	vuint32	 			CountBits_1( void ) const override;

							// Set all bits in set to 1/0.
virtual void				ClearAll( void ) override;
virtual void				SetAll	( void ) override;
							
							// Fill all bits in mArray starting from inStartValue by inItem.
virtual void				SetBits  ( 
								ID_TYPE inLeft	= 1, 
								ID_TYPE inRight	= (ID_TYPE) -1 ) override;
								
virtual void				ClearBits( 
								ID_TYPE inLeft	= 1, 
								ID_TYPE inRight = (ID_TYPE) -1 ) override;

							// Fill all bytes in mArray with a given BYTE value.
/*
virtual void				SetAllBytes( 
								vuint8 inFillValue, 
								ID_TYPE inStartByte = 0 );
*/								

	protected://////////////////////////////////////////////////////////////////////////////////

virtual void 				InitSelf( ID_TYPE inMaxValue ) override;


	private:////////////////////////////////////////////////////////////////////////////////////

		bool				mType;	 // true - bitset filled 1, false - 0.
};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_BitSet_h
