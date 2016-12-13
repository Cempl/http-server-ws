/**********************************************************************************************/
/* FBL_ExportProperties.h                                                   				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_ExportProperties_h
	#define _FBL_ExportProperties_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Properties/FBL_Property.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// This module classes:
// 
SMART_CLASS( Prop_FieldDelimiter );
SMART_CLASS( Prop_RecordDelimiter );
SMART_CLASS( Prop_HasColumnHeader );
SMART_CLASS( Prop_Encoding );


/**********************************************************************************************/
class Prop_FieldDelimiter : public PropertyString
{
	public://///////////////////////////////////////////////////////////////////////////////////
 
 							Prop_FieldDelimiter( const UChar* inDelimiter )
 							:
 								PropertyString( String("FieldDelimiter"), inDelimiter )
 							{
							}

							Prop_FieldDelimiter( const char* inDelimiter )
 							:
 								PropertyString( String("FieldDelimiter"), inDelimiter )
 							{
							}

							Prop_FieldDelimiter( const String& inDelimiter )
 							:
 								PropertyString( String("FieldDelimiter"), inDelimiter )
							{
							} 							
};


/**********************************************************************************************/
class Prop_RecordDelimiter : public PropertyString
{
	public://///////////////////////////////////////////////////////////////////////////////////
 
 							Prop_RecordDelimiter( const UChar* inDelimiter )
 							:
 								PropertyString( "RecordDelimiter", inDelimiter )
 							{
							}

							Prop_RecordDelimiter( const char* inDelimiter )
 							:
 								PropertyString( "RecordDelimiter", inDelimiter )
 							{
							}

							Prop_RecordDelimiter( const String& inDelimiter )
 							:
 								PropertyString( String("RecordDelimiter"), inDelimiter )
							{
							} 					
};


/**********************************************************************************************/
class Prop_HasColumnHeader : public PropertyBoolean
{
	public://///////////////////////////////////////////////////////////////////////////////////
 
 							Prop_HasColumnHeader( bool inOn )
 							:
 								PropertyBoolean( "HasColumnHeader", inOn )
 							{
							}
 							
};


/**********************************************************************************************/
class Prop_MaxRecordsToImport : public PropertyLong
{
	public://///////////////////////////////////////////////////////////////////////////////////
	
							Prop_MaxRecordsToImport( vuint32 inMaxRecordsToImport )
							:
								PropertyLong( "MaxRecordsToImport", (vint32) inMaxRecordsToImport )
							{
							}	
};


#pragma mark -


/**********************************************************************************************/
class Prop_Encoding_Export : public PropertyString
{
	public://///////////////////////////////////////////////////////////////////////////////////
 
 							Prop_Encoding_Export( const UChar* inEncoding )
 							:
 								PropertyString( "Encoding", inEncoding )
 							{
							}

							Prop_Encoding_Export( const char* inEncoding )
 							:
 								PropertyString( "Encoding", inEncoding )
 							{
							}

							Prop_Encoding_Export( const String& inEncoding )
 							:
 								PropertyString( String("Encoding"), inEncoding )
							{
							} 					 							
};


/**********************************************************************************************/
inline 
I_PropertyContainer_Ptr Create_Export_Properties(
	const String& 	inFieldDelimiter,
	const String& 	inRecordDelimiter,
	bool 			inHasColumnHeader,
	const String& 	inEncoding )
{
	I_PropertyContainer* pPC = new PropertyContainer;
	
	pPC->Add(new Prop_FieldDelimiter(inFieldDelimiter));
	pPC->Add(new Prop_RecordDelimiter(inRecordDelimiter));
	pPC->Add(new Prop_HasColumnHeader(inHasColumnHeader));
	pPC->Add(new Prop_Encoding_Export(inEncoding));

	return pPC;
}


/**********************************************************************************************/
inline 
I_PropertyContainer_Ptr Create_Export_Properties(
	const UChar* 	inFieldDelimiter,
	const UChar* 	inRecordDelimiter,
	bool 			inHasColumnHeader,
	const UChar* 	inEncoding )
{
	I_PropertyContainer* pPC = new PropertyContainer;
	
	pPC->Add(new Prop_FieldDelimiter(inFieldDelimiter));
	pPC->Add(new Prop_RecordDelimiter(inRecordDelimiter));
	pPC->Add(new Prop_HasColumnHeader(inHasColumnHeader));
	pPC->Add(new Prop_Encoding_Export(inEncoding));

	return pPC;
}


/**********************************************************************************************/
inline 
I_PropertyContainer_Ptr Create_Export_PropertiesA(
	const char* 	inFieldDelimiter,
	const char* 	inRecordDelimiter,
	bool 			inHasColumnHeader,
	const char* 	inEncoding )
{
	I_PropertyContainer* pPC = new PropertyContainer;
	
	pPC->Add(new Prop_FieldDelimiter(inFieldDelimiter));
	pPC->Add(new Prop_RecordDelimiter(inRecordDelimiter));
	pPC->Add(new Prop_HasColumnHeader(inHasColumnHeader));
	pPC->Add(new Prop_Encoding_Export(inEncoding));

	return pPC;
}


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_ExportProperties_h
