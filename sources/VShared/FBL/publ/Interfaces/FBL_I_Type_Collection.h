/**********************************************************************************************/
/* FBL_I_Type_Collection.h		                                           					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_Type_Collection_h
	#define _FBL_I_Type_Collection_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Interfaces/FBL_I_Type.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE(I_Type_Collection);
/**********************************************************************************************/
// 
//StubInterface
interface FBL_SHARED_EXP_CLASS I_Type_Collection : public I_Unknown
{

virtual 					~I_Type_Collection( void );

	public://///////////////////////////////////////////////////////////////////////////////////

	//
	// Collection type specific
	//

	// Member count

};


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_I_Type_Collection
