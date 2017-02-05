/**********************************************************************************************/
/* FBL_I_Localizable.h 		                                                      			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_Localizable_h 
	#define _FBL_I_Localizable_h
#pragma once


// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Util_classes/FBL_String.h> 

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// forward declarations: 
//
interface I_Locale;
interface I_Collator;
interface I_Converter;
//
SMART_INTERFACE( I_Localizable );
//
SMART_ARRAY_OF_CLASS_PTRS( ArrayOfLocalizables, I_Localizable* );


/**********************************************************************************************/
// Kind of singlenton. 
// Returns pointer to a I_Localizable object, which have SYSTEM settings.
// 
FBL_SHARED_EXP I_Localizable_Ptr GetLocalizableSys( void );


/**********************************************************************************************/
// inEncoding -- if empty, then system default.
// 
FBL_SHARED_EXP I_Locale*		CreateLocale( const char* inEncoding );


/**********************************************************************************************/
FBL_SHARED_EXP I_Converter* 	CreateConverter( void );
FBL_SHARED_EXP I_Converter* 	CreateConverter( const char* inEncoding );


/**********************************************************************************************/
// Singlenton: returns converter UTF16 <=> UTF8.
// We make it as singlenton, because it is often used.
//
FBL_SHARED_EXP I_Converter* GetConverter_UTF8( void );


/**********************************************************************************************/
interface FBL_SHARED_EXP_CLASS I_Locale
{
virtual 					~I_Locale( void );

virtual const char*			getName( void ) const  = 0;

	
	// ---------------------
	// Methods:

virtual I_Collator*			CreateCollator( void ) const = 0;

virtual vint32 				strToLower(
								UChar*			dest, 
								vint32 			destCapacity,
             					const UChar*	src, 
             					vint32 			srcLength,
             					UErrorCode*		pErrorCode ) const = 0;	

virtual vint32 				strToUpper(
								UChar*			dest, 
								vint32 			destCapacity,
             					const UChar*	src, 
             					vint32 			srcLength,
             					UErrorCode*		pErrorCode ) const = 0;	

};


/**********************************************************************************************/
interface FBL_SHARED_EXP_CLASS I_Collator
{
virtual 					~I_Collator( void );

virtual	UColAttributeValue	getAttribute( 
								UColAttribute 		inAttr, 
								UErrorCode&			status ) const = 0;
								
virtual	void				setAttribute( 
								UColAttribute 		inAttr, 
								UColAttributeValue	inValue,
								UErrorCode&			status ) = 0;

virtual UCollationResult	compare(
								const UChar* 	source, 
								vint32 			sourceLength,
								const UChar* 	target, 
								vint32 			targetLength,
								UErrorCode&		status ) const = 0;
								
							// Find first occurrence of pattern in the source
							// like u_strstr, but using collator for compare.
virtual UChar*				strstr(
								const UChar* 	source, 
								vint32 			sourceLength,
								const UChar* 	pattern, 
								vint32 			patternLength,
								UErrorCode&		status ) const = 0;

							// Two collators consider as equal if all their attributes are equal.
virtual bool				get_IsEqual( I_Collator* inpCollator ) = 0;

virtual void*				get_Imp( void ) const  = 0;
								
};


/**********************************************************************************************/
// interface for I_Converter of ICU or other implementation.
// 
interface FBL_SHARED_EXP_CLASS I_Converter
{
virtual 					~I_Converter( void );
 
	// ---------------------
	// Properties:
 
virtual const String&		get_Encoding( void )  = 0;

virtual bool				get_IsSingleByte( void ) const  = 0;

virtual UConverterType		getType( void ) const  = 0;

virtual	vuint16				getMaxCharSize( void ) const  = 0;


	// ---------------------
	// Methods:

virtual vint32				fromUChars(
								char*			dest, 
								vint32 			destCapacity,
				                const UChar*	src, 
				                vint32 			srcLength,
				                UErrorCode*		pErrorCode ) const = 0;

virtual vint32				toUChars(
					             UChar*			dest, 
					             vint32 		destCapacity,
 					             const char*	src, 
 					             vint32 		srcLength,
					             UErrorCode*	pErrorCode ) const = 0;	

virtual void				toUChars(
					             UChar**		dest, 
					             const UChar* 	destLimit,
 					             const char**	src, 
								 const char*	srcLimit, 
								 bool			inFlush,
					             UErrorCode*	pErrorCode ) const = 0;
};


#pragma mark -


/**********************************************************************************************/
/**	This interface provides functionality of a localizable object.
	This can be database, table or field for example.

	This class implements kind of "property inheritance":
		* If an object have own e.g. I_Locale, than it is returned.

		* Otherwise a parent's locale/convertor/collator will be returned.
		Important is that IF a parent's parameter is changed, then child object
		start return the new value of that parameter. 
*/
//StubInterface
interface FBL_SHARED_EXP_CLASS I_Localizable : public I_Unknown
{
virtual						~I_Localizable( void );

	// ---------------------
	// Properties:

		// <Parent> [r/w]

virtual I_Localizable_Ptr	get_Parent( void ) const  = 0; 		

							/** inCopyAllSettings is used when we need to copy from parent
								not-individual settings only. */
virtual void				put_Parent( 
								I_Localizable_Ptr	inParent,
								bool				inCopyAllSettings = true )  = 0;

		// <LocaleName> [r/w]

virtual const char*			get_LocaleName( void ) const  = 0; //OLDKEEPAS: mLocaleName
virtual void				put_LocaleName( const char* inLocaleName )  = 0; 
virtual void				put_LocaleName( const String& inLocaleName )  = 0; 

		// <CollationAttribute> [r/w]

virtual UColAttributeValue	get_CollationAttribute( 
								UColAttribute		inAttribute ) const  = 0;

virtual void				put_CollationAttribute( 
								UColAttribute		inAttribute,
								UColAttributeValue 	inValue )  = 0;

		// <IOEncoding> [r/w]
							/// Returns encoding for IO of this object.
virtual const String&		get_IOEncoding( void ) const  = 0;	//OLDKEEPAS: mIOEncoding

							/// Sets a new encoding for IO of this object.
virtual void				put_IOEncoding( const char* inEncoding ) = 0;
virtual void				put_IOEncoding( const String& inEncoding ) = 0;


		// <StorageEncoding> [r/w]
							/// Returns encoding for Storage of this object.
virtual const String&		get_StorageEncoding( void ) const  = 0; //OLDKEEPAS: mStorageEncoding

							/// Sets a new encoding for Storage of this object
virtual void				put_StorageEncoding( const char* inEncoding ) = 0;
virtual void				put_StorageEncoding( const String& inEncoding ) = 0;


	// ---------------------
	// Reset settings to use-parent methods.

virtual void				UseParentCollationAttribute( UColAttribute inAttribute ) = 0;



	// ---------------------
	// Informational methods.
		
		// <HasOwnSettings> [r/w]
virtual bool				get_HasOwnSettings( void ) const  = 0;
		
		// <HasOwnLocale> [r/w]
virtual bool				get_HasOwnLocale( void ) const  = 0;

		// <HasOwnCollationAttributes> [r/w]
virtual bool				get_HasOwnCollationAttributes( void ) const  = 0;

		// <HasOwnCollationAttribute> [r/w]
virtual bool				get_HasOwnCollationAttribute( 
								UColAttribute inAttribute ) const  = 0;

		// <HasOwnStorageConverter> [r/w]
virtual bool				get_HasOwnStorageConverter( void ) const  = 0;

		// <HasOwnIOConverter> [r/w]
virtual bool				get_HasOwnIOConverter( void ) const  = 0;


	// ---------------------
	// Byte swapping:

		// <NeedSwap> [r/o]
virtual bool				get_NeedSwap( void ) const  = 0;


	// ---------------------
	// Access to objects:

		// ICU native objects.

		//
		// ATTENTION!!!
		// Avoid store this ptrs - store I_Localizable* instead.
		//
		
virtual I_Locale*			get_Locale( void ) const  = 0;
virtual I_Collator*			get_Collator( void ) const  = 0;

virtual I_Converter*		get_StorageConverter( void ) const  = 0;
virtual I_Converter*		get_IOConverter( void ) const  = 0;


	// ---------------------
	// Convertion methods:

virtual UChar*				ConvertToUnicode(
								const char* 	inStrA, 
								tslen 			inLen1,
								UChar* 			outStrU, 
								tslen 			inLen2,
								I_Converter* 	inConverter = nullptr ) = 0;

virtual char*				ConvertFromUnicode(
								const UChar* 	inStrU, 
								tslen 			inLen1,
								char* 			outStrA, 
								tslen 			inLen2,
								I_Converter* 	inConverter = nullptr ) = 0;

	// ---------------------
	// Child array methods:
	
virtual void				AddLocalizable(
								I_Localizable* inChild ) = 0;

virtual void				RemoveLocalizable(
								I_Localizable* inChild ) = 0;
};


/**********************************************************************************************/
// RZ: Do not use this directly. For internal use!
//
namespace internal
{
	// -------------------------
	// function pointers:

typedef I_Localizable_Ptr	(*GetLocalizableSys_PTR)( void );

typedef I_Converter* 		(*CreateConverter_void_PTR)( void );
typedef I_Converter* 		(*CreateConverter_a_PTR)( const char* inEncoding );

typedef I_Locale* 			(*CreateLocale_PTR)( const char* inEncoding );


	// -------------------------
	// implementation of pointers:

FBL_SHARED_EXP extern GetLocalizableSys_PTR	pGetLocalizableSys;

FBL_SHARED_EXP extern CreateConverter_void_PTR	pCreateConverter_void;
FBL_SHARED_EXP extern CreateConverter_a_PTR		pCreateConverter_a;

FBL_SHARED_EXP extern CreateLocale_PTR			pCreateLocale;

}


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_I_Localizable_h
