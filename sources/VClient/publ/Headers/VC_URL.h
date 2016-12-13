/**********************************************************************************************/
/* VC_URL.h																					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                       					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _VC_URL_h
	#define _VC_URL_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Interfaces/FBL_I_Location.h>
#include <VShared/FBL/publ/Util_classes/FBL_String.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_CLASS(URL_Location);


/**********************************************************************************************/
class FBL_EXP_CLASS URL_Location : public I_Location
{
		IMPLEMENT_UNKNOWN(URL_Location)											
		BEGIN_INTERFACE_TABLE(URL_Location)
			IMPLEMENT_INTERFACE(I_Location)
		END_INTERFACE_TABLE()

	public:////////////////////////////////////////////////////////////////////////////////////////

							URL_Location( 
								const String& inHostAddress,
								const String& inDbName,
								vuint16 inPortNumber );

// I_Location API: 

virtual I_Location_Ptr		Clone( void ) const override;

virtual	const String&		get_Path( void ) const override;
virtual	void				put_Path( const UChar* inFullPath ) override;
virtual	void				put_Path( const String& inFullPath ) override;

virtual	bool				get_IsEmpty( void ) const;

virtual	bool				get_IsUndefined( void ) const override; 

virtual I_Location_Ptr		get_ChildLocation( const char* inName ) const override;
virtual I_Location_Ptr		get_ChildLocation( const UChar* inName ) const override;
virtual I_Location_Ptr		get_ChildLocation( const String& inName ) const override;

virtual I_Location_Ptr		get_ParentDirectory( void ) const override;


// THIS CLASS API: 

			// <HostAddress>
		const String&		get_HostAddress( void ) const;

			/// <Port>
		vuint16				get_Port( void ) const;
		void				put_Port( vuint16 inPort );

			/// <DbName>
		const String&		get_DbName( void ) const;
		void				put_DbName( const char* inName );
		void				put_DbName( const UChar* inName );
		void				put_DbName( const String& inName );


	private:////////////////////////////////////////////////////////////////////////////////////

		String				mHostAddress;	///< Remote host address: either DNS or IP.
		String				mDbName;		///< Name of the database on the server.
		vuint16				mPortNumber;	///< Name of the database
};

/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


#endif // _VC_URL_h
