/**********************************************************************************************/
/* VSQL_I_VProject.h 		                                                     			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _VSQL_I_VProject_h 
	#define _VSQL_I_VProject_h
#pragma once

#include <VShared/VSQL/Headers/VSQL.h>

// FBL:
#include <VShared/FBL/publ/Interfaces/FBL_I_Disk_Location.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Value.h>
#include <VShared/FBL/publ/Util_classes/FBL_String.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
VSQL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE(I_VProject);
SMART_INTERFACE(I_VReport);
SMART_INTERFACE(I_Cursor);
SMART_INTERFACE(I_SqlDatabase);


/**********************************************************************************************/
SMART_ARRAY_OF_CLASS_PTRS( ArrayOfProjects, I_VProject* );


/**********************************************************************************************/
//StubInterface
interface FBL_SHARED_EXP_CLASS I_VProject : public I_Unknown
{
	public://////////////////////////////////////////////////////////////////////////

virtual						~I_VProject( void );

	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// Project File Properties:

		// <IsOpen> [r/o]
							// Returns TRUE if storage is opened now. 
							// Must be TRUE after Open(), must be FALSE, after Close().
							// CALLED BY: ProjectManager::Destroy_Projects
virtual	bool				get_IsOpen( void ) const = 0;

		// <IsRemote> [r/o]
							// TRUE if Project is located on server.
							// CALLED BY: ProjectManager::Destroy_Projects
virtual bool				get_IsRemote( void ) const = 0;

		// <IsReadOnly> [r/o]
							// Returns TRUE if storage is for read only.
							// CALLED BY: VSTUDIO
virtual	bool				get_IsReadOnly( void ) const = 0;

		// <Loction>
							// CALLED BY: CreateVProject() factory
virtual	void				put_Location( I_Disk_Location_Ptr inProjectLocation ) = 0;

		// <Name> [r/o]
							// Returns name of project( based on project path/file name )
							// CALLED BY: ProjectManager::get_Project
virtual const String&		get_Name( void ) const = 0;

		// <Path> [r/o]
							// DEPRECATED
virtual const String&		get_Path( void ) const = 0;


	// ---------------------
	// Project File Methods:

							// Create project container - must be called put_Location() before
							// DEPRECATED - because will be called always locally, but not in protocol.
virtual	void				Create( void ) = 0;
virtual	void				Create( bool inRAM ) = 0;

							// Opens project container( database, file etc. )
							// DEPRECATED - because will be called always locally, but not in protocol.
virtual void                Open( void ) = 0;
virtual void				Open( bool inRAM ) = 0;

							// Flushes all changes
							// DEPRECATED - because will be called always locally, but not in protocol.
virtual	void				Flush( void ) = 0;

							// Closes project container( database, file etc. )
							// CALLED BY: ProjectManager::Destroy_Projects
virtual void				Close( void ) = 0;
							

	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// Project Documents:

							// Create empty item 
							// CALLED BY: VSTUDIO
virtual String				CreateDocument( 
								String	inItemPath, 
								String	inItemType, 
								String	inName ) = 0;

							// Returns count of all items
							// CALLED BY: VSTUDIO
virtual	vuint32				GetDocumentCount( void ) = 0;

							// Reads info about item( without data ) to show it in VSTUDIO
							// CALLED BY: VSTUDIO
							// DEPRECATED ARGUMENT: outRestore
virtual	void				GetDocument( 
								vuint32			inRecID, 
								String&			outID, 
								String&			outName, 
								String&			outPath, 
								String&			outType, 
								String&			outComment, 
								bool&			outRestore ) = 0;


	// ---------------------
	// Document Properties:

		// <Comment> [r/w]

							// Changes comment of an item
							// CALLED BY: VSTUDIO
virtual bool				put_Comment( 
								String	inProjectItemID, 
								String	inComment ) = 0;	

		// <Data> [r/w]

							// Changes data of item( XML or plain string )
							// CALLED BY: VSTUDIO
virtual String				get_Data( 
								String	inProjectItemID ) = 0;

							// Returns data of item( XML or plain string )
							// CALLED BY: VSTUDIO
virtual bool				put_Data( 
								String	inProjectItemID, 
								String	inData ) = 0;	

		// <IsRestore>

							// DEPRECATED
virtual bool				put_IsRestore( 
								String			inProjectItemID, 
								bool			inRestore ) = 0;	
								
		// <IsLocked>

							// DEPRECATED
virtual bool				get_IsLocked( 
								String	inProjectItemID ) = 0;
    
        // <LockInfo>  [r/w]
    
                            // Returns lock info of item
                            // CALLED BY: VSTUDIO
virtual String              get_LockInfo(
                                String  inProjectItemID ) = 0;
    
                            // Sets or removes lock for item, returns TRUE on success
                            // CALLED BY: VSTUDIO
virtual bool                put_LockInfo(
                                String  inProjectItemID,
                                String  inLockInfo ) = 0;

		// <Type> [r/o]

							// Returns type of item( "report", "query" etc. )
							// CALLED BY: VSTUDIO
virtual String				get_Type( 
								String	inProjectItemID ) = 0;


	// ---------------------
	// Document Methods:

							// Changes name of item
							// CALLED BY: VSTUDIO
virtual bool				Rename( 
								String	inProjectItemID, 
								String	inNewName ) = 0;
								
							// Changes path of item
							// CALLED BY: VSTUDIO
virtual bool				Move( 
								String	inProjectItemID, 
								String	inNewPath ) = 0;

							// Deletes item with specified Id
							// CALLED BY: VSTUDIO
virtual bool				Delete( 
								String	inProjectItemID ) = 0;


	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// Report Properties:

		// <ReportCount> [r/o]

							// Returns the count of reports inside of the project
							// CALLED BY: ADK
virtual	 vuint32			get_ReportCount( void ) = 0;


	// ---------------------
	// Report Methods:

							// Returns the name of report inside of the project by index
							// CALLED BY: ADK
virtual	 String				ReportName( vuint32 inIndex ) = 0;


	// ---------------------
	// Report Factories (for Valentina DB only):


							// Returns the report at the specified index.
							// Returns NULL if report is not found.
							// inIndex is 1 ..
							// CALLED BY: ADK
virtual	 I_VReport_Ptr		MakeNewReport( 
								vuint32				inReportIndex,
								I_SqlDatabase_Ptr	inDataSource,
								const String&		inQuery = String::sEmpty(),
                                ECursorLocation     inCursorLocation = kClient, 
                                ELockType           inLockType = kReadOnly, 
                                ECursorDirection    inCursorDirection = kForwardOnly,
								const ArrayOfValues_Ptr inBindings = nullptr ) = 0;


							// Returns the report with the specified name.
							// Returns NULL if report is not found.
							// CALLED BY: ADK
virtual	 I_VReport_Ptr		MakeNewReport( 
								const String&		inReportName,
								I_SqlDatabase_Ptr	inDataSource,
								const String&		inQuery = String::sEmpty(),
                                ECursorLocation     inCursorLocation = kClient, 
                                ELockType           inLockType = kReadOnly, 
                                ECursorDirection    inCursorDirection = kForwardOnly,
								const ArrayOfValues_Ptr inBindings = nullptr ) = 0;


	// ---------------------
	// Report Factories (for ANY Supported Datasource):

							// Universal factory, which is able (in ideal) to use 
							// any datasource, supported by VREPORT.
							// 
							// inDataSource should be a string of format:
							//	"ds_name: some_string_with_params"
							//
							// Example:
                            //		"mysql://host=127.0.0.1, user=sa, pass=sa, db=db1, timeout=5"
                            //		"odbc://dsn=access, user=name, pass=123"
							//		"postgresql://host=127.0.0.1, port=5432, dbname=db1, user=sa, password=sa"
                            //		"sqlite://path_to_local_db"
                            //      "valentina://path_to_local_valentina_db"
							//		"vserver://host=192.168.1.100, user=sa, pass=sa, dbname=db1, port=15432, timeout=10, ssl=false, options=ABCDEFG12345678"
							// Notes:
                            //      Parser of this connection string allows you write string literals without single quotes and with single quotes.
                            //      Inside of single-quoted literal you can use  escape character to escape single quote.
                            //      You can use ',', ';' or spaces as delimiters of parameters. Delimiting spaces are allowed only for single-quoted values.

virtual	 I_VReport_Ptr		MakeNewReport( 
								vuint32					inReportIndex,
								const String&			inDataSource,
								const String&			inQuery = String::sEmpty(),
								const ArrayOfValues_Ptr inBindings = nullptr ) = 0;

virtual	 I_VReport_Ptr		MakeNewReport( 
								const String&			inReportName,
								const String&			inDataSource,
								const String&			inQuery = String::sEmpty(),
								const ArrayOfValues_Ptr inBindings = nullptr ) = 0;
							

	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// Report Factories (for VKERNEL):
														
							// Returns the report with the specified name.
							// Returns NULL if report is not found.
							// CALLED BY: Node_Select::ExecuteSqlForReport
virtual	 I_VReport_Ptr		MakeNewReport( 
								const String&		inReportName,
								I_Cursor_Ptr		inpCursor ) = 0;

};


/**********************************************************************************************/
VSQL_End_Namespace


#include <VShared/FBL/publ/Headers/FBL_post_header.h>


#endif // _VSQL_I_VProject_h
