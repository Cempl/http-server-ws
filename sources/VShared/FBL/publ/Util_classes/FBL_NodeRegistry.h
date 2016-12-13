/**********************************************************************************************/
/* FBL_NodeRegistry.h	                                                    				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_NodeRegistry_h 
	#define _FBL_NodeRegistry_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Unknown.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>

// STD:
#include <cstddef>
#include <map>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_CLASS(NodeRegistry);
SMART_CLASS(NodeRegistryEx);
//
SMART_ARRAY_OF_SIMPLES( ArrayOfPointers, ptrdiff_t );


/**********************************************************************************************/
// It uses in order to find the corresponded-to-original node in just-copying node-tree.
// ( 1. Node_Tbl_Constraint* is used in Node_Tbl_References_Specification
//   2. Node_Block* as pointer to upper block is used in Node_Block)
// 
// Key		- ptrdiff_t ("old-node" address)
// Value	- ptrdiff_t ("new-node" address)
//
//
class FBL_SHARED_EXP_CLASS NodeRegistry : public I_Unknown
{
	IMPLEMENT_UNKNOWN(NodeRegistry)											
		BEGIN_INTERFACE_TABLE(NodeRegistry)
	END_INTERFACE_TABLE()

	public://///////////////////////////////////////////////////////////////////////////////////

		typedef std::map<ptrdiff_t, ptrdiff_t>			TNodeMap;



	public://///////////////////////////////////////////////////////////////////////////////////

							NodeRegistry( void );
virtual		 				~NodeRegistry( void );


	public://///////////////////////////////////////////////////////////////////////////////////

virtual	void				Register( ptrdiff_t inOldNode, ptrdiff_t inNewNode );
virtual	void				Unregister( ptrdiff_t inOldNode );
virtual	void				Erase( void );

		ptrdiff_t			Find( ptrdiff_t inOldNode );

		int					GetCount( void ) { return (int) mNodeMap.size(); }


	protected://////////////////////////////////////////////////////////////////////////////////

		TNodeMap				mNodeMap;
};


/**********************************************************************************************/
class FBL_SHARED_EXP_CLASS NodeRegistryEx : public NodeRegistry
{
	public://///////////////////////////////////////////////////////////////////////////////////

							NodeRegistryEx( void );
virtual		 				~NodeRegistryEx( void );


	public://///////////////////////////////////////////////////////////////////////////////////

virtual	void				Register( ptrdiff_t inOldNode, ptrdiff_t inNewNode ) override;
virtual	void				Unregister( ptrdiff_t inOldNode ) override;
virtual	void				Erase( void ) override;

		bool				GetPair( vuint32 inIndex, ptrdiff_t& outKey, ptrdiff_t& outValue );
		

	protected://////////////////////////////////////////////////////////////////////////////////

		ArrayOfPointers_Ptr	mpKeys;
};


/**********************************************************************************************/
FBL_SHARED_EXP NodeRegistry* 	Get_Node_Registry( void );
FBL_SHARED_EXP NodeRegistryEx* 	Get_Tbl_Constraint_Registry( void );
FBL_SHARED_EXP NodeRegistryEx* 	Get_Node_Block_Registry( void );
FBL_SHARED_EXP NodeRegistryEx* 	Get_Tbl_References_Spec_Registry( void );


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_NodeRegistry_h
