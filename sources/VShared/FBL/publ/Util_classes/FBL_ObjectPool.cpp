/**********************************************************************************************/
/* FBL_ObjectPool.cpp																		  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>
#include "FBL_ObjectPool.h"

/**********************************************************************************************/
FBL_Begin_Namespace

/**********************************************************************************************/
#if FBL_INTERNAL_THREADSAFE || FBL_EXTERNAL_THREADSAFE
	#if FBL_TEST_CODE
		ArrayOfUllong_Ptr	gThreadIDArray;
		True_Thread_Mutex	gThreadIDArrayMutex;
	#endif //FBL_TEST_CODE
#endif // FBL_INTERNAL_THREADSAFE || FBL_EXTERNAL_THREADSAFE		


/**********************************************************************************************/
FBL_End_Namespace

