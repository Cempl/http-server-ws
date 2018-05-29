/**********************************************************************************************/
/* VSQL_ErrorMap.h										    								  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _VSQL_ErrorMap_h
	#define _VSQL_ErrorMap_h
#pragma once

// VSQL:
#include <VShared/VSQL/Headers/VSQL.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
VSQL_Begin_Namespace


/**********************************************************************************************/
extern FBL_SHARED_EXP FBL::ErrorEntry gErrorMap[];
extern FBL_SHARED_EXP vuint32 gErrorMapSize;


/**********************************************************************************************/
FBL_SHARED_EXP FBL::ErrorEntry* GetErrorMapEntry( vuint32 i );
FBL_SHARED_EXP vuint32 GetErrorMapSize( void );


/**********************************************************************************************/
VSQL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _VSQL_ErrorMap_h
