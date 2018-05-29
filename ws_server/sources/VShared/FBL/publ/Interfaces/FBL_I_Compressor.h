/**********************************************************************************************/
/* FBL_I_Compressor.h                                                     					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_Compressor_h 
	#define _FBL_I_Compressor_h

#include <VShared/FBL/publ/Headers/FBL.h>

#include <VShared/FBL/publ/Interfaces/FBL_Public_IIDs.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE(I_Compressor);


/**********************************************************************************************/
// Declare the prototype of the factory of compressor
//
typedef I_Compressor* (*MAKE_COMPRESSOR_FUNC_PTR)();


/**********************************************************************************************/
const bool kForCompression = true;
const bool kForUncompression = false;


/**********************************************************************************************/
/*
	Compressor - is abstraction layer around different compression algorithms.

	** Compressor have own INTERNAL buffer. On default 32Kb. 

	** You must have own SRC EXTERNAL buffer where you have data to be compressed, or to be uncompressed.
		This buffer can be big. e.g. 10MB.

	--------------------------------
	COMPRESSION:

	** SRC EXTERNAL buffer is used as INPUT source by compressor.
	** we should in loop call Compress(), this compress data until internal buffer is full.
	** we now can copy compressed portion of data from internal buffer and memcpy() it into 
			second external buffer or e.g. write it into BLOB field.
    ** then we do ResetBuffer();   
	** then go to the next iteration if Compress() have return TRUE. 

		while( MoreData )
		{
			MoreData = Compress() next part into small INTERNAL BUFFER
			save data from internal buffer
			ResetBuffer( true );
		}			

	As you can see we can use RAM as low as original external buffer + 32Kb of internal buffer.		

	--------------------------------
	UNCOMPRESSION:

	** Uncompression is not symmetric operation from point of view that we use internal buffer
		as INPUT source. This means that we need in loop fill it by data, then call Uncompress().
		Uncompressed data will be uncompressed directly into external big buffer.
	
	So we again use in RAM only internal buffer + SIZE of uncompressed data.

*/
interface FBL_SHARED_EXP_CLASS I_Compressor : public I_Unknown
{
	public://///////////////////////////////////////////////////////////////////////////////////

		// note that decompression speed is relatively insensitive to the compression level.
		enum 
		{
			kDontCompress 		= 0, 	// don't do any compression
			kFastestCompress	= 1, 	// compress as fast as possible 
			kDefaultCompress	= 6,	// default compression level
			kTightestCompress 	= 9	 	// compress as tightly as possible
		};							 	
		
		
	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// Construction:

virtual						~I_Compressor( void ) {}
							
	// ---------------------
	// Properties:
	
virtual	short				get_Level( void ) const		= 0;
virtual	void				put_Level( short inLevel ) 	= 0;
virtual	vuint32 			EstimateMaxCompressedBytesFor( vuint32 inDataSize ) = 0;

	// ---------------------
	// Compression methods:
	
							// inSrc - external buffer that keep data to be compressed.
							// inSrcLen - the size of data.
virtual void 				PrepareCompress( 
								const void* inExternalSrcBuffer, 
								vuint32		inExternalSrcBufferSize ) = 0;

							// Compresses data from the external buffer into internal buffer 
							// using the specified compression level. 
							// Returns the size of compressed data (in bytes)
							// that was written into internal buffer. 
virtual	vuint32 				Compress( void ) = 0;

							// Gives for a compression-lib the chance to cleanup after compression.
virtual void 				FinalizeCompress( void ) = 0;

	// ---------------------
	// Uncompression methods:
		
							// inSrc - external buffer that keep data to be uncompressed.
							// inSrcLen - the size of data.
virtual void 				PrepareUncompress( 
								const void* inExternalDestBuffer, 
								vuint32		inExternalDestBufferSize ) = 0;

							// Uncompress data from internal buffer into external buffer assigned
							// by PrepareUncompress(). inBytesInInternalBuffer allow us to have in
							// the internal buffer bytes less then its size.
							// Returns the size of uncompressed data (in bytes)
							// that was written into external destination buffer. 
virtual	vuint32 				Uncompress( vuint32 inBytesInInternalBuffer ) = 0;

							// Gives for a compression-lib the chance to cleanup after uncompression.
virtual void 				FinalizeUncompress( void ) = 0;

	// ---------------------
	// Internal Buffer methods:
		
virtual	void*				get_Buffer( void ) const = 0;

virtual	vuint32				get_BufferSize( void ) const = 0;
virtual	void				put_BufferSize( vuint32 inNewSize ) = 0;

virtual	void				ResetBuffer( bool inCompression ) = 0;
		 					
};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_I_Compressor_h

