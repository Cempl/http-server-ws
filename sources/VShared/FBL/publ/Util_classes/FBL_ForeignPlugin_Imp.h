/**********************************************************************************************/
/* FBL_ForeignPlugin_Imp.h                                                     				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_ForeignPlugin_Imp_h 
	#define _FBL_ForeignPlugin_Imp_h
#pragma once

#include <VShared/FBL/publ/Interfaces/FBL_I_ForeignPlugin.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
class FBL_SHARED_EXP_CLASS ForeignPlugin_imp : 
	public I_ForeignPlugin
{
		// imp - do not make IUNKNOWN

	public://///////////////////////////////////////////////////////////////////////////////////

							ForeignPlugin_imp( void );

virtual		 				~ForeignPlugin_imp( void );
				
					
	public://///////////////////////////////////////////////////////////////////////////////////

virtual	void*				get_Xtra( void ) const override  			{ return mXtraData; }									
virtual	void				put_Xtra( void* inXtraData ) override  	{ mXtraData = inXtraData; } 

virtual void**				get_BackRef( void ) const override  		{ return mBackRef; }
virtual	void				put_BackRef( void** inBackRef ) override  { mBackRef = inBackRef; }
	
virtual void				put_Finalize( FINALAIZE_PTR inFunc ) override  { mpFinalize = inFunc; }

	private:////////////////////////////////////////////////////////////////////////////////////

		void**				mBackRef;
		void*				mXtraData;	
		FINALAIZE_PTR		mpFinalize;
};


/**********************************************************************************************/
// We need this class for ObjectPtr implementation,
// because ForeignPlugin_imp is abstract class.
//
class FBL_SHARED_EXP_CLASS ForeignPlugin : 
	public ForeignPlugin_imp
{
		IMPLEMENT_UNKNOWN(ForeignPlugin)											
		BEGIN_INTERFACE_TABLE(ForeignPlugin)
			IMPLEMENT_INTERFACE(I_ForeignPlugin)
		END_INTERFACE_TABLE()

	public://///////////////////////////////////////////////////////////////////////////////////

							ForeignPlugin( void )
							{
							}

virtual		 				~ForeignPlugin( void )
							{
							}				
};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_ForeignPlugin_Imp_h
