/**********************************************************************************************/
/* FBL_ConverterBase.cpp                                                      				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Util_classes/FBL_ConverterBase.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
ConverterBase::ConverterBase( const char* inEncoding )
:
	mConverter( nullptr )
{
	if( inEncoding == nullptr )
	{
		mConverter = GetLocalizableSys()->get_IOConverter();
		mOwnConverter = false;
	}
	else
	{
		mConverter = CreateConverter( inEncoding );		
		mOwnConverter = true;
	}

	mMaxCharSize = (vuint8) mConverter->getMaxCharSize();
}


/**********************************************************************************************/
ConverterBase::ConverterBase( I_Converter* inICUConverter )
:
	mConverter( inICUConverter ),
	mOwnConverter( false )
{
	mMaxCharSize = (vuint8) mConverter->getMaxCharSize();
}


/**********************************************************************************************/
ConverterBase::~ConverterBase( void )
{
	if( mOwnConverter )
		delete mConverter;
}


/**********************************************************************************************/
FBL_End_Namespace
