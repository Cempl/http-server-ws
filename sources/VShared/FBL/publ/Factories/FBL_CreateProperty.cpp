/**********************************************************************************************/
/* FBL_CreateProperty.cpp 	                                               					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_Property.h>

#include <VShared/FBL/publ/Properties/FBL_Property.h>
#include <VShared/FBL/publ/Properties/FBL_FieldProperties.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
I_Property* CreateProperty( PROPERTY_TYPE inType, const UChar* inName )
{
	String s(inName);
	return CreateProperty(inType, s);
}	


/**********************************************************************************************/
I_Property* CreateProperty( PROPERTY_TYPE inType, const String& inName )
{
	I_Property* pp = nullptr;
	switch( inType )
	{
		case TYPE_BOOLEAN:
			pp = new PropertyBoolean( inName );
			break;

		case TYPE_LONG:
			pp = new PropertyLong( inName );
			break;

		case TYPE_STRING:
			pp = new PropertyString( inName );
			break;

		case TYPE_COLLECTION:
			pp = new PropertyCollection( inName );
			break;

		case TYPE_I_UNKNOWN:
			pp = new PropertyIUnknown( inName );
			break;

		case TYPE_METHOD:
			// Use default language here:
			pp = new Prop_Method( inName, kMethod_SQL );	
			break;
			
		default:
		break;	
	}

	return pp;
}


/**********************************************************************************************/
FBL_End_Namespace












