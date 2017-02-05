/**********************************************************************************************/
/* FBL_Value_Factory.h																		  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_Value_Factory_h
   #define _FBL_Value_Factory_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Interfaces/FBL_I_Value.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE(I_Value);


/**********************************************************************************************/
// This function registers a value implementation in the global Value Factory.
// It allows to developers add own I_Value implementations.
//
// @Param inType -- defines what type must be created.
// @param inFactory -- factory for creation of the given type
//
FBL_SHARED_EXP	bool		RegisterValue(
								VALUE_TYPE			inType,
								MAKE_VALUE_FUNC_PTR inFactory );


/**********************************************************************************************/
namespace internal 
{

/**********************************************************************************************/
// Factories to create custom-type values:
//
FBL_SHARED_EXP	I_Value*	CreateValue_Enum( 
	I_Type_Enumerated_Ptr	inpType,
	vuint16 				inFlags = fNone,	
	bool					inIsRemote = false );


/**********************************************************************************************/
// INTERNAL Factory to create I_Value instance of required type.
//
// (!!!) You should use in your code the virtual Value factory FBL::I_Database::CreateValue();
//
// @Param inType  -- defines what type must be created.
//
// @param inFlags -- field flags. Is used to define if this value 
//					 should accept NULLs.
//
// @param inData -- depends on type of field. E.g. for String types it is max length.
//
// @param inIsRemote -- VCLIENT.dll set this to TRUE, VKERNEL.dll to FALSE.
//
FBL_SHARED_EXP	I_Value* 	CreateValue(
								VALUE_TYPE  inType,
								vuint16		inFlags,
								void*		inParam1 = nullptr,
								void*		inParam2 = nullptr,
								bool		inIsRemote = false );


/**********************************************************************************************/
// Helpers wrapper-factories to create String/Text values:
//
FBL_SHARED_EXP	I_Value*	CreateValue_String( 
								vuint16 			inFlags = fNone,
								tslen 				inMaxLen = 20,
								I_Localizable_Ptr 	inLocalizable = nullptr, 
								bool				inIsRemote = false );

FBL_SHARED_EXP	I_Value*	CreateValue_VarChar( 
								vuint16 			inFlags = fNone,
								tslen 				inMaxLen = 20,
								I_Localizable_Ptr 	inLocalizable = nullptr, 
								bool				inIsRemote = false );

FBL_SHARED_EXP	I_Value*	CreateValue_Text( 
								vuint16 			inFlags = fNone,
								I_Localizable_Ptr 	inLocalizable = nullptr, 
								bool				inIsRemote = false );


/**********************************************************************************************/
// function registration standard values
//
void FBL_SHARED_EXP RegisterStandardValues( void );


/**********************************************************************************************/
} // namespace internal


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_Value_Factory_h
