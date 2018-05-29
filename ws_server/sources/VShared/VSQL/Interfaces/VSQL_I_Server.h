/**********************************************************************************************/
/* VSQL_I_Server.h																			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef __VSQL_I_Server_h
	#define __VSQL_I_Server_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Util_classes/FBL_String.h>

// VSQL:
#include <VShared/VSQL/Headers/VSQL.h>
#include <VShared/VSQL/Database/VSQL_QueryResult.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Connection.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
VSQL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE(I_DatabaseInfo);
SMART_INTERFACE(I_ClientInfo);


/**********************************************************************************************/
// This enum is used with I_Server::Restart() method to allow specify
// the server behaviour on restart.
//
enum EShutdownType
{
	/// Server will wait until all the clients will be disconnected
	/// during specified amount of time. If the time will up and there 
	/// are still active connections then shutdown operation will fail 
	/// with error code ERR_OPERATION_TIMED_OUT.
	kWaitForActiveClients = 1,

	/// Server terminates all active client connections with 
	/// ERR_CONNECTION_FORCEDLY_CLOSED error code. But the 
	/// administrator connection will not be closed.
	kImmediate
};


/**********************************************************************************************/
// User entry struct.
//
struct UserEntry
{
	String	Name;
	bool	IsAdmin;
	
	bool operator == (const UserEntry& that)
	{
		return Name == that.Name && IsAdmin == that.IsAdmin;
	}
};


/**********************************************************************************************/
SMART_INTERFACE(I_Server);


/**********************************************************************************************/
/**	Interface I_Server provides you with ability to 
	administer remote Valentina Server instances. 
	You must have administrator privileges to use it.
 */
//StubInterface
interface FBL_SHARED_EXP_CLASS I_Server : public I_Unknown
{
virtual						~I_Server( void );

	// --------------------
	// Server variables

							/** Returns the version of VServer instance. */
virtual	const String&		get_Version( void ) = 0;

							/** Returns value of the server variable by its name. */
virtual	String				get_Variable( const UChar* inName ) const = 0;
virtual	String				get_Variable( const String& inName ) const = 0;

							/**	Updates value of the server variable. 
								Returns operation status code:
								0 - value updated on the server;
								1 - value scheduled for update - it takes place only  
								after server restart. 
								-1 - no connection made. */
virtual	vint32				put_Variable( 
								const UChar* inName, 
								const UChar* inNewVal ) = 0;

virtual	vint32				put_Variable( 
								const String& inName, 
								const String& inNewVal ) = 0;

	// --------------------
	// Properties

							/** Returns the number of active connections. */
virtual	vuint32				get_ConnectionCount( void ) const  = 0;

		
	// --------------------
	// Registration methods

							/**	Registers existing database on the server; 
								this method tries to find the 'inName'.vdb database file at the
								location pointed by 'SystemCatalog' variable; 
								if the file exists then new entry will be appended to the
								'master' database and the database becomes  available to the clients;
								@param inName -- name of the existing database file to bind 
									(without .vdb extension). */
virtual	void				RegisterDatabase(
								const UChar* inName,
								const UChar* inPath = nullptr ) = 0;
								
virtual	void				RegisterDatabase(
								const String& inName,
								const String& inPath = String::sEmpty() ) = 0;

							/**	Unregisters existing database from VServer. 
								Returns true if database has been succesfully 
								unregistered and false if it cannot be deregistered 
								immediately (requires VServer to restart). */
virtual	bool				UnregisterDatabase( const UChar* inName ) = 0;
virtual	bool				UnregisterDatabase( const String& inName ) = 0;


							/** Cancel all connections to the inDbName database*/
virtual void				CancelDbConnections( const String& inDbName ) = 0;

							/**	Registers existing project on the server; 
								this method tries to find the 'inName'.vsp project file at the
								location pointed by 'SystemProjectCatalog' variable; 
								if the file exists then new entry will be appended to the
								'master' database and the project becomes  available to the clients;
								@param inName -- name of the existing project file to bind 
									(without .vdb extension). */
virtual	void				RegisterProject(
								const UChar* inName,
								const UChar* inPath = nullptr ) = 0;
								
virtual	void				RegisterProject(
								const String& inName,
								const String& inPath = String::sEmpty() ) = 0;

							/**	Unregisters existing project from VServer. 
								Returns true if project has been succesfully 
								unregistered and false if it cannot be deregistered 
								immediately (requires VServer to restart). */
virtual	bool				UnregisterProject( const UChar* inName ) = 0;
virtual	bool				UnregisterProject( const String& inName ) = 0;

	// --------------------
	// Methods to work with database

							// Returns the count of databases on the server
virtual	vuint16				get_DatabaseCount( void ) const  = 0;

							// Returns the count of projects on the server
virtual	vuint16				get_ProjectCount( void ) const  = 0;

							// Returns database description object.
virtual	I_DatabaseInfo_Ptr	get_DatabaseInfo( vuint16 inIndex ) = 0;

virtual	I_DatabaseInfo_Ptr	get_DatabaseInfo( const String& inDbName ) = 0;


	// --------------------
	// Users management

							// Returns client description object.
virtual	I_ClientInfo_Ptr	get_ClientInfo(vuint16 inIndex) = 0;

virtual	void				AddUser(
								const UChar* inName, 
								const UChar* inPassword,
								bool inAdmin ) = 0;

virtual	void				AddUser(
								const String& inName, 
								const String& inPassword,
								bool inAdmin ) = 0;
    
virtual	void				RemoveUser( const UChar* inName ) = 0;
virtual	void				RemoveUser( const String& inName ) = 0;

							/**	Changes user password. */
virtual	void				ChangeUserPassword(
								const UChar* inName, 
								const UChar* inNewPassword ) = 0;
virtual	void				ChangeUserPassword(
								const String& inName, 
								const String& inNewPassword ) = 0;

							/** Returns the count of the registered users. */
virtual	vuint32				get_UserCount( void ) const  = 0;

							/** Gets information about single user by index. */
virtual	void				get_UserEntry( 
								vuint32 inIndex, 
								UserEntry& outEntry ) const = 0;

	// --------------------
	// Server management 

							/** Terminates existing connection.
								@param inConnID - connection identifier. */
virtual	void 				CancelConnection( vuint32 inConnID ) = 0;

							/** Try to restart server during specified amount of time. 
								@param inType - specifies server behavior on restart.
								@param inSec - restart timeout (not yet supported). */
virtual	void				Restart( 
								EShutdownType inType = kWaitForActiveClients,
								vuint32 inSec = 20 ) = 0;

							/** Try to shutdown server during specified amount of time. 
								@param inType - specifies server behavior on restart.
								@param inSec - shutdown timeout (not yet supported). */
virtual	bool				Shutdown(
								EShutdownType inType = kWaitForActiveClients,
								vuint32 inSec	= 20 ) = 0;

	// --------------------
	// Refresh status information

							/** Refreshes the list of the server variables and their values. */
virtual	void				Refresh( void ) = 0;


};


/**********************************************************************************************/
VSQL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif //__VSQL_I_Server_h










