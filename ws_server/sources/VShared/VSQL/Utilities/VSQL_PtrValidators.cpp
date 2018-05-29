/**********************************************************************************************/
/* VSQL_PtrValidators.cpp                                                  					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017 															  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/VSQL/Utilities/VSQL_PtrValidators.h>
#include <VShared/FBL/publ/Util_classes/FBL_DatabaseManager.h>
#include <VShared/VSQL/Util_classes/VSQL_ProjectManager.h>


/**********************************************************************************************/
VSQL_Begin_Namespace


/**********************************************************************************************/
#if !FBL_MAC_CARBON

bool IsValidProjectPointer( I_VProject* inPointer )
{
	return (bool)ProjectManager::get_ProjectManager()->FindIndex(inPointer);
}

#endif // !FBL_MAC_CARBON


/**********************************************************************************************/
bool IsValidCursorPointer( VSQL::I_Cursor* inPointer )
{
	ArrayOfDatabases_Ptr dbs = DatabaseManager::get_DatabaseManager()->get_Databases();
	vuint32 count = dbs->get_Count();
	
	for( vuint32 i = 1; i <= count; ++i )
	{
		I_SqlDatabase* pDb = dynamic_cast<I_SqlDatabase*>( dbs->get_ItemAt( i ) );
		if( pDb )
		{			
			vuint32 cursorCount = pDb->get_CursorCount();
			for( vuint32 j = 1; j <= cursorCount; ++j )
			{
				VSQL::I_Cursor* pCursor = pDb->get_Cursor(j);
			
				if( pCursor == inPointer )
					return true;		
			}
		}
	}
	
	return false;
}


/**********************************************************************************************/
bool IsValidFieldPointer( I_Field* inPointer )
{
	ArrayOfDatabases_Ptr dbs = DatabaseManager::get_DatabaseManager()->get_Databases();
	vuint32 count = dbs->get_Count();
	
	for( vuint32 i = 1; i <= count; ++i )
	{
		// Looking in normal "tables":
		FBL::I_Database* pDb = dbs->get_ItemAt( i );
		
		if( IsValidFieldPointer_FBL( pDb, inPointer) )
			return true;
					
		// Looking in cursors:
		I_SqlDatabase* psqlDb = dcast<I_SqlDatabase*>( pDb );
		if( psqlDb == nullptr )
			return false;

		if( IsValidFieldPointer_VSQL( psqlDb, inPointer) )
			return true;
	}
	
	return false;
}


#pragma mark -

/**********************************************************************************************/
bool IsValidFieldPointer_FBL( 
	FBL::I_Database*	inpDb, 
	I_Field*			inPointer )
{
//	FBL_CHECK( inpDb );
	
	vuint32 tblCount = inpDb->get_TableCount();
	for( vuint32 j = 1; j <= tblCount; ++j )
	{
		I_Table_Ptr pTbl = inpDb->get_Table(j);
		
		if( FieldExist( pTbl, inPointer ) )
			return true;			
	}
	
	return false;		
}


/**********************************************************************************************/
bool IsValidFieldPointer_VSQL( 
	I_SqlDatabase*	inpDb, 
	I_Field*			inPointer )
{
//	FBL_CHECK( inpDb );
	
	vuint32 cursorCount = inpDb->get_CursorCount();
	for( vuint32 j = 1; j <= cursorCount; ++j )
	{
		VSQL::I_Cursor* pCursor = inpDb->get_Cursor(j);
		I_Table_Ptr pTbl = dcast<I_Table*>(pCursor);
		
		if( FieldExist( pTbl, inPointer ) )
			return true;			
	}
	
	return false;
}


/**********************************************************************************************/
bool FieldExist( 
	I_Table_Ptr			inpTbl,
	I_Field*			inPointer)
{
//	FBL_CHECK( inpTbl );
	
	vuint16 fldCount = inpTbl->get_FieldCount(); 
	for( vuint16 k = 1; k <= fldCount; ++k )
	{
		I_Field_Ptr pFld = inpTbl->get_Field(k);
		
		if( pFld.get() == inPointer )
			return true;		
	}	
	
	return false;
}	

	
/**********************************************************************************************/
VSQL_End_Namespace
