/**********************************************************************************************/
/* FBL_String.h	 		                                                   					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_String_h 
	#define _FBL_String_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Interfaces/FBL_I_String.h>
#include <VShared/FBL/publ/Util_Strings/FBL_String_Len_Const.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// Forward declarations: 
//
SMART_INTERFACE( I_OStream );
SMART_INTERFACE( I_IStream );
SMART_INTERFACE( I_String );
SMART_INTERFACE( I_StringFactory );

interface I_Converter;


/**********************************************************************************************/
// Copy C-string to the pascal string.
// Can't be used for Unicode, because it assume that c-string is zero-end. 
//
FBL_SHARED_EXP void c2pstrcpy( 
	const char* 	inSrc, 
	StringPtr 		inDest,
	vuint16			inMaxChars = 255 );
		
		
/**********************************************************************************************/
// Copy pascal string to C string. Can't be used for the same buffer, because uses FBL::memcpy().
//
FBL_SHARED_EXP void p2cstrcpy( 
	const StringPtr inSrc, 
	char* 			inDest, 
	vuint16			inBuffSize = 256 );
								

/**********************************************************************************************/
// THIS module declarations:
//
class String;

SMART_ARRAY_OF_CLASSES( ArrayOfStringsObj, String );
SMART_ARRAY_OF_CLASS_PTRS( ArrayOfStrings, String* );
SMART_ARRAY_OF_SMART_PTRS( ArrayOfStringArrays, ArrayOfStrings_Ptr );


/**********************************************************************************************/
class FBL_SHARED_EXP_CLASS String
{
	public://///////////////////////////////////////////////////////////////////////////////////

static const char*			sEmptyPtrA; 
static const UChar*			sEmptyPtr; 


static	I_StringFactory*	sFactory;	///< Factory for string creation.


	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// Construction/destruction:

							String( void );

							String( const String& inStr );

							String( String&& inStr );

							String( Const_I_String_Ptr inStr );

							/**	Construct from Unicode-string.
								The characters to place in the UnicodeString.
								'inStr' must be NULL (U+0000) terminated. */
               				String( 
								const UChar*	inStr, 
								tslen			inLen = -1 );

							String( 
								const UChar*	inStart, 
								const UChar*	inEnd );

							String( 
								const char*		inStr, 
								tslen			inLen = -1, 
								const char*		inEncoding = nullptr,
								EStringImpType	inImpType = kICUImpl );

							String( 
								const char* 	inStr, 
								tslen			inLen, 
								I_Converter*	inConverter,
								EStringImpType 	inImpType = kICUImpl );

							/** Just allocate buffer. */
							String( 
								tslen			inCapacity,
								UChar			c = 0,			///< Used to initially fill the string.
								tslen			inCount = 0 );	///< Specifies how many code points c are to be written in the string.

							/** Just allocate buffer. */
							String( 
								tslen			inCapacity,
								char			c,			///< Used to initially fill the string.
								tslen			inCount);	///< Specifies how many code points c are to be written in the string.


virtual						~String( void );


	// ---------------------
	// Assignment:


		String&				operator= ( const UChar*	inSrc  );
		String&				operator= ( const String&	inText );
		String&				operator= ( String&&		inText );
		String&				operator= ( const char*		inText );	// uses SYS convertor.

		String&				setTo( const String& inText );
		String&				setTo( const UChar*  inText, tslen srcLength );
		String&				setTo( const char*   inText, tslen srcLength );

		String&				clear( void );


	// ---------------------
	// Comparison - bitwise only - for international comparison use collation.

		bool				operator==( const String& inStr ) const;
		bool				operator!=( const String& inStr ) const;
		bool				operator> ( const String& inStr ) const;
		bool				operator< ( const String& inStr ) const;
		bool				operator>=( const String& inStr ) const;
		bool				operator<=( const String& inStr ) const;

		int					compare( const String& inStr) const;
		int					compare( const UChar* inSrc, tslen inLen = -1 ) const;
		int					compare( const char* inSrc, tslen inLen = -1 ) const;

		bool				operator==( const UChar* inStr ) const;
		bool				operator!=( const UChar* inStr ) const;
		bool				operator==( const char*  inStr ) const;
		bool				operator!=( const char*  inStr ) const;
		
		int					caseCompare( const String& inStr, vuint32 inOptions ) const;
		int					caseCompare( const UChar*  inSrc, tslen inLen, vuint32 inOptions ) const;
		int					caseCompare( const char*   inSrc, tslen inLen, vuint32 inOptions ) const;

		bool				startsWith( const String& inStr ) const;
		bool				endsWith  ( const String& inStr ) const;


	// ---------------------
	// Length & Buffer Methods:

		tslen				getCapacity( void ) const;
		tslen				length( void ) const;
		bool				isEmpty( void ) const;


	// ---------------------
	// Work with UChar* Buffer:
		
							// Returns read-only buffer.
		const UChar*		getBuffer( void ) const;

							// Returns read-writeable buffer. Cannot be nested.
							// Must follow by releaseBuffer().
		UChar*				getBuffer( tslen inCapacity );
		void				releaseBuffer( tslen inNewLength = -1 );

		
	// ---------------------
	// Work with char* Buffer:

							// Returns read-only buffer.
		const char*			getBufferA( const char* inEncoding = nullptr ) const;

							// Returns read-write buffer. Calls cannot be nested.
							// Must follow by releaseBuffer().
		char*				getBufferA( tslen inCapacity );
		void				releaseBufferA( tslen inNewLength = -1 );


	// ---------------------
	// Pointers to UChar* str:

							/// Returns pointer to the read-only null-terminated string's buffer.
		const UChar*		c_str( void ) const;

		const UChar*		begin( void ) const;
		const UChar*		end( void ) const;


	// ---------------------
	// Assigment of Numbers to String:

		String&				setInt   	( vint32 		inValue );
		String&				setUInt  	( vuint32  	inValue );
		String&				setLLong	( vint64 	inValue );
		String&				setULLong 	( vuint64  	inValue );
		String&				setDouble	( double 	inValue );

		String&				setIntAsHex ( vint32 		inValue );
		String&				setLLongAsHex(vint64 	inValue );


	// ---------------------
	// Appending of Numbers to String:

		String&				addInt    	( vint32   	inValue );
		String&				addUInt   	( vuint32  	inValue );
		String&				addLLong  	( vint64   	inValue );
		String&				addULLong 	( vuint64  	inValue );
		String&				addDouble 	( double 	inValue );


	// ---------------------
	// Extractions of Numbers from String:

		vint32				parseInt   	( void ) const;
		vuint32				parseUInt  	( void ) const;
		vint64				parseLLong	( void ) const;
		vuint64				parseULLong ( void ) const;
		double				parseDouble	( void ) const;

 
		String&				operator<<  ( vint32   	inValue )	{ return addInt(inValue); 	}
		String&				operator<<  ( vuint32  	inValue )	{ return addUInt(inValue); 	}
		String&				operator<<  ( vint64   	inValue )	{ return addLLong(inValue); }
		String&				operator<< 	( vuint64  	inValue )	{ return addULLong(inValue);}
		String&				operator<< 	( double 	inValue )	{ return addDouble(inValue);}
		String&				operator<< 	( const String&	Value );


	// ---------------------
	// Appending to string:

		String&				operator+=( UChar ch );
		String&				operator+=( const String& inSrc );
		String&				operator+=( const char* inSrc );
		String&				operator+=( const UChar* inSrc );

							/**	Append the characters in inSrc to the String object 
								at offset start.
								@param inSrc the source for the new characters
								@param inSrcLength the number of Unicode characters in inSrc
								@return a reference to this */
		String&				append(
								const UChar* inSrc,
								tslen inSrcLength );

		String&				append(
								const char* inSrc,
								tslen inSrcLength );


	// ---------------------
	// Substrings extraction:

							// Extracts inCount chars from the inFirst position. 
							// if inCount is -1 then extracts from inFirst to the end.
		String				mid( tslen inFirst, tslen inCount = -1 ) const;

							///	Extracts \a inCount chars from the left.
		String				left( tslen inCount ) const;

							///	Extracts \a inCount chars from the left.
		String				right( tslen inCount ) const;


	// ---------------------
	// String manipulation:
							/**	Insert the characters in inText into the String object
								at offset inStart. */
		String&				insert(	tslen inStart, 
									const String& inText );

							/**	Remove all characters from the String object.
								@return a reference to this. */
		String&				remove( void );

							/**	Remove the characters in the range [inStart, start + length) 
								from the String object.
								@param start the offset of the first character to remove
								@param length the number of characters to remove
								@return a reference to this. */
		String&				remove(	tslen inStart, 
									tslen inLength = -1 );

							/**	Reverse this String in place.
								@return a reference to this. */
		String&				reverse( void );

							/**	Truncate this String to the inTargetLength.
								@param inTargetLength the desired length of this UnicodeString.
								@return true if the text was truncated, false otherwise. */
		bool				truncate( tslen inTargetLength );

							/**	Trims leading and trailing whitespace from this String.
								@return a reference to this. */
		String&				trim( void );

							/**	Replace the characters in the range @inStart, @inStart + @inLength
								with the characters in @inText. */
		String&				replace( 
								tslen	inStart, 
								tslen	inLength, 
								const String& inText);


	// ---------------------
	// Case manipulation:

							/**	Convert the characters in this to UPPER CASE following the 
								conventions of the default locale.
								@return A reference to this. */
		String&				toUpper( void );

							/**	Convert the characters in this to lower case following 
								the conventions of the default locale.
								@return A reference to this. */
		String&				toLower( void );


	// ---------------------
	// Find operation:

							/**	Search for the first occurrence of \a ch character in the 
								string from the \a inStart position. 
								@return 0-based character position within string or -1 if not found. */
		vint32				find( UChar ch, tslen inStart = 0 ) const;

							/// Look for a specific sub-string like "C" strstr.
		vint32				find( const UChar* str, tslen inStart = 0 ) const;

							/**	Search in the reverse order for the first occurrence 
								of \a ch character in the string */
		vint32				reverseFind( UChar ch ) const;

		vint32				findOneOf( const UChar* inCharSet, tslen inStart = 0 ) const;
		vint32				findOneOf( const char* inCharSet, tslen inStart = 0 ) const;

		tslen				indexOf( const String& inStr ) const;


	// ---------------------
	// Character access :
							

		UChar				operator[]( tslen inOffset ) const;

							/**	Return the code unit at specified offset.
								If the offset is not valid (0..length()-1) then U+ffff is returned.
								@param inOffset a valid offset into the text
								@return the code unit at offset @inOffset
								       or 0xffff if the offset is not valid for this string */
		UChar				charAt( tslen inOffset ) const;

							/**	Set the character at the specified offset to the specified character.
								@param inOffset A valid offset into the text of the character to set
								@param inCh The new character
								@return A reference to this. */
		String&				setCharAt(
								tslen inOffset, 
								UChar inCh);

		String&				setCharAt(
								tslen inOffset, 
								char inCh);


							/** This tricky function can be used to avoid copying of buffer */
		UChar*				getBufferOwnership( void );
		char*				getBufferOwnershipA( void );


	// ---------------------
	// XML methods:

							/**	Replaces any XML-disabled characters within 
								string with its string equivalent. 
								For example: replaces '>' with "&gt;" and so on. */
		String&				addXmlEncoding( void );

							/**	Replaces any XML-disabled characters' in 
								its string form to its native form.
								For example: replaces "&gt;" with '>' and so on. */
		String&				removeXmlEncoding( void );

	// ---------------------
	// JSON methods:

							/**	Replaces any JSON-disabled characters within
								string with its string equivalent. 
								For example: replaces '\t' with "\\t" and so on. */
		String&				addJsonEncoding( void );


							/**	Escapes given character within string. */
		String&				escape( UChar inChar );


	// ---------------------

		bool				isSingleByte( void ) const;


	// ---------------------
	
static const String&		sEmpty( void );
static void					Cleanup( void );


	private:////////////////////////////////////////////////////////////////////////////////////

		I_String*			mpString;

static 	const String*		sEmptyString; // signlenton.


	private:////////////////////////////////////////////////////////////////////////////////////


friend FBL_SHARED_EXP I_OStream& operator <<( I_OStream& inStream, const String& inStr );
friend FBL_SHARED_EXP I_IStream& operator >>( I_IStream& inStream, String& inStr );

friend String operator+( const UChar* inCStr, const String& inStr );
friend String operator+( const String& inStrLeft, const String& inStrRight );
friend String operator+( const char* inCStr, const String& inStr );
friend String operator+( const String& inStr, const char* inCStr );
};


/**********************************************************************************************/
inline 
String operator+( const UChar* inCStr, const String& inStr )
{
	String s(inCStr);
	s += inStr;
	return s;
}


/**********************************************************************************************/
inline 
String operator+( const String& inStrLeft, const String& inStrRight )
{
	String s(inStrLeft);
	s += inStrRight;
	return s;
}


/**********************************************************************************************/
inline 
String operator+( const char* inCStr, const String& inStr )
{
	String s( inCStr, -1, (char*) nullptr );
	
	s += inStr;
	
	return s;
}


/**********************************************************************************************/
inline 
String operator+( const String& inStr, const char* inCStr )
{
	String s(inStr);
	s += inCStr;
	return s;
}


/**********************************************************************************************/
inline 
String operator+( const String& inStr, const UChar inCh )
{
	String s(inStr);
	s += inCh;
	return s;
}


/**********************************************************************************************/
inline 
bool operator == ( const UChar* inCStr, const String& inStr )
{
    return bool( inStr.compare(inCStr) == 0 );
}


/**********************************************************************************************/
FBL_SHARED_EXP I_OStream& operator <<( I_OStream& inStream, const String& inStr );
FBL_SHARED_EXP I_IStream& operator >>( I_IStream& inStream, String& inStr );


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // _FBL_String_h
