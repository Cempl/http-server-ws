/**********************************************************************************************/
/* FBL_I_FldPicture.h	                                                      				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_FldPicture_h 
	#define _FBL_I_FldPicture_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
#if FBL_WIN
#define PicHandle  HANDLE
#endif 


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE(I_FldPicture);


/**********************************************************************************************/
// Interface I_FldPicture.
//
//StubInterface
interface FBL_SHARED_EXP_CLASS I_FldPicture : public I_Unknown
{

virtual							~I_FldPicture( void );


	//----------------------
	// Properties:

		// <DefQuality> [r/w]
								/**	Default quality level used when write 
									data to this field in JPEG format. */
virtual vuint16					get_DefQuality( void ) const = 0;
virtual void					put_DefQuality( vuint16 inValue ) = 0;


		// <PictureType> [r/o]
								/**	Returns the type (format) 
									of the picture for the current record. */
virtual PictType				get_PictureType( void ) const  = 0;


#if FBL_USE_PAINTLIB

	//----------------------
	// Methods:
								/** Reads picture from the field.
									@return DIB handle for Win, MacPict handle for Mac. 
										This handle must then be destroyed (with ::GlobalFree() 
										on Win or with FreeHandle() on Mac). */
virtual PicHandle				ReadPicture( void ) = 0;
#endif // FBL_USE_PAINTLIB

virtual bool					ReadPictureAs( 
									PictType 	inPicType,
									vuint16 	inOptions,
									vuint8*&	outPictData,
									vuint32&	outSize ) = 0;

#if FBL_USE_PAINTLIB
								/**	Writes picture in given format with specified options. 
									@param inPicture - PICT for MAC, DIB for Windows;
									@param inTargetType - type of result picture stored to disk.
									@param inConvertOptions - options for picture conversion  
										(in case when source and target format are not the same). 
										Means compression level when saving picture as JPEG.
									@param inHandleSize is required for V4MD, because Director 
										use own Handle API, and our kernel cannot get size self. */
virtual bool 					WritePictureAs(
									PicHandle 	inPicture,
									PictType	inTargetType,
									vuint16 	inConvertOptions = 50,
									vuint32 	inHandleSize = 0 ) = 0;

								/**	Writes PixMap in a given format with the specified options.
									@param inPicture - image pixels in pixMap format.
									@param inWidth - image width.
									@param inHeight - image height.
									@param inBitDepth - image bit depth.
									@param inTargetType - type of result picture stored to disk.
									@param inConvertOptions - options for picture conversion  
										(in case when source and target format are not the same). 
										Means compression level when saving picture as JPEG. */
virtual bool 					WritePixMapAs(
									void*	 	inPixels,
									vint32	 	inWidth,
									vint32	 	inHeight,
									vint32	 	inBitDepth,
									PictType 	inTargetType,
									vuint16 	inConvertOptions = 50 ) = 0;

								/**	Reads picture from the field as PixMap.
									Memory allocated with this function is returned via the 
									outPixels pointer. It should be then freed delete. 
									(although delete [] may cause problems :()
									@param outPixels	- pixel data in pixMap format.
									@param outWidth		- width of the image.
									@param outHeight	- height of the image.
									@param outBitDepth	- bit depth of the image. */
virtual bool 					ReadAsPixMap(
									void*&		outPixels,
									vint32&		outWidth,
									vint32&		outHeight,
									vint32&		outBitDepth ) = 0;

#endif // FBL_USE_PAINTLIB

}; 


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_I_FldPicture_h
