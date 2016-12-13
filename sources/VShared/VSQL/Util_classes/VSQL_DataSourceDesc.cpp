/**********************************************************************************************/
/* VSQL_DataSourceDesc.cpp                                                 					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Util_classes/FBL_String.h>
#include <VShared/FBL/publ/Values/Factories/FBL_Value_Factories.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Connectable.h>

#include <VShared/VSQL/Util_classes/VSQL_DataSourceDesc.h>

/**********************************************************************************************/
VSQL_Begin_Namespace

/**********************************************************************************************/
DataSourceDesc::~DataSourceDesc()
{
}


/**********************************************************************************************/
void DataSourceDesc::Init( void )
{
	mpParamNames = new ArrayOfStringsObj();
	mpParamValues = new ArrayOfValues();
}


/**********************************************************************************************/
vuint32 DataSourceDesc::get_ParamCount( void ) const
{
	FBL_CHECK( mpParamNames && mpParamValues );
	FBL_CHECK( mpParamNames->get_Count() == mpParamValues->get_Count() );

	return mpParamNames->get_Count();
}


/**********************************************************************************************/
void DataSourceDesc::put_Param( 
	const String&		inParamName,
	I_Value_Ptr			inParamValue )
{
	FBL_CHECK( mpParamNames && mpParamValues );
	FBL_CHECK( mpParamNames->get_Count() == mpParamValues->get_Count() );

	mpParamNames->AddItem( inParamName );
	mpParamValues->AddItem( inParamValue );
}


/**********************************************************************************************/
const String& DataSourceDesc::get_ParamName( vuint32 inParamIndex ) const
{
	FBL_CHECK( mpParamNames && mpParamValues );
	FBL_CHECK( mpParamNames->get_Count() == mpParamValues->get_Count() );

	return mpParamNames->get_ItemAt(inParamIndex);
}


/**********************************************************************************************/
Const_I_Value_Ptr DataSourceDesc::get_ParamValue( vuint32 inParamIndex ) const
{
	FBL_CHECK( mpParamNames && mpParamValues );
	FBL_CHECK( mpParamNames->get_Count() == mpParamValues->get_Count() );

	return mpParamValues->get_ItemAt(inParamIndex);
}


/**********************************************************************************************/
Const_I_Value_Ptr DataSourceDesc::get_ParamValue( const String& inParamName ) const
{
	FBL_CHECK( mpParamNames && mpParamValues );
	FBL_CHECK( mpParamNames->get_Count() == mpParamValues->get_Count() );

	vuint32 index = mpParamNames->FindIndex( inParamName );
	return (index) ? mpParamValues->get_ItemAt(index) : nullptr;
}


#pragma mark -

/**********************************************************************************************/
void DataSourceDesc::From( I_IStream_Ptr inStream, bool inBlock )
{
	// No implementation because nobody need it
	argused2( inStream, inBlock );
}


/**********************************************************************************************/
void DataSourceDesc::From( I_PacketRcv* inPacket, bool inBlock )
{
#ifdef V_SERVER

	mDataSourceType = (VSQL::EDataSourceType)inPacket.get_UShortParam();

	if( mDataSourceType == VSQL::kValentina )
	{
		ProxyPtr inDbId = inPacket->get_RemoteIDParam();
		I_RemoteProxy* proxy = reinterpret_cast<I_RemoteProxy*>(inDbId);
		if( !proxy )
			FBL_Throw(ERR_INTERNAL_INVALID_PARAMETER);
		I_SqlDatabase_Ptr db = QI( proxy->get_I_Unknown(), I_SqlDatabase );

		outDataSourceDesc->put_ValentinaDb( db );
	}

	vuint32 paramCount = inPacket->get_ULongParam();

	String paramName;
	I_Value_Ptr paramValue;
	for( vuint32 i = 1; i <= paramCount; ++i )
	{
		// Get param name
		inPacket->get_StringParam( paramName );

		// Get param value
		GetValueParam( inPacket, paramValue, NULL );

		VALUE_TYPE vt = inPacket->get_ULongParam();
		vuint32 maxChars = inPacket->get_ULongParam();
		vuint16 attr = vuint16( inPacket->get_BoolParam() ? fNullable : fNone);

		paramValue = FBL::internal::CreateValue(vt, attr, &maxChars, NULL, false );

		bool isNull = inPacket->get_BoolParam();
		paramValue->put_IsNull( isNull );

		if( isNull == false )
		{
			I_Serializable_Ptr pSerial = QI( paramValue, I_Serializable );
			if( pSerial )
				pSerial->From(&inPacket);
		}

		// Put the param into this
		put_Param( paramName, paramValue );
	}
#else
	//
	// Not implemented - because we need to pass the info to vServer only
	//
	argused2( inPacket, inBlock );
#endif // V_SERVER
}


/**********************************************************************************************/
void DataSourceDesc::To( I_OStream_Ptr inStream, bool inBlock ) const
{
	// No implementation because nobody need it
	argused2( inStream, inBlock );
}


/**********************************************************************************************/
void DataSourceDesc::To( I_PacketSnd*  inPacket, bool inBlock ) const
{
#ifdef V_SERVER
	//
	// Not implemented - because we need to pass the info to vServer only
	//
	argused2( inPacket, inBlock );
#else
	
	argused1(inBlock);
	FixParamCount fixParamCount( inPacket );

	inPacket->put_UShortParam( (vuint16)mDataSourceType );
	if( mDataSourceType == VSQL::kValentina )
	{
		I_Connectable* pDb = QI( mpValentinaDb, I_Connectable );
		FBL_CHECK( pDb );
		inPacket->put_RemoteIDParam( pDb->get_RemoteID() );
	}

	vuint32 paramCount = get_ParamCount();
	inPacket->put_ULongParam( paramCount );
	for( vuint32 i = 1; i <= paramCount; ++i )
	{
		const String& paramName = get_ParamName(i);
		Const_I_Value_Ptr paramValue = get_ParamValue(i);

		// Put param name
		inPacket->put_StringParam( paramName );
		
		// Put param value
		FBL_CHECK( paramValue );
		inPacket->put_ULongParam( paramValue->get_Type() );
		inPacket->put_ULongParam( paramValue->get_Length() );
		inPacket->put_BoolParam( paramValue->get_IsNullable() );

		bool isNull = paramValue->get_IsNull();
		inPacket->put_BoolParam( isNull );

		if( isNull == false ) 
		{
			Const_I_Serializable_Ptr pSerial = QI( paramValue, I_Serializable );
			if( pSerial )
				pSerial->To(inPacket);
		}
	}
#endif // V_SERVER
}


#pragma mark -

/**********************************************************************************************/
DataSourceDesc_Ptr CreateDataSource_Valentina( 
	I_SqlDatabase_Ptr		inpDb,
	ECursorLocation			inCursorLocation,
	ELockType				inLockType,
	ECursorDirection		inCursorDirection )
{
	DataSourceDesc_Ptr pRes = new DataSourceDesc( kValentina );
	pRes->put_ValentinaDb( inpDb );

	I_Value_Ptr pValCursorLocation = CreateValueUShort(true,nullptr,nullptr);
	pValCursorLocation->put_UShort( (vuint16)inCursorLocation );
	pRes->put_Param( "ECursorLocation",  pValCursorLocation );

	I_Value_Ptr pValLockType = CreateValueUShort(true,nullptr,nullptr);
	pValLockType->put_UShort( (vuint16)inLockType );
	pRes->put_Param( "ELockType", pValLockType );

	I_Value_Ptr pValCursorDirection = CreateValueUShort(true,nullptr,nullptr);
	pValCursorDirection->put_UShort( (vuint16)inCursorDirection );
	pRes->put_Param( "ECursorDirection", pValCursorDirection );

	return pRes;
}


/**********************************************************************************************/
/*
DataSourceDesc_Ptr CreateDataSource_SqlLite( String inParam1 ... )
{

}
*/


/**********************************************************************************************/
VSQL_End_Namespace
