/**********************************************************************************************/
/* FBL_EncryptionKey.h                                                     					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Util_classes/FBL_EncryptionKey.h>

#include <VShared/FBL/publ/Util_classes/FBL_String.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
EncryptionKey::EncryptionKey( 
	const vuint8* 	inKeyString, 
	vuint32 		inKeyLen )
:
	mLen( inKeyLen ),
	mKey( new vuint8[mLen] )
{
	memcpy( mKey, inKeyString, mLen );
}


/**********************************************************************************************/
EncryptionKey::EncryptionKey( 
	const EncryptionKey& inSrc )
:
	mLen( inSrc.mLen ),
	mKey( new vuint8[mLen] )
{
	memcpy( mKey, inSrc.mKey, mLen );
}


/**********************************************************************************************/
EncryptionKey::~EncryptionKey( void )
{
	if( mKey )
		delete [] mKey;
}


/**********************************************************************************************/
String EncryptionKey::ToString( void )
{
	String s;

	for( vuint32 i = 0; i < mLen; ++i )
		s += (char)mKey[i];

	return s;
}


/**********************************************************************************************/
FBL_End_Namespace
