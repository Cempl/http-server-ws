/**********************************************************************************************/
/* FBL_I_PropertyContainer.h	                                               				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_PropertyContainer_h
	#define _FBL_I_PropertyContainer_h

#include <VShared/FBL/publ/Interfaces/FBL_I_Property.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE(I_PropertyContainer);


/**********************************************************************************************/
// This class specifies the API of a PropertyContainer object.
// 
interface FBL_SHARED_EXP_CLASS I_PropertyContainer : public I_Unknown
{
virtual						~I_PropertyContainer( void );

	// ---------------------
	// PropertyContainer:
		
		// <Count> [r/o]
							/** Returns the count of PropertyContainer */
virtual vuint32 			get_Count( void ) const  = 0;

		// <Property> [r/o]
							/** Returns a property by index */
virtual I_Property_Ptr		get_Property( vuint32 inIndex ) const  = 0;	

		// <Property> [r/o]
							/** Returns a property by name. Case insensetive. */
virtual I_Property_Ptr		get_Property( const UChar* inName  ) const  = 0;
virtual I_Property_Ptr		get_Property( const String& inName ) const  = 0;

	
	// ---------------------
	// Methods to work with PropertyContainer:		

							/** Adds a new property to the PropertyContainer set.
								If exists property with the same name then it is replaced. */
virtual void 				Add( I_Property_Ptr inProperty ) = 0;

							/** Deletes a property from the PropertyContainer set. */
virtual void 				Remove( const I_Property_Ptr inProperty )  = 0;

							/** Deletes a property from the PropertyContainer set. */
virtual void 				Remove( const UChar* inName )  = 0;

							/** Deletes a property from the PropertyContainer set. */
virtual void 				Remove( const String& inName )  = 0;

							/** Deletes all properties from the PropertyContainer set. */
virtual void 				Clear( void )  = 0;

}; 


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_I_PropertyContainer_h
