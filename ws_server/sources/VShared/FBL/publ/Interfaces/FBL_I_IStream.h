/**********************************************************************************************/
/* FBL_I_IStream.h			                                                  				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_IStream_h 
	#define _FBL_I_IStream_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>

// STREAMS:
#include <VShared/FBL/publ/Interfaces/FBL_I_Stream.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_OStream.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Disk_Location.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE(I_OStream);
SMART_INTERFACE(I_IStream);
SMART_INTERFACE(I_File);


/**********************************************************************************************/
							/** Factory of socket-based streams. 
								This factory method actually returns pointer
								to an object implemented both interfaces -
								I_IStream & I_OStream. So you can use dynamic_cast<>
								to query I_OStream interface. */
		I_IStream* 			CreateSocketStream( 
								vuint32	inTimeOut,	// Timeout for recv_n()/send_n() in seconds.
								vuint32	inBufSize = 1440, 
								bool	inSSLRequired = false );


							/** Factory of text-based streams (on disk file only). 
								@param inLoc - location of the disk file. */
FBL_SHARED_EXP	I_IStream* 	CreateTextStream( I_Location_Ptr inLoc );
FBL_SHARED_EXP	I_IStream* CreateTextStreamOnFile( I_File_Ptr inpFile );
FBL_SHARED_EXP	I_File_Ptr CreateTextFile( I_Location_Ptr inLoc );
FBL_SHARED_EXP	I_File_Ptr CreateOrOpenTextFile( I_Location_Ptr inLoc );


/**********************************************************************************************/
// interface class that represents the Input Stream functionality.
//
interface FBL_SHARED_EXP_CLASS I_IStream : virtual public I_Stream
{

virtual 					~I_IStream( void );

    //---------------------
    // Properties:

                            /** Returns TRUE if we reach EOF of stream.
                                Note that this cannot be really implemented for all streams 
                                and CanRead() is more reliable than Eof() */
virtual bool                get_Eof( void ) const  = 0;

							/**
								Pop/Push count of zombi packets which must be red from the 
								stream and ignored
								on the nearest reading the stream (in case of socket-timeout 
								exception some number of packets were stacked in stream).
							*/
virtual vuint32				PopZombiPacket( void ) = 0;
virtual void				PushZombiPacket( void ) = 0;
virtual void				Invalidate( void ) = 0;

    //---------------------
    // Status methods:
                            /** returns TRUE if some data is available in the stream right now,
                                 so that calling Read() wouldn't block */
virtual bool                CanRead( void ) const  = 0;


    // ---------------------
    // Input methods:

                            /** Read at most the given number of bytes from the stream  
                                there are 2 possible situations here: either there is nothing 
                                at all in the stream right now in which case Read() blocks until 
                                something appears (use CanRead() to avoid this) or there is already 
                                some data available in the stream and then Read() doesn't block but 
                                returns just the data it can read without waiting for more    
                                in any case, if there are not enough bytes in the stream right now,
                                LastRead() value will be less than size but greater than 0. 
                                If it is 0, it means that EOF has been reached.	*/
virtual I_IStream&          get(
								  void* 		inBuffer, 
								  vuint32 		inHowMuch )            = 0;

                            /** Copies the entire contents of this stream into streamOut, 
                            	stopping only when EOF is reached or an error occurs. */
virtual I_IStream&          get( I_OStream& streamOut )                 = 0;

							/// Reads the boolean value from the stream
virtual I_IStream&			get( bool& inValue )                       = 0;		

							/// Reads the vuint8 value from the stream
virtual I_IStream&			get( vuint8& outValue )                     = 0;		

							/// Reads the char value from the stream
virtual I_IStream&			get( char& inValue )					   = 0;

							/// Reads the vint16 from the stream
virtual I_IStream&			get( vint16& outValue )                     = 0;		

							/// Reads the vuint16 from the stream
virtual I_IStream&			get( vuint16& outValue )                    = 0;		

							/// Reads the vint32 value from the stream
virtual I_IStream&			get( vint32& outValue )                      = 0;		

							/// Reads the vuint32 value from the stream
virtual I_IStream&			get( vuint32& outValue )                     = 0;		

#if FBL_MAC
							/// Reads the size_t value from the stream
virtual I_IStream&			get( size_t& outValue )                     = 0;		
#endif //FBL_MAC
							/// Reads the float value from the stream
virtual I_IStream&			get( float& outValue )                     = 0;		

							/// Reads the double value from the stream
virtual I_IStream&			get( double& outValue )                    = 0;	

							/// Reads the vint64 value from the stream
virtual I_IStream&			get( vint64& inValue )                      = 0;		

							/// Reads the vuint64 from the stream
virtual I_IStream&			get( vuint64& inValue )                     = 0;

							/**	Reads the array of UChars from the stream. */
virtual I_IStream&          get(
								 UChar* outBuffer, 
								 tslen inLen )						   = 0;

};


/**********************************************************************************************/
class ArraySet;
class BitSet;


/**********************************************************************************************/
I_IStream& operator>>( I_IStream& inStream, ArraySet*& outSet );
I_IStream& operator>>( I_IStream& inStream, BitSet*& outSet );


/**********************************************************************************************/
template <class T>
I_IStream& operator >> (I_IStream& s, T& outValue )
{
	s.get( outValue );
	return s;
}


/**********************************************************************************************/
//  Disallow these operation; linker will prevent us from using it.
//
I_IStream& operator >> (I_IStream& s, char*& outValue );


/**********************************************************************************************/
// This operation should never be called.
inline I_IStream& operator >> (I_IStream& s, UChar*& outValue )
{
	argused2(s, outValue);
	FBL_CHECK(0);

	return s;
}


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_I_IStream_h
