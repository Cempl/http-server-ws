/**********************************************************************************************/
/* VSQL_QueryResult.h                                    	                 				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _VSQL_QueryResult_h 
	#define _VSQL_QueryResult_h
#pragma once

#include <VShared/FBL/publ/Util_classes/FBL_ForeignPlugin_Imp.h>

#include <VShared/VSQL/Interfaces/VSQL_I_SqlDatabase.h>
#include <VShared/VSQL/Interfaces/VSQL_I_Cursor.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
VSQL_Begin_Namespace


/**********************************************************************************************/
// THIS module declarations:
//
SMART_CLASS(QueryResult);


/**********************************************************************************************/
// This class is for Database::SqlQuery() method result.
// 
class FBL_SHARED_EXP_CLASS QueryResult :
	public ForeignPlugin_imp
{	
		IMPLEMENT_UNKNOWN(QueryResult)											
		BEGIN_INTERFACE_TABLE(QueryResult)
			IMPLEMENT_INTERFACE(I_ForeignPlugin)
		END_INTERFACE_TABLE()
 
	public://///////////////////////////////////////////////////////////////////////////////////
	
							QueryResult( void );
							QueryResult( const vuint16		inOptions );
							QueryResult( const QueryResult& inQueryResult );

virtual						~QueryResult( void );

	
	public://///////////////////////////////////////////////////////////////////////////////////

		//
		// Deprecated! Use hasType() instead in order to know all the types this result has.
		// http://www.valentina-db.com/bt/view.php?id=5516
		//
		EQueryResult		get_Type( void ) const;

		bool				hasType( EQueryResult inResultType ) const;
		
		EQueryResultEx		get_TypeEx( void ) const;

		vuint16				get_Options() const;

		EQueryFlags			get_Flags() const;

		void				SetFlags( EQueryFlags inFlag );

			// <Cursor>	
			
		I_Cursor_Ptr		get_AsCursor( void ) const;
		void				put_AsCursor( I_Cursor_Ptr	inpCursor );		

			// <Boolean>	
			
		bool				get_AsBoolean( void ) const;
		void				put_AsBoolean( bool	inValue );		

			// <ULong>	
			
		vuint32				get_AsULong( void ) const;
		void				put_AsULong( vuint32 inValue );
				
			// <String>
		const String*		get_AsString( void ) const;

							// pass ownership of inpValue to QueryReult!
		void				put_AsString( String* inpValue );
		
							// Creates a new String object and copies inValue string.
		void				put_AsString( const String&	inValue );

		REC_ID				get_LastRecID( void ) const;
		void				put_LastRecID( REC_ID inRecID );

			// <Logs>

		ArrayOfStringsObj_Ptr	get_Warnings( void ) const; 	
		void					put_Warnings( ArrayOfStringsObj_Ptr inpWarnings ); 

		ArrayOfStringsObj_Ptr	get_Tunes( void ) const; 	
		void					put_Tunes( ArrayOfStringsObj_Ptr inpTunes ); 
	
		ArrayOfStringsObj_Ptr	get_Outputs( void ) const;
		void					put_Outputs( ArrayOfStringsObj_Ptr inpOutputs ); 

			// <Explanation>

		const String*			get_Explanation( void ) const; 	
		
								// pass ownership of inExplainStr to QueryResult!
		void					put_Explanation( String* inExplainStr );

		DTFormat&				get_DTFormat( void )
									{ return mDTFormat; }


	protected://////////////////////////////////////////////////////////////////////////////////

	// Result type:

		EQueryResult			mResultType;		// For deprecated get_Type()
		EQueryResultEx			mResultTypeEx;		// Multiple query result.
		EQueryFlags				mFlags;
		vuint16					mOptions;

	// Result data:

		I_Cursor_Ptr			mpCursor;
		bool					mBoolValue;
		vuint32					mUlongValue;
		String*					mpStringValue;
		REC_ID					mLastRecID;

		ArrayOfStringsObj_Ptr	mpWarnings;
		ArrayOfStringsObj_Ptr	mpTunes;
		ArrayOfStringsObj_Ptr	mpOutputs;
		
	// Explaination of result:
		
		String*					mpExplainStr;	// xml format.

		DTFormat				mDTFormat;
};


/**********************************************************************************************/
VSQL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _VSQL_QueryResult_h
