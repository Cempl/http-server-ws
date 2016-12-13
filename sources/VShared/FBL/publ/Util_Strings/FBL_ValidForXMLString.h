/**********************************************************************************************/
/* FBL_ValidForXMLString.h																	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_ValidForXMLString_h
	#define _FBL_ValidForXMLString_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
FBL_SHARED_EXP bool ContainInvalidXMLChar( String& inString );


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_ValidForXMLString_h
