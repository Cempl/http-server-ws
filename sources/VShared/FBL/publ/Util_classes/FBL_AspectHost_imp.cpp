/**********************************************************************************************/
/* FBL_AspectHost_imp.cpp                 		                               				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Util_classes/FBL_AspectHost_imp.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
AspectHost_imp::AspectHost_imp( 
	vuint16 inMaxAspCount )
{
	mKnownAspectCount = inMaxAspCount;
	
	mAspects = new I_Aspect_Ptr [inMaxAspCount];
	
	memset( mAspects, 0, inMaxAspCount * sizeof(I_Aspect_Ptr) );
}


/**********************************************************************************************/
AspectHost_imp::~AspectHost_imp( void )
{
	for( vuint16 i = 0; i < mKnownAspectCount; ++i )
		mAspects[i] = nullptr;

	if( mAspects )
	{
		delete [] mAspects;
	}
}


/**********************************************************************************************/
void AspectHost_imp::AddAspect( I_Aspect_Ptr inAspect ) 
{
	I_Aspect::ID id = inAspect->get_ID();
	
	if( id >= mKnownAspectCount ) 
		throw xAspectError( ERR_ASPECT_BAD_ID );
		
	if( mAspects[id] != nullptr )
		throw xAspectError( ERR_ASPECT_ALREADY_EXIST );	
		
	mAspects[id] = inAspect;
}


/**********************************************************************************************/
void AspectHost_imp::RemoveAspect( 
	I_Aspect::ID inAspectID ) 
{
	mAspects[inAspectID] = nullptr;
}


/**********************************************************************************************/
void AspectHost_imp::SendMessageToAll( 
	vint32 		inMsg, 
	void* 		inParam )
{
	for( vuint32 i = 0; i < mKnownAspectCount ; ++i )
	{
		I_Aspect_Ptr p = mAspects[i];
		if( p )
		{
			p->OnMessage( inMsg, inParam );
		}
	}
}


/**********************************************************************************************/
FBL_End_Namespace
