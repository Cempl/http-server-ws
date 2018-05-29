/**********************************************************************************************/
/* FBL_Notification.cpp																		  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>
#include "FBL_Notification.h"
#include <VShared/FBL/publ/Interfaces/FBL_I_Connectable.h>


/**********************************************************************************************/
FBL_Begin_Namespace

/**********************************************************************************************/
void Notification::From( I_IStream_Ptr inStream, bool inBlock )
{
	// No implementation because nobody need it
	argused2( inStream, inBlock );
}


/**********************************************************************************************/
void Notification::From( I_PacketRcv* inPacket, bool inBlock )
{
	argused1(inBlock);

	mTimeStamp = (time_t)inPacket->get_LLongParam();
	mInitiatorClientID = (size_t) inPacket->get_LLongParam();
	inPacket->get_StringParam( mChannel );
	inPacket->get_StringParam( mPayLoad );
	mIsSystem = inPacket->get_BoolParam();
	mPriority = (ENotificationPriority)inPacket->get_UShortParam();
	
}


/**********************************************************************************************/
void Notification::To( I_OStream_Ptr inStream, bool inBlock ) const
{
	// No implementation because nobody need it
	argused2( inStream, inBlock );
}


/**********************************************************************************************/
void Notification::To( I_PacketSnd*  inPacket, bool inBlock ) const
{
	argused1(inBlock);
	FixParamCount fixParamCount( inPacket );

	inPacket->put_LLongParam( (vint64)mTimeStamp );
	inPacket->put_LLongParam( (vint64)mInitiatorClientID );
	inPacket->put_StringParam( mChannel );
	inPacket->put_StringParam( mPayLoad );
	inPacket->put_BoolParam( mIsSystem );
	inPacket->put_UShortParam( (vuint16)mPriority );
}


/**********************************************************************************************/
FBL_End_Namespace

