/**********************************************************************************************/
/* FBL_Util_FileExt.cpp	                                                					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Util_Paths/FBL_Util_FileExt.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_Location.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Disk_Location.h>
#include <VShared/FBL/publ/Util_classes/FBL_String.h>
#include <VShared/FBL/publ/Util_classes/FBL_StringConversion.h>
#include <VShared/FBL/publ/Util_Strings/FBL_ustdio.h>
#include <VShared/FBL/publ/Util_classes/FBL_ResourceManager.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
void SetVolumeDescriptionExt( const UChar* inExt );
void SetVolumeDataExt( const UChar* inExt );
void SetVolumeIndexExt( const UChar* inExt );
void SetVolumeBlobsExt( const UChar* inExt );

void SetVolumeDescriptionExt( const char* inExt );
void SetVolumeDataExt( const char* inExt );
void SetVolumeIndexExt( const char* inExt );
void SetVolumeBlobsExt( const char* inExt );


/**********************************************************************************************/
#pragma mark -
#pragma mark === MacOS Variables ====

#if FBL_MAC

const OSType	gSignature 				= 'VALA';

const OSType	gDescription 			= 'Vdsc';
const OSType	gMainDataFile 			= 'Vdat';
const OSType	gBlobsDataFile 			= 'Vblb';
const OSType	gIndexDataFile 			= 'Vind';
const OSType	gTmpDataFile 			= 'Vtmp';


void GetCreator( char*& outStr )
{
	OSType signature = ResourceManager::get_ResourceManager()->get_Signature();
	char* pS = (char*) &signature;

	#if FBL_BIG_ENDIAN
		outStr[0] = pS[0];
		outStr[1] = pS[1];
		outStr[2] = pS[2];
		outStr[3] = pS[3];
	#else
		outStr[0] = pS[3];
		outStr[1] = pS[2];
		outStr[2] = pS[1];
		outStr[3] = pS[0];
	#endif // FBL_BIG_ENDIAN	
}

void SetCreator( const char* inStr )
{
	OSType signature = ResourceManager::get_ResourceManager()->get_Signature();
	char* pS = (char*) &signature;

	#if FBL_BIG_ENDIAN
		pS[0] = inStr[0];
		pS[1] = inStr[1];
		pS[2] = inStr[2];
		pS[3] = inStr[3];
	#else
		pS[0] = inStr[3];
		pS[1] = inStr[2];
		pS[2] = inStr[1];
		pS[3] = inStr[0];
	#endif // FBL_BIG_ENDIAN	
}

#endif // FBL_MAC


/**********************************************************************************************/
#pragma mark -
#pragma mark === Methods ====


/**********************************************************************************************/
void SetExtensions( 
	const UChar* inVDB, 
	const UChar* inDAT, 
	const UChar* inBLB, 
	const UChar* inIND )
{
	SetVolumeDescriptionExt( inVDB );
	SetVolumeDataExt( inDAT );
	SetVolumeBlobsExt( inBLB );	
	SetVolumeIndexExt( inIND );
}
	

/**********************************************************************************************/
void SetExtensions( 
	const char*  inVDB, 
	const char*  inDAT, 
	const char*  inBLB, 
	const char*  inIND )
{
	SetVolumeDescriptionExt( inVDB );
	SetVolumeDataExt( inDAT );
	SetVolumeBlobsExt( inBLB );	
	SetVolumeIndexExt( inIND );
}


#pragma mark -

/**********************************************************************************************/
// Allow easy return back default settings for file extensions.
//
void SetDefaultExtentions( void )
{
	String vdb(".vdb");
	String dat(".dat");
	String ind(".ind");
	String blb(".blb");
	String tmp(".tmp");
	
	SetVolumeDescriptionExt( vdb.c_str() );
    SetVolumeDataExt( dat.c_str() );
    SetVolumeIndexExt( ind.c_str() );
    SetVolumeBlobsExt( blb.c_str() );

#if FBL_REQUIRE_LOCK_FILE
	String lck(".lck");
	ResourceManager::get_ResourceManager()->put_LockDataFileExt( lck.c_str() );
#endif // FBL_REQUIRE_LOCK_FILE
}


/**********************************************************************************************/
void SetVolumeDescriptionExt( const UChar* inExt )
{
	ResourceManager::get_ResourceManager()->put_DescriptionExt( inExt );
}


/**********************************************************************************************/
void SetVolumeDataExt( const UChar* inExt )
{
	ResourceManager::get_ResourceManager()->put_MainDataFileExt( inExt );
}


/**********************************************************************************************/
void SetVolumeIndexExt( const UChar* inExt )
{
	ResourceManager::get_ResourceManager()->put_IndexDataFileExt( inExt );
}


/**********************************************************************************************/
void SetVolumeBlobsExt( const UChar* inExt )
{
	ResourceManager::get_ResourceManager()->put_BlobsDataFileExt( inExt );
}


#pragma mark -


/**********************************************************************************************/
void SetVolumeDescriptionExt( const char* inExt )
{
	StToUTF16 st(inExt);
	SetVolumeDescriptionExt( st.c_str() );
}

/**********************************************************************************************/
void SetVolumeDataExt( const char* inExt )
{
	StToUTF16 st(inExt);
	SetVolumeDataExt( st.c_str() );
}

/**********************************************************************************************/
void SetVolumeIndexExt( const char* inExt )
{
	StToUTF16 st(inExt);
	SetVolumeIndexExt( st.c_str() );
}

/**********************************************************************************************/
void SetVolumeBlobsExt( const char* inExt )
{
	StToUTF16 st(inExt);
	SetVolumeBlobsExt( st.c_str() );
}

#pragma mark -


/**********************************************************************************************/
// Returns TRUE if OS require extention for vdb file.
// MacOS will return false.
//
bool ExtRequired( void )
{
#if FBL_MAC

	bool ExtRequired = false;	
//	if( UEnvironment::GetOSVersion() >= 0x00001000 )
//		ExtRequired = true;

#else // FBL_MAC

	bool ExtRequired = true;		

#endif // FBL_MAC

	return ExtRequired;
}


/**********************************************************************************************/
// This function get the location of the first DESC file.
// DESC file may not exists yet.
//
// We need build Locations for all rest volumes of database AND may be correct the extension 
// of DESC file (if user have not specify it, but OS require it then we add ext).
//
// The idea of algorithm is next:
//	* we get from DESC location its parent folder.
//	* then we ask the parent Location to build new child location with name as we need.
//
void MakeDbFileNames(
	const String&	inSpecName,
	String*			outVdbName,
	String*			outDatName,
	String*			outBlbName,
	String*			outIndName,
	String*			outTmpName,
	String*			outLockName )
{
	bool SpecHasExt = false;
	bool VSP = false;

	vint32 DotPos = inSpecName.reverseFind( UChar('.') );	
	
	if( DotPos != -1 )
	{
		String ext = inSpecName.right( inSpecName.length() - DotPos );

		// Tmp solution. If we see special "vsp" extension it means it is VProject database
		// We need to get "db_name.vsp" instead of "db_name.vsp.vdb"
		if( ext.caseCompare(".vsp", vuint32(-1)) == 0 )
		{
			VSP = true;
			SetExtensions( "vsp", "", "", "" );
		}

		if( !ext.isEmpty() && ext.caseCompare(
								ResourceManager::get_ResourceManager()->get_DescriptionExt(),
								vuint32(-1)) == 0 )
			SpecHasExt = true;
		else
			DotPos = -1; // may be we have found DOT, but this is part of name.
	}
	
	String justName( inSpecName.c_str(), DotPos );

	if( outVdbName )
	{
		if( SpecHasExt || ExtRequired() )
		{
			*outVdbName = justName + ResourceManager::get_ResourceManager()->get_DescriptionExt();
		}
		else
		{
			*outVdbName = justName;			
		}
	}
	
	if( outDatName )	*outDatName = justName + ResourceManager::get_ResourceManager()->get_MainDataFileExt();
	if( outBlbName )	*outBlbName = justName + ResourceManager::get_ResourceManager()->get_BlobsDataFileExt();
	if( outIndName )	*outIndName = justName + ResourceManager::get_ResourceManager()->get_IndexDataFileExt();
	if( outTmpName )	*outTmpName = justName + ResourceManager::get_ResourceManager()->get_TmpDataFileExt();

#if FBL_REQUIRE_LOCK_FILE
	if( outLockName )	*outLockName = justName + ResourceManager::get_ResourceManager()->get_LockDataFileExt();
#else
    argused1( outLockName );
#endif // FBL_REQUIRE_LOCK_FILE

	// Restore default extensions for database
	if( VSP )
		SetDefaultExtentions();
}


/**********************************************************************************************/
bool HasExtension( const String&	inFileName )
{
	return ( inFileName.reverseFind('.') > 0 ) ? true : false;
}


/**********************************************************************************************/
FBL_End_Namespace
