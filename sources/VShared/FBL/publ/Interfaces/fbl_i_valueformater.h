/**********************************************************************************************/
/* FBL_I_ValueFormater.h                                                      				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_ValueFormater_h
	#define _FBL_I_ValueFormater_h
#pragma once

// STD:
#include <memory>

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Utilities/FBL_DateTimeEncode.h>

// FINAL: 
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace 


/**********************************************************************************************/
SMART_INTERFACE( I_ValueFormater );


/**********************************************************************************************/
FBL_SHARED_EXP I_ValueFormater_Ptr CreateLocalValueFormater(
	const String*		inPattern,
	const EFormat		inFormat,
	const String*		inpLocale );


/**********************************************************************************************/
typedef aptr<String> String_Ptr;


/**********************************************************************************************/
// interface FBL_SHARED_EXP_CLASS class
//
interface FBL_SHARED_EXP_CLASS I_ValueFormater : public I_Unknown
{

virtual 					~I_ValueFormater( void );

virtual	void				Init( 
								 const String*		inPattern, 
								 const EFormat		inFormat,
								 const String*		inpLocale ) = 0;

virtual	String_Ptr			Format_Int( const vint64& inValue ) = 0;

virtual	String_Ptr			Format_Double( const double& inValue ) = 0;

virtual	String_Ptr			Format_Date( const DateEncoded& inValue ) = 0;

virtual	String_Ptr			Format_Time( const TimeEncoded&	inValue ) = 0;

virtual	String_Ptr			Format_DateTime( const DateTimeEncoded&	inValue ) = 0;


}; 


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_I_ValueFormater_h







