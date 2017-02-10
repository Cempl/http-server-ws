/**********************************************************************************************/
/* FBL_Location_Disk_FSSpec.h                                                      			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_Location_Disk_FSSpec_h 
	#define _FBL_Location_Disk_FSSpec_h
#pragma once

#include <VShared/FBL/publ/Location/FBL_Location.h>

// FINAL: 
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
#if FBL_SUPPORT_FSSPEC


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
FBL_SHARED_EXP extern bool gUseCarbon;


/**********************************************************************************************/
SMART_CLASS(Location_Disk_FSSpec);


/**********************************************************************************************/
class FBL_SHARED_EXP_CLASS Location_Disk_FSSpec : public Location
{
	public://///////////////////////////////////////////////////////////////////////////////////
 
		//------------------
		// Construction:
		
							/** Expect Full path or just FileName. */
							Location_Disk_FSSpec( const String* inFullPath = NULL );

#if FBL_MAC
	 						Location_Disk_FSSpec( const FSSpec* inSpec ) : Location( (UChar*) NULL )
	 							{ put_FSSpec( inSpec );	}	
#endif // FBL_MAC

							Location_Disk_FSSpec( const Location_Disk_FSSpec& inOther );

virtual						~Location_Disk_FSSpec( void );
					
		Location_Disk_FSSpec& operator=( const Location_Disk_FSSpec& inOther );


	public://///////////////////////////////////////////////////////////////////////////////////

// I_Location API:

virtual	const String&		get_Path( void ) const;
virtual	void				put_Path( const String& inFullPath );
virtual	void				put_Path( const UChar* inFullPath );

virtual	bool				get_IsUndefined( void ) const;

virtual I_Location_Ptr		get_ChildLocation( const char* inName ) const;
virtual I_Location_Ptr		get_ChildLocation( const String& inName ) const;

virtual I_Location_Ptr		get_ParentDirectory( void ) const;

virtual I_Location_Ptr		Clone( void ) const 
								{ return new Location_Disk_FSSpec(*this); }


	public://///////////////////////////////////////////////////////////////////////////////////

// I_Disk_Location API:

		//------------------
		// Properties:

virtual	const String&		get_FileName( void ) const;
			
virtual	bool				get_Exists( void ) const;
virtual	bool				get_IsDirectory( void ) const;


	public://///////////////////////////////////////////////////////////////////////////////////

// For MacOS FSSpec:

#if FBL_MAC
		const FSSpec*		get_FSSpec( void ) const;  
		void				put_FSSpec( const FSSpec* inSpec );
#endif //FBL_MAC

	private:////////////////////////////////////////////////////////////////////////////////////
 
#if FBL_MAC
mutable	FSSpec				mFSSpec;
mutable	bool 				mSpecExists;
#endif //FBL_MAC

}; 


/**********************************************************************************************/
FBL_End_Namespace

#endif // FBL_SUPPORT_FSSPEC

#endif // _FBL_Location_Disk_FSSpec_h
