/**********************************************************************************************/
/* FBL_CompareOperators.h																	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                       					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef __FBL_CompareOperators_h 
	#define __FBL_CompareOperators_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>
	
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// This template functions allows to apply comparision operators to ANY class that have 
// methods IsEqual() and IsLessThan().


/**********************************************************************************************/
template <class T>
bool operator == (T& a, T& b)
{
	return a.IsEqual( b );
}


/**********************************************************************************************/
template <class T>
bool operator != (T& a, T& b)
{
	return !a.IsEqual( b );
}


/**********************************************************************************************/
template <class T>
bool operator < (T& a, T& b)
{
	return a.IsLessThan( b );
}

/**********************************************************************************************/
template <class T>
bool operator <= (T& a, T& b)
{
	return a.IsLessOrEqual( b );
}


/**********************************************************************************************/
template <class T>
bool operator > (T& a, T& b)
{
	return !a.IsLessOrEqual( b );
}

/**********************************************************************************************/
template <class T>
bool operator >= (T& a, T& b)
{
	return !a.IsLessThan( b );
}


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // __FBL_CompareOperators_h
