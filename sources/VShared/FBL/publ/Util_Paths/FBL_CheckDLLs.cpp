/**********************************************************************************************/
/* FBL_CheckDLLs.cpp                                                     					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>


#include <VShared/FBL/publ/File_OS/FBL_DynamicLibrary.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
const char* GetKernelDllName( void );
const char* GetClientDllName( void );


/**********************************************************************************************/
bool IsKernelDLLAvailable( bool inTryAgain )
{
	static bool firstTimeCalled = true;
	static bool available = false;

	if( firstTimeCalled || (!available && inTryAgain) )
	{
		firstTimeCalled = false;
	
		#if FBL_WIN
			DynamicLibrary dll;
			available = dll.Load(GetKernelDllName());
		#else
			return available = true;
		#endif // FBL_WIN
	}

	return available;
}


/**********************************************************************************************/
bool IsClientDLLAvailable( bool inTryAgain )
{
	static bool firstTimeCalled = true;
	static bool available = false;

	if( firstTimeCalled || (!available && inTryAgain) )
	{
		firstTimeCalled = false;
		
		#if FBL_WIN
			DynamicLibrary dll;
			available = dll.Load(GetClientDllName());
		#else
			return true;
		#endif // FBL_WIN
	}

	return available;
}


/**********************************************************************************************/
FBL_End_Namespace
