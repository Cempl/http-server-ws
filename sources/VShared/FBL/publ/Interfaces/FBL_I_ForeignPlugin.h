/**********************************************************************************************/
/* FBL_I_ForeignPlugin.h                                                       				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_ForeignPlugin_h 
	#define _FBL_I_ForeignPlugin_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE( I_ForeignPlugin );


/**********************************************************************************************/
/** 
	This interface allow us to develop plugins, such as Valentina for REALbasic,
	Valentina for Java, ..., which do have garbage collection. 
	
	Problem is that Kernel on some commands can destroy Kernel C++ objects,
	which are pointed by Host IDE of that languages (REALbasic, Java, ...) 	

	Solution is that Kernel Objects that require such link, 
	must be inherited from this interface and they must implement it.
	
	As result, 
	1) each such object will have DIRECT POINTER to IDE object,
		so having Kernel Object, we can easy get pointer to IDE object.
		This allow us easy keep 1 : 1 mapping between kernel and IDE objects.
		Direct pointer to IDE object should be stored in the Xtra property.
		You can store in the Xtra even some structure if you need many data.

	2) Usually each IDE object have pointer to Kernel Object.
		we must ZERO this pointer, so IDE object will not try delete already
		delete kernel object. This must be made from Destructor of ForeignPlugin 
		subclass before Finalise call.

	3) Destructor of ForeignPlugin subclass must call mpFinalize function 
		if it is specified. This give chance to IDE clenup something. 
		Usually reduce counting for IDE object.
		
	Note: BackRef and Finalize usually are alternatives. 
		i.e. usually only one of them is used.
		BackRef -- can do work in most cases.
		Finalize -- can do some more complex work if needed.		
*/
interface FBL_SHARED_EXP_CLASS I_ForeignPlugin : public I_Unknown
{
	typedef void (*FINALAIZE_PTR)(I_ForeignPlugin* inKernelObject );


virtual 					~I_ForeignPlugin( void );

virtual	void*				get_Xtra( void ) const  = 0;										
virtual	void				put_Xtra( void* inXtraData )  = 0;

virtual void**				get_BackRef( void ) const  = 0;
virtual	void				put_BackRef( void** inBackRef )  = 0;
	
virtual void				put_Finalize( FINALAIZE_PTR inFunc )  = 0;	

};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_I_ForeignPlugin_h
