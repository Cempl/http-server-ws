/**********************************************************************************************/
/* FBL_SetIterator.h																		  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                       					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_SetIterator_h
   #define _FBL_SetIterator_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Util_classes/FBL_ForeignPlugin_Imp.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_CLASS( SetIterator );


/**********************************************************************************************/
/*
	This is abstract class (Interface) 
	that define API for Set iterator.
	
	ID_TYPE -- is just vuint32 type.
*/
class FBL_SHARED_EXP_CLASS SetIterator : public ForeignPlugin_imp
{
		IMPLEMENT_UNKNOWN(SetIterator)											
		BEGIN_INTERFACE_TABLE(SetIterator)
			IMPLEMENT_INTERFACE(I_ForeignPlugin)
		END_INTERFACE_TABLE()

	public://///////////////////////////////////////////////////////////////////////////////////

							SetIterator( void );
virtual						~SetIterator( void );

	// ---------------------	
	// Low - level methods:

							/// Retuns item value, i.e. RecID (1..N).	
virtual ID_TYPE	 			operator* ( void ) const = 0;

							/// Move iterator to the first/last item of the Set.
							/// Returns false only if set is empty.
virtual bool				First( void ) 	= 0;
virtual bool				Last( void )	= 0;
							
							/// Move iterator to the next/prev item of the Set.
							/// Returns false if we are at last/first item.						
virtual bool				Next( void ) 	= 0;
virtual bool				Prev( void )	= 0;

							/// Set iterator on the specified item of set.
							/// Returns true if such item found.
							/// @param inIndex is 1 - based.
virtual bool				GoTo( vuint32 inIndex ) = 0;


	// ---------------------
	// High level methods:

		/* These methods are combination of e.g. 	
		 			iter.Next(); 
		 			*iter;
		 	so we reduce in 2 times the number of calls of virtual functions. */
	
							/** Move iterator to the first/last item of the Set.
							 	Returns value of item if it found, else return 0.
								Zero can be considered as "not found". */
virtual ID_TYPE				FirstItem( void ) 	= 0;
virtual ID_TYPE				LastItem( void )	= 0;
							
							/** Move iterator to the next/prev item of the Set.
								Returns value of item if it found, else return 0.
								Zero can be considered as "not found". */
virtual ID_TYPE				NextItem( void ) 	= 0;
virtual ID_TYPE				PrevItem( void )	= 0;

};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_SetIterator_h
