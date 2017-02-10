/**********************************************************************************************/
/* FBL_I_Link_2.h		                                 	                   				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_Link2_h
	#define _FBL_I_Link2_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// forward declarations: 
//
SMART_INTERFACE(I_Table);

SMART_CLASS( Set );
SMART_CLASS( ArraySet );
SMART_CLASS( BitSet );


/**********************************************************************************************/
// THIS module declarations:
//
SMART_INTERFACE( I_Link2 );
 

/**********************************************************************************************/
// I_Link2 is an interface that provides abstraction of LINK between 2 tables of database.
//
// A Link that have I_Link2 interface ALWAYS have I_Link interface also.
// I_Link2 interface just provides much more comfortable analogs of I_Link methods,
// because often there is no need to specify tables A and B if link is between 2 tables only.
// And in real life 95% of links are such - between two tables only.	
//
//StubInterface
interface FBL_SHARED_EXP_CLASS I_Link2 : public I_Unknown
{

virtual						~I_Link2( void );

	// ---------------------
	// Properties:

		// <LeftType> [r/o]
							/** Returns the relation type for the left branch. 
								Can be 1 (kOne) or M (kMany). */
virtual	LinkType			get_LeftType( void ) const 									= 0;

		// <RightType> [r/o]
							/** Returns the relation type for the right branch.
								Can be 1 (kOne) or M (kMany). */
virtual	LinkType			get_RightType( void ) const 								= 0;


		// <IsRecursive> [r/o]
                            /** Returns TRUE if this link between 2 tables is recursive,
                            	i.e. in fact it is made on the single table.  */
virtual bool				get_IsRecursive( void ) const 								= 0;


	// ---------------------
	// Record Methods:

							/** Links given records of 2 Tables */
virtual void				LinkRecords2( REC_ID inLeft, REC_ID inRight )   = 0; //KEEPAS: I_Link2_LinkRecords_REC_ID_REC_ID

							/** Unlinks given records of 2 Tables */
virtual void				UnlinkRecords2( REC_ID inLeft, REC_ID inRight ) = 0; //KEEPAS: I_Link2_UnlinkRecords_REC_ID_REC_ID


	// ---------------------
	// Not-Recursive Link Methods:

							/**	Returns the count of linked records in the direction
								1 -> Many. */
virtual	vuint32				CountLinkedFrom1ToMany( REC_ID inRecID ) const				= 0;

							/**	Returns the count of linked records in the direction 
								Many -> 1. */
virtual vuint32				CountLinkedFromManyTo1( REC_ID inRecID ) const				= 0;

							/**	Returns the records from "One"-table linked to 
								inRecID from "Many"-table. */
virtual ArraySet_Ptr		FindLinkedFrom1ToMany( REC_ID inRecID ) const				= 0;

							/**	Returns the records from "Many"-table linked to 
								inRecID from "One"-table. If not have linked eturns NULL. */
virtual ArraySet_Ptr		FindLinkedFromManyTo1( REC_ID inRecID ) const				= 0;

							/**	Returns all the linked records from "Many"-table. */
virtual BitSet_Ptr			FindAllLinkedInManyTable( void ) const						= 0;

							/**	Returns all the linked records from "One"-table. */
virtual BitSet_Ptr			FindAllLinkedIn1Table( void ) const							= 0;


	// ---------------------
	// Recursive Search Methods on RecID:

							/** For recursive link only. Find parents for inRecID on inLevel level
								If inOnlyThatLevel == true - the only this level records included.*/
virtual ArraySet_Ptr		FindAncestors( 
								REC_ID				inRecID, 
								vuint32				inLevel = vuint32_max, 
								bool				inOnlyThatLevel = false,
								bool				inIncludingStartPoint = false ) const = 0;

							/** For recursive link only. Find brothers for inRecID on inLevel level
                                If inOnlyThatLevel == true - the only this level records included.
                                Siblings	- level 1 brothers.
                                Cousins		- level2 brothers... */
virtual ArraySet_Ptr		FindBrothers( 
								REC_ID				inRecID, 
								vuint32				inLevel = 1, 
								bool				inOnlyThatLevel = false,
								bool				inIncludingStartPoint = false ) const = 0;

							/** For recursive link only. Find children for inRecID on inLevel level
								If inOnlyThatLevel == true - the only this level records included.*/
virtual ArraySet_Ptr		FindDescendants( 
								REC_ID				inRecID, 
								vuint32				inLevel = vuint32_max, 
								bool				inOnlyThatLevel = false,
								bool				inIncludingStartPoint = false ) const = 0;


	// ---------------------
	// Recursive Search Methods on Set:

							/** For recursive link only. Find children for inRecID on inLevel level
								If inOnlyThatLevel == true - the only this level records included. */
virtual ArraySet_Ptr		FindDescendants( 
								Set_Ptr 			inSet,
								vuint32				inLevel = vuint32_max, 
								bool				inOnlyThatLevel = false,
								bool				inIncludingStartPoint = false ) const = 0;

							/** For recursive link only. Find parents for inRecID on inLevel level
								If inOnlyThatLevel == true - the only this level records included. */
virtual ArraySet_Ptr		FindAncestors( 
								Set_Ptr 			inSet,
								vuint32				inLevel = vuint32_max, 
								bool				inOnlyThatLevel = false,
								bool				inIncludingStartPoint = false ) const = 0;

							/** For recursive link only. Find brothers for inRecID on inLevel level
                                If inOnlyThatLevel == true - the only this level records included.
                                Siblings	- level 1 brothers.
                                Cousins		- level2 brothers... */
virtual ArraySet_Ptr		FindBrothers( 
								Set_Ptr 			inSet,
								vuint32				inLevel = 1, 
								bool				inOnlyThatLevel = false,
								bool				inIncludingStartPoint = false ) const = 0;


	// ---------------------
	// Copy Links Methods:
    //
    // These methods allow to do REFACTORING of a link from one kind to another WHEN they have RECORDS!
    // i.e. You can convert RDB-link into ObjectPtr or BinaryLink.
	// Read details:
    //	http://www.valentina-db.com/dokuwiki/doku.php?id=valentina:vcomponents:vkernel:vlink:refactoring:refactoring
	// Note, that exists SQL analogs of these API commands:
    //	https://www.valentina-db.com/dokuwiki/doku.php?id=valentina:vcomponents:vsql:reference:copy_links
    
							/** Copy "links" from source link to this one. */
virtual	void				CopyLinksFrom( 
                                I_Link2_Ptr inpSourceLink ) = 0;

							/** This link must be M::M.
								Copy "links" from source links pair to this one. */
virtual	void				CopyLinksFrom( 
								I_Link2_Ptr inpSourceLink1,
								I_Link2_Ptr inpSourceLink2 ) = 0;

							/** This link must be M::M.
								Copy this M::M "links" to target links pair. */
virtual	void				CopyLinksTo( 
								I_Link2_Ptr inDestinationLink1,
								I_Link2_Ptr inDestinationLink2 ) = 0;

	// ---------------------
	// Helper Methods:

							/**	Determines the type of link between two tables.
								Both tables must belong to this link otherwise
								result will be kInvalidLinkDirection. */
virtual ELinkDirection		DetectLinkDirection(
								Const_I_Table_Ptr inTableA, 
								Const_I_Table_Ptr inTableB ) const = 0;

							/**	Returns "One"-table if @inTable points to "Many"-table or
								returns "Many"-table if @inTable points to "One"-table or
								returns NULL otherwise. */
virtual	I_Table_Ptr			get_OppositeTable( Const_I_Table_Ptr inTable ) const  = 0;


};


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_I_Link2_h
