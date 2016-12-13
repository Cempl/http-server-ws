/**********************************************************************************************/
/* FBL_I_Encryptable.h 		                                               					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_Encryptable_h 
	#define _FBL_I_Encryptable_h
	
// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// 
SMART_INTERFACE(I_Encryptable);
SMART_CLASS(EncryptionKey);


/**********************************************************************************************/
SMART_ARRAY_OF_CLASS_PTRS( ArrayOfEncryptables, I_Encryptable* );


 /**********************************************************************************************/
// Interface I_Encryptable specify API for an item that can be encrypted,
// for example Database, Table, Field.
//
//StubInterface
interface FBL_SHARED_EXP_CLASS I_Encryptable : public I_Unknown
{
virtual						~I_Encryptable( void );

	// ---------------------
	// Properties:

		// [IsEncrypted] r/o

virtual bool				get_IsEncrypted( void ) const  = 0;

virtual bool				get_IsStructureEncrypted( void ) const  = 0;


	// ---------------------
	// Encryption methods:

							/// For encrypted item, you must specify key to be used 
							/// before you start work with it.
virtual	void				UseEncryptionKey(
								EncryptionKey_Ptr inKey ) = 0;

							/// Returns true in case when access to object needs 
							/// password to be specified first.
virtual	bool				RequiresEncryptionKey( void ) const = 0;

							/// Returns true in case when object can be 
							/// encrypted.
virtual	bool				CanBeEncrypted( void ) const = 0;

							// Encrypts object using specified encryption key.
							// @inKey - key to be used for encryption.
virtual void				Encrypt( EncryptionKey_Ptr inKey ) = 0;

							// Decrypts object using specified encryption key.
							// @inKey - key that was used for encryption.
virtual void				Decrypt( EncryptionKey_Ptr inKey ) = 0;
				
							/// Assign a new key for encrypted item.
							/// If item was not encrypted before, then OldKey must be NULL,
							/// otherwise you must specify old key to be able change it.
virtual	void				ChangeEncryptionKey(
								EncryptionKey_Ptr inOldKey,
								EncryptionKey_Ptr inNewKey ) = 0;

virtual void				VerifyEncryptionKey( EncryptionKey_Ptr inKey ) const = 0;


	// ---------------------
	// Structure encryption methods:
	// must be used only for Database object!

							/// For encrypted item, you must specify key to be used 
							/// before you start work with it.
virtual	void				UseStructureEncryptionKey(
								EncryptionKey_Ptr inKey ) = 0;

							// Encrypts object using specified encryption key.
							// @inKey - key to be used for encryption.
virtual void				EncryptStructure( EncryptionKey_Ptr inKey ) = 0;

							// Decrypts object using specified encryption key.
							// @inKey - key that was used for encryption.
virtual void				DecryptStructure( EncryptionKey_Ptr inKey ) = 0;
				
							/// Assign a new key for encrypted item.
							/// If item was not encrypted before, then OldKey must be NULL,
							/// otherwise you must specify old key to be able change it.
virtual	void				ChangeStructureEncryptionKey(
								EncryptionKey_Ptr inOldKey,
								EncryptionKey_Ptr inNewKey ) = 0;

virtual void				VerifyStructureEncryptionKey( EncryptionKey_Ptr inKey ) const = 0;


	// ---------------------
	// Methods to implement encryption attributes inheritance:

virtual vuint32				RegisterChildObject( I_Encryptable* inChild ) = 0;

virtual vuint32				UnRegisterChildObject( I_Encryptable* inChild ) = 0;

}; 


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_I_Encryptable_h
