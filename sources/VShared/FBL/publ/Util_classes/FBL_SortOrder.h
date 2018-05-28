/**********************************************************************************************/
/* FBL_SortOrder.h			                                                      			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_SortOrder_h 
	#define _FBL_SortOrder_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Field.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_CLASS( SortOrder );


/**********************************************************************************************/
/// Helper class, to build the sort order of fields for Table sorting.
//
class SortItem
{
	public://///////////////////////////////////////////////////////////////////////////////////
 
 							SortItem( 	
								vuint16			inFieldIndex,
								EOrder			inOrder	= kAsc )
							:
								mpField( nullptr ),
								mpOriginalField( nullptr ),
   								mFieldIndex( inFieldIndex ),
								mJoinColumnIndex(0),
 								mOrder( inOrder )
 							{
 							}

							SortItem( 	
								I_Field_Ptr 	inField,
								EOrder			inOrder	= kAsc )
							:
								mpField( inField.get() ),
								mpOriginalField( mpField ),
   								mFieldIndex( 0 ),
								mJoinColumnIndex(0),
 								mOrder( inOrder )
 							{
 							}
 
 
	public://///////////////////////////////////////////////////////////////////////////////////
 
		I_Field*			mpField;
		I_Field*			mpOriginalField;
    
		vuint16				mFieldIndex;
		vuint16				mJoinColumnIndex;
		
		EOrder				mOrder;
};


/**********************************************************************************************/
/// Utility class that specifies Array of fields that must be used for Table sorting.
/// It is used in the Table_Sort() function.
///
/// Analog of ORDER BY in the SQL.
/// 
/// Note, that SortItems are used as simple pointers, we do not need here SmartPtrs.
// 
class SortOrder : public ArrayOfPtrs<SortItem*>
{
	public://///////////////////////////////////////////////////////////////////////////////////
 
				 			SortOrder( void ) : 
				 				ArrayOfPtrs<SortItem*>( 2, kOwnItems )
				 			{
				 			}
		
        					// We have array of SortItems.
                            // This method allows to check if some I_Field* is used by a SortItem.
		bool				Include( I_Field_Ptr inField )
							{
                                bool res = false;
                            
								for( SortItem* pItem : *this )
								{
									if( pItem->mpField == inField )
                                    {
										res = true;
                                        break;
                                    }
								}
                                
								return res;
							}
};


/**********************************************************************************************/
FBL_End_Namespace

#endif // _FBL_SortOrder_h
