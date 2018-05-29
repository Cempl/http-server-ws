/**********************************************************************************************/
/* FBL_ForeignPlugin_Imp.cpp                                                     			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Util_classes/FBL_ForeignPlugin_Imp.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
ForeignPlugin_imp::ForeignPlugin_imp( void )
{
	mBackRef 	= nullptr;
	mXtraData	= nullptr;
	mpFinalize	= nullptr;
}


/**********************************************************************************************/
ForeignPlugin_imp::~ForeignPlugin_imp( void )
{
	// We clear back ref and call Finalize, so plugin object
	// can kill self but will not kill me.
	if( mBackRef )
	{
		*mBackRef = nullptr;
		mBackRef = nullptr;
	}
	
	if( mpFinalize )
	{
		try
		{
			(*mpFinalize)( this );
		}
		SWALLOW_CATCH
	}
	
	// Forget plugin object:
	mXtraData = nullptr;
}


/**********************************************************************************************/
FBL_End_Namespace
