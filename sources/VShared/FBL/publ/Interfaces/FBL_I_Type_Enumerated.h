/**********************************************************************************************/
/* FBL_I_Type_Enumerated.h	                                               					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_Type_Enumerated_h
	#define _FBL_I_Type_Enumerated_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Interfaces/FBL_I_Type.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
const vuint16 ENUM_8_IDENT_COUNT  = (vuint16)    (256-1);
const vuint16 ENUM_16_IDENT_COUNT = (vuint16)(256*256-1);

const vuint16 DEFAULT_MAX_IDENT_COUNT = ENUM_8_IDENT_COUNT;


/**********************************************************************************************/
SMART_INTERFACE(I_Type_Enumerated);


/**********************************************************************************************/
// Factories to create and define enum type
//
FBL_SHARED_EXP I_Type_Enumerated_Ptr CreateEnumType( 
	I_Database_Ptr			inpDb,
	const String&			inName,
	ArrayOfStrings_Ptr		inIdents,
	vuint16					inMaxIdentCount = DEFAULT_MAX_IDENT_COUNT );

FBL_SHARED_EXP I_Type_Enumerated_Ptr CreateEnumType( 
	I_Database_Ptr			inpDb,
	const String&			inName,
	ArrayOfStringsObj_Ptr	inIdents,
	vuint16					inMaxIdentCount = DEFAULT_MAX_IDENT_COUNT );


/**********************************************************************************************/
// 
//StubInterface
interface FBL_SHARED_EXP_CLASS I_Type_Enumerated : public I_Unknown
{

virtual 					~I_Type_Enumerated( void );

	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// Properties:

		// <LocaleList> [r/o]
							/// Returns list of defined locales.
virtual ArrayOfStringsObj_Ptr get_LocaleList( void ) const = 0;


		// <MaxIdentCount>
							/// Returns the size of predefined values.
							/// EXAMPLE: 
							///    CREATE TYPE bug_status AS ENUM8 ('old', 'new');
							///    returns 256.
virtual vuint16				get_MaxIdentCount( void ) const = 0;

							/// Allows to change the size of ENUM storage between 1 and 2 bytes.
                            /// inValue must be one of two: ENUM_8_IDENT_COUNT or ENUM_16_IDENT_COUNT
virtual void				put_MaxIdentCount( vuint16 inValue ) = 0;

		// <IdentCount> [r/o]	
							/// Returns the count of predefined values.
							/// EXAMPLE: 
							///    CREATE TYPE bug_status AS ENUM ('old', 'new');
							///    returns 2.
virtual vuint16				get_IdentCount( void ) const = 0;

		// <IdentList> [r/o]	

virtual ArrayOfStringsObj_Ptr	get_IdentList( void ) const = 0;

virtual ArrayOfStringsObj_Ptr	get_LocaleIdentList( const String&	inLocale ) const = 0;


		// <IdentAsString> [r/o]	

							/// Returns 'old' for inIndex=1 for above example.
virtual const String&		get_IdentAsString( vuint16 inIndex ) const = 0;

							/// Returns string correposnded to inIndex and the asked Locale.
virtual const String&		get_LocaleIdentAsString(
								const String&	inLocale,
								vuint16			inIndex ) const = 0;


		// <IdentAsLong> [r/o]	
							/// For 'old' return 1 ...
virtual vuint16				get_IdentAsLong( const String& inValueName ) const = 0;

							/// For 'old' return 1 ...
virtual vuint16				get_LocaleIdentAsLong(
								const String& inLocale,
								const String& inValueName ) const = 0;


	// ---------------------
	// Ident Methods:
					
							/// Adds one more IDENT into ENUM type.
							/// Should be used only on 
							/// 'old' for 1, 'new' for 2 ...
virtual void				AddIdent( const String& inValueName ) = 0;

							/// Adds set of IDENTs at once.
virtual	void				AddIdents( ArrayOfStrings_Ptr inpIdents ) = 0;

							/// Adds all IDENTs for particular locale into ENUM type.
virtual void				AddLocale(
								const String&		inLocale,
								ArrayOfStrings_Ptr	inIdents ) = 0;

							/// Remove inLocale idents.
virtual void				DropLocale( const String& inLocale ) = 0;

							/// Change IDENT in ENUM type.
virtual void				ChangeIdent( 
								const String& inOldValueName,
								const String& inNewValueName ) = 0;

							/// Change IDENT in ENUM type for particular locale.
virtual void				ChangeLocaleIdent(
								const String& inLocale,
								const String& inOldValueName,
								const String& inNewValueName ) = 0;

};


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_I_Type_Enumerated
