/**********************************************************************************************/
/* FBL_Convert.h 		                                                      				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_Convert_h 
	#define _FBL_Convert_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Type_Enumerated.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// forward declarations: 
//
SMART_INTERFACE( I_Value );


/**********************************************************************************************/
// Type of function that executes conversion from one type to another.
//
typedef void (*CONVERT_FUNC_PTR)( 
					const I_Value*	inValue,
					      I_Value*	outValue );


/**********************************************************************************************/
/**	Execute convertion of value from one type to other.

	@param inTypeFrom -- current type of value
	@param inTypeDest -- new type we want to get.
	@param inDataFrom -- pointer to the first byte of current value.
	@param inDataDest -- pointer to the first byte of buffer for new value.

	@return -- TRUE if conversion was successful. 
*/
FBL_SHARED_EXP bool ConvertValue(
	vuint32 		inValueType,
	vuint32 		outValueType,
	const I_Value*	inValue,
	      I_Value*	outValue );


/**********************************************************************************************/
FBL_SHARED_EXP bool ConvertValue(
	const I_Value*	inValue,
	I_Value*		outValue );
		  

/**********************************************************************************************/
/**	Execute convertion of value from one type to other and create instance of value of new type.

	@param inValue -- pointer to the current value.
	@param inResultType -- new type we want to get.
	
	@return -- new value if conversion was successful, otherwise NULL. 
*/
FBL_SHARED_EXP I_Value_Ptr ConvertValue(
	const I_Value& 			inValue,
	VALUE_TYPE 				inResultType );
	
FBL_SHARED_EXP I_Value_Ptr ConvertValue(
	const I_Value&			inValue,
	I_Type_Enumerated_Ptr	inResultType );


/**********************************************************************************************/
/**	Class which get 2 types for conversion, obtain pointer on worker function.
	Now it is able do faster conversion of many values in the batch mode. 
	For example array of shorts to array of longs. 
	
	NOTE: it is not symmetrical. You need another TypeConverter to convert back.
*/
class FBL_SHARED_EXP_CLASS TypeConverter
{
	public://///////////////////////////////////////////////////////////////////////////////////
 
							TypeConverter(
								vuint32 		inTypeFrom, 		
								vuint32 		inTypeDest );

		bool				IsGood( void ) const 
								{ return bool( mFuncPtr != nullptr ); }					
							
		void				DoConversion(
								const I_Value*	inValue,
								      I_Value*	outValue )
							{
								mFuncPtr( inValue, outValue );							
							}
							

	private:////////////////////////////////////////////////////////////////////////////////////
 
		CONVERT_FUNC_PTR	mFuncPtr;
};


/**********************************************************************************************/
// Helper function for comparing two values:
// Values must be from the same value category.
// If values have differenet types - converting one of them.
//
int FBL_SHARED_EXP SmartCompare( Const_I_Value_Ptr inFirstVal, Const_I_Value_Ptr inSecondVal );


/**********************************************************************************************/
CONVERT_FUNC_PTR FBL_SHARED_EXP get_Convertor( vuint32 inValueType, vuint32 outValueType );


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_Convert_h
