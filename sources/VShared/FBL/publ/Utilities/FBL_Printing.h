/**********************************************************************************************/
/* FBL_Printing.h																			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_Printing_h
	#define _FBL_Printing_h
#pragma once

// VSHARED:
#include <VShared/VSQL/Interfaces/VSQL_I_VReport.h>

// STD:
#include <map>


/**********************************************************************************************/
FBL_Begin_Namespace

#ifdef _WIN32

										/** Allocate and initilize LPDEVMODEW structure for
										 *  printer using specified options. */
FBL_SHARED_EXP LPDEVMODEW				CreatePrinterDevMode( 
											wchar_t*						printer, 
											const std::map<String, String>&	opts );

#endif // _WIN32

										/** Returns name of default printer. */
FBL_SHARED_EXP String					GetDefaultSystemPrinter( void );

										/** Returns list of local printers' names. */
FBL_SHARED_EXP ArrayOfStringsObj_Ptr	GetPrintersNames( void );

										/** Returns map with key/values using options string. */
FBL_SHARED_EXP std::map<String, String>	ParsePrintOptions(
											VSQL::I_VReport_Ptr	inReport,
											const FBL::String&	inOptions );

										/** Sends report to a printer, returns ID of created job,
										 *  or zero on error. */
FBL_SHARED_EXP int						SendReportToPrinter(
											VSQL::I_VReport_Ptr	inReport,
											const FBL::String&	inPrinter,
											const FBL::String&	inOptions,
											vuint32				inStartPageIndex,
											vuint32				inEndPageIndex );

#ifndef _WIN32

										/** Sends PDF to a printer, returns ID of created job,
										 *  or zero on error. */
FBL_SHARED_EXP int						SendPDFToPrinter( 
											const String&					inJobName,
											const std::map<String, String>&	inOptions,
											const String&					inFilePDF,
											const String&					inPrinter );

#endif // _WIN32


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_Printing_h
