/**********************************************************************************************/
/* FBL_DateTimeEncode.h																	  	  */
/*                                                                       					  */
/* Copyright Paradigma 1993-2011															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_DateTimeEncode_h 
	#define _FBL_DateTimeEncode_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


#pragma mark -
#pragma mark === Date ===


/**********************************************************************************************/
#if FBL_BIG_ENDIAN

union DateEncoded
{
	vint32  encoded;		// vint32 value, the more date, the more value.
						// Dates can be <0, this means before 0 year.
	struct decoded
	{
		signed   y	: 23;		// year		-2^22 to +2^22
		unsigned m	: 4;		// month, 	1 - 12
		unsigned d	: 5;		// day, 	1 - 31
	} decoded;
}; 

#else // For Windows and Mac (with Intel processor) we need to have back order of bit fields:

union DateEncoded
{
	vint32  encoded;		// vint32 value, the more date, the more value.
						// Dates can be <0, this means before 0 year.
	struct
	{
		unsigned d	: 5;		// day, 	1 - 31
		unsigned m	: 4;		// month, 	1 - 12
		signed   y	: 23;		// year		-2^22 to +2^22
	} decoded;
}; 

#endif // FBL_BIG_ENDIAN


/**********************************************************************************************/ 
inline vint32 EncodeDate( vint32 inYear, vuint16 inMonth, vuint16 inDay )
{
	FBL_CHECK( inMonth <= 12 ); 
	FBL_CHECK( inDay <= 31 ); 

	DateEncoded date;
	
	date.decoded.y = inYear;
	date.decoded.m = inMonth;
	date.decoded.d = inDay;
	
	return date.encoded;
}

inline void DecodeDate( vint32 inDate, vint32& outYear, vuint16& outMonth, vuint16& outDay )
{
	DateEncoded date;
	
	date.encoded = inDate;
	
	outYear 	= date.decoded.y;
	outMonth	= date.decoded.m;
	outDay		= date.decoded.d;	
}


#pragma mark -
#pragma mark === Time ===


/**********************************************************************************************/
#if FBL_BIG_ENDIAN

union TimeEncoded
{
	vuint32  encoded;	// vuint32 value, the more time, the more value.
	
	struct
	{
		unsigned h	: 5;	// hours		0 - 23
		unsigned m	: 6;	// minutes, 	0 - 59
		unsigned s	: 6;	// seconds, 	0 - 59
		unsigned ms	: 15;	// mili seconds,0 - 999
	} decoded;
};

#else // FBL_BIG_ENDIAN

union TimeEncoded
{
	vuint32  encoded;	// vuint32 value, the more time, the more value.
	
	struct
	{
		unsigned ms	: 15;	// mili seconds,0 - 999
		unsigned s	: 6;	// seconds, 	0 - 59
		unsigned m	: 6;	// minutes, 	0 - 59
		unsigned h	: 5;	// hours		0 - 23
	} decoded;
};

#endif // FBL_BIG_ENDIAN


/**********************************************************************************************/ 
#if FBL_BIG_ENDIAN

union DateTimeEncoded
{
	vint64 encoded;
	struct
	{
		DateEncoded date;
		TimeEncoded time;
	} decoded;
};

#else // FBL_BIG_ENDIAN

union DateTimeEncoded
{
	vint64 encoded;
	struct
	{
		TimeEncoded time;
		DateEncoded date;
	} decoded;
};

#endif // FBL_BIG_ENDIAN


/**********************************************************************************************/ 
inline vuint32 EncodeTime( 
	vuint16 inHour, 
	vuint16 inMinutes, 
	vuint16 inSeconds, 
	vuint16 inMilliSeconds )
{
	FBL_CHECK( inHour <= 24 ); 
	FBL_CHECK( inMinutes <= 59 ); 
	FBL_CHECK( inSeconds <= 59 ); 
	FBL_CHECK( inMilliSeconds <= 999 ); 

	TimeEncoded time;

	time.decoded.h = inHour;
	time.decoded.m = inMinutes;
	time.decoded.s = inSeconds;
	time.decoded.ms = inMilliSeconds;

	return time.encoded;
}

inline void DecodeTime( 
	vuint32 inTime, 
	vuint16& outHour, 
	vuint16& outMinutes, 
	vuint16& outSeconds, 
	vuint16& outMilliSeconds )
{
	TimeEncoded time;

	time.encoded = inTime;

	outHour 	= (vuint16) time.decoded.h;
	outMinutes	= (vuint16) time.decoded.m;
	outSeconds	= (vuint16) time.decoded.s;
	outMilliSeconds	= (vuint16) time.decoded.ms;
}



#pragma mark -
#pragma mark === DateTime ===


/**********************************************************************************************/
#if FBL_BIG_ENDIAN

inline void EncodeDateTime( vint64* outDateTime, vint32 inDate, vuint32 inTime )
{
	vint32* p = (vint32*) outDateTime;
	
	p[0] = (vint32) inDate;
	p[1] = (vint32) inTime;
}


inline void DecodeDateTime( vint64* inDateTime, vint32* outDate, vuint32* outTime )
{
	vint32* p = (vint32*) inDateTime;

	*outDate =  (vint32) p[0];
	*outTime = (vuint32) p[1];
}


inline vint32 DateTime_GetDate( vint64* inDateTime )
{
	vint32* p = (vint32*) inDateTime;
	return p[0];
}


inline vuint32 DateTime_GetTime( vint64* inDateTime )
{
	vint32* p = (vint32*) inDateTime;
	return (vuint32) p[1];
}


inline void DateTime_SetDate( vint64* ioDateTime, vint32 inDate )
{
	vint32* p = (vint32*) ioDateTime;
	p[0] = inDate;
}


inline void DateTime_SetTime( vint64* ioDateTime, vuint32 inTime )
{
	vuint32* p = (vuint32*) ioDateTime;
	p[1] = inTime;
}


/**********************************************************************************************/
#else // FBL_BIG_ENDIAN


inline void EncodeDateTime( vint64* outDateTime, vint32 inDate, vuint32 inTime )
{
	vint32* p = (vint32*) outDateTime;
	
	p[0] = (vint32) inTime;
	p[1] = (vint32) inDate;
}

inline void DecodeDateTime( vint64* inDateTime, vint32* outDate, vuint32* outTime )
{
	vint32* p = (vint32*) inDateTime;

	*outDate =  (vint32) p[1];
	*outTime = (vuint32) p[0];
}

inline vint32 DateTime_GetDate( vint64* inDateTime )
{
	vint32* p = (vint32*) inDateTime;
	return p[1];
}

inline vuint32 DateTime_GetTime( vint64* inDateTime )
{
	vint32* p = (vint32*) inDateTime;
	return (vuint32) p[0];
}

inline void DateTime_SetDate( vint64* ioDateTime, vint32 inDate )
{
	vint32* p = (vint32*) ioDateTime;
	p[1] = inDate;
}

inline void DateTime_SetTime( vint64* ioDateTime, vuint32 inTime )
{
	vuint32* p = (vuint32*) ioDateTime;
	p[0] = inTime;
}

#endif // FBL_BIG_ENDIAN


/**********************************************************************************************/
inline vint64 EncodeDateTime( 
	vint32	inYear,	
	vuint16	inMonth,		
	vuint16	inDay,

	vuint16	inHour,	
	vuint16	inMinutes,	
	vuint16	inSeconds, 
	vuint16	inMilliSeconds )
{
	FBL_CHECK( inMonth <= 12 ); 
	FBL_CHECK( inDay <= 31 ); 
	FBL_CHECK( inHour <= 24 ); 
	FBL_CHECK( inMinutes <= 59 ); 
	FBL_CHECK( inSeconds <= 59 ); 
	FBL_CHECK( inMilliSeconds <= 999 ); 


	DateTimeEncoded dt;
	
	dt.decoded.date.decoded.y = inYear;
	dt.decoded.date.decoded.m = inMonth;
	dt.decoded.date.decoded.d = inDay;

	dt.decoded.time.decoded.h = inHour;
	dt.decoded.time.decoded.m = inMinutes;
	dt.decoded.time.decoded.s = inSeconds;
	dt.decoded.time.decoded.ms = inMilliSeconds;

	return dt.encoded;
}


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_DateTimeEncode_h


