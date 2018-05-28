/**********************************************************************************************/
/* FBL_Algs_Table.cpp																		  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Algorithms/FBL_Algs_Table.h>

#include <VShared/FBL/publ/Properties/FBL_FieldProperties.h> 


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
I_Field_Ptr CreateNumericField( 
	I_Table_Ptr 	inTable, 
	const String& 	inName,
	VALUE_TYPE		inType,
	vuint16			inFlags,
	const String& 	inMethod )
{
	I_PropertyContainer_Ptr props;	
	
	//if( inMethod != NULL && *inMethod != 0 )
	if( inMethod.isEmpty() == false )
	{
		props = new PropertyContainer();
		props->Add( new Prop_MethodSql( inMethod ) );
	}
	
	I_Field_Ptr pField = inTable->CreateField( inName, inType, inFlags, props );

	return pField;
}
		

/**********************************************************************************************/
I_Field_Ptr CreateFloatField(
	I_Table_Ptr 	inTable, 
	const String& 	inName,
	vuint16			inFlags,
	const String& 	inMethod,
	vuint16			inPrecision,
	vuint16			inScale )
{
	I_PropertyContainer_Ptr props = new PropertyContainer();
	
	if( inPrecision )
		props->Add( new Prop_Precision( inPrecision ) );
	if( inScale )
		props->Add( new Prop_Scale( inScale ) );

	if( inMethod.isEmpty() == false )
		props->Add( new Prop_MethodSql( inMethod ) );

	I_Field_Ptr pField = inTable->CreateField( inName, kTypeFloat, inFlags, props );

	return pField;
}


/**********************************************************************************************/
I_Field_Ptr CreateDoubleField(
	I_Table_Ptr 	inTable, 
	const String& 	inName,
	vuint16			inFlags,
	const String& 	inMethod,
	vuint16			inPrecision,
	vuint16			inScale )
{
	I_PropertyContainer_Ptr props = new PropertyContainer();

	if( inPrecision )
		props->Add( new Prop_Precision( inPrecision ) );
	if( inScale )
		props->Add( new Prop_Scale( inScale ) );

	if( inMethod.isEmpty() == false )
		props->Add( new Prop_MethodSql( inMethod ) );

	I_Field_Ptr pField = inTable->CreateField( inName, kTypeDouble, inFlags, props );

	return pField;
}


/**********************************************************************************************/
I_Field_Ptr CreateStringField( 
	I_Table_Ptr 	inTable, 
	const String& 	inName,
	vuint32			inMaxLength,
	vuint16			inFlags,
	const String& 	inMethod )	
{
	I_PropertyContainer_Ptr props = new PropertyContainer();	

	props->Add( new Prop_MaxLen(inMaxLength) );

	if( inMethod.isEmpty() == false )
		props->Add( new Prop_MethodSql( inMethod ) );

	// ---------------
	I_Field_Ptr pField = inTable->CreateField( inName, kTypeString, inFlags, props );
		
	return pField;
}


/**********************************************************************************************/
I_Field_Ptr CreateVarCharField( 
	I_Table_Ptr 	inTable, 
	const String& 	inName,
	vuint32			inMaxLength,
	vuint16			inFlags,
	const String& 	inMethod )	
{
	I_PropertyContainer_Ptr props = new PropertyContainer();	

	props->Add( new Prop_MaxLen(inMaxLength) );

	if( inMethod.isEmpty() == false )
		props->Add( new Prop_MethodSql( inMethod ) );

	// ---------------
	I_Field_Ptr pField = inTable->CreateField( inName, kTypeVarChar, inFlags, props );
		
	return pField;
}


/**********************************************************************************************/
I_Field_Ptr CreateVarBinaryField(
	I_Table_Ptr 	inTable, 
	const String& 	inName,
	vuint32			inMaxLength,
	vuint16			inFlags,
	const String& 	inMethod )	
{
	I_PropertyContainer_Ptr props = new PropertyContainer();	

	props->Add( new Prop_MaxLen(inMaxLength) );

	if( inMethod.isEmpty() == false )
		props->Add( new Prop_MethodSql( inMethod ) );

	// ---------------
	I_Field_Ptr pField = inTable->CreateField( inName, kTypeVarBinary, inFlags, props );
		
	return pField;
}


/**********************************************************************************************/
I_Field_Ptr CreateBLOBField( 
	I_Table_Ptr 	inTable, 
	const String& 	inName,
	vuint32			inSegmentSize,
	vuint16			inFlags )	
{
	I_PropertyContainer_Ptr props = new PropertyContainer();	
	
	props->Add( new Prop_SegmentSize(inSegmentSize) );

	// ---------------
	I_Field_Ptr	pField = inTable->CreateField( inName, kTypeBLOB, inFlags, props );
		
	return pField;
}


/**********************************************************************************************/
I_Field_Ptr CreateTextField( 
	I_Table_Ptr 	inTable, 
	const String& 	inName,
	vuint32			inSegmentSize,
	vuint16			inFlags,
	const String& 	inMethod )	
{
	I_PropertyContainer_Ptr props = new PropertyContainer();	
	
	props->Add( new Prop_SegmentSize(inSegmentSize) );

	if( inMethod.isEmpty() == false )
		props->Add( new Prop_MethodSql( inMethod ) );

	// ---------------
	I_Field_Ptr pField = inTable->CreateField( inName, kTypeText, inFlags, props );
		
	return pField;
}


/**********************************************************************************************/
I_Field_Ptr CreatePictureField( 
	I_Table_Ptr 	inTable, 
	const String& 	inName,
	vuint32			inSegmentSize,
	vuint16			inFlags )	
{
	I_PropertyContainer_Ptr props = new PropertyContainer();	
	
	props->Add( new Prop_SegmentSize(inSegmentSize) );

	// ---------------
	I_Field_Ptr pField = inTable->CreateField( inName, kTypePicture, inFlags, props );
		
	return pField;
}


/**********************************************************************************************/
I_Field_Ptr CreateObjectPtr( 
	I_Table_Ptr 	inTable, 
	const String& 	inName,
	I_Table_Ptr 	inTarget,
	EOnDeletion		inOnDeletion,
	vuint16			inFlags,
	const String& 	inLinkName )
{
	I_PropertyContainer_Ptr props = new PropertyContainer();	
	
	props->Add( new Prop_Target(inTarget) );
	props->Add( new Prop_OnDeletion(inOnDeletion) ); 
	
	if( inLinkName.isEmpty() == false )
		props->Add( new Prop_LinkName( inLinkName ) );

	// ---------------
	I_Field_Ptr pField = inTable->CreateField( inName, kTypeObjectPtr, inFlags, props );
		
	return pField;
}


/**********************************************************************************************/
I_Field_Ptr CreateEnumField( 
	I_Table_Ptr 			inTable, 
	const String& 			inName,
	I_Type_Enumerated_Ptr 	inpType,
	vuint16					inFlags )
{
	I_PropertyContainer_Ptr props = new PropertyContainer();	

	props->Add( new Prop_EnumType(inpType) );

	// ---------------
	I_Field_Ptr pField;

	switch( inpType->get_MaxIdentCount() )
	{
		case ENUM_8_IDENT_COUNT:
		{
			pField = inTable->CreateField( inName, kTypeEnum8, inFlags, props );
		} break;

		case ENUM_16_IDENT_COUNT:
		{
			pField = inTable->CreateField( inName, kTypeEnum16, inFlags, props );
		} break;

		default:
		{
			FBL_Throw( xFeatureError( ERR_FEATURE_NOT_SUPPORTED, "Not enum8 or enum16" ) );
		}
	}
	

	return pField;
}


/**********************************************************************************************/
I_Field_Ptr CreateMoneyField(
	I_Table_Ptr 	inTable, 
	const String& 	inName,
	vuint16			inFlags,
	const String& 	inMethod )
{
	I_PropertyContainer_Ptr props = new PropertyContainer();

	if( inMethod.isEmpty() == false )
		props->Add( new Prop_MethodSql( inMethod ) );

	I_Field_Ptr pField = inTable->CreateField( inName, kTypeMoney, inFlags, props );

	return pField;
}


/**********************************************************************************************/
I_Field_Ptr CreateVariantField(
	I_Table_Ptr 	inTable, 
	const String& 	inName,
	vuint16			inFlags )
{
	I_Field_Ptr pField = inTable->CreateField( inName, kTypeVariant, inFlags );
	return pField;
}


/**********************************************************************************************/
ArrayOfValues_Ptr GetNotBLOBValues( I_Table_Ptr inTable )
{
	FBL_CHECK( inTable );
	ArrayOfValues_Ptr result;

	vuint16 fldCount = inTable->get_FieldCount();
	if( fldCount )
		result = new ArrayOfValues();

	for( vuint16 i = 1; i <= fldCount; ++i )
	{
		I_Field_Ptr	pField = inTable->get_Field( i );
		I_Value_Ptr pValue;

		switch( pField->get_Type() )
		{
			case kTypeBLOB:
			case kTypePicture: 
			{
				;
			} break;

			default: 
			{
				// IS: 25.02.2008
				// http://valentina-db.com/bt/view.php?id=3048
				//pValue = pField->get_Value(forAdd);
				pValue = fbl_const_cast( pField->get_Value() );
				FBL_CHECK(pValue);							

			} break;
		}
		
		result->AddItem( pValue );
	}

	return result;
}


/**********************************************************************************************/
ArrayOfSerializable_Ptr GetSerializables( ArrayOfValues_Ptr inpValues )
{
	FBL_CHECK( inpValues );
	ArrayOfSerializable_Ptr result;

	vuint32 valCount = inpValues->get_Count();
	if( valCount )
		result = new ArrayOfSerializable();

	for( vuint32 i = 1; i <= valCount; ++i )
	{
		I_Value_Ptr	pValue = inpValues->get_ItemAt( i );		
		I_Serializable_Ptr pSerial = (pValue) ? QI(pValue, I_Serializable) : nullptr;
		result->AddItem( pSerial );
	}

	return result;
}


/**********************************************************************************************/
FBL_End_Namespace
