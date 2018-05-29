/**********************************************************************************************/
/* VSQL_PtrValidators.h																		  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                         					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _VSQL_PtrValidators_h
	#define _VSQL_PtrValidators_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Interfaces/FBL_I_Field.h>

#include <VShared/VSQL/Interfaces/VSQL_I_SqlDatabase.h>
#include <VShared/VSQL/Interfaces/VSQL_I_Cursor.h>
#include <VShared/VSQL/Interfaces/VSQL_I_VProject.h>

/**********************************************************************************************/
VSQL_Begin_Namespace


/**********************************************************************************************/
FBL_SHARED_EXP bool IsValidProjectPointer( I_VProject* inPointer );
FBL_SHARED_EXP bool IsValidCursorPointer( VSQL::I_Cursor* inPointer );
FBL_SHARED_EXP bool IsValidFieldPointer( I_Field* inPointer );



// Helpers.
bool IsValidFieldPointer_FBL( 
	FBL::I_Database*	inpDb, 
	I_Field*			inPointer );

bool IsValidFieldPointer_VSQL( 
	I_SqlDatabase*		inpDb, 
	I_Field*			inPointer );
	
bool FieldExist( 
	I_Table_Ptr			inpTbl,
	I_Field*			inPointer);
	

/**********************************************************************************************/
VSQL_End_Namespace


/**********************************************************************************************/
#endif // _VSQL_PtrValidators_h
