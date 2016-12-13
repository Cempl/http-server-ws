/**********************************************************************************************/
/* FBL_I_Link.h 		                                                      				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_Link_h 
	#define _FBL_I_Link_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Interfaces/FBL_I_Table.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_PropertyContainer.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_Storage.h>


// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// forward declarations: 
//
SMART_INTERFACE( I_ForeignPlugin );

SMART_CLASS( BitSet );
SMART_CLASS( ArraySet );


/**********************************************************************************************/
// THIS module declarations:
//
SMART_INTERFACE( I_Link );


/**********************************************************************************************/
SMART_ARRAY_OF_SMART_PTRS( ArrayOfLinks, I_Link_Ptr );
SMART_ARRAY_OF_CLASSES( ArrayOfLinkPointers, I_Link* );


/**********************************************************************************************/
/**
	I_Link is an interface that provides abstraction of LINK between Tables of database.

	* BranchCount property.
	Link always relate at least 2 Tables. It can relate more records.

	Link register self in each related table. So a Table instance is aware about related tables
	and can iterate links to do some work.

	* Name property. 
	Important to note that the name of link may depends from DIRECTION we consider: Ta->Tb or Tb->Ta 
*/

//StubInterface
interface FBL_SHARED_EXP_CLASS I_Link : public I_Storage
{

virtual 					~I_Link( void );

	// ---------------------
	// Properties:

		// <BranchCount>
							/** Returns the number of branches for this link. */
virtual vuint16				get_BranchCount( void ) const 					= 0; 

		// <ID> [r/o]
							/// ID of link. Tmp link has negative ID.
virtual	Link_ID				get_ID( void ) const  = 0; 

		// <IsBetween> [r/o]
							/** Returns true if links both specified Tables. */
virtual	bool				get_IsBetween( 
								Const_I_Table_Ptr inTableA, 
								Const_I_Table_Ptr inTableB ) const 			= 0;

virtual I_Database_Ptr		get_Database( void ) const 						= 0;

		// <IsStatic> [r/o]	//! Returns TRUE if this link is static, i.e. it was created 
							//! before Create()/Open() call. Link self determinate
							//! that it is static and store this info.
virtual bool				get_IsStatic( void ) const  = 0; 

		// <IsSystem>
		/// Returns TRUE if this is system Link.
virtual bool				get_IsSystem( void ) const  = 0; 

		// <IsTemporary>
							/**	Returns TRUE if this link is tmp and must not be saved into 
								database schema. */
virtual bool				get_IsTemporary( void ) const 					= 0;

		// <Kind>
							/**	Returns the link type.*/
virtual ELinkKind			get_Kind( void ) const 							= 0;

		// <Name> 
							/** Returns the name of link. */
virtual	const String&		get_LinkName( void ) const 						= 0; //OLDKEEPAS: mLinkName


							/** Returns the name of link. */
virtual	void				put_LinkName( const String& inName ) 			= 0;


		// <OnDelete> [r/w]
					
virtual	EOnDeletion			get_OnDelete( void ) const 						= 0;
virtual	void				put_OnDelete( EOnDeletion inValue )				= 0;

		// <OnUpdate> [r/w]
virtual	EOnUpdate			get_OnUpdate( void ) const 						= 0;
virtual	void				put_OnUpdate( EOnUpdate inValue )				= 0;


	// ---------------------
	// Tables properties:
	
	
		// <Table> [r/o]	
							/**	Returns table used in link by index. 
								Note: for ObjectPtr the first is pointed table, 
								the second is ptr(s) table(s).*/
virtual	I_Table_Ptr			get_Table( vuint32 inIndex ) const  			= 0;

		// <Owner>
							///	get/set owner of the link.
virtual	I_Table_Ptr			get_Owner( void ) const  						= 0;
virtual	void				put_Owner( I_Table_Ptr inOwner ) 				= 0;

		// <Count>
							/// Returns count of all associations (linked pairs)
virtual vuint32				get_Count( void ) 								= 0;


	// ---------------------
	// Search methods:

							/// Returns TRUE if records are linked. 
virtual	bool				get_IsLinked( REC_ID inKeys[] ) const 			= 0;

	
							/**	Returns the count of linked recods in the direction from
								inTableA to inTableB. */
virtual	vuint32				CountLinked( 
								REC_ID				inRecID,
								Const_I_Table_Ptr	inTableA, 
								Const_I_Table_Ptr	inTableB, 
								ERecursionDirection inDir = kFromParentToChild ) const 	= 0;

							/**	Returns the records from inTableB linked to inRecID from 
								inTableA. If not have linked returns NULL. */
virtual	ArraySet_Ptr		FindLinked( 
								REC_ID				inRecID,
								Const_I_Table_Ptr	inTableA, 
								Const_I_Table_Ptr	inTableB, 
								ERecursionDirection inDir = kFromParentToChild ) const	= 0;

							/**	Returns the records from inTableB linked to inRecID from
								inTableA and only to it. If not have linked returns NULL. */
virtual	ArraySet_Ptr		FindExclusivelyLinked( 
								REC_ID				inRecID,
								Const_I_Table_Ptr	inTableA, 
								Const_I_Table_Ptr	inTableB, 
								ERecursionDirection inDir = kFromParentToChild ) const	= 0;

							/**	Returns the all records from inTableB linked to inTableA.
								If not have linked returns NULL. */
virtual	BitSet_Ptr			FindAllLinked( 
								Const_I_Table_Ptr	inTableA, 
								Const_I_Table_Ptr	inTableB, 
								ERecursionDirection inDir = kFromParentToChild ) const	= 0;

							/**	Returns a BitSet of records from table inTableB linked to 
								ANY record in inSetA from inTableA. */
virtual BitSet_Ptr			FindLinkedAsBitSet( 
								Set_Ptr 			inSetA,
								Const_I_Table_Ptr 	inTableA,
								Const_I_Table_Ptr 	inTableB, 
								ERecursionDirection inDir = kFromParentToChild ) const	= 0;


	// ---------------------
	// Recursive Search Methods:

							/** For recursive link only. Find children for inRecID on inLevel level
								If inOnlyThatLevel == true - the only this level records included.
								inIncludingStartPoint - inRecID will be added to result. */
virtual ArraySet_Ptr		FindDescendants( 
								REC_ID				inRecID, 
								Const_I_Table_Ptr	inTableA,
								vuint32				inLevel = vuint32_max, 
								bool				inOnlyThatLevel = false,
								bool				inIncludingStartPoint = false )  const = 0;

							/** For recursive link only. Find parents for inRecID on inLevel level
								If inOnlyThatLevel == true - the only this level records included.
								inIncludingStartPoint - inRecID will be added to result.*/
virtual ArraySet_Ptr		FindAncestors( 
								REC_ID				inRecID, 
								Const_I_Table_Ptr	inTableA,
								vuint32				inLevel = vuint32_max, 
								bool				inOnlyThatLevel = false,
								bool				inIncludingStartPoint = false)  const = 0;

							/** For recursive link only. Find brothers for inRecID on inLevel level
								If inOnlyThatLevel == true - the only this level records included.
								Siblings	- level 1 brothers.
								Cousins		- level2 brothers....
								inIncludingStartPoint - inRecID will be added to result.
								*/
virtual ArraySet_Ptr		FindBrothers( 
								REC_ID				inRecID, 
								Const_I_Table_Ptr	inTableA,
								vuint32				inLevel = 1, 
								bool				inOnlyThatLevel = false,
								bool				inIncludingStartPoint = false )  const = 0;

							/** For recursive link only. Find children for inSet on inLevel level
							If inOnlyThatLevel == true - the only this level records included.
							inIncludingStartPoint - inSet will be added to result. */
virtual ArraySet_Ptr		FindDescendants( 
								Set_Ptr 			inSet,
								Const_I_Table_Ptr	inTableA,
								vuint32				inLevel = vuint32_max, 
								bool				inOnlyThatLevel = false,
								bool				inIncludingStartPoint = false )  const = 0;

							/** For recursive link only. Find parents for inSet on inLevel level
							If inOnlyThatLevel == true - the only this level records included.
							inIncludingStartPoint - inSet will be added to result.*/
virtual ArraySet_Ptr		FindAncestors( 
								Set_Ptr 			inSet,
								Const_I_Table_Ptr	inTableA,
								vuint32				inLevel = vuint32_max, 
								bool				inOnlyThatLevel = false,
								bool				inIncludingStartPoint = false)  const = 0;

							/** For recursive link only. Find brothers for inSet on inLevel level
							If inOnlyThatLevel == true - the only this level records included.
							Siblings	- level 1 brothers.
							Cousins		- level2 brothers....
							inIncludingStartPoint - inSet will be added to result.
							*/
virtual ArraySet_Ptr		FindBrothers( 
								Set_Ptr 			inSet,
								Const_I_Table_Ptr	inTableA,
								vuint32				inLevel = 1, 
								bool				inOnlyThatLevel = false,
								bool				inIncludingStartPoint = false )  const = 0;


	// ---------------------
	// Link/unlink methods:

							/**	Adds to the link a new record with given set of keys.
								Array must contains right count of keys,
								in the order of branches. */
virtual void				LinkRecords( REC_ID inKeys[] )							= 0;

							/** Removes from the link record which contains given set of keys. 
								Existed Table records are not deleted. */
virtual void				UnlinkRecords( REC_ID inKeys[] )						= 0;

							/** Version of method for recursive link on single table. */
virtual void				UnlinkRecords( 
								REC_ID 				inRecID,
								Const_I_Table_Ptr	inTableA,
								ERecursionDirection inDir = kFromParentToChild )	= 0;


	// ---------------------
	// Link modification methods:

							/** Deletes all records linked to the inRecID of inTableA.
								Record inRecID of inTableA is not deleted by this function.
								
								Actually the link can 
									{refuse, delete some records, update some records}								
								depending on the DeletionControl. 
								
								inOnDeletion == true if we unlink records in deletion process.
								In this case we skip zero filling, for speedup.

								ERRORS: errRestrict. */
virtual void 				DeleteLinkedRecords( 
								REC_ID 				inRecID,
								Const_I_Table_Ptr	inTableA,
								bool				inOnDeletion = false,
								ERecursionDirection inDir = kFromParentToChild )	= 0;

							/** Deletes all records linked to any record of inTableA.
								
								Actually the link can 
									{refuse, delete some records, update some records}.
								depending on the DeletionControl. 	
								
								inOnDeletion == true if we unlink records in deletion process.
								In this case we skip zero filling, for speedup.
								
								ERRORS: errRestrict. */
virtual void 				DeleteAllLinkedRecords( 
								Const_I_Table_Ptr	inTableA,
								bool				inOnDeletion = false,
								ERecursionDirection inDir = kFromParentToChild ) = 0;

							/**	This method updates the linked records depending on OnUpdateBehavior 
								in the direction from inTableA. */
virtual void				UpdateLinkedRecords	( 
								REC_ID		inKey,
								I_Table_Ptr	inTableA,
								bool		inNoCheck = false,
								ERecursionDirection inDir = kFromParentToChild )	= 0;


	// ---------------------
	// Check methods:

							/**	Checks the possibility to link the current record 
								from ptr table to key table. */
virtual	bool				IsUpdateAllowedFor( 
								Const_I_Table_Ptr	inTable,
								REC_ID				inRecID,
								ERecursionDirection inDir = kFromParentToChild ) const = 0;

							/**	Returns false if at link doesn't allow deletion of 
								record for this inTable. If inRecID is 0 - use current record
								of inTable. */
virtual	bool				IsDeletionAllowedFor( 
								Const_I_Table_Ptr	inTable,
								REC_ID				inRecID,
								ERecursionDirection inDir = kFromParentToChild ) const	= 0;



	// ---------------------
	// Utility methods:
							/// Flushes files of Link from Cache to Device if needed.
							/// If inFlushTables then linked tables will be flushed also.
virtual void 				FlushLink( bool inFlushTables = true ) = 0;

virtual void 				Compact( void ) = 0;
virtual void 				Defragment( void ) = 0;

virtual bool 				Diagnose( 
								I_OStream_Ptr inOut,
								EVerboseLevel inLevel = kVerbose_Normal ) = 0;

virtual bool 				Diagnose( 
								I_Location_Ptr	inReportLocation,
								EVerboseLevel	inLevel = kVerbose_Normal ) = 0;

virtual void 				Repair( void ) = 0;


	public://///////////////////////////////////////////////////////////////////////////////////
 
							// to Resolve problem in plugins with ObjectPtr field,
							// which is both a link and a field. This function allows us to have 
							// for ObjectPtr the second instance of ForeignPlugin and access it.
							// dynamic_cast<> will return ForeignPlugin for Field.
							// This method will return for Link. 
virtual I_ForeignPlugin_Ptr	get_ForeignPlugin( void )  = 0; 

}; 



#pragma mark -
#pragma mark === ArrayOfLinks algorithms ===


/**********************************************************************************************/
I_Link_Ptr FindLink( 
	const ArrayOfLinks_Ptr	inLinks, 
	const String&			inLinkName	);
	

/**********************************************************************************************/
I_Link_Ptr FindLink( 
	const ArrayOfLinkPointers_Ptr inLinks, 
	const String&			inLinkName	);


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_I_Link_h
