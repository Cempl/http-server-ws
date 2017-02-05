/**********************************************************************************************/
/* FBL_FieldProperties.h                                                   					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/
// This file contains classes-properties. 
// This need only for comfort use on properties declaration.
//

#ifndef _FBL_FieldProperties_h
	#define _FBL_FieldProperties_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Properties/FBL_Property.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Table.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Type.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Type_Enumerated.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// forward declarations: 
//
SMART_INTERFACE( I_ENode );
SMART_INTERFACE( I_Field );
SMART_INTERFACE( I_Table );
SMART_ARRAY_OF_SMART_PTRS( ArrayOfFields, I_Field_Ptr );


/**********************************************************************************************/
#pragma mark class Prop_MaxLen
//
class FBL_SHARED_EXP_CLASS Prop_MaxLen : public PropertyLong
{
	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// Construction/Destruction
	 	
							Prop_MaxLen( vuint32 inMaxLen );
							
virtual						~Prop_MaxLen( void );

};


#pragma mark -

/**********************************************************************************************/
#pragma mark class Prop_SegmentSize
//
class FBL_SHARED_EXP_CLASS Prop_SegmentSize : public PropertyLong
{
	public://///////////////////////////////////////////////////////////////////////////////////
	 	
							Prop_SegmentSize( vuint32 inSegmentSize );

virtual 					~Prop_SegmentSize( void );	
};


#pragma mark -

/**********************************************************************************************/
#pragma mark class Prop_Precision
//
class FBL_SHARED_EXP_CLASS Prop_Precision : public PropertyLong
{
	public://///////////////////////////////////////////////////////////////////////////////////
	 	
							Prop_Precision( vuint16 inPrecision );

virtual 					~Prop_Precision( void );	
};


/**********************************************************************************************/
#pragma mark class Prop_Scale
//
class FBL_SHARED_EXP_CLASS Prop_Scale : public PropertyLong
{
	public://///////////////////////////////////////////////////////////////////////////////////
	 	
							Prop_Scale( vuint16 inScale );

virtual 					~Prop_Scale( void );	
};


#pragma mark -

/**********************************************************************************************/
#pragma mark class Prop_Method
//
// Specification of property that keep Text of BaseObject Method (calcualted field).
// Also this property have additional value Lagnauge that of that text.
// to obtain this property it needs do direct cast to this class.
// 
class FBL_SHARED_EXP_CLASS Prop_Method : public PropertyString
{
	public://///////////////////////////////////////////////////////////////////////////////////
 
 							Prop_Method( 
 								const UChar* inMethodText,
 								TMETHOD_LANGUAGE inLanguage );
 							
 							Prop_Method( 
 								const String&	inMethodText,
 								TMETHOD_LANGUAGE inLanguage );

virtual						~Prop_Method( void );

virtual PROPERTY_TYPE		get_Type( void ) const override 
								{ return TYPE_METHOD; }


	public://///////////////////////////////////////////////////////////////////////////////////
 							
 		TMETHOD_LANGUAGE	get_Languge( void ) const 
 								{ return mLanguage; }
 							
 							
	private:////////////////////////////////////////////////////////////////////////////////////
 
 		TMETHOD_LANGUAGE	mLanguage;
};


#pragma mark -


/**********************************************************************************************/
#pragma mark class Prop_MethodSql
//
// The next step of specialisation -- for SQL expressions methods.
// Constructor now have just one parameter.
//
class FBL_SHARED_EXP_CLASS Prop_MethodSql : public Prop_Method
{
	public://///////////////////////////////////////////////////////////////////////////////////
 
 							Prop_MethodSql( 
 								const UChar* inMethodText );
								
							Prop_MethodSql( 
 								const String& inMethodText );

virtual						~Prop_MethodSql( void );
};


#pragma mark -


/**********************************************************************************************/
#pragma mark class Prop_Target
//
// Property for VObjectPtr field.
// It's REQUIRED property for create field of this type.
//
class FBL_SHARED_EXP_CLASS Prop_Target : public PropertyIUnknown
{
	public://///////////////////////////////////////////////////////////////////////////////////
 
 							Prop_Target( 
 								I_Table_Ptr inTarget );
								
virtual 					~Prop_Target( void );			
};


#pragma mark -


/**********************************************************************************************/
#pragma mark class Prop_Encoding
//
class FBL_SHARED_EXP_CLASS Prop_Encoding : public PropertyString
{
	public://///////////////////////////////////////////////////////////////////////////////////
 
 							Prop_Encoding( 
 								const UChar* inEncoding );

virtual 					~Prop_Encoding( void );			
};


#pragma mark -


/**********************************************************************************************/
#pragma mark class Prop_LocaleName
//
class FBL_SHARED_EXP_CLASS Prop_LocaleName : public PropertyString
{
	public://///////////////////////////////////////////////////////////////////////////////////
 
 							Prop_LocaleName( 
 								const UChar* inLocale );
 							
 							Prop_LocaleName( 
 								const String& inLocale );

virtual 					~Prop_LocaleName( void );			
};


#pragma mark -
#pragma mark === Link On Values ===


/**********************************************************************************************/
// Helper properties to create link on values
/**********************************************************************************************/
#pragma mark class Prop_Key
//
class FBL_SHARED_EXP_CLASS Prop_Key : public PropertyIUnknown
{
	public://///////////////////////////////////////////////////////////////////////////////////

							Prop_Key( I_Field_Ptr inKey );
virtual 					~Prop_Key( void );

};


#pragma mark -


/**********************************************************************************************/
#pragma mark class Prop_Ptr
//
class FBL_SHARED_EXP_CLASS Prop_Ptr : public PropertyIUnknown
{
	public://///////////////////////////////////////////////////////////////////////////////////

							Prop_Ptr( I_Field_Ptr inPtr );
virtual 					~Prop_Ptr( void );

};


#pragma mark -


/**********************************************************************************************/
#pragma mark class Prop_OnDeletion
//
class FBL_SHARED_EXP_CLASS Prop_OnDeletion : public PropertyLong
{
	public://///////////////////////////////////////////////////////////////////////////////////

							Prop_OnDeletion( EOnDeletion inOnDeletion );
virtual 					~Prop_OnDeletion( void );

};


#pragma mark -


/**********************************************************************************************/
#pragma mark class Prop_OnUpdate
//
class FBL_SHARED_EXP_CLASS Prop_OnUpdate : public PropertyLong
{
	public://///////////////////////////////////////////////////////////////////////////////////

							Prop_OnUpdate( EOnUpdate inOnUpdate );
virtual 					~Prop_OnUpdate( void );
};


/**********************************************************************************************/
I_PropertyContainer_Ptr Create_Link_On_Value_Properties(
	I_Field_Ptr		inKey,
	I_Field_Ptr		inPtr,
	EOnDeletion		inOnDeletion = kOnDelete_SetNull,
	EOnUpdate		inOnUpdate = kOnUpdate_SetNull );


#pragma mark -
#pragma mark === RDB Link ===


/**********************************************************************************************/
// Helper properties to create link on values
/**********************************************************************************************/
#pragma mark class Prop_RDB_Keys
//
class FBL_SHARED_EXP_CLASS Prop_RDB_Keys : public PropertyIUnknown
{
	public://///////////////////////////////////////////////////////////////////////////////////

							Prop_RDB_Keys( ArrayOfFields_Ptr inKeys );
virtual 					~Prop_RDB_Keys( void );

};


#pragma mark -


/**********************************************************************************************/
#pragma mark class Prop_RDB_Ptrs
//
class FBL_SHARED_EXP_CLASS Prop_RDB_Ptrs : public PropertyIUnknown
{
	public://///////////////////////////////////////////////////////////////////////////////////

							Prop_RDB_Ptrs( ArrayOfFields_Ptr inPtrs );
virtual 					~Prop_RDB_Ptrs( void );
};


#pragma mark -


/**********************************************************************************************/
I_PropertyContainer_Ptr Create_Link_On_Values_N_Properties(
	ArrayOfFields_Ptr	inKeys,
	ArrayOfFields_Ptr	inPtrs,
	EOnDeletion			inOnDeletion = kOnDelete_SetNull,
	EOnUpdate			inOnUpdate   = kOnUpdate_SetNull );


#pragma mark -
#pragma mark === Constraints ===


/**********************************************************************************************/
#pragma mark class Prop_IndexType
//
class FBL_SHARED_EXP_CLASS Prop_IndexType : public PropertyLong
{
	public://///////////////////////////////////////////////////////////////////////////////////

							Prop_IndexType( EIndexType inIndexType );
virtual 					~Prop_IndexType( void );

};


/**********************************************************************************************/
// Helper properties to create PrimaryKey
/**********************************************************************************************/
#pragma mark class Prop_PrimaryKey
//
class FBL_SHARED_EXP_CLASS Prop_PrimaryKey : public PropertyIUnknown
{
	public://///////////////////////////////////////////////////////////////////////////////////

							/**	For create PRIMARY KEY on single field. */
							Prop_PrimaryKey( I_Field_Ptr		inField );

							/**	For create PRIMARY KEY on multiple fields. */
							Prop_PrimaryKey( ArrayOfFields_Ptr	inFieldArray );


	protected://////////////////////////////////////////////////////////////////////////////////

virtual 					~Prop_PrimaryKey( void );
};


#pragma mark -


/**********************************************************************************************/
#pragma mark class Prop_Unique
//
class FBL_SHARED_EXP_CLASS Prop_Unique : public PropertyIUnknown
{
	public://///////////////////////////////////////////////////////////////////////////////////

							/**	For create UNIQUE constraint on single field. */
							Prop_Unique( I_Field_Ptr		inField );

							/**	For create UNIQUE constraint on multiple fields.*/
							Prop_Unique( ArrayOfFields_Ptr	inFieldArray );


	protected://////////////////////////////////////////////////////////////////////////////////

virtual 					~Prop_Unique( void );
};


#pragma mark -


/**********************************************************************************************/
#pragma mark class Prop_Indexed
//
class FBL_SHARED_EXP_CLASS Prop_Indexed : public PropertyIUnknown
{
	public://///////////////////////////////////////////////////////////////////////////////////

							/**	For INDEX on single field. */
							Prop_Indexed( I_Field_Ptr		inField );

							/**	For INDEX on multiple fields. */
							Prop_Indexed( ArrayOfFields_Ptr	inFieldArray );


	protected://////////////////////////////////////////////////////////////////////////////////

virtual 					~Prop_Indexed( void );
};


#pragma mark -


/**********************************************************************************************/
// Foreign key properties.
FBL_SHARED_EXP I_PropertyContainer_Ptr Create_ForeignKey_Properties(
	ArrayOfFields_Ptr	inKeys,
	ArrayOfFields_Ptr	inPtrs,
	EOnDeletion			inOnDeletion = kOnDelete_SetNull,
	EOnUpdate			inOnUpdate = kOnUpdate_SetNull );


/**********************************************************************************************/
FBL_SHARED_EXP I_PropertyContainer_Ptr Create_ForeignKey_Properties(
	I_Field_Ptr		inKey,
	I_Field_Ptr		inPtr,
	EOnDeletion		inOnDeletion = kOnDelete_SetNull,
	EOnUpdate		inOnUpdate = kOnUpdate_SetNull );


#pragma mark -


/**********************************************************************************************/
FBL_SHARED_EXP I_PropertyContainer_Ptr Create_Binary_Link_Properties(
	I_Table_Ptr		inLeftTable,
	I_Table_Ptr		inRightTable,
	LinkType		inLeftType = 1,
	LinkType		inRightType = 0,
	EOnDeletion		inOnDelete = kOnDelete_Cascade,
	EStorageType	inStorageType = kStorage_Default,
	ELinkStorageKind inStorageKind = kLnkPermanent );


#pragma mark -
#pragma mark class Prop_LeftTable, Prop_RightTable


/**********************************************************************************************/
//	Properties for create Binary_Link
//
class Prop_LeftTable : public PropertyIUnknown
{
	public://///////////////////////////////////////////////////////////////////////////////////

							Prop_LeftTable( I_Table_Ptr inTable );


	protected://////////////////////////////////////////////////////////////////////////////////

virtual 					~Prop_LeftTable( void );
};


/**********************************************************************************************/
class Prop_RightTable : public PropertyIUnknown
{
	public://///////////////////////////////////////////////////////////////////////////////////

							Prop_RightTable( I_Table_Ptr inTable );


	protected://////////////////////////////////////////////////////////////////////////////////

virtual 					~Prop_RightTable( void );
};


/**********************************************************************************************/
#pragma mark class Prop_IsSingleByte
//
class Prop_IsSingleByte : public PropertyBoolean
{
	public://///////////////////////////////////////////////////////////////////////////////////

							Prop_IsSingleByte( bool inValue );

virtual 					~Prop_IsSingleByte( void );			
};


/**********************************************************************************************/
class FBL_SHARED_EXP_CLASS Prop_LinkName : public PropertyString
{
	public://///////////////////////////////////////////////////////////////////////////////////
 
 							Prop_LinkName( const UChar*		inLinkName ); 							
 							Prop_LinkName( const String&	inLinkName );

virtual						~Prop_LinkName( void );

};


/**********************************************************************************************/
#pragma mark class Prop_EnumType
//
// Property for VEnum custom-typed fields.
// It's REQUIRED property for create field of this type.
//
class FBL_SHARED_EXP_CLASS Prop_EnumType : public PropertyIUnknown
{
	public://///////////////////////////////////////////////////////////////////////////////////

							Prop_EnumType( I_Type_Enumerated_Ptr inpType );

virtual 					~Prop_EnumType( void );			

};


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // _FBL_FieldProperties_h
