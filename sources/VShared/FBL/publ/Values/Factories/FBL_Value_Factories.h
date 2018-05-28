/**********************************************************************************************/
/* FBL_Value_Factories.h																	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_Value_Factories_h
   #define _FBL_Value_Factories_h
#pragma once

#define BOOST_USE_THREADS 1

// fbl inculdes
#include <VShared/FBL/publ/Interfaces/FBL_I_Value.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// ADDED RZ 05/29/11 
//
// SET of Value Factories comfortable for building of SQL Binding Array.
//
// USAGE EXAMPLE:
//		ArrayOfValues_Ptr pValues = new ArrayOfValues();
//		pValues->AddItem( toValue_varchar(mName) );
//		pValues->AddItem( toValue_double(70.5) );
//		pValues->AddItem( toValue_long(size) );
//
//
FBL_SHARED_EXP I_Value* toValue_bool ( vuint32 inValue );
FBL_SHARED_EXP I_Value* toValue_byte ( vuint8  inValue );

FBL_SHARED_EXP I_Value* toValue_short ( vint16  inValue );
FBL_SHARED_EXP I_Value* toValue_ushort( vuint16 inValue );

FBL_SHARED_EXP I_Value* toValue_medium ( vint32  inValue );
FBL_SHARED_EXP I_Value* toValue_umedium( vuint32 inValue );

FBL_SHARED_EXP I_Value* toValue_long ( vint32  inValue );
FBL_SHARED_EXP I_Value* toValue_ulong( vuint32 inValue );

FBL_SHARED_EXP I_Value* toValue_llong ( vint64  inValue );
FBL_SHARED_EXP I_Value* toValue_ullong( vuint64 inValue );

FBL_SHARED_EXP I_Value* toValue_float ( float  inValue );
FBL_SHARED_EXP I_Value* toValue_double( double inValue );

FBL_SHARED_EXP I_Value* toValue_string ( const FBL::String& inStrObject );
FBL_SHARED_EXP I_Value* toValue_varchar( const FBL::String& inStrObject );
FBL_SHARED_EXP I_Value* toValue_text   ( const FBL::String& inStrObject );

FBL_SHARED_EXP I_Value* toValue_date( 
								vint32		inYear,
								vuint16		inMonth, 
								vuint16		inDay );

FBL_SHARED_EXP I_Value* toValue_time( 
								vuint16 	inHours, 
								vuint16 	inMinutes,
								vuint16 	inSeconds, 
								vuint16		inMilliseconds );

FBL_SHARED_EXP I_Value* toValue_datetime( 
								vint32		inYear,
								vuint16		inMonth, 
								vuint16		inDay,
								vuint16 	inHours, 
								vuint16 	inMinutes,
								vuint16 	inSeconds, 
								vuint16		inMilliseconds );


#pragma mark -


/**********************************************************************************************/
// Functions-factories creating value of VKERNEL types
//
//	Below is the set of factory functions to create values through 
//	the map of registered value types. This way is slower than direct 
//	creation but this is the only way to create values of the user-defined types. 
/**********************************************************************************************/

// For numeric integer fields param1 and param2 are not used.
FBL_SHARED_EXP	I_Value*	CreateValueBoolean		( bool inNullable, void* inParam1 = nullptr, void* inParam2 = nullptr );

FBL_SHARED_EXP	I_Value*	CreateValueByte			( bool inNullable, void* inParam1 = nullptr, void* inParam2 = nullptr );

FBL_SHARED_EXP	I_Value*	CreateValueShort		( bool inNullable, void* inParam1 = nullptr, void* inParam2 = nullptr );
FBL_SHARED_EXP	I_Value*	CreateValueUShort		( bool inNullable, void* inParam1 = nullptr, void* inParam2 = nullptr );

FBL_SHARED_EXP	I_Value*	CreateValueLong			( bool inNullable, void* inParam1 = nullptr, void* inParam2 = nullptr );
FBL_SHARED_EXP	I_Value*	CreateValueULong		( bool inNullable, void* inParam1 = nullptr, void* inParam2 = nullptr );

FBL_SHARED_EXP	I_Value*	CreateValueLLong		( bool inNullable, void* inParam1 = nullptr, void* inParam2 = nullptr );
FBL_SHARED_EXP	I_Value*	CreateValueULLong		( bool inNullable, void* inParam1 = nullptr, void* inParam2 = nullptr );


// param1: vuint16* Precission	optional. Default = gFloatDefaultPrecision
// param2: vuint16* Scale		optional. Default = gFloatDefaultScale
FBL_SHARED_EXP	I_Value*	CreateValueFloat		( bool inNullable, void* inParam1 = nullptr, void* inParam2 = nullptr );
FBL_SHARED_EXP	I_Value*	CreateValueDouble		( bool inNullable, void* inParam1 = nullptr, void* inParam2 = nullptr );

// param1: tslen MaxLen			optional. Default = 20
// param2: I_Localizable*		optional. NULL -> system default
FBL_SHARED_EXP	I_Value*	CreateValueString		( bool inNullable, void* inParam1 = nullptr, void* inParam2 = nullptr );

// param1: tslen MaxLen			optional. Default = kMaxLengthOfVarChar_UTF16
// param2: I_Localizable*		optional. NULL -> system default
FBL_SHARED_EXP	I_Value*	CreateValueVarChar		( bool inNullable, void* inParam1 = nullptr, void* inParam2 = nullptr );

// param1: const I_Database*
FBL_SHARED_EXP	I_Value*	CreateValueDate			( bool inNullable, void* inParam1, void* inParam2 = nullptr );
FBL_SHARED_EXP	I_Value*	CreateValueTime			( bool inNullable, void* inParam1, void* inParam2 = nullptr );
FBL_SHARED_EXP	I_Value*	CreateValueDateTime		( bool inNullable, void* inParam1, void* inParam2 = nullptr );

// param1 and param2 are not used.
FBL_SHARED_EXP	I_Value*	CreateValueMedium		( bool inNullable, void* inParam1 = nullptr, void* inParam2 = nullptr );
FBL_SHARED_EXP	I_Value*	CreateValueUMedium		( bool inNullable, void* inParam1 = nullptr, void* inParam2 = nullptr );

// param1: vuint32 MaxLen
FBL_SHARED_EXP	I_Value*	CreateValueFixedBinary	( bool inNullable, void* inParam1, void* inParam2 = nullptr );
FBL_SHARED_EXP	I_Value*	CreateValueVarBinary	( bool inNullable, void* inParam1, void* inParam2 = nullptr );
FBL_SHARED_EXP	I_Value*	CreateValueBLOB			( bool inNullable, void* inParam1, void* inParam2 = nullptr );

FBL_SHARED_EXP	I_Value*	CreateValuePicture		( bool inNullable, void* inParam1, void* inParam2 = nullptr );

// param1: vint32 MaxLen, optional.
// param2: I_Localizable*
FBL_SHARED_EXP	I_Value*	CreateValueText			( bool inNullable, void* inParam1 = nullptr, void* inParam = nullptr );

// param1 and param2 are not used.
FBL_SHARED_EXP	I_Value*	CreateValueObjectPtr	( bool inNullable, void* inParam1 = nullptr, void* inParam2 = nullptr );

FBL_SHARED_EXP	I_Value*	CreateValueMoney		( bool inNullable, void* inParam1 = nullptr, void* inParam2 = nullptr );

FBL_SHARED_EXP	I_Value*	CreateValueCompound		( ArrayOfValues_Ptr inValues );

FBL_SHARED_EXP	I_Value*	CreateValueVariant		( bool inNullable, void* inParam1, void* inParam2 );


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_Value_Factories_h
