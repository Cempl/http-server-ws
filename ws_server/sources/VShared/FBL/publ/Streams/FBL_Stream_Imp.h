/**********************************************************************************************/
/* FBL_Stream_Imp.h																			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_Stream_Imp_h 
	#define _FBL_Stream_Imp_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Interfaces/FBL_I_Stream.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_IStream.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_CLASS(Stream_Imp);


/**********************************************************************************************/
// Class that implement many functionality of I_Stream, but still is abstract,
// because some functions will be overriden in the implementation specific classes.
// 
class FBL_SHARED_EXP_CLASS Stream_Imp : virtual public I_Stream
{

    public://///////////////////////////////////////////////////////////////////////////////////

							Stream_Imp( void );
virtual 					~Stream_Imp( void ) {} 


    public://///////////////////////////////////////////////////////////////////////////////////

    // ---------------------
	// Properties:

virtual vint32  	        get_LastError( void ) const override  { return mLastError; }
virtual vuint32    	        get_LastCount( void ) const override  { return mLastCount; }
virtual flength        	    get_ByteSize( void ) const override  { return 0; }
virtual	vint32				get_Handle( void ) const override  { return 0; }


    // ---------------------
    // Status methods:

virtual void                Reset( void ) override { mLastError = 0; }


	protected://////////////////////////////////////////////////////////////////////////////////////
									
							/**	Tries to put exactly inHowMuch bytes to the target location.
							 	Returns the number of actually written bytes. */
virtual vuint32				DoPut(
								const void* 	inBuffer,
								vuint32 		inHowMuch) = 0;

virtual vuint32				DoGet(
								void* 		inBuffer,
								vuint32 	inHowMuch ) = 0;
								  

	protected://////////////////////////////////////////////////////////////////////////////////////

		vint32				mLastError;  	// Last error
		vuint32            	mLastCount;	 	// Last count
};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_Stream_Imp_h


