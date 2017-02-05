/**********************************************************************************************/
/* FBL_ImportResult.h																		  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_ImportResult_h
	#define _FBL_ImportResult_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Util_classes/FBL_ForeignPlugin_Imp.h>
#include <VShared/FBL/publ/Util_classes/FBL_String.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace

/**********************************************************************************************/
SMART_CLASS( ImportResult );


/**********************************************************************************************/
// This class is for Table::Import() method result.
// 
class FBL_SHARED_EXP_CLASS ImportResult :
	public ForeignPlugin_imp
{	
		IMPLEMENT_UNKNOWN(ImportResult)											
		BEGIN_INTERFACE_TABLE(ImportResult)
			IMPLEMENT_INTERFACE(I_ForeignPlugin)
		END_INTERFACE_TABLE()
 
	public://///////////////////////////////////////////////////////////////////////////////////
	
							ImportResult( void );
							ImportResult( const vuint16		inOptions );
							ImportResult( const ImportResult& inImportResult );

virtual						~ImportResult( void );

	
	public://///////////////////////////////////////////////////////////////////////////////////

		vuint16				get_Options() const;

			// <ULong>	
			
		vuint32				get_AsULong( void ) const;
		void				put_AsULong( vuint32 inValue );
				

			// <Logs>

		ArrayOfStringsObj_Ptr	get_Warnings( void ) const; 	
		void					put_Warnings( ArrayOfStringsObj_Ptr inpWarnings ); 
	
		ArrayOfStringsObj_Ptr	get_Outputs( void ) const;
		void					put_Outputs( ArrayOfStringsObj_Ptr inpOutputs ); 


	protected://////////////////////////////////////////////////////////////////////////////////

	// Result type:

		vuint16					mOptions;

	// Result data:

		vuint32					mUlongValue;

		ArrayOfStringsObj_Ptr	mpWarnings;
		ArrayOfStringsObj_Ptr	mpOutputs;
		
};


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_Export_h

