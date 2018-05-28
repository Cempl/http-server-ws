/**********************************************************************************************/
/* FBL_I_ValueApproximate.h                                                      			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_ValueApproximate_h
	#define _FBL_I_ValueApproximate_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Interfaces/FBL_I_Value.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE(I_ValueApproximate);
SMART_INTERFACE(I_Value);


/**********************************************************************************************/
// Interface class -- defines I_ValueApproximate API.
// 
interface FBL_SHARED_EXP_CLASS I_ValueApproximate : public I_Unknown
{

virtual 					~I_ValueApproximate( void );


	// Example: 123.45
	// Precision is 5 (all digits count).
	// Scale is 2 (digits count right after decimal point)

	// ---------------------
	// Properties:
	
		// <Precision> [r/w]
							/// Get/put the precision for this value.
virtual	vuint16				get_Precision( void ) const									= 0;
virtual	void				put_Precision( vuint16 inPrecision )						= 0;
	
		// <Scale> [r/w]
							/// Get/put the scale of value.
virtual	vuint16				get_Scale( void ) const										= 0;
virtual	void				put_Scale( vuint16 inScale )								= 0;


}; 


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_I_ValueApproximate_h
