/**********************************************************************************************/
/* FBL_File.h																				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_File_h 
	#define _FBL_File_h
#pragma once

#include <VShared/FBL/publ/Interfaces/FBL_I_Disk_Location.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_File.h>
#include <VShared/FBL/publ/Util_classes/FBL_AspectHost_imp.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_CLASS( FileIterator );
SMART_CLASS( File );


/**********************************************************************************************/
// We can not make file writable via put_IsReadOnly(false) call in case of fExternalRO.
enum EReadOnlyState
{
	fWritable				= 0,		// (r/w) There is no RO.
	fExternalRO				= 1 << 0,   // (r/w) True if RO by OS.
	fInternalRO      		= 1 << 1	// (r/w) True if RO by Valentina.
};


/**********************************************************************************************/
// List of messages to aspects of File.
//
const vint32	kAspectMsg_OnFileClose = 1;	
 

/**********************************************************************************************/
// This class implements many functionality of I_File, but still is abstract,
// because some functions will be overriden in OS specific classes.
// 
class FBL_SHARED_EXP_CLASS File : 
	public I_File,
	public AspectHost_imp, 
	public noncopyable
{
    public://///////////////////////////////////////////////////////////////////////////////////

							File( void );
virtual 					~File( void ); 


    public://///////////////////////////////////////////////////////////////////////////////////

// I_Storage

virtual	void				put_IsReadOnly( bool inValue ) override;

virtual bool				get_Exists( void ) const override  
							{
								FBL_ENGINE_LOCK
								return mpLocation ? (bool) mpLocation->get_Exists() : false;
							}

virtual flength				get_Size( void ) const override;

virtual void          		Close( void ) override;
virtual void				Create( void ) override;
virtual void          		Open( void ) override;

virtual void          		ThrowOut( void ) override;


    public://///////////////////////////////////////////////////////////////////////////////////

// I_File

	// ---------------------
	// Properties:

virtual I_Location_Ptr		get_Location( void ) const override  { return mpLocation; }
virtual void				put_Location( I_Location_Ptr inLoc) override  
								{ mpLocation = fbl_dynamic_cast<I_Disk_Location>(inLoc); }	

virtual flength 			get_Length( void ) const override;
virtual void				put_Length( flength inValue, bool inSaveDisk = true ) override;

virtual flength				get_Pos( void ) const override 	{ return mPosition; }
virtual void				put_Pos( flength pos ) override 	{ mPosition = pos; 	}

virtual	bool				get_EOF( void ) const override  
								{ return bool( get_Pos() == get_Length() ); }

virtual bool				get_IsEmpty( void ) const override  
								{ return bool( get_Length() == 0 ); }


	// ---------------------
	// Read/Write

virtual vuint32				Read( 
								char* 		inBuffer,
								vuint32 		inHowMuch ) override;

virtual vuint32				Write( 
								const char* inBuffer,
								vuint32 		inHowMuch ) override;

virtual vuint32				Read( 
								char* 		inBuffer,
								flength		inFrom, 
								vuint32 		inHowMuch ) override;
								
virtual	vuint32				Write( 
								const char* inBuffer, 
								flength		inFrom, 
								vuint32 		inHowMuch ) override;

virtual	void				EnsureLength(
								flength 	inValue, 
								bool 		inSaveDisk = true ) override	;


    public://///////////////////////////////////////////////////////////////////////////////////

// File

virtual void				put_Location( I_Disk_Location_Ptr inLoc)  
								{ mpLocation = inLoc; }	


	protected:////////////////////////////////////////////////////////////////////////////////// 

// File

virtual const String& 		get_Name( void ) const  
								{ return mpLocation ? mpLocation->get_FileName() : String::sEmpty(); }	
								
virtual const String&		get_Path( void ) const  
								{ return mpLocation ? mpLocation->get_Path() : String::sEmpty(); }	


		// Must be overriden in child classes:

							/** File is opened. Must close it. */
virtual	void				DoClose( void ) 							= 0;

							/** File not exists. Must create it. */
virtual	void				DoCreate( void ) 							= 0;

							/** File is closed. Must open it.
								Returns TRUE if file exists and was successfully opened. */
virtual	void				DoOpen( void )								= 0;

							/** Change length of file. */
virtual void				DoPutLength( flength aLength )				= 0;

							/** Get length of closed file. */
virtual flength				DoGetLengthOfClosed( void ) const  			= 0;

							/** Get length of opened file. */
virtual flength				DoGetLengthOfOpened( void ) const  			= 0;

							/** File is closed. Trash it. */
virtual void          		DoThrowOut( void )							= 0;

virtual vuint32				DoRead( 
								char* 		inBuffer,
								flength		inFrom, 
								vuint32 		inHowMuch )				= 0;
								
virtual	vuint32				DoWrite( 
								const char* inBuffer, 
								flength		inFrom, 
								vuint32 		inHowMuch )				= 0;

/*virtual	vuint32				DoWrite( 
									const char* inBuffer, 
									vuint32 		inHowMuch )
							{
								// DEFAULT implementation. For Subclasses that can do
								// optimization here should be overriden.
								return DoWrite( inBuffer, mPosition, inHowMuch );
							}*/

	protected:////////////////////////////////////////////////////////////////////////////////// 

		void				ClearDataMembers( void );

		void				SetLength( flength inValue )  { mLength = inValue; } 
			

	protected:////////////////////////////////////////////////////////////////////////////////// 

		I_Disk_Location_Ptr	mpLocation;		// specify location of file on disk.

		vuint16				mReadOnly;		// > 0 if the file is marked as read-only or
											// it is located on locked volume.


	private://////////////////////////////////////////////////////////////////////////////////// 
	
       	flength				mLength; 		// The length of the file.
        									// For speed we cache it here.
        
		flength 			mPosition;		// The current position in the file.
};


/**********************************************************************************************/
inline flength File::get_Length( void ) const 
{
	// if file is opened then Open() already have init mLength.
	// so if mLength is not inited then file is closed.
	return ( mLength == flength(-1) ) ? DoGetLengthOfClosed() : mLength;
}


/**********************************************************************************************/
inline void File::put_Length( flength inValue, bool inSaveDisk )
{
	FBL_CHECK( get_IsOpen() );

	if( inSaveDisk )
		DoPutLength( inValue );
	
	mLength = inValue;
	
	if( mPosition > inValue )
		mPosition = inValue;
}


/**********************************************************************************************/
inline void File::EnsureLength(
	flength 	inValue, 
	bool 		inSaveDisk )	
{
	// It gets with writing using cashed iterator (mLength==-1)
	if( inValue > mLength || mLength == flength(-1) )
		File::put_Length( inValue, inSaveDisk );
}


/**********************************************************************************************/
inline void File::ClearDataMembers( void ) 
{
	mReadOnly	= fWritable;
	mLength 	= flength(-1);	// undefined.
	mPosition	= 0;
}


/**********************************************************************************************/
inline vuint32 File::Read( 
	char* 		inBuffer,
	vuint32 		inHowMuch )
{
	FBL_CHECK( get_IsOpen() );
	
	return Read( inBuffer, mPosition, inHowMuch );
}


/**********************************************************************************************/
inline vuint32 File::Write( 
	const char* inBuffer,
	vuint32 		inHowMuch )
{
	FBL_CHECK( get_IsOpen() );
	
	return Write( inBuffer, mPosition, inHowMuch );
}						

						
/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_File_h

