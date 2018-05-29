/**********************************************************************************************/
/* FBL_JSON_Message.cpp 								                    				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Util_classes/FBL_String.h>
#include <VShared/FBL/publ/Util_Strings/FBL_JSON_Message.h>


/**********************************************************************************************/
FBL_Begin_Namespace

#pragma mark -

/**********************************************************************************************/
String CreateMessage( 
	ActionType		inAction, 
	ObjectType		inObjectType, 
	const String&	inObjectName )
{
	String res( "{ \"Action\": " );

	switch( inAction )
	{
		case kCreateActionType:		{ res << "\"kCreateActionType\", ";		} break;
		case kDeleteActionType:		{ res << "\"kDeleteActionType\", ";		} break;
		case kAlterActionType:		{ res << "\"kAlterActionType\", ";		} break;

		default:
			FBL_CHECK(false);
	}

	res << "\"ObjectType\": ";

	switch( inObjectType )
	{
		case kDatabaseObjectType:	{ res << "\"kDatabaseObjectType\", ";	} break;
		case kTableObjectType:		{ res << "\"kTableObjectType\", ";		} break;
		case kViewObjectType:		{ res << "\"kViewObjectType\", ";		} break;
		case kLinkObjectType:		{ res << "\"kLinkObjectType\", ";		} break;
		case kTriggerObjectType:	{ res << "\"kTriggerObjectType\", ";	} break;
		case kConstraintObjectType:	{ res << "\"kConstraintObjectType\", ";	} break;
		case kSPObjectType:			{ res << "\"kSPObjectType\", ";			} break;
		case kFieldObjectType:		{ res << "\"kFieldObjectType\", ";		} break;
		case kKeyValueObjectType:	{ res << "\"kKeyValueObjectType\", ";	} break;

		default:
			FBL_CHECK(false);
	}

	res << "\"ObjectName\": \"";
	String encodedObjectName( inObjectName );
	res << encodedObjectName.addJsonEncoding();
	res << "\" }";
	return res;
}


/**********************************************************************************************/
FBL_End_Namespace
