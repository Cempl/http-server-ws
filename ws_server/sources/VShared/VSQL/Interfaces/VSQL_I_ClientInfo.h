/**********************************************************************************************/
/* VSQL_I_ClientInfo.h																		  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef __VSQL_I_ClientInfo_h
	#define __VSQL_I_ClientInfo_h
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
SMART_INTERFACE(I_ClientInfo);


/**********************************************************************************************/
//StubInterface
interface FBL_SHARED_EXP_CLASS I_ClientInfo : public I_Unknown
{
virtual						~I_ClientInfo();
	
	// ---------------------
	// Properties:

virtual	const String&		get_ConnectionType(void) const = 0;

virtual	const String&		get_Address( void ) const  = 0;

virtual	const String&		get_Login( void ) const  = 0;

virtual	vuint16				get_Port( void ) const  = 0;

virtual	RemoteID			get_ConnectionID( void ) const  = 0;

virtual	vint32				get_CursorCount( void ) const  = 0;

virtual	void				Refresh( void ) = 0;

							// Return last activity time of the client in msec.
virtual vuint64				get_LastActivityTime( void ) = 0;

							// Return login time of the client in msec.
virtual vuint64				get_LoginTime( void ) = 0;
};


/**********************************************************************************************/
VSQL_End_Namespace


/**********************************************************************************************/
#endif //__VSQL_I_ClientInfo_h








