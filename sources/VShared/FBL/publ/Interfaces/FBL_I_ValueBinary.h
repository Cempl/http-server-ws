/**********************************************************************************************/
/* FBL_I_ValueBinary.h                                                     					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_ValueBinary_h
	#define _FBL_I_ValueBinary_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Interfaces/FBL_I_Value.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// forward declarations:
//
SMART_INTERFACE(I_Value);


/**********************************************************************************************/
// this file declrations:
//
SMART_INTERFACE(I_ValueBinary);


/**********************************************************************************************/
/**
	Interface class that specify API of work with Binary value.

	A binary value contains own internal buffer. We can get its size by I_Value::get_ByteLength().

	Binary value can have value less then buffer. So we must store an actual length of each value.
	To get the actual length we can use I_Value::get_Length().

	NOTE: I_Value::put_Size() will work as change of column size. 
	NOTE: There is sense to use I_Value::put_Length() to truncate the current value.
			No sense try to grow it. 
*/
interface FBL_SHARED_EXP_CLASS I_ValueBinary : public I_Unknown
{

virtual 					~I_ValueBinary( void );

	// ---------------------
	// Value Methods:
	
							/** Copies data from the internal buffer into outDataBuffer
								that have the size inBufferSize.
								If buffer is greater of value then copies the whole value
								and returns the number of copied bytes.
								If buffer is smaller of value, then copies inBufferSize bytes,
								returns inBuferSize. */
virtual vuint32				get_Data( 
								vuint8* 	outDatabuffer, 
								vuint32 	inBufferSize ) const  = 0;

							/** Copies data from the specified buffer into the internal buffer. */
virtual void				put_Data( 
								vuint8* 	inDataBuffer, 
								vuint32 	inBufferSize )  = 0;
																
};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_I_ValueBinary_h
