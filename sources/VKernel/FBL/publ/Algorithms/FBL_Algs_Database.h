/**********************************************************************************************/
/* FBL_Algs_Database.h																		  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_Algs_Database_h
   #define _FBL_Algs_Database_h
#pragma once

#include <VShared/FBL/publ/Interfaces/FBL_I_Database.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Table.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Link.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace

/**********************************************************************************************/
// Return db name if db-location set and empty string otherwise.
//
FBL_EXP String GetDatabaseName( I_Database_Ptr inDatabase );


/**********************************************************************************************/
FBL_EXP bool get_IsProject( I_Database_Ptr inpDatabase );


/**********************************************************************************************/
// Methods to access the array of databases.
// You can get a database by its index or name.
// In the case of wrong index or name it returns NULL pointer.
//
FBL_EXP I_Database_Ptr get_Database( vuint32 inIndex );
FBL_EXP I_Database_Ptr get_Database( const UChar* inName );


/**********************************************************************************************/
// Returns the current version of database format.
// i.e. any new database file produced by this Valentina engine,
// will have this db format version. This method with pair in GetDatabaseVersion()
// allow for your application to see if app can just open given DB, 
// or it needs call some Convert_v1_v2() method at first.
//
FBL_EXP vuint16 	GetCurrentVersion( void );


/**********************************************************************************************/
// The following methods work on a closed database:

// Returns the format version of a database file.
// inLocation should point the Description Volume (.vdb) of the database.
// Returns 0 in case of not recognized Valentina format.
// Works even on a closed database.
//
// Format of database can change only PARADIGMA TEAM, but we avoid do this,
// because changed format usually breaks compatibility. 
//
FBL_EXP vuint32 GetDatabaseVersion( I_Location_Ptr inLocation );

// Returns the schema version from the custom header of a database file.
// inLocation should point the Description Volume (.vdb) of database.
FBL_EXP vuint16 GetSchemaVersion( I_Location_Ptr inLocation );

// Returns the database mode from custom header of a database file.
// inLocation should point the Description Volume (.vdb) of database.
FBL_EXP DbMode GetDatabaseMode( I_Location_Ptr inLocation );

// Returns true if the database structure is encrypted.
// inLocation should point the Description Volume (.vdb) of database.
FBL_EXP bool GetIsStructureEncrypted( I_Location_Ptr inLocation );


/**********************************************************************************************/
FBL_EXP void OnClientDestroy( size_t inClientID );


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_Algs_Database_h
