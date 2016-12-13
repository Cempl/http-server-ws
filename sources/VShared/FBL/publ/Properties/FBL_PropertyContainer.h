/**********************************************************************************************/
/* FBL_PropertyContainer.h			                                          				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_PropertyContainer_h
	#define _FBL_PropertyContainer_h

#pragma once

#include <VShared/FBL/publ/Interfaces/FBL_I_PropertyContainer.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_CLASS( PropertyContainer );


/**********************************************************************************************/
// class PropertyContainer implements I_PropertyContainer interface.
// 
class FBL_SHARED_EXP_CLASS PropertyContainer : public I_PropertyContainer
{
		IMPLEMENT_UNKNOWN(PropertyContainer)											
		BEGIN_INTERFACE_TABLE(PropertyContainer)										
			IMPLEMENT_INTERFACE(I_PropertyContainer)										
		END_INTERFACE_TABLE()

	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// Construction/Destruction
							
							PropertyContainer( void );


	protected://////////////////////////////////////////////////////////////////////////////////

virtual						~PropertyContainer( void );


	public://///////////////////////////////////////////////////////////////////////////////////

// I_PropertyContainer interface:

	// ---------------------
	// Properties:

virtual	vuint32				get_Count	( void )				const override;
virtual	I_Property_Ptr		get_Property( vuint32 inIndex )		const override;
virtual I_Property_Ptr		get_Property( const UChar* inName ) const override;
virtual I_Property_Ptr		get_Property( const String& inName )  const override;


	// ---------------------
	// Methods:

virtual	void				Add( I_Property_Ptr inProperty ) override;

virtual	void				Remove( const I_Property_Ptr inProperty ) override;
virtual	void				Remove( const UChar* inName ) override;
virtual void 				Remove( const String& inName ) override;

virtual	void				Clear( void ) override;
							

	protected://////////////////////////////////////////////////////////////////////////////////

		ArrayOfProperties_Ptr mItems;		// All Properties
};


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_PropertyContainer_h
