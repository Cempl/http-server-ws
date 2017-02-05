/**********************************************************************************************/
/* FBL_Util_PathA.cpp	 	                                               					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_Disk_Location.h>
#include <VShared/FBL/publ/Utilities/FBL_Util_PathA.h>

 
/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
const char* ExtractNameFromFullPath( const char* inFullPath, char inDelim )
{
	char* p = nullptr;

	if( inFullPath )
		p = (char*) strrchr( inFullPath, inDelim ? inDelim : I_Disk_Location::sPathDelimiterA );
		
	return p ? (p + 1) : inFullPath;
}


/**********************************************************************************************/
void ExtractDirectoryFromPath( const char inPath[], char* outDir )
{
	const char* pAppName = ExtractNameFromFullPath( inPath );	
	size_t len = pAppName - inPath;
	strncpy(outDir, inPath, len);
	outDir[len] = '\0';
}


/**********************************************************************************************/
const char* ExtractExtFromFileName( const char* inNameWithExt )
{
	if( const char* p = strstr(inNameWithExt, ".") )
		return p + 1;
	else
		return inNameWithExt;
}


/**********************************************************************************************/
char* ExtractBaseFromFileName( const char* inFileName, char* outBase )
{
	if( const char* p = strstr(inFileName, ".") )
	{
		int len = static_cast<int>(p - inFileName);
		strncpy(outBase, inFileName, len);
		outBase[len] = '\0';
		
		return outBase;
	}
	else
	{
		return nullptr;
	}
}


/**********************************************************************************************/
void ExtractBaseNameFromFullPath( const char inPath[], char* outBase )
{
	const char* pFileName = ExtractNameFromFullPath( inPath );
	const char* p = strstr(pFileName, ".");
    
    size_t len = (p)   ? static_cast<size_t>( p - pFileName ) 
                    : strlen( pFileName );
	
    strncpy(outBase, pFileName, len);
	outBase[len] = '\0';
}


/**********************************************************************************************/
void SplitHostPort( char* inpSrc, MemPtr<char>* outpHost, vint32& outPort )
{
	// Caller have to allocate strlen(inpSrc) + 1 bytes for outpHost.
	//
	FBL_CHECK( inpSrc );
	FBL_CHECK( strlen(inpSrc) == strlen(*outpHost) );
	FBL_CHECK( strlen(inpSrc) + 1 == (*outpHost).get_Size() );

	size_t buffSize = (*outpHost).get_Size();
	memset( (*outpHost).begin(), 0 , buffSize );
	
	// Looking for the last position of ':' in inpString
	char* pSrcDelim = (char*) strrchr( inpSrc, ':' );
	
	if( pSrcDelim )
	{
		memcpy( (*outpHost).begin(), inpSrc, pSrcDelim - inpSrc );
		outPort = atol( pSrcDelim + 1 );
	}
	else 
	{
		memcpy( (*outpHost).begin(), inpSrc, buffSize );
	}
}


/**********************************************************************************************/
FBL_End_Namespace
