/**********************************************************************************************/
/* FBL_StringConversion.h																	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_StringConversion_h
	#define _FBL_StringConversion_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Util_classes/FBL_ConverterBase.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
class String;


/**********************************************************************************************/
///	Converts Unicode strings to the codepage strings.
///
class FBL_SHARED_EXP_CLASS StFromUTF16 : public ConverterBase
{
	public://///////////////////////////////////////////////////////////////////////////////////
	 
	 						StFromUTF16( 
	 							const UChar* 	inUStr, 
	 							tslen 			inLen = -1,
	 							const char* 	inEncoding = nullptr );

	 						StFromUTF16( 
	 							const UChar* 	inUStr, 
	 							const UChar* 	inEnd, 
	 							const char* 	inEncoding = nullptr );							

							StFromUTF16( 
								const String& 	inStr,
								const char* 	inEncoding = nullptr );							
								
							// -----------------
	 						StFromUTF16( 
	 							const UChar* 	inUStr, 
	 							tslen 			inLen,
	 							I_Converter* 	inConverter );

	 						StFromUTF16( 
	 							const UChar* 	inUStr, 
	 							const UChar* 	inEnd, 
	 							I_Converter* 	inConverter );

							StFromUTF16( 
								const String& 	inStr,
								I_Converter* 	inConverter );							

							StFromUTF16( 
								I_Converter* 	inConverter );

							~StFromUTF16( void );


	public://///////////////////////////////////////////////////////////////////////////////////

		const char*			c_str( void ) const		{ return mAStr;	}
		tslen				length( void ) const	{ return mLen;	}

		
							// Can assign a new string into this object.
		void				Convert( 
								const UChar*	inUStr, 
								tslen			inLen = -1 );
		
		void				Convert( 
								const UChar*	inUStr, 
								const UChar* 	inEnd = nullptr );
								
		void				Convert( 
								const String&	inStr );


	private:////////////////////////////////////////////////////////////////////////////////////
		
		void				Free( void );
		void				Init( void );

							/**	Converts @inUStr into target encoding. Target string 
								will be stored in internal buffer.
								It can be enlarged dynamically to store 
								result, so truncation will never occurred. */
		void				ConvertUsingInternalBuffer( 
								const UChar*	inUStr, 
								tslen			inLen );


	private:////////////////////////////////////////////////////////////////////////////////////

		char*				mAStr;
		tslen				mLen;
							
							/// OPTIMIZATION: to avoid memory allocation for small strings.
		static const tslen	LOCAL_LEN = 64;
		char				mLocal[LOCAL_LEN + 1];
};


/**********************************************************************************************/
///	Converts codepage string to Unicode string.
///
class FBL_SHARED_EXP_CLASS StToUTF16 : public ConverterBase
{
	public://///////////////////////////////////////////////////////////////////////////////////

	 						StToUTF16( 
	 							const char* 	inAStr, 
	 							tslen 			inLen = -1,
								const char* 	inEncoding = nullptr );							

	 						StToUTF16( 
	 							const char* 	inAStr, 
	 							const char*		inEnd,
								const char* 	inEncoding = nullptr );							

							StToUTF16( 
								vuint32 		inToAllocate,
								const char* 	inEncoding = nullptr );
								

							// -----------------
	 						StToUTF16( 
	 							const char* 	inAStr, 
	 							tslen 			inLen,
								I_Converter* 	inConverter );							

	 						StToUTF16( 
	 							const char* 	inAStr, 
	 							const char*		inEnd,
								I_Converter* 	inConverter );							

							StToUTF16(
								vuint32 		inToAllocate, 
								I_Converter* 	inConverter );								

							~StToUTF16( void );


	public://///////////////////////////////////////////////////////////////////////////////////

		const UChar*		c_str( void ) const		{ return mUStr; }
		tslen				length( void ) const	{ return mLen;	}

		void				Convert( 
								const char*		inAStr,
								tslen			inLen = -1 );


	private:////////////////////////////////////////////////////////////////////////////////////

		void				Free( void );
		void				Init( void );
							
							/**	Converts @inAStr into target encoding. Target string 
								will be stored in internal buffer.
								It can be enlarged dynamically to store 
								result, so truncation will never occurred. */
		void				ConvertUsingInternalBuffer( 
								const char*		inAStr, 
								tslen			inLen );


	private:////////////////////////////////////////////////////////////////////////////////////

		UChar*				mUStr;
		tslen				mLen;
														
							/// OPTIMIZATION: To avoid memory allocation for small strings.
		static const tslen	LOCAL_LEN = 64;
		UChar				mLocal[LOCAL_LEN + 1];
};


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif //_FBL_StringConversion_h
