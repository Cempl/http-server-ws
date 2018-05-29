/**********************************************************************************************/
/* FBL_CreateStream.cpp 	                                                   				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/File_OS/FBL_File_OS.h>
#include <VShared/FBL/publ/Streams/FBL_Stream_OnFile.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
I_IStream* CreateTextStream( I_Location_Ptr inLoc )
{	
	return new Text_Stream_OnFile( CreateTextFile(inLoc) );
}	


/**********************************************************************************************/
I_IStream* CreateTextStreamOnFile( I_File_Ptr inpFile )
{	
	return new Text_Stream_OnFile( inpFile );
}	


/**********************************************************************************************/
static I_File_Ptr PrepareTextFile( I_Location_Ptr inLoc )
{
	File_OS* pFileOS = new File_OS;	 // to get access to File_OS methods.
	I_File_Ptr pF = pFileOS;
 
	pFileOS->put_Location( inLoc );

	#if FBL_MAC
		pFileOS->put_MacType( 'TEXT' );
	#endif // FBL_MAC

	return pF;
}


/**********************************************************************************************/
I_File_Ptr CreateTextFile( I_Location_Ptr inLoc )
{
	#if TARGET_OS_IPHONE
	
		argused1( inLoc );
		return nullptr;
	
	#else
	
		I_File_Ptr pF = PrepareTextFile( inLoc );
		pF->Create();
		return pF;
	
	#endif // TARGET_OS_IPHONE
}	


/**********************************************************************************************/
I_File_Ptr CreateOrOpenTextFile( I_Location_Ptr inLoc )
{
	#if TARGET_OS_IPHONE
	
		argused1( inLoc );
		return nullptr;
	
	#else
	
		I_File_Ptr pF = PrepareTextFile( inLoc );
		if( pF->get_Exists() )
			pF->Open();
		else
			pF->Create();

		return pF;
	
	#endif // TARGET_OS_IPHONE
}


/**********************************************************************************************/
FBL_End_Namespace

