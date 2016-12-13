/**********************************************************************************************/
/* FBL_XML.cpp																				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>
#include <VShared/FBL/publ/Util_classes/FBL_String.h>
#include <VShared/FBL/publ/Util_classes/FBL_StringConversion.h>
#include <VShared/FBL/publ/Values/FBL_Value_String.h>
#include <VShared/FBL/publ/Values/FBL_Value_Numeric.h>
#include <VShared/FBL/publ/Values/FBL_Value_Double.h>

#include <VShared/FBL/publ/Utilities/FBL_XML.h>

#include <stdlib.h>

/**********************************************************************************************/
FBL_Begin_Namespace

/**********************************************************************************************/
XML::XML( const String&	inXMLStr )
{
	mpXMLDoc = new pugi::xml_document();
	
	pugi::xml_parse_result res = mpXMLDoc->load_string( inXMLStr.getBufferA() );
	if( res == false )
		mError << res.description();
}


/**********************************************************************************************/
XML::~XML()
{
	ForgetObject( mpXMLDoc );
}


/**********************************************************************************************/
bool XML::isValid( void )
{
	return mError.isEmpty();
}


/**********************************************************************************************/
String XML::get_ErrorStr( void )
{
	return mError;
}


/**********************************************************************************************/
String XML::get_Value( const char* inpPath )
{
	String res;	
	if( mpXMLDoc )
	{
		pugi::xpath_node_set nodeset = mpXMLDoc->select_nodes( inpPath );
		pugi::xpath_node node = nodeset.first();
		pugi::xml_node node_value = node.node().first_child();
		
		res << node_value.value();
	}
	return res;
}


/**********************************************************************************************/
ArrayOfValueArrays_Ptr XML::get_Matrix( const char* inpPath )
{
	ArrayOfValueArrays_Ptr res;
	
	if( mpXMLDoc )
	{
		// "//Root/Params"
		pugi::xpath_node_set root = mpXMLDoc->select_nodes( inpPath );
		for( pugi::xpath_node_set::const_iterator it = root.begin(); it != root.end(); ++it )
		{
			// "//Root/Params/Row"
			pugi::xml_node rowNode = it->node();
			for( pugi::xml_node params = rowNode.first_child(); params; params = rowNode.next_sibling() )
			{
				ArrayOfValues_Ptr pValueRow = new ArrayOfValues();

				for( pugi::xml_node param = params.first_child(); param; param = params.next_sibling() )
				{
					// For each "//Root/Params/Row/Param"
					I_Value_Ptr pValue;
					String paramType = param.attribute("Type").value();
					if( paramType.caseCompare("bool",0) == 0 )
					{
						pValue = new Value_bool();
						pValue->put_Boolean( param.text().as_bool() );
					}
					else if( paramType.caseCompare("null",0) == 0 )
					{
						pValue = new Value_bool_null();
						pValue->put_IsNull(true);
					}
					else if( paramType.caseCompare("integer",0) == 0 )
					{
						pValue = new Value_llong();
						pValue->put_LLong( param.text().as_llong() );
					}
					else if( paramType.caseCompare("float",0) == 0 )
					{
						pValue = new Value_double();
						pValue->put_Double( param.text().as_double() );
					}
					else
					{
						// default:
						pValue = new Value_string();
						pValue->put_String( param.text().as_string() );
					}
					
					pValueRow->AddItem( pValue );
				}
				
				if( res == nullptr )
					res = new ArrayOfValueArrays();
				res->AddItem(pValueRow);
			}
		}
	}
	
	return res;
}


/**********************************************************************************************/
FBL_End_Namespace
