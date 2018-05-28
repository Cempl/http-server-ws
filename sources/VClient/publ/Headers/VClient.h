/**********************************************************************************************/
/* VClient.h																				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                       					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _VClient_h
	#define _VClient_h
#pragma once


/**********************************************************************************************/
// This must be always defined for client builds
#ifndef FBL_CLIENT
#	define FBL_CLIENT 1
#endif // FBL_CLIENT


/**********************************************************************************************/
// FBL:
#include <VShared/FBL/publ/Interfaces/FBL_I_Connection.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Database.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Location.h>

// VSQL:
#include <VShared/VSQL/Interfaces/VSQL_I_SqlConnection.h>
#include <VShared/VSQL/Interfaces/VSQL_I_ClientInfo.h>
#include <VShared/VSQL/Interfaces/VSQL_I_DatabaseInfo.h>
#include <VShared/VSQL/Interfaces/VSQL_I_Server.h>
#include <VShared/VSQL/Interfaces/VSQL_I_SqlDatabase.h>
#include <VShared/VSQL/Interfaces/VSQL_I_VProject.h>
#include <VShared/VSQL/Interfaces/VSQL_I_SqliteDatabase.h>


/**********************************************************************************************/
// Define VCLIENT namespace
// 
FBL_Begin_Namespace

#if Uses_Namespace
#define VCLIENT						vclient
#define VCLIENT_Begin_Namespace		namespace FBL { namespace VCLIENT {
#define VCLIENT_End_Namespace			} }
#define VCLIENT_Using_Namespace		using namespace FBL::VCLIENT;
#else
#define VCLIENT						
#define VCLIENT_Begin_Namespace		
#define VCLIENT_End_Namespace		
#define VCLIENT_Using_Namespace		
#endif

FBL_End_Namespace


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
extern vuint32 gClientCacheSize;	// Defined in bytes


/**********************************************************************************************/
const vuint32 DEFAULT_CACHE_SIZE = 10 * 1024 * 1024;

const vuint32 MAX_DATABASE 		= 64;
const vuint32 MAX_HOSTNAME 		= 128;
const vuint32 MAX_USER_NAME 	= 32;
const vuint32 MAX_USER_PASSWORD = 64;
const vuint32 MAX_OPT_LEN 		= 64;
const vuint32 MAX_CONNSTR 		= 1024;
const vuint32 MAX_PATH_LEN 		= 512;

const vuint32 MAX_EVENTNAME_LEN	= MAX_DATABASE;
const vuint32 MAX_ACTION_LEN 	= MAX_CONNSTR;
const vuint32 MAX_COMMENT_LEN	= MAX_DATABASE;


/**********************************************************************************************/
FBL_End_Namespace


#pragma mark -
 

/**********************************************************************************************/
VCLIENT_Begin_Namespace


/**********************************************************************************************/
// inClientCacheSize specifies the maximal size of RAM that VClient 
// can eat on this computer (defined in bytes).
//
FBL_EXP vuint32				ValentinaInitC( 
								vuint32 	inClientCacheSize = DEFAULT_CACHE_SIZE );

// Finalize work with Valentina Client.
//
FBL_EXP void				ValentinaShutDownC( void );


/**********************************************************************************************/
// Factory of I_Connection for VCLIENT.DLL
//
FBL_EXP I_Connection_Ptr 	CreateConnection( 
                                const String& 	inHost,
                                const String& 	inUserName,
                                const String& 	inPassword,
                                const char*		inEncoding = nullptr,
                                vuint16			inPort = kDefaultPort,
                                vuint16 		inConnectTimeout = kDefaultTimeout,
                                const char* 	inOptions = nullptr );

FBL_EXP VSQL::I_SqlDatabase* CreateSqlDatabase(
                                I_Connection_Ptr	inConnection );

FBL_EXP VSQL::I_VProject*	CreateVProject(
								I_Connection_Ptr	inConnection,
								I_Disk_Location_Ptr	inLocation );

FBL_EXP VSQL::I_Server*		CreateServer(
								I_Connection_Ptr	inConnection );

FBL_EXP VSQL::I_SqliteDatabase_Ptr	CreateSqliteDatabase(
										I_Connection_Ptr	inConnection,
										I_Disk_Location_Ptr	inLocation,
										int					inMaxRetryCount = gvSqlite_default_MaxRetryCount,
										int					inBusySleepTime_ms = gvSqlite_default_BusySleepTime_ms );


/**********************************************************************************************/
/*
	This function allow you find all specified Bonjour services.
	inType		- name of service, e.g. "_valentina._tcp"
	inDomain 	- currently NULL or empty string ""
	inSerial 	- serial number that allow to use this function. 
*/	
FBL_EXP ArrayOfStrings_Ptr	LocateBonjourService( 
								const char* inType, 
								const char* inDomain );


/**********************************************************************************************/
/*
	This function allows you to check is a server running on given host and port.
*/	
FBL_EXP bool				IsServerRunning( 
								const String&	inHost, 
								vuint16			inPort,
								bool			inSSLUsed );


/**********************************************************************************************/
VCLIENT_End_Namespace


/**********************************************************************************************/
#endif // _VClient_h
