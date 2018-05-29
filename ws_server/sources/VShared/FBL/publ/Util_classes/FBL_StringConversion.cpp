/**********************************************************************************************/
/* FBL_StringConversion.cpp                                                      			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Util_classes/FBL_StringConversion.h>

#include <VShared/FBL/publ/Util_classes/FBL_String.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// This converter is created by GetConverter_UTF8() singlenton,
// and lives up to the end of appication. 
//
namespace internal
{
static	True_Thread_Mutex			sConverterUTF8SingletonMutex;
#if STD_THREADING_SAFE
static	std::atomic<I_Converter*>	sConverterUTF8;
#else
static	I_Converter* volatile		sConverterUTF8 = nullptr;
#endif // STD_THREADING_SAFE
}


/**********************************************************************************************/
I_Converter* GetConverter_UTF8( void )
{
	// Double-Checked Locking Pattern
	// http://www.aristeia.com/Papers/DDJ_Jul_Aug_2004_revised.pdf
	// C++11 - http://preshing.com/20130930/double-checked-locking-is-fixed-in-cpp11/
	//

	#if STD_THREADING_SAFE
		I_Converter* tmp = internal::sConverterUTF8.load();
		if( tmp == nullptr )
		{
			StLockGuard<True_Thread_Mutex> lock(internal::sConverterUTF8SingletonMutex);
			tmp = internal::sConverterUTF8.load();
			if( tmp == nullptr )
			{
				tmp = CreateConverter( "UTF8" );
				internal::sConverterUTF8.store(tmp);
			}
		}
	
		return tmp;
	#else
		if( internal::sConverterUTF8 == nullptr )
		{
			StLockGuard<True_Thread_Mutex> lock(internal::sConverterUTF8SingletonMutex);
			if( internal::sConverterUTF8 == nullptr )
			{
				// http://www.drdobbs.com/cpp/c-and-the-perils-of-double-checked-locki/184405726
				// Use tmp var to avoid second thread thinking that sGlobalProperties is not null
				// before construction is really finished in the current thread.
				// It is because "var = new type()" is not atomic.
				// There are 3 steps:
				// Step 1. Allocate memory to hold a Singleton object.
				// Step 2. Construct a Singleton object in the allocated memory.
				// Step 3. Make var point to the allocated memory.
				// Compilers are sometimes allowed to swap Steps 2 and 3.
				//
				I_Converter* volatile pTemp = CreateConverter( "UTF8" );
				internal::sConverterUTF8 = pTemp;
			}
		}
		
		return internal::sConverterUTF8;
	#endif // STD_THREADING_SAFE
}


#pragma mark -


/**********************************************************************************************/
StFromUTF16::StFromUTF16( 
	const String& 	inStr,
	const char* 	inEncoding )
:
	ConverterBase( inEncoding ),
	mAStr( nullptr ),
	mLen( 0 )
{
	Init();
	ConvertUsingInternalBuffer( inStr.c_str(), inStr.length() );
}


/**********************************************************************************************/
StFromUTF16::StFromUTF16( 
	const UChar* 	inUStr, 
	tslen 			inLen,
	const char* 	inEncoding )
:
	ConverterBase( inEncoding ),
	mAStr(nullptr),
	mLen(0)
{
	Init();
	ConvertUsingInternalBuffer(inUStr, inLen);
}


/**********************************************************************************************/
StFromUTF16::StFromUTF16( 
	const UChar* 	inUStr, 
	const UChar* 	inEnd, 
	const char* 	inEncoding )
:
	ConverterBase( inEncoding ),
	mAStr(nullptr),
	mLen(0)
{
	Init();
	ConvertUsingInternalBuffer( inUStr, tslen(inEnd - inUStr) );
}


#pragma mark -


/**********************************************************************************************/
StFromUTF16::StFromUTF16( 
	I_Converter* 	inConverter )
:
	ConverterBase( inConverter ),
	mAStr(nullptr),
	mLen(0)
{
	Init();
}


/**********************************************************************************************/
StFromUTF16::StFromUTF16( 
	const String& 	inStr,
	I_Converter* 	inConverter )
:
	ConverterBase( inConverter ),
	mAStr(nullptr),
	mLen(0)
{
	Init();
	ConvertUsingInternalBuffer( inStr.c_str(), inStr.length() );
}


/**********************************************************************************************/
StFromUTF16::StFromUTF16( 
	const UChar* 	inUStr, 
	tslen 			inLen,
	I_Converter* 	inConverter )
:
	ConverterBase( inConverter ),
	mAStr(nullptr),
	mLen(0)
{
	Init();
	ConvertUsingInternalBuffer( inUStr, inLen );
}


/**********************************************************************************************/
StFromUTF16::StFromUTF16( 
	const UChar* 	inUStr, 
	const UChar* 	inEnd,
	I_Converter* 	inConverter )
:
	ConverterBase( inConverter ),
	mAStr(nullptr),
	mLen(0)
{
	Init();
	ConvertUsingInternalBuffer( inUStr,  tslen(inEnd - inUStr) );
}


#pragma mark -


/**********************************************************************************************/
StFromUTF16::~StFromUTF16( void ) 
{
	Free();
}


/**********************************************************************************************/
void StFromUTF16::Init( void ) 
{
	*mLocal = 0;
}


#pragma mark -


/**********************************************************************************************/
void StFromUTF16::Convert( 
	const UChar*	inUStr, 
	tslen			inLen )
{
	Free();

	if( !inUStr )
		return;

	ConvertUsingInternalBuffer( inUStr, inLen );
}


/**********************************************************************************************/
void StFromUTF16::Convert( 
	const UChar*	inUStr, 
	const UChar* 	inEnd )
{
	Free();

	if( !inUStr )
		return;

	ConvertUsingInternalBuffer( inUStr, tslen(inEnd - inUStr) );
}


/**********************************************************************************************/
void StFromUTF16::Convert( 
	const String&	inStr )
{
	Convert( inStr.c_str(), inStr.length() );
}


#pragma mark -


/**********************************************************************************************/
void StFromUTF16::ConvertUsingInternalBuffer( 
	const UChar* 	inUStr, 
	tslen 			inLen )
{
	if( !inUStr )
		return;

	mLen = (inLen != -1) ? inLen : vu_strlen( inUStr );

	vint32 NewLen	  = 0;
	vint32 SrcLen	  = mLen;

	try
	{
		// Set buffer pointer.
        vint32 TargetLen = SrcLen * mMaxCharSize;
        vint32 BuffSize  = TargetLen + 1 * mMaxCharSize;

		mAStr = (BuffSize > LOCAL_LEN) ? new char[BuffSize] : mLocal;
		UErrorCode Error = U_ZERO_ERROR;

		// Try in the loop to convert the string.
		while( 1 )
		{
			NewLen = mConverter->fromUChars( mAStr, BuffSize,
										inUStr, SrcLen, &Error );
			if( U_SUCCESS(Error) )
			{
				mLen = NewLen;
				break;
			}
			else
			{
				if( Error == U_BUFFER_OVERFLOW_ERROR )
				{
					// Not enough buffer size, correct it.
					if( mAStr != mLocal )
					{
						delete [] mAStr;
						mAStr = nullptr;
					}
					
					TargetLen = NewLen;
					BuffSize  = TargetLen + 1 * mMaxCharSize;

					mAStr = (BuffSize > LOCAL_LEN) ? new char[BuffSize] : mLocal;
					Error = U_ZERO_ERROR;
				}
				else
				{
					// FIXME: provide correct error code.
					THROW_ERROR_INTERNAL( ERR_INTERNAL_ERROR );
				}
			}
		}
	}
	catch(...)
	{
		if( mAStr != mLocal )
		{
			delete [] mAStr;
			mAStr = nullptr;
		}
		
		throw;
	}

    mAStr[mLen] = '\0';
}


/**********************************************************************************************/
void StFromUTF16::Free( void ) 
{
	if( mAStr != mLocal )
	{
	 	delete [] mAStr;
		mAStr = nullptr;
		mLen = 0;
	}
}


#pragma mark -
#pragma mark ====== StToUTF16 ======= 


/**********************************************************************************************/
StToUTF16::StToUTF16( 
	vuint32 		inToAllocate,
	const char* 	inEncoding )
:
	ConverterBase( inEncoding )
{
	fixme_argused1(inToAllocate); //???
	Init();
}


/**********************************************************************************************/
StToUTF16::StToUTF16( 
	const char* 	inAStr, 
	tslen 			inLen,
	const char* 	inEncoding )
:
	ConverterBase( inEncoding )
{
	Init();
	ConvertUsingInternalBuffer( inAStr, inLen );
}


/**********************************************************************************************/
StToUTF16::StToUTF16( 
	const char* 	inAStr, 
	const char*		inEnd,
	const char* 	inEncoding )
:
	ConverterBase( inEncoding )
{
	Init();
	ConvertUsingInternalBuffer( inAStr, tslen(inEnd - inAStr) );
}


#pragma mark -


/**********************************************************************************************/
StToUTF16::StToUTF16( 
	vuint32 		inToAllocate,
	I_Converter* 	inConverter )
:
	ConverterBase( inConverter )
{
	fixme_argused1(inToAllocate); //???
	Init();
}


/**********************************************************************************************/
StToUTF16::StToUTF16( 
	const char* 	inAStr, 
	tslen 			inLen,
	I_Converter* 	inConverter )
:
	ConverterBase( inConverter )
{
	Init();
	ConvertUsingInternalBuffer( inAStr, inLen );
}


/**********************************************************************************************/
StToUTF16::StToUTF16( 
	const char* 	inAStr, 
	const char*		inEnd,
	I_Converter* 	inConverter )
:
	ConverterBase( inConverter )
{
	Init();
	ConvertUsingInternalBuffer( inAStr, tslen(inEnd - inAStr) );
}


#pragma mark -


/**********************************************************************************************/
StToUTF16::~StToUTF16( void ) 
{
	Free();
}


/**********************************************************************************************/
void StToUTF16::Init( void ) 
{
	mUStr = nullptr;
	mLocal[ mLen = 0 ] = 0;
}


#pragma mark -


/**********************************************************************************************/
void StToUTF16::Convert( const char* inAStr, tslen inLen )
{
	Free();
	ConvertUsingInternalBuffer(inAStr, inLen);
}


#pragma mark -


/**********************************************************************************************/
void StToUTF16::ConvertUsingInternalBuffer( const char* inAStr, tslen inLen )
{
	if( !inAStr )
		return;

	tslen SrcLen = (inLen != - 1 ) ? inLen : tslen( strlen(inAStr) );

	UErrorCode Status	= U_ZERO_ERROR;

	vint32	NewLen		= 0;
	vint32	TargetLen	= SrcLen;

	// Set buffer pointer.
	mUStr = (TargetLen + 1) > LOCAL_LEN ? new UChar[TargetLen + 1] : mLocal;
	
	// Try in the loop to convert the string.
	try
	{
		while( 1 )
		{
			NewLen = mConverter->toUChars(mUStr, TargetLen + 1, inAStr, SrcLen, &Status);
			
			if( U_SUCCESS(Status) )
			{
				mLen = NewLen;
				break;
			}
			else
			{
				if( Status == U_BUFFER_OVERFLOW_ERROR )
				{
					// Not enought buffer size, correct it.
                    // NewLen contains required buffer size, so the second attempt must be successful.
					if( mUStr != mLocal )
					{
						delete [] mUStr;
						mUStr = nullptr;
					}
					
					mUStr = (NewLen + 1) > LOCAL_LEN ? new UChar[NewLen + 1] : mLocal;
					
					TargetLen = NewLen;
					Status = U_ZERO_ERROR;
				}
				else
				{
					// FIXME: provide correct error code.
					THROW_ERROR_INTERNAL( ERR_INTERNAL_ERROR );
				}
			}
		}
	}
	catch(...) // not enough RAM ? wow ...
	{
		if( mUStr != mLocal )
		{
			delete [] mUStr;
			mUStr = nullptr;
		}

		throw;
	}
}


/**********************************************************************************************/
void StToUTF16::Free( void ) 
{
	if( mUStr != mLocal )
	{
	 	delete [] mUStr;
		mUStr = nullptr;
		mLen = 0;
	}
}


/**********************************************************************************************/
FBL_End_Namespace
