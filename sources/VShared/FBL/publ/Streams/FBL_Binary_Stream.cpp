/**********************************************************************************************/
/* FBL_Binary_Stream.cpp                                                   					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Streams/FBL_Binary_Stream.h>
#include <VShared/FBL/publ/Util_Strings/FBL_ustdio.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
Binary_Stream::Binary_Stream( void )
: 
	Stream_Imp() 
{
	mWback = nullptr;
	mWbacksize =
	mWbackcur = 0;
}


/**********************************************************************************************/
Binary_Stream::~Binary_Stream( void )
{
	delete [] mWback;
}


/**********************************************************************************************/
I_OStream& Binary_Stream::put(
	const void* inBuffer, 
	vuint32 		inHowMuch)
{ 
	mLastCount = DoPut(inBuffer, inHowMuch); 
	if (mLastCount <= inHowMuch)
	  mLastError = ERR_STREAM_WRITE_ERROR;
	return * (I_OStream*) this;
}


/**********************************************************************************************/

#pragma mark -


/**********************************************************************************************/
I_IStream& Binary_Stream::get(
	void* 		inBuffer, 
	vuint32 		inHowMuch )
{
	char *p = (char *)inBuffer;
	mLastCount = 0;

	vuint32 read = GetWBack(inBuffer, inHowMuch);
	for( ;; )
	{
/*		if( (vint32)inHowMuch - (vint32)read < 0l )
		{
			int i = 1;
			i;
		}*/

		inHowMuch -= read;
		mLastCount += read;
		p += read;

		if ( !inHowMuch )
		{
			// we read the requested amount of data
			break;
		}

		if ( p != inBuffer && !CanRead() )
		{
			// we have already read something and we would block in DoGet()
			// now: don't do it but return immediately
			break;
		}

		read = DoGet(p, inHowMuch);
		if ( !read )
		{
			// no more data available
			break;
		}
	}

	return * (I_IStream*) this;
}


/**********************************************************************************************/
I_IStream& Binary_Stream::get( I_OStream& streamOut )
{
	char inBuff[BUF_TEMP_SIZE];

	for( ;; )
	{
		vuint32 bytes_read = get(inBuff, FBL_SIZEOF(inBuff)).get_LastCount();
		if ( !bytes_read )
			break;

		if ( streamOut.put((void*)inBuff, bytes_read).get_LastCount() != bytes_read )
			break;
	}

	return * (I_IStream*) this;
}


/**********************************************************************************************/
I_IStream& Binary_Stream::get( UChar* inBuffer, tslen inChars )
{
	get(reinterpret_cast<void*>(inBuffer), static_cast<vuint32>(inChars) * sizeof(UChar));
	return *this;
}


/**********************************************************************************************/
#if FBL_MAC
I_OStream& Binary_Stream::put( size_t inValue )
{
	#if FBL_64
	return put( (vuint64)inValue );
	#else
	return put( (vuint32)inValue );	
	#endif // FBL_64
}
#endif // FBL_MAC

/**********************************************************************************************/
#if FBL_MAC
I_IStream& Binary_Stream::get( size_t& outValue )
{
	#if FBL_64
	return get( (vuint64&)outValue );
	#else
	return get( (vuint32&)outValue );	
	#endif // FBL_64
}
#endif // FBL_MAC

#pragma mark -

 
/**********************************************************************************************/
char* Binary_Stream::AllocSpaceWBack( vint32 sizeNeeded )
{
	// get number of bytes left from previous wback buffer
	vuint32 toget = vuint32(mWbacksize - mWbackcur);

	// allocate a buffer large enough to hold prev + new data
	char* temp_b = new char[sizeNeeded + toget];

	if (!temp_b)
		return nullptr;

	// copy previous data (and free old buffer) if needed
	if (mWback)
	{
		memmove(temp_b + sizeNeeded, mWback + mWbackcur, toget);
		delete [] mWback;
	}

	// done
	mWback = temp_b;
	mWbackcur = 0;
	mWbacksize = vuint32(sizeNeeded + toget);

	return mWback;
}


/**********************************************************************************************/
vuint32 Binary_Stream::GetWBack(
	void* inBuff, 
	vuint32 inSize )
{
	if (!mWback)
	    return 0;

	// how many bytes do we have in the buffer?
	vuint32 toget = vuint32(mWbacksize - mWbackcur);

	if ( inSize < toget )
	{
		// we won't read everything
		toget = inSize;
	}

	// copy the data from the cache
	memcpy(inBuff, mWback + mWbackcur, toget);

	mWbackcur += toget;
	if ( mWbackcur == mWbacksize )
	{
		// TODO: should we really free it here all the time? maybe keep it?
		delete [] mWback;
		mWback = nullptr;
		mWbacksize = 0;
		mWbackcur = 0;
	}

	// return the number of bytes copied
	return toget;
}


/**********************************************************************************************/
I_OStream& Binary_Stream::put( const UChar* inString, tslen inLen )
{
	tslen Len = inLen;
	if( Len == -1 )
	{
		Len = static_cast<tslen>(vu_strlen(inString));
	}

	put(reinterpret_cast<const void*>(inString), Len * sizeof(UChar));
	return *this;
}


/**********************************************************************************************/
I_OStream& Binary_Stream::put( const char* inString, tslen inLen )
{
	tslen Len = inLen;
	if( Len == -1 )
	{
		Len = static_cast<tslen>(strlen(inString));
	}

	put(reinterpret_cast<const void*>(inString), static_cast<vuint32>(Len));
	return *this;
}


#pragma mark -


/**********************************************************************************************/
void Binary_Stream::IndentRight( void )
{
	// KP 10/05/2006
	// TODO

	return;
}


/**********************************************************************************************/
void Binary_Stream::IndentLeft( void )
{
	// KP 10/05/2006
	// TODO

	return;
}


/**********************************************************************************************/
I_OStream& Binary_Stream::WriteIndent( void )
{
	// KP 10/05/2006
	// TODO

	return *this;
}


/**********************************************************************************************/
FBL_End_Namespace


