/**********************************************************************************************/
/* FBL_I_Location.h 	                                                      				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_Location_h 
	#define _FBL_I_Location_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace
SMART_INTERFACE(I_Location);


/**********************************************************************************************/
/**
	Interface that implements cross-platform abstraction of File Location.
	
	We can have different implementations of Location:
	* Local Disk (HDD) location (can differ from differnet platforms).
	* Remote Disk (HDD) location
	* URL location
	* RAM loation of file.
	
	All these implementations allow provide location as STRING. 
	We use this fact to build this abstraction.
*/
interface FBL_SHARED_EXP_CLASS I_Location : public I_Unknown
{

virtual						~I_Location( void );


							// virtual ctor.
virtual I_Location_Ptr		Clone( void ) const  = 0;	


	//------------------
	// Properties:

		// <Path> 
							/** The full path of this location.							
								It can be:	
									c:/Databases/my_db.vdb 										
									file:///c:/Databases/my_db.vdb 
									http://www.youserver.com:15432/my_db or	
							*/
virtual	const String&		get_Path( void ) const  = 0;	//OLDKEEPAS: mPath
virtual	void				put_Path( const UChar* inFullPath )  = 0;
virtual	void				put_Path( const String& inFullPath )  = 0;


		// <IsUndefined>
							/// Returns TRUE if this location object do not have specified yet. 
virtual	bool				get_IsUndefined( void ) const  = 0; 

							/** If this Location object points to a directory then
								returns a new Location object that corresponds to the child 
								directory withing the current directory. Otherwise returns NULL. 
								If child directory with that name does not exists returns NULL. 
								Attempt to call of this func for FILE will throw ERR_NOT_A_FOLDER 
								error. */

virtual I_Location_Ptr		get_ChildLocation( const char* inName ) const = 0;
virtual I_Location_Ptr		get_ChildLocation( const UChar* inName ) const = 0;
virtual I_Location_Ptr		get_ChildLocation( const String& inName ) const = 0;


							/** If this Location object points to a directory then
								returns parent directory.
								If this Location object points to a ROOT directory then
								returns just Clone of this Location object.
								If this Location object points to a file then returns
								its parent dorectory. */
virtual I_Location_Ptr		get_ParentDirectory( void ) const = 0;

};


/**********************************************************************************************/
FBL_End_Namespace


#include <VShared/FBL/publ/Headers/FBL_post_header.h>


#endif // _FBL_I_Location_h
