/**********************************************************************************************/
/* FBL_Value_Factory.cpp																	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Factories/FBL_Value_Factory.h>

// FBL
#include <VShared/FBL/publ/Interfaces/FBL_I_Localizable.h>
#include <VShared/FBL/publ/Values/Factories/FBL_Value_Factories.h>
#include <VShared/FBL/publ/Values/FBL_Value_Enum.h>

// STD:
#include <map>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// Factory of values.
// 	 
typedef std::map<VALUE_TYPE, MAKE_VALUE_FUNC_PTR> TVALUE_FACTORY;


/**********************************************************************************************/
static TVALUE_FACTORY gValueFactory;


/**********************************************************************************************/
bool RegisterValue(
	VALUE_TYPE			inType,
	MAKE_VALUE_FUNC_PTR	inFactory )
{
	if( gValueFactory.find( inType ) == gValueFactory.end() )
	{
	 	gValueFactory[inType] = inFactory;
	 	return true;
	}
	else
	{
		return false;
	}
}


#pragma mark ===== INTERNAL ======

/**********************************************************************************************/
namespace internal 
{

#pragma mark -

/**********************************************************************************************/
I_Value* CreateValue_Enum( 
	I_Type_Enumerated_Ptr	inpType,
	vuint16 				inFlags,	
	bool					inIsRemote )
{
	FBL_CHECK( inpType );
	I_Value* pValue = nullptr;

	switch( inpType->get_MaxIdentCount() )
	{
		case ENUM_8_IDENT_COUNT:
		{
			pValue = (bool(inFlags & fNullable)) ? 
				new Value_enum_null8( inpType ) : new Value_enum8( inpType );
		} break;

		case ENUM_16_IDENT_COUNT:
		{
			pValue = (bool(inFlags & fNullable)) ? 
				new Value_enum_null16( inpType ) : new Value_enum16( inpType );
		} break;

		default:
		{
			FBL_Throw( xFeatureError( ERR_FEATURE_NOT_SUPPORTED, "Not enum8 or enum16" ) );
		}
	}			

	if( inIsRemote )
		pValue->put_IsRemote( inIsRemote );

	return pValue;
}


/**********************************************************************************************/
I_Value* CreateValue(
	VALUE_TYPE 	inType, 
	vuint16		inFlags,
	void*		inParam1,
	void*		inParam2,
	bool		inIsRemote  )
{
	TVALUE_FACTORY::iterator p = gValueFactory.find( inType );
	
	if( p == gValueFactory.end() )
		return nullptr;
	
	MAKE_VALUE_FUNC_PTR pFactory = (*p).second;	
	
	bool Nullable = bool(inFlags & fNullable);
	
	I_Value* pValue = pFactory( Nullable, inParam1, inParam2 );
	
	// on default value has inIsRemote = false, 
	// so we need to assign it only if it is really remote.
	if( inIsRemote )
		pValue->put_IsRemote( inIsRemote );
	
	return pValue;
}


/**********************************************************************************************/
void RegisterStandardValues(void)
{
	RegisterValue(	kTypeBoolean,		CreateValueBoolean		);
	RegisterValue(	kTypeByte,			CreateValueByte			);

	RegisterValue(	kTypeShort,			CreateValueShort		);
	RegisterValue(	kTypeUShort,		CreateValueUShort		);

	RegisterValue(	kTypeMedium,		CreateValueMedium		);
	RegisterValue(	kTypeUMedium,		CreateValueUMedium		);

	RegisterValue(	kTypeLong,			CreateValueLong			);
	RegisterValue(	kTypeULong,			CreateValueULong		);

	RegisterValue(	kTypeLLong,			CreateValueLLong		);
	RegisterValue(	kTypeULLong,		CreateValueULLong		);

	RegisterValue(	kTypeFloat,			CreateValueFloat		);
	RegisterValue(	kTypeDouble,		CreateValueDouble		);

	RegisterValue(	kTypeDate,			CreateValueDate			);
	RegisterValue(	kTypeTime,			CreateValueTime			);
	RegisterValue(	kTypeDateTime,		CreateValueDateTime		);
	
	RegisterValue(	kTypeString,		CreateValueString		);
	RegisterValue(	kTypeVarChar,		CreateValueVarChar		);

	RegisterValue(	kTypeFixedBinary,	CreateValueFixedBinary );
	RegisterValue(	kTypeVarBinary,		CreateValueVarBinary	);

	RegisterValue(	kTypeBLOB,			CreateValueBLOB			);
	RegisterValue(	kTypePicture,		CreateValuePicture		);
	RegisterValue(	kTypeText,			CreateValueText			);

	RegisterValue(	kTypeObjectPtr,		CreateValueObjectPtr	);
	
	RegisterValue(	kTypeRecID,			CreateValueULong	);
	RegisterValue(	kTypeOID,			CreateValueULLong	);
	
	RegisterValue(	kTypeMoney,			CreateValueMoney		);
}


#pragma mark -
#pragma mark ===== INTERNAL WRAPPER FACTORIES =====


/**********************************************************************************************/
I_Value* CreateValue_String( 
	vuint16 			inFlags, 
	tslen 				inMaxLen, 
	I_Localizable_Ptr 	inLocalizable,
	bool				inIsRemote )
{
	return CreateValue( kTypeString, inFlags, &inMaxLen, inLocalizable.get(), inIsRemote );
}


/**********************************************************************************************/
I_Value* CreateValue_VarChar( 
	vuint16 			inFlags, 
	tslen 				inMaxLen, 
	I_Localizable_Ptr 	inLocalizable,
	bool				inIsRemote )
{
	return CreateValue( kTypeVarChar, inFlags, &inMaxLen, inLocalizable.get(), inIsRemote );
}


/**********************************************************************************************/
I_Value* CreateValue_Text( 
	vuint16 			inFlags, 
	I_Localizable_Ptr 	inLocalizable,
	bool				inIsRemote )
{
	return CreateValue( kTypeText, inFlags, nullptr, inLocalizable.get(), inIsRemote );
}


/**********************************************************************************************/
} // namespace internal


/**********************************************************************************************/
FBL_End_Namespace
