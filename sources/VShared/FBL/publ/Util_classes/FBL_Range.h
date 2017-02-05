/**********************************************************************************************/
/* FBL_Range.h   																		  	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_Range_h 
	#define _FBL_Range_h 
	
#include <VShared/FBL/publ/Interfaces/FBL_I_Value.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_CLASS	( Range );
SMART_ARRAY_OF_SMART_PTRS( ArrayOfRanges, Range_Ptr );


/**********************************************************************************************/
// Utility Class that allows us to work with Range of I_Value.
//
// IMPORTANT: this Range can use ONLY I_Values that are comparable and have ORDER.
// For example, "range of BLOB values" does not have sense.			 
// 
// We need to ensure that both values have the same type	
// so it's impossible to have a range like (int, string) 
//
// INVARIANT: LeftValue <= RightValue.
// INVARIANT: if LeftValue == RightValue then the flags Included must be TRUE.
// User can easy ensure this comparing values before creation of Range object.
//
//
// NOTE:
// Range is "undefined" when [-inf, +inf] e.c. Range( true, NULL, NULL, true )
//
// NO OPERATIONS ARE DEFINED ON "undefined" range!!!
//
// Range is unlimited when (-inf, +inf)  e.c. Range( false, NULL, NULL, false )
// It's impossible to have NULL as a point value of range.
//

class FBL_SHARED_EXP_CLASS Range : public I_Unknown
{
		IMPLEMENT_SINGLE_UNKNOWN( Range )

	public://///////////////////////////////////////////////////////////////////////////////////

		// ---------------------
		// Construction:
	
							// On Default create range (-inf, +inf),
							// so later you can assign only one point to get correct range.
							Range(	bool				inLeftIncluded 	= false,
									Const_I_Value_Ptr	inLeftValue		= nullptr, 									
									Const_I_Value_Ptr	inRightValue	= nullptr, 
									bool				inRightIncluded = false );

		Range_Ptr			ConvertToType( vuint32 inType ) const;
							

	protected://////////////////////////////////////////////////////////////////////////////////
		
		virtual				~Range( void );
							

	public://///////////////////////////////////////////////////////////////////////////////////

		// ---------------------
		// Properties:

			// <LeftValue>

		Const_I_Value_Ptr	get_LeftValue( void ) const 	{ return mLeftValue; }		
		void				put_LeftValue( Const_I_Value_Ptr inValue );
		
			// <RightValue>

		Const_I_Value_Ptr	get_RightValue( void ) const  { return mRightValue; }		
		void				put_RightValue( Const_I_Value_Ptr inValue );

			// <IsLeftIncluded>

		bool				get_IsLeftIncluded( void ) const  
								{ return mLeftIncluded; }

		void				put_IsLeftIncluded( bool inIncluded )  
								{ mLeftIncluded = inIncluded; }

			// <IsRightIncluded>

		bool				get_IsRightIncluded( void ) const  
								{ return mRightIncluded; }

		void				put_IsRightIncluded( bool inIncluded )  
								{ mRightIncluded = inIncluded; }
							
			// <IsEmpty> [r/o]				
							
							/**	Returns TRUE if this Range is EMPTY.
								A Range is EMPTY if we have magic combination of values. */
		bool				get_IsEmpty( void ) const;

			// <Type> [r/o]				

		vuint32				get_Type( void ) const;


	public://///////////////////////////////////////////////////////////////////////////////////

		// ---------------------
		// Comparision Methods:
							
							/** Returns TRUE if this Range includes the specified inValue. */
		bool				DoesInclude( Const_I_Value_Ptr inValue ) const;
		
							/**	Returns TRUE if this Range includes the specified inRange. */
		bool				DoesInclude( Const_Range_Ptr inRange ) const;

							/** Returns TRUE if this Range is equal to inRange.	*/
		bool				IsEqual( Const_Range_Ptr inRange ) const;
							
							/**	Returns TRUE if this Range is less than inRange.
								It is less if left point is less. */
		bool				IsLessThan( Const_Range_Ptr inRange ) const;


	public://///////////////////////////////////////////////////////////////////////////////////

		// ---------------------
		// Range Methods:

friend	FBL_SHARED_EXP 
		Range_Ptr			DoUnion( 	
								Const_Range_Ptr a, 
								Const_Range_Ptr b );

friend	FBL_SHARED_EXP 
		Range_Ptr			DoIntersection( 	
								Const_Range_Ptr a, 
								Const_Range_Ptr b );

friend	FBL_SHARED_EXP 
		void 				SwapRanges(	
								Const_Range_Ptr* inout_1, 
								Const_Range_Ptr* inout_2 );
								

		// ---------------------
		// Dump Methods:

		String				get_String( void );


	private://///////////////////////////////////////////////////////////////////////////////////
		
		// ---------------------
		// Helper Methods:

		bool				CheckLeft( Const_I_Value_Ptr inValue ) const;
		bool				CheckRight( Const_I_Value_Ptr inValue ) const;

		bool				IsLeftEqual( 
									Const_I_Value_Ptr inValue, 
									bool included ) const;

		bool				IsRightEqual( 
									Const_I_Value_Ptr inValue, 
									bool included ) const;

		bool				Invariant( void ) const;
		

	private://///////////////////////////////////////////////////////////////////////////////////

		// ---------------------
		// Data:

		I_Value_Ptr 		mLeftValue;			
		I_Value_Ptr			mRightValue;			

		bool				mLeftIncluded;		 
		bool				mRightIncluded;
};


/**********************************************************************************************/
// Range's arithmetics:

									/**	If result.get_IsEmpty() then these ranges diverges! */
FBL_SHARED_EXP Range_Ptr			DoIntersection( 	
										Const_Range_Ptr a, 
										Const_Range_Ptr b );

FBL_SHARED_EXP ArrayOfRanges_Ptr 	DoIntersection( 	
										Const_Range_Ptr inR, 
										Const_ArrayOfRanges_Ptr inArr,
										vuint32* inFromPos = nullptr );

FBL_SHARED_EXP ArrayOfRanges_Ptr 	DoIntersection( 	
										Const_ArrayOfRanges_Ptr inArrA, 
										Const_ArrayOfRanges_Ptr inArrB );

									/**	If result.get_IsEmpty() then these ranges diverges! */
FBL_SHARED_EXP Range_Ptr			DoUnion( 	
										Const_Range_Ptr a, 
										Const_Range_Ptr b );
							
FBL_SHARED_EXP ArrayOfRanges_Ptr 	DoUnion( 	
										Const_Range_Ptr inR, 
										Const_ArrayOfRanges_Ptr inArr,
										vuint32* inFromPos = nullptr );

FBL_SHARED_EXP ArrayOfRanges_Ptr 	DoUnion( 	
										Const_ArrayOfRanges_Ptr inArrA, 
										Const_ArrayOfRanges_Ptr inArrB );
								
									/** Swap two ranges if needed in such way
										Choose Range with minimal LeftValue in inout_1
										take into included flag. */
FBL_SHARED_EXP void 				SwapRanges(	
										Const_Range_Ptr* inout_1, 
										Const_Range_Ptr* inout_2 );
								

/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_Range_h
