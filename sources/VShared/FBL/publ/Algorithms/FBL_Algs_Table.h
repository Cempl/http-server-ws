/**********************************************************************************************/
/* FBL_Algs_Table.h																			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/
/*
	This file include common algorithms around I_Table interface.
	Here you can find:
	
	* Comfortable wrappers to I_Table::CreateField() to simply creation of different
		field types. They hide usgae of properties array. The first parameter of these 
		wrappers is an I_Table object, that play role of 'this'.
	
	* MORE				
*/

#ifndef _FBL_Algs_Table_h
   #define _FBL_Algs_Table_h
#pragma once

#include <VShared/FBL/publ/Interfaces/FBL_I_Database.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Field.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Table.h>

#include <VShared/FBL/publ/Properties/FBL_PropertyContainer.h>


// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// Common alg for numeric fields. Do not use directly.
//
FBL_SHARED_EXP I_Field_Ptr CreateNumericField( 
	I_Table_Ptr 	inTable, 
	const String& 	inName,
	VALUE_TYPE		inType,
	vuint16			inFlags = fNone,
	const String& 	inMethod = String::sEmpty() );
		

/**********************************************************************************************/
FBL_SHARED_EXP I_Field_Ptr CreateFloatField(
	I_Table_Ptr 	inTable, 
	const String& 	inName,
	vuint16			inFlags = fNone,
	const String& 	inMethod = String::sEmpty(),
	vuint16			inPrecision = 0,
	vuint16			inScale = 0 );

/**********************************************************************************************/
FBL_SHARED_EXP I_Field_Ptr CreateDoubleField(
	I_Table_Ptr 	inTable, 
	const String& 	inName,
	vuint16			inFlags = fNone,
	const String& 	inMethod = String::sEmpty(),
	vuint16			inPrecision = 0,
	vuint16			inScale = 0 );

/**********************************************************************************************/
inline I_Field_Ptr CreateBooleanField(
	I_Table_Ptr 	inTable, 
	const String& 	inName,
	vuint16			inFlags = fNone,
	const String& 	inMethod = String::sEmpty() )
{
	return CreateNumericField( inTable, inName, kTypeBoolean, inFlags, inMethod );
}

/**********************************************************************************************/
inline I_Field_Ptr CreateByteField(
	I_Table_Ptr 	inTable, 
	const String& 	inName,
	vuint16			inFlags = fNone,
	const String& 	inMethod = String::sEmpty() )
{
	return CreateNumericField( inTable, inName, kTypeByte, inFlags, inMethod );
}

/**********************************************************************************************/
inline I_Field_Ptr CreateShortField(
	I_Table_Ptr 	inTable, 
	const String& 	inName,
	vuint16			inFlags = fNone,
	const String& 	inMethod = String::sEmpty() )
{
	return CreateNumericField( inTable, inName, kTypeShort, inFlags, inMethod );
}

/**********************************************************************************************/
inline I_Field_Ptr CreateUShortField(
	I_Table_Ptr 	inTable, 
	const String& 	inName,
	vuint16			inFlags = fNone,
	const String& 	inMethod = String::sEmpty() )
{
	return CreateNumericField( inTable, inName, kTypeUShort, inFlags, inMethod );
}

/**********************************************************************************************/
inline I_Field_Ptr CreateMediumField(
	I_Table_Ptr 	inTable, 
	const String& 	inName,
	vuint16			inFlags = fNone,
	const String& 	inMethod = String::sEmpty() )
{
	return CreateNumericField( inTable, inName, kTypeMedium, inFlags, inMethod );
}

/**********************************************************************************************/
inline I_Field_Ptr CreateUMediumField(
	I_Table_Ptr 	inTable, 
	const String& 	inName,
	vuint16			inFlags = fNone,
	const String& 	inMethod = String::sEmpty() )
{
	return CreateNumericField( inTable, inName, kTypeUMedium, inFlags, inMethod );
}

/**********************************************************************************************/
inline I_Field_Ptr CreateLongField(
	I_Table_Ptr 	inTable, 
	const String& 	inName,
	vuint16			inFlags = fNone,
	const String& 	inMethod = String::sEmpty() )
{
	return CreateNumericField( inTable, inName, kTypeLong, inFlags, inMethod );
}

/**********************************************************************************************/
inline I_Field_Ptr CreateULongField(
	I_Table_Ptr 	inTable, 
	const String& 	inName,
	vuint16			inFlags = fNone,
	const String& 	inMethod = String::sEmpty() )
{
	return CreateNumericField( inTable, inName, kTypeULong, inFlags, inMethod );
}

/**********************************************************************************************/
inline I_Field_Ptr CreateLLongField(
	I_Table_Ptr 	inTable, 
	const String& 	inName,
	vuint16			inFlags = fNone,
	const String& 	inMethod = String::sEmpty() )
{
	return CreateNumericField( inTable, inName, kTypeLLong, inFlags, inMethod );
}

/**********************************************************************************************/
inline I_Field_Ptr CreateULLongField(
	I_Table_Ptr 	inTable, 
	const String& 	inName,
	vuint16			inFlags = fNone,
	const String& 	inMethod = String::sEmpty() )
{
	return CreateNumericField( inTable, inName, kTypeULLong, inFlags, inMethod );
}

/**********************************************************************************************/
inline I_Field_Ptr CreateDateField(
	I_Table_Ptr 	inTable, 
	const String& 	inName,
	vuint16			inFlags = fNone,
	const String& 	inMethod = String::sEmpty() )
{
	return CreateNumericField( inTable, inName, kTypeDate, inFlags, inMethod );
}

/**********************************************************************************************/
inline I_Field_Ptr CreateTimeField(
	I_Table_Ptr 	inTable, 
	const String& 	inName,
	vuint16			inFlags = fNone,
	const String& 	inMethod = String::sEmpty() )
{
	return CreateNumericField( inTable, inName, kTypeTime, inFlags, inMethod );
}

/**********************************************************************************************/
inline I_Field_Ptr CreateDateTimeField(
	I_Table_Ptr 	inTable, 
	const String& 	inName,
	vuint16			inFlags = fNone,
	const String& 	inMethod = String::sEmpty() )
{
	return CreateNumericField( inTable, inName, kTypeDateTime, inFlags, inMethod );
}


/**********************************************************************************************/
FBL_SHARED_EXP I_Field_Ptr CreateStringField( 
	I_Table_Ptr 	inTable, 
	const String& 	inName,
	vuint32			inMaxLength = 20,
	vuint16			inFlags = fNone,
	const String& 	inMethod = String::sEmpty() );	


/**********************************************************************************************/
FBL_SHARED_EXP I_Field_Ptr CreateVarCharField( 
	I_Table_Ptr 	inTable, 
	const String& 	inName,
	vuint32			inMaxLength = 1022,
	vuint16			inFlags = fNone,
	const String& 	inMethod = String::sEmpty() );	


/**********************************************************************************************/
FBL_SHARED_EXP I_Field_Ptr CreateBLOBField( 
	I_Table_Ptr 	inTable, 
	const String& 	inName,
	vuint32			inSegmentSize = 512,
	vuint16			inFlags = fNone );	


/**********************************************************************************************/
FBL_SHARED_EXP I_Field_Ptr CreateTextField( 
	I_Table_Ptr 	inTable, 
	const String& 	inName,
	vuint32			inSegmentSize = 512,
	vuint16			inFlags = fNone,
	const String& 	inMethod = String::sEmpty() );	


/**********************************************************************************************/
FBL_SHARED_EXP I_Field_Ptr CreatePictureField( 
	I_Table_Ptr 	inTable, 
	const String& 	inName,
	vuint32			inSegmentSize = 512,
	vuint16			inFlags = fNone );	


/**********************************************************************************************/
FBL_SHARED_EXP I_Field_Ptr CreateObjectPtr( 
	I_Table_Ptr 	inTable, 
	const String& 	inName,
	I_Table_Ptr 	inTarget,
	EOnDeletion		inOnDeletion = kOnDelete_Cascade,
	vuint16			inFlags = fIndexed + fNullable,
	const String& 	inLinkName = String::sEmpty() );


/**********************************************************************************************/
FBL_SHARED_EXP I_Field_Ptr CreateEnumField( 
	I_Table_Ptr 			inTable, 
	const String& 			inName,
	I_Type_Enumerated_Ptr 	inpType,
	vuint16					inFlags = fNullable );


/**********************************************************************************************/
FBL_SHARED_EXP I_Field_Ptr CreateMoneyField(
	I_Table_Ptr 	inTable, 
	const String& 	inName,
	vuint16			inFlags = fNone,
	const String& 	inMethod = String::sEmpty() );


/**********************************************************************************************/
FBL_SHARED_EXP ArrayOfValues_Ptr GetNotBLOBValues( I_Table_Ptr inTable );


/**********************************************************************************************/
FBL_SHARED_EXP ArrayOfSerializable_Ptr GetSerializables( ArrayOfValues_Ptr inpValues );


/**********************************************************************************************/

FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_Algs_Table_h
