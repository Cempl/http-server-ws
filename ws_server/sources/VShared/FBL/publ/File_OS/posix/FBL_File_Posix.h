/**********************************************************************************************/
/* FBL_File_Posix.h																		      */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef __FBL_File_Posix_h
#define __FBL_File_Posix_h
#pragma once

#include <VShared/FBL/publ/File_OS/FBL_File.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>

/**********************************************************************************************/
#if FBL_UNIX

#include <fcntl.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_CLASS(File_Posix);


/**********************************************************************************************/
class FBL_SHARED_EXP_CLASS File_Posix : public File
{
		IMPLEMENT_UNKNOWN(File_Posix)
		BEGIN_INTERFACE_TABLE(File_Posix)
			IMPLEMENT_INTERFACE(I_File)
			IMPLEMENT_INTERFACE(I_AspectHost)
		END_INTERFACE_TABLE()

    public://///////////////////////////////////////////////////////////////////////////////////

	enum SeekPosition { begin = SEEK_SET, current = SEEK_CUR, end = SEEK_END };

						File_Posix( void );
virtual					~File_Posix( void );


    public://///////////////////////////////////////////////////////////////////////////////////

// I_Storage

virtual bool 		   		get_IsOpen( void ) const override		{ return (mFd != -1); }
virtual bool				get_IsReadOnly( void ) const override	{ return (bool)mReadOnly; }

virtual void 				Flush( void ) override;


	protected://////////////////////////////////////////////////////////////////////////////////

// File

virtual void				DoClose( void ) override;
virtual void				DoCreate( void ) override;
virtual void				DoOpen( void ) override;

virtual void				DoPutLength( flength aLength ) override;

virtual flength				DoGetLengthOfClosed( void ) const override;
virtual flength				DoGetLengthOfOpened( void ) const override;

virtual void				DoThrowOut( void ) override;

virtual vuint32				DoRead(
							char* 		inBuffer,
							flength		inFrom,
							vuint32 		inHowMuch ) override;

virtual	vuint32				DoWrite(
							const char* inBuffer,
							flength		inFrom,
							vuint32 		inHowMuch ) override;


	protected://////////////////////////////////////////////////////////////////////////////////

// File_Posix

virtual flength				Seek( flength inOff, vuint16 inFrom );


	protected://////////////////////////////////////////////////////////////////////////////////

		int					mFd;
		
							// Optimization to flush only if file really changed.
							// Since "fsync" is time-consuming even if empty OS buffers.
		bool				mNeedFlush;
};


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // FBL_UNIX


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // __FBL_File_Posix_h

