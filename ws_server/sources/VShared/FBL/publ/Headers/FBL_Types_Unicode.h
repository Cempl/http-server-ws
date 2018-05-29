/**********************************************************************************************/
/* FBL_Types_Unicode.h                                                     					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_Types_Unicode_h 
	#define _FBL_Types_Unicode_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL_Macros.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
typedef vint32 UErrorCode;

const vint32 U_ZERO_ERROR				=  0;
const vint32 U_BUFFER_OVERFLOW_ERROR 	= 15; // tmp here?


/**********************************************************************************************/
/* Use the following to determine if an UErrorCode represents */
/* operational success or failure. */

/**
 * Does the error code indicate success?
 * @stable ICU 2.0
 */
inline bool U_SUCCESS(UErrorCode code) { return (bool)(code<=U_ZERO_ERROR); }
/**
 * Does the error code indicate a failure?
 * @stable ICU 2.0
 */
inline bool U_FAILURE(UErrorCode code) { return (bool)(code>U_ZERO_ERROR); }


/**********************************************************************************************/
// Define UChar to be compatible with wchar_t if possible, 
//
#ifdef VC_SIZEOF_WCHAR_T // this is tmp protection!
#error "rename VC_SIZEOF_WCHAR_T to FBL_SIZEOF_WCHAR_T"
#endif //

#ifdef VC_SIZEOF_WCHAR_T // this is tmp protection!
#error "rename VC_SIZEOF_WCHAR_T to FBL_SIZEOF_WCHAR_T"
#endif //

// on default we think that sizeof(wchar_t) == 2
#ifndef FBL_SIZEOF_WCHAR_T
	#if FBL_POSIX_API
		#define FBL_SIZEOF_WCHAR_T 4
	#else
		#define FBL_SIZEOF_WCHAR_T 2
	#endif // __GNUC__
#endif // FBL_SIZEOF_WCHAR_T

#if FBL_SIZEOF_WCHAR_T == 2
	typedef wchar_t UChar;
#else
	typedef vuint16 UChar;
#endif

typedef long int UChar32;


/**********************************************************************************************/
/**
 * UCOL_LESS is returned if source string is compared to be less than target
 * string in the u_strcoll() method.
 * UCOL_EQUAL is returned if source string is compared to be equal to target
 * string in the u_strcoll() method.
 * UCOL_GREATER is returned if source string is compared to be greater than
 * target string in the vu_strcoll() method.
 * @see u_strcoll()
 * <p>
 * Possible values for a comparison result 
 * @stable ICU 2.0
 */
typedef enum {
  /** string a == string b */
  UCOL_EQUAL    = 0,
  /** string a > string b */
  UCOL_GREATER    = 1,
  /** string a < string b */
  UCOL_LESS    = -1
} UCollationResult ;


/**********************************************************************************************/
/** Enum containing attribute values for controling collation behavior.
 * Here are all the allowable values. Not every attribute can take every value. The only
 * universal value is UCOL_DEFAULT, which resets the attribute value to the predefined  
 * value for that locale 
 * @stable ICU 2.0
 */
typedef enum {
  /** accepted by most attributes */
  UCOL_DEFAULT = -1,

  /** Primary collation strength */
  UCOL_PRIMARY = 0,
  /** Secondary collation strength */
  UCOL_SECONDARY = 1,
  /** Tertiary collation strength */
  UCOL_TERTIARY = 2,
  /** Default collation strength */
  UCOL_DEFAULT_STRENGTH = UCOL_TERTIARY,
  UCOL_CE_STRENGTH_LIMIT,
  /** Quaternary collation strength */
  UCOL_QUATERNARY=3,
  /** Identical collation strength */
  UCOL_IDENTICAL=15,
  UCOL_STRENGTH_LIMIT,

  /** Turn the feature off - works for UCOL_FRENCH_COLLATION, 
      UCOL_CASE_LEVEL, UCOL_HIRAGANA_QUATERNARY_MODE
      & UCOL_DECOMPOSITION_MODE*/
  UCOL_OFF = 16,
  /** Turn the feature on - works for UCOL_FRENCH_COLLATION, 
      UCOL_CASE_LEVEL, UCOL_HIRAGANA_QUATERNARY_MODE
      & UCOL_DECOMPOSITION_MODE*/
  UCOL_ON = 17,
  
  /** Valid for UCOL_ALTERNATE_HANDLING. Alternate handling will be shifted */
  UCOL_SHIFTED = 20,
  /** Valid for UCOL_ALTERNATE_HANDLING. Alternate handling will be non ignorable */
  UCOL_NON_IGNORABLE = 21,

  /** Valid for UCOL_CASE_FIRST - 
      lower case sorts before upper case */
  UCOL_LOWER_FIRST = 24,
  /** upper case sorts before lower case */
  UCOL_UPPER_FIRST = 25,

  UCOL_ATTRIBUTE_VALUE_COUNT

} UColAttributeValue;


/**********************************************************************************************/
/**
 * Base letter represents a primary difference.  Set comparison
 * level to UCOL_PRIMARY to ignore secondary and tertiary differences.
 * Use this to set the strength of a I_Collator object.
 * Example of primary difference, "abc" &lt; "abd"
 * 
 * Diacritical differences on the same base letter represent a secondary
 * difference.  Set comparison level to UCOL_SECONDARY to ignore tertiary
 * differences. Use this to set the strength of a I_Collator object.
 * Example of secondary difference, "ä" >> "a".
 *
 * Uppercase and lowercase versions of the same character represents a
 * tertiary difference.  Set comparison level to UCOL_TERTIARY to include
 * all comparison differences. Use this to set the strength of a I_Collator
 * object.
 * Example of tertiary difference, "abc" &lt;&lt;&lt; "ABC".
 *
 * Two characters are considered "identical" when they have the same
 * unicode spellings.  UCOL_IDENTICAL.
 * For example, "ä" == "ä".
 *
 * UCollationStrength is also used to determine the strength of sort keys 
 * generated from UCollator objects
 * These values can be now found in the UColAttributeValue enum.
 * @stable ICU 2.0
 **/
typedef UColAttributeValue UCollationStrength;

/** Attributes that collation service understands. All the attributes can take UCOL_DEFAULT
 * value, as well as the values specific to each one. 
 * @stable ICU 2.0
 */
typedef enum {
     /** Attribute for direction of secondary weights - used in French.\ 
      * Acceptable values are UCOL_ON, which results in secondary weights
      * being considered backwards and UCOL_OFF which treats secondary
      * weights in the order they appear.*/
     UCOL_FRENCH_COLLATION, 
     /** Attribute for handling variable elements.\ 
      * Acceptable values are UCOL_NON_IGNORABLE (default)
      * which treats all the codepoints with non-ignorable 
      * primary weights in the same way,
      * and UCOL_SHIFTED which causes codepoints with primary 
      * weights that are equal or below the variable top value
      * to be ignored on primary level and moved to the quaternary 
      * level.*/
     UCOL_ALTERNATE_HANDLING, 
     /** Controls the ordering of upper and lower case letters.\ 
      * Acceptable values are UCOL_OFF (default), which orders
      * upper and lower case letters in accordance to their tertiary
      * weights, UCOL_UPPER_FIRST which forces upper case letters to 
      * sort before lower case letters, and UCOL_LOWER_FIRST which does 
      * the opposite. */
     UCOL_CASE_FIRST, 
     /** Controls whether an extra case level (positioned before the third
      * level) is generated or not.\ Acceptable values are UCOL_OFF (default), 
      * when case level is not generated, and UCOL_ON which causes the case
      * level to be generated.\ Contents of the case level are affected by
      * the value of UCOL_CASE_FIRST attribute.\ A simple way to ignore 
      * accent differences in a string is to set the strength to UCOL_PRIMARY
      * and enable case level. */
     UCOL_CASE_LEVEL,
     /** Controls whether the normalization check and necessary normalizations
      * are performed.\ When set to UCOL_OFF (default) no normalization check
      * is performed.\ The correctness of the result is guaranteed only if the 
      * input data is in so-called FCD form (see users manual for more info).\ 
      * When set to UCOL_ON, an incremental check is performed to see whether the input data
      * is in the FCD form.\ If the data is not in the FCD form, incremental 
      * NFD normalization is performed. */
     UCOL_NORMALIZATION_MODE, 
     /** An alias for UCOL_NORMALIZATION_MODE attribute */
     UCOL_DECOMPOSITION_MODE = UCOL_NORMALIZATION_MODE,
     /** The strength attribute.\ Can be either UCOL_PRIMARY, UCOL_SECONDARY,
      * UCOL_TERTIARY, UCOL_QUATERNARY or UCOL_IDENTICAL.\ The usual strength
      * for most locales (except Japanese) is tertiary.\ Quaternary strength 
      * is useful when combined with shifted setting for alternate handling
      * attribute and for JIS x 4061 collation, when it is used to distinguish
      * between Katakana  and Hiragana (this is achieved by setting the 
      * UCOL_HIRAGANA_QUATERNARY mode to on.\ Otherwise, quaternary level
      * is affected only by the number of non ignorable code points in
      * the string.\ Identical strength is rarely useful, as it amounts 
      * to codepoints of the NFD form of the string. */
     UCOL_STRENGTH,  
     /** when turned on, this attribute 
      * positions Hiragana before all  
      * non-ignorables on quaternary level
      * This is a sneaky way to produce JIS
      * sort order */     
     UCOL_HIRAGANA_QUATERNARY_MODE,
     /** when turned on, this attribute 
      * generates a collation key
      * for the numeric value of substrings
      * of digits. This is a way to get '100' 
      * to sort AFTER '2'.*/          
     UCOL_NUMERIC_COLLATION, 
     UCOL_ATTRIBUTE_COUNT
} UColAttribute;


/**********************************************************************************************/
/**
 * Enum for specifying basic types of converters
 * @see ucnv_getType
 * @stable ICU 2.0
 */
typedef enum {
    UCNV_UNSUPPORTED_CONVERTER = -1,
    UCNV_SBCS = 0,
    UCNV_DBCS = 1,
    UCNV_MBCS = 2,
    UCNV_LATIN_1 = 3,
    UCNV_UTF8 = 4,
    UCNV_UTF16_BigEndian = 5,
    UCNV_UTF16_LittleEndian = 6,
    UCNV_UTF32_BigEndian = 7,
    UCNV_UTF32_LittleEndian = 8,
    UCNV_EBCDIC_STATEFUL = 9,
    UCNV_ISO_2022 = 10,

    UCNV_LMBCS_1 = 11,
    UCNV_LMBCS_2, 
    UCNV_LMBCS_3,
    UCNV_LMBCS_4,
    UCNV_LMBCS_5,
    UCNV_LMBCS_6,
    UCNV_LMBCS_8,
    UCNV_LMBCS_11,
    UCNV_LMBCS_16,
    UCNV_LMBCS_17,
    UCNV_LMBCS_18,
    UCNV_LMBCS_19,
    UCNV_LMBCS_LAST = UCNV_LMBCS_19,
    UCNV_HZ,
    UCNV_SCSU,
    UCNV_ISCII,
    UCNV_US_ASCII,
    UCNV_UTF7,
    UCNV_BOCU1,
    UCNV_UTF16,
    UCNV_UTF32,
    UCNV_CESU8,
    UCNV_IMAP_MAILBOX,

    /* Number of converter types for which we have conversion routines. */
    UCNV_NUMBER_OF_SUPPORTED_CONVERTER_TYPES

} UConverterType;


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // _FBL_Types_Unicode_h









