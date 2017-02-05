/**********************************************************************************************/
/* FBL_Util_Path_Win.cpp 	                                               					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Utilities/FBL_Util_Path.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
#if FBL_WIN

/*void MakePathInTheSameFolder( FILE_PATH* inOtherFile, FILE_PATH* outPath, char* inFileName )
{
	vuint32 len = ::strlen( inOtherFile );
	char* e = inOtherFile + len;
	
	while( *e != '/' && inOtherFile < e )	
		e--;
	
	if( inOtherFile < e ) // We have found delimiter, so this is some kind of path.
	{
		// Now e points on the latest delimiter. We must copy string including it
		vuint32 offset =  vuint32(e - inOtherFile + 1);
		memcpy( outPath, inOtherFile, offset );

		// append new name to the new path.
		::strcpy( outPath + offset, inFileName );
	}
	else // this is simple file name. Just copy to outPath the inNewName
	{
		::strcpy( outPath, inFileName );
	}
} */


#endif // FBL_WIN


/**********************************************************************************************/
FBL_End_Namespace
