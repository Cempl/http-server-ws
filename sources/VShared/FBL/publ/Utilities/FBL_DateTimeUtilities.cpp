/**********************************************************************************************/
/* FBL_DateTimeUtilities.cpp 					                                  			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Headers/FBL_Version.h>

#include <VShared/FBL/publ/Utilities/FBL_DateTimeUtilities.h>
#include <VShared/FBL/publ/Util_classes/FBL_DTFormat.h>
#include <VShared/FBL/publ/Util_Strings/FBL_ASCIIToFromNumeric.h>

#include <VShared/FBL/publ/Util_Strings/FBL_String_Len_Const.h>
#include <VShared/FBL/publ/Util_Strings/FBL_ustdio.h>

#include <VShared/FBL/publ/TestTools/FBL_TestSystem_Globals.h>

#include <time.h>

#if FBL_UNIX || FBL_MAC_MACHO
#include <sys/time.h>
#endif // FBL_UNIX

#include <cctype>
#include <string>	

#if FBL_UNIX
#	include <langinfo.h>
#endif // FBL_UNIX


/**********************************************************************************************/
Std_Using_Namespace


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
vuint64 GetTimeStamp_2( void )
{
	// Get current timestamp (seems to be (micro-secs * 10) in answer. 1 mcs = (0.000,001s) ).
#if FBL_WIN

	// Resolutions on XP seems to be 15ms (millisec) (0.015s).
	// Resolution on Windows 7 seems to be sub-millisecond.
	
#define OFFSET_TO_EPOC ((__int64) 134774 * 24 * 60 * 60 * 1000 * 1000 * 10)

	static __int64 offset=0, freq;
	LARGE_INTEGER t_cnt;

	if( !offset )
	{
		LARGE_INTEGER li;
		FILETIME ft;
		GetSystemTimeAsFileTime(&ft);
		li.LowPart=ft.dwLowDateTime;
		li.HighPart=ft.dwHighDateTime;
		offset=li.QuadPart-OFFSET_TO_EPOC;
		QueryPerformanceFrequency(&li);
		freq=li.QuadPart;
		QueryPerformanceCounter(&t_cnt);
		offset-=t_cnt.QuadPart/freq*10000000+t_cnt.QuadPart%freq*10000000/freq;
	}

	QueryPerformanceCounter(&t_cnt);

	return t_cnt.QuadPart/freq*10000000+t_cnt.QuadPart%freq*10000000/freq+offset;

#else // FBL_WIN

	#if FBL_MAC_CARBON

		vuint32 ticks = TickCount();
		vuint32 tv_usec = ticks * 1000;
		vuint32 tv_sec  = ticks / 1000;

		return vuint64(tv_sec * 10000000) + vuint64(tv_usec * 10);

	#elif FBL_UNIX

		// resolution of such approach is nano-secs.
		struct timespec tp;
		clock_gettime( CLOCK_REALTIME, &tp );
		return vuint64(tp.tv_sec*10000000) + (vuint64) tp.tv_nsec/100;

	#else 

		// resolution of such approach is micro-secs.
		struct timeval tick_time;
		gettimeofday(&tick_time, 0);
		return vuint64(tick_time.tv_sec * 10000000) + vuint64(tick_time.tv_usec * 10);

	#endif // FBL_MAC_CARBON

#endif // FBL_WIN
}


/**********************************************************************************************/
/// Helper function. Expect inpDate in 'YYYY/MM/DD' format.
//
DateEncoded GetDate( const char* inpDate )
{
	DateEncoded result;

	std::string date(inpDate);

	vint16 buildYear  = (vint16) atol( date.substr(0,4).c_str() );
	vint16 buildMonth = (vint16) atol( date.substr(5,2).c_str() );
	vint16 buildDay   = (vint16) atol( date.substr(8,2).c_str() );

	result.decoded.y = buildYear;
	result.decoded.m = buildMonth;
	result.decoded.d = buildDay;

	return result;
}


/**********************************************************************************************/
DateEncoded GetBuildDate( void )
{
#if FBL_TEST_CODE
	
    if( *gTest_BuildDate )
        return GetDate( gTest_BuildDate );
    
#endif // FBL_TEST_CODE
 
	return GetDate( kBuildDate );

}


/**********************************************************************************************/
DateEncoded GetOldEmbeddedLicenseDate( void )
{
	return GetDate( kOldEmbeddedLicenseDate );
}


/**********************************************************************************************/
vint32 CheckFormatOfDateString(
	const char* inStr,
	const char* inFormatStr,
	char		inSeparator );

vint32 CheckFormatOfTimeString(
	const char* inStr,
	char		inSeparator );


#pragma mark -


/**********************************************************************************************/
/**
	@note Very rare case in Valentina Kernel: implemented ability for tests to change 
    result of this function using global vars defined in the FBL_TestSystem_Globals.cpp
	If Debug build is used in other debug products than var is zero and the normal code works.

	@return The current date using C runtime.
*/

void GetCurrentDate( 
	vint32&		outYear, 
	vuint16& 	outMonth,
	vuint16& 	outDay )
{

#if FBL_TEST_CODE

	if( gTest_Year != 0 )
	{
        outYear	 = gTest_Year.load();
        outMonth = gTest_Month.load();
        outDay   = gTest_Day.load();

		return; // so normal code will not work at all.
    }

#endif // FBL_TEST_CODE

	time_t t;
	time( &t );
	
    struct tm* cur_time = localtime( &t );

	outYear	 =	 vint32( cur_time->tm_year + 1900 );
	outMonth =	vuint16( cur_time->tm_mon + 1 );
	outDay   =	vuint16( cur_time->tm_mday );
}


/**********************************************************************************************/
/// Function returns the current date using C runtime.
//
vint32 GetCurrentDate( void )
{
	vint32 result = 0;
    
	vint32 	y;
	vuint16 m;
	vuint16 d;

	GetCurrentDate( y, m, d );
	
	((DateEncoded&)result).decoded.y 	= 	y; 
	((DateEncoded&)result).decoded.m 	= 	m; 
	((DateEncoded&)result).decoded.d 	= 	d; 
	
	return result; 
}


/**********************************************************************************************/
// Function returns current time using C runtime.
//
void GetCurrTime( 
	vuint16& outHours, 
	vuint16& outMinutes, 
	vuint16& outSeconds, 
	vuint16& outMilliseconds )
{
#if FBL_WIN

	SYSTEMTIME st;	

	::GetLocalTime(&st);

	outHours		= (vuint16)st.wHour;
	outMinutes		= (vuint16)st.wMinute;
	outSeconds		= (vuint16)st.wSecond;
	outMilliseconds = (vuint16)st.wMilliseconds;

#else // FBL_WIN

	struct timeval tv;
	struct timezone tz;

	gettimeofday( &tv, &tz );
	struct tm* cur_time = localtime( &tv.tv_sec );

	outHours		= (vuint16) cur_time->tm_hour;
	outMinutes		= (vuint16) cur_time->tm_min;
	outSeconds		= (vuint16) cur_time->tm_sec;
	outMilliseconds = (vuint16) (tv.tv_usec/1000);

#endif // FBL_WIN
}


/**********************************************************************************************/
// Function returns current date time using C runtime.
//
void GetCurrentDateTime( 
	vint32&	outYear, 
	vuint16& outMonth, 
	vuint16& outDay,
	vuint16& outHours, 
	vuint16& outMinutes, 
	vuint16& outSeconds, 
	vuint16& outMilliseconds )
{
#if FBL_WIN

	SYSTEMTIME st;

	::GetLocalTime(&st);	

	outYear	 		= (vint32)st.wYear;
	outMonth 		= (vuint16)st.wMonth;
	outDay   		= (vuint16)st.wDay;
	outHours		= (vuint16)st.wHour;
	outMinutes		= (vuint16)st.wMinute;
	outSeconds		= (vuint16)st.wSecond;
	outMilliseconds = (vuint16)st.wMilliseconds;

#else // FBL_WIN

	struct timeval tv;
	struct timezone tz;

	gettimeofday( &tv, &tz );
	struct tm* cur_time = localtime( &tv.tv_sec );

	outYear	 		= vint32( cur_time->tm_year + 1900 );
	outMonth 		= vuint16( cur_time->tm_mon + 1);
	outDay   		= (vuint16) cur_time->tm_mday;
	outHours		= (vuint16) cur_time->tm_hour;
	outMinutes		= (vuint16) cur_time->tm_min;
	outSeconds		= (vuint16) cur_time->tm_sec;
	outMilliseconds = (vuint16) (tv.tv_usec/1000);

#endif // FBL_WIN
}


/**********************************************************************************************/
// Function returns current date time using C runtime.
//
vint64 GetCurrentDateTime( void )
{
	vint64 result = 0;
	vint32 	y;
	vuint16 m;
	vuint16 d;
	vuint16	h;
	vuint16	minute;
	vuint16 s;
	vuint16 ms;
	
	GetCurrentDateTime( y, m, d, h, minute, s, ms	);	
	
	((DateTimeEncoded&)result).decoded.time.decoded.h	= 	h; 
	((DateTimeEncoded&)result).decoded.time.decoded.m	=	minute; 
	((DateTimeEncoded&)result).decoded.time.decoded.s	=	s; 
	((DateTimeEncoded&)result).decoded.time.decoded.ms	=	ms; 
	((DateTimeEncoded&)result).decoded.date.decoded.y 	= 	y; 
	((DateTimeEncoded&)result).decoded.date.decoded.m 	= 	m; 
	((DateTimeEncoded&)result).decoded.date.decoded.d 	= 	d;
	
	return result; 
}


/**********************************************************************************************/
// Calculate nr of day since year 0 in new date-system (from 1615)
//
vint32 GetDaysCount(vint32 Y,vuint16 M,vuint16 D)
{
	vint32 	delsum;
	int 	temp;

	if (Y < 200){
		if( (Y = Y+1900) < 1970 )
	  		Y += 100;
	}

	delsum = (vint32) (365L * Y + 31 * (M - 1) + D);

	if( M <= 2 )
	  	Y--;
	else  delsum -= (vint32) (M * 4 + 23) / 10;

	temp = (int) ((Y / 100 + 1) * 3) / 4;

	return ( delsum + (int) Y/4 - temp );

}


/**********************************************************************************************/
// Function returns difference in seconds or LLONG_MAX for huge diff.
//
vint64 GetDateTimeDifference( vint64 inValue1, vint64 inValue2 )
{
	FBL_CHECK( inValue1 > inValue2 );

	vint32 days1 = GetDaysCount( 
		((DateTimeEncoded&)inValue1).decoded.date.decoded.y,
		((DateTimeEncoded&)inValue1).decoded.date.decoded.m,
		((DateTimeEncoded&)inValue1).decoded.date.decoded.d );

	vint32 days2 = GetDaysCount( 
		((DateTimeEncoded&)inValue2).decoded.date.decoded.y,
		((DateTimeEncoded&)inValue2).decoded.date.decoded.m,
		((DateTimeEncoded&)inValue2).decoded.date.decoded.d );

	vint64 diff = (days1- days2)* 24 * 60 * 60 +
		(
		(((DateTimeEncoded&)inValue1).decoded.time.decoded.h * 3600L + 
		((DateTimeEncoded&)inValue1).decoded.time.decoded.m * 60 +
		((DateTimeEncoded&)inValue1).decoded.time.decoded.s ) 
		-
		(((DateTimeEncoded&)inValue2).decoded.time.decoded.h * 3600L + 
		((DateTimeEncoded&)inValue2).decoded.time.decoded.m * 60 +
		((DateTimeEncoded&)inValue2).decoded.time.decoded.s )
		);

	return diff;
}


/**********************************************************************************************/
// Function returns the current date, time and time separators and format of date.
//
void GetCurrentDateTimeFormat( 
	DTFormat&	outDTFormat )
{
	// we assign DEFAULT values into outDTFormat just in case.
	outDTFormat.mDateSep = '/';
	outDTFormat.mTimeSep = ':';
	outDTFormat.mDateFormat = kMDY;
	

#if FBL_MAC
{
	#if TARGET_OS_IPHONE
	{
		#warning "Not implemented for iPHONE. Still TODO probably."
	}
	#else
	{
		// Get Itlc
		Handle ItlcHandle = ::GetResource('itlc', 0);	
		if( ItlcHandle )
		{
			ItlcRecord* pItlcRecord = (ItlcRecord*)(*ItlcHandle);
			vint16 script = pItlcRecord->itlcSystem;

			::ReleaseResource( ItlcHandle );

			// Get Itlb
			Handle ItlbHandle = ::GetResource('itlb', script);
			if( ItlbHandle )
			{
				ItlbRecord* pItlbRecord = (ItlbRecord*)(*ItlbHandle);

				vint16 NumberID = pItlbRecord->itlbNumber;
				::ReleaseResource( ItlbHandle );

				// Get Itl0:
				Handle Itl0Handle = ::GetResource('itl0', NumberID);
				if( Itl0Handle )
				{
					Intl0Rec* pItl0Record = (Intl0Rec*)(*Itl0Handle);

					outDTFormat.mDateSep = pItl0Record->dateSep;
					outDTFormat.mTimeSep = pItl0Record->timeSep;
					outDTFormat.mDateFormat = (EDateFormat) pItl0Record->dateOrder;  // cast to 0,1,2 values: mdy, dmy,ymd
					::ReleaseResource( Itl0Handle );
				}
			}
		}
	}
	#endif // TARGET_OS_IPHONE
}	
#endif // FBL_MAC 

#if FBL_WIN
{
	TCHAR cc[2] = { 0,0 };

	::GetLocaleInfo( LOCALE_SYSTEM_DEFAULT, LOCALE_SDATE, cc, 2 );	
	outDTFormat.mDateSep = cc[0];

	::GetLocaleInfo( LOCALE_SYSTEM_DEFAULT, LOCALE_STIME, cc, 2 );	
	outDTFormat.mTimeSep = cc[0];

	::GetLocaleInfo( LOCALE_SYSTEM_DEFAULT, LOCALE_IDATE, cc, 2 );	
	outDTFormat.mDateFormat = (EDateFormat)(cc[0] - '0') ;  // cast to 0,1,2 values.
}
#endif // FBL_WIN

#if FBL_UNIX
{
	char* format = nl_langinfo( D_FMT );
	char* formatTime = nl_langinfo( T_FMT );		

	switch( format[1] )
	{
		case 'm':
		{
			if( format[4] == 'd' )
				outDTFormat.mDateFormat = kMDY;
			else
				outDTFormat.mDateFormat = kMYD;
		}break;

		case 'd':
		{
			if( format[4] == 'm' )
				outDTFormat.mDateFormat = kDMY;
			else
				outDTFormat.mDateFormat = kDYM;
		}break;

		case 'y':
		{
			if( format[4] == 'd' )
				outDTFormat.mDateFormat = kYDM;
			else
				outDTFormat.mDateFormat = kYMD;
		}break;

		default:
		// ERROR
		break;
	}

	outDTFormat.mDateSep = format[2];

	// http://www.valentina-db.com/bt/view.php?id=3158
	//
	// In some locales nl_langinfo( T_FMT ) can return "%r" instead of full 
	// description. Detailed: http://www.open-std.org/jtc1/sc22/wg14/www/docs/n715.htm
	// 
	// If it return something which is smallest "%H:%M:%S" then used default time separator ":"
	if( strlen( formatTime ) > 7 )
		outDTFormat.mTimeSep = formatTime[2];
}	
#endif // FBL_UNIX
}


#pragma mark -


/**********************************************************************************************/
// some kind of PATTERN string.
//  '-' - means a negative minus.
//	'n' - means a number.
//   2  - or other number after 'n' means limit of digits in the number. 
//	's' - separator
//	'e' - end
//
const char* kMDY_FormatStr = "-n2sn2s-ne";
const char* kDMY_FormatStr = "-n2sn2s-ne";
const char* kYMD_FormatStr = "-nsn2sn2e";

const char* kMYD_FormatStr = "-n2s-nsn2e";
const char* kDYM_FormatStr = "-n2s-nsn2e";
const char* kYDM_FormatStr = "-nsn2sn2e";


/**********************************************************************************************/
// Dispatcher. 
//
// Returns ZERO if string fits to current DateTime format. 
// Otherwise returns 1-based index  of a bad character.
//
vint32 CheckTimeStringFormat(
	const char* inTimeStr,
	const DTFormat* pDTFormat )
{
	char sep = (char) pDTFormat->mTimeSep;

	return CheckFormatOfTimeString( inTimeStr, sep );
}


/**********************************************************************************************/
// Dispatcher. 
//
// Returns ZERO if string fits to current DateTime format. 
// Otherwise returns 1-based index  of a bad character.
//
vint32 CheckDateStringFormat(
	const char* inDateStr,
	const DTFormat* pDTFormat )
{
	char sep = (char) pDTFormat->mDateSep;

	switch( pDTFormat->mDateFormat )		
		{
		case kMDY:
			return CheckFormatOfDateString( inDateStr, kMDY_FormatStr, sep );
		
		case kDMY:
			return CheckFormatOfDateString( inDateStr, kDMY_FormatStr, sep );

		case kYMD:
			return CheckFormatOfDateString( inDateStr, kYMD_FormatStr, sep );

		case kMYD:
			return CheckFormatOfDateString( inDateStr, kMYD_FormatStr, sep );

		case kDYM:
			return CheckFormatOfDateString( inDateStr, kDYM_FormatStr, sep );

		case kYDM:
			return CheckFormatOfDateString( inDateStr, kYDM_FormatStr, sep );

		default:
			return -1;
	}
}


/**********************************************************************************************/
// Dispatcher. 
//
// Returns ZERO if string fits to current DateTime format. 
// Otherwise returns 1-based index  of a bad character.
//
vint32 CheckDateTimeStringFormat(
	const char* inDateTimeStr,
	const DTFormat* pDTFormat )
{
	// We find SINGLE expected SPACE between DATA and TIME parts of DateTime value:
	char* p = (char*) inDateTimeStr;
	while( *p && *p != ' ' )
		++p;
		
	if( *p == 0 ) // we have not found SPACE:
	{
		// actually it is legal to have only DATE part, so 
		return CheckDateStringFormat( inDateTimeStr, pDTFormat );
	}
	else // space is found
	{
		if( p == inDateTimeStr ) // this is first charcter, ops
			return 1;
	
		// Now we have p that points to SPACE
		// We copy date part into tmp buffer to make it zero based.
		// Unfortunantly we cannot do just [*p = 0]. 
 		char DatePart[kDateStrMaxLength + 1];
		strncpy( DatePart, inDateTimeStr, kDateStrMaxLength );
 		DatePart[ FBL::Min((ptrdiff_t)kDateStrMaxLength, p - inDateTimeStr) ] = 0;
 		
		int res1 = CheckDateStringFormat( DatePart, pDTFormat );
		if( res1 == 0 ) // check time part:
		{
			vint32 res2 = CheckTimeStringFormat( p + 1, pDTFormat );
			if( res2 == 0 )
			{
				return 0;
			}
			else
			{
				return vint32((p - inDateTimeStr) + 1 + res2);
			}
		}
		else
		{
			return res1;
		}
	}
}


#pragma mark -


/**********************************************************************************************/
bool Check_IsOnlyDigit( char ch );
bool Check_IsOnlyDigit( char ch )
{
	switch( ch )
	{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			return true;
		
		default:
			return false;	
	}
}

/**********************************************************************************************/
// NUMBER: digit+
//
// i.e. one or more digits.
//
int Check_Number( const char*& inDateTimeStr, vuint8 inMaxDigits );

int Check_Number( 
	const char*& inDateTimeStr, vuint8 inMaxDigits )
{
	if( Check_IsOnlyDigit(*inDateTimeStr++) )
	{
		--inMaxDigits;
	
		while( Check_IsOnlyDigit(*inDateTimeStr++) )
		{
			if( inMaxDigits-- == 0 ) // ops, we have more digits than allowed for this number.
			{
				--inDateTimeStr;
				return 1; 
			}
		}
			
		--inDateTimeStr;	
		return 0; // all right, we have found number with at least ONE digit
	}
	else
	{
		--inDateTimeStr;
		return 1; // first chacter is bad.
	}
}


/**********************************************************************************************/
// RZ: LEXER for Date Time string parsing.
//  
// IDEA is: we specify some format string, then use it for check if given string correspond to it.
//
//		n - ANY NUMBER
//		n2 - NUMBER with 2 digits
//		n3 - NUMBER with 3 digits
//		s - separator	
//
// We have 3 formats of date:
//
//		YMD						- NUMBER s NUMBER2 s NUMBER2 e  
//		DMY						- NUMBER2 s NUMBER2 s -NUMBER e
//		MDY 					- NUMBER2 s NUMBER2 s -NUMBER e	
//
// RETURNS: 0 - on success
//			index of bad character, 1-based.
//
vint32 CheckFormatOfDateString(
	const char* inStr,
	const char* inFormatStr,
	char		inSeparator )
{	
	const char* pOriginalStart = inStr;
	
	char FormatCh;
	while( (FormatCh = *inFormatStr++) )
	{
		char StrCh = *inStr;
		if( StrCh == 0 ) 
			goto exit;
		
		switch( FormatCh )
		{
			case '-': // optional +/- can be
			{
				if( StrCh == FormatCh ) 
				{
					// MATCH! We have found it. Okay, go to the next chars in both strings:
					inStr++;
				}
				else
				{
					// not a problem, it was optional, do not move inStr,
					// and go to the next char in pattern.
				}							
			}break;
			
			case 'n': // NUMBER expected
			{
				// check if number have limit on digits:
				vuint8 limit = 255;
				if( isdigit( (unsigned char)*inFormatStr ) )
					limit = (*inFormatStr++) - '0';
			
				if( Check_Number( inStr, limit ) != 0 )
					goto exit;				
			}break;

			case 's': // separator expected
			{
				if( StrCh == inSeparator )
				{
					// MATCH! Okay, we are still on the track! 
					inStr++;
				}
				else // FATAL: 
				{					
					goto exit;
				}
			}break;
			
			case 'e':
			{
				goto exit;
			}break;
			
			default:
			{
				// we should not enter this because WE self make format string.
				FBL_CHECK( false );
			}break;	
					
		} // switch			
	} // while

exit:	
	if( *inStr == 0 && *inFormatStr == 0 )
	{
		return 0; // we have finish both strings.		
	}
	else // one of strings was not finished, so report current position of DT string:
	{		
		return vint32((inStr - pOriginalStart) + 1);
	}
}


/**********************************************************************************************/
// We have just 1 format for time:
//
//		HH[:MM[:SS[:MS]]]		NUMBER2 s NUMBER2 s NUMBER2 s NUMBER3 e
//
vint32 CheckFormatOfTimeString(
	const char* inStr,
	char		inSeparator )
{	
	const char* pOriginalStart = inStr;

	if( Check_Number(inStr, 2) == 0 ) // Hours
	{
		if( *inStr == inSeparator )
		{
			inStr++; 
			if( Check_Number(inStr, 2) == 0 ) // Minutes
			{
				if( *inStr == inSeparator )
				{
					inStr++; 
					if( Check_Number(inStr, 2) == 0 ) // Seconds
					{
						if( *inStr == inSeparator )
						{
							inStr++; 
							if( Check_Number(inStr, 3) == 0 ) // Mili-seconds
								if( *inStr == 0 ) return 0; else goto bad_exit;
							else
								goto bad_exit;
						}	
					}
					else
					{
						goto bad_exit;
					}					
				}

				if( *inStr == 0 ) return 0; else goto bad_exit;
			}
			else
			{
				goto bad_exit;
			}
		}
		
		if( *inStr == 0 ) return 0; else goto bad_exit;
	}
	// we have not found hours. ops.

bad_exit:
	return vint32((inStr - pOriginalStart) + 1);
}


#pragma mark -


/**********************************************************************************************/
bool CheckDateValueLimits( DateEncoded& d )
{
	if( d.encoded == 0 )
		return true;

	int month = d.decoded.m;
	if( 1 <= month && month <= 12 )
	{
		int day = d.decoded.d;
		if( 1 <= day && day <= 31 )
			return true;
	}
	
	return false;
}


/**********************************************************************************************/
bool CheckTimeValueLimits( TimeEncoded& t )
{
	if( t.encoded == 0 )
		return true;

	int hours = t.decoded.h;

	if( 0 <= hours && hours <= 24 )
	{
		int min = t.decoded.m;
		if( 0 <= min && min <= 59 )
		{
			int sec = t.decoded.s;
			if( 0 <= sec && sec <= 59 )
			{
				int ms = t.decoded.ms;
				if( 0 <= ms && ms <= 999 )
					return true;		
			}
		}
	}
	
	return false;
}


/**********************************************************************************************/
FBL_End_Namespace
