/**********************************************************************************************/
/* FBL_Util_Path.cpp	 	                                               					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_Disk_Location.h>
#include <VShared/FBL/publ/Util_Strings/FBL_ustdio.h>
#include <VShared/FBL/publ/Utilities/FBL_Util_Path.h>

 
/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
const UChar* ExtractNameFromFullPath( const UChar* inFullPath, UChar inDelim  )
{
	UChar* p = nullptr;

	if( inFullPath )
		p = vu_strrchr( inFullPath, inDelim ? inDelim : I_Disk_Location::sPathDelimiterU );
	
	return p ? (p + 1) : inFullPath;
}

 
/**********************************************************************************************/
FBL_End_Namespace
