/**********************************************************************************************/
/* FBL_I_Type.h		                                                     					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_Type_h
	#define _FBL_I_Type_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_IStream.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_OStream.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Serializable.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE(I_Database);
SMART_INTERFACE(I_Type);
SMART_ARRAY_OF_SMART_PTRS( ArrayOfTypes, I_Type_Ptr );

/**********************************************************************************************/
// See
// http://farrago.sourceforge.net/design/UserDefinedTypesAndRoutines.html
// http://developer.postgresql.org/pgdocs/postgres/sql-createtype.html


/**********************************************************************************************/
// Any type belongs to some of this category. 
//
//
enum TYPE_CATEGORY
{
	vcUnknownType = 0,

	vcIntegerType,		// max value:  vint64
	vcUIntegerType,		// max value:  vuint64
	vcRealType,			// max value:  double
	vcStringType,		// max value:  string
	vcDateType, 
	vcTimeType,
	vcDateTimeType,
	vcBinaryType,

	// CREATE TYPE metric_meter AS DOUBLE;
	// ... WHERE metric_meter_field > 10 ...   - ERROR - inconsistent types
	// ... WHERE metric_meter_field > NEW metric_meter(10) ...   - OK
	// ... WHERE CAST(metric_meter_field, DOUBLE) > 10 ...   - OK
	vcScalarType,
	
	// CREATE TYPE Shape2D
	//	NOT INSTANTIABLE
	//	NOT FINAL;
	//
	// CREATE TYPE Rectangle UNDER Shape2D
	//	AS(
	//		width DOUBLE DEFAULT 1,
	//		length DOUBLE DEFAULT 2
	//	)
	//	INSTANTIABLE
	//	FINAL;
	//
	// CREATE TABLE t1(
	//	id INT NOT NULL PRIMARY KEY,
	//	f1 Rectangle CHECK (f1.width > 0 && f1.length > 0) );
	//
	// INSERT INTO t1 VALUES( 1, NEW Rectangle(10,20) ); 
	// SELECT f1.length() FROM t1;
	//
	// CREATE FUNCTION getRectangle( inRecID ULONG ) RETURNS Rectangle
	// BEGIN
	//  DECLARE res Rectangle;
	//	SET res = SELECT f1 FROM t1 WHERE RecID = inRecID;
	//  return res;
	// END;
	//
	vcCompositeType,

	// !!!!!!!!!!!!!
	// Actually it is a single special value EnumeratedValue with constraint (predefined set of values)
	// 1. it may answer with string representation on get_String() and numeric representation
	//     on get_Long(). ('closed' == 3, ...)
	// 2. check constraint (the only enumerated values (or their names) are accepted).
	// !!!!!!!!!!!!!
	//
	// CREATE TYPE bug_status AS ENUM ('unknown', 'new', 'closed', 'open');
	//
	// CREATE TABLE bug (
	//	id INT,
	//	status bug_status
	//	);
	//
	// INSERT INTO bug VALUES( 1, NEW bug_status( 'closed' ) ); -- Explicit bug_status( 'closed' )
	// INSERT INTO bug VALUES( 2, NEW bug_status( 1 ) );		-- Explicit bug_status( 'unknown' )
	// INSERT INTO bug VALUES( 3, 'new' );						-- Implicit bug_status( 'new' )
	// INSERT INTO bug VALUES( 4, 4 );							-- Implicit bug_status( 'open' )
	//
	// INSERT INTO bug VALUES( 5, NEW bug_status( 'updated' ) );	-- Error
	// INSERT INTO bug VALUES( 5, NEW bug_status( 5 ) );			-- Error
	// INSERT INTO bug VALUES( 5, 'updated' );						-- Error
	// INSERT INTO bug VALUES( 5, 5 );								-- Error

	// SELECT status, CAST(status, INT) FROM bug ORDER BY status
	// -- unknown,	1
	// -- new,		2
	// -- closed,	3
	// -- open,		4
	//
	// DECLARE var1 String, var2 long, var3 bug_status;
	// SET var1 = SELECT status FROM bug WHERE RecID = 1;	-- var1 == 'closed'
	// SET var2 = SELECT status FROM bug WHERE RecID = 1;	-- var2 == 3
	// SET var3 = SELECT status FROM bug WHERE RecID = 1;	-- var3 == NEW bug_status(3) == NEW bug_status('closed')
	//
	//
	vcEnumeratedType,

	vcMoneyType,
	
	//
	vcCollectionType
};


/**********************************************************************************************/
// 
//StubInterface
interface FBL_SHARED_EXP_CLASS I_Type : public I_Unknown
{
	 	
virtual 					~I_Type( void );


	// ---------------------
	// Properties:

		// ID

virtual	Type_ID				get_ID( void ) const = 0;

		// Category

virtual TYPE_CATEGORY		get_Category( void ) const = 0;	

		// Database
							/// The type is registered to particular db.
virtual I_Database_Ptr		get_Database( void ) const = 0;

		// IsRemote
							/// Returns TRUE if this custom type object belongs to the remote Database.
virtual bool 				get_IsRemote( void ) const = 0;	

							/// internal. Do not call it directly. 
							///  Used By FBL::CreateCustomType() factory.
virtual void 				put_IsRemote( bool inValue ) = 0;	

		// Length
							/// Returns the defined length.
							/// String(200) - 200
virtual vuint32				get_MaxChars( void ) const = 0; 

		// TypeName
							// Rectangle, double, bug_status....
virtual const String&		get_TypeName( void ) const = 0;

virtual void				put_TypeName( const String& inName ) = 0;

		// Instantiable

virtual bool 				get_IsInstantiable( void ) const = 0;	

virtual void 				put_IsInstantiable( bool inValue ) = 0;	

		// Final

virtual bool 				get_IsFinal( void ) const = 0;	

virtual void 				put_IsFinal( bool inValue ) = 0;	

		// TIMESTAMP
							// Any changes to the type increase its timestamp to get a way
							// to cache type's data on vClient side.
virtual TIMESTAMP			get_TimeStamp( void ) const = 0;


		// Default value
		// Following at least PostgreSQL - it is for NULL-substitution.
		// If the field has no default value we put NULL,  zero or empty-string
		// depends on the field nature. But in case the type has default value we should put
		// that value instead.

virtual const String&		get_DefaultValue( void ) const = 0;

virtual void				put_DefaultValue( const String& inValue ) = 0;



	public://///////////////////////////////////////////////////////////////////////////////////

	// Inheritance

		// Base type
							/// Shape2D for Rectangle
virtual void				RegisterBaseType( const I_Type_Ptr inpType ) = 0;

virtual const I_Type_Ptr	get_BaseType( void ) const = 0;

		// Derived types

							/// Rectangle, Circle ... for Shape2D
virtual void				RegisterDerivedType( const I_Type_Ptr inpType ) = 0;

virtual void				UnRegisterDerivedType( const I_Type_Ptr inpType ) = 0;

virtual vuint16 			get_DerivedTypeCount( void ) const = 0;

virtual I_Type_Ptr			get_DerivedType( vuint16 inIndex )  const = 0;

}; 


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_I_Type
