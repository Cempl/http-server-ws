/**********************************************************************************************/
/* FBL_I_AspectHost.h 	                                                      				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_AspectHost_h
	#define _FBL_I_AspectHost_h
#pragma once

#include <VShared/FBL/publ/Interfaces/FBL_I_Aspect.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace  
 

/**********************************************************************************************/
SMART_INTERFACE(I_AspectHost);


/**********************************************************************************************/
// Class which should have Aspects, must have this interface.
// 
interface FBL_SHARED_EXP_CLASS I_AspectHost : public I_Unknown
{
virtual						~I_AspectHost( void );

	// ---------------------
	// Properties:
		
		// <KnownAspectCount>
							/** Returns the number of aspects known to this host.
								Note, that a host can contain even zero aspects,
								but it still have chance at runtime get more aspects. */
virtual	vuint16				get_KnownAspectCount( void ) const = 0;	

		// <Aspect>
							/** Returns aspect by its ID.
								Errors: errBadAspectID */
virtual I_Aspect_Ptr		get_Aspect( 
								I_Aspect::ID inAspectID ) const = 0;

	// ---------------------
	// Methods:

							/** Add Aspect to host.
								Error: errBadAspectID, errAspectAlreadyExist */
virtual	void				AddAspect( 
								I_Aspect_Ptr inAspect ) = 0;

							/** Host must delete specified aspect.
								If aspect not found, then simply ignore call. */
virtual void				RemoveAspect( 
								I_Aspect::ID inAspectID ) = 0;
		
virtual void 				SendMessageToAll( 
								vint32 		inMsg, 
								void* 		inParam = nullptr ) = 0;
};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_I_AspectHost_h
