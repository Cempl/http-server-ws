/**********************************************************************************************/
/* FBL_File_Stream_T.h																		  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_File_Stream_T_h 
	#define _FBL_File_Stream_T_h
#pragma once

// FBL: 
#include <VShared/FBL/publ/Interfaces/FBL_I_Stream.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_File.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
template<class STREAM>
class File_Stream_T : public STREAM
{
		IMPLEMENT_UNKNOWN(File_Stream_T)											
		BEGIN_INTERFACE_TABLE(File_Stream_T)										
			IMPLEMENT_INTERFACE(I_IStream)										
		END_INTERFACE_TABLE()

	public://///////////////////////////////////////////////////////////////////////////////////

							File_Stream_T( I_File_Ptr inFile )
								: mpFile( inFile )
							{
							}

virtual flength        	    get_ByteSize( void ) const override;


	// ---------------------
    // Position methods:

virtual flength             Seek( 
								flength 	inOffset, 
								ESeekMode 	inMode ) override;
								
virtual flength             Tell( void ) const override;


    // ---------------------
    // Stream methods:

virtual	void				Flush( void ) override;
virtual	void				Close( void ) override;


	public://///////////////////////////////////////////////////////////////////////////////////

// I_IStream API:

    // ---------------------
    // Properties:

virtual bool                get_Eof( void ) const override
								{ return bool( ERROR_TYPE(STREAM::mLastError) == ERR_STREAM_EOF); }

virtual vuint32				PopZombiPacket( void ) override
								{ return 0; }

virtual void				PushZombiPacket( void ) override
								{}

virtual void				Invalidate( void ) override
								{}

    // ---------------------
    // Status methods:

virtual bool                CanRead( void ) const override
								{ return bool( ERROR_TYPE(STREAM::mLastError) != ERR_STREAM_EOF); }


	protected://////////////////////////////////////////////////////////////////////////////////


virtual vuint32              	DoPut( const void* 	inBuffer, 
					               vuint32 		inHowMuch) override;

virtual vuint32              	DoGet( void* 		inBuffer, 
								   vuint32 		inHowMuch ) override;


	protected://////////////////////////////////////////////////////////////////////////////////

		I_File_Ptr			mpFile;

};


/**********************************************************************************************/
template<class STREAM>
flength File_Stream_T<STREAM>::get_ByteSize( void ) const 
{
	return mpFile->get_Length();
}


/**********************************************************************************************/
template<class STREAM>
flength File_Stream_T<STREAM>::Seek( flength inOffset, ESeekMode inMode ) 
{
	flength currPos = mpFile->get_Pos();
	flength newPos = 0;
	vint32 tmpLastError = ERR_STREAM_EOF;
	
	switch( inMode )
	{
	    case kFromStart: 
	    {
	  		newPos = inOffset;
	    }break;
			
	    case kFromCurrent: 
	    {
			newPos = currPos + inOffset;
	    }break;
			
	    case kFromCurrentBack: 
	    {
			newPos = currPos - inOffset;
			tmpLastError = ERR_STREAM_BOF;
	    }break;
			
	    case kFromEnd:
		{
			flength flen = mpFile->get_Length();
			newPos = flen - inOffset;
			tmpLastError = ERR_STREAM_BOF;
		}break;
		
		default:		
	 		return INVALID_OFFSET;
	}

	try
	{
    	mpFile->put_Pos( newPos );
	}
	catch(...)
	{
		STREAM::mLastError = tmpLastError;
	}
	
	return newPos;
}


/**********************************************************************************************/
template<class STREAM>
flength File_Stream_T<STREAM>::Tell( void ) const 
{
	return mpFile->get_Pos();
}


/**********************************************************************************************/
template<class STREAM>
void File_Stream_T<STREAM>::Flush( void )
{
	mpFile->Flush();
}


/**********************************************************************************************/
template<class STREAM>
void File_Stream_T<STREAM>::Close( void )
{
	mpFile->Close();
}


/**********************************************************************************************/
template<class STREAM>
vuint32 File_Stream_T<STREAM>::DoPut(
	const void* 	inBuffer, 
	vuint32 		inHowMuch)
{
   return mpFile->Write( (char*)inBuffer, inHowMuch );
}


/**********************************************************************************************/
template<class STREAM>
vuint32 File_Stream_T<STREAM>::DoGet( void* inBuffer, vuint32 inHowMuch )
{
	return mpFile->Read((char*)inBuffer, inHowMuch);
}


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_Stream_h

