/**********************************************************************************************/
/* FBL_CreateLocalizable.h                                                 					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_Localizable.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
namespace internal
{
	// ---------------------
	// implementation of pointers:
	
GetLocalizableSys_PTR		pGetLocalizableSys;

CreateConverter_void_PTR	pCreateConverter_void;
CreateConverter_a_PTR		pCreateConverter_a;

CreateLocale_PTR			pCreateLocale;
	
} // namespace internal


#pragma mark -
// factories as wrappers around function pointers


/**********************************************************************************************/
I_Localizable_Ptr GetLocalizableSys( void )
{
	return internal::pGetLocalizableSys();
}


/**********************************************************************************************/
I_Converter* CreateConverter( void )
{
	return internal::pCreateConverter_void();
}


/**********************************************************************************************/
I_Converter* CreateConverter( const char* inEncoding )
{
	return internal::pCreateConverter_a( inEncoding );
}


/**********************************************************************************************/
I_Locale* CreateLocale( const char* inEncoding )
{
	return internal::pCreateLocale( inEncoding );
}


/**********************************************************************************************/
FBL_End_Namespace
