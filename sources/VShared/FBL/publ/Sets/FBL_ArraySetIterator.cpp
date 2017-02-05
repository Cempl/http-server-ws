/**********************************************************************************************/
/* FBL_ArraySetIterator.cpp																	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Sets/FBL_ArraySetIterator.h>

#include <VShared/FBL/publ/Sets/FBL_ArraySet.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
ArraySetIterator::ArraySetIterator( void ) :
	mpSet( nullptr )
{
	mpCurrentItem = nullptr;
	mColumnCount = 1;
}


/**********************************************************************************************/
ArraySetIterator::ArraySetIterator( const ArraySet& inSet ) :
	mpSet( &inSet )
{
	mpCurrentItem = mpSet->mpStart;
	mColumnCount = 1;
}


/**********************************************************************************************/
ArraySetIterator::~ArraySetIterator( void ) 	
{ 

}


#pragma mark -


/**********************************************************************************************/
void ArraySetIterator::ReadRow( ID_TYPE* outRow ) const
{
	memcpy(outRow, mpCurrentItem, sizeof(*mpCurrentItem) * mColumnCount);
}

 
/**********************************************************************************************/
bool ArraySetIterator::First( void ) 	
{ 
	if( mpSet->mpStart == mpSet->mpFinish )
		return false;
	else
	{ 
		mpCurrentItem = mpSet->mpStart; 
		return true;
	}
}

/**********************************************************************************************/
bool ArraySetIterator::Last( void ) 	
{ 
	if( mpSet->mpStart == mpSet->mpFinish )
	{
		return false;		
	}
	else
	{ 
		mpCurrentItem = mpSet->mpFinish - mColumnCount; 
		return true;
	}
}

/**********************************************************************************************/
bool ArraySetIterator::Next( void ) 	
{ 
	if( mpCurrentItem + mColumnCount < mpSet->mpFinish )
	{
		mpCurrentItem += mColumnCount;
		return true;
	}
	else 
	{
		return false;	
	}
}

/**********************************************************************************************/
bool ArraySetIterator::Prev( void ) 	
{ 
	if( mpCurrentItem - mColumnCount >= mpSet->mpStart )
	{
		mpCurrentItem -= mColumnCount;
		return true;
	}
	else 
	{
		return false;			
	}
}

/**********************************************************************************************/
bool ArraySetIterator::GoTo( vuint32 inIndex )
{
	FBL_CHECK( inIndex > 0 ); 	
	
	// IS: 20080222
	// http://valentina-db.com/bt/view.php?id=3046
	// It is reported by Florian team - if we use put_RecID() to next after the last record
	// in cursor over join we wrongfully allowed it here. And get the crash trying to read some 
	// varChar field on not-existed position.
	//

	// if( inIndex <= mpSet->get_Count() )
	if( inIndex <= (mpSet->get_Count() / mColumnCount ) )
	{
		mpCurrentItem = mpSet->mpStart + (inIndex - 1) * mColumnCount;
		return true;
	}
	else
	{	
		mpCurrentItem = nullptr;
		return false;
	}
}


/**********************************************************************************************/
FBL_End_Namespace
