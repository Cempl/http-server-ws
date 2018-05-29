/**********************************************************************************************/
/* FBL_FileWin.h																		 	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                       					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_FileWin_h 
	#define _FBL_FileWin_h
#pragma once

#include <VShared/FBL/publ/File_OS/FBL_File.h>
#include <VShared/FBL/publ/Util_classes/FBL_String.h>


/**********************************************************************************************/
#if FBL_WIN


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_CLASS(FileWin);


/**********************************************************************************************/
class FBL_SHARED_EXP_CLASS FileWin : public File
{
		IMPLEMENT_UNKNOWN(FileWin)											
		BEGIN_INTERFACE_TABLE(FileWin)
			IMPLEMENT_INTERFACE(I_File)
			IMPLEMENT_INTERFACE(I_AspectHost)
		END_INTERFACE_TABLE()
 
    public://///////////////////////////////////////////////////////////////////////////////////

	enum SeekPosition { begin = 0x0, current = 0x1, end = 0x2 };

							FileWin( void );
virtual						~FileWin( void );


    public://///////////////////////////////////////////////////////////////////////////////////

// I_Storage

virtual bool  		   		get_IsOpen( void ) const  { return (mHandle != INVALID_HANDLE_VALUE); }				
virtual	bool				get_IsReadOnly( void ) const  { return (bool)mReadOnly; }

virtual void 				Flush( void );


	protected:////////////////////////////////////////////////////////////////////////////////// 

// File

virtual	void				DoClose( void );
virtual	void				DoCreate( void );
virtual	void				DoOpen( void );

virtual void				DoPutLength( flength aLength );

virtual flength				DoGetLengthOfClosed( void ) const;
virtual flength				DoGetLengthOfOpened( void ) const;

virtual void          		DoThrowOut( void );

virtual vuint32				DoRead( 
								char* 		inBuffer,
								flength		inFrom, 
								vuint32 		inHowMuch );

virtual	vuint32				DoWrite( 
									const char* inBuffer, 
									flength		inFrom, 
									vuint32 		inHowMuch );

/*virtual	vuint32				DoWrite( 
									const char* inBuffer, 
									vuint32 		inHowMuch );*/

	protected:////////////////////////////////////////////////////////////////////////////////// 

// FileWin

virtual flength				Seek( flength inOff, vuint16 inFrom );
	

	protected:////////////////////////////////////////////////////////////////////////////////// 
    	
		HANDLE				mHandle;
		
							// Optimization to flush only if file really changed.
							// Since "fsync" is time-consuming even if empty OS buffers.
		bool				mNeedFlush;		
};


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // FBL_WIN


/**********************************************************************************************/
#endif // _FBL_FileWin_h

