/**********************************************************************************************/
/* FBL_ErrorMap.h										    								  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_ErrorMap_h
	#define _FBL_ErrorMap_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace

struct ErrorEntry;

/**********************************************************************************************/
FBL_SHARED_EXP extern ErrorEntry gErrorMap[];
FBL_SHARED_EXP extern const vuint32 gErrorMapSize;


/**********************************************************************************************/
FBL_SHARED_EXP ErrorEntry* GetErrorMapEntry( vuint32 i );
FBL_SHARED_EXP vuint32 GetErrorMapSize( void );


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_ErrorMap_h
