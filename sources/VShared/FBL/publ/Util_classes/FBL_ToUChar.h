/**********************************************************************************************/
/* FBL_ToUChar.h	 		                                                   				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_ToUChar_h
	#define _FBL_ToUChar_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Util_Strings/FBL_String_Len_Const.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// Stack based class to convert numeric value into UChar* string.
// Is used mainly to send some parameters into xException.
// 
class FBL_SHARED_EXP_CLASS ToUCharPtr
{
	public://///////////////////////////////////////////////////////////////////////////////////

							ToUCharPtr( vint16 inValue );
							ToUCharPtr( vint32 inValue );
							ToUCharPtr( vint64 inValue );

							ToUCharPtr( vuint16 inValue );
							ToUCharPtr( vuint32 inValue );
							ToUCharPtr( vuint64 inValue );

	const UChar*			c_str( void ) const  { return mStr; }


	private:///////////////////////////////////////////////////////////////////////////////////
	
			UChar			mStr[kLLongStrMaxLen + 1];	// Max
};


/**********************************************************************************************/
// Stack based class to convert numeric value into char* string.
// Is used mainly to send some parameters into xException.
// 
class FBL_SHARED_EXP_CLASS ToCharPtr
{
	public://///////////////////////////////////////////////////////////////////////////////////

							ToCharPtr( vint16 inValue ); 	
							ToCharPtr( vint32 inValue ); 	
							ToCharPtr( vint64 inValue );

							ToCharPtr( vuint16 inValue );	
							ToCharPtr( vuint32 inValue );	
							ToCharPtr( vuint64 inValue );

	const char*				c_str( void ) const  { return mStr; }


	private:///////////////////////////////////////////////////////////////////////////////////
	
			char			mStr[kLLongStrMaxLen + 1];	// Max
};


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // _FBL_String_h
