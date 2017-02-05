/**********************************************************************************************/
/* FBL_Value_Enum.cpp                                                    					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

//#include <VShared/FBL/publ/Values/FBL_Value_Enum.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Value.h>
#include <VShared/FBL/publ/Util_classes/FBL_ToUChar.h>
#include <VShared/FBL/publ/Utilities/FBL_TypeUtilities.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
void Convert_str_enum( 
	const I_Value*	inValue, 
	I_Value*		outValue );

void Convert_enum_str( 
	const I_Value*	inValue, 
	I_Value*		outValue );


/**********************************************************************************************/
// WRAPPER: I_Value => UChar*
//
void Convert_str_enum( 
	const I_Value*	inValue, 
	I_Value*		outValue )
{
	outValue->put_String( inValue->get_String() );
}


/**********************************************************************************************/
void Convert_enum_str( 
	const I_Value*	inValue, 
	I_Value*		outValue )
{	
	outValue->put_String( inValue->get_String() );
}


/**********************************************************************************************/

FBL_End_Namespace
