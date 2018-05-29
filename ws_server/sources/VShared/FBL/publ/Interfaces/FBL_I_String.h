/**********************************************************************************************/
/* FBL_I_String.h 		                                                      				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_String_h
	#define _FBL_I_String_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>

// FINAL: 
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE( I_String );


/**********************************************************************************************/
// Interface I_String.
//
interface FBL_SHARED_EXP_CLASS I_String : public I_Unknown
{

virtual 					~I_String( void );


	// ---------------------
	// Virtual "constructors":


virtual	I_String*			Clone( void ) const = 0;


	// ---------------------
	// Assignment:

virtual	void				SetTo( Const_I_String_Ptr inSrc ) = 0;
virtual	void				SetTo( const UChar* srcChars, tslen srcLength = -1 ) = 0;
virtual	void				SetTo( const char* inText, tslen srcLength = -1 ) = 0;


	// ---------------------
	// Comparison methods:

virtual int					Compare( 
								Const_I_String_Ptr inSrc ) const  = 0;

virtual int					Compare( 
								const UChar* inStr, 
								tslen inStrLen = -1 ) const  = 0;

virtual int					Compare( 
								const char* inStr, 
								tslen inStrLen = -1 ) const  = 0;

virtual int					CaseCompare( 
								Const_I_String_Ptr inSrc, 
								vuint32 inOptions ) const  = 0;

virtual int					CaseCompare( 
								const UChar* inStr, 
								tslen inStrLen,
								vuint32 inOptions ) const  = 0;

virtual int					CaseCompare( 
								const char* inStr, 
								tslen inStrLen,
								vuint32 inOptions ) const  = 0;

virtual bool				StartsWith( Const_I_String_Ptr inStr ) const = 0;
virtual bool				EndsWith( Const_I_String_Ptr inStr ) const = 0;


	// ---------------------
	// Length & buffer operations:

		// <Capacity> [r/o]
virtual tslen				get_Capacity( void ) const  = 0;

		// <Length> [r/o]
virtual tslen				get_Length( void ) const  = 0;

		// <IsEmpty> [r/o]
virtual bool				get_IsEmpty( void ) const  = 0;

virtual UChar*				GetBufferU( tslen inCapacity ) = 0;
virtual const UChar*		GetBufferU( void ) const = 0;
virtual void				ReleaseBufferU( tslen inNewLength = -1 ) = 0;
virtual UChar*				GetBufferOwnershipU( void ) = 0;

virtual char*				GetBufferA( tslen inCapacity ) = 0;
virtual const char*			GetBufferA( const char* inEncoding = nullptr ) const = 0;
virtual void				ReleaseBufferA( tslen inNewLength = -1 ) = 0;
virtual char*				GetBufferOwnershipA( void ) = 0;


	// ---------------------
	// Assigment of numbers to String:

virtual void				ConvertInt_10 ( vint32 inValue ) = 0;
virtual void				ConvertUInt_10 ( vuint32 inValue ) = 0;
virtual void				ConvertLLong_10 ( vint64 inValue ) = 0;
virtual void				ConvertULLong_10 ( vuint64 inValue ) = 0;

virtual void				ConvertInt_16 ( vint32 inValue ) = 0;
virtual void				ConvertLLong_16( vint64 inValue ) = 0;

virtual void				ConvertDouble( double inValue ) = 0;

virtual void				ConvertAndAppendInt_10( vint32 inValue ) = 0;
virtual void				ConvertAndAppendUInt_10( vuint32 inValue ) = 0;
virtual void				ConvertAndAppendLLong_10( vint64 inValue ) = 0;
virtual void				ConvertAndAppendULLong_10( vuint64 inValue ) = 0;
virtual void				ConvertAndAppendDouble( double inValue ) = 0;


	// ---------------------
	// Extractions of numbers from String:

virtual vint32				ParseInt   	( void ) const = 0;
virtual vuint32				ParseUInt  	( void ) const = 0;
virtual vint64				ParseLLong	( void ) const = 0;
virtual vuint64				ParseULLong ( void ) const = 0;
virtual double				ParseDouble	( void ) const = 0;


	// ---------------------
	// Appending to string:

virtual void				Append( Const_I_String_Ptr inStr ) = 0;

virtual void				Append(
								const UChar* inSrc,
								tslen inSrcLen ) = 0;

virtual void				Append(
								const char* inSrc,
								tslen inSrcLen ) = 0;

virtual void				Append( UChar inCh ) = 0;


	// ---------------------
	// Substrings extraction:

							/**	Exctracts \a inCount chars from \a inFirst position. */
virtual	I_String_Ptr		Mid( tslen inFirst, tslen inCount = -1 ) const = 0;

							///	Exctracts \a inCount chars from the left.
virtual	I_String_Ptr		Left( tslen inCount ) const = 0;

							///	Exctracts \a inCount chars from the left.
virtual	I_String_Ptr		Right( tslen inCount ) const = 0;


	// ---------------------
	// String manipulation:

virtual void				Insert(	
								tslen inStart,
								Const_I_String_Ptr inText ) = 0;

							/**	Remove the characters in the range [inStart, start + length) 
								from the String object.
								@param start the offset of the first character to remove
								@param length the number of characters to remove
								@return a reference to this. */
virtual void				Remove(	void ) = 0;

virtual void				Remove(	
								tslen inStart,
								tslen inLength = -1 ) = 0;

							/**	Reverse this String in place.
								@return a reference to this. */
virtual void				Reverse( void ) = 0;


							/**	Truncate this String to the inTargetLength.
								@param inTargetLength the desired length of this UnicodeString.
								@return true if the text was truncated, false otherwise. */
virtual	bool				Truncate( tslen inTargetLength ) = 0;

							/**	Trims leading and trailing whitespace from this String.
								@return a reference to this. */
virtual void				Trim( void ) = 0;

							
							/**	Replace the characters in the range @inStart, @inStart + @inLength
								with the characters in @inText. */
virtual void				Replace( 
								tslen inStart, 
								tslen inLength, 
								Const_I_String_Ptr inText) = 0;


	// ---------------------
	// Case manipulation:

							/**	Convert the characters in this to UPPER CASE following the 
								conventions of the default locale.
								@return A reference to this. */
virtual void				ToUpper( void ) = 0;

							/**	Convert the characters in this to lower case following 
								the conventions of the default locale.
								@return A reference to this. */
virtual void				ToLower( void ) = 0;


	// ---------------------
	// Find operation:

							/**	Search for the first occurence of \a ch character in the 
								string from the \a inStart position. 
								@return 0-based character position within string or -1 if not found. */
virtual	vint32				Find( UChar ch, tslen inStart = 0 ) const  = 0;

							/**	Search in the reverse order for the first occurence 
								of \a ch character in the string */
virtual	vint32				ReverseFind( UChar ch ) const  = 0;

virtual vint32				FindOneOf( 
								const UChar* inCharSet, 
								tslen inStart = 0 ) const  = 0;

virtual vint32				FindOneOf( 
								const char* inCharSet, 
								tslen inStart = 0 ) const  = 0;


							// Look for a specific sub-string like "C" strstr.
virtual vint32				Find( 
								 const UChar* str, 
								 tslen inStart = 0 ) const  = 0;

virtual tslen				IndexOf( Const_I_String_Ptr inStr ) const = 0;


	// ---------------------
	// Character access :

		// <CharAt> [r/o]
virtual UChar				get_CharAt( tslen inOffset ) const = 0;
virtual void				put_CharAt(
								tslen inOffset, 
								UChar inCh) = 0;

virtual void				put_CharAt(
								tslen inOffset, 
								char inCh) = 0;


	// ---------------------
	// XML methods:

							/**	Replaces any XML-disabled characters within 
								string with its string equivalent. 
								For example: replaces '>' with "&gt;" and so on. */
virtual void				AddXmlEncoding( void ) = 0;

							/**	Replaces any XML-disabled characters' in 
								its string form to its native form.
								For example: replaces "&gt;" with '>' and so on. */
virtual void				RemoveXmlEncoding( void ) = 0;


	// ---------------------
	// JSON methods:

							/**	Replaces any JSON-disabled characters within
								string with its string equivalent. 
								For example: replaces '\t' with "\\t;" and so on. */
virtual void				AddJsonEncoding( void ) = 0;


							/**	Escapes given character within string. */
virtual void				Escape( UChar inChar ) = 0;


virtual bool				get_IsSingleByte( void ) const  = 0;


}; 


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_I_String_h







