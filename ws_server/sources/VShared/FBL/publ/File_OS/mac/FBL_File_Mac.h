/**********************************************************************************************/
/* FBL_File_Mac.h																		 	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_File_Mac_h 
	#define _FBL_File_Mac_h
#pragma once

#include <VShared/FBL/publ/File_OS/FBL_File.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
#if FBL_MAC

#if !__MACH__
	#include <StandardFile.h>
#endif // __MACH__


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_CLASS( File_Mac );


/**********************************************************************************************/
class FBL_SHARED_EXP_CLASS File_Mac : public File
{
		IMPLEMENT_UNKNOWN(File_Mac)											
		BEGIN_INTERFACE_TABLE(File_Mac)										
			IMPLEMENT_INTERFACE(I_File)
			IMPLEMENT_INTERFACE(I_AspectHost)
		END_INTERFACE_TABLE()

    public://///////////////////////////////////////////////////////////////////////////////////

							File_Mac( 
								OSType 			inCreator 	= '????',
								OSType 			inFileType  = '????' );
							
virtual						~File_Mac( void );


    public://///////////////////////////////////////////////////////////////////////////////////

// I_Storage

virtual bool  		   		get_IsOpen( void ) const override
							{
								FBL_ENGINE_LOCK
								return bool(mRefNum != 0);
							}
virtual	bool				get_IsReadOnly( void ) const override
							{
								FBL_ENGINE_LOCK
								return (bool)mReadOnly;
							}

virtual void 				Flush( void ) override;


	protected:////////////////////////////////////////////////////////////////////////////////// 

// File

virtual void          		DoClose( void ) override;
virtual void				DoCreate( void ) override;

virtual flength				DoGetLengthOfClosed( void ) const override;
virtual flength				DoGetLengthOfOpened( void ) const override;

virtual void          		DoOpen( void ) override;
virtual void				DoPutLength( flength aLength ) override;
virtual void				DoThrowOut( void ) override;

virtual vuint32				DoRead(
								char* 		inBuffer,
								flength		inFrom, 
								vuint32		inHowMuch ) override;
								
virtual vuint32				DoWrite( 
								const char* inBuffer,
								flength		inFrom, 
								vuint32		inHowMuch ) override;


    public://///////////////////////////////////////////////////////////////////////////////////

// File_Mac

		// MacCreator and MacType should be assigned before you create() file. 

		OSType				get_MacCreator( void ) const { return mMacCreator; }
		void				put_MacCreator( OSType inMacCreator )
								{ mMacCreator = inMacCreator; } 

		OSType				get_MacType( void ) const { return mMacType; }
		void				put_MacType( OSType inMacType )
								{ mMacType = inMacType; } 

#if FBL_SUPPORT_FSSPEC
		const FSSpec*		GetSpec( void ) const;
#endif // FBL_SUPPORT_FSSPEC

		flength				GetPos( void ) const;
		void				SetPos( flength pos );


	private:////////////////////////////////////////////////////////////////////////////////////
    		
		OSType				mMacCreator;
		OSType				mMacType;

		vint16				mRefNum;
		
							// Optimization to flush only if file really changed.
							// Since "fsync" is time-consuming even if empty OS buffers.
		bool				mNeedFlush;
};


/**********************************************************************************************/
FBL_End_Namespace

#endif // FBL_MAC

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_File_Mac_h

