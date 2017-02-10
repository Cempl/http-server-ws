/**********************************************************************************************/
/* FBL_BinaryUtilities.cpp                                                          		  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include "FBL_BinaryUtilities.h"

// VSHARED:
#include <VShared/FBL/publ/Values/FBL_Value_Binary.h>
#include <VShared/FBL/publ/File_OS/FBL_File_OS.h>
#include <VShared/FBL/publ/Utilities/FBL_ByteToHex.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
static I_Value_Ptr Binary_FromFile_ToValue_imp(
	I_Disk_Location_Ptr inLocation,
	flength&			outLength,
	VALUE_TYPE			inType )
{
	I_Value_Ptr pResult;
	
	I_File_Ptr file = new File_OS();
	file->put_Location(fbl_const_cast<I_Location>(inLocation));	
	file->Open();

#if FBL_WIN || FBL_MAC
	// http://www.valentina-db.com/bt/view.php?id=3063
	// Probably we should not bother of doc-files opened in MS-Word (not "flushed") 
	// before doing import from that file at all.
	// Or we need some common strategy to open file exclusive-for-reading on all OS
	// and apply it here.
	if( file->get_IsReadOnly() )
		FBL_Throw( xOSFileError( ERR_OS_FILE_BUSY, inLocation->get_FileName().c_str() ) );
#endif // FBL_WIN

	try
	{
		outLength = file->get_Length();
		MemPtr<char> FileData( (vuint32) outLength );
		// IS: 03.03.2008
		// http://valentina-db.com/bt/view.php?id=3056
		// Reading to the NULL memory provide assert on win and may get to the crash on Mac.
		if( outLength )
			file->Read( FileData, 0, (vuint32) outLength );

		switch( inType )
		{
			case kTypeFixedBinary:
				pResult = new Value_fixedbinary( (vuint32)outLength );
			break;
			
			case kTypeVarBinary:
				pResult = new Value_varbinary( (vuint32)outLength );
			break;

			case kTypePicture:
				pResult = new Value_picture( (vuint32)outLength );
			break;
			
			default:
				FBL_CHECK(false);
			break;
		}
		
		I_ValueBinary_Ptr pValueBinary = QI( pResult, I_ValueBinary );
		FBL_CHECK( pValueBinary );
		pValueBinary->put_Data( (vuint8*)FileData.begin(), (vuint32)outLength );
		
	}
	catch( ... ) 
	{
		file->Close();
		throw;
	}
	
	file->Close();
	
	return pResult;
}


#pragma mark -

/**********************************************************************************************/
I_Value_Ptr Picture_FromFile_ToValue( I_Disk_Location_Ptr inLocation, flength& outLength )
{
	return Binary_FromFile_ToValue_imp( inLocation, outLength, kTypePicture );
}


/**********************************************************************************************/
I_Value_Ptr Binary_FromFile_ToValue( I_Disk_Location_Ptr inLocation, flength& outLength )
{
	return Binary_FromFile_ToValue_imp( inLocation, outLength, kTypeFixedBinary );
}


#pragma mark -

/**********************************************************************************************/
String Binary_FromFile_ToString( I_Disk_Location_Ptr inLocation )
{
	String res;
	
	I_File_Ptr file = new File_OS();
	file->put_Location(fbl_const_cast<I_Location>(inLocation));	
	file->Open();

#if FBL_WIN || FBL_MAC
	// http://www.valentina-db.com/bt/view.php?id=3063
	// Probably we should not bother of doc-files opened in MS-Word (not "flushed") 
	// before doing import from that file at all.
	// Or we need some common strategy to open file exclusive-for-reading on all OS
	// and apply it here.
	if( file->get_IsReadOnly() )
		FBL_Throw( xOSFileError( ERR_OS_FILE_BUSY, inLocation->get_FileName().c_str() ) );
#endif // FBL_WIN

	try
	{
		flength fileLength = file->get_Length();
		MemPtr<vuint8> FileData( (vuint32) fileLength );
		if( fileLength )
			file->Read( (char*)FileData.begin(), 0, (vuint32) fileLength );

		res << DataToHexString(FileData, (vuint32)FileData.get_Size(), false, 0);
	}
	catch( ... ) 
	{
		file->Close();
		throw;
	}
	
	file->Close();
	
	return res;
}


/**********************************************************************************************/
FBL_End_Namespace
