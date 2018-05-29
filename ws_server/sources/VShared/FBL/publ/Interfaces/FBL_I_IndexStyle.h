/**********************************************************************************************/
/* FBL_I_IndexStyle.h 	                                                   					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_IndexStyle_h 
	#define _FBL_I_IndexStyle_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// THIS module declarations:
//
SMART_INTERFACE(I_IndexStyle);

SMART_ARRAY_OF_SMART_PTRS( ArrayOfIndexStyles, I_IndexStyle_Ptr );
SMART_ARRAY_OF_CLASSES( ArrayOfIndexStylesPointers, I_IndexStyle* );

/**********************************************************************************************/
/**
	If some Vstring or VarChar or VText field has option IndexByWords is TRUE,
	then this class can specify additional parameters for its index.		
	This parameters allow to developer/user better control the full text indexing.
	
	This parameters have no affect if option IndexByWords is FALSE.
*/
//StubInterface
interface FBL_SHARED_EXP_CLASS I_IndexStyle : public I_Unknown
{

virtual 					~I_IndexStyle( void );	


	// ---------------------
	// Properties:

virtual	IndStyle_ID			get_ID( void ) const  = 0;

		// <Name>

							//! returns the name of this index style.
virtual	const String&		get_Name( void ) const  = 0; //OLDKEEPAS: mName

		// <IsTemporary>
							/**	Returns TRUE if this style is tmp and must not be saved into 
								database schema. */
virtual bool				get_IsTemporary( void ) const  = 0;

							//! @error NameNotUnique. 
virtual	void				put_Name( const String& inNewName ) = 0;

};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_I_IndexStyle_h
