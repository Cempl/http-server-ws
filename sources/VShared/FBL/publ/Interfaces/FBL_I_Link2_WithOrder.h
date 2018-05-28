/**********************************************************************************************/
/* FBL_I_Link2_WithOrder.h                                 	                   				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_Link2_WithOrder_h
	#define _FBL_I_Link2_WithOrder_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Sets/FBL_ArraySet.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// THIS module declarations:
//
SMART_INTERFACE( I_Link2_WithOrder );
 

/**********************************************************************************************/
// PROBLEM:
//
// Sometimes we need to keep order of items in the list.
// For example:  Document -> Paragraphs.
//
// Normally this is not easy task for relational DBMS,
// Because when we use PK-FK link, we specify only the fact of link of 2 records, but not order.
// in RBDMS world, developers use additional double/int field to specify order of sub-items,
// but this solution requires additional code, which is not trivial btw.
//
//
/**********************************************************************************************/
// So may be we can ivent something to resolve this task in Valentina DB? :-)
// And YES - we going to introduce BinaryLinkWithOrder!
//
// It is clear that BinaryLinkWithOrder have sense for 1:M and M:M links.
//	Realy, 1:1 cannot keep any order.
//
/**********************************************************************************************/
// Normal BinaryLink does not care about order of child records.
// We have something as Parent{456} -> Childs{3, 5, 8, 12, 16},
//	i.e. we keep list of child recIDs sorted.
//
// I_Link2 allows us to ADD child-items to the list using LinkRecords2( inLeft, inRight ).
// 	And remove pairs using UnlinkRecords2().
//
//
/**********************************************************************************************/
// IF we drop sorting in the child list, THEN we will be able to keep ORDER of items:
//	Parent{456} -> Childs{5, 16, 3, 12}
//
// I_Link2_WithOrder interface allows to assign the whole child-list and it will be stored as given,
// without sorting.
//
// For BinaryLinkWithOrder we still must be able to use
//	* I_Link2::LinkRecords() - it will append item to the end of list.
//  * I_Link2::UnlinkRecords() - it will remove pair from the list as usually,
//		just it will use linear search in the list to find that item.
//
//StubInterface
interface FBL_SHARED_EXP_CLASS I_Link2_WithOrder : public I_Unknown
{

virtual						~I_Link2_WithOrder( void );


	// --------------------
    // List Methods:
    
    						// Links inLeft parent record to inChildList of child records.
                            // @inChildList is stored as is, without sorting.
                            // If inLeft item have some list, it is totally replaced by this operation.
virtual	void				LinkRecords2(
								REC_ID 			inLeft,
								ArraySet_Ptr	inChildList	) = 0;

};


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_I_Link2_WithOrder_h
