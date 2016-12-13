/**********************************************************************************************/
/* FBL_NodeRegistry.cpp                                                						  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Util_classes/FBL_NodeRegistry.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
NodeRegistry::NodeRegistry( void )
{
}


/**********************************************************************************************/
NodeRegistry::~NodeRegistry( void )
{
}


#pragma mark -


/**********************************************************************************************/
void NodeRegistry::Register( ptrdiff_t inOldNode, ptrdiff_t inNewNode ) 
{
	if( inOldNode )
	{
		mNodeMap.insert(
		std::make_pair(
		inOldNode,
		inNewNode
		) );
	}
}


/**********************************************************************************************/
void NodeRegistry::Unregister( ptrdiff_t inOldNode ) 
{
	if( inOldNode )
	{
		mNodeMap.erase( inOldNode );
	}
}


/**********************************************************************************************/
void NodeRegistry::Erase( void ) 
{
	mNodeMap.clear();
}


/**********************************************************************************************/
ptrdiff_t NodeRegistry::Find( ptrdiff_t inOldNode ) 
{
	TNodeMap::iterator i = mNodeMap.find(inOldNode);
	if( i == mNodeMap.end() )
		return 0;

	return i->second;
}


#pragma mark ====== NodeRegistryEx Class =======
#pragma mark -


/**********************************************************************************************/
NodeRegistryEx::NodeRegistryEx( void )
	: mpKeys( new ArrayOfPointers() )
{
}


/**********************************************************************************************/
NodeRegistryEx::~NodeRegistryEx( void )
{
}


/**********************************************************************************************/
void NodeRegistryEx::Register( ptrdiff_t inOldNode, ptrdiff_t inNewNode ) 
{
	if( inOldNode )
	{
		mNodeMap.insert(
			std::make_pair(
			inOldNode,
			inNewNode
			) );

		mpKeys->AddItem(inOldNode);
	}
}


/**********************************************************************************************/
void NodeRegistryEx::Unregister( ptrdiff_t inOldNode ) 
{
	if( inOldNode )
	{
		mNodeMap.erase( inOldNode );
		mpKeys->RemoveItem(inOldNode);
	}
}


/**********************************************************************************************/
void NodeRegistryEx::Erase( void ) 
{
	mNodeMap.clear();
	mpKeys->DisposeItems();
}


/**********************************************************************************************/
bool NodeRegistryEx::GetPair( vuint32 inIndex, ptrdiff_t& outKey, ptrdiff_t& outValue ) 
{
	bool res(false);
	if( mpKeys->get_Count() >= inIndex )
	{
		outKey = mpKeys->get_ItemAt( inIndex );
		outValue = Find( outKey );

		res = (bool)outValue;
	}
	return res;
}



#pragma mark -



/**********************************************************************************************/
// Singleton of NodeRegistry.
//
NodeRegistry* Get_Node_Registry( void )
{
	static NodeRegistry_Ptr sNodeRegistry = nullptr;

	if( sNodeRegistry == nullptr )
	{
		sNodeRegistry = new NodeRegistry;
	}

	return sNodeRegistry.get();
}


/**********************************************************************************************/
// Singleton of NodeRegistry.
//
NodeRegistryEx* Get_Tbl_Constraint_Registry( void )
{
	static NodeRegistryEx_Ptr sTblConstraintRegistry = nullptr;

	if( sTblConstraintRegistry == nullptr )
	{
		sTblConstraintRegistry = new NodeRegistryEx;
	}

	return sTblConstraintRegistry.get();
}


/**********************************************************************************************/
// Singleton of NodeRegistry.
//
NodeRegistryEx* Get_Tbl_References_Spec_Registry( void )
{
	static NodeRegistryEx_Ptr sTblRefSpecRegistry = nullptr;

	if( sTblRefSpecRegistry == nullptr )
	{
		sTblRefSpecRegistry = new NodeRegistryEx;
	}

	return sTblRefSpecRegistry.get();
}


/**********************************************************************************************/
// Singleton of NodeRegistry.
//
NodeRegistryEx* Get_Node_Block_Registry( void )
{
	static NodeRegistryEx_Ptr sBlockRegistry = nullptr;

	if( sBlockRegistry == nullptr )
	{
		sBlockRegistry = new NodeRegistryEx;
	}

	return sBlockRegistry.get();
}


/**********************************************************************************************/
FBL_End_Namespace
