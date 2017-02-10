/**********************************************************************************************/
/* FBL_I_ValueDateTime.h                                                   					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_ValueDateTime_h
	#define _FBL_I_ValueDateTime_h
#pragma once

// FBL:
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
SMART_INTERFACE(I_ValueDateTime);


/**********************************************************************************************/
// Interface class -- defines I_ValueDateTime API.
// 
interface FBL_SHARED_EXP_CLASS I_ValueDateTime : public I_Unknown
{
	 	
virtual 					~I_ValueDateTime( void );

	// ---------------------
	// Properties:
	
		// <DateTimeFormat> [r/o]
							/// Get date format.
virtual	const DTFormat*		get_DTFormat( void ) const 							= 0;


	// ---------------------
	// Date properties:

							/// Get/put the Year of current date.
virtual	vint32				get_Year( void ) const										= 0;
virtual	void				put_Year( vint32 inYear )									= 0;

		// <Month> [r/w]
							/// Get/put the Month of current date.
virtual	vuint16				get_Month( void ) const										= 0;
virtual	void				put_Month( vuint16 inMonth )								= 0;
		
		// <Day> [r/w]
							/// Get/put the Day of current date.
virtual	vuint16				get_Day( void ) const										= 0;
virtual	void				put_Day( vuint16 inDay )									= 0;

	// ---------------------
	// Date Methods:

							/// Get/put the Year, Month and Day of current date.
virtual	void				get_Date( 
								vint32&	outYear, 
								vuint16& outMonth, 
								vuint16& outDay ) const									= 0;

virtual	void				put_Date( 
								vint32	inYear, 
								vuint16	inMonth, 
								vuint16	inDay )											= 0;


	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// Time properties:

		// [r/w] <Hour>
							/// get/put the Hours of current date.
virtual	vuint16				get_Hours( void ) const										= 0;
virtual	void				put_Hours( vuint16 inHours )								= 0;

		// [r/w] <Minutes>
							/// get/put the Minutes of current date.
virtual	vuint16				get_Minutes( void ) const									= 0;
virtual	void				put_Minutes( vuint16 inMinutes )							= 0;
		
		// [r/w] <Seconds>
							/// get/put the Seconds of current date.
virtual	vuint16				get_Seconds( void ) const									= 0;
virtual	void				put_Seconds( vuint16 inSeconds )							= 0;

		// [r/w] <Millisecond>
							/// get/put the Milliseconds of current date.
virtual	vuint16				get_Milliseconds( void ) const								= 0;
virtual	void				put_Milliseconds( vuint16 inMilliseconds )					= 0;


	// ---------------------
	// Time Methods:

							/// get/put the Hours, Minutes, Seconds and Milliseconds
							/// of current date.
virtual	void				get_Time( 
								vuint16& 	outHours, 
								vuint16& 	outMinutes, 
								vuint16& 	outSeconds,
								vuint16& 	outMilliseconds ) const						= 0;
			
virtual	void				put_Time( 
								vuint16 	inHours, 
								vuint16 	inSeconds, 
								vuint16 	inMinutes,
								vuint16		inMilliseconds )							= 0;


	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// DateTime Methods:

							/// get/put the Date and Time.
virtual	void				get_DateTime( 
								vint32&		outYear, 
								vuint16& 	outMonth, 
								vuint16& 	outDay,
								vuint16& 	outHours, 
								vuint16& 	outMinutes, 
								vuint16& 	outSeconds,
								vuint16& 	outMilliseconds ) const						= 0;
			
virtual	void				put_DateTime( 
								vint32		inYear, 
								vuint16 	inMonth, 
								vuint16 	inDay,
								vuint16 	inHours, 
								vuint16 	inMinutes,
								vuint16 	inSeconds, 
								vuint16		inMilliseconds )							= 0;	
}; 


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_I_ValueDateTime_h
