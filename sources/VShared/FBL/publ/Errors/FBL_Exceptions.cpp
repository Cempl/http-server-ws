/**********************************************************************************************/
/* FBL_Exceptions.cpp                                                      					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Errors/FBL_Exceptions.h>

#include <VShared/FBL/publ/Util_classes/FBL_String.h>
#include <VShared/FBL/publ/Util_classes/FBL_String.h>
#include <VShared/FBL/publ/Util_Strings/FBL_ustdio.h>

#include <VShared/FBL/publ/Util_classes/FBL_StringConversion.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_Connection.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
void ThrowInternalError( 
	ERROR_TYPE	inErrorCode,
	const char* inFile, 
	const char* inFunc, 
	vuint32		inLine,
	const char* inMsg )
{
	String desc( "FILE: " );
	desc << inFile << " FUNCTION: " << inFunc << " LINE: " << inLine;
	throw xInternalError( inErrorCode, desc.getBufferA(), inMsg );
}


/**********************************************************************************************/
void ThrowInternalError( 
	ERROR_TYPE	inErrorCode,
	const char* inFile, 
	const char* inFunc, 
	vuint32		inLine,
	const UChar* inMsg )
{
	String desc( "FILE: " );
	desc << inFile << " FUNCTION: " << inFunc << " LINE: " << inLine;
	throw xInternalError( inErrorCode, desc.c_str(), inMsg );
}


/**********************************************************************************************/
void ThrowInternalError( 
	ERROR_TYPE	inErrorCode,
	const char* inFile, 
	const char* inFunc, 
	vuint32		inLine,
	const UChar* inMsg1,
	const UChar* inMsg2,
	const UChar* inMsg3 )
{
	String desc( "FILE: " );
	desc << inFile << " FUNCTION: " << inFunc << " LINE: " << inLine;
	throw xInternalError( inErrorCode, desc.c_str(), inMsg1, inMsg2, inMsg3 );
}


/**********************************************************************************************/
FormatErrorMessagePtr gpFormatErrorMessage;


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

	return ( pRight->mCode == inErrorType ) ? pRight : nullptr;
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
xException::xException( 
	ERROR_TYPE inErrorCode, 
	const UChar* inArg1,
	const UChar* inArg2,
	const UChar* inArg3,
	const UChar* inArg4)
:
	mErrorCode( inErrorCode ),
	mpConnection(nullptr)
{
//	DebugStr( "\p" );

	mArgs[0] = inArg1 ? new String(inArg1) : nullptr;
	mArgs[1] = inArg2 ? new String(inArg2) : nullptr;
	mArgs[2] = inArg3 ? new String(inArg3) : nullptr;
	mArgs[3] = inArg4 ? new String(inArg4) : nullptr;
}


/**********************************************************************************************/
xException::xException( 
	ERROR_TYPE	inErrorCode,
	const char* inArg1,
	const char* inArg2,
	const char* inArg3,
	const char* inArg4)
:
	mErrorCode( inErrorCode ),
	mpConnection(nullptr)
{
//	DebugStr( "\p" );

	mArgs[0] = inArg1 ? new String(inArg1) : nullptr;
	mArgs[1] = inArg2 ? new String(inArg2) : nullptr;
	mArgs[2] = inArg3 ? new String(inArg3) : nullptr;
	mArgs[3] = inArg4 ? new String(inArg4) : nullptr;
}


/**********************************************************************************************/
xException::xException( 
	ERROR_TYPE inErrorCode,
	const String& inFinalMessage,
	I_Connection* inConn )
:
	mErrorCode( inErrorCode ),
	mpConnection(inConn)
{
	if( mpConnection )
		mpConnection->AddRef();

	mArgs[0] = inFinalMessage.isEmpty() ? nullptr : new String(inFinalMessage);
	mArgs[1] = nullptr;
	mArgs[2] = nullptr;
	mArgs[3] = nullptr;
}


/**********************************************************************************************/
xException::xException( const xException& inExc )
{
	mpConnection = inExc.mpConnection;
	if( mpConnection )
		mpConnection->AddRef();

	mErrorCode = inExc.mErrorCode;
	mArgs[0] = inExc.mArgs[0] ? new String(*inExc.mArgs[0]) : nullptr;
	mArgs[1] = inExc.mArgs[1] ? new String(*inExc.mArgs[1]) : nullptr;
	mArgs[2] = inExc.mArgs[2] ? new String(*inExc.mArgs[2]) : nullptr;
	mArgs[3] = inExc.mArgs[3] ? new String(*inExc.mArgs[3]) : nullptr;
}


/**********************************************************************************************/
xException& xException::operator =( const xException& inExc )
{
	Clear();

	mErrorCode = inExc.mErrorCode;
	mArgs[0] = inExc.mArgs[0] ? new String(*inExc.mArgs[0]) : nullptr;
	mArgs[1] = inExc.mArgs[1] ? new String(*inExc.mArgs[1]) : nullptr;
	mArgs[2] = inExc.mArgs[2] ? new String(*inExc.mArgs[2]) : nullptr;
	mArgs[3] = inExc.mArgs[3] ? new String(*inExc.mArgs[3]) : nullptr;
	
	return *this;
}


/**********************************************************************************************/
xException::~xException( void ) throw()
{
	Clear();
}


/**********************************************************************************************/
void xException::Clear( void )
{
	ForgetObject( mArgs[0] );
	ForgetObject( mArgs[1] );
	ForgetObject( mArgs[2] );
	ForgetObject( mArgs[3] );

	if( mpConnection )
		mpConnection->Release();
}


#pragma mark -


/**********************************************************************************************/
String xException::get_ErrorString( void ) const
{
	String res( "UNKNOWN" );
	
 	if( mpConnection )
	{
		// Check whether we have the last error string desription.
		// It may be sent by the server on fatal connection error.
		if( mArgs[0] )
		{
			res = *mArgs[0];
		}
		else
		{
			// Request the VServer for the last error description.
			res = mpConnection->get_ErrorDescription();
		}
	}
	else
	{
		if( gpFormatErrorMessage )
		{
			ErrorEntry* pE = FBL::FindErrorEntry( mErrorCode );

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
const String& xException::get_Param( vuint32 inIndex ) const 
{
	FBL_CHECK(inIndex <= 4);
	switch (inIndex)
	{
		case 1:
		case 2:
		case 3:
		case 4:
			return mArgs[inIndex - 1] ? *mArgs[inIndex - 1] : String::sEmpty();
			
		default:
			return String::sEmpty();
	};
}


#pragma mark -


/**********************************************************************************************/
// This function search for %S
//
UChar* FormatErrorMessage_LocateS( 
	const UChar* inFormatString );


UChar* FormatErrorMessage_LocateS( 
	const UChar* inFormatString )
{
	UChar* pProcentChar = (UChar*) inFormatString;
	
	do
	{
		pProcentChar = vu_strchr( pProcentChar, UChar('%') );
		
		if( pProcentChar == nullptr )
			return nullptr;
		else
		{
			if( *(pProcentChar+1) == UChar('S') )
				return pProcentChar;
			else
				++pProcentChar;
		}

	}while( true );
	
	return nullptr;	
}


/**********************************************************************************************/
// This function implement simplest behavior of printf() for UChar*
//
void FormatErrorMessage_printf( 
		UChar*          ioBuffer,
		vuint32         inBufferSize, 
		const UChar*    inFormatString,
		const UChar*    inStr1,
		const UChar*    inStr2,
		const UChar*    inStr3,
		const UChar*    inStr4 )
{
	// to be able use loop, we pack parameter into array.
	const UChar* ptrs[4] = { inStr1, inStr2, inStr3, inStr4 };
	
	UChar* pd = ioBuffer;
	UChar* ps = (UChar*) inFormatString;
	
	vuint32 i = 0;
	
	do{
		// we find next %U in the format string:
		// pE point on found % or after end of format string.
		UChar* pPS = FormatErrorMessage_LocateS( ps );
		if( pPS )
		{
			tslen ToCopy = tslen(pPS - ps);
			ToCopy = FBL::Min( ToCopy, (tslen)inBufferSize  );

			vu_strncpy( pd, ps, ToCopy );
			pd += ToCopy;
			ps += ToCopy;			
			inBufferSize -= ToCopy;
			if( inBufferSize == 0 )
				break;
			
			if( *pPS == UChar('%') )
			{
				const UChar* pstr = ptrs[i++];				
				if( pstr )
				{
					vuint32 plen = (vuint32) vu_strlen( pstr );
					plen = FBL::Min(plen, inBufferSize);

					vu_strncpy( pd, pstr, (vint32) plen );
					pd += plen;
					inBufferSize -= plen; 
				}
				else // just copy %S from format string.
				{
					vu_strncpy( pd, pPS, 2 );
					pd += 2;
					inBufferSize -= 2;
				}
				
				ps += 2;
			}
			else
			{
				// we have not found %U, so break loop.
				break;
			}
		}
		else // we have come to the end of format string.
		{
			vu_strncpy( pd, ps, (vint32) inBufferSize );
			break;
		}
		
	}while( inBufferSize );	
}


/**********************************************************************************************/
// RZ: This method is used only by V4MD now. Hmm.
//
const char* FormatErrorMessage( 
	ErrorEntry*  inErrorEntry, 
	const char* const inArgs[] )
{

// static members are the big trouble for MT since such data are common for all the threads.
// But this method is used by V4MD only...
//
static StFromUTF16 st((UChar*)nullptr);

static UChar arg1[256];
static UChar arg2[256];
static UChar arg3[256];
static UChar arg4[256];

	if( inArgs[0] )
		vu_strcpy(arg1, inArgs[0]);
	else
		arg1[0] = 0;

	if( inArgs[1] )
		vu_strcpy(arg2, inArgs[1]);
	else
		arg2[0] = 0;

	if( inArgs[2] )
		vu_strcpy(arg3, inArgs[2]);
	else
		arg3[0] = 0;

	if( inArgs[3] )
		vu_strcpy(arg4, inArgs[3]);
	else
		arg4[0] = 0;

	const UChar* args[] = {
		arg1,
		arg2,
		arg3,
		arg4
	};

	if( gpFormatErrorMessage )
	{
		const UChar* pStr = gpFormatErrorMessage(inErrorEntry, args);
		st.Convert( pStr, (const UChar*) nullptr );
	}

	return st.c_str();

}


/**********************************************************************************************/
FBL_End_Namespace
