/**********************************************************************************************/
/* VSQL_Exceptions.cpp                                                      				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Util_classes/FBL_String.h>

#include <VShared/VSQL/Errors/VSQL_ErrorMap.h>
#include <VShared/VSQL/Errors/VSQL_Exceptions.h>


/**********************************************************************************************/
VSQL_Begin_Namespace

/**********************************************************************************************/
const ERROR_TYPE gUserErrorRangeStart = 0x50000;
const ERROR_TYPE gUserErrorRangeEnd	= 0x50FFF;


/**********************************************************************************************/
ErrorEntry* FindErrorEntry( ERROR_TYPE inErrorType )
{
	ErrorEntry* pLeft 	= gErrorMap;
	ErrorEntry* pRight	= gErrorMap + (gErrorMapSize - 1);
	ErrorEntry* pMiddle;
	
	while( pLeft < pRight )
	{
		pMiddle = pLeft + ((pRight - pLeft) >> 1);
		if( pMiddle->mCode < inErrorType )
				pLeft 	= pMiddle + 1;
		else	pRight	= pMiddle;
	}

	return ( pRight->mCode < inErrorType ) ? nullptr : pRight;
}


/**********************************************************************************************/
ErrorEntry* FindErrorEntry( const char*	inErrorStr )
{
	FBL_CHECK( inErrorStr );

	ErrorEntry* result(nullptr);

	for( vuint32 i = 0; i < gErrorMapSize; ++i )
	{
		if( fbl_strcasecmp( gErrorMap[i].mSymbol, inErrorStr ) == 0 )
		{
			result = &gErrorMap[i];
			break;
		}
	}

	return result;
}


/**********************************************************************************************/
String xVSQLException::get_ErrorString( void ) const
{
	String res("UNKNOWN");

	if( mErrorCode >= gUserErrorRangeStart && mErrorCode <= gUserErrorRangeEnd )
	{
		// User defined error.
		if( mArgs[0] )
			res =  mArgs[0]->c_str();
	}
	else
	{
		// Kernel errors.
		if( gpFormatErrorMessage )
		{
			ErrorEntry* pE = VSQL::FindErrorEntry( mErrorCode );

			const UChar* args[] = {
				mArgs[0] ? mArgs[0]->c_str() : res.c_str(),
				mArgs[1] ? mArgs[1]->c_str() : res.c_str(),
				mArgs[2] ? mArgs[2]->c_str() : res.c_str(),
				mArgs[3] ? mArgs[3]->c_str() : res.c_str()
			};

			res = gpFormatErrorMessage( pE, args );
		}
	}

	return res;
}


/**********************************************************************************************/
VSQL_End_Namespace
