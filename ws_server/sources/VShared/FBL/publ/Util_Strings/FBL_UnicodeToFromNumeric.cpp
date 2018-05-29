/**********************************************************************************************/
/* FBL_UnicodeToFromNumeric.cpp                                           					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Util_Strings/FBL_ustdio.h>
#include <VShared/FBL/publ/Util_Strings/FBL_UnicodeToFromNumeric.h>

#include <VShared/FBL/publ/Util_Strings/FBL_String_Len_Const.h>

#include <stdlib.h>
#include <stdio.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
//	UChar*  =>  vint32
//
vint32 u_utol( const UChar* inStr )  
{
	if( inStr )
	{	
		const vuint32 buff_size = kLongStrMaxLen + 1;
		char buf[buff_size];
	
		vuint32 len = (vuint32) vu_strlen( inStr );

		// CONVERT to char* string
		vu_u2a( inStr, buf, (tslen) Min(len, kLongStrMaxLen) );
		
		// use ANSI char* library to convert to number
		return atol( buf );
	}
	else
	{
		return 0;
	}
}


/**********************************************************************************************/
//	vint32  =>  UChar*
//
// Not safe. 
// Safe is: type_traits<vint32>::to_str()
//
UChar* u_ltou( vint32 inNum, UChar* outBuf )
{
	int len;

	const vuint32 buff_size = kLongStrMaxLen + 1;
	char buf[buff_size + 1];

	// use ANSI char* library to convert from the number
	len = snprintf( buf, buff_size, "%ld", (long) inNum );
		
	// CONVERT char* to UChar* string
	vu_a2u( buf, outBuf, len );		// not safe. We don't know size of buffer.

	return outBuf + len;
}


/**********************************************************************************************/
//	UChar*  =>  vuint32
//
vuint32 u_ustoul( const UChar* inStr )
{
	if( inStr )
	{
		char buf[kULongStrMaxLen + 1];

		vuint32 len = (vuint32) vu_strlen( inStr );
		
		// CONVERT to char* string
		vu_u2a( inStr, buf, (tslen) Min(len, kULongStrMaxLen) );

		// use ANSI char* library to convert to number
		char* stop_char = nullptr;    
		return strtoul( buf, &stop_char, 10 );
	}
	else
	{
		return 0;
	}
}


/**********************************************************************************************/
//	vuint32  =>  UChar*
//
UChar* u_ultous( vuint32 inNum, UChar* outBuf )
{
	const vuint32 buff_size = kULongStrMaxLen + 1;
	char buf[kULongStrMaxLen + 1];

	// use ANSI char* library to convert from the number
	int len = snprintf( buf, buff_size, "%lu", (unsigned long) inNum );

	// CONVERT char* to UChar* string
	vu_a2u( buf, outBuf, len );

	return outBuf + len;
}


/**********************************************************************************************/
//	UChar*  =>  double
//
double u_utof( const UChar* inStr )
{
	char buf[kDoubleStrMaxLen + 1];
	if( inStr )
	{
		vuint32 len = (vuint32) vu_strlen(inStr);
		vu_u2a(inStr, buf, tslen( Min(len, kDoubleStrMaxLen) ) );

		return atof(buf);
	}
	else 
	{
		return 0.0;
	}
}


/**********************************************************************************************/
//	double  =>  UChar*
//
UChar* u_ftos( double inNum, UChar* outBuf )
{
	const vuint32 buff_size = kDoubleStrMaxLen + 1;
	char buf[buff_size];

	// use ANSI char* library to convert from the number
	int len = snprintf( buf, buff_size, "%g", inNum );	

	// CONVERT char* to UChar* string
	vu_a2u(buf, outBuf, len );

	return outBuf + len;
}


/**********************************************************************************************/
//	UChar*  =>  vint64
//
vint64 u_utoll( const UChar* inStr )
{
	char buf[kLLongStrMaxLen + 1];
	if( inStr )
	{
		vuint32 len = (vuint32) vu_strlen(inStr);
		vu_u2a(inStr, buf, tslen( Min(len, kLLongStrMaxLen) ) );
	}
	else
	{
		return 0;
	}

#ifdef _MSC_VER

	vint64 value = (vint64) _atoi64( buf );

#else

	char* stopChar = nullptr;
	vint64 value = (vint64) strtoll( buf, &stopChar, 10 );

#endif //_MSC_VER

	return value;
}


/**********************************************************************************************/
//	vint64  =>  UChar*
//
UChar* u_lltou( vint64 inNum, UChar* outBuf ) 
{
	const vuint32 buff_size = kLLongStrMaxLen + 1;
	char str[buff_size + 1];
	
	// use ANSI char* library to convert from the number
#ifdef 	_MSC_VER
	int len = snprintf( str, buff_size, "%I64i", inNum );
#else
	int len = snprintf( str, buff_size, "%lli", (long long) inNum );
#endif

	// CONVERT char* to UChar* string
	vu_a2u(str, outBuf, len );

	return outBuf + len;
}


/**********************************************************************************************/
//	UChar*  =>  vuint64
//
vuint64 u_ustoull( const UChar* inStr )
{
	char buf[kULLongStrMaxLen + 1];
	
	if( inStr )
	{
		vuint32 len = (vuint32) vu_strlen(inStr);
		vu_u2a(inStr, buf, tslen( Min(len, kULLongStrMaxLen)) );
	}
	else
	{
		return 0;
	}
	
#ifdef _MSC_VER

	vuint64 value = 0;
	sscanf(buf, "%I64u", &value);

#else

	unsigned long long value = 0;
	sscanf(buf, "%llu", &value);
    
#endif //_MSC_VER

	return value;
}


/**********************************************************************************************/
//	vuint64  =>  UChar*
//
UChar* u_ulltous( vuint64 inNum, UChar* outBuf )
{
	const vuint32 buff_size = kULLongStrMaxLen + 1;
	char str[buff_size];
	
	// use ANSI char* library to convert from the number
#ifdef 	_MSC_VER
	int len = snprintf( str, buff_size, "%I64u", inNum );
#else
	int len = snprintf( str, buff_size, "%llu", (unsigned long long) inNum );
#endif

	// CONVERT char* to UChar* string
	vu_a2u(str, outBuf, len );

	return outBuf + len;
}


/**********************************************************************************************/
FBL_End_Namespace
