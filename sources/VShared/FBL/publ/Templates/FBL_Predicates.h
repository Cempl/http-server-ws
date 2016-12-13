/**********************************************************************************************/
/* FBL_Predicates.h																			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_Predicates_h 
	#define _FBL_Predicates_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Util_classes/FBL_String.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// This predicate allow compare e.g. names of fields, tables, ... using STL algorithms.
// elem of type T must have get_Name() method.
//
template <class T>
class Name_eq
{
		const String name;

	public://///////////////////////////////////////////////////////////////////////////////////
 
		explicit Name_eq( const UChar* inName ) : name( inName ) {}
		
		bool operator() (const T& elem )
		{
			//const UChar* s = elem->get_Name();
			const String& s = elem->get_Name();
			return name.caseCompare(s.c_str(), 0) == 0;
		}	
};


/**********************************************************************************************/
// This predicate allow compare e.g. ID values of fields, tables, ... using STL algorithms.
// elem of type T must have get_ID() method.
//
template <class ID_TYPE, class T>
class ID_eq
{
		ID_TYPE mID;

	public://///////////////////////////////////////////////////////////////////////////////////
 
		explicit ID_eq( ID_TYPE inID ) : mID( inID ) {}
		
		bool operator() (const T& elem )
		{ 
			return bool(elem->get_ID() == mID); 
		}	
};


/**********************************************************************************************/
// This predicate set to null property IsNull of fields, values ... using STL algorithm.
// Elem of type T must have put_IsNull() method.
/*template<class T>
class put_IsNull
{
		bool value;

	public://///////////////////////////////////////////////////////////////////////////////////
 
		explicit put_IsNull( bool inValue) : value( inValue ) {}
		
		void operator() (const T& elem )
		{ 
			const_cast<T&>( elem )->put_IsNull(value); 
		}	
};*/

/**********************************************************************************************/
FBL_End_Namespace

#endif
