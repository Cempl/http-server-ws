/**********************************************************************************************/
/* FBL_I_Seriazible.h                                                      					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_Seriazible_h
	#define _FBL_I_Seriazible_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Interfaces/FBL_I_IStream.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_OStream.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_VNETW_Packet.h>


// FINAL: 
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE(I_Serializable);
SMART_ARRAY_OF_SMART_PTRS( ArrayOfSerializable, I_Serializable_Ptr );


/**********************************************************************************************/
/// Interface of a serializable object.
// 
interface FBL_SHARED_EXP_CLASS I_Serializable : public I_Unknown
{
virtual						~I_Serializable( void );


    	// ----------------
		// Read Methods:

							/**	Read the object dump from the input stream.
								@param inStream - input stream from which to read from.
								@param inBlock - specifies whether to block in case when 
							 		not enough data available in the input stream. If 
									\a inBlock == true and not all the data is available 
									then call will block until all the data will be 
									available or until EOF will be reached. */
virtual void				From( 
								I_IStream_Ptr 	inStream,
								bool 			inBlock = true ) = 0;

virtual void				From( 
                                I_PacketRcv* 	inPacket,
								bool 			inBlock = true ) = 0;


    	// ----------------
		// Write Methods:

							/**	Dump the state of the object to the output stream.
								@param inStream - output stream to which dump to.
								@param inBlock - specifies whether to block in case when 
									not enough free space available in the stream's input 
								 	buffer. */
virtual void				To( 
								I_OStream_Ptr 	inStream,
								bool 			inBlock = true ) const = 0;

virtual void				To( 
                                I_PacketSnd*	inPacket,
								bool 			inBlock = true ) const = 0;

};


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_I_Seriazible_h
