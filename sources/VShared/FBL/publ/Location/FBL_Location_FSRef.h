/**********************************************************************************************/
/* FBL_Location_FSRef.h		                                                      			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef __FBL_Location_FSRef_h 
	#define __FBL_Location_FSRef_h
#pragma once

// FBL: 
#include <VShared/FBL/publ/Location/FBL_Location.h>

// FINAL: 
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
#if FBL_SUPPORT_FSREF


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_CLASS(Location_Disk_FSRef);


/**********************************************************************************************/
class FBL_SHARED_EXP_CLASS Location_Disk_FSRef : public Location
{
	public://///////////////////////////////////////////////////////////////////////////////////
 
		//------------------
		// Construction:
		
							// Expect Full path or just FileName. 
							// Note, that Location object makes a COPY of the given string !!!
							Location_Disk_FSRef( 
								const String* inFullPath = NULL );

//							Location_Disk_FSRef( 
//								const char* inFullPath_UTF8 );
							
	 						Location_Disk_FSRef( 
	 							HFSUniStr255* 	inName, 		// inName can be NULL.
	 							const FSRef* 	inObjectRef );	// Ref of an existed object.

	 						Location_Disk_FSRef( 
	 							const FSRef* 	inParentRef,
	 							HFSUniStr255& 	inName );

							Location_Disk_FSRef( 
								const Location_Disk_FSRef& inOther );

virtual						~Location_Disk_FSRef( void );
					
		Location_Disk_FSRef& operator=( const Location_Disk_FSRef& inOther );


	public://///////////////////////////////////////////////////////////////////////////////////

// I_Location API:

virtual	const String&		get_Path( void ) const;
virtual	void				put_Path( const String& inFullPath );
virtual	void				put_Path( const UChar*  inFullPath );

virtual	bool				get_IsUndefined( void ) const;

virtual I_Location_Ptr		get_ChildLocation( const char*   inName ) const;
virtual I_Location_Ptr		get_ChildLocation( const UChar*  inName ) const;
virtual I_Location_Ptr		get_ChildLocation( const String& inName ) const;

virtual I_Location_Ptr		get_ParentDirectory( void ) const;

virtual I_Location_Ptr		Clone( void ) const 
								{ return new Location_Disk_FSRef(*this); }


	public://///////////////////////////////////////////////////////////////////////////////////

// I_Disk_Location API:

		//------------------
		// Properties:

virtual	const String&		get_FileName( void ) const;
	  		
virtual	bool				get_Exists( void ) const;
virtual	bool				get_IsDirectory( void ) const;


	public://///////////////////////////////////////////////////////////////////////////////////

// For MacOS FSSpec:

#if FBL_SUPPORT_FSSPEC

		const FSSpec*		get_FSSpec( void ) const;  
		void				put_FSSpec( const FSSpec* inSpec );

#endif // FBL_SUPPORT_FSSPEC


	public://///////////////////////////////////////////////////////////////////////////////////

// For MacOS FSSRef:

		const FSRef*		get_ParentFSRef( void ) const;  

		OSStatus			get_ObjectFSRef( FSRef* outRef ) const;  
		void				put_ObjectFSRef( const FSRef* inRef );


	private:////////////////////////////////////////////////////////////////////////////////////
 
mutable	bool 				mParentRefExists;
mutable	bool 				mObjectRefExists;

mutable	FSRef				mParentFSRef;
mutable	FSRef				mObjectFSRef;

		HFSUniStr255		mName;	// copy of the name of object, 
									// to be able re-create it after ThrowOut().
}; 


/**********************************************************************************************/
FBL_End_Namespace

#endif // FBL_SUPPORT_FSREF

#endif // __FBL_Location_FSRef_h
