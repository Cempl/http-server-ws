/**********************************************************************************************/
/* FBL_Printing.cpp																			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

// POSIX:
#ifndef _WIN32
#include <dlfcn.h>
#include <unistd.h>
#endif // _WIN32

// WIN32:
#ifdef _WIN32
#include <winspool.h>
#endif // _WIN32

// VSHARED:
#include <VShared/FBL/publ/Starter/FBL_Starter.h>
#include <VShared/FBL/publ/Utilities/FBL_Printing.h>
#include <VShared/FBL/publ/Util_classes/FBL_StringConversion.h>
#include <VShared/FBL/publ/Util_classes/FBL_W2U.h>
#include <VShared/VSQL/Interfaces/VSQL_I_VReport.h>

// STD:
#include <vector>

/**********************************************************************************************/
FBL_Begin_Namespace


//////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark helper functions
//////////////////////////////////////////////////////////////////////////


/**********************************************************************************************/
static std::vector<String> SplitOptions( const String& s )
{
    std::vector<String> r;
    
	tslen first = 0;
	
	const tslen len = s.length();
	for( tslen i = 0 ; i < len ; ++i )
	{
		if( s[ i ] == ',' )
		{
			if( i )
				r.push_back( s.mid( first, i - first ).trim() );
            
			first = i + 1;
		}
	}
    
	if( first < len )
		r.push_back( s.mid( first ).trim() );
    
    return r;
}


/**********************************************************************************************/
#ifdef _WIN32

/**********************************************************************************************/
// Helper function to apply paper format by name
//
static short GetPaperSizeID( std::string name )
{
	static const std::map<std::string, short> papers {
		{ "letter"				, DMPAPER_LETTER },
		{ "letter small"		, DMPAPER_LETTERSMALL },
		{ "tabloid"				, DMPAPER_TABLOID },
		{ "ledger"				, DMPAPER_LEDGER },
		{ "legal"				, DMPAPER_LEGAL },
		{ "statement"			, DMPAPER_STATEMENT },
		{ "executive"			, DMPAPER_EXECUTIVE },
		{ "a2"					, DMPAPER_A2 },
		{ "a3"					, DMPAPER_A3 },
		{ "a4"					, DMPAPER_A4 },
		{ "a4 extra"			, DMPAPER_A4_EXTRA },
		{ "a4 plus"				, DMPAPER_A4_PLUS },
		{ "a4 small"			, DMPAPER_A4SMALL },
		{ "a5"					, DMPAPER_A5 },
		{ "a6"					, DMPAPER_A6 },
		{ "b4"					, DMPAPER_B4 },
		{ "b4 (iso)"			, DMPAPER_ISO_B4 },
		{ "b4 (jis)"			, DMPAPER_B4 },
		{ "b5"					, DMPAPER_B5 },
		{ "b5 (jis)"			, DMPAPER_B5 },
		{ "folio"				, DMPAPER_FOLIO },
		{ "quarto"				, DMPAPER_QUARTO },
		{ "9 x 11"				, DMPAPER_9X11 },
		{ "9x11"				, DMPAPER_9X11 },
		{ "10 x 11"				, DMPAPER_10X11 },
		{ "10x11"				, DMPAPER_10X11 },
		{ "12 x 11"				, DMPAPER_12X11 },
		{ "12x11"				, DMPAPER_12X11 },
		{ "15 x 11"				, DMPAPER_15X11 },
		{ "15x11"				, DMPAPER_15X11 },
		{ "10 x 14"				, DMPAPER_10X14 },
		{ "10x14"				, DMPAPER_10X14 },
		{ "11 x 17"				, DMPAPER_11X17 },
		{ "11x17"				, DMPAPER_11X17 },
		{ "note"				, DMPAPER_NOTE },
		{ "envelope"			, DMPAPER_ENV_ITALY },
		{ "envelope monarch"	, DMPAPER_ENV_MONARCH },
		{ "monarch"				, DMPAPER_ENV_MONARCH },
		{ "envelope #9"			, DMPAPER_ENV_9 },
		{ "envelope #10"		, DMPAPER_ENV_10 },
		{ "envelope #11"		, DMPAPER_ENV_11 },
		{ "envelope #12"		, DMPAPER_ENV_12 },
		{ "envelope #14"		, DMPAPER_ENV_14 },
		{ "envelope dl"			, DMPAPER_ENV_DL },
		{ "envelope b4"			, DMPAPER_ENV_B4 },
		{ "envelope b5"			, DMPAPER_ENV_B5 },
		{ "envelope b6"			, DMPAPER_ENV_B6 },
		{ "envelope c5"			, DMPAPER_ENV_C5 },
		{ "envelope c3"			, DMPAPER_ENV_C3 },
		{ "envelope c4"			, DMPAPER_ENV_C4 },
		{ "envelope c6"			, DMPAPER_ENV_C6 },
		{ "envelope c65"		, DMPAPER_ENV_C65 },
		{ "envelope invite"		, DMPAPER_ENV_INVITE },
		{ "c"					, DMPAPER_CSHEET },
		{ "d"					, DMPAPER_DSHEET },
		{ "e"					, DMPAPER_ESHEET },
		{ "japanese postcard"	, DMPAPER_JAPANESE_POSTCARD },
		{ "fanfold"				, DMPAPER_FANFOLD_US },
		{ "letter extra"		, DMPAPER_LETTER_EXTRA },
		{ "legal extra"			, DMPAPER_LEGAL_EXTRA },
		{ "tabloid extra"		, DMPAPER_TABLOID_EXTRA },
	};
	
	for( char& c : name )
		c = static_cast<char>( tolower( c ) );

	const auto it = papers.find( name );
	
	return it != papers.end( ) ? it->second : 0;
}

/**********************************************************************************************/
#endif // _WIN32


/**********************************************************************************************/
#ifndef _WIN32

/**********************************************************************************************/
struct cups_option_t;

/**********************************************************************************************/
struct cups_dest_t {
    char *name, *instance;
    int is_default;
    int num_options;
    cups_option_t *options;
};

/**********************************************************************************************/
struct CupsAPI
{
	int			(*cupsAddOption)		( const char* name, const char* value, int num_options, cups_option_t** options );
    void		(*cupsFreeDests)        ( int num_dests, cups_dest_t* dests );
    void		(*cupsFreeOptions)		( int num_options, cups_option_t* options );
	const char*	(*cupsGetDefault)		( void );
    int			(*cupsGetDests)         ( cups_dest_t** dests );
    const char* (*cupsGetOption)        ( const char* name, int num_options, cups_option_t* options );
	const char* (*cupsLastErrorString)	( void );
	int			(*cupsPrintFile)		( const char*, const char*, const char*, int, cups_option_t* );
};

/**********************************************************************************************/
static CupsAPI* CreateCupsAPI( void )
{
	// Open shared library
	
#ifdef __linux__
	
	void* hnd = dlopen( "libcups.so.2", RTLD_LAZY | RTLD_LOCAL );
	if( !hnd ) hnd = dlopen( "libcups.so.1", RTLD_LAZY | RTLD_LOCAL );
	if( !hnd ) hnd = dlopen( "libcups.so.3", RTLD_LAZY | RTLD_LOCAL );
	if( !hnd ) hnd = dlopen( "libcups.so.4", RTLD_LAZY | RTLD_LOCAL );
	
#else // __linux__

	void* hnd = dlopen( "libcups.2.dylib", RTLD_LAZY | RTLD_LOCAL );
	if( !hnd ) hnd = dlopen( "libcups.1.dylib", RTLD_LAZY | RTLD_LOCAL );
	if( !hnd ) hnd = dlopen( "libcups.3.dylib", RTLD_LAZY | RTLD_LOCAL );
	if( !hnd ) hnd = dlopen( "libcups.4.dylib", RTLD_LAZY | RTLD_LOCAL );
	
#endif // __linux__

	if( !hnd )
		return nullptr;
	
	// Collect pointers to functions
	
	CupsAPI* const r = new CupsAPI;
	
#define CUPS_GET_FUNC( f ) \
	r->f = reinterpret_cast<decltype( r->f )>( dlsym( hnd, #f ) ); \
	if( !r->f ) { delete r; return nullptr; }
	
	CUPS_GET_FUNC( cupsAddOption	   );
	CUPS_GET_FUNC( cupsFreeDests	   );
    CUPS_GET_FUNC( cupsFreeOptions	   );
	CUPS_GET_FUNC( cupsGetDefault	   );
    CUPS_GET_FUNC( cupsGetDests        );
    CUPS_GET_FUNC( cupsGetOption       );
	CUPS_GET_FUNC( cupsPrintFile	   );
    CUPS_GET_FUNC( cupsLastErrorString );
	
#undef CUPS_GET_FUNC	
	
	// Return result
	
	return r;	
}

/**********************************************************************************************/
static const CupsAPI* GetCupsAPI( void )
{
	static const CupsAPI* const r = CreateCupsAPI();
	return r;
}

/**********************************************************************************************/
// On OS X CUPS accepts only "Device Name" as a printer name.
// But system preferences and all standard dialogs show "descriptional" name
// (see https://www.cups.org/documentation.php/doc-1.7/ref-printers-conf.html, Info directive)
// This name is accessible from the CUPS API as the "printer-info" attribute
// in the list of destinations.
// This function returns:
// * Printer "Device Name", if it's "printer-info" matches @inPrinter
// * @inPrinter, if appropriate "printer-info" wasn't found.
//
static String GetCupsPrinterNameByDescription( const String& inPrinter )
{
    const CupsAPI* const api = GetCupsAPI();
    if( !api )
        return inPrinter;
    
    String r = inPrinter;
    
    cups_dest_t* dests = nullptr;
    const int    count = api->cupsGetDests( &dests );
    
    String lprinter = inPrinter;
	lprinter.toLower();
	
    for( int i = 0 ; i < count ; ++i )
    {
		String printerInfo( api->cupsGetOption( "printer-info", dests[ i ].num_options, dests[ i ].options ), -1, GetConverter_UTF8() );
		printerInfo.toLower();
		
        if( printerInfo == lprinter )
        {
			r = String( dests[ i ].name, -1, GetConverter_UTF8() );
            break;
        }
    }
    
    api->cupsFreeDests( count, dests );
    
    return r;
}


#endif // _WIN32


//////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark functions
//////////////////////////////////////////////////////////////////////////


/**********************************************************************************************/
#ifdef _WIN32

/**********************************************************************************************/
LPDEVMODEW CreatePrinterDevMode( 
	wchar_t*						pDevice, 
	const std::map<String, String>& opts )
{
	if( !pDevice )
		return NULL;

   HANDLE     hPrinter = 0;
   LPDEVMODEW pDevMode = 0;
   DWORD      dwNeeded = 0;
   DWORD	  dwRet	   = 0;

   if( !OpenPrinterW( pDevice, &hPrinter, NULL ) )
       return NULL;

   /*
    * Step 1:
    * Allocate a buffer of the correct size.
    */ 

   dwNeeded = DocumentPropertiesW(
	   NULL,
       hPrinter,       /* Handle to our printer. */ 
       pDevice,        /* Name of the printer. */ 
       NULL,           /* Asking for size, so */ 
       NULL,           /* these are not used. */ 
       0 );            /* Zero returns buffer size. */ 

   pDevMode = (LPDEVMODEW) malloc( dwNeeded );

   /*
    * Step 2:
    * Get the default DevMode for the printer and
    * modify it for your needs.
    */ 

   dwRet = DocumentPropertiesW(
	   NULL,
       hPrinter,
       pDevice,
       pDevMode,       /* The address of the buffer to fill. */ 
       NULL,           /* Not using the input buffer. */ 
       DM_OUT_BUFFER); /* Have the output buffer filled. */ 

   if( dwRet != IDOK )
   {
       /* If failure, cleanup and return failure. */ 
       free( pDevMode );
       ClosePrinter( hPrinter );

       return NULL;
   }

   /*
    * Make changes to the DevMode which are supported.
    */ 

	auto it = opts.find( "color" );
	if( it != opts.end() )
		pDevMode->dmColor = it->second == "yes" ? DMCOLOR_COLOR : DMCOLOR_MONOCHROME;

	it = opts.find( "collate" );
	if( it != opts.end() )
		pDevMode->dmCollate = it->second == "yes" ? DMCOLLATE_TRUE : DMCOLLATE_FALSE;
	
	it = opts.find( "copies" );
	if( it != opts.end() )
		pDevMode->dmCopies = (short) it->second.parseLLong();

	it = opts.find( "landscape" );
	if( it != opts.end() )
		pDevMode->dmOrientation = it->second == "yes" ? DMORIENT_LANDSCAPE : DMORIENT_PORTRAIT;

	it = opts.find( "duplex" );
	if( it != opts.end() )
	{
		pDevMode->dmDuplex = it->second == "yes" ? 
			pDevMode->dmOrientation == DMORIENT_LANDSCAPE ? DMDUP_HORIZONTAL : DMDUP_VERTICAL :
			DMDUP_SIMPLEX;
	}
	
	it = opts.find( "papersize" );
	if( it != opts.end() )
	{
		pDevMode->dmPaperSize = (short) it->second.parseLLong();
	}
	else
	{
		it = opts.find( "paper" );
		if( it != opts.end() )
		{
			const short paper = GetPaperSizeID( StFromUTF16( it->second, GetConverter_UTF8() ).c_str() );
			if( paper > 0 )
				pDevMode->dmPaperSize = paper;
		}
	}

	it = opts.find( "printquality" );
	if( it != opts.end() )
		pDevMode->dmPrintQuality = (short) it->second.parseLLong();

	it = opts.find( "scaling" );
	if( it != opts.end() )
		pDevMode->dmScale = (short) it->second.parseLLong();

   /*
    * Step 3:
    * Merge the new settings with the old.
    * This gives the driver an opportunity to update any private
    * portions of the DevMode structure.
    */ 

    dwRet = DocumentPropertiesW(
	   NULL,
       hPrinter,
       pDevice,
       pDevMode,       /* Reuse our buffer for output. */ 
       pDevMode,       /* Pass the driver our changes. */ 
       DM_IN_BUFFER |  /* Commands to Merge our changes and */ 
       DM_OUT_BUFFER); /* write the result. */ 

   /* Finished with the printer */ 
   ClosePrinter( hPrinter );

   if( dwRet != IDOK )
   {
       /* If failure, cleanup and return failure. */ 
       free( pDevMode );
       return NULL;
   }

   /* Return the modified DevMode structure. */ 
   return pDevMode;
}

/**********************************************************************************************/
#endif // _WIN32

/**********************************************************************************************/
String GetDefaultSystemPrinter( void )
{
#ifdef _WIN32
	
	wchar_t szPrinterName[ MAX_PATH ];
	unsigned long lPrinterNameLength = MAX_PATH;

	if( GetDefaultPrinterW( szPrinterName, &lPrinterNameLength ) )
		return StW2U( szPrinterName ).u_str();

	return String();
			
#else // _WIN32
	
	const CupsAPI* const api = GetCupsAPI();
	if( !api )
		return String();
	
	String r;
    
    const char* s = api->cupsGetDefault();
    if( s )
		r = String( s, -1, GetConverter_UTF8() );

    if( !r.length() )
    {
        // Try to get user-defined default printer from "cupsGetDests"
        
        cups_dest_t* dests = nullptr;
        const int    count = api->cupsGetDests( &dests );
        
        for( int i = 0 ; i < count ; ++i )
        {
            if( dests[ i ].is_default )
            {
				r = String( dests[ i ].name, -1, GetConverter_UTF8() );
                break;
            }
        }
        
        api->cupsFreeDests( count, dests );
    }
    
	return r;	
	
#endif // _WIN32
}

/**********************************************************************************************/
ArrayOfStringsObj_Ptr GetPrintersNames( void )
{
	FBL_ENGINE_LOCK
	
	ArrayOfStringsObj_Ptr r = new ArrayOfStringsObj;
	
#ifdef _WIN32
	
	wchar_t def[ MAX_PATH ];
	unsigned long defLength = MAX_PATH;

	if( !GetDefaultPrinterW( def, &defLength ) )
		def[ 0 ] = 0;
			
	DWORD count;
	DWORD needed;
	EnumPrintersW( PRINTER_ENUM_FAVORITE | PRINTER_ENUM_LOCAL, NULL, 2, NULL, 0, &needed, &count );
 
	LPPRINTER_INFO_2W info = (LPPRINTER_INFO_2W) malloc( needed );
 
	if( EnumPrintersW( PRINTER_ENUM_FAVORITE | PRINTER_ENUM_LOCAL, NULL, 2, (LPBYTE) info, needed, &needed, &count ) )
	{
		for( DWORD i = 0 ; i < count ; ++i )
		{
			const wchar_t* name = info[ i ].pPrinterName;
			if( !name )
				continue;
			
			if( name == def )
				r->InsertAfterAt( StW2U( name ).u_str(), 0 );
			else
				r->AddItem( StW2U( name ).u_str() );
		}
	}

	free( info );

#else // _WIN32
	
	const CupsAPI* const api = GetCupsAPI();
	if( !api )
		return r;
	
	cups_dest_t* dests = nullptr;
	const int    count = api->cupsGetDests( &dests );

	for( int i = 0 ; i < count ; ++i )
	{
		FBL::String name( dests[ i ].name, -1, GetConverter_UTF8() );
		
		if( dests[ i ].is_default )
			r->InsertAfterAt( std::move( name ), 0 );
		else
			r->AddItem( std::move( name ) );
	}
        
    api->cupsFreeDests( count, dests );
    
#endif // _WIN32

	return r;	
}
	
/**********************************************************************************************/
std::map<String, String> ParsePrintOptions(
	VSQL::I_VReport_Ptr	inReport,
	const FBL::String&	inOptions )
{
	if( !inReport )
		return std::map<String, String>();
	
	std::map<String, String> r;

	// Default options

	r[ "fit-to-page" ] = "yes";
	r[ "landscape"	] = inReport->get_PaperOrientationLandscape() ? "yes" : "no";
	r[ "paper"		] = inReport->get_DesignPaperFormat();

	// Options from string

	for( String& o : SplitOptions( inOptions ) )
	{
		const vint32 i = o.find( '=' );
		if( i > 0 )
			r[ o.mid( 0, i ).trim().toLower() ] = o.mid( i + 1 ).trim();
	}	
	
	return r;
}

/**********************************************************************************************/
int SendReportToPrinter(
	VSQL::I_VReport_Ptr	inReport,
	const FBL::String&	inPrinter,
	const FBL::String&	inOptions,
	vuint32				inStartPageIndex,
	vuint32				inEndPageIndex )
{
	if( !inReport )
		return 0;
	
	FBL_ENGINE_LOCK

#ifdef _WIN32

	static HMODULE reportDLL = LoadLibraryA( GetVReportDllName() );
	static FARPROC proc      = reportDLL ? GetProcAddress( reportDLL, "SendReportToWindowsPrinterC" ) : 0;

	if( proc )
	{
		int (*pfunc)( VSQL::I_VReport*, const char*, const char*, vuint32, vuint32 );

		pfunc = (decltype(pfunc)) proc; 

		return pfunc( 
			inReport.get(), 
			StFromUTF16( inPrinter, GetConverter_UTF8() ).c_str(),
			StFromUTF16( inOptions, GetConverter_UTF8() ).c_str(),
			inStartPageIndex,
			inEndPageIndex );
	}

	FBL_Throw( xFeatureError( ERR_FEATURE_NOT_SUPPORTED, "You need VReport library to print locally report from Valentina Server on Windows" ) );
	
	return 0;

#else // _WIN32

	// Parse options
	
	std::map<String, String> opts = ParsePrintOptions( inReport, inOptions );

	// Process/update options

	double scaleK = 1.;
	
	{
		auto it = opts.find( "scaling" );
		if( it != opts.end() )
		{
			const int scaling = it->second.parseLLong();
			if( scaling > 0 && scaling < 10000 )
				scaleK = scaling / 100.;
			
			opts.erase( it );
		}
	}
	    
	auto it = opts.find( "duplex" );
	if( it != opts.end() && it->second == "yes" )
	{
		bool landscape = false;
		
		it = opts.find( "orientation-requested" );
		if( it != opts.end() && it->second == "4" )
			landscape = true;

		it = opts.find( "landscape" );
		if( it != opts.end() && it->second == "yes" )
			landscape = true;
		
		opts[ "sides" ] = landscape ? "two-sided-short-edge" : "two-sided-long-edge";
	}
	
	it = opts.find( "paper" );
	if( it != opts.end() && opts.find( "pagesize" ) == opts.end() )
		opts[ "pagesize" ] = it->second;
	
	it = opts.find( "reverse" );
	if( it != opts.end() && it->second == "yes" && opts.find( "outputorder" ) == opts.end() )
		opts[ "outputorder" ] = "reverse";
	
    I_Disk_Location_Ptr tmpLocation = I_Disk_Location::GetTempFile( "vreport" );
	const String        tmpFile		= tmpLocation ? tmpLocation->get_Path() : String();

	const int oldZoom = inReport->get_PrintZoom();
	
	try
	{
		inReport->set_PrintZoom( 100 * scaleK );
		
		inReport->PrintToDisk( tmpLocation, ReportPrintType::kToPDF, inStartPageIndex, inEndPageIndex );
	}
	catch(...)
	{
		inReport->set_PrintZoom( oldZoom );
		throw;
	}

	inReport->set_PrintZoom( oldZoom );
	
	String jobName( "vreport" );
	jobName.addLLong( clock() );
	
	const int id = SendPDFToPrinter( jobName, opts, tmpFile, inPrinter );

	unlink( StFromUTF16( tmpFile, GetConverter_UTF8() ).c_str() );
	
	return id;

#endif // _WIN32	
}

/**********************************************************************************************/
#ifndef _WIN32

/**********************************************************************************************/
int SendPDFToPrinter( 
	const String&					inJobName,
	const std::map<String, String>&	inOptions,
	const String&					inFilePDF,
	const String&					inPrinter )
{
	FBL_ENGINE_LOCK
	
	const CupsAPI* const api = GetCupsAPI();
	if( !api )
		return 0;

	int            num_options	= 0;
	cups_option_t* options		= nullptr;;

	for( const auto& it : inOptions )
	{
		if( it.first.length() )
		{
			num_options = api->cupsAddOption( 
				StFromUTF16( it.first, GetConverter_UTF8() ).c_str(), 
				StFromUTF16( it.second, GetConverter_UTF8() ).c_str(),
				num_options,
				&options );
		}
	}
	
	String printer = inPrinter;
    if( printer.length() )
        printer = GetCupsPrinterNameByDescription( printer );
	
	if( !printer.length() )
		printer = GetDefaultSystemPrinter();
	
	int rc;
	
	{	
		FBL_ENGINE_UNLOCK
		
		StFromUTF16 u8printer( printer, GetConverter_UTF8() );
		StFromUTF16 u8filePDF( inFilePDF, GetConverter_UTF8() );
		StFromUTF16 u8jobName( inJobName, GetConverter_UTF8() );
	
		rc = api->cupsPrintFile( 
			u8printer.c_str(),
			u8filePDF.c_str(),
			u8jobName.c_str(),
			num_options,
			options );
	}

	api->cupsFreeOptions( num_options, options );
	
	return rc;
}

/**********************************************************************************************/
#endif // _WIN32


/**********************************************************************************************/
FBL_End_Namespace
