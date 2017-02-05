/**********************************************************************************************/
/* FBL_XML.h																				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_XML_h
	#define _FBL_XML_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Value.h>
#include <VShared/FBL/publ/Utilities/pugiXML/pugixml.hpp>

// STD:
#include <stdint.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>

/**********************************************************************************************/
// See
// https://github.com/zeux/pugixml
//

/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// Another XML parser (simple + UTF8 support)
//

/**********************************************************************************************/
struct FBL_SHARED_EXP_CLASS XML
{
	public://///////////////////////////////////////////////////////////////////////////////////

								XML( const String& inXMLStr );
	
virtual							~XML();

	public://///////////////////////////////////////////////////////////////////////////////////
	
		bool						isValid( void );
		
		String						get_ErrorStr( void );

		String						get_Value( const char* inpPath );

		ArrayOfValueArrays_Ptr		get_Matrix( const char* inpPath );

	
	protected://////////////////////////////////////////////////////////////////////////////////
	
		pugi::xml_document*			mpXMLDoc;
		String						mError;
};


/**********************************************************************************************/
FBL_End_Namespace

#endif // _FBL_XML_h
