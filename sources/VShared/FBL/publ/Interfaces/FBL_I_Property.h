/**********************************************************************************************/
/* FBL_I_Property.h	                                                      					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_Property_h
	#define _FBL_I_Property_h

#include <VShared/FBL/publ/Headers/FBL.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// property types: 
//
enum PROPERTY_TYPE
{
	TYPE_BOOLEAN,
	TYPE_LONG,
	TYPE_STRING,
	TYPE_COLLECTION,
	TYPE_ENUM,
	TYPE_I_UNKNOWN,

	TYPE_METHOD
};


/**********************************************************************************************/
SMART_INTERFACE(I_Property);
SMART_INTERFACE(I_PropertyContainer);


/**********************************************************************************************/
SMART_ARRAY_OF_SMART_PTRS( ArrayOfProperties, I_Property_Ptr );


/**********************************************************************************************/
// Public factories to create I_Property implementations.
// @Param inType -- defines what type of property must be created.
// @param inName -- defines name of property.
//
FBL_SHARED_EXP	I_Property* 	CreateProperty( PROPERTY_TYPE inType, const UChar* inName );
FBL_SHARED_EXP	I_Property* 	CreateProperty( PROPERTY_TYPE inType, const String& inName );


/**********************************************************************************************/
// Interface class -- defines I_Property API.
// 
interface FBL_SHARED_EXP_CLASS I_Property : public I_Unknown
{
virtual						~I_Property( void );

							/** Virtual constructor. */
virtual	I_Property_Ptr		Clone( void ) const  = 0;


	// ---------------------
	// Properties:

		// <Type> [r/o]
							/** Returns the type of the property. */
virtual PROPERTY_TYPE		get_Type( void ) const  = 0;	

		// <Name> [r/o]
							/** Returns the name of the property. */
virtual const String& 		get_Name( void ) const  = 0;	 //OLDKEEPAS: mName

		
	// ---------------------
	// Value Properties:

		// <Boolean>
							/** Returns the bool value. */
virtual bool 				get_Boolean( void ) const = 0;	

							/** Sets the bool value. */
virtual void 				put_Boolean( bool inValue )= 0;	

		// <Long>
		
							/** Returns the vint32 value for vint32 Type property or key for enum. */
virtual vint32 				get_Long( void ) const = 0;	

							/** Sets the vint32 value for vint32 Type property or key for enum. */
virtual void 				put_Long( vint32 inValue ) = 0;	

		// <String>
							/** Returns the string value or corresponding string for enum type. */
virtual const String&		get_String( void ) const = 0; //OLDKEEPAS: mStringValue	

							/** Sets the string value or corresponding string for enum type. */
virtual void				put_String( const UChar* inValue ) = 0;	
virtual void				put_String( const String& inValue ) = 0;	

		// <I_Unknown>
							/** Returns the smart pointer to I_Unknown interface FBL_SHARED_EXP_CLASS. */
virtual I_Unknown_Ptr		get_I_Unknown( void ) const = 0;	

							/** Sets the the smart pointer to I_Unknown interface FBL_SHARED_EXP_CLASS. */
virtual void				put_I_Unknown( I_Unknown_Ptr inValue ) = 0;	


		// <List>
							/** Returns the values list. */
virtual const I_PropertyContainer_Ptr get_Collection( void ) const = 0;	

							/** Sets the values list. */
virtual void				put_Collection( const I_PropertyContainer_Ptr inValue ) = 0;	

}; 


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_I_Property_h
