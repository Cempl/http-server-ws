/**********************************************************************************************/
/* FBL_I_FldString.h	                                                      				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_FldString_h 
	#define _FBL_I_FldString_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Sets/FBL_BitSet.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE(I_FldString);
SMART_INTERFACE(I_Localizable);
SMART_INTERFACE(I_Value);


/**********************************************************************************************/
// Interface Class
//
//StubInterface
interface FBL_SHARED_EXP_CLASS I_FldString : public I_Unknown
{

virtual						~I_FldString( void );


	//----------------------
	// Properties:

 		// <IndexByWords>  [r/w]		
							/// Returns TRUE if index must be by words.	
virtual	bool				get_IndexByWords( void ) const  = 0;

							/** Set ON/OFF the flag fIndexByWords. 
								If index exists, then this operation require rebuilt index.
								If index is absent on disk, then simply changes flag. */
virtual	void				put_IndexByWords( bool inOnOff ) = 0;

		// <IsSingleByte> [r/o]

virtual	bool				get_IsSingleByte( void ) const  = 0;


	// --------------------
	// Search methods:
							/** Returns a result of search in which each resulting value begins 
								with inValue. If inValue is NULL then result will be NULL. */
virtual BitSet_Ptr			FindStartsWith( 
								Const_I_Value_Ptr inValue,
								Const_BitSet_Ptr  inSelection = nullptr, 
								ESearch				inSearchPref = kPreferIndexed ) const = 0;

							/** Returns a result of search in which each resulting value ends 
								with inValue. If inValue is NULL then result will be NULL. */
virtual BitSet_Ptr			FindEndsWith( 
								Const_I_Value_Ptr inValue,
								Const_BitSet_Ptr  inSelection = nullptr, 
								ESearch			  inSearchPref = kPreferIndexed ) const = 0;

							/** Returns a result of search in which each resulting value contains 
								inValue substring. If inValue is NULL then result will be NULL. */
virtual BitSet_Ptr			FindContains( 
								Const_I_Value_Ptr inValue,
								Const_BitSet_Ptr  inSelection = nullptr, 
								ESearch			  inSearchPref = kPreferIndexed ) const = 0;

							/** Split the give string to words and return string of these
								words seprated by SPACE. It returns exactly the same set
								of words as Valentina will add to index. */	
virtual String				SplitToWords( const String& inStr ) = 0;								


}; 
 

/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_I_FldString_h
