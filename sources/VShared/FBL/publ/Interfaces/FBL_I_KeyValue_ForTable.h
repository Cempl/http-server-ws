/**********************************************************************************************/
/* FBL_I_KeyValue_ForTable.h		                                                      	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_KeyValue_ForTable_h
	#define _FBL_I_KeyValue_ForTable_h
#pragma once

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// this file declarations:
//
SMART_INTERFACE(I_KeyValue_ForTable);

/**********************************************************************************************/
// Forward declarations:
//
SMART_INTERFACE(I_Table);


/****************************************************x*****************************************/
//StubInterface
interface FBL_SHARED_EXP_CLASS I_KeyValue_ForTable : public I_Unknown
{
	// ---------------------
	// Construction:

virtual 						~I_KeyValue_ForTable( void );

	// ---------------------
	// Properties:

virtual I_Table_Ptr				get_Table( void ) const = 0;

};



/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_I_KeyValue_ForTable_h
