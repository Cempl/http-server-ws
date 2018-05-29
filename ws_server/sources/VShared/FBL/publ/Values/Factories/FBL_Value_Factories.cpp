/**********************************************************************************************/
/* FBL_Value_Factories.cpp																	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Values/Factories/FBL_Value_Factories.h>

// VShared/FBL/publ:
#include <VShared/FBL/publ/Interfaces/FBL_I_Localizable.h> 

#include <VShared/FBL/publ/Values/FBL_Value_Binary.h>
#include <VShared/FBL/publ/Values/FBL_Value_Compound.h>
#include <VShared/FBL/publ/Values/FBL_Value_Date.h>
#include <VShared/FBL/publ/Values/FBL_Value_DateTime.h>
#include <VShared/FBL/publ/Values/FBL_Value_Double.h>
#include <VShared/FBL/publ/Values/FBL_Value_Float.h>
#include <VShared/FBL/publ/Values/FBL_Value_Numeric.h>
#include <VShared/FBL/publ/Values/FBL_Value_String.h>
#include <VShared/FBL/publ/Values/FBL_Value_Text.h>
#include <VShared/FBL/publ/Values/FBL_Value_Time.h>
#include <VShared/FBL/publ/Values/FBL_Value_Money.h>
#include <VShared/FBL/publ/Values/FBL_Value_Variant.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
I_Value* toValue_bool( vuint32 inValue )
{
	I_Value* pValue = new FBL::Value_bool();
	
	pValue->put_Boolean( inValue );
	
	return pValue;
}


/**********************************************************************************************/
I_Value* toValue_byte( vuint8 inValue )
{
	I_Value* pValue = new FBL::Value_byte();
	
	pValue->put_Byte( inValue );
	
	return pValue;
}


/**********************************************************************************************/
I_Value* toValue_short( vint16 inValue )
{
	I_Value* pValue = new FBL::Value_short();
	
	pValue->put_Short( inValue );
	
	return pValue;
}


/**********************************************************************************************/
I_Value* toValue_ushort( vuint16 inValue )
{
	I_Value* pValue = new FBL::Value_ushort();
	
	pValue->put_UShort( inValue );
	
	return pValue;
}


/**********************************************************************************************/
I_Value* toValue_medium( vint32 inValue )
{
	I_Value* pValue = new FBL::Value_medium();
	
	pValue->put_Long( inValue );
	
	return pValue;
}


/**********************************************************************************************/
I_Value* toValue_umedium( vuint32 inValue )
{
	I_Value* pValue = new FBL::Value_umedium();
	
	pValue->put_ULong( inValue );
	
	return pValue;
}


/**********************************************************************************************/
I_Value* toValue_long( vint32 inValue )
{
	I_Value* pValue = new FBL::Value_long();
	
	pValue->put_Long( inValue );
	
	return pValue;
}


/**********************************************************************************************/
I_Value* toValue_ulong( vuint32 inValue )
{
	I_Value* pValue = new FBL::Value_ulong();
	
	pValue->put_ULong( inValue );
	
	return pValue;
}


/**********************************************************************************************/
I_Value* toValue_llong( vint64 inValue )
{
	I_Value* pValue = new FBL::Value_llong();
	
	pValue->put_LLong( inValue );
	
	return pValue;
}


/**********************************************************************************************/
I_Value* toValue_ullong( vuint64 inValue )
{
	I_Value* pValue = new FBL::Value_ullong();
	
	pValue->put_ULLong( inValue );
	
	return pValue;
}


/**********************************************************************************************/
I_Value* toValue_float( float inValue )
{
	I_Value* pValue = new FBL::Value_float();
	
	pValue->put_Float( inValue );
	
	return pValue;
}


/**********************************************************************************************/
I_Value* toValue_double( double inValue )
{
	I_Value* pValue = new FBL::Value_double();
	
	pValue->put_Double( inValue );
	
	return pValue;
}


/**********************************************************************************************/
I_Value* toValue_string( const FBL::String& inStrObject )
{
	I_Value* pValue = new FBL::Value_string( inStrObject.length() + 1 );

	pValue->put_String( inStrObject.begin(), inStrObject.end() ); // not nullable
	
	return pValue;
}


/**********************************************************************************************/
I_Value* toValue_varchar( const FBL::String& inStrObject )
{
	I_Value* pValue = new FBL::Value_varchar( inStrObject.length() + 1 );

	pValue->put_String(	inStrObject.begin(), inStrObject.end() ); // not nullable
	
	return pValue;
}


/**********************************************************************************************/
I_Value* toValue_text( const FBL::String& inStrObject )
{
	I_Value* pValue = new FBL::Value_text_null();
	
	pValue->put_String( inStrObject.begin(), inStrObject.end() );
	
	return pValue;
}


/**********************************************************************************************/
I_Value* toValue_date( 
	vint32		inYear,
	vuint16		inMonth, 
	vuint16		inDay )
{
	Value_date* pValue = new FBL::Value_date();
	
	pValue->put_Date( inYear, inMonth, inDay );
	
	return pValue;
}


/**********************************************************************************************/
I_Value* toValue_time( 
	vuint16 	inHours, 
	vuint16 	inMinutes,
	vuint16 	inSeconds, 
	vuint16		inMilliseconds )
{
	Value_time* pValue = new FBL::Value_time();
	
	pValue->put_Time( inHours, inMinutes, inSeconds, inMilliseconds );
	
	return pValue;
}


/**********************************************************************************************/
I_Value* toValue_datetime( 
	vint32		inYear,
	vuint16		inMonth, 
	vuint16		inDay,
	vuint16 	inHours, 
	vuint16 	inMinutes,
	vuint16 	inSeconds, 
	vuint16		inMilliseconds )
{
	Value_datetime* pValue = new FBL::Value_datetime();
	
	pValue->put_Date( inYear, inMonth, inDay );
	pValue->put_Time( inHours, inMinutes, inSeconds, inMilliseconds );
	
	return pValue;
}


#pragma mark -


/**********************************************************************************************/
I_Value* CreateValueBoolean( bool inNullable, void* inParam1, void* inParam2 )
{
	argused2( inParam1, inParam2 );

	if( inNullable )
		return new Value_bool_null();
	else
		return new Value_bool();
}


/**********************************************************************************************/
I_Value* CreateValueByte( bool inNullable, void* inParam1, void* inParam2 )
{
	argused2( inParam1, inParam2 );
	
	if( inNullable )
		return new Value_byte_null();
	else
		return new Value_byte();
}


/**********************************************************************************************/
I_Value* CreateValueShort( bool inNullable, void* inParam1, void* inParam2 )
{
	argused2( inParam1, inParam2 );
	
	if( inNullable )
		return new Value_short_null();
	else
		return new Value_short();
}


/**********************************************************************************************/
I_Value* CreateValueUShort( bool inNullable, void* inParam1, void* inParam2 )
{
	argused2( inParam1, inParam2 );
	
	if( inNullable )
		return new Value_ushort_null();
	else
		return new Value_ushort();
}


/**********************************************************************************************/
I_Value* CreateValueLong( bool inNullable, void* inParam1, void* inParam2 )
{
	argused2( inParam1, inParam2 );
	
	if( inNullable )
		return new Value_long_null();
	else
		return new Value_long();
}


/**********************************************************************************************/
I_Value* CreateValueULong( bool inNullable, void* inParam1, void* inParam2 )
{
	argused2( inParam1, inParam2 );
	
	if( inNullable )
		return new Value_ulong_null();
	else
		return new Value_ulong();
}


/**********************************************************************************************/
I_Value* CreateValueLLong( bool inNullable, void* inParam1, void* inParam2 )
{
	argused2( inParam1, inParam2 );
	
	if( inNullable )
		return new Value_llong_null();
	else
		return new Value_llong();
}


/**********************************************************************************************/
I_Value* CreateValueULLong( bool inNullable, void* inParam1, void* inParam2 )
{
	argused2( inParam1, inParam2 );
	
	if( inNullable )
		return new Value_ullong_null();
	else
		return new Value_ullong();
}


#pragma mark -



/**********************************************************************************************/
I_Value* CreateValueFloat( bool inNullable, void* inParam1, void* inParam2 )
{
	vuint16 Precission = inParam1 ? *reinterpret_cast<vuint16*>(inParam1) : gFloatDefaultPrecision;
	vuint16 Scale      = inParam2 ? *reinterpret_cast<vuint16*>(inParam2) : gFloatDefaultScale;
	
	if( inNullable )
		return new Value_float_null( Precission, Scale );
	else
		return new Value_float( Precission, Scale );
}


/**********************************************************************************************/
I_Value* CreateValueDouble( bool inNullable, void* inParam1, void* inParam2 )
{
	vuint16 Precission = inParam1 ? *reinterpret_cast<vuint16*>(inParam1) : gDoubleDefaultPrecision;
	vuint16 Scale      = inParam2 ? *reinterpret_cast<vuint16*>(inParam2) : gDoubleDefaultScale;
	
	if( inNullable )
		return new Value_double_null( Precission, Scale );
	else
		return new Value_double( Precission, Scale );
}


#pragma mark -


/**********************************************************************************************/
I_Value* CreateValueString( bool inNullable, void* inParam1, void* inParam2 )
{
	I_Value* res = nullptr;
	
	tslen MaxLen = inParam1 ? *reinterpret_cast<tslen*>(inParam1) : 20;
	
	I_Localizable* pLocalizable = (I_Localizable*) inParam2;
	
	if( inNullable )
		res = new Value_string_null( MaxLen, pLocalizable );
	else
		res = new Value_string( MaxLen, pLocalizable );
	
	return res;	
}


/**********************************************************************************************/
I_Value* CreateValueVarChar( bool inNullable, void* inParam1, void* inParam2 )
{
	argused1( inNullable );

	tslen MaxLen = inParam1 ? *reinterpret_cast<tslen*>(inParam1) : kMaxLengthOfVarChar_UTF16;

	I_Localizable* pLocalizable = (I_Localizable*) inParam2;
	
	if( inNullable )
		return new Value_varchar_null( MaxLen, pLocalizable );
	else
		return new Value_varchar( MaxLen, pLocalizable );
}


#pragma mark -


/**********************************************************************************************/
I_Value* CreateValueDate( bool inNullable, void* inParam1, void* inParam2 )
{
	argused1( inParam2 );
	
	if( !inParam1 )
	{
		// set default ?
	}
	
	I_Database* p = reinterpret_cast<I_Database*>(inParam1);

	if( inNullable )
		return new Value_date_null( p );
	else
		return new Value_date( p );
}


/**********************************************************************************************/
I_Value* CreateValueTime( bool inNullable, void* inParam1, void* inParam2 )
{
	argused1( inParam2 );
	
	if( !inParam1 )
	{
		// set default ?
	}
	
	I_Database* p = reinterpret_cast<I_Database*>(inParam1);
	
	if( inNullable )
		return new Value_time_null( p );
	else
		return new Value_time( p );
}


/**********************************************************************************************/
I_Value* CreateValueDateTime( bool inNullable, void* inParam1, void* inParam2 )
{
	argused1( inParam2 );
	
	if( !inParam1 )
	{
		// set default ?
	}
	
	I_Database* p = reinterpret_cast<I_Database*>(inParam1);
	
	if( inNullable )
		return new Value_datetime_null( p );
	else
		return new Value_datetime( p );
}


/**********************************************************************************************/
I_Value* CreateValueMedium( bool inNullable, void* inParam1, void* inParam2 )
{
	argused2( inParam1, inParam2 );
	
	if( inNullable )
		return new Value_medium_null();
	else
		return new Value_medium();
}


/**********************************************************************************************/
I_Value* CreateValueUMedium( bool inNullable, void* inParam1, void* inParam2 )
{
	argused2( inParam1, inParam2 );

	if( inNullable )
		return new Value_umedium_null();
	else
		return new Value_umedium();
}


#pragma mark -
#pragma mark ==== Binary Value Factories


/**********************************************************************************************/
I_Value* CreateValueFixedBinary( bool inNullable, void* inParam1, void* inParam2 )
{
	argused1( inParam2 );
	
	vuint32 MaxLen = inParam1   ? *reinterpret_cast<vuint32*>(inParam1)               
                                : 20;

	if( inNullable )
		return new Value_fixedbinary_null( MaxLen, 2 );
	else
		return new Value_fixedbinary( MaxLen, 2 );
}


/**********************************************************************************************/
I_Value* CreateValueVarBinary( bool inNullable, void* inParam1, void* inParam2 )
{
	argused1( inParam2 );
	
	vuint32 MaxLen = inParam1   ? *reinterpret_cast<vuint32*>(inParam1)
                                : 20;
	
	if( inNullable )
		return new Value_varbinary_null( MaxLen, 0 );
	else
		return new Value_varbinary( MaxLen, 0 );
}


/**********************************************************************************************/
I_Value* CreateValueBLOB( bool inNullable, void* inParam1, void* inParam2 )
{
	argused3( inNullable, inParam1, inParam2 );

	return new Value_blob_null();
}


/**********************************************************************************************/
I_Value* CreateValuePicture( bool inNullable, void* inParam1, void* inParam2 )
{
	argused3( inNullable, inParam1, inParam2 );

	return new Value_picture_null();
}


/**********************************************************************************************/
I_Value* CreateValueText( bool inNullable, void* inParam1, void* inParam2 )
{
	argused1( inNullable );

	I_Localizable* pLocalizable = (I_Localizable*) inParam2;
	
	Value_text_null* pv = new Value_text_null( pLocalizable );

	// We return text value with a buffer of the required size.
	if( inParam1 )
		pv->ResizeChars( *(vint32*) inParam1 );
	
	return pv;
}


/**********************************************************************************************/
I_Value* CreateValueObjectPtr( bool inNullable, void* inParam1, void* inParam2 )
{
	argused2( inParam1, inParam2 );
	
	if( inNullable )
		return new Value_objectPtr_null();
	else
		return new Value_objectPtr();
}


/**********************************************************************************************/
I_Value* CreateValueCompound( ArrayOfValues_Ptr inValues )
{
    return new Value_Compound( inValues );
}


/**********************************************************************************************/
I_Value* CreateValueMoney( bool inNullable, void* inParam1, void* inParam2 )
{
	argused2( inParam1, inParam2 );
	
	if( inNullable )
		return new Value_money_null();
	else
		return new Value_money();
}


/**********************************************************************************************/
I_Value* CreateValueVariant( bool inNullable, void* inParam1, void* inParam2 )
{
	argused1( inParam2 );

	I_Database* p = reinterpret_cast<I_Database*>(inParam1);

	if( inNullable )
		return new Value_Variant_null(p);
	else
		return new Value_Variant(p);
}


/**********************************************************************************************/
FBL_End_Namespace
