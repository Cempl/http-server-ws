/**********************************************************************************************/
/* FBL_StValueChanger.h																		  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_StValueChanger_h 
	#define _FBL_StValueChanger_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>

// FINAL:	
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// Stack based template to provide exception safe change of value for some varible. 
// 
template <class T> class StValueChanger
{
	public:

		StValueChanger( T& inVariable, T inNewValue ) :
			mVariable(inVariable)
		{
			mpPrevValue	= mVariable;
			mVariable	= inNewValue;
		}

		~StValueChanger( void ) {
			mVariable = mpPrevValue;
		}

	protected:

		T&	mVariable;	
		T	mpPrevValue;


	private:
        
        StValueChanger( const StValueChanger& inOther );
        void operator=( const StValueChanger& inOther );
};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_StValueChanger_h

