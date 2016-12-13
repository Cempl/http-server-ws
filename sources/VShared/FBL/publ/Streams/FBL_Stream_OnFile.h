/**********************************************************************************************/
/* FBL_Stream_OnFile.h																		  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_Stream_OnFile_h 
	#define _FBL_Stream_OnFile_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Streams/FBL_File_Stream_T.h>
#include <VShared/FBL/publ/Streams/FBL_Text_Stream.h>
#include <VShared/FBL/publ/Streams/FBL_Binary_Stream.h>
#include <VShared/FBL/publ/Util_Strings/FBL_UnicodeFileSignature.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
typedef File_Stream_T<Binary_Stream> Binary_Stream_OnFile;


/**********************************************************************************************/
class Text_Stream_OnFile : public File_Stream_T<Text_Stream>
{
	public://///////////////////////////////////////////////////////////////////////////////////

		typedef File_Stream_T<Text_Stream> TBase;

	public://///////////////////////////////////////////////////////////////////////////////////

							Text_Stream_OnFile( I_File_Ptr inFile );

virtual flength				Seek( flength inOffset, ESeekMode inMode ) override;
	

		I_File_Ptr			get_File( void );


	private:////////////////////////////////////////////////////////////////////////////////////

		vuint32				mSignatureSize;	///< Size of the file's signature in bytes
};


/**********************************************************************************************/
// We cannot use SMART_CLASS() macro since the class 
// is defined through typedef keyword instead of class.
//
FBL_SMART_PTR(Binary_Stream_OnFile);
FBL_SMART_PTR(Text_Stream_OnFile);


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_Stream_OnFile_h


