/**********************************************************************************************/
/* FBL_I_RemoteProxy.h                                                       				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_RemoteProxy_h
	#define _FBL_I_RemoteProxy_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// This interface can be implemented by the object 
// that can be passed between client-server. It is 
// used now in the implementation of the property classes.
// 
interface FBL_SHARED_EXP_CLASS I_RemoteProxy : public I_Unknown
{

virtual						~I_RemoteProxy();

	// ---------------------
	// Properties:

virtual I_Unknown_Ptr		get_I_Unknown( void ) const = 0;

};


/**********************************************************************************************/
SMART_INTERFACE(I_RemoteProxy);


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_I_RemoteProxy_h
