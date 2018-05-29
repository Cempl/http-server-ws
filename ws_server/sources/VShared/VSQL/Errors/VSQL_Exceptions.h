/**********************************************************************************************/
/* VSQL_Exceptions.h			   															  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _VSQL_Exceptions_h 
	#define _VSQL_Exceptions_h
#pragma once

#include <VShared/FBL/publ/Errors/FBL_Exceptions.h>


/**********************************************************************************************/
VSQL_Begin_Namespace

/**********************************************************************************************/
FBL_SHARED_EXP extern const ERROR_TYPE gUserErrorRangeStart;
FBL_SHARED_EXP extern const ERROR_TYPE gUserErrorRangeEnd;


/**********************************************************************************************/
class FBL_SHARED_EXP_CLASS xVSQLException : public FBL::xException
{
	public://///////////////////////////////////////////////////////////////////////////////////
	
		//------------------
		// Construction
							
							xVSQLException( 
								ERROR_TYPE	inErrorCode = ERR_INTERNAL_ERROR,
								const UChar* inArg1 = nullptr,
								const UChar* inArg2 = nullptr,
								const UChar* inArg3 = nullptr,
								const UChar* inArg4 = nullptr )
							:
								xException( inErrorCode, inArg1, inArg2, inArg3, inArg4 )/*,
								mLine( inLine )*/
							{
							}

							xVSQLException( 
								ERROR_TYPE inErrorCode,
								const char* inArg1,
								const char* inArg2 = nullptr,
								const char* inArg3 = nullptr,
								const char* inArg4 = nullptr )
							:
								xException( inErrorCode, inArg1, inArg2, inArg3, inArg4 )
							{
							}

							xVSQLException( 
								ERROR_TYPE inErrorCode,
								const String& 	inFinalMessage,
								I_Connection* 	pConn)
							:
								xException( inErrorCode, inFinalMessage, pConn )
							{
							}
														
virtual						~xVSQLException( void ) throw()
							{
							}
							

	public://///////////////////////////////////////////////////////////////////////////////////

virtual	String				get_ErrorString( void ) const override;
	
	
	protected://///////////////////////////////////////////////////////////////////////////	
	
		//------------------
		// Dummy method
		
virtual	void				DummyCall( void ) const  {}

};


/**********************************************************************************************/
/**	Finds error entry in the map of existing error entries.
	@param inErrorCode - error code to find to.
	@return pointer to those entry if it was found or NULL,
		if there is no such error code. 
*/
FBL_SHARED_EXP ErrorEntry* FindErrorEntry( ERROR_TYPE inErrorCode );
FBL_SHARED_EXP ErrorEntry* FindErrorEntry( const char*	inErrorStr );


/**********************************************************************************************/
VSQL_End_Namespace


/**********************************************************************************************/
#include <VShared/VSQL/Errors/VSQL_ErrorGroups.h>


/**********************************************************************************************/
#endif // _VSQL_Exceptions_h
