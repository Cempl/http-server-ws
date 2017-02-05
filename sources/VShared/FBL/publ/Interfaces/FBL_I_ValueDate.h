/**********************************************************************************************/
/* FBL_I_ValueDate.h                                                      					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_ValueDate_h
	#define _FBL_I_ValueDate_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Interfaces/FBL_I_Value.h>
#include <VShared/FBL/publ/Util_classes/FBL_DTFormat.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// forward declarations:
//
SMART_INTERFACE(I_Value);


/**********************************************************************************************/
// this file declarations:
//
SMART_INTERFACE(I_ValueDate);


/**********************************************************************************************/
// Interface class -- defines I_ValueDate API.
// 
interface FBL_SHARED_EXP_CLASS I_ValueDate : public I_Unknown
{

virtual 					~I_ValueDate( void );


	// ---------------------
	// Properties:
	
		// <DateTimeFormat> [r/o]
							/// Get the date format for this value.
virtual	const DTFormat*		get_DTFormat( void ) const 									= 0;

							/// Get/put the Year of value.
virtual	vint32				get_Year( void ) const										= 0;
virtual	void				put_Year( vint32 inYear )									= 0;

		// <Month> [r/w]
							/// Get/put the Month of value.
virtual	vuint16				get_Month( void ) const										= 0;
virtual	void				put_Month( vuint16 inMonth )								= 0;
		
		// <Day> [r/w]
							/// Get/put the Day of value.
virtual	vuint16				get_Day( void ) const										= 0;
virtual	void				put_Day( vuint16 inDay )									= 0;

	// ---------------------
	// Value Methods:
	
							/// Get/put the Year, Month and Day in the single call.
virtual	void				get_Date( 
								vint32&	outYear, 
								vuint16& outMonth, 
								vuint16& outDay ) const									= 0;

virtual	void				put_Date( 
								vint32	inYear, 
								vuint16	inMonth, 
								vuint16	inDay )											= 0;

}; 


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_I_ValueDate_h
