/**********************************************************************************************/
/* FBL_Range.cpp  																			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Values/FBL_Convert.h>

#include <VShared/FBL/publ/Util_classes/FBL_Range.h>


/**********************************************************************************************/
FBL_Begin_Namespace 


/**********************************************************************************************/
Range::Range(
	bool				inLeftIncluded, 
	Const_I_Value_Ptr	inLeftValue, 
	Const_I_Value_Ptr	inRightValue, 
	bool				inRightIncluded )
{								
	mLeftValue = (inLeftValue == nullptr) ?  nullptr : inLeftValue->Clone();
	mRightValue = (inRightValue == nullptr) ?  nullptr : inRightValue->Clone();

	mLeftIncluded = inLeftIncluded;
	mRightIncluded = inRightIncluded;
	
	// Added by IN, to fix build on Linux DebugMT_64 target
#if FBL_TEST_CODE		
	
	FBL_CHECK_EX( Invariant(), gRangeInvariant_Assert );
	
#endif // FBL_TEST_CODE	
}


/**********************************************************************************************/
Range::~Range( void ) 
{
}


/**********************************************************************************************/
Range_Ptr Range::ConvertToType( vuint32 inType ) const
{
	I_Value_Ptr pv1 = mLeftValue  ? ConvertValue( *mLeftValue , inType ) : nullptr;
	I_Value_Ptr pv2 = mRightValue ? ConvertValue( *mRightValue, inType ) : nullptr;
	
	return new Range( mLeftIncluded, pv1, pv2, mRightIncluded );	
}


/**********************************************************************************************/
bool Range::Invariant( void ) const 
{
	if( mLeftValue != nullptr && mRightValue != nullptr )
	{
		// KP prevention crash in client mode
		if( mLeftValue->get_IsRemote() )
			return true;

		int cmp = mLeftValue->Compare( *mRightValue );
	
		bool r1 = mLeftValue->get_Type() == mRightValue->get_Type(); 
		bool r2 = (cmp <= 0) ? true : false; 				
		bool r3 = (cmp == 0) ? (mRightIncluded == mLeftIncluded) : true;
		
		return r1 && r2 && r3;
	} 
	else
	{
		if( mLeftValue == nullptr && mRightValue == nullptr )
		{
			return mRightIncluded == mLeftIncluded ? true : false;
		}
		else
		{
			bool r1 = (mLeftValue == nullptr) ? (mLeftIncluded == false) : true;
			bool r2 = (mRightValue == nullptr) ? (mRightIncluded == false) : true;
			
			return r1 && r2;
		}
	}

}


#pragma mark -


/**********************************************************************************************/
void Range::put_LeftValue( Const_I_Value_Ptr inValue )  
{
	mLeftValue = (inValue == nullptr) ? nullptr : inValue->Clone();
}


/**********************************************************************************************/
void Range::put_RightValue( Const_I_Value_Ptr inValue )  
{
	mRightValue = (inValue == nullptr) ? nullptr : inValue->Clone();
}


/**********************************************************************************************/
bool Range::get_IsEmpty( void ) const 
{
	if( (mLeftValue == nullptr) && (mRightValue == nullptr) && 
		mLeftIncluded == true && mRightIncluded == true )
	{	
		return true;
	}
	else
	{
		return false;
	}
}


/**********************************************************************************************/
vuint32 Range::get_Type( void ) const 
{
	if( mLeftValue )	
		return mLeftValue->get_Type();
	
	if( mRightValue )	
		return mRightValue->get_Type();
		
	return kTypeEmpty;
}


#pragma mark -


/**********************************************************************************************/
bool Range::DoesInclude( Const_I_Value_Ptr inValue ) const 
{
	if( CheckLeft(inValue) && CheckRight(inValue) )
		return true;
			
	return false;
}

/**********************************************************************************************/
bool Range::DoesInclude( Const_Range_Ptr inRange ) const 
{
	if( CheckLeft( inRange->mLeftValue ) && 
		CheckRight( inRange->mRightValue ) )
	{
		return true;
	}
	
	return false;
}

/**********************************************************************************************/
bool Range::IsEqual( Const_Range_Ptr inRange ) const 
{
	return		IsLeftEqual( inRange->mLeftValue, inRange->mLeftIncluded ) 
			&&	IsRightEqual( inRange->mRightValue, inRange->mRightIncluded );
}


/**********************************************************************************************/
bool Range::IsLessThan( Const_Range_Ptr inRange ) const 
{ 
	return ( CheckLeft( inRange->mLeftValue ) ); 
}


#pragma mark -

 
/**********************************************************************************************/
// TRUE if inValue can be in this range relative to range's left side
//
bool Range::CheckLeft( Const_I_Value_Ptr inValue ) const 
{
	// Suppose that this range is not empty! (Defined range)
	FBL_CHECK( !get_IsEmpty() );
	
	// inValue always more then -inf.
	if( mLeftValue == nullptr )
		return true;

	// If Range's LeftValue is not inf then NULL as inValue, can't be in Range.
	if( inValue == nullptr )
		return false;

	int result =  mLeftValue->Compare( *inValue );

	if( result < 0 )
		return true;
		
	if( result == 0 )
		return mLeftIncluded;

	return false;
}


/**********************************************************************************************/
//	TRUE if inValue can be in this range relative to range's right side
//
bool Range::CheckRight( Const_I_Value_Ptr inValue ) const 
{
	// Suppose that this range is not empty! (Defined range)
	FBL_CHECK( !get_IsEmpty() );
	
	// inValue is always less then +inf.
	if( mRightValue == nullptr )
		return true;

	// If Range's RightValue is not inf then NULL as inValue, can't be in Range.
	if( inValue == nullptr )
		return false;

	int result = mRightValue->Compare( *inValue );
	
	if( result > 0 )
		return true;
		
	if( result == 0 )
		return mRightIncluded;

	return false;
}


/**********************************************************************************************/
//	Returns TRUE if this Range's left-edge	is equal to inValue with included flag combination.
//
bool Range::IsLeftEqual( Const_I_Value_Ptr inValue, bool included ) const 
{
	Const_I_Value_Ptr value = mLeftValue;

	// TRUE if both values are NULL and FALSE for one of them is NULL	
	if( value == nullptr && inValue == nullptr )
		return true;

	if( value == nullptr || inValue == nullptr )
		return false;
		
	if( value->Compare( *inValue ) == 0 && 
		included == mLeftIncluded )
	{
		return true;
	}
			
	return false;
}


/**********************************************************************************************/
//	Returns TRUE if this Range's right-edge	is equal to inValue with included flag combination.
//
bool Range::IsRightEqual( Const_I_Value_Ptr inValue, bool included ) const 
{
	Const_I_Value_Ptr value = mRightValue;

	// TRUE if both values are NULL and FALSE for one of them is NULL	
	if( value == nullptr && inValue == nullptr )
		return true;

	if( value == nullptr || inValue == nullptr )
		return false;

	if( value->Compare( *inValue ) == 0 &&
		included == mRightIncluded )
	{
		return true;
	}
			
	return false;
}


#pragma mark -


/**********************************************************************************************/
// Dump Range into given OStream as string in format [v1, v2]
//
String Range::get_String( void )
{
	String str(64);

	str += mLeftIncluded ? '[' : '(';
	str += mLeftValue ? mLeftValue->get_String() : "-inf";
	str += ", "	;
	str += mRightValue ? mRightValue->get_String() : "+inf";
	str += mRightIncluded ? ']' : ')';
	
	return str;
}


#pragma mark -


/**********************************************************************************************/
void SwapRanges(	
	Const_Range_Ptr* inout_1, 
	Const_Range_Ptr* inout_2 ) 
{
	// Choose Range with minimal LeftValue into inout_1 take into included flag
	Const_Range_Ptr helpRange  = nullptr;

	if( (*inout_2)->IsLessThan( *inout_1 ) || 
		(*inout_2)->IsLeftEqual( 
							(*inout_1)->get_LeftValue(), 
							(*inout_1)->get_IsLeftIncluded() ) )
	{
		helpRange = *inout_2;
		*inout_2 = *inout_1;
		*inout_1 = helpRange;
	} 
}


/**********************************************************************************************/
Range_Ptr DoIntersection( 	
	Const_Range_Ptr a, 
	Const_Range_Ptr b ) 
{
	SwapRanges( &a, &b );

	// Now a->mLeftValue <= than b->mLeftValue
	// If these ranges overlap each other INTERSECTION SHOULD be done.
	if( a->DoesInclude( b->get_LeftValue() ) )
	{
		// Calculate Right side of INTERSECTION
		Const_I_Value_Ptr rightValue;
		bool rightIncluded;
		
		if( b->DoesInclude( a->get_RightValue() ) )
		{
			// a = (1, 10) ; b = (2, 15)
			rightValue = a->get_RightValue();
			rightIncluded = a->get_IsRightIncluded();
		
		} else
		{
			// a = (1, 10); b = (2, 5)
			rightValue = b->get_RightValue();
			rightIncluded = b->get_IsRightIncluded();		
		} 
		
		return ( new Range( b->get_IsLeftIncluded(),
							 b->get_LeftValue(),
							 rightValue,
							 rightIncluded ) 
				);
	}
	
	// These Ranges can't to be under INTERSECTION.
	return nullptr;
}


/**********************************************************************************************/
//	inArr must be sorted already
//	inFromPos returns position of last element of the array, which has been intersect with range
//
ArrayOfRanges_Ptr DoIntersection( 	
	Const_Range_Ptr inR, 
	Const_ArrayOfRanges_Ptr inArr,
	vuint32* inFromPos ) 
{
	ArrayOfRanges_Ptr pRes;
	Range_Ptr pCur;
	vuint32 start = 1;
	vuint32 pos = 0;
	vuint32 Count = inArr->get_Count();
	
	if( inFromPos )
	{
		start = *inFromPos;
		pos = *inFromPos;
	}
	
	for( vuint32 i = start; i <= Count; ++i)
	{
		pCur = DoIntersection( inR, (Const_Range_Ptr) inArr->get_ItemAt( i ) );
		if( pCur != nullptr )
		{
			if( pRes == nullptr )
				pRes = new ArrayOfRanges();
			pRes->AddItem( pCur );
			pos = i;
		}
		else
		{ 
			// we already found intersection and do not need in work with other elements of array
			if( pRes && pRes->get_Count() != 0 )
				break;
		}
	}
	
	if( inFromPos )
		*inFromPos = pos;

	return pRes;
}

/**********************************************************************************************/
//	inArrA and inArrB must be sorted already
//
ArrayOfRanges_Ptr DoIntersection( 	
	Const_ArrayOfRanges_Ptr inArrA, 
	Const_ArrayOfRanges_Ptr inArrB ) 
{
	ArrayOfRanges_Ptr pRes = new ArrayOfRanges();
	ArrayOfRanges_Ptr pCur;
	vuint32 fromPos = 1;
	
	for( vuint32 i = 1; i <= inArrA->get_Count(); ++i )
	{
		Range_Ptr pR = inArrA->get_ItemAt( i );
		pCur = DoIntersection( pR, inArrB, &fromPos );
		
		if( pCur != nullptr )
		{			
			for( vuint32 j = 1; j <= pCur->get_Count(); ++j )
				pRes->AddItem( pCur->get_ItemAt( j ) );
		}
	}
	
	
	if( pRes->get_Count() == 0 )
		pRes = nullptr;

	return pRes;
}


/**********************************************************************************************/
Range_Ptr DoUnion( 	
	Const_Range_Ptr a, 
	Const_Range_Ptr b ) 
{
	SwapRanges( &a, &b );
	
	// Now a->mLeftValue <= than b->mLeftValue
	
	// If these ranges at least toaching each other UNION SHOULD be done.
	// We need two conditions because of this situation: a= (1, 10);  b = [10, 20)
	if( a->DoesInclude( b->get_LeftValue() ) ||
		b->DoesInclude( a->get_RightValue() ) )
	{
		// Calculate Right side of UNION
		Const_I_Value_Ptr rightValue;
		bool rightIncluded;
		
		if( a->DoesInclude( b->get_RightValue() ) )
		{
			// a = (1, 10); b = (5, 9)
			rightValue = a->get_RightValue();
			rightIncluded = a->get_IsRightIncluded();
		}
		else
		{
			// a = (1, 10); b = (5, 12)
			rightValue = b->get_RightValue();
			rightIncluded = b->get_IsRightIncluded();
		}

		return ( new Range( a->get_IsLeftIncluded(),
							 a->get_LeftValue(),
							 rightValue,
							 rightIncluded ) 
				);
				
	}
	
	// These Ranges can't to be under UNION.
	return nullptr;
}


/**********************************************************************************************/
//	inArr must be sorted already
//	inFromPos returns position of last element of the array, which has been union with range
//
ArrayOfRanges_Ptr DoUnion( 	
	Const_Range_Ptr inR, 
	Const_ArrayOfRanges_Ptr inArr,
	vuint32* inFromPos ) 
{
	ArrayOfRanges_Ptr pRes;
	Range_Ptr pCur;
	vuint32 start = 1;
	vuint32 pos = 0;
	vuint32 Count = inArr->get_Count();
	
	if( inFromPos )
	{
		start = *inFromPos;
		pos = *inFromPos;
	}
	
	for( vuint32 i = start; i <= Count; ++i)
	{
		pCur = DoUnion( inR, (Const_Range_Ptr) inArr->get_ItemAt( i ) );
		if( pCur != nullptr )
		{
			if( pRes == nullptr )
				pRes = new ArrayOfRanges();
			pRes->AddItem( pCur );
			pos = i;
		}
		else
		{ 
			// we already found intersection and do not need in work with other elements of array
			if( pRes && pRes->get_Count() != 0 )
				break;
		}
	}
	
	if( inFromPos )
		*inFromPos = pos;

	return pRes;
}


/**********************************************************************************************/
//	inArrA and inArrB must be sorted already
//
ArrayOfRanges_Ptr DoUnion( 	
	Const_ArrayOfRanges_Ptr inArrA, 
	Const_ArrayOfRanges_Ptr inArrB ) 
{
	ArrayOfRanges_Ptr pRes = new ArrayOfRanges();
	ArrayOfRanges_Ptr pCur;
	vuint32 fromPos = 1;
	
	for( vuint32 i = 1; i <= inArrA->get_Count(); ++i )
	{
		Range_Ptr pR = inArrA->get_ItemAt( i );
		pCur = DoUnion( pR, inArrB, &fromPos );
		
		if( pCur != nullptr )
		{			
			for( vuint32 j = 1; j <= pCur->get_Count(); ++j )
				pRes->AddItem( pCur->get_ItemAt( j ) );
		}
	}
	
	
	if( pRes->get_Count() == 0 )
		pRes = nullptr;

	return pRes;
}


/**********************************************************************************************/
FBL_End_Namespace


