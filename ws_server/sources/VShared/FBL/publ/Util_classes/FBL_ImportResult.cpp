/**********************************************************************************************/
/* FBL_ImportResult.cpp			                                                   			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/
 
#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Util_classes/FBL_ImportResult.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
ImportResult::ImportResult()
:
	mOptions(0),
	mUlongValue( 0 )
{
}


/**********************************************************************************************/
ImportResult::ImportResult( const vuint16 inOptions )
:
	mOptions( inOptions ),

	mUlongValue( 0 )
{
	if( inOptions & kWarnings )
		mpWarnings = new ArrayOfStringsObj();
		
	if( inOptions & kOutputs )
		mpOutputs = new ArrayOfStringsObj();	
}


/**********************************************************************************************/
ImportResult::ImportResult( const ImportResult& inImportResult )
:
	mOptions(0),

	mUlongValue( inImportResult.get_AsULong() )
{
}


/**********************************************************************************************/
ImportResult::~ImportResult( void )
{
}


#pragma mark -

/**********************************************************************************************/
vuint16 ImportResult::get_Options() const 
{
	return mOptions;
}


/**********************************************************************************************/
vuint32 ImportResult::get_AsULong( void ) const 
{
	return mUlongValue;
}


/**********************************************************************************************/
void ImportResult::put_AsULong( vuint32 inValue ) 
{
	mUlongValue = inValue;
}


/**********************************************************************************************/
ArrayOfStringsObj_Ptr ImportResult::get_Warnings( void ) const 
{
	FBL_CHECK( (mOptions & kWarnings) ? mpWarnings != nullptr : mpWarnings == nullptr);
	return mpWarnings;
}


/**********************************************************************************************/
void ImportResult::put_Warnings( ArrayOfStringsObj_Ptr inpWarnings ) 
{	
	mOptions |= kWarnings;
	mpWarnings = inpWarnings;
}


/**********************************************************************************************/
ArrayOfStringsObj_Ptr ImportResult::get_Outputs( void ) const 
{
	FBL_CHECK( (mOptions & kOutputs) ? mpOutputs != nullptr : mpOutputs == nullptr);
	return mpOutputs;
}


/**********************************************************************************************/
void ImportResult::put_Outputs( ArrayOfStringsObj_Ptr inpOutputs ) 
{	
	mOptions |= kOutputs;
	mpOutputs = inpOutputs;
}


/**********************************************************************************************/
FBL_End_Namespace
