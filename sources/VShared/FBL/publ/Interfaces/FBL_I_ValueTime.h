/**********************************************************************************************/
/* FBL_I_ValueTime.h                                                      					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_ValueTime_h
	#define _FBL_I_ValueTime_h
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
SMART_INTERFACE(I_ValueTime);


/**********************************************************************************************/
// Interface class - defines I_ValueTime API.
// 
interface FBL_SHARED_EXP_CLASS I_ValueTime : public I_Unknown
{
virtual 					~I_ValueTime( void );

	// ---------------------
	// Properties:
	
		// <DateTimeFormat> [r/o]
							/// Get the datetime format.
virtual	const DTFormat*		get_DTFormat( void ) const									= 0;

		// [r/w] <Hour>
							/// Get/put the Hours of Time value.
virtual	vuint16				get_Hours( void ) const										= 0;
virtual	void				put_Hours( vuint16 inHours )								= 0;

		// [r/w] <Minutes>
							/// Get/put the Minutes of Time value.
virtual	vuint16				get_Minutes( void ) const									= 0;
virtual	void				put_Minutes( vuint16 inMinutes )							= 0;
		
		// [r/w] <Seconds>
							/// Get/put the Seconds of Time value.
virtual	vuint16				get_Seconds( void ) const									= 0;
virtual	void				put_Seconds( vuint16 inSeconds )							= 0;

		// [r/w] <Millisecond>
							/// Get/put the Milliseconds of Time value.
virtual	vuint16				get_Milliseconds( void ) const								= 0;
virtual	void				put_Milliseconds( vuint16 inMilliseconds )					= 0;


	// ---------------------
	// Value Methods:

							/// Get/put the Hours, Minutes, Seconds and Milliseconds
							/// of the current date.
virtual	void				get_Time( 
								vuint16& outHours, 
								vuint16& outMinutes, 
								vuint16& outSeconds,
								vuint16& outMilliseconds ) const						= 0;
			
virtual	void				put_Time( 
								vuint16 	inHours, 
								vuint16 	inMinutes = 0, 
								vuint16 	inSeconds = 0,
								vuint16	inMilliseconds = 0 )							= 0;
}; 


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_I_ValueTime_h
