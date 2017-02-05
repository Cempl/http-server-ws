/**********************************************************************************************/
/* FBL_CreateEnumType.cpp 	                                                   				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_Type_Enumerated.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Database.h>
#include <VShared/FBL/publ/Util_classes/FBL_String.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// Factory to create and define enum type
//
I_Type_Enumerated_Ptr CreateEnumType( 
	I_Database_Ptr		inpDb,
	const String&		inName,
	ArrayOfStrings_Ptr	inIdents,
	vuint16				inMaxIdentCount )
{	
	FBL_CHECK( inpDb );

	I_Type_Ptr pType = inpDb->CreateType( inName, vcEnumeratedType );
	I_Type_Enumerated_Ptr pEnumType = QI( pType, I_Type_Enumerated );

	try
	{
		pEnumType->put_MaxIdentCount( inMaxIdentCount );
		pEnumType->AddIdents( inIdents );
	}
	catch(...)
	{
		inpDb->DropType( pType );
		throw;
	}

	return pEnumType;
}	


/**********************************************************************************************/
// Factory to create and define enum type
//
I_Type_Enumerated_Ptr CreateEnumType( 
	I_Database_Ptr			inpDb,
	const String&			inName,
	ArrayOfStringsObj_Ptr	inIdents,
	vuint16					inMaxIdentCount )
{	
	FBL_CHECK( inpDb );

	I_Type_Ptr pType = inpDb->CreateType( inName, vcEnumeratedType );
	I_Type_Enumerated_Ptr pEnumType = QI( pType, I_Type_Enumerated );

	try
	{
		pEnumType->put_MaxIdentCount( inMaxIdentCount );

		vuint32 count = inIdents->get_Count();
		for( vuint32 i = 1; i <= count; ++i )
		{
			const String& ident = inIdents->get_ItemAt(i);
			pEnumType->AddIdent( ident );
		}
	}
	catch(...)
	{
		inpDb->DropType( pType );
		throw;
	}

	return pEnumType;
}	


/**********************************************************************************************/
FBL_End_Namespace

