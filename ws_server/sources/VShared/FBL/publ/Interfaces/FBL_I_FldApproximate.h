/**********************************************************************************************/
/* FBL_I_FldApproximate.h 		                                               				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_FldApproximate_h 
	#define _FBL_I_FldApproximate_h

#include <VShared/FBL/publ/Headers/FBL.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE(I_FldApproximate);


/**********************************************************************************************/
/** 
	Interface of Approximate field.
*/
//StubInterface
interface FBL_SHARED_EXP_CLASS I_FldApproximate : public I_Unknown
{

virtual						~I_FldApproximate( void );
		
	// Example: 123.45
	// Precision is 5 (all digits count).
	// Scale is 2 (digits count right after decimal point)

	// ---------------------
	// Properties:
	
		// <Precision> [r/w]
							/// Get/put the precision for this field.
virtual	vuint16				get_Precision( void ) const									= 0;
virtual	void				put_Precision( vuint16 inPrecision )						= 0;
	
		// <Scale> [r/w]
							/// Get/put the scale of field.
virtual	vuint16				get_Scale( void ) const										= 0;
virtual	void				put_Scale( vuint16 inScale )								= 0;

}; 


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif /// _FBL_I_FldApproximate_h
