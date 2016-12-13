/**********************************************************************************************/
/* FBL_IndexStyleProperties.h                                              					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/
// This file contains classes-properties. 
// This need only for comfort use on properties declaration.
//

#ifndef _FBL_IndexStyleProperties_h
	#define _FBL_IndexStyleProperties_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Properties/FBL_Property.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Value.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// Supported properties for IndexStyle are:
//
// bywords as bool	
// IgnoreLength as vint32
// IgnoreWords as string
// 
/**********************************************************************************************/
// If this property simply exists in the container then we consider this as TRUE always.
//
class Prop_ByWords : public PropertyBoolean
{	
	public://///////////////////////////////////////////////////////////////////////////////////
 
							Prop_ByWords( void ) : 
								PropertyBoolean( String("bywords"), true )
							{								
							}	
};


/**********************************************************************************************/
// If index style just contains this property then we consider it as ByWords = TRUE.
//
class Prop_IgnoreWords : public PropertyString
{	
	public://///////////////////////////////////////////////////////////////////////////////////
 
							Prop_IgnoreWords( 
								const String& inWords )
							: 
								PropertyString( String("IgnoreWords"), inWords )
							{								
							}
};


/**********************************************************************************************/
class Prop_IgnoreLength : public PropertyLong
{	
	public://///////////////////////////////////////////////////////////////////////////////////
 
							Prop_IgnoreLength( vuint32 inLength ) : 
								PropertyLong( String("IgnoreLength"), (vint32) inLength )
							{								
							}	
};


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // _FBL_IndexStyleProperties_h
