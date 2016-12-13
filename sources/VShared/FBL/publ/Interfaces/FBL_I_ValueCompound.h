/**********************************************************************************************/
/* FBL_I_ValueCompound.h                                                   					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_ValueCompound_h
	#define _FBL_I_ValueCompound_h
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


/**********************************************************************************************/
// this file declarations:
//
SMART_INTERFACE(I_ValueCompound);


/**********************************************************************************************/
// Interface that defines specific API to work with a Value_Compound.
//
interface FBL_SHARED_EXP_CLASS I_ValueCompound : public I_Unknown
{

virtual 					~I_ValueCompound( void );

	// ---------------------
	// Properties:

		// <IsVariableLength> 
							/** Returns TRUE if this compound value has variable length.
                            	Usually this means that at least one sub-value is a String value.
                            	Variable length values are stored into index in different way.
                                They must use prefix length 2 bytes. */				
virtual	bool				get_IsVariableLength( void ) const = 0;
	

		// <SubValue> 
        					// Returns Nth subValue of this compound value.
virtual	I_Value_Ptr			get_SubValue( vuint16 inValueIndex ) const = 0;


		// <SubValueCount> 
        					// Returns the count of sub-values.
virtual vuint32				get_SubValueCount( void ) const = 0;
																
};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_I_ValueCompound_h
