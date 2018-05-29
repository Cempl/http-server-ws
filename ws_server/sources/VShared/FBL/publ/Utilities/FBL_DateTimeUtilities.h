/**********************************************************************************************/
/* FBL_DateTimeUtilities.h																  	  */
/*                                                                       					  */
/* Copyright Paradigma 1993-2011															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_DateTimeUtilities_h 
	#define _FBL_DateTimeUtilities_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Utilities/FBL_DateTimeEncode.h>

// CRT:
#include <time.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
FBL_SHARED_EXP vuint64		GetTimeStamp_2( void );


/**********************************************************************************************/
FBL_SHARED_EXP DateEncoded	GetBuildDate( void );


/**********************************************************************************************/
FBL_SHARED_EXP DateEncoded	GetOldEmbeddedLicenseDate( void );


/**********************************************************************************************/
class DTFormat;


#pragma mark -
#pragma mark === Get CURRENT DateTime ===


/**********************************************************************************************/
// Function returns current date using C runtime.
//
FBL_SHARED_EXP void GetCurrentDate( 
	vint32&	outYear, 
	vuint16& outMonth, 
	vuint16& outDay );

FBL_SHARED_EXP vint32 GetCurrentDate( void );


/**********************************************************************************************/
// Function returns current time using C runtime.
//
FBL_SHARED_EXP void GetCurrTime( 
	vuint16& outHours, 
	vuint16& outMinutes, 
	vuint16& outSeconds, 
	vuint16& outMilliseconds );


/**********************************************************************************************/
// Function returns current date time using C runtime.
//
FBL_SHARED_EXP void GetCurrentDateTime( 
	vint32&	outYear, 
	vuint16& outMonth, 
	vuint16& outDay,
	vuint16& outHours, 
	vuint16& outMinutes, 
	vuint16& outSeconds, 
	vuint16& outMilliseconds );


/**********************************************************************************************/
// Function returns current date time using C runtime.
//
FBL_SHARED_EXP vint64 GetCurrentDateTime( void );


/**********************************************************************************************/
// Calculate nr of day since year 0 in new date-system (from 1615)
//
FBL_SHARED_EXP vint32 GetDaysCount(vint32 Y,vuint16 M,vuint16 D);


/**********************************************************************************************/
// Function returns difference in seconds or LLONG_MAX for huge diff.
//
FBL_SHARED_EXP vint64 GetDateTimeDifference( vint64 inValue1, vint64 inValue2 );


/**********************************************************************************************/
// Function returns current date, time and time separators and format of date.
//
FBL_SHARED_EXP void GetCurrentDateTimeFormat( 
	DTFormat&	outDTFormat );


#pragma mark -
#pragma mark === Validation Methods  ===


/**********************************************************************************************/
vint32 CheckTimeStringFormat(
	const char* inTimeStr,
	const DTFormat* pDTFormat );

vint32 CheckDateStringFormat(
	const char* inDateStr,
	const DTFormat* pDTFormat );


vint32 CheckDateTimeStringFormat(
	const char* inDateTimeStr,
	const DTFormat* pDTFormat );


/**********************************************************************************************/
FBL_SHARED_EXP bool CheckDateValueLimits( DateEncoded& d );
FBL_SHARED_EXP bool CheckTimeValueLimits( TimeEncoded& t );

DateEncoded GetDate( const char* inpDate );


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_DateTimeUtilities_h


