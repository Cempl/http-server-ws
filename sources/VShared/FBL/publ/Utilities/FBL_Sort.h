/**********************************************************************************************/
/* FBL_Sort.h																			 	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef __FBL_Sort_h 
	#define __FBL_Sort_h

#include <VShared/FBL/publ/Headers/FBL.h>

#include <VShared/FBL/publ/Templates/FBL_QuickSort_Threaded_T.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>

// STD:
#include <cstddef>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// This is the number of threads to be used for threaded SORTING.
// For now we specify here gold middle for 2-8 CPU cores.
//
// TODO: in ideal this should be asked from the SYSTEM as CPU core number * Some K
// Also you must change MAX_THREAD_RECURSION_LEVEL along with this value until 
// SORT_THREADS_COUNT is just defined here.
//
#define SORT_THREADS_COUNT 16


/**********************************************************************************************/
// RZ 10/08/10 
// I think we should use THREADED sort only if we have many enough items
// to split between N threads. 
//
// Really, let we have 16 threads. It is silly to split 16 items array into  
// 16 threads. But it will be okay if each thread will get at least e.g. 32 items.
//
// Also keep in mind that currently we create and destroy N threads each time.
// Future, we need to use pattern "pool of threads + tasks" like in VServer and ACE.
// Then cost of thread usage will go down.
//
#define SORT_THREADED_MIN_COUNT		128 * SORT_THREADS_COUNT   // = 2048 items


/**********************************************************************************************/
// FBL::sort(). Declaration similar to std::sort().
//
template <typename T>
void sort(
	T*	inpStart,		
	T*	inpFinish )
{
    #if FBL_MT_REINDEX || FBL_EXTERNAL_THREADSAFE || defined( __arm__ )
    
		// 1. At least numeric fields reindex faster a little.
		// 2. Tmp for MT config - in case of multi-threaded sort we get into deadlock because that 
		//	  threads' algorithms use public interface methods inside - so it is "mutexed" access.
		//	  But "initiator" of sorting (Reindex for example) keeps that mutex already.
		//	  To solve it we should carefully remove any "public" calls.
		// 3. Tmp for ARM, because of crash in std::sort, called by qsort_threaded.
		//
		std::sort( inpStart, inpFinish );
	
    #else // FBL_MT_REINDEX
	
       	FBL_CHECK( inpStart && inpFinish && (inpStart <= inpFinish) );

		ptrdiff_t ItemsToSort = inpFinish - inpStart;
		if( ItemsToSort > SORT_THREADED_MIN_COUNT )
		{
			qsort_threaded<T>( inpStart, inpFinish );
		}
		else
		{
			std::sort( inpStart, inpFinish );
		}
    
	#endif //FBL_MT_REINDEX
}


/**********************************************************************************************/
// FBL::sort(). Declaration similar to std::sort().
//
template <typename T, typename C>
void sort(
	T*	inpStart,		
	T*	inpFinish,
	C	inComparator )
{
	#if FBL_MT_REINDEX || FBL_EXTERNAL_THREADSAFE || defined( __arm__ )
	
		// 1. At least numeric fields reindex faster a little.
		// 2. Tmp for MT config - in case of multi-threaded sort we get into deadlock because that 
		//	  threads' algorithms use public interface methods inside - so it is "mutexed" access.
		//	  But "initiator" of sorting (Reindex for example) keeps that mutex already.
		//	  To solve it we should carefully remove any "public" calls.
		// 3. Tmp for ARM, because of crash in std::sort, called by qsort_threaded.
		//	  
		std::sort( inpStart, inpFinish, inComparator );
	
    #else // FBL_MT_REINDEX
    
		FBL_CHECK( inpStart && inpFinish && (inpStart <= inpFinish) );

		ptrdiff_t ItemsToSort = inpFinish - inpStart;		
		if( ItemsToSort > SORT_THREADED_MIN_COUNT )
		{
			qsort_threaded<T,C>( inpStart, inpFinish, inComparator );
		}
		else
		{
			std::sort( inpStart, inpFinish, inComparator );
		}
    
	#endif //FBL_MT_REINDEX		
}


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // __FBL_Sort_h

