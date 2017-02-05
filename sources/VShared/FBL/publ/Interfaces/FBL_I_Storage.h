/**********************************************************************************************/
/* FBL_I_Storage.h 		                                                     				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_Storage_h 
	#define _FBL_I_Storage_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// IF database is DISK based then default table is also disk based.
// If database is RAM based then default table also is RAM based.
//
// And for disk based db there is sense sometimes create RAM-based TMP table, if it is small.
// Information about such table must NOT be stored in any disk structures. It can be listed 
// of course in RAM structures. So on system failure, this table simply disapper. 
//
// It seems there is no sense (never) for RAM-based db, create disk based tables. 
// 


/**********************************************************************************************/
SMART_INTERFACE(I_Storage);


/**********************************************************************************************/
// Abstraction of Storage -- an object that have files to be stored on Device.
// As Device we consider e.g HDD or RAM, i.e. something where we can store files.
// 
//StubInterface
interface FBL_SHARED_EXP_CLASS I_Storage : public I_Unknown
{
virtual 					~I_Storage( void );

	// ---------------------
	// Properties:

		// <Exists> [r/o]
							/** Return TRUE if this Storage already have files on device.
								After Create() must be TRUE.
								After ThrowOut() must be FALSE. */
virtnew bool				get_Exists( void ) const  = 0;  

		// <IsOpen> [r/o]
							/** Returns TRUE if storage is opened now. 
								Must be TRUE after Open(), must be FALSE, after Close(). */
virtnew	bool				get_IsOpen( void ) const = 0;
		
		// <IsReadOnly> [r/w]
							/** TRUE if Storage is read-only (located on locked volume, 
								its files marked as read-only or marked as RO by user. */
virtnew	bool				get_IsReadOnly( void ) const = 0;

virtnew	void				put_IsReadOnly( bool inValue ) = 0;

virtnew flength				get_Size( void ) const  = 0;

	// ---------------------
	// Storage methods:

							/** Creates files of this object on Device. 
								File is opened after Create().
								If such file already exists then Create() erase it 
								and create a new empty file.
							*/
virtnew	void				Create( void ) = 0;
		
							/** Opens files of this object on Device.
								@error ERR_FILE_OPEN - in case file already is opened.
							*/
virtnew void 				Open( void ) = 0;

							/** Closes files of this object.
								@error ERR_FILE_NOT_OPEN - in case file already is closed.
							*/
virtnew void 				Close( void ) = 0;

							/** Flushes files of object from Cache to Device if needed.
							*/
virtnew	void				Flush( void ) = 0;

							/** Removes files of this object from Device.
								@error xOSFileError{ ERR_FILE_OPEN } - file is opened.
								@error xInternalError{ ERR_INTERNAL_EMBFILE_IS_OPENED }
							*/
virtnew void 				ThrowOut( void ) = 0;

}; 


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_I_Storage_h
