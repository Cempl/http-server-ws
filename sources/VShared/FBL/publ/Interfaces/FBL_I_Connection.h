/**********************************************************************************************/
/* FBL_I_Connection.h																		  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef __FBL_I_Connection_h
	#define __FBL_I_Connection_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Util_classes/FBL_EncryptionKey.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_VNETW_Packet.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_Compressor.h>

#include <VNetwork/Util_Classes/FBL_StubRegistry.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// Integer state variables.
//
enum EStateVariable {
	kFilesTransferMode,
	k__StateItemCountInteger
};


/**********************************************************************************************/
// State variable values (integer).
//
enum EStateVariableValue {

	/**	Files for operations like to/fromFile, import/export
		are sent over network connection. */
	kNetwork,

	/**	All files are taken from server (i.e. all received path 
		(as parameters of method) point to the files in server's 
		local file system. */
	kLocal,

	k__StateItemValueCountInteger
};


/**********************************************************************************************/
// server response codes
//
enum EResponseCode
{
	/* General purpose response codes */
	kOkResponse			= 'O',
	kErrorResponse		= 'E',	/* problem while executing command */
	kFatalResponse		= 'F',	/* problem while executing command - connection must be closed */
	kNoticeResponse		= 'N',	/* warning message */

	/* Start up cycle */
	kBackendKeyData		= 'K',

	/* Query cycle */
	kCompletedResponse	= 'C',	/* A SQL command completed normally */
	kCopyInResponse		= 'G',	/* The server is ready to copy data from the client */
	kCopyOutResponse	= 'H',	/* The server is ready to copy data to the client */


	/* Cursor cycle */
	kCursorResponse		= 'R',
	kStringResponse		= 'X',

	/* Notification */
	kNotification		= 'T',

	kResponse_Dummy = 0xFFFFFFFF
};


/**********************************************************************************************/
SMART_INTERFACE(I_IStream);
SMART_INTERFACE(I_OStream);
SMART_INTERFACE(I_NotificationCenter);
SMART_CLASS(NotificationQueue);

/**********************************************************************************************/
const vuint16 kDefaultPort 			= 15432;
const vuint16 kDefaultEmbPort 		= 15433;
const vuint16 kDefaultSSLPort 		= 15434;

const vuint16 kDefaultHTTPPort		= 0;
const vuint16 kDefaultHTTPSPort		= 0;

const vuint16 kDefaultRESTPort		= 0;
const vuint16 kDefaultREST_SSL_Port	= 0;

const vuint16 kDefaultSNMPPort		= 15161;

const vuint16 kDefaultSqlitePort 	= 15532;
const vuint16 kDefaultSqliteSSLPort = 15534;

const vuint16 kDefaultNotificationPort = 15436;

const vuint16 kDefaultTimeout = 60 * 5;		// 5 minutes - if client needs more or less he can set
											// any timeout at any time.


/**********************************************************************************************/
SMART_INTERFACE(I_Connection);


/**********************************************************************************************/
enum ErrorLevel
{
	kErrorLevel_Undefined				= 0,

	// Critical error related to connect to the notification port (connection will be closed).
	kErrorLevel_Notifications			= 1,

	// Critical errors (connection will be closed).
	kErrorLevel_Connection				= 2,

	// Not-critical errors (connection still alive).
	kErrorLevel_Regular					= 3
};


/**********************************************************************************************/
// Interface I_Connection represents a physical connection between VClient and VServer. 
// 
interface FBL_SHARED_EXP_CLASS I_Connection : public I_Unknown
{
virtual						~I_Connection( void );

virtual	RemoteID			Open( void ) = 0;

							/** Closes existing connection.
								@param inGraceful - whether to send terminate 
									paket to the server (thus allowing it to close this 
									connection and free all it's used resources. 
									Practically always must be true (except some rare 
									cases not listed here). */
virtual	void				Close( bool inGraceful = true ) = 0;


virtual I_NotificationCenter_Ptr	GetNotificationCenter( void ) = 0;

virtual NotificationQueue_Ptr		GetNotificationQueue( void ) = 0;


	// ---------------------
	// Properties:

							/** Makes this connection secure. Should be called before open(). */
virtual void				UseSSL( void ) = 0;

							/** Allows to connect to the notification port to receive notifications
								from vServer in a dedicated thread. Should be called before open(). */
virtual void				UseNotifications( void ) = 0;

							/** http://valentina-db.com/bt/view.php?id=7579
								vServer sends notification port to vClient itself, but it could be
								wrong port in some cases like tunneling, port-forwarding and so on.
								This method is for force set notification port to the vClient. */
virtual void				ForceUseNotificationPort( vuint16 inPort ) = 0;

virtual const char*			get_Encoding( void ) const  = 0;
virtual bool				get_IsSingleByte( void ) const   = 0;

		// <Stream>
virtual	FBL::I_IStream_Ptr	get_IStream( void )  = 0;
virtual	FBL::I_OStream_Ptr	get_OStream( void )  = 0;

		// <IsConnected> [r/o]
virtual	bool				get_IsConnected( void )  = 0;

		// <Is64bitConnection> [r/o]
virtual	bool				get_Is64bitConnection( void )  = 0;

		// <HostName> [r/o]
virtual	const String&		get_HostName( void ) const  = 0;	//OLDKEEPAS: mHostName

		// <UserName> [r/o]
virtual	const String&		get_UserName( void ) const  = 0; //OLDKEEPAS: mUserName

		// <Password> [r/o]
virtual	const String&		get_Password( void ) const  = 0;	//OLDKEEPAS: mPassword

		// <Port> [r/o]
virtual	vuint16				get_Port( void ) const  = 0;

		// <ConnectionID> [r/o]
virtual	vuint32				get_ID( void ) const  = 0;		// Deprecated method to get proxy-address as vuint32
virtual	RemoteID			get_RemoteID( void ) const  = 0; // Used by vClient to get real connection proxy-address

		// <HighestServerProtocol> [r/o]
virtual VERSION_TYPE		get_HighestProtocolServerSupport( void ) const = 0;
virtual void				put_HighestProtocolServerSupport( VERSION_TYPE inValue ) = 0;

		// <HighestCommonProtocol> [r/o]
virtual VERSION_TYPE		get_HighestCommonProtocol( void ) const = 0;


		// <ResponseTimeout> [r/w]

							/// Returns server's response timeout.
virtual	vuint32				get_ResponseTimeout( void ) = 0;
							

							/** This method can be used to change server's 
								response timeout. You can change it to any 
								appropriate value before performing any 
								vint32 time operation. 
								@param inTimeoutSec - the operation timeout in seconds. */
virtual	void				put_ResponseTimeout( vuint32 inTimeoutSec ) = 0;

virtual	void				HandleError(
								ErrorLevel		inErrorLevel,
								I_PacketRcv* 	inPacket = nullptr ) = 0;

virtual	void				VerifyResponse( 
								I_PacketRcv& 	inPacket,
								COMMAND_NAME	inCmd = kOkResponse ) const = 0;

virtual	const String&		get_ErrorDescription(  ) const  = 0;	//OLDKEEPAS: mErrorDesc

virtual	const String&		get_FinalMessage( void ) const  = 0; //OLDKEEPAS: mFinalMessage


		// <StateVariable> [r/w]
		
virtual EStateVariableValue	get_StateVariable( 
								EStateVariable inVarType ) const  = 0;

virtual void				put_StateVariable( 
								EStateVariable inVarType,
								EStateVariableValue inValue ) = 0;

virtual ClientStubRegistry* get_ClientStubRegistry( void ) = 0;

							// Each connection has own compressor.
virtual I_Compressor_Ptr	get_Compressor( void )  = 0;

							// Each connection has own mutex.
virtual True_Own_Thread_Mutex_Recursive&	get_Lock( void )  = 0;
};


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif //__FBL_I_Connection_h










