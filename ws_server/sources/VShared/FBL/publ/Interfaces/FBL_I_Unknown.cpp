/**********************************************************************************************/
/* FBL_I_Unknown.cpp	                                                      				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h> 

#include <VShared/FBL/publ/Headers/FBL.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
#if FBL_USE_MEMPOOL


/**********************************************************************************************/
// This gPoolRecycle should be ONE per thread, and live the whole life of thread.
// scope_alloc -ators will be created on this gPoolRecycle.
//
NS_BOOST_MEMORY::block_pool		gPoolRecycle;


/**********************************************************************************************/
// This is the pointer on the CURRENT allocator. 
// ANY I_Unknown child-class will respect this pointer, thanks to I_Unknown::operator new().
// 
// When you want create some objects inside of MEMORY POOL, you must assign this pointer to pool
// then set it back to NULL when you done.
// 
// When pointer is NULL, then objects are created on the main system pool.
//
// If you have place objects to POOL and you want destroy them all together killing pool,
// then you must care that for these objects will NOT be called destructors.
// Usually this is easy to do, because will exists TOP object of hierarchy stored in the POOl, 
// which you point by a smart_ptr. So you need just add one line 
//		pTopObj.free()
// after this smart_ptr will not destroy underline object. 
// 
// You still can call destructors for objects stored in the MEM POOL, in this case 
// pool just free them so we can forget about such objects at all. This also may benefit from 
// MEM POOL because it is faster of system pool.
//
NS_BOOST_MEMORY::scoped_alloc* pAllocator = NULL;


/**********************************************************************************************/
void* I_Unknown::operator new( size_t inSize )	
{									
	void* res;						
	
	res = pAllocator ? pAllocator->allocate(inSize) 
					 : malloc(inSize);	// еееее RZ TODO  use DebugNew  function here !!! еее
	
	return res;		
}

#endif // FBL_USE_MEMPOOL


#pragma mark -


/**********************************************************************************************/
void* InterfaceTableQueryInterface( 
	const void* 			inThis, 
	const INTERFACE_ITEM* 	inTable, 
	InterfaceID		 		inIID )
{
	void* res = nullptr;

	if( inIID == IID_I_Unknown )
	{
		// first entry should be offset of I_Unknown interface:
		res = (char*)inThis + inTable->offset;
	}
	else
	{
		while( inTable->iid )
		{
			if( inTable->iid == inIID )
			{
				res = (char*)inThis + inTable->offset;
				break;
			}
			
			++inTable;
		}
	}
	
	return res;
}


/**********************************************************************************************/
FBL_End_Namespace
