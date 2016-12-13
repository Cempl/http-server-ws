/**********************************************************************************************/
/* FBL_DumpResult.h																			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_DumpResult_h
	#define _FBL_DumpResult_h
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
SMART_CLASS( DumpResult );


/**********************************************************************************************/
// This class is for Database::LoadDumpEx() method result.
// 
class FBL_SHARED_EXP_CLASS DumpResult :
	public ForeignPlugin_imp
{	
		IMPLEMENT_UNKNOWN(DumpResult)
		BEGIN_INTERFACE_TABLE(DumpResult)
			IMPLEMENT_INTERFACE(I_ForeignPlugin)
		END_INTERFACE_TABLE()
 
	public://///////////////////////////////////////////////////////////////////////////////////
	
							DumpResult( void );
							DumpResult( const vuint16 inOptions );
							DumpResult( const DumpResult& inDumpResult );

virtual						~DumpResult( void );

	
	public://///////////////////////////////////////////////////////////////////////////////////

		vuint16				get_Options() const;

			// <SuccessfulCount>
			
		vuint32				get_SuccessfulCount( void ) const;
		void				put_SuccessfulCount( vuint32 inValue );

			// <FailedCount>
			
		vuint32				get_FailedCount( void ) const;
		void				put_FailedCount( vuint32 inValue );
	

			// <Logs>

		ArrayOfStringsObj_Ptr	get_Warnings( void ) const; 	
		void					put_Warnings( ArrayOfStringsObj_Ptr inpWarnings ); 
	
		ArrayOfStringsObj_Ptr	get_Outputs( void ) const;
		void					put_Outputs( ArrayOfStringsObj_Ptr inpOutputs ); 


	protected://////////////////////////////////////////////////////////////////////////////////

	// Result type:

		vuint16					mOptions;

	// Result data:

		vuint32					mSuccessfulCount;
		vuint32					mFailedCount;

		ArrayOfStringsObj_Ptr	mpWarnings;
		ArrayOfStringsObj_Ptr	mpOutputs;
		
};


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_DumpResult_h

