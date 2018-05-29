/**********************************************************************************************/
/* FBL_PtrValidators.cpp                                                   					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Utilities/FBL_PtrValidators.h>
#include <VShared/FBL/publ/Util_classes/FBL_DatabaseManager.h>


/**********************************************************************************************/
FBL_Begin_Namespace

/**********************************************************************************************/
bool IsValidDbPointer( I_Database* inPointer )
{
	return (bool)DatabaseManager::get_DatabaseManager()->FindIndex(inPointer);
}


/**********************************************************************************************/
bool IsValidTypePointer( I_Type* inPointer )
{	
	ArrayOfDatabases_Ptr dbs = DatabaseManager::get_DatabaseManager()->get_Databases();
	vuint32 count = dbs->get_Count();

	for( vuint32 i = 1; i <= count; ++i )
	{
		I_Database* pDb = dbs->get_ItemAt( i );

		vuint32 typeCount = pDb->get_TypeCount();
		for( vuint32 j = 1; j <= typeCount; ++j )
		{
			I_Type_Ptr pType = pDb->get_Type(j);

			if( pType.get() == inPointer )
				return true;		
		}
	}

	return false;
}


/**********************************************************************************************/
bool IsValidTablePointer( I_Table* inPointer )
{	
	ArrayOfDatabases_Ptr dbs = DatabaseManager::get_DatabaseManager()->get_Databases();
	vuint32 count = dbs->get_Count();
	
	for( vuint32 i = 1; i <= count; ++i )
	{
		I_Database* pDb = dbs->get_ItemAt( i );
		
		vuint32 tblCount = pDb->get_TableCount();
		for( vuint32 j = 1; j <= tblCount; ++j )
		{
			I_Table_Ptr pTbl = pDb->get_Table(j);
		
			if( pTbl.get() == inPointer )
				return true;		
		}
	}

	return false;
}


/**********************************************************************************************/
bool IsValidLinkPointer( I_Link* inPointer )
{
	ArrayOfDatabases_Ptr dbs = DatabaseManager::get_DatabaseManager()->get_Databases();
	vuint32 count = dbs->get_Count();
	
	for( vuint32 i = 1; i <= count; ++i )
	{
		I_Database* pDb = dbs->get_ItemAt( i );
		
		vuint32 lnkCount = pDb->get_LinkCount();
		for( vuint32 j = 1; j <= lnkCount; ++j )
		{
			I_Link_Ptr pLnk = pDb->get_Link(j);
		
			if( pLnk.get() == inPointer )
				return true;		
		}
	}
	
	return false;
}


/**********************************************************************************************/
bool IsValidKeyValuePointer( I_KeyValue* inPointer )
{
	ArrayOfDatabases_Ptr dbs = DatabaseManager::get_DatabaseManager()->get_Databases();
	vuint32 count = dbs->get_Count();
	
	for( vuint32 i = 1; i <= count; ++i )
	{
		I_Database* pDb = dbs->get_ItemAt( i );
		
		vuint32 kvCount = pDb->get_KeyValueCount();
		for( vuint32 j = 1; j <= kvCount; ++j )
		{
			I_KeyValue_Ptr pKeyValue = pDb->get_KeyValue(j);
		
			if( pKeyValue.get() == inPointer )
				return true;		
		}
	}
	
	return false;
}


/**********************************************************************************************/
bool IsValidIndexStylePointer( I_IndexStyle* inPointer )
{
	ArrayOfDatabases_Ptr dbs = DatabaseManager::get_DatabaseManager()->get_Databases();
	vuint32 count = dbs->get_Count();
	
	for( vuint32 i = 1; i <= count; ++i )
	{
		I_Database* pDb = dbs->get_ItemAt( i );
		
		vuint32 ISCount = pDb->get_IndexStyleCount();
		for( vuint32 j = 1; j <= ISCount; ++j )
		{
			I_IndexStyle_Ptr pIS = pDb->get_IndexStyle_ByIndex(j);
		
			if( pIS.get() == inPointer )
				return true;		
		}
	}
	
	return false;
}


/**********************************************************************************************/
FBL_End_Namespace
