/**********************************************************************************************/
/* FBL_QuickSort_Long_Threaded.h														 	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef __FBL_QuickSort_Long_Threaded_h 
	#define __FBL_QuickSort_Long_Threaded_h

#include <VShared/FBL/publ/Threads/FBL_Synch.h>

#include <algorithm>


// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// SORT_THREADS_COUNT is defined to be 16 now (max count of threads to sort anything).
// Each thread may sort some range or produce another pair of threads (partitioning).
// So we have:
// 		        =  1 threads for level 1
// 		1+2     =  3 threads for level 2
// 		1+2+4   =  7 threads for level 3
// 		1+2+4+8 = 15 threads for level 4
//
const vuint32 MAX_THREAD_RECURSION_LEVEL = 4;



/**********************************************************************************************/
#if FBL_MAC // AND SDK >= 10.7  ... how to check this ?
	#define USE_OWN_SORT_ALGS 1
#endif // FBL_MAC


/**********************************************************************************************/
template<typename _Tp>
inline const _Tp&
fbl_median(const _Tp& __a, const _Tp& __b, const _Tp& __c)
{
  // concept requirements
  	if (__a < __b)
        if (__b < __c)
          return __b;
        else if (__a < __c)
          return __c;
        else
          return __a;
    else if (__a < __c)
        return __a;
    else if (__b < __c)
        return __c;
    else
        return __b;
}

template<typename _Tp, typename _Compare>
inline const _Tp&
fbl_median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
{
  // concept requirements
    if (__comp(__a, __b))
    	if (__comp(__b, __c))
     	 	return __b;
    	else if (__comp(__a, __c))
      		return __c;
    	else
      		return __a;
    else if (__comp(__a, __c))
    	return __a;
    else if (__comp(__b, __c))
    	return __c;
    else
    	return __b;
}


#pragma mark -
#pragma mark ====== Helper Methods ======= 


/**********************************************************************************************/
// We need this structure to send it as void* parameter into the entry point func of a thread.
//
template <class T>
struct Task_Partition
{
	vuint32		mRecursionLevel;
	
	T*			mBegin;
	T*			mEnd;	
};


template <class T, class C>
struct Task_Partition_Cmp
{
	vuint32		mRecursionLevel;
	
	T*			mBegin;
	T*			mEnd;	
	
	C			mComparator;
};


#pragma mark -


/**********************************************************************************************/
template <class T>
FBL_THREAD_ROUTINE( thread_qsort, inTaskPtr )
{
	// Extract data from parameter:
	Task_Partition<T>* pTask = (Task_Partition<T>*) inTaskPtr;
	
	T* ps = pTask->mBegin;
	T* pe = pTask->mEnd;
	
	if( ps < pe )
	{
		if( pTask->mRecursionLevel <= MAX_THREAD_RECURSION_LEVEL )
		{
            auto pivot = *std::next( ps, std::distance(ps,pe)/2 );
        
			// make two sub-arrays:	
			T* pm = std::partition( ps, pe,
            			[pivot](const T& em){ return em < pivot; } );
            
			FBL_CHECK( (ps < pe) && (ps <= pm) && (pm <= pe) );

			Task_Partition<T> task1 = { pTask->mRecursionLevel + 1, ps, pm };
			Task_Partition<T> task2 = { pTask->mRecursionLevel + 1, pm, pe };

			FBL_Thread pChildThread1( thread_qsort<T>, &task1 );
			FBL_Thread pChildThread2( thread_qsort<T>, &task2 );

			pChildThread1.join();
			pChildThread2.join();
		}
		else 
		{
			// do sorting of array in the current thread.
			std::sort( ps, pe );
		}
	}

	return 0;
}
 
 
/**********************************************************************************************/
template <class T, class C>
FBL_THREAD_ROUTINE( thread_qsort, inTaskPtr )
{
	// Extract data from parameter:
	Task_Partition_Cmp<T,C>* pTask = (Task_Partition_Cmp<T,C>*) inTaskPtr;
	
	T* ps = pTask->mBegin;
	T* pe = pTask->mEnd;
	

	if( ps < pe )
	{
		if( pTask->mRecursionLevel <= MAX_THREAD_RECURSION_LEVEL )
		{
			// make two sub-arrays:
            auto pivot = *std::next( ps, std::distance(ps,pe)/2 );

			T* pm = std::partition( ps, pe,
            			[&](const T& em){ return pTask->mComparator(em, pivot); } );
            

			FBL_CHECK( (ps < pe) && (ps <= pm) && (pm <= pe) );

			Task_Partition_Cmp<T,C> task1 = { pTask->mRecursionLevel + 1, ps, pm, pTask->mComparator };
			Task_Partition_Cmp<T,C> task2 = { pTask->mRecursionLevel + 1, pm, pe, pTask->mComparator };

			// Attention: Passing local var as argument to the thread is dangerous!
			// There is no guarantee that the var will not be destroyed before thread finish.
			// Here, following .Join() fix this - "main" is waiting pChildThread1 finish.
			FBL_Thread pChildThread1( thread_qsort<T,C>, &task1 );
			FBL_Thread pChildThread2( thread_qsort<T,C>, &task2 );

			pChildThread1.join();
			pChildThread2.join();
		}
		else 
		{
			// do sorting of array in the current thread.
			std::sort( ps, pe, pTask->mComparator );
		}
	}

	return 0;
}
 
 
#pragma mark -
#pragma mark ====== Entry Point - Sort Threaded ======= 
  
 
/**********************************************************************************************/
// USAGE: 
//    qsort_threaded<>( inpStart, inpFinish );
//
template <class T>
void qsort_threaded(
	T*		inBegin,		
	T*		inEnd )		
{
	Task_Partition<T> task1 = { 1, inBegin, inEnd };

	FBL_Thread pChildThread1( thread_qsort<T>, &task1 );
	pChildThread1.join();
}


/**********************************************************************************************/
// USAGE: 
//    qsort_threaded<>( inpStart, inpFinish );
//
template <class T, class C>
void qsort_threaded(
	T*		inBegin,		
	T*		inEnd,
	C		inComparator )		
{
	Task_Partition_Cmp<T,C> task1 = { 1, inBegin, inEnd, inComparator };

	// Attention: Passing local var as argument to the thread is dangerous!
	// There is no guarantee that the var will not be destroyed before thread finish.
	// Here, following .Join() fix this - "main" is waiting pChildThread1 finish.
	FBL_Thread pChildThread1( thread_qsort<T,C>, &task1 );
	pChildThread1.join();
}


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // __FBL_QuickSort_Long_Threaded_h

