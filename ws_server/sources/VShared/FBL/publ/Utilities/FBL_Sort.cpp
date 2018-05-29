/**********************************************************************************************/
/* FBL_Sort.cpp						                                                          */
/*                                                                                            */
/* Copyright Paradigma Software, 1993-2011                                                    */
/* All Rights Reserved.                                                                       */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Utilities/FBL_Sort.h>

#include <VShared/FBL/publ/Templates/FBL_QuickSort_Threaded_T.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
/*void sort(
	vuint32*	inpStart,		
	vuint32*	inpFinish )
{
	FBL_CHECK( inpStart && inpFinish && (inpStart <= inpFinish) );
	
	vuint32 Count = inpFinish - inpStart;
	
	if( Count > SORT_THREADED_MIN_COUNT )
	{
		qsort_threaded<>( inpStart, inpFinish );
	}
	else
	{
		std::sort( inpStart, inpFinish );
	}
}*/


/**********************************************************************************************/
FBL_End_Namespace
