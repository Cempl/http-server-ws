/**********************************************************************************************/
/* FBL_AspectHost_imp.h 	                                                   				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_AspectHost_imp_h 
	#define _FBL_AspectHost_imp_h
#pragma once

#include <VShared/FBL/publ/Interfaces/FBL_I_AspectHost.h>

#include <VShared/FBL/publ/Errors/FBL_Exceptions.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// Simple Implementation of I_AspectHost that use fixed size aray as storage of aspects.
// This class should be used as parent of some class that want to be AspectHost.
//
// VERY IMPORTANT: 
// 		Assume this host know N aspects. Then these Aspects must have IDs starting from zero,
//		and ocupie range [0..N-1]. 
//		This allow us implement a very effective search of an Aspect in host by its ID.	
//		Search need just one jump in array of aspects.
// 
class FBL_SHARED_EXP_CLASS AspectHost_imp : public I_AspectHost 
{
		// imp -> so no IUNKNOWN

							AspectHost_imp( const AspectHost_imp& inSrc );
		AspectHost_imp&		operator=( const AspectHost_imp& inSrc );
	
	
    public:////////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// Construction:
  
							AspectHost_imp( 
								vuint16 inMaxAspCount );

virtual						~AspectHost_imp( void );


    public:////////////////////////////////////////////////////////////////////////////////////

// I_AspectHost

virtual	vuint16				get_KnownAspectCount( void ) const override  
								{ return mKnownAspectCount; }	

virtual I_Aspect_Ptr		get_Aspect( 
								I_Aspect::ID inAspectID ) const override 
							{
								if( inAspectID >= mKnownAspectCount ) 
									throw xAspectError( ERR_ASPECT_BAD_ID );

								return mAspects[ inAspectID ];
							}

virtual	void				AddAspect( 
								I_Aspect_Ptr inAspect ) override;
								
virtual void				RemoveAspect( 
								I_Aspect::ID inAspectID ) override;

virtual void 				SendMessageToAll( 
								vint32 		inMsg, 
								void* 		inParam = nullptr ) override;


    private:///////////////////////////////////////////////////////////////////////////////////

		I_Aspect_Ptr*		mAspects;		
		vuint16				mKnownAspectCount;
};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_AspectHost_imp_h
