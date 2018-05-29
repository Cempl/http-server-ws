/**********************************************************************************************/
/* FBL_EncryptionKey.h                                                     					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_EncryptionKey_h 
	#define _FBL_EncryptionKey_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Util_classes/FBL_String.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_CLASS(EncryptionKey);


/**********************************************************************************************/
class FBL_SHARED_EXP_CLASS EncryptionKey : public I_Unknown
{
 		IMPLEMENT_SINGLE_UNKNOWN( EncryptionKey )
 
	public://///////////////////////////////////////////////////////////////////////////////////
	 
							EncryptionKey( 
								const vuint8* 	inKeyString, 
								vuint32 		inKeyLen );

							EncryptionKey( 
								const EncryptionKey& inSrc );
	
							~EncryptionKey( void );

			String			ToString( void );

			vuint32			Size( void ) const { return mLen; }
			vuint8*			Key	( void ) const { return mKey; }	
							
	protected:////////////////////////////////////////////////////////////////////////////////// 
	
		vuint32				mLen;		 
		vuint8*				mKey;
};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_EncryptionKey_h


