/**********************************************************************************************/
/* FBL_PtrValidators.h																		  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_PtrValidators_h
	#define _FBL_PtrValidators_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Interfaces/FBL_I_IndexStyle.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_Database.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Table.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Link.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_KeyValue.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
FBL_SHARED_EXP bool IsValidDbPointer( I_Database* inPointer );
FBL_SHARED_EXP bool IsValidTypePointer( I_Type* inPointer );
FBL_SHARED_EXP bool IsValidTablePointer( I_Table* inPointer );
FBL_SHARED_EXP bool IsValidLinkPointer( I_Link* inPointer );
FBL_SHARED_EXP bool IsValidKeyValuePointer( I_KeyValue* inPointer );
FBL_SHARED_EXP bool IsValidIndexStylePointer( I_IndexStyle* inPointer );

/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // _FBL_FBL_PtrValidators_h
