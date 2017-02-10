/**********************************************************************************************/
/* VSQL_I_VReport.h 				                                               			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _VSQL_I_VReport_h
	#define _VSQL_I_VReport_h
#pragma once


// FBL:
#include <VShared/FBL/publ/Interfaces/FBL_I_Disk_Location.h>
#include <VShared/FBL/publ/Util_classes/FBL_String.h>

// VSQL:
#include <VShared/VSQL/Headers/VSQL.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
#if FBL_WIN
#define PicHandle  HANDLE
#endif


/**********************************************************************************************/
FBL_Begin_Namespace

enum PictType;

FBL_End_Namespace


/**********************************************************************************************/
VSQL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE(I_VReport);

/**********************************************************************************************/
//StubInterface
interface FBL_SHARED_EXP_CLASS I_VReport : public I_Unknown
{
virtual						~I_VReport( void );

	public://///////////////////////////////////////////////////////////////////////////////////

    // ---------------------
    // Properties:

		// <DesignPaperFormat> [r/o]

								// Returns the name of format used to design the report.
virtual	String					get_DesignPaperFormat( void ) const = 0;
		
		// <PageCount> [r/o]

								// Returns the count of pages that will be produced fro this report
								// using the specified Cursor and the current Page format settings.
virtual	 vint32			        get_PageCount( void ) = 0;

		// <PaperSize> [r/w]

virtual	 void			   	    get_PaperSize( size_t& outWidth, size_t& outHeight ) const =  0;
virtual	 void				    set_PaperSize( size_t inWidth, size_t inHeight ) = 0;

		// <PreviewHeight> [r/w]

								// The preview height in pixels.
virtual  vuint32				get_PreviewHeight( void ) const = 0;
virtual  void		            set_PreviewHeight( vuint32 inHeight ) = 0;

		// <PreviewWidth> [r/w]

								// The preview width in pixels.
virtual  vuint32			    get_PreviewWidth( void ) const = 0;
virtual  void			        set_PreviewWidth( vuint32 inWidth ) = 0;

		// <PreviewZoom> [r/w]

								// Specifies the ZOOM value for preview page. Affected only preview
								// pages generated after this change.
virtual  vuint32			    get_PreviewZoom( void ) const = 0;
virtual  void                   set_PreviewZoom( vuint32 inZoom ) = 0;

		// <PrintZoom> [r/w]

								// Specifies the ZOOM value for print methods. Affected only 
								// PDF/PS/Printer outputs.
virtual  vuint32			    get_PrintZoom( void ) const = 0;
virtual  void                   set_PrintZoom( vuint32 inZoom ) = 0;


	public://///////////////////////////////////////////////////////////////////////////////////	 

	// ---------------------
	// Parameters Methods

virtual void					SetParameterValue(
									const String& inName,
									const String& inValue) = 0;

	public://///////////////////////////////////////////////////////////////////////////////////

	// -----------------------
	// Util methods

								/** Enable or disable adding debug info. */
virtual	void					EnableDebugInfo(bool value) = 0;

								/** Enable or disable logging. */
virtual	void					EnableLogging(bool value) = 0;


	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// Preview Methods:

#if FBL_USE_PAINTLIB
								// Returns the bitmap picture for win, PICT for MAC
								// of the Nth report page.
virtual  PicHandle	            PreviewPage(
									vuint32						inPageIndex ) = 0;
#endif // FBL_USE_PAINTLIB

    // ---------------------
    // Printing Methods:

								// Returns list with names of printers located on a client side
virtual	ArrayOfStringsObj_Ptr	GetLocalPrinters( void ) const = 0;

								// Returns list with names of printers located on a server side
virtual	ArrayOfStringsObj_Ptr	GetServerPrinters( void ) const = 0;

								// Prints pages from inFirst to inLast to buffer
								// Buffer must be destroyed by call ReleaseBuffer
virtual  void                   PrintToBuffer(
									void*						&outBuffer,
									vuint32						&outSize,
									ReportPrintType				inPrintType,
									vuint32						inStartPageIndex = 1,
									vuint32						inEndPageIndex	 = 0 ) = 0;

								// Prints pages from inFirst to inLast to file
virtual  void                   PrintToDisk(
									FBL::I_Disk_Location_Ptr	inPrintLocation,
									ReportPrintType				inPrintType,
									vuint32						inStartPageIndex = 1,
									vuint32						inEndPageIndex	 = 0 ) = 0;

								// Free memory allocated by PrintToBuffer
								// inBuffer - address of that buffer.
virtual	 void					ReleaseBuffer( 
									void*						inBuffer ) = 0;


virtual void					PrintToLocalPrinter(
									const FBL::String&	inPrinter,
									const FBL::String&	inOptions,
									vuint32				inStartPageIndex = 1,
									vuint32				inEndPageIndex	 = 0) = 0;

								/** Sends report to printer on a server (or local printer for local projects).
								*
								* @inOptions - the same as for PrintToLocalPrinter method.
								*/
virtual void					PrintToServerPrinter(
									const FBL::String&	inPrinter,
									const FBL::String&	inOptions,
									vuint32				inStartPageIndex = 1,
									vuint32				inEndPageIndex	 = 0 ) = 0;

								// Executes JavaScript script.
								// Returns string representation of a result.
virtual	String					RunScript( const String& script ) = 0;


	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// Internal Properties, only for VStudio:

		// <PaperOrientationLandscape> [r/w]

								// The paper orientation
virtual	 bool			   	    get_PaperOrientationLandscape( void ) const =  0;
virtual	 void				    set_PaperOrientationLandscape( bool inLandscape ) = 0;

		// <TopMargin> [r/w]
								// The top margin in pixels.
virtual  vuint32			    get_TopMargin( void ) const = 0;
virtual  void			        set_TopMargin( vuint32 inWidth ) = 0;

		// <BottomMargin> [r/w]
								// The top margin in pixels.
virtual  vuint32			    get_BottomMargin( void ) const = 0;
virtual  void			        set_BottomMargin( vuint32 inWidth ) = 0;

		// <LeftMargin> [r/w]
								// The top margin in pixels.
virtual  vuint32			    get_LeftMargin( void ) const = 0;
virtual  void			        set_LeftMargin( vuint32 inWidth ) = 0;

		// <RightMargin> [r/w]
								// The top margin in pixels.
virtual  vuint32			    get_RightMargin( void ) const = 0;
virtual  void			        set_RightMargin( vuint32 inWidth ) = 0;

		// <Logs> [r/o]
								// The list of warnings and errors.
virtual  String			        get_Logs( void ) const = 0;
};

/**********************************************************************************************/
VSQL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _VSQL_I_VReport_h
