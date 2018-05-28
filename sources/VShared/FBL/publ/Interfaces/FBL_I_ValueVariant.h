/**********************************************************************************************/
/* FBL_I_ValueVariant.h                                                    					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_ValueVariant_h
	#define _FBL_I_ValueVariant_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Interfaces/FBL_I_Value.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// forward declarations:
//
SMART_INTERFACE(I_Value);
SMART_INTERFACE(I_Database);


/**********************************************************************************************/
// this file declarations:
//
SMART_INTERFACE(I_ValueVariant);


/**********************************************************************************************/
/**
	Interface class that specify API of work with Variant value.

*/
interface FBL_SHARED_EXP_CLASS I_ValueVariant : public I_Unknown
{

virtual 					~I_ValueVariant( void );

	// ---------------------
	// Value Methods:
	
							/** Returns a particular value type,
								 which this ValueVariant contains at the moment. */
virtual vuint32				get_VariantValueType( void ) const = 0;
	
							/** Return particular value which this ValueVariant points 
								at the moment. */
virtual I_Value_Ptr			get_VariantValue( void ) const  = 0;

							/** Set particular value to this ValueVariant.
								Throws error in case of unsupported inpValue type. */
virtual	void				put_VariantValue( Const_I_Value_Ptr inpValue ) = 0;


};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_I_ValueVariant_h
