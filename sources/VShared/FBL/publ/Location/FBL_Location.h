/**********************************************************************************************/
/* FBL_Location.h			                                                      			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_Location_h 
	#define _FBL_Location_h
#pragma once

// FBL: 
#include <VShared/FBL/publ/Headers/FBL.h>

// FINAL: 
#include <VShared/FBL/publ/Interfaces/FBL_I_Disk_Location.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
class String;


/**********************************************************************************************/
SMART_CLASS( Location );


/**********************************************************************************************/
/**
	Abstraction that incapsulates a description of the disk file location.

	It is very comfortable to pass the location from high-level -- user,
	to the lowest level -- File class, because we have the same API for all types of description.

	On Windows we work only with paths.
	On Linux we work only with POSIX paths.
	On MacOS X Macho we work only with POSIX paths.
	On MacOS CARBON we have: MacOS paths, FSSpec, FSRef.
*/
class FBL_SHARED_EXP_CLASS Location : 
	public I_Disk_Location,
	public noncopyable
{
		IMPLEMENT_UNKNOWN( Location )
		BEGIN_INTERFACE_TABLE(Location)
			IMPLEMENT_INTERFACE(I_Disk_Location)
			IMPLEMENT_INTERFACE(I_Location)
		END_INTERFACE_TABLE()

	public://///////////////////////////////////////////////////////////////////////////////////
 
		//------------------
		// Construction:

							/** Format of path is according to the current platofrm, e.g. 
									Windows		"C:\dbs\mydb.vdb"
									MacOS 		"Hdd:dbs:mydb.vdb"	
									POSIX 		"/dbs/mydb.vdb"	
							*/
							Location( const String* inFullPath = nullptr );
							Location( const UChar* inFullPath );
							
							Location( const Location& inOther );

virtual						~Location( void );
					
		Location&			operator=( const Location& inOther );


	public://///////////////////////////////////////////////////////////////////////////////////

// I_Location API:

virtual	const String&		get_Path( void ) const override;
virtual	void				put_Path( const String& inFullPath ) override;
virtual	void				put_Path( const UChar* inFullPath ) override;

virtual	bool				get_IsUndefined( void ) const override;

virtual I_Location_Ptr		get_ChildLocation( const char* inName ) const override;
virtual I_Location_Ptr		get_ChildLocation( const UChar* inName ) const override;
virtual I_Location_Ptr		get_ChildLocation( const String& inName ) const override;

virtual I_Location_Ptr		get_ParentDirectory( void ) const override;

virtual I_Location_Ptr		Clone( void ) const override;


	public://///////////////////////////////////////////////////////////////////////////////////

// I_Disk_Location API:

		//------------------
		// Properties:

virtual	const String&		get_FileName( void ) const override;

virtual	bool				get_IsDirectory( void ) const override;
virtual	void				put_IsDirectory( bool inValue ) override;
			
virtual	bool				get_Exists( void ) const override;


	public://///////////////////////////////////////////////////////////////////////////////////

#if FBL_SUPPORT_FSSPEC

// For MacOS FSSpec:

virtual	const FSSpec*		get_FSSpec( void ) const override				{ return NULL; }
virtual	void				put_FSSpec( const FSSpec* inSpec ) override		{ argused1(inSpec); }

#endif // FBL_SUPPORT_FSSPEC


	public://///////////////////////////////////////////////////////////////////////////////////

// THIS CLASS API:

static void					MakeNewAppLocation( void );


	protected:////////////////////////////////////////////////////////////////////////////////// 
 
mutable	String*				mpFullPath;	// Location in the string representation
		

	protected:////////////////////////////////////////////////////////////////////////////////// 
 
		enum EDirInfo { kUnknown, kDirectory, kFile };

		EDirInfo			mIsDirectory; /// Keeps the info about kind of location.
}; 


/**********************************************************************************************/
FBL_End_Namespace

#endif // _FBL_Location_h
