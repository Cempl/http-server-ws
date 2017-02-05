/**********************************************************************************************/
/* FBL_I_Stream.h			                                                      			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_Stream_h 
	#define _FBL_I_Stream_h
#pragma once

// FBL vs VSQL:
#include <VShared/FBL/publ/Headers/FBL.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// Defining how seeking have to be perform
// from start position, from end ore from current position of the stream
//
enum ESeekMode
{
	kFromStart,
	kFromCurrent,
	kFromCurrentBack,
	kFromEnd
};


/**********************************************************************************************/
const vuint32 INVALID_OFFSET = vuint32(-1);


/**********************************************************************************************/
SMART_INTERFACE(I_Stream);


/**********************************************************************************************/
// interface I_Stream -- Specifies methods of abstraction Stream.
//
// Also stream object can expose interfaces 
//		I_IStream -- contains methods to read from stream.
// and  I_OStream -- contains methods to write into stream.
//
interface FBL_SHARED_EXP_CLASS I_Stream : public I_Unknown
{

virtual 					~I_Stream( void );

	// ---------------------
	// Properties:

		// <LastError> [r/o]
			                /// Returns the last error code.
virtual vint32             	get_LastError( void ) const  = 0;

		// <LastError> [r/o]
                            /** Returns the number of bytes read by the last call 
							    to Read(), GetC(), Write(), PutC() or Peek().
                                this should be used to discover whether that call 
                                succeeded in reading all the requested data or not. */
virtual vuint32             get_LastCount( void ) const  = 0;

		// <Size>  [r/o]
                            // Returns the stream size
virtual flength             get_ByteSize( void ) const  = 0;

        // <Handle> [r/o] 
							/// Returns the low level descriptor of the object that is wraped 
							/// by this stream class.
virtual	vint32				get_Handle( void ) const  = 0; 


    // ---------------------
    // position functions
                            /** Moves the stream pointer to the given position (if the stream allow it)
                                Returns INVALID_OFFSET on error. */
virtual flength             Seek( 
								flength 	inOffset, 
								ESeekMode 	inMode )  = 0;

                            /// Return the current position of the stream pointer or INVALID_OFFSET
virtual flength             Tell( void ) const  = 0;


    // ---------------------
    // stream functions

							/** Flushes data if needed. */
virtual	void				Flush( void ) = 0;	

							/// Closes the streame
virtual	void				Close( void ) = 0;	

 
    // ---------------------
    // status functions

                            /// reset the stream state
virtual void                Reset( void ) = 0;

};


/**********************************************************************************************/
// Manipulator support
//
inline I_Stream& operator << (
	I_Stream& s, 
	I_Stream& (*inManip)(I_Stream&) )
{
	return inManip(s);
}


/**********************************************************************************************/
// flush manipulator
//
inline I_Stream& flush(I_Stream& inStream)
{
	inStream.Flush();
	return inStream;
}


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_I_Stream_h
