/**********************************************************************************************/
/* FBL_W2U.cpp                                                      						  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Util_classes/FBL_W2U.h>
#include <VShared/FBL/publ/Util_classes/FBL_String.h>
#include <VShared/FBL/publ/Util_Strings/FBL_ustdio.h>

#include <wchar.h>


/**********************************************************************************************/
/*#if defined(__MWERKS__)
size_t wcslen(const wchar_t *s);
size_t wcslen(const wchar_t *s)
{
    size_t n = 0;
    while ( *s++ )
        n++;

    return n;
}
#endif // __MWERKS__
*/

/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
StW2U::StW2U( const wchar_t* inWStr, tslen inLen )
:	
	mUStr( nullptr ),
	mLen( 0 )
{
	Convert(inWStr, inLen);
}


/**********************************************************************************************/
void StW2U::Convert( const wchar_t* inWStr, tslen inLen )
{
#if FBL_WIN

	mUStr = (UChar*)inWStr;
	mLen  = ( inLen == -1 ? (vint32) std::wcslen(inWStr) : inLen );

#else

	tslen len = (vint32) (inLen == -1 ? wcslen(inWStr) : inLen);
	mUStr = new UChar[len + 1];

	UErrorCode error = U_ZERO_ERROR;
	vu_strFromWCS(mUStr, len + 1, (vint32*)&mLen, inWStr, len, &error);
	FBL_CHECK(error == U_ZERO_ERROR);

#endif //FBL_WIN
}


/**********************************************************************************************/
StW2U::~StW2U()
{
#if !FBL_WIN
	delete [] mUStr;
#endif
}


/**********************************************************************************************/
const UChar* StW2U::u_str( void )
{
	return mUStr;
}


/**********************************************************************************************/
StU2W::StU2W( const UChar* inUStr, tslen inLen )
{
	Convert(inUStr, inLen);
}


/**********************************************************************************************/
StU2W::StU2W( const String& inStr )
{
	Convert(inStr.c_str(), inStr.length());
}


/**********************************************************************************************/
void StU2W::Convert( const UChar* inUStr, tslen inLen )
{
#if FBL_WIN

	mWStr = (UChar*)inUStr;
	mLen  = (inLen == -1 ? (vint32)std::wcslen(inUStr) : inLen);

#else

	tslen len = (inLen == -1 ? vu_strlen(inUStr) : inLen);
	mWStr = new wchar_t[len + 1];

	UErrorCode error = U_ZERO_ERROR;
	vu_strToWCS(mWStr, len + 1, (vint32*)&mLen, inUStr, len, &error);
	FBL_CHECK(error == U_ZERO_ERROR);

#endif //FBL_WIN
}


/**********************************************************************************************/
StU2W::~StU2W()
{
#if !FBL_WIN
	delete [] mWStr;
#endif
}


/**********************************************************************************************/
const wchar_t* StU2W::w_str( void )
{
	return mWStr;
}


/**********************************************************************************************/
FBL_End_Namespace


