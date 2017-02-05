/**********************************************************************************************/
/* FBL_ustdio.h																				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_ustdio_h
	#define _FBL_ustdio_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Util_classes/FBL_String.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Localizable.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// At first we declare types of function pointers:

// detectUnicodeSignature
typedef const char* (*vu_detectUnicodeSignature_ptr)(
						const char* source,
                        vint32 sourceLength,
                        vint32 *signatureLength,
                        UErrorCode *pErrorCode );

// is...
typedef	bool	(*vu_isalpha_ptr)( UChar32 inCh );
typedef	bool	(*vu_isdigit_ptr)( UChar32 inCh );
typedef	bool	(*vu_isspace_ptr)( UChar32 inCh );

// sprintf
typedef int 	(*vu_sprintf_ptr)(UChar* s, const UChar* format, va_list args );

// strchr
typedef	UChar*  (*vu_strchr_ptr) ( const UChar* inStr, UChar ch );
typedef	UChar* 	(*vu_strrchr_ptr)( const UChar* inStr, UChar ch );

// strcmp
typedef	int 	(*vu_strcmp_uu_ptr)( const UChar* inStr1, const UChar* inStr2 );
typedef	int 	(*vu_strcmp_ua_ptr)( const UChar* inStr1, const char*  inStr2 );
typedef	int 	(*vu_strcmp_au_ptr)( const char*  inStr1, const UChar* inStr2 );

// strcoll
typedef vint32 	(*vu_strcoll_uu_ptr)(
					const UChar *inStr1, tslen inLen1,
					const UChar *inStr2, tslen inLen2, 
					I_Collator* inCollator );

typedef vint32 	(*vu_strcoll_ss_ptr)(
					const String& inStr1,
					const String& inStr2, 
					I_Collator* inCollator );

// strcpy
typedef	UChar* 	(*vu_strcpy_uu_ptr)( UChar* inDest, const UChar* inSource );
typedef	UChar* 	(*vu_strcpy_ua_ptr)( UChar* inDest, const char*  inSource );

// stricmp
typedef	int 	(*vu_stricmp_uu_ptr)( const UChar* inStr1, const UChar* inStr2 );
typedef	int 	(*vu_stricmp_ua_ptr)( const UChar* inStr1, const char*  inStr2 );
typedef	int 	(*vu_stricmp_au_ptr)( const char*  inStr1, const UChar* inStr2 );

// strlen
typedef tslen 	(*vu_strlen_ptr)( const UChar* inStr );

// strncat
typedef	UChar* 	(*vu_strncat_ptr)( UChar* inDest, const UChar* inSource, tslen inLen );

// strncmp
typedef	int 	(*vu_strncmp_uu_ptr)( const UChar* inStr1, const UChar* inStr2, tslen inCount );
typedef	int 	(*vu_strncmp_ua_ptr)( const UChar* inStr1, const char* inStr2, tslen inCount );
typedef	int 	(*vu_strncmp_au_ptr)( const char* inStr1, const UChar* inStr2, tslen inCount );

// strncpy
typedef	UChar* 	(*vu_strncpy_ptr)( UChar* inDest, const UChar* inSource, tslen inLen );

// vu_u2a, vu_a2u
typedef void 	(*vu_u2a_ptr)( const UChar* inUStr, char* ioAStr, tslen inLen, bool inSetTermNull );
typedef void 	(*vu_a2u_ptr)( const char* inAStr, UChar* ioUStr, tslen inLen, bool inSetTermNull );


// Unicode <> Default Code Page
typedef vint32 	(*vu_FromUnicodeToDefaultCP_ptr)( 
					const UChar* inUStr, tslen inULen, char* outBuf, tslen inLen );
					
typedef vint32 	(*vu_FromDefaultCPToUnicode_ptr)( 
					const char* inStr, tslen inLen, UChar* outBuf, tslen inULen );

// UChar <=> wchar_t
typedef UChar* (*vu_strFromWCS_ptr)( 
						UChar*			dest,
			            vint32			destCapacity, 
			            vint32*			pDestLength,
			            const wchar_t*	src,
			            vint32			srcLength,
			            UErrorCode*		pErrorCode );

typedef wchar_t* 	(*vu_strToWCS_ptr)( 
						wchar_t*		dest, 
			           	vint32			destCapacity,
			           	vint32*			pDestLength,
			           	const UChar*	src, 
			           	vint32			srcLength,
			           	UErrorCode*		pErrorCode );


typedef UChar 	(*vu_toupper_ptr)( UChar inChar );

#pragma mark -


/**********************************************************************************************/
// detectUnicodeSignature
FBL_SHARED_EXP extern vu_detectUnicodeSignature_ptr pvu_detectUnicodeSignature;

// is...
FBL_SHARED_EXP extern vu_isalpha_ptr 		pvu_isalpha;
FBL_SHARED_EXP extern vu_isdigit_ptr 		pvu_isdigit;
FBL_SHARED_EXP extern vu_isspace_ptr 		pvu_isspace;

// sprintf
//FBL_SHARED_EXP extern vu_sprintf_ptr		pvu_sprintf;

// strchr
FBL_SHARED_EXP extern vu_strchr_ptr			pvu_strchr;
FBL_SHARED_EXP extern vu_strrchr_ptr		pvu_strrchr;

// strcmp
FBL_SHARED_EXP extern vu_strcmp_uu_ptr		pvu_strcmp_uu;
FBL_SHARED_EXP extern vu_strcmp_ua_ptr		pvu_strcmp_ua;
FBL_SHARED_EXP extern vu_strcmp_au_ptr		pvu_strcmp_au;

// strcoll
FBL_SHARED_EXP extern vu_strcoll_uu_ptr		pvu_strcoll_uu;
FBL_SHARED_EXP extern vu_strcoll_ss_ptr		pvu_strcoll_ss;

// strcpy
FBL_SHARED_EXP extern vu_strcpy_uu_ptr		pvu_strcpy_uu;
FBL_SHARED_EXP extern vu_strcpy_ua_ptr		pvu_strcpy_ua;

// stricmp
FBL_SHARED_EXP extern vu_stricmp_uu_ptr		pvu_stricmp_uu;
FBL_SHARED_EXP extern vu_stricmp_ua_ptr		pvu_stricmp_ua;
FBL_SHARED_EXP extern vu_stricmp_au_ptr		pvu_stricmp_au;

// strlen
FBL_SHARED_EXP extern vu_strlen_ptr 		pvu_strlen;

// strncat
FBL_SHARED_EXP extern vu_strncat_ptr		pvu_strncat;

// strncmp
FBL_SHARED_EXP extern vu_strncmp_uu_ptr		pvu_strncmp_uu;
FBL_SHARED_EXP extern vu_strncmp_ua_ptr		pvu_strncmp_ua;
FBL_SHARED_EXP extern vu_strncmp_au_ptr		pvu_strncmp_au;

// strncpy
FBL_SHARED_EXP extern vu_strncpy_ptr		pvu_strncpy;

// vu_u2a, vu_a2u
FBL_SHARED_EXP extern vu_u2a_ptr			pvu_u2a;
FBL_SHARED_EXP extern vu_a2u_ptr			pvu_a2u;

// Unicode <> Default Code Page
FBL_SHARED_EXP extern vu_FromUnicodeToDefaultCP_ptr	pFromUnicodeToDefaultCP;
FBL_SHARED_EXP extern vu_FromDefaultCPToUnicode_ptr	pFromDefaultCPToUnicode;

// UChar <=> wchar_t
FBL_SHARED_EXP extern vu_strFromWCS_ptr		pvu_strFromWCS;
FBL_SHARED_EXP extern vu_strToWCS_ptr		pvu_strToWCS;

// toUpper
FBL_SHARED_EXP extern vu_toupper_ptr 		pvu_toupper;


#pragma mark -


/**********************************************************************************************/
// Also we provide inline methods-wrappers to above functon pointers.
// Thanks to this we again get back ability to have overloaded function names:


/**********************************************************************************************/
// detectUnicodeSignature
//
inline const char* vu_detectUnicodeSignature(
	const char*		source,
	vint32			sourceLength,
	vint32*			signatureLength,
	UErrorCode*		pErrorCode )
{
	return pvu_detectUnicodeSignature(source, sourceLength, signatureLength, pErrorCode );
}


/**********************************************************************************************/
// is...
//
inline bool vu_isalpha( UChar32 inCh )	{ return pvu_isalpha( inCh ); }
inline bool vu_isdigit( UChar32 inCh )	{ return pvu_isdigit( inCh ); }
inline bool vu_isspace( UChar32 inCh )	{ return pvu_isspace( inCh ); }


/**********************************************************************************************/
/*inline int vu_sprintf(UChar* s, const UChar* format, ...)
{	
	va_list ap;
	
	va_start( ap, format );
	
	int retVal = pvu_sprintf( s, format, ap );
	
	va_end(ap);
	
	return retVal;	 
};*/


/**********************************************************************************************/
// strchr
//
inline UChar* vu_strchr( const UChar* inStr, UChar ch )
	{ return pvu_strchr( inStr, ch ); }

inline UChar* vu_strrchr( const UChar* inStr, UChar ch )
	{ return pvu_strrchr( inStr, ch ); }


/**********************************************************************************************/
// strcmp
//
inline int vu_strcmp( const UChar* inStr1, const UChar* inStr2 )
	{ return pvu_strcmp_uu( inStr1, inStr2 ); }

inline int vu_strcmp( const UChar* inStr1, const char*  inStr2 )
	{ return pvu_strcmp_ua( inStr1, inStr2 ); }

inline int vu_strcmp( const char* inStr1, const UChar* inStr2 )
	{ return pvu_strcmp_au( inStr1, inStr2 ); }


/**********************************************************************************************/
// strcoll
//
inline vint32 vu_strcoll( 
	const UChar *inStr1, tslen inLen1,
	const UChar *inStr2, tslen inLen2, 
	I_Collator* inCollator = nullptr )
{
	return pvu_strcoll_uu( inStr1, inLen1, inStr2, inLen2, inCollator );
}

inline vint32 vu_strcoll( 
	const String& inStr1,
	const String& inStr2, 
	I_Collator* inCollator = nullptr )
{
	return pvu_strcoll_ss( inStr1, inStr2, inCollator );
}


/**********************************************************************************************/
// strcpy
//
inline UChar* vu_strcpy( UChar* inDest, const UChar* inSource )
	{ return pvu_strcpy_uu( inDest, inSource ); }

inline UChar* vu_strcpy( UChar* inDest, const char* inSource )
	{ return pvu_strcpy_ua( inDest, inSource ); }


/**********************************************************************************************/
// strcmp
//
inline int vu_stricmp( const UChar* inStr1, const UChar* inStr2 )
	{ return pvu_stricmp_uu( inStr1, inStr2 ); }

inline int vu_stricmp( const UChar* inStr1, const char*  inStr2 )
	{ return pvu_stricmp_ua( inStr1, inStr2 ); }

inline int vu_stricmp( const char*  inStr1, const UChar* inStr2 )
	{ return pvu_stricmp_au( inStr1, inStr2 ); }


/**********************************************************************************************/
// strlen
//
inline tslen vu_strlen( const UChar* inStr )
	{ return pvu_strlen( inStr ); }


/**********************************************************************************************/
// strncat
//
inline UChar* vu_strncat( UChar* inDest, const UChar* inSource, tslen inLen )
	{ return pvu_strncat( inDest, inSource, inLen ); }


/**********************************************************************************************/
inline int vu_strncmp( const UChar* inStr1, const UChar* inStr2, tslen inCount )
	{ return pvu_strncmp_uu( inStr1, inStr2, inCount ); }

inline int vu_strncmp( const UChar* inStr1, const char* inStr2, tslen inCount )
	{ return pvu_strncmp_ua( inStr1, inStr2, inCount ); }

inline int vu_strncmp( const char* inStr1, const UChar* inStr2, tslen inCount )
	{ return pvu_strncmp_au( inStr1, inStr2, inCount ); }


/**********************************************************************************************/
// strncpy
//
inline UChar* vu_strncpy( UChar* inDest, const UChar* inSource, tslen inLen )
	{ return pvu_strncpy( inDest, inSource, inLen ); }


#pragma mark -


/**********************************************************************************************/
/**	Converts Unicode string to ASCII string.
	Can be used only for the so-called "invariant" characters, 
	i.e. characters with codes 0 - 127. */
inline void vu_u2a( 
	const UChar* inUStr, 
	char* ioAStr, 
	tslen inLen, 
	bool  inSetTermNull = true )
{
	pvu_u2a( inUStr, ioAStr, inLen, inSetTermNull );
}

/**********************************************************************************************/
/**	Converts ASCII string to Unicode string.
	Can be used only for the so-called "invariant" characters, 
	i.e. characters with codes 0 - 127. */
inline void vu_a2u( 
	const char* inAStr, 
	UChar*		ioUStr, 
	tslen		inLen, 
	bool		inSetTermNull = true )
{
	pvu_a2u( inAStr, ioUStr, inLen, inSetTermNull );
}


#pragma mark -


/**********************************************************************************************/
//! Returns the length of the output string, not counting the terminating NULL; 
//! if the length is greater than inLen, then the string will not fit 
//! and a buffer of the indicated length would need to be passed in.
//! Send inULen = -1 if you don't know its len.
///
inline vint32 vu_FromUnicodeToDefaultCP( 
	const UChar* inUStr, tslen inULen, char* outBuf, tslen inLen )
{
	return pFromUnicodeToDefaultCP( inUStr, inULen, outBuf, inLen );
}


/**********************************************************************************************/
//! Returns the length of the output string, not counting the terminating NULL; 
//! if the length is greater than oiLen, then the string will not fit 
//! and a buffer of the indicated length would need to be passed in 
///
inline vint32 vu_FromDefaultCPToUnicode( 
	const char* inStr, tslen inLen, UChar* outBuf, tslen inULen )
{
	return pFromDefaultCPToUnicode( inStr, inULen, outBuf, inLen );
}


#pragma mark -


/**********************************************************************************************/
// UChar <=> wchar_t
//
inline UChar* vu_strFromWCS( 
	UChar*			dest,
	vint32			destCapacity, 
	vint32*			pDestLength,
	const wchar_t*	src,
	vint32			srcLength,
	UErrorCode*		pErrorCode )
{
	return pvu_strFromWCS( dest, destCapacity, pDestLength, src, srcLength, pErrorCode );
}


/**********************************************************************************************/
inline wchar_t* vu_strToWCS( 
	wchar_t*		dest, 
	vint32			destCapacity,
	vint32*			pDestLength,
	const UChar*	src, 
	vint32			srcLength,
	UErrorCode*		pErrorCode )
{
	return pvu_strToWCS( dest, destCapacity, pDestLength, src, srcLength, pErrorCode );
}


#pragma mark -


/**********************************************************************************************/
// toUpper
//
inline UChar vu_toUpper( UChar inCh )	
{ 
	return pvu_toupper( inCh ); 
}


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_ustdio_h
