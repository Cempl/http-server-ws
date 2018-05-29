/**********************************************************************************************/
/* FBL_Type_Traits.h                                                      					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_Type_Traits_h 
	#define _FBL_Type_Traits_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Util_Strings/FBL_String_Len_Const.h>
#include <VShared/FBL/publ/Util_Strings/FBL_ustdio.h>
#include <VShared/FBL/publ/Util_Strings/FBL_UnicodeToFromNumeric.h>
#include <VShared/FBL/publ/Utilities/FBL_ByteSwap.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_VNETW_Packet.h>

// STD:
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif


/**********************************************************************************************/
FBL_Begin_Namespace

// Helpers:

/**********************************************************************************************/
inline bool IsTrueStr( const char* inString )
{
	return (inString) ?
			(	(inString[0] == 'T' || inString[0] == 't') &&	
				(inString[1] == 'R' || inString[1] == 'r') &&
				(inString[2] == 'U' || inString[2] == 'u') &&
				(inString[3] == 'E' || inString[3] == 'e') ) : false;
}


/**********************************************************************************************/
inline bool IsTrueStr( const UChar* inString )
{
	return (inString) ?
			(	(inString[0] == UChar('T') || inString[0] == UChar('t')) &&	
				(inString[1] == UChar('R') || inString[1] == UChar('r')) &&
				(inString[2] == UChar('U') || inString[2] == UChar('u')) &&
				(inString[3] == UChar('E') || inString[3] == UChar('e')) ) : false;
}


/**********************************************************************************************/
// Accumulate type-specific functions.
// 
template<class T>
struct type_traits
{
	typedef T      	 		value_type;
	typedef int      	 	int_type;

static void 				assign(T& c1, const T& c2);
static value_type* 			assign(value_type* s, vuint32 n, value_type a);

static vuint32 				length(const value_type* s);	// up to zero.
static vuint32 				max_text_length( void );

static bool 				eq		(const T& c1, const T& c2);
static bool 				lt		(const T& c1, const T& c2);

static int	 				compare	(const T& c1, const T& c2);
static int	 				compare_voids(const void* c1, const void* c2);
static int 					compare(const value_type* s1, const value_type* s2, vuint32 n);

static void 				swap( T& v1, T& v2 );

static const value_type* 	find(const value_type* s, vuint32 n, const T& a);

static value_type* 			move(value_type* s1, const value_type* s2, vuint32 n);
static value_type* 			copy(value_type* s1, const value_type* s2, vuint32 n);

static int_type 			to_int_type(const T& c);

static UChar*				to_str( const T* c1, UChar* outString, tslen inBufferChars );
static void					from_str( const UChar* inString, T* c1 );

static char*				to_str( const T* c1, char* outString, tslen inBufferChars );
static void					from_str( const char* inString, T* c1 );

static void					swap_bytes( T& t );

static void					From(  	  T* outpVal, I_PacketRcv* inPacket, bool inBlock );
static void					To( const T*  inpVal, I_PacketSnd* inPacket, bool inBlock );

static void					FromBinaryRepresentation(		T* outpVal, const char* inpBuffer );
static void					ToBinaryRepresentation(	const	T* inpVal,	char* outpBuffer );

static void					increment( value_type* inValue );

static const char* 			type_name_str( void );

};


/**********************************************************************************************/
// Implementation: type_traits<T>
//

/**********************************************************************************************/
template <class T>
inline void 
type_traits<T>::assign(T& c1, const T& c2)
{
	c1 = c2;
}


/**********************************************************************************************/
template <class T>
inline bool 
type_traits<T>::eq(const T& c1, const T& c2)
{
	return c1 == c2;
}


/**********************************************************************************************/
template <class T>
inline bool 
type_traits<T>::lt(const T& c1, const T& c2)
{
	return c1 < c2;
}


/**********************************************************************************************/
template <class T>
inline int 
type_traits<T>::compare(const T& c1, const T& c2)
{
	return eq(c1,c2) ? 0 
					 : (lt(c1,c2) ? -1 : +1);
}


/**********************************************************************************************/
template <class T>
inline int 
type_traits<T>::compare_voids(const void* c1, const void* c2)
{
	return eq( *(T*)c1, *(T*)c2) ? 0 
					 			 : (lt( *(T*)c1, *(T*)c2 ) ? -1 : +1);

}


/**********************************************************************************************/
template <class T>
inline int 
type_traits<T>::compare(const value_type* s1, const value_type* s2, vuint32 n)
{
	return memcmp( s1, s2, n );
}


/**********************************************************************************************/
template <class T>
inline vuint32 type_traits<T>::length(const value_type* s)
{
	vuint32 len = 0;
    
	while( !eq(*s, T()) )
	{
		++s;
		++len;
	}
	
    return len;
}


/**********************************************************************************************/
template <class T> 
inline const typename type_traits<T>::value_type*
type_traits<T>::find(const value_type* s, vuint32 n, const T& a)
{
	if (n == 0)
		return 0;
    
	const value_type* p = s;
	vuint32 i = 0;
    
	while (!eq(*p, a))
	{
		++i;
		if (i == n)
			return 0;
		++p;
	}
	
    return p;
}


/**********************************************************************************************/
template <class T>
inline typename type_traits<T>::value_type*
type_traits<T>::move(value_type* s1, const value_type* s2, vuint32 n)
{
	value_type* r = s1;
	
    if (s1 < s2)
	{
		for (vuint32 i = 0; i < n; ++i)
			assign(*s1++, *s2++);
	}
	else if (s2 < s1)
	{
		s1 += n;
		s2 += n;

		for (; n > 0; --n)
			assign(*--s1, *--s2);
	}

	return r;
}


/**********************************************************************************************/
template <class T>
inline typename type_traits<T>::value_type*
type_traits<T>::copy(value_type* s1, const value_type* s2, vuint32 n)
{
	value_type* r = s1;

	for (vuint32 i = 0; i < n; ++i)
		assign(*s1++, *s2++);

	return r;
}


/**********************************************************************************************/
template <class T>
inline typename type_traits<T>::value_type*
type_traits<T>::assign(value_type* s, vuint32 n, value_type a)
{
	value_type* r = s;
	for (vuint32 i = 0; i < n; ++i)
		assign(*s++, a);
	return r;
}


/**********************************************************************************************/
template <class T>
inline typename type_traits<T>::int_type
type_traits<T>::to_int_type(const T& c)
{
	return int_type(c);
}


#pragma mark -
#pragma mark === SPECIALIZATIONs of max_text_length() === 


/**********************************************************************************************/
template <> 
inline
vuint32 type_traits<bool>::max_text_length( void )
{
	return kBoolStrMaxLen;
}


/**********************************************************************************************/
template <> 
inline
vuint32 type_traits<vuint8>::max_text_length( void )
{
	return kUCharStrMaxLen;
}


/**********************************************************************************************/
template <> 
inline
vuint32 type_traits<vint16>::max_text_length( void )
{
	return kShortStrMaxLen; 
}


/**********************************************************************************************/
template <> 
inline
vuint32 type_traits<vuint16>::max_text_length( void )
{
	return kUShortStrMaxLen;  
}


/**********************************************************************************************/
template <> 
inline
vuint32 type_traits<vint32>::max_text_length( void )
{
	return kLongStrMaxLen;
}


/**********************************************************************************************/
template <> 
inline
vuint32 type_traits<vuint32>::max_text_length( void )
{
	return kULongStrMaxLen;
}


/**********************************************************************************************/
template <> 
inline
vuint32 type_traits<vint64>::max_text_length( void )
{
	return kLLongStrMaxLen; 
}


/**********************************************************************************************/
template <> 
inline
vuint32 type_traits<vuint64>::max_text_length( void )
{
	return kULLongStrMaxLen;
}


/**********************************************************************************************/
template <> 
inline
vuint32 type_traits<float>::max_text_length( void )
{
	return kFloatStrMaxLen;
}


/**********************************************************************************************/
template <> 
inline
vuint32 type_traits<double>::max_text_length( void )
{
	return kDoubleStrMaxLen;
}


#pragma mark -
#pragma mark === SPECIALIZATION of to_str() ===


/**********************************************************************************************/
template <> 
inline
UChar* type_traits<bool>::to_str( const bool* c1, UChar* outString, tslen inBufferChars )
{
	const vuint32 buff_size = kBoolStrMaxLen + 1;
	char bool_str[buff_size];

	int num = snprintf( bool_str, buff_size, "%lu", (unsigned long) *c1 );
		
	if( num > inBufferChars )
		num = inBufferChars;
		
	vu_a2u( bool_str, outString, num );

	return outString + num;
}


/**********************************************************************************************/
template <> 
inline
UChar* type_traits<vuint8>::to_str( const vuint8* c1, UChar* outString, tslen inBufferChars )
{
	const vuint32 buff_size = kUCharStrMaxLen + 1;
	char vuint8_str[buff_size];
	
	int num = snprintf( vuint8_str, buff_size, "%lu", (unsigned long) *c1 );

	// we need truncate string?
	if( num > inBufferChars )
		num = inBufferChars;
	
	vu_a2u( vuint8_str, outString, num );

	return outString + num;
}


/**********************************************************************************************/
template <> 
inline
UChar* type_traits<vint16>::to_str( const vint16* c1, UChar* outString, tslen inBufferChars )
{
	const vuint32 buff_size = kShortStrMaxLen + 1;
	char short_str[buff_size];

	int num = snprintf( short_str, buff_size, "%ld", (long) *c1 );

	if( num > inBufferChars )
		num = inBufferChars;

	vu_a2u( short_str, outString, num );

	return outString + num;
}


/**********************************************************************************************/
template <> 
inline
UChar* type_traits<vuint16>::to_str( const vuint16* c1, UChar* outString, tslen inBufferChars )
{
	const vuint32 buff_size = kUShortStrMaxLen + 1;
	char ushort_str[buff_size];
	
	int num = snprintf( ushort_str, buff_size, "%lu", (unsigned long) *c1 );
	
	if( num > inBufferChars )
		num = inBufferChars;

	vu_a2u( ushort_str, outString, num );

	return outString + num;
}


/**********************************************************************************************/
template <> 
inline
UChar* type_traits<vint32>::to_str( const vint32* c1, UChar* outString, tslen inBufferChars )
{
	const vuint32 buff_size = kLongStrMaxLen + 1;
	char long_str[buff_size];
	
	int num = snprintf( long_str, buff_size, "%ld", (long) *c1 );

	if( num > inBufferChars )
		num = inBufferChars;

	vu_a2u( long_str, outString, num );

	return outString + num;
}


/**********************************************************************************************/
template <> 
inline
UChar* type_traits<vuint32>::to_str( const vuint32* c1, UChar* outString, tslen inBufferChars )
{
	const vuint32 buff_size = kULongStrMaxLen + 1;
	char ulong_str[buff_size];
	
	int num = snprintf( ulong_str, buff_size, "%lu", (unsigned long) *c1 );

	if( num > inBufferChars )
		num = inBufferChars;

	vu_a2u(ulong_str, outString, num);

	return outString + num;
}


/**********************************************************************************************/
template <> 
inline
UChar* type_traits<vint64>::to_str( const vint64* c1, UChar* outString, tslen inBufferChars )
{
	const vuint32 buff_size = kLLongStrMaxLen + 1;
	char llong_str[buff_size];

#ifdef _MSC_VER
	int num = snprintf( llong_str, buff_size, "%I64i", *c1 );
#else // non-Visual
	int num = snprintf( llong_str, buff_size, "%lli", (long long int) *c1 );
#endif // _MSC_VER

	if( num > inBufferChars )
		num = inBufferChars;
		
	vu_a2u( llong_str, outString, num );

	return outString + num;
}


/**********************************************************************************************/
template <> 
inline
UChar* type_traits<vuint64>::to_str( const vuint64* c1, UChar* outString, tslen inBufferChars )
{
	const vuint32 buff_size = kULLongStrMaxLen + 1;
	char ullong_str[buff_size];

#ifdef _MSC_VER
	int num = snprintf( ullong_str, buff_size, "%I64u", *c1 );
#else // non-Visual
	int num = snprintf( ullong_str, buff_size, "%llu", (long long int) *c1 );
#endif // _MSC_VER

	if( num > inBufferChars )
		num = inBufferChars;
		
	vu_a2u( ullong_str, outString, num );

	return outString + num;
}


/**********************************************************************************************/
template <> 
inline
UChar* type_traits<float>::to_str( const float* c1, UChar* outString, tslen inBufferChars )
{
	const vuint32 buff_size = kFloatStrMaxLen + 1;
	char float_str[buff_size];
	
	int num = snprintf( float_str, buff_size, "%G", *c1 );
	
	if( num > inBufferChars )
		num = inBufferChars;
		
	vu_a2u( float_str, outString, num );

	return outString + num;
}


/**********************************************************************************************/
template <> 
inline
UChar* type_traits<double>::to_str( const double* c1, UChar* outString, tslen inBufferChars )
{
	const vuint32 buff_size = kDoubleStrMaxLen + 1;
	char double_str[buff_size];

	int num = snprintf( double_str, buff_size, "%G", *c1 );

	if( num > inBufferChars )
		num = inBufferChars;

	vu_a2u( double_str, outString, num );

	return outString + num;
}


/**********************************************************************************************/
/*template <> 
inline
inline UChar* type_traits<ldouble>::to_str( const ldouble* c1, UChar* outString )
{
	char ldouble_str[kDoubleStrMaxLen + 1];
	vuint32 num = (vuint32) sprintf( ldouble_str, "%G", *c1 );
	u_charsToUChars( ldouble_str, outString, kDoubleStrMaxLen + 1 );
	return outString + num;
}*/


#pragma mark -
#pragma mark === SPECIALIZATION of to_str() (ASCII) ===


/**********************************************************************************************/
template<> 
inline
char* type_traits<bool>::to_str( const bool* c1, char* outString, tslen inBufferChars )
{
	int num = snprintf( outString, (size_t)inBufferChars, "%lu", (unsigned long) *c1 );

	tslen char_count = ( num < 0 ) ? inBufferChars : num;
	outString[char_count] = '\0';

	return outString + char_count;
}


/**********************************************************************************************/
template <> 
inline
char* type_traits<vuint8>::to_str( const vuint8* c1, char* outString, tslen inBufferChars )
{
	int num = snprintf( outString, (size_t)inBufferChars, "%lu", (unsigned long) *c1 );

	if( num > inBufferChars )
		num = inBufferChars;

	outString[num] = '\0';

	return outString + num;
}


/**********************************************************************************************/
template <> 
inline
char* type_traits<vint16>::to_str( const vint16* c1, char* outString, tslen inBufferChars )
{
	int num = snprintf( outString, (size_t)inBufferChars, "%ld", (long) *c1 );
	
	if( num > inBufferChars )
		num = inBufferChars;

	outString[num] = '\0';

	return outString + num;
}


/**********************************************************************************************/
template <> 
inline
char* type_traits<vuint16>::to_str( const vuint16* c1, char* outString, tslen inBufferChars )
{
	int num = snprintf( outString, (size_t)inBufferChars, "%lu", (unsigned long) *c1 );
	
	if( num > inBufferChars )
		num = inBufferChars;

	outString[num] = '\0';

	return outString + num;
}


/**********************************************************************************************/
template <> 
inline
char* type_traits<vint32>::to_str( const vint32* c1, char* outString, tslen inBufferChars )
{
	int num = snprintf( outString, (size_t)inBufferChars, "%ld", (long) *c1 );
	
	if( num > inBufferChars )
		num = inBufferChars;

	outString[num] = '\0';

	return outString + num;
}


/**********************************************************************************************/
template <> 
inline
char* type_traits<vuint32>::to_str( const vuint32* c1, char* outString, tslen inBufferChars )
{
	int num = snprintf( outString, (size_t)inBufferChars, "%lu", (unsigned long) *c1 );
	
	if( num > inBufferChars )
		num = inBufferChars;

	outString[num] = '\0';

	return outString + num;
}


/**********************************************************************************************/
template <> 
inline
char* type_traits<vint64>::to_str( const vint64* c1, char* outString, tslen inBufferChars )
{
#ifdef _MSC_VER
	int num = snprintf( outString, (size_t)inBufferChars, "%I64i", *c1 );
#else // non-Visual
	int num = snprintf( outString, (size_t)inBufferChars, "%lli", (long long int) *c1 );
#endif

	tslen char_count = ( num < 0 ) ? inBufferChars : num;
	outString[char_count] = '\0';

	return outString + char_count;
}


/**********************************************************************************************/
template <> 
inline
char* type_traits<vuint64>::to_str( const vuint64* c1, char* outString, tslen inBufferChars )
{
#ifdef _MSC_VER
	int num = snprintf( outString, (size_t)inBufferChars, "%I64u", *c1 );
#else // non-Visual
	int num = snprintf( outString, (size_t)inBufferChars, "%llu", (long long int) *c1 );
#endif
	
	if( num > inBufferChars )
		num = inBufferChars;

	outString[num] = '\0';

	return outString + num;
}


/**********************************************************************************************/
template <> 
inline
char* type_traits<float>::to_str( const float* c1, char* outString, tslen inBufferChars )
{
	int num = snprintf( outString, (size_t)inBufferChars, "%G", *c1 );
	
	if( num > inBufferChars )
		num = inBufferChars;

	outString[num] = '\0';

	return outString + num;
}


/**********************************************************************************************/
template <> 
inline
char* type_traits<double>::to_str( const double* c1, char* outString, tslen inBufferChars )
{
	int num = snprintf( outString, (size_t)inBufferChars, "%G", *c1 );
	
	if( num > inBufferChars )
		num = inBufferChars;

	outString[num] = '\0';

	return outString + num;
}


/**********************************************************************************************/
template <> 
inline
char* type_traits<char>::to_str( const char* c1, char* outString, tslen inBufferChars )
{
	if( outString )
	{
		(void) strncpy( outString, c1, (size_t)inBufferChars );
		
		outString[inBufferChars] = '\0';

		return outString + strlen(outString);
	}
	else
	{
		return nullptr;
	}
}


#pragma mark -
#pragma mark === SPECIALIZATION of from_str() (Unicode) ===


/**********************************************************************************************/
template <> 
inline
void type_traits<bool>::from_str( const UChar* inString, bool* c1 )
{
	if( inString == nullptr )
	{
		*c1 = false;
	}
	else if( *inString == UChar('1') )
	{ 
		*c1 = true; 
		
	}else if( IsTrueStr( inString ) ) 
	{
		*c1 = true;
	} 
	else
	{
		 *c1 = false;
	}
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vuint8>::from_str( const UChar* inString, vuint8* c1 )
{
	*c1 = (vuint8) u_utol( inString );
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vint16>::from_str( const UChar* inString, vint16* c1 )
{
	if( IsTrueStr( inString ) )
		*c1 = (vint16) 1;
	else
		*c1 = (vint16) u_utol( inString );
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vuint16>::from_str( const UChar* inString, vuint16* c1 )
{
	if( IsTrueStr( inString ) )
		*c1 = (vuint16) 1;
	else
		*c1 = (vuint16) u_utol( inString );
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vint32>::from_str( const UChar* inString, vint32* c1 )
{
	if( IsTrueStr( inString ) )
		*c1 = 1;
	else
		*c1 = u_utol( inString );
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vuint32>::from_str( const UChar* inString, vuint32* c1 )
{
	if( IsTrueStr( inString ) )
		*c1 = 1;
	else
		*c1 = u_ustoul( inString );
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vint64>::from_str( const UChar* inString, vint64* c1 )
{
	if( IsTrueStr( inString ) )
		*c1 = 1;
	else
		*c1 = u_utoll(inString);
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vuint64>::from_str( const UChar* inString, vuint64* c1 )
{
	if( IsTrueStr( inString ) )
		*c1 = 1;
	else
		*c1 = u_ustoull(inString);
}


/**********************************************************************************************/
template <> 
inline
void type_traits<float>::from_str( const UChar* inString, float* c1 )
{
	if( IsTrueStr( inString ) )
		*c1 = (float) 1;
	else
		*c1 = (float) u_utof( inString );
}


/**********************************************************************************************/
template <> 
inline
void type_traits<double>::from_str( const UChar* inString, double* c1 )
{
	if( IsTrueStr( inString ) )
		*c1 = (double) 1;
	else
		*c1 = u_utof( inString );
}


#pragma mark -
#pragma mark === SPECIALIZATION of from_str() (ASCII) ===


/**********************************************************************************************/
template <> 
inline
void type_traits<bool>::from_str( const char* inString, bool* c1 )
{
	if( *inString == UChar('1') )
	{ 
		*c1 = true; 
		
	}
	else if( IsTrueStr( inString ) ) 
	{
		*c1 = true;
	}  
	else
	{
		 *c1 = false;
	}
}


/**********************************************************************************************/
template<>
inline 
void type_traits<vuint8>::from_str( const char* inString, vuint8* c1 )
{
	*c1 = (vuint8) atol( inString );
}


/**********************************************************************************************/
template<> 
inline 
void type_traits<vint16>::from_str( const char* inString, vint16* c1 )
{
	if( IsTrueStr( inString ) )
		*c1 = (vint16) 1;
	else
		*c1 = (vint16) atol( inString );
}


/**********************************************************************************************/
template<> 
inline 
void type_traits<vuint16>::from_str( const char* inString, vuint16* c1 )
{
	if( IsTrueStr( inString ) )
		*c1 = (vuint16) 1;
	else
		*c1 = (vuint16) atol( inString );
}


/**********************************************************************************************/
template<> 
inline 
void type_traits<vint32>::from_str( const char* inString, vint32* c1 )
{
	if( IsTrueStr( inString ) )
		*c1 = (vint32) 1;
	else
		*c1 = (vint32)atol( inString );
}


/**********************************************************************************************/
template<> 
inline 
void type_traits<vuint32>::from_str( const char* inString, vuint32* c1 )
{
	if( IsTrueStr( inString ) )
	{
		*c1 = (vuint32) 1;
	}
	else
	{
		char* stopChar = nullptr;
		*c1 = (vuint32)strtoul(inString, &stopChar, 10);
	}
}


/**********************************************************************************************/
template<> 
inline 
void type_traits<vint64>::from_str( const char* inString, vint64* c1 )
{
	if( IsTrueStr( inString ) )
	{
		*c1 = (vint64) 1;
	}
	else
	{
		#if _MSC_VER

			*c1 = (vint64) _atoi64(inString);

		#else // non-Visual:

			char* stopChar = nullptr;
			*c1 = (vint64) strtoll(inString, &stopChar, 10);

		#endif // _MSC_VER
	}
}


/**********************************************************************************************/
template<> 
inline 
void type_traits<vuint64>::from_str( const char* inString, vuint64* c1 )
{
	if( IsTrueStr( inString ) )
	{
		*c1 = (vuint64) 1;
	}
	else
	{
		#ifdef _MSC_VER

			#if _MSC_VER > 1310		
					sscanf_s(inString, "%I64u", c1);
			#else
					sscanf(inString, "%I64u", c1);
			#endif //_MSC_VER

		#else // non-Visual

			sscanf(inString, "%llu", (long long int*) c1);
//			char* stopChar = 0;
//			*c1 = (vuint64) strtoull(inString, &stopChar, 10);

		#endif //_MSC_VER
	}
}


/**********************************************************************************************/
template<> 
inline 
void type_traits<float>::from_str( const char* inString, float* c1 )
{
	if( IsTrueStr( inString ) )
		*c1 = (float) 1;
	else
		*c1 = static_cast<float>(atof(inString));
}


/**********************************************************************************************/
template<> 
inline 
void type_traits<double>::from_str( const char* inString, double* c1 )
{
	if( IsTrueStr( inString ) )
		*c1 = (double) 1;
	else
		*c1 = atof(inString);
}


/**********************************************************************************************/
#if U_SIZEOF_WCHAR_T == 2
template <> 
inline void type_traits<UChar>::from_str( const UChar* inString, UChar* c1 )
{
	// Since inString must be NULL terminated (???)
	// then we can simply use u_strcpy
	vu_strcpy(c1, inString);
}
#endif // U_SIZEOF_WCHAR_T == 2


#pragma mark -
#pragma mark === SPECIALIZATION of swap_bytes() ===


/**********************************************************************************************/
template <> 
inline
void type_traits<bool>::swap_bytes( bool& v )
{
	argused1(v);
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vuint8>::swap_bytes( vuint8& v )
{
	argused1(v);
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vint16>::swap_bytes( vint16& v )
{
	Swap_2_bytes(&v);
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vuint16>::swap_bytes( vuint16& v )
{
	Swap_2_bytes((vint16*)&v);
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vint32>::swap_bytes( vint32& v )
{
	Swap_4_bytes(&v);
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vuint32>::swap_bytes( vuint32& v )
{
	Swap_4_bytes((vint32*)&v);
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vint64>::swap_bytes( vint64& v )
{
	Swap_8_bytes((double*)&v);
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vuint64>::swap_bytes( vuint64& v )
{
	Swap_8_bytes((double*)&v);
}


/**********************************************************************************************/
template <> 
inline
void type_traits<float>::swap_bytes( float& v )
{
	Swap_4_bytes((vint32*)&v);
}


/**********************************************************************************************/
template <> 
inline
void type_traits<double>::swap_bytes( double& v )
{
	Swap_8_bytes((double*)&v);
}


#pragma mark -
#pragma mark === SPECIALIZATION of From() ===

/**********************************************************************************************/
template <> 
inline
void type_traits<bool>::From( bool* outpVal, I_PacketRcv* inPacket, bool inBlock )
{
	argused1(inBlock);
	(*outpVal) = inPacket->get_BoolParam();
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vuint8>::From( vuint8* outpVal, I_PacketRcv* inPacket, bool inBlock )
{
	argused1(inBlock);
	(*outpVal) = inPacket->get_UCharParam();
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vint16>::From( vint16* outpVal, I_PacketRcv* inPacket, bool inBlock )
{
	argused1(inBlock);
	(*outpVal) = inPacket->get_ShortParam();
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vuint16>::From( vuint16* outpVal, I_PacketRcv* inPacket, bool inBlock )
{
	argused1(inBlock);
	(*outpVal) = inPacket->get_UShortParam();
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vint32>::From( vint32* outpVal, I_PacketRcv* inPacket, bool inBlock )
{
	argused1(inBlock);
	(*outpVal) = inPacket->get_LongParam();
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vuint32>::From( vuint32* outpVal, I_PacketRcv* inPacket, bool inBlock )
{
	argused1(inBlock);
	(*outpVal) = inPacket->get_ULongParam();
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vint64>::From( vint64* outpVal, I_PacketRcv* inPacket, bool inBlock )
{
	argused1(inBlock);
	(*outpVal) = inPacket->get_LLongParam();
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vuint64>::From( vuint64* outpVal, I_PacketRcv* inPacket, bool inBlock )
{
	argused1(inBlock);
	(*outpVal) = inPacket->get_ULLongParam();
}


/**********************************************************************************************/
template <> 
inline
void type_traits<float>::From( float* outpVal, I_PacketRcv* inPacket, bool inBlock )
{
	argused1(inBlock);
	(*outpVal) = inPacket->get_FloatParam();
}


/**********************************************************************************************/
template <> 
inline
void type_traits<double>::From( double* outpVal, I_PacketRcv* inPacket, bool inBlock )
{
	argused1(inBlock);
	(*outpVal) = inPacket->get_DoubleParam();
}


#pragma mark -
#pragma mark === SPECIALIZATION of FromBinaryRepresentation() ===

/**********************************************************************************************/
template <> 
inline
void type_traits<bool>::FromBinaryRepresentation( bool* outpVal, const char* inpBuffer )
{
	(*outpVal) = *( reinterpret_cast<const bool*>( inpBuffer ) );
}

/**********************************************************************************************/
template <> 
inline
void type_traits<vuint8>::FromBinaryRepresentation( vuint8* outpVal, const char* inpBuffer )
{
	(*outpVal) = *( reinterpret_cast<const vuint8*>( inpBuffer ) );
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vint16>::FromBinaryRepresentation( vint16* outpVal, const char* inpBuffer )
{
	(*outpVal) = *( reinterpret_cast<const vint16*>( inpBuffer ) );
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vuint16>::FromBinaryRepresentation( vuint16* outpVal, const char* inpBuffer )
{
	(*outpVal) = *( reinterpret_cast<const vuint16*>( inpBuffer ) );
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vint32>::FromBinaryRepresentation( vint32* outpVal, const char* inpBuffer )
{
	(*outpVal) = *( reinterpret_cast<const vint32*>( inpBuffer ) );
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vuint32>::FromBinaryRepresentation( vuint32* outpVal, const char* inpBuffer )
{
	(*outpVal) = *( reinterpret_cast<const vuint32*>( inpBuffer ) );
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vint64>::FromBinaryRepresentation( vint64* outpVal, const char* inpBuffer )
{
	(*outpVal) = *( reinterpret_cast<const vint64*>( inpBuffer ) );
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vuint64>::FromBinaryRepresentation( vuint64* outpVal, const char* inpBuffer )
{
	(*outpVal) = *( reinterpret_cast<const vuint64*>( inpBuffer ) );
}


/**********************************************************************************************/
template <> 
inline
void type_traits<float>::FromBinaryRepresentation( float* outpVal, const char* inpBuffer )
{
	(*outpVal) = *( reinterpret_cast<const float*>( inpBuffer ) );
}


/**********************************************************************************************/
template <> 
inline
void type_traits<double>::FromBinaryRepresentation( double* outpVal, const char* inpBuffer )
{
	(*outpVal) = *( reinterpret_cast<const double*>( inpBuffer ) );
}


#pragma mark -
#pragma mark === SPECIALIZATION of To() ===

/**********************************************************************************************/
template <> 
inline
void type_traits<bool>::To( const bool* inpVal, I_PacketSnd* inPacket, bool inBlock )
{
	argused1(inBlock);
	inPacket->put_BoolParam(*inpVal);
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vuint8>::To( const vuint8* inpVal, I_PacketSnd* inPacket, bool inBlock )
{
	argused1(inBlock);
	inPacket->put_UCharParam(*inpVal);
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vint16>::To( const vint16* inpVal, I_PacketSnd* inPacket, bool inBlock )
{
	argused1(inBlock);
	inPacket->put_ShortParam(*inpVal);
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vuint16>::To( const vuint16* inpVal, I_PacketSnd* inPacket, bool inBlock )
{
	argused1(inBlock);
	inPacket->put_UShortParam(*inpVal);
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vint32>::To( const vint32* inpVal, I_PacketSnd* inPacket, bool inBlock )
{
	argused1(inBlock);
	inPacket->put_LongParam(*inpVal);
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vuint32>::To( const vuint32* inpVal, I_PacketSnd* inPacket, bool inBlock )
{
	argused1(inBlock);
	inPacket->put_ULongParam(*inpVal);
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vint64>::To( const vint64* inpVal, I_PacketSnd* inPacket, bool inBlock )
{
	argused1(inBlock);
	inPacket->put_LLongParam(*inpVal);
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vuint64>::To( const vuint64* inpVal, I_PacketSnd* inPacket, bool inBlock )
{
	argused1(inBlock);
	inPacket->put_ULLongParam(*inpVal);
}


/**********************************************************************************************/
template <> 
inline
void type_traits<float>::To( const float* inpVal, I_PacketSnd* inPacket, bool inBlock )
{
	argused1(inBlock);
	inPacket->put_FloatParam(*inpVal);
}


/**********************************************************************************************/
template <> 
inline
void type_traits<double>::To( const double* inpVal, I_PacketSnd* inPacket, bool inBlock )
{
	argused1(inBlock);
	inPacket->put_DoubleParam(*inpVal);
}


#pragma mark -
#pragma mark === SPECIALIZATION of ToBinaryRepresentation() ===

/**********************************************************************************************/
template <> 
inline
void type_traits<bool>::ToBinaryRepresentation( const bool* inpVal, char* outpBuffer )
{
	memcpy( outpBuffer, inpVal, sizeof(bool) );
}

/**********************************************************************************************/
template <> 
inline
void type_traits<vuint8>::ToBinaryRepresentation( const vuint8* inpVal, char* outpBuffer )
{
	memcpy( outpBuffer, inpVal, sizeof(vuint8) );
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vint16>::ToBinaryRepresentation( const vint16* inpVal, char* outpBuffer )
{
	memcpy( outpBuffer, inpVal, sizeof(vint16) );
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vuint16>::ToBinaryRepresentation( const vuint16* inpVal, char* outpBuffer )
{
	memcpy( outpBuffer, inpVal, sizeof(vuint16) );
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vint32>::ToBinaryRepresentation( const vint32* inpVal, char* outpBuffer )
{
	memcpy( outpBuffer, inpVal, sizeof(vint32) );
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vuint32>::ToBinaryRepresentation( const vuint32* inpVal, char* outpBuffer )
{
	memcpy( outpBuffer, inpVal, sizeof(vuint32) );
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vint64>::ToBinaryRepresentation( const vint64* inpVal, char* outpBuffer )
{
	memcpy( outpBuffer, inpVal, sizeof(vint64) );
}


/**********************************************************************************************/
template <> 
inline
void type_traits<vuint64>::ToBinaryRepresentation( const vuint64* inpVal, char* outpBuffer )
{
	memcpy( outpBuffer, inpVal, sizeof(vuint64) );
}


/**********************************************************************************************/
template <> 
inline
void type_traits<float>::ToBinaryRepresentation( const float* inpVal, char* outpBuffer )
{
	memcpy( outpBuffer, inpVal, sizeof(float) );
}


/**********************************************************************************************/
template <> 
inline
void type_traits<double>::ToBinaryRepresentation( const double* inpVal, char* outpBuffer )
{
	memcpy( outpBuffer, inpVal, sizeof(double) );
}


#pragma mark -
#pragma mark === SPECIALIZATIONs of increment() ===


/**********************************************************************************************/
template <class T>
inline
void type_traits<T>::increment( value_type* inValue )
{
	(*inValue)++;
}


/**********************************************************************************************/
template <> 
inline
void type_traits<bool>::increment( bool* inValue )
{
	if( *inValue == false )
		*inValue = true;
}


#pragma mark -
#pragma mark === type_name_str() specialization ===


/**********************************************************************************************/
// This method allows us to get some string that have type name to recognize it in e.g. logs.
// At least for vkernel tests.
//
template <>
inline const char* type_traits<bool>::type_name_str( void ) { return "bool"; }

template <>
inline const char* type_traits<vuint8>::type_name_str( void ) { return "vuint8"; }

template <>
inline const char* type_traits<vint16>::type_name_str( void ) { return "vint16"; }

template <>
inline const char* type_traits<vuint16>::type_name_str( void ) { return "vuint16"; }

template <>
inline const char* type_traits<vint32>::type_name_str( void ) { return "vint32"; }

template <>
inline const char* type_traits<vuint32>::type_name_str( void ) { return "vuint32"; }

template <>
inline const char* type_traits<vint64>::type_name_str( void ) { return "vint64"; }

template <>
inline const char* type_traits<vuint64>::type_name_str( void ) { return "vuint64"; }

template <>
inline const char* type_traits<float>::type_name_str( void ) { return "float"; }

template <>
inline const char* type_traits<double>::type_name_str( void ) { return "double"; }


#pragma mark -
#pragma mark === typedefs


/**********************************************************************************************/
typedef type_traits<bool>		bool_traits;
typedef type_traits<vuint8>		byte_traits;

typedef type_traits<vint16>		short_traits;
typedef type_traits<vuint16>	ushort_traits;

typedef type_traits<vint32>		long_traits;
typedef type_traits<vuint32>	ulong_traits;

typedef type_traits<vint64>		llong_traits;
typedef type_traits<vuint64>	ullong_traits;

typedef type_traits<float>		float_traits;
typedef type_traits<double>		double_traits;

//typedef type_traits<UChar>	string_traits;
//typedef short_traits			string_traits;
typedef type_traits<vuint8>		blob_traits;


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_Type_Traits_h
