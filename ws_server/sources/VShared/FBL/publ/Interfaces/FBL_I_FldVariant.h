/**********************************************************************************************/
/* FBL_I_FldVariant.h 		                                                      			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_FldVariant_h
	#define _FBL_I_FldVariant_h

#include <VShared/FBL/publ/Headers/FBL.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE(I_FldVariant);


/**********************************************************************************************/
/** 
	Interface of Variant Class.
*/
//StubInterface
interface FBL_SHARED_EXP_CLASS I_FldVariant : public I_Unknown
{

virtual						~I_FldVariant( void );

	// ---------------------
	// Properties:
	
 		// <Compressed>	[r/w]
 							/** TRUE if the field must use compression for its data. */
virtual	bool				get_Compressed( void ) const = 0;
virtual	void				put_Compressed( bool inValue ) = 0;
	
};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif /// _FBL_I_FldVariant_h
