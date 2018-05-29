/**********************************************************************************************/
/* VSQL_QueryResult.cpp			                                                   			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/
 
#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/VSQL/Database/VSQL_QueryResult.h>


/**********************************************************************************************/
VSQL_Begin_Namespace


/**********************************************************************************************/
QueryResult::QueryResult()
:
	mResultType( kQueryResult_Empty ),
	mResultTypeEx( kQueryResultEx_Empty ),
	mFlags( kNone ),
	mOptions(0),
	
	mpCursor( nullptr ),
	mBoolValue(false),
	mUlongValue( 0 ),
	mpStringValue( nullptr ),
	mLastRecID(0),
	mpExplainStr( nullptr )
{
}


/**********************************************************************************************/
QueryResult::QueryResult( const vuint16 inOptions )
:
	mResultType( kQueryResult_Empty ),
	mResultTypeEx( kQueryResultEx_Empty ),	
	mFlags( kNone ),
	mOptions( inOptions ),

	mpCursor( nullptr ),
	mBoolValue(false),
	mUlongValue( 0 ),
	mpStringValue( nullptr ),
	mLastRecID(0),
	mpExplainStr( nullptr )
{
	if( inOptions & kWarnings )
		mpWarnings = new ArrayOfStringsObj();
		
	if( inOptions & kTunes )
		mpTunes = new ArrayOfStringsObj();
		
	if( inOptions & kOutputs )
		mpOutputs = new ArrayOfStringsObj();	
}


/**********************************************************************************************/
QueryResult::QueryResult( const QueryResult& inQueryResult )
:
	mResultType( inQueryResult.get_Type() ),
	mResultTypeEx( inQueryResult.mResultTypeEx ),
	mFlags( inQueryResult.get_Flags() ),
	mOptions(0),

	mpCursor( inQueryResult.get_AsCursor() ),
	mBoolValue( inQueryResult.get_AsBoolean() ),
	mUlongValue( inQueryResult.get_AsULong() ),
	mLastRecID( inQueryResult.get_LastRecID() )
{
	mpStringValue = new String( *(inQueryResult.get_AsString()) );
}


/**********************************************************************************************/
QueryResult::~QueryResult( void )
{
	ForgetObject( mpStringValue );
	ForgetObject( mpExplainStr );
}


#pragma mark -

/**********************************************************************************************/
EQueryResult QueryResult::get_Type( void ) const 
{
	return mResultType;
}


/**********************************************************************************************/
bool QueryResult::hasType( EQueryResult inResultType ) const
{
	// We need such an "ugly" code because of inability to change EQueryResult values
	// to make them comparable to EQueryResultEx values - we have to support "old" clients.
	//
	
	bool res = false;
	
	switch( inResultType )
	{
		case kQueryResult_Empty:
		{
			res = (mResultTypeEx & kQueryResultEx_Empty);
		} break;

		case kQueryResult_Cursor:
		{
			res = (mResultTypeEx & kQueryResultEx_Cursor);		
		} break;

		case kQueryResult_Boolean:
		{
			res = (mResultTypeEx & kQueryResultEx_Boolean);		
		} break;

		case kQueryResult_ULong:
		{
			res = (mResultTypeEx & kQueryResultEx_ULong);		
		} break;

		case kQueryResult_String:
		{
			res = (mResultTypeEx & kQueryResultEx_String);		
		} break;
		
		default:
			FBL_CHECK(false);
	}
	
	return res;
}


/**********************************************************************************************/
EQueryResultEx QueryResult::get_TypeEx( void ) const 
{
	return mResultTypeEx;
}


/**********************************************************************************************/
vuint16 QueryResult::get_Options() const 
{
	return mOptions;
}


/**********************************************************************************************/
EQueryFlags QueryResult::get_Flags( void ) const 
{
	return mFlags;
}


/**********************************************************************************************/
void QueryResult::SetFlags( EQueryFlags inFlag )
{
	mFlags = (inFlag) ? (EQueryFlags)(mFlags | inFlag) : kNone;
}


#pragma mark -

/**********************************************************************************************/
I_Cursor_Ptr QueryResult::get_AsCursor( void ) const 
{
	return mpCursor;
}


/**********************************************************************************************/
void QueryResult::put_AsCursor( I_Cursor_Ptr inpCursor ) 
{
	mResultType = kQueryResult_Cursor;
	mResultTypeEx = (EQueryResultEx)(mResultTypeEx | kQueryResultEx_Cursor);
	mpCursor	= inpCursor;
}


#pragma mark -


/**********************************************************************************************/
bool QueryResult::get_AsBoolean( void ) const 
{
	return mBoolValue;
}


/**********************************************************************************************/
void QueryResult::put_AsBoolean( bool inValue ) 
{
	mResultType = kQueryResult_Boolean;
	mResultTypeEx = (EQueryResultEx)(mResultTypeEx | kQueryResultEx_Boolean);	
	mBoolValue  = inValue;
}


#pragma mark -


/**********************************************************************************************/
vuint32 QueryResult::get_AsULong( void ) const 
{
	return mUlongValue;
}


/**********************************************************************************************/
void QueryResult::put_AsULong( vuint32 inValue ) 
{
	mResultType = kQueryResult_ULong;
	mResultTypeEx = (EQueryResultEx)(mResultTypeEx | kQueryResultEx_ULong);	
	mUlongValue = inValue;
}


#pragma mark -


/**********************************************************************************************/
const String* QueryResult::get_AsString( void ) const 
{
	return mpStringValue;
}


/**********************************************************************************************/
void QueryResult::put_AsString( String*	inpValue ) 
{
	// if we have string here we need destroy it
	ForgetObject( mpStringValue );

	mResultType   = kQueryResult_String;
	mResultTypeEx = (EQueryResultEx)(mResultTypeEx | kQueryResultEx_String);
	mpStringValue = inpValue;
}


/**********************************************************************************************/
void QueryResult::put_AsString( const String& inValue ) 
{
	mResultType   = kQueryResult_String;
	mResultTypeEx = (EQueryResultEx)(mResultTypeEx | kQueryResultEx_String);
	mpStringValue = new String( inValue );
}


/**********************************************************************************************/
REC_ID QueryResult::get_LastRecID( void ) const
{
	return mLastRecID;
}


/**********************************************************************************************/
void QueryResult::put_LastRecID( REC_ID inRecID )
{
	mLastRecID = inRecID;
}


/**********************************************************************************************/
ArrayOfStringsObj_Ptr QueryResult::get_Warnings( void ) const 
{
	FBL_CHECK( (mOptions & kWarnings) ? mpWarnings != nullptr : mpWarnings == nullptr);
	return mpWarnings;
}


/**********************************************************************************************/
void QueryResult::put_Warnings( ArrayOfStringsObj_Ptr inpWarnings ) 
{	
	mOptions |= kWarnings;
	mpWarnings = inpWarnings;
}


/**********************************************************************************************/
ArrayOfStringsObj_Ptr QueryResult::get_Tunes( void ) const 
{
	FBL_CHECK( (mOptions & kTunes) ? mpTunes != nullptr : mpTunes == nullptr);
	return mpTunes;
}


/**********************************************************************************************/
void QueryResult::put_Tunes( ArrayOfStringsObj_Ptr inpTunes ) 
{
	mOptions |= kTunes;
	mpTunes = inpTunes;
}


/**********************************************************************************************/
ArrayOfStringsObj_Ptr QueryResult::get_Outputs( void ) const 
{
	FBL_CHECK( (mOptions & kOutputs) ? mpOutputs != nullptr : mpOutputs == nullptr);
	return mpOutputs;
}


/**********************************************************************************************/
void QueryResult::put_Outputs( ArrayOfStringsObj_Ptr inpOutputs ) 
{	
	mOptions |= kOutputs;
	mpOutputs = inpOutputs;
}


#pragma mark -

/**********************************************************************************************/
const String* QueryResult::get_Explanation( void ) const 
{
	return mpExplainStr;
}


/**********************************************************************************************/
void QueryResult::put_Explanation( String* inExplainStr )
{
	// if we have string here we need destroy it
	ForgetObject( mpExplainStr );

	mpExplainStr = inExplainStr;
}


/**********************************************************************************************/
VSQL_End_Namespace
