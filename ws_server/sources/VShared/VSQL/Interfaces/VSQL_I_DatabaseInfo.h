/**********************************************************************************************/
/* VSQL_I_DatabaseInfo.h																	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _VSQL_I_DatabaseInfo_h 
	#define _VSQL_I_DatabaseInfo_h
#pragma once

// VSQL:
#include <VShared/VSQL/Headers/VSQL.h>

// FBL:
#include <VShared/FBL/publ/Util_classes/FBL_String.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
VSQL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE(I_DatabaseInfo);
SMART_INTERFACE(I_ClientInfo);


/**********************************************************************************************/
//StubInterface
interface FBL_SHARED_EXP_CLASS I_DatabaseInfo : public I_Unknown
{

virtual						~I_DatabaseInfo();

	// ---------------------
	// Properties:

virtual const String&		get_Name( void ) const  = 0;

virtual const String&		get_Path( void ) const  = 0;

virtual vuint16				get_ClientCount( void ) const  = 0;

virtual vint32				get_CursorCount( void ) const  = 0;

virtual I_ClientInfo_Ptr	get_ClientInfo( vuint16 inIndex ) = 0;

virtual void				Refresh( void ) = 0;
};


/**********************************************************************************************/
VSQL_End_Namespace


/**********************************************************************************************/
#endif //_VSQL_I_DatabaseInfo_h








