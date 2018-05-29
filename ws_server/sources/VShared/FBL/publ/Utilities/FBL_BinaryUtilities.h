/**********************************************************************************************/
/* FBL_BinaryUtilities.h                                                        			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_BinaryUtilities_h
	#define _FBL_BinaryUtilities_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
class String;
SMART_INTERFACE(I_Disk_Location);
SMART_INTERFACE(I_Value);


/**********************************************************************************************/
FBL_SHARED_EXP I_Value_Ptr Picture_FromFile_ToValue(
	I_Disk_Location_Ptr inLocation,
	flength&			outLength );


/**********************************************************************************************/
FBL_SHARED_EXP I_Value_Ptr Binary_FromFile_ToValue(
	I_Disk_Location_Ptr inLocation,
	flength&			outLength );


/**********************************************************************************************/
FBL_SHARED_EXP String Binary_FromFile_ToString(
	I_Disk_Location_Ptr inLocation );


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_BinaryUtilities_h

