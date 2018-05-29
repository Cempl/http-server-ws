/**********************************************************************************************/
/* FBL_ToUChar.cpp                    		                                  				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Util_classes/FBL_ToUChar.h>

#include <VShared/FBL/publ/Values/FBL_type_traits.h>
#include <VShared/FBL/publ/Util_Strings/FBL_UnicodeToFromNumeric.h>


/**********************************************************************************************/
FBL_Begin_Namespace 


/**********************************************************************************************/
ToUCharPtr::ToUCharPtr( vint16 inValue )
{
	u_ltou( (vint32)inValue, mStr);
}


/**********************************************************************************************/
ToUCharPtr::ToUCharPtr( vint32 inValue )
{
	u_ltou(inValue, mStr);
}


/**********************************************************************************************/
ToUCharPtr::ToUCharPtr( vint64 inValue )
{
	u_lltou(inValue, mStr);
}


/**********************************************************************************************/
ToUCharPtr::ToUCharPtr( vuint16 inValue )
{
	u_ultous( (vuint32)inValue, mStr);
}


/**********************************************************************************************/
ToUCharPtr::ToUCharPtr( vuint32 inValue )
{
	u_ultous(inValue, mStr);
}


/**********************************************************************************************/
ToUCharPtr::ToUCharPtr( vuint64 inValue )
{
	u_ulltous(inValue, mStr);
}


#pragma mark -


/**********************************************************************************************/
ToCharPtr::ToCharPtr( vint16 inValue )
{
	type_traits<vint16>::to_str( &inValue, mStr, kLLongStrMaxLen + 1 );
}


/**********************************************************************************************/
ToCharPtr::ToCharPtr( vint32 inValue )
{
	type_traits<vint32>::to_str( &inValue, mStr, kLLongStrMaxLen + 1 );
}


/**********************************************************************************************/
ToCharPtr::ToCharPtr( vint64 inValue )
{
	type_traits<vint64>::to_str( &inValue, mStr, kLLongStrMaxLen + 1 );
}


/**********************************************************************************************/
ToCharPtr::ToCharPtr( vuint16 inValue )
{
	type_traits<vuint16>::to_str( &inValue, mStr, kLLongStrMaxLen + 1 );
}


/**********************************************************************************************/
ToCharPtr::ToCharPtr( vuint32 inValue )
{
	type_traits<vuint32>::to_str( &inValue, mStr, kLLongStrMaxLen + 1 );
}


/**********************************************************************************************/
ToCharPtr::ToCharPtr( vuint64 inValue )
{
	type_traits<vuint64>::to_str( &inValue, mStr, kLLongStrMaxLen + 1 );
}


/**********************************************************************************************/
FBL_End_Namespace
