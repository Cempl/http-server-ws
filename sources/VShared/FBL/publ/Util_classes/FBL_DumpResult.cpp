/**********************************************************************************************/
/* FBL_DumpResult.cpp			                                                   			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/
 
#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Util_classes/FBL_DumpResult.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
DumpResult::DumpResult()
:
	mOptions(0),
	mSuccessfulCount(0),
	mFailedCount(0)
{
}


/**********************************************************************************************/
DumpResult::DumpResult( const vuint16 inOptions )
:
	mOptions( inOptions ),

	mSuccessfulCount( 0 ),
	mFailedCount(0)
{
	if( inOptions & kWarnings )
		mpWarnings = new ArrayOfStringsObj();
		
	if( inOptions & kOutputs )
		mpOutputs = new ArrayOfStringsObj();	
}


/**********************************************************************************************/
DumpResult::DumpResult( const DumpResult& inDumpResult )
:
	mOptions(0),

	mSuccessfulCount( inDumpResult.get_SuccessfulCount() ),
	mFailedCount( inDumpResult.get_FailedCount() )
{
}


/**********************************************************************************************/
DumpResult::~DumpResult( void )
{
}


#pragma mark -

/**********************************************************************************************/
vuint16 DumpResult::get_Options() const
{
	return mOptions;
}


/**********************************************************************************************/
vuint32 DumpResult::get_SuccessfulCount( void ) const
{
	return mSuccessfulCount;
}


/**********************************************************************************************/
void DumpResult::put_SuccessfulCount( vuint32 inValue )
{
	mSuccessfulCount = inValue;
}


/**********************************************************************************************/
vuint32 DumpResult::get_FailedCount( void ) const
{
	return mFailedCount;
}


/**********************************************************************************************/
void DumpResult::put_FailedCount( vuint32 inValue )
{
	mFailedCount = inValue;
}


/**********************************************************************************************/
ArrayOfStringsObj_Ptr DumpResult::get_Warnings( void ) const
{
	FBL_CHECK( (mOptions & kWarnings) ? mpWarnings != nullptr : mpWarnings == nullptr);
	return mpWarnings;
}


/**********************************************************************************************/
void DumpResult::put_Warnings( ArrayOfStringsObj_Ptr inpWarnings )
{	
	mOptions |= kWarnings;
	mpWarnings = inpWarnings;
}


/**********************************************************************************************/
ArrayOfStringsObj_Ptr DumpResult::get_Outputs( void ) const
{
	FBL_CHECK( (mOptions & kOutputs) ? mpOutputs != nullptr : mpOutputs == nullptr);
	return mpOutputs;
}


/**********************************************************************************************/
void DumpResult::put_Outputs( ArrayOfStringsObj_Ptr inpOutputs ) 
{	
	mOptions |= kOutputs;
	mpOutputs = inpOutputs;
}


/**********************************************************************************************/
FBL_End_Namespace
