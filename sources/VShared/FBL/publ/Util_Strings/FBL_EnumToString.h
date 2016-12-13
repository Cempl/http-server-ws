/**********************************************************************************************/
/* FBL_EnumToString.h																		  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_EnumToString_h
	#define _FBL_EnumToString_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Type.h>
#include <VShared/FBL/publ/Util_classes/FBL_DTFormat.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
FBL_SHARED_EXP const char* TypeCategory2String( TYPE_CATEGORY inTypeCategory );
FBL_SHARED_EXP const char* DatabaseMode2String( DbMode inMode );
FBL_SHARED_EXP const char* Bool2String( bool inValue );
FBL_SHARED_EXP const char* StorageType2String( EStorageType inStorageType );
FBL_SHARED_EXP const char* DateFormat2String( EDateFormat inDateFormat );
FBL_SHARED_EXP const char* DumpSortOrder2String( EDumpSortOrder inDumpSortedOrder );
FBL_SHARED_EXP const char* ColAttribValue2String( UColAttributeValue inValue );
FBL_SHARED_EXP const char* TableKind2String( ETableKind inTableKind );
FBL_SHARED_EXP const char* LinkType2String( LinkType inLinkType );
FBL_SHARED_EXP const char* OnDelete2String( EOnDeletion inValue );
FBL_SHARED_EXP const char* OnUpdate2String( EOnUpdate inValue );
FBL_SHARED_EXP const char* LinkKind2String( ELinkKind inValue );


/**********************************************************************************************/
FBL_SHARED_EXP TYPE_CATEGORY String2TypeCategory( const char* inStr );
FBL_SHARED_EXP DbMode String2DatabaseMode( const char* inStr );
FBL_SHARED_EXP EDateFormat String2DateFormat( const char* inStr );
FBL_SHARED_EXP EDumpSortOrder String2DumpSortOrder( const char* inStr );
FBL_SHARED_EXP UColAttributeValue String2ColAttribValue( const char* inStr );
FBL_SHARED_EXP EStorageType String2StorageType( const char* inStr );


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_memory_h
