/**********************************************************************************************/
/* FBL_I_FldBlob.h 		                                                      				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_FldBlob_h 
	#define _FBL_I_FldBlob_h

#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_OStream.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Disk_Location.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE(I_FldBlob);


/**********************************************************************************************/
/** 
	Interface of BLOB Class.
*/
//StubInterface
interface FBL_SHARED_EXP_CLASS I_FldBlob : public I_Unknown
{

virtual						~I_FldBlob( void );
		
	// ---------------------
	// Properties:
	
		// <SegmentSize> [r/w]
virtual	vuint32				get_SegmentSize( void ) const  = 0;

							/// Must be inited before the database files are created.
virtual	void				put_SegmentSize( vuint32 inSegmentSize ) = 0;

 		// <Compressed>	[r/w]	
 							/** TRUE if the field must use compression for its data. */
virtual	bool				get_Compressed( void ) const  = 0; 
virtual	void				put_Compressed( bool inValue ) = 0; 
		
		// <DataSize> [r/w]
							/** Returns the size of the data in the current BLOB */
virtual vuint32				get_DataSize( void ) const = 0;

							/** Set the length of the BLOB record.
								Can as reduce it, so increase. */
virtual void				put_DataSize( vuint32 inValue ) = 0;
		
		// <FirstSegment> [r/o]			
							/** Returns the index of the first segment of the 
								current BLOB value. */
virtual vuint32				get_FirstSegment( void ) const  = 0; 


	// ---------------------
	// Value Access:

							/// Deletes a BLOB-record, reference in the table record become NULL.
virtual void				DeleteData( void ) = 0;							
													
							/** Reads from the disk to the buffer an accociated BLOB record if any.
								Returns the number of bytes actually readed.*/
virtual vuint32				ReadData( 
								char* inBuffer, 
								vuint32 inBytes ) const = 0;

							/// Adds a new BLOB record and store its reference in the Table.
virtual void				WriteData( 
								const char* inBuffer, 
								vuint32 		inBytes ) = 0;
		
		
	// ---------------------
	// Disk file storing:

							/// Restores from BLOB field a disk file into specified location. 
virtual void 				ToFile( I_Disk_Location_Ptr inLocation ) = 0;
		
							/// Stores a disk file at specified location into BLOB field.
virtual void 				FromFile( I_Disk_Location_Ptr inLocation ) = 0;


	// ---------------------
	// Features:

							/**	Compacts data. 
								Segments at the end of file are moved to deleted segments.
								@param inCount	- the count of compact operations,
									if we put 0 - All data will be compacted.
								@returns - true if BLOB is compacted. */
virtual bool 				Compact	( vuint32 inCount = 0 ) = 0;
		
							/// Defragment BLOB records. All segments of the records 
							/// must go one for one.
virtual void 				Defragment	( void ) = 0;


	public://///////////////////////////////////////////////////////////////////////////////////
	
#if FBL_TEST_CODE

	// ---------------------
	// TEST helpers:
	// We need this to build automated tests.

virtual	void				Dump( I_OStream& inOut ) = 0;

#endif // FBL_TEST_CODE

}; 


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif /// _FBL_I_FldBlob_h
