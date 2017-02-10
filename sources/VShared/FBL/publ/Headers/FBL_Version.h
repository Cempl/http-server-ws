/**********************************************************************************************/
/* FBL_Version.h																			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                       					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_Version_h
    #define _FBL_Version_h
#pragma once

// NO NAMESPACE because macros here. 


/**********************************************************************************************/
#define kKernelVersionMajor					7	// 0x04
#define kKernelVersionMinor					0	// 0x00
#define kKernelVersionRevision				0	// 0x00

#define kKernelVersionMajorStr				"7"
#define kKernelVersionMinorStr				"0"
#define kKernelVersionRevisionStr			"0"

#define kSpecialBuildString					"b10"
 

/**********************************************************************************************/
// Added for 3.x
// BuildDate allows for engine to know WHEN it was built.
// This allows to check if a serial number is expired and should be rejected.
// This constant must be changed manually (!!!) at least for RELEASE builds.
// Its format "YYYY/MM/DD".
//
#define kBuildDate							"2017/01/25"


/**********************************************************************************************/
// DO NOT change this! Its FIXED date "2005/05/01".
//
#define kOldEmbeddedLicenseDate				"2005/05/01"


/**********************************************************************************************/
#define kLegalCopyright						"Copyright (C) Paradigma Software, Inc., 1998-2017"


/**********************************************************************************************/
#define kKernelVersionMinorAndRevision		0x00
#define kKernelVersionInternalRelease		0x00 


/**********************************************************************************************/
// AUTO: Glue them all together for easy comparison
//
#ifndef REZ
#define	kKernelVersion ((vuint32(kKernelVersionMajor) << 24) | (vuint32(kKernelVersionMinor & 0x0F) << 20) | (vuint32(kKernelVersionRevision & 0x0F)	<< 16) | (vuint32(kKernelVersionStage) <<  8) | (vuint32(kKernelVersionInternalRelease)))
#endif


/**********************************************************************************************/
//#if FBL_MAC
//#define kKernelVersionStage				betaStage
//#else
#define kKernelVersionStage					0x80
//#endif


/**********************************************************************************************/
// AUTO: This macro produces a full version of kernel as string: e.g.  "2.0.5" or "2.1"
// If last digit is zero it is not used.
//
#if kKernelVersionRevision == 0
#define	kVersionString		kKernelVersionMajorStr "." kKernelVersionMinorStr
#else
#define	kVersionString		kKernelVersionMajorStr "." kKernelVersionMinorStr "." kKernelVersionRevisionStr
#endif // kKernelVersionRevision == 0


/**********************************************************************************************/
// AUTO: This macro produces a full version of kernel as string including special build info:
//		 e.g. "2.0.5 b12". Note a space between numbers.
//
#define	kKernelVersionString				kVersionString " " kSpecialBuildString


/**********************************************************************************************/
// AUTO: This macro produces NON-string, but list of comma separated values.
//		 It is used on Windows by V4MD Visual project for example.
//
#define kKernelVersion_List_WIN  kKernelVersionMajor, kKernelVersionMinor, kKernelVersionRevision, 0


/**********************************************************************************************/
#endif // _FBL_Version_h


