/**********************************************************************************************/
/* FBL_I_Connectable.h 			                                                      		  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef __FBL_I_Connectable_h
	#define __FBL_I_Connectable_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Connection.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE(I_Connectable);


/**********************************************************************************************/
interface FBL_SHARED_EXP_CLASS I_Connectable : public I_Unknown
{
virtual						~I_Connectable( void );

	// ---------------------
	// Methods:

							/** Read remote object's identifier from the input stream. */
virtual void				ReadRemoteID( void ) = 0;

							/** Write remote object's identifier to the output stream. */
virtual void				WriteRemoteID( void ) const = 0;

							/** Retrieves connectable object structure from the stream. */
virtual void				RecvStructure( void ) = 0;

							/** Retrieves connectable object structure from the stream. */
virtual void				SendStructure( void ) = 0;


	// ---------------------
	// Properties:

		// <Connection> [r/w]
virtual I_Connection_Ptr	get_Connection( void ) const  = 0;
virtual void				put_Connection( I_Connection_Ptr inConnection )  = 0;

		// <RemoteID> [r/w]
virtual RemoteID			get_RemoteID( void ) const  = 0;
virtual void				put_RemoteID( RemoteID inRemoteID )  = 0;

virtual bool				get_IsStatic( void ) const  = 0;


}; 


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // __FBL_I_Connectable_h
