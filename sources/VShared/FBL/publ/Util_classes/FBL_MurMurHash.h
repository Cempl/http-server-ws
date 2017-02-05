/**********************************************************************************************/
/* FBL_MurMurHash.h																			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_MurMurHash_h
	#define _FBL_MurMurHash_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
/// Helper class, to build MurMur hash.
// Create an instance, Add data (can be number times)
// and finally call Calculate() to get a result;
//
class FBL_SHARED_EXP_CLASS MurMurHash
{
	public://///////////////////////////////////////////////////////////////////////////////////
 
 							MurMurHash();

virtual						~MurMurHash()
								{}
 
 	public://///////////////////////////////////////////////////////////////////////////////////
 
		void				Add( const unsigned char* inpData, int inLen );
		
		unsigned int		Calculate( void );


	protected://////////////////////////////////////////////////////////////////////////////////

		void				Init( void );

		void				MixTail( const unsigned char **inppData, int* iopLen );
		
 
	protected://////////////////////////////////////////////////////////////////////////////////
 
		unsigned int		mHash;
		unsigned int		mTail;
		unsigned int		mCount;
		unsigned int		mSize;
};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_MurMurHash_h
