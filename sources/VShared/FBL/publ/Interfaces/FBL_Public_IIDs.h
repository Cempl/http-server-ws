/**********************************************************************************************/
/* FBL_Public_IIDs.h		                                                   				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/
// DO NOT INCLUDE THIS FILE DIRECTLY !
// It comes via FBL.h -> I_Unknonw.h
//
#ifndef _FBL_Public_IIDs_h 
	#define _FBL_Public_IIDs_h

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// Analog of COM's IID type.
// RZ: I use other name to avoid conflicts to MS COM type.
//
typedef vuint32 InterfaceID;			


/**********************************************************************************************/
// DESCRIPTION:
//
// This file contains IID for public interfaces.
// IID of Protected and Private interfaces are in FBL_Prot_IIDs.h and FBL_Priv_IIDs.h
// 
// We split interfaces into group by first letter.
// Each groupcan have 99 items total 
// Public Interfaces use 1-50, Protected 51-80, Private 81-99
//
// Numbers as 100, 200, 300 are not used (reserved ;-)
//
/**********************************************************************************************/
const vuint32 IID_I_Unknown 				= 1;

// A = 100
const vuint32 IID_I_Aspect 					= 100;
const vuint32 IID_I_AspectHost				= 101;

// B = 200

// C = 300
const vuint32 IID_I_Connectable				= 300 + 1;
const vuint32 IID_I_Connection				= 300 + 2;
const vuint32 IID_I_Cursor					= 300 + 3;
const vuint32 IID_I_ClientInfo				= 300 + 4;
const vuint32 IID_I_Compressor				= 350 + 5;

// D = 400
const vuint32 IID_I_Database				= 400 + 1;
const vuint32 IID_I_Disk_Location			= 400 + 2;
const vuint32 IID_I_DatabaseInfo			= 400 + 3;
const vuint32 IID_I_DataSource				= 400 + 4;
const vuint32 IID_I_DataSourceCursor		= 400 + 5;

// E = 500
const vuint32 IID_I_Error					= 500 + 1;
const vuint32 IID_I_Encryptable				= 500 + 2;

// F = 600
const vuint32 IID_I_Field	 				= 600 + 1;
const vuint32 IID_I_File	 				= 600 + 2;
const vuint32 IID_I_FldBlob 				= 600 + 3;
const vuint32 IID_I_FldObjectPtr			= 600 + 4;
const vuint32 IID_I_FldObjectsPtr			= 600 + 5;
const vuint32 IID_I_FldPicture				= 600 + 6;
const vuint32 IID_I_FldString				= 600 + 7;
const vuint32 IID_I_ForeignPlugin			= 600 + 8;
const vuint32 IID_I_FldApproximate			= 600 + 9;
const vuint32 IID_I_FldVariant 				= 600 + 10;

// G = 700

// H = 800

// I = 900
const vuint32 IID_I_IndexStyle				= 900 + 1;
const vuint32 IID_I_IStream 				= 900 + 2;
const vuint32 IID_I_Stream 					= 900 + 3;

// J = 1200

// K = 1100
const vuint32 IID_I_KeyValue   				= 1100 + 1;
const vuint32 IID_I_KeyValue_ForTable   	= 1100 + 2;
const vuint32 IID_I_KeyValue_ForLink   		= 1100 + 3;

// L = 1000
const vuint32 IID_I_Link					= 1000 + 1;
const vuint32 IID_I_Link2					= 1000 + 2;
const vuint32 IID_I_Location				= 1000 + 3;
const vuint32 IID_I_Localizable				= 1000 + 4;
const vuint32 IID_I_LogFile					= 1000 + 5;
const vuint32 IID_I_LicenceManager			= 1000 + 6;
const vuint32 IID_I_LicenceFile				= 1000 + 7;
const vuint32 IID_I_LinkOnValues			= 1000 + 8;
const vuint32 IID_I_Link2_WithOrder			= 1000 + 9;

// M = 1300

// N = 1400
const vuint32 IID_I_NotificationCenter		= 1400 + 1;

// O = 1500
const vuint32 IID_I_OStream					= 1500 + 1;

// P = 1600
const vuint32 IID_I_Property 				= 1600 + 1;
const vuint32 IID_I_PropertyContainer 		= 1600 + 2;

// Q = 1700

// R = 1800
const vuint32 IID_I_RecordBuffer			= 1800 + 1;
const vuint32 IID_I_RemoteProxy			 	= 1800 + 2;

// S = 1900
const vuint32 IID_I_Serializable			= 1900 + 1;
const vuint32 IID_I_Storage				 	= 1900 + 2;
const vuint32 IID_I_String				 	= 1900 + 3;
const vuint32 IID_I_StringFactory			= 1900 + 4;
const vuint32 IID_I_Server				 	= 1900 + 5;

//
const vuint32 IID_I_SqlDatabase			 	= 1900 + 5;
const vuint32 IID_I_SqlConnection			= 1900 + 6;
const vuint32 IID_I_SqliteDatabase			= 1900 + 7;
const vuint32 IID_I_SqliteStatement			= 1900 + 8;
const vuint32 IID_I_SqliteCursor			= 1900 + 9;
const vuint32 IID_I_SqlStatement			= 1900 + 10;


// T = 2000
const vuint32 IID_I_Table					= 2000 + 1;
const vuint32 IID_I_Task					= 2000 + 2;
const vuint32 IID_I_TaskManager				= 2000 + 3;
const vuint32 IID_I_Type					= 2000 + 4;
const vuint32 IID_I_Type_Composite			= 2000 + 5;
const vuint32 IID_I_Type_Enumerated			= 2000 + 6;
const vuint32 IID_I_Task_MT					= 2000 + 7;

// U = 2100

// V = 2200
const vuint32 IID_I_Value					= 2200 + 1;
const vuint32 IID_I_ValueBinary				= 2200 + 2;
const vuint32 IID_I_ValueDate				= 2200 + 3;
const vuint32 IID_I_ValueDateTime			= 2200 + 4;
const vuint32 IID_I_ValueTime				= 2200 + 5;
const vuint32 IID_I_ValueApproximate		= 2200 + 6;
const vuint32 IID_I_VReport					= 2200 + 7;
const vuint32 IID_I_VProject				= 2200 + 8;
const vuint32 IID_I_ValueFormater			= 2200 + 9;
const vuint32 IID_I_ValueCompound			= 2200 + 10;
const vuint32 IID_I_ValueVariant			= 2200 + 11;


// W = 2300

// X = 2400

// Y = 2500

// Z = 2600



/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_Public_IIDs_h
