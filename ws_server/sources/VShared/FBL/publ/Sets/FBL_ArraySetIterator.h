/**********************************************************************************************/
/* FBL_ArraySetIterator.h																	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                       					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_ArrayIterator_h
   #define _FBL_ArrayIterator_h
#pragma once

#include <VShared/FBL/publ/Sets/FBL_SetIterator.h>
#include <VShared/FBL/publ/Sets/FBL_ArraySet.h>
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_CLASS( ArraySetIterator );


/**********************************************************************************************/
class FBL_SHARED_EXP_CLASS ArraySetIterator : public SetIterator
{
	public://///////////////////////////////////////////////////////////////////////////////////

								ArraySetIterator( void );
								ArraySetIterator( const ArraySet& inSet );

virtual							~ArraySetIterator( void );							

	// -------------------------
	// Properties:

		vuint32					get_ColumnCount( void ) const  
									{ return mColumnCount; } 
									
		void					put_ColumnCount( vuint32 inColumns )  
									{ mColumnCount = inColumns; } 


	// -------------------------
	// Value methods:

virtual ID_TYPE	 				operator* ( void ) const override { return *mpCurrentItem; }
								
								/**	Reads contiguously the row of values from the current 
									position with length mColumnCount. */
virtual void 					ReadRow( ID_TYPE* outRow ) const;


	// -------------------------
	// Navigation:

virtual bool					First( void ) override;
virtual bool					Last( void ) override;
 	
virtual bool					Next( void ) override;	
virtual bool					Prev( void ) override;	

virtual bool					GoTo( vuint32 inIndex ) override;
		
		
	// -------------------------
	// High level methods:

virtual ID_TYPE					FirstItem( void ) override 
									{ return ArraySetIterator::First() ? *mpCurrentItem : 0; }
								
virtual ID_TYPE					LastItem( void ) override
									{ return ArraySetIterator::Last() ? *mpCurrentItem : 0; }
							
virtual ID_TYPE					NextItem( void ) override
									{ return ArraySetIterator::Next() ? *mpCurrentItem : 0; }
								
virtual ID_TYPE					PrevItem( void ) override
									{ return ArraySetIterator::Prev() ? *mpCurrentItem : 0; }

							
    protected://////////////////////////////////////////////////////////////////////////////////

		Const_ArraySet_Ptr		mpSet;
		ID_TYPE*				mpCurrentItem;
		
		vuint32 				mColumnCount; 	///< On default = 1.
												///< For n-dimensional array can be n, to step by records.
};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_ArrayIterator_h
