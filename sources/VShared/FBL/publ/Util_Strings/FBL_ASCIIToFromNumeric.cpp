/**********************************************************************************************/
/* FBL_ASCIIToFromNumeric.cpp																  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/


#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Util_Strings/FBL_ASCIIToFromNumeric.h>

#include <stdlib.h>
#include <stdio.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
vint32 va_atol( const char* inStr )
{
	return atol(inStr);
}


/**********************************************************************************************/
char* va_ltoa( vint32 inNum, char* outBuf )
{
	int len = sprintf(outBuf, "%ld", (long)inNum);
	return outBuf + len;
}


/**********************************************************************************************/
vuint32 va_atoul( const char* inStr )
{
	if( inStr )
	{
		char* stopChar = nullptr;    
		return strtoul(inStr, &stopChar, 10);
	}
	else
	{
		return 0;
	}
}


/**********************************************************************************************/
char* va_ultoa( vuint32 inNum, char* outBuf )
{
	int len = sprintf(outBuf, "%lu", (unsigned long) inNum);

	return outBuf + len;
}


/**********************************************************************************************/
double va_atof( const char* inStr )
{
	return atof(inStr);
}


/**********************************************************************************************/
char* va_ftoa( double inNum, char* outBuf )
{
	int len = sprintf( outBuf, "%g", inNum );	

	return outBuf + len;
}


/**********************************************************************************************/
vint64 va_atoll( const char* inStr )
{
#ifdef _MSC_VER
	vint64 value = 0;
	sscanf(inStr, "%I64i", &value);
#else
	long long value = 0;
	sscanf(inStr, "%lli", &value);
#endif //_MSC_VER

	return value;
}


/**********************************************************************************************/
char* va_lltoa( vint64 inNum, char* outBuf )
{
#ifdef 	_MSC_VER
	int len = sprintf( outBuf, "%I64i", inNum );
#else
	int len = sprintf( outBuf, "%lli", (long long) inNum );
#endif

	return outBuf + len;
}


/**********************************************************************************************/
vuint64	va_atoull( const char* inStr )
{
	if( !inStr )
		return 0;

#ifdef _MSC_VER
	vuint64 value = 0;
	sscanf(inStr, "%I64u", &value);
#else
	unsigned long long value = 0;
	sscanf(inStr, "%llu", &value);
#endif //_MSC_VER

	return value;
}


/**********************************************************************************************/
char* va_ulltoa( vuint64 inNum, char* outBuf )
{
#ifdef 	_MSC_VER
	int len = sprintf( outBuf, "%I64u", inNum );
#else
	int len = sprintf( outBuf, "%llu", (unsigned long long) inNum );
#endif

	return outBuf + len;
}


/**********************************************************************************************/
FBL_End_Namespace
