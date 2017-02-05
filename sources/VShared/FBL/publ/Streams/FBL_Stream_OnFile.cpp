/**********************************************************************************************/
/* FBL_Stream_OnFile.cpp                                                   					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Streams/FBL_Stream_OnFile.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
Text_Stream_OnFile::Text_Stream_OnFile( I_File_Ptr inFile )
	: TBase( inFile )
{
// еее
// FIXME
//	mSignatureSize = WriteUnicodeSignature( "UTF-16", inFile );

	// signature must be on start of the file.
	FBL_CHECK( inFile->get_Pos() == 0 );
	const vuint8* pSignature = Get_FileSignature_UTF_16( &mSignatureSize );	

	inFile->Write( (const char*) pSignature, mSignatureSize );
	inFile->Flush();
}


/**********************************************************************************************/
flength Text_Stream_OnFile::Seek( flength inOffset, ESeekMode inMode ) 
{
	// Correct offset
	flength Off = inOffset;
	if( inMode == kFromStart )
	{
		// Take into account the Unicode signature of file.
		Off += mSignatureSize;
	}

	return TBase::Seek( Off, inMode );
}


/**********************************************************************************************/
I_File_Ptr Text_Stream_OnFile::get_File( void )
{
	return mpFile;
}


/**********************************************************************************************/
FBL_End_Namespace


