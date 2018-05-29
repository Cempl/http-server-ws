/**********************************************************************************************/
/* VSQL_DataSourceDesc.h                                                   					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _VSQL_DataSourceDesc_h
	#define _VSQL_DataSourceDesc_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Interfaces/FBL_I_Value.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Serializable.h>

// VSQL:
#include <VShared/VSQL/Headers/VSQL.h>
#include <VShared/VSQL/Interfaces/VSQL_I_SqlDatabase.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
VSQL_Begin_Namespace


/**********************************************************************************************/
SMART_CLASS	( DataSourceDesc );


/**********************************************************************************************/
// Factories:
//
FBL_SHARED_EXP DataSourceDesc_Ptr CreateDataSource_Valentina( 
	I_SqlDatabase_Ptr		inpDb,
	ECursorLocation			inCursorLocation,
	ELockType				inLockType,
	ECursorDirection		inCursorDirection );

//FBL_SHARED_EXP DataSourceDesc_Ptr CreateDataSource_SqlLite( String inParam1 ... );


/**********************************************************************************************/
enum EDataSourceType
{
	kUndefined	= 0,
	kValentina	= 1,
	kSqlLite	= 2,
	kPostgreSql	= 3 
};


/**********************************************************************************************/
// This class describes connection properties to any data source
//
class FBL_SHARED_EXP_CLASS DataSourceDesc : public I_Serializable
{
		IMPLEMENT_UNKNOWN(DataSourceDesc)											
			BEGIN_INTERFACE_TABLE(DataSourceDesc)										
				IMPLEMENT_INTERFACE(I_Serializable)										
			END_INTERFACE_TABLE()

	public://///////////////////////////////////////////////////////////////////////////////////

								DataSourceDesc()
								:
									mDataSourceType(kUndefined)
								{
									Init();
								}

								DataSourceDesc( EDataSourceType inType )
								:
									mDataSourceType(inType)
								{
									Init();
								}
							
virtual							~DataSourceDesc( void );


	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// Properties:

		void					put_Type( EDataSourceType inType )
									{ mDataSourceType = inType; }

		EDataSourceType			get_Type( void ) const
									{ return mDataSourceType; }

		// <Connection params Key/Value> [r/w]

		vuint32					get_ParamCount( void ) const;

		void					put_Param( 
									const String&		inParamName,
									I_Value_Ptr			inParamValue );

		const String&			get_ParamName( 
									vuint32				inParamIndex ) const;

		Const_I_Value_Ptr		get_ParamValue( 
									vuint32				inParamIndex ) const;

		Const_I_Value_Ptr		get_ParamValue( 
									const String&		inParamName ) const;


		// Valentina specific properties:

		I_SqlDatabase_Ptr		get_ValentinaDb( void )
									{ return mpValentinaDb; }

		void					put_ValentinaDb( I_SqlDatabase_Ptr inpDb )
									{ mpValentinaDb = inpDb; }
								

	public://///////////////////////////////////////////////////////////////////////////////////

	// interface I_Serializable:

virtual void					From( I_IStream_Ptr inStream, bool inBlock = true ) override;
virtual void					From( I_PacketRcv* 	inPacket, bool inBlock = true ) override;

virtual void					To( I_OStream_Ptr inStream, bool inBlock = true ) const override;
virtual void					To( I_PacketSnd*  inPacket, bool inBlock = true ) const override;


	protected://////////////////////////////////////////////////////////////////////////////////
		
		void					Init( void );

	private:////////////////////////////////////////////////////////////////////////////////////


		EDataSourceType			mDataSourceType;

		ArrayOfStringsObj_Ptr	mpParamNames;
		ArrayOfValues_Ptr		mpParamValues;

		I_SqlDatabase_Ptr		mpValentinaDb;
};


/**********************************************************************************************/
VSQL_End_Namespace

#endif // _VSQL_DataSourceDesc_h
