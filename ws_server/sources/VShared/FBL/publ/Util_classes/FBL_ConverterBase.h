/**********************************************************************************************/
/* FBL_ConverterBase.h																		  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_ConverterBase_h
	#define _FBL_ConverterBase_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Interfaces/FBL_I_Localizable.h>
#include <VShared/FBL/publ/Util_Strings/FBL_ustdio.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
/**
	Parent class for StToUTF16 and StFromUTF16.
	Should not be be used directly, so we protect destructor.	

	It has 2 constructors: one for inEncoding, another for inConverter.
*/
class FBL_SHARED_EXP_CLASS ConverterBase : public noncopyable
{
	protected://////////////////////////////////////////////////////////////////////////////////

virtual						~ConverterBase( void );


	protected://////////////////////////////////////////////////////////////////////////////////

							ConverterBase( 
								const char* 	inEncoding );
								
							ConverterBase( 
								I_Converter* 	inICUConverter );


	public://///////////////////////////////////////////////////////////////////////////////////

		I_Converter*		get_Converter( void ) const 
								{ return mConverter; }


	protected://////////////////////////////////////////////////////////////////////////////////
		
		I_Converter*		mConverter;
		bool				mOwnConverter;
		
		vuint8				mMaxCharSize;
};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif //_FBL_ConverterBase_h
