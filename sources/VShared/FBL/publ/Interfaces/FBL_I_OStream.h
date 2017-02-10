/**********************************************************************************************/
/* FBL_I_OStream.h		                                                      				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_OStream_h 
	#define _FBL_I_OStream_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>

// STREAMS:
#include <VShared/FBL/publ/Interfaces/FBL_I_Stream.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_IStream.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE(I_IStream);
SMART_INTERFACE(I_OStream);


/**********************************************************************************************/
// interface class that represents the Output Stream functionality.
//
interface FBL_SHARED_EXP_CLASS I_OStream : virtual public I_Stream
{

virtual 					~I_OStream( void );

    // ---------------------
    // IO functions

                            /** Write method that writes data to its particular storage medium 
				                (such as memory or a disk file). Write attempts to write up to size bytes 
				                from buffer. The number of bytes actually written, can be retrieved by  
				                LastWrite() method call .	*/
virtual I_OStream&          put(
								  const void* 	inBuffer, 
								  vuint32 		inHowMuch )             = 0;

                            /** Copies the entire contents of this stream_in into stream, 
                            	stopping only if error occurs. */
virtual I_OStream&          put( I_IStream& stream_in )                 = 0;

							/// Writes the boolean value to the stream
virtual I_OStream&  		put( bool inValue )                         = 0;

							/// Writes the vuint8 value to the stream
virtual I_OStream&			put( vuint8 inValue )                        = 0;

							/// Writes the char value to the stream
virtual I_OStream&			put( char inValue )							= 0;

							/// Writes the vint16 value to the stream
virtual I_OStream&			put( vint16 inValue )                        = 0;		

							/// Writes the vuint16 value to the stream
virtual I_OStream&			put( vuint16 inValue )                       = 0;		

							/// Writes the vint32 value to the stream
virtual I_OStream&			put( vint32 inValue )                         = 0;		

							/// Writes the vuint32 value to the stream
virtual I_OStream&			put( vuint32 inValue )                        = 0;		

#if FBL_MAC
							/// Writes the size_t value to the stream
virtual I_OStream&			put( size_t inValue )                        = 0;		

#endif //FBL_MAC
							/// Writes the float value to the stream
virtual I_OStream&			put( float inValue )                        = 0;		

							/// Writes the double value to the stream
virtual I_OStream&			put( double inValue )                       = 0;		

							/// Writes the vint64 value to the stream
virtual I_OStream&			put( vint64 inValue )                        = 0;		

							/// Writes the vuint64 value to the stream
virtual I_OStream&			put( vuint64 inValue )                       = 0;

							/// Writes an array of UChars to the stream.
virtual I_OStream&			put( const UChar* inString, tslen inLen = -1 ) = 0;

							/// Writes an array of chars to the stream.
virtual I_OStream&			put( const char* inString, tslen inLen = -1 ) = 0;

	// ---------------------
	// Indent methods:
							// Increase/Reduce indent counter.
virtual void				IndentRight( void ) = 0;

							// If we have indent zero, then do not change it.
virtual void				IndentLeft( void ) = 0;

							// Write into stream string with current indent 
							// i.e. put N '\t' - chars, where N is current indent.
virtual I_OStream&			WriteIndent( void ) = 0;

};



/**********************************************************************************************/
class StAddIndent
{
	public://///////////////////////////////////////////////////////////////////////////////////	 
	
							 StAddIndent( I_OStream& inOStream ) : mOStream( inOStream )
							 	{ mOStream.IndentRight(); }
							 	
							~StAddIndent( void ) 
								{ mOStream.IndentLeft(); }
			
								
	protected:////////////////////////////////////////////////////////////////////////////////// 

		I_OStream&			mOStream;
	
};


/**********************************************************************************************/
class ArraySet;
class BitSet;


/**********************************************************************************************/
I_OStream& operator<<( I_OStream& inStream, const ArraySet* inSet );
I_OStream& operator<<( I_OStream& inStream, const BitSet* inSet );



/**********************************************************************************************/
template <class T>
I_OStream& operator << (I_OStream& s, const T& inValue )
{
	s.put( inValue );
	return s;
}


/**********************************************************************************************/
// Manipulator support
//
inline 
I_OStream& operator<<( I_OStream& s, 
					   I_OStream& (*inManip)(I_OStream&) )
{
	return inManip(s);
}


/**********************************************************************************************/
// flush manipulator
//
inline 
I_OStream& flush(I_OStream& inStream)
{
	inStream.Flush();
	return inStream;
}


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_I_OStream_h
