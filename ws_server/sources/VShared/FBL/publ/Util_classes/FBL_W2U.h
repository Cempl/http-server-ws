/**********************************************************************************************/
/* FBL_W2U.h	 		                                                   					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_W2U_h
	#define _FBL_W2U_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
class String;


/**********************************************************************************************/
// 
class FBL_SHARED_EXP_CLASS StW2U
{
	public://///////////////////////////////////////////////////////////////////////////////////
	
							StW2U( const wchar_t* inWStr, tslen inLen = -1 );

							~StW2U();

		const UChar*		u_str( void );


	private://///////////////////////////////////////////////////////////////////////////////////

		void				Convert( const wchar_t* inWStr, tslen inLen );


	private://///////////////////////////////////////////////////////////////////////////////////

		UChar*				mUStr;
		tslen				mLen;
};


/**********************************************************************************************/
// 
class FBL_SHARED_EXP_CLASS StU2W
{
	public://///////////////////////////////////////////////////////////////////////////////////
	
							StU2W( const UChar* inWStr, tslen inLen = -1 );
							StU2W( const String& inStr );
							
							~StU2W();

		const wchar_t*		w_str( void );


	private://///////////////////////////////////////////////////////////////////////////////////

		void				Convert( const UChar* inUStr, tslen inLen );


	private://///////////////////////////////////////////////////////////////////////////////////

		wchar_t*			mWStr;
		tslen				mLen;
};


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // _FBL_W2U_h
