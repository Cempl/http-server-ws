/**********************************************************************************************/
/* FBL_I_Aspect.h 		                                                      				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_Aspect_h
	#define _FBL_I_Aspect_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace  
 

/**********************************************************************************************/
SMART_INTERFACE(I_Aspect);
SMART_INTERFACE(I_AspectHost);


/**********************************************************************************************/
// Abstraction of a "feature" that can be attached to a separate object of I_AspectHost at runtime. 
// This abstraction all us to have alternative to inheritance.
// Another its major difference, we change behavior of not the whole class, but only some instances.
// In the same time, instance of AspectHost still is able return a requested interface. 
// 
interface FBL_SHARED_EXP_CLASS I_Aspect   : public I_Unknown
{
virtual						~I_Aspect( void );
	
	// Each Aspect must have unique ID in the scope of Host.
	typedef vuint16 ID;	

	// ---------------------
	// Properties:

		// <ID>
							// Returns ID of this Aspect class.
virtual ID					get_ID( void ) const = 0;						

		// <I_AspectHost> [r/o]
							// Host that keep this aspect.
							// Aspect cannot be assigned to other host.
virtual	I_AspectHost_Ptr	get_Host( void ) const = 0;


	// ---------------------
	// Methods:

							// RZ: question: may be it should be throw() ?
							// because it is called from loop SendMessageToAll(), which must 
							// send message to all aspects.
virtual	vint32				OnMessage( 
								vint32 		inMsg, 
								void* 		inParam = nullptr ) = 0;
};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_I_Aspect_h
