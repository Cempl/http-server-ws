/**********************************************************************************************/
/* VSDK_Objects.h			                                                   				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/
//
// This file is included in the end of VSDK2.h header, 
// so no need do any includes here.
//
/**********************************************************************************************/
FBL_Begin_Namespace

using namespace vsql;


/**********************************************************************************************/
class VDK_Database
{
	public://///////////////////////////////////////////////////////////////////////////////////
 
							VDK_Database( EStorageType 	inStorage = kStorage_Disk );
							VDK_Database( I_Connection_Ptr	inpConnection );


// Access to FBL::I_Database interface:	
		
	FBL::I_Database_Ptr 		operator->( void )		{ return mpFblDatabase; }			
	FBL::Const_I_Database_Ptr	operator->( void ) const{ return mpFblDatabase; }

	FBL::I_Database_Ptr 		get( void )				{ return mpFblDatabase; }			
	FBL::Const_I_Database_Ptr	get( void ) const		{ return mpFblDatabase; }


// Access to I_SqlDatabase interface:	

	I_SqlDatabase_Ptr			ToSql( void )			{ return mpSqlDatabase; }			
	VSQL::Const_I_SqlDatabase_Ptr ToSql( void ) const	{ return mpSqlDatabase; }


	protected:////////////////////////////////////////////////////////////////////////////////// 

	I_SqlDatabase_Ptr		mpSqlDatabase;
	FBL::I_Database*		mpFblDatabase;
};


/**********************************************************************************************/
class VDK_Table
{
	public://///////////////////////////////////////////////////////////////////////////////////
 
							VDK_Table( const String& inName );

		I_Table_Ptr 		operator->( void )			{ return mpTable; }			
		Const_I_Table_Ptr 	operator->( void ) const	{ return mpTable; }

		I_Table_Ptr			get( void )					{ return mpTable; }
		Const_I_Table_Ptr	get( void ) const			{ return mpTable; }

	protected:////////////////////////////////////////////////////////////////////////////////// 

 		I_Table_Ptr			mpTable;  
};


/**********************************************************************************************/
// This class cannot be instantiated directly, so we make protected constructor.
//
class VDK_Field
{
	protected:////////////////////////////////////////////////////////////////////////////////// 
 
							VDK_Field( 
								I_Field_Ptr inFld );

	public://///////////////////////////////////////////////////////////////////////////////////

		I_Field_Ptr 		operator->( void )			{ return mpField; }			
		Const_I_Field_Ptr 	operator->( void ) const	{ return mpField; }

		I_Field_Ptr 		get( void )					{ return mpField; }			
		Const_I_Field_Ptr	get( void ) const			{ return mpField; }
	
	protected:////////////////////////////////////////////////////////////////////////////////// 

 		I_Field_Ptr			mpField;  
};


#pragma mark -


/**********************************************************************************************/
#define VDK_NUMERIC_CLASS_DECL(cname)												\
class cname : public VDK_Field														\
{																					\
	public:	cname(																	\
				const String&	inName,												\
				FieldAttributes inFlags = fNone,									\
				const UChar*	inMethodText = NULL );								\
};


/**********************************************************************************************/
VDK_NUMERIC_CLASS_DECL( VDK_Boolean );
VDK_NUMERIC_CLASS_DECL( VDK_Byte );
VDK_NUMERIC_CLASS_DECL( VDK_Short );
VDK_NUMERIC_CLASS_DECL( VDK_UShort );
VDK_NUMERIC_CLASS_DECL( VDK_Medium );
VDK_NUMERIC_CLASS_DECL( VDK_UMedium );
VDK_NUMERIC_CLASS_DECL( VDK_Long );
VDK_NUMERIC_CLASS_DECL( VDK_ULong );
VDK_NUMERIC_CLASS_DECL( VDK_LLong );
VDK_NUMERIC_CLASS_DECL( VDK_ULLong );
VDK_NUMERIC_CLASS_DECL( VDK_Float );
VDK_NUMERIC_CLASS_DECL( VDK_Double );

VDK_NUMERIC_CLASS_DECL( VDK_Date );
VDK_NUMERIC_CLASS_DECL( VDK_Time );
VDK_NUMERIC_CLASS_DECL( VDK_DateTime );


/**********************************************************************************************/
class VDK_String : public VDK_Field
{
	public://///////////////////////////////////////////////////////////////////////////////////
 
								VDK_String( 
									const String&	inName,
									vuint32			inMaxLength = 20, 
									FieldAttributes inFlags = fNone,
									const UChar*	inMethodText = NULL );

// Access to FBL::I_FldString interface:

	FBL::I_FldString_Ptr		ToIString( void );
	FBL::Const_I_FldString_Ptr	ToIString( void ) const;

	protected:////////////////////////////////////////////////////////////////////////////////// 

mutable	I_FldString*			mpFldString;  
};

/**********************************************************************************************/
class VDK_VarChar : public VDK_Field
{
	public://///////////////////////////////////////////////////////////////////////////////////
 
								VDK_VarChar( 
									const String&	inName,
									vuint32			inMaxLength = 2044, 
									FieldAttributes inFlags = fNone,
									const UChar*	inMethodText = NULL );

// Access to FBL::I_FldString interface:

	FBL::I_FldString_Ptr		ToIString( void );
	FBL::Const_I_FldString_Ptr	ToIString( void ) const;

	protected:////////////////////////////////////////////////////////////////////////////////// 

mutable	I_FldString*			mpFldString;  
};


/**********************************************************************************************/
class VDK_Text : public VDK_Field
{
	public://///////////////////////////////////////////////////////////////////////////////////
 
								VDK_Text( 
									const String&	inName,
									vuint32			inSegmentSize = 512, 
									FieldAttributes inFlags = fNone,
									const UChar*	inMethodText = NULL );

// Access to FBL::I_FldString interface:

	FBL::I_FldString_Ptr		ToIString( void );
	FBL::Const_I_FldString_Ptr	ToIString( void ) const;

// Access to FBL::I_FldBLOB interface:

	FBL::I_FldBlob_Ptr			ToIBlob( void );
	FBL::Const_I_FldBlob_Ptr	ToIBlob( void ) const;

	protected:////////////////////////////////////////////////////////////////////////////////// 

mutable	I_FldString*			mpFldString;  
mutable	I_FldBlob*				mpFldBlob; 
};


/**********************************************************************************************/
class VDK_BLOB : public VDK_Field
{
	public://///////////////////////////////////////////////////////////////////////////////////
 
								VDK_BLOB( 
									const String&	inName,
									vuint32			inSegmentSize = 512 );

// Access to FBL::I_FldBLOB interface:

	FBL::I_FldBlob_Ptr			ToIBlob( void );
	FBL::Const_I_FldBlob_Ptr	ToIBlob( void ) const;

	protected:////////////////////////////////////////////////////////////////////////////////// 

mutable	I_FldBlob*				mpFldBlob; 
};


/**********************************************************************************************/
class VDK_Picture : public VDK_Field
{
	public://///////////////////////////////////////////////////////////////////////////////////
 
								VDK_Picture( 
									const String&	inName,
									vuint32			inSegmentSize = 512 );

// Access to FBL::I_FldBLOB interface:

	FBL::I_FldBlob_Ptr			ToIBlob( void );
	FBL::Const_I_FldBlob_Ptr	ToIBlob( void ) const;

// Access to FBL::I_FldPicture interface:

	FBL::I_FldPicture_Ptr		ToIPicture( void );
	FBL::Const_I_FldPicture_Ptr	ToIPicture( void ) const;


	protected:////////////////////////////////////////////////////////////////////////////////// 

mutable	I_FldBlob*				mpFldBlob; 
mutable	I_FldPicture*			mpFldPicture; 
};


/**********************************************************************************************/
class VDK_ObjectPtr : public VDK_Field
{
	public://///////////////////////////////////////////////////////////////////////////////////
 
							VDK_ObjectPtr( 
								const String&	inName,
								VDK_Table*		inTarget,
								EOnDeletion		inOnDeletion = kOnDelete_Cascade,
								FieldAttributes inFlags = FieldAttributes(fIndexed + fNullable),
								const String&	inLinkName = String::sEmpty() );

// Access to FBL::I_FldObject interface:

	I_FldObjectPtr_Ptr			ToIObjectPtr( void );
	Const_I_FldObjectPtr_Ptr	ToIObjectPtr( void ) const;


	protected:////////////////////////////////////////////////////////////////////////////////// 

mutable	I_FldObjectPtr*		mpFldObjectPtr; 
};


/**********************************************************************************************/
class VDK_BinaryLink
{
	public://///////////////////////////////////////////////////////////////////////////////////
 
							VDK_BinaryLink( 
								const String&	inName,
								VDK_Table&		inLeftTable,
								VDK_Table&		inRightTable,
								LinkType		inLeftPower = kOne,
								LinkType		inRightPower = kMany,
								EOnDeletion		inOnDelete = kOnDelete_SetNull,
								EStorageType	inStorageType = kStorage_Default );
							

// Access to FBL::I_FldObject interface:

		I_Link_Ptr			operator->( void )			{return mpLink; }
		Const_I_Link_Ptr	operator->( void ) const	{return mpLink; }

// Access to FBL::I_FldObject interface:

		I_Link2_Ptr			ToILink2( void );
		Const_I_Link2_Ptr	ToILink2( void ) const;

	protected:////////////////////////////////////////////////////////////////////////////////// 

		I_Link_Ptr			mpLink; 
mutable	I_Link2*			mpLink2;
};


/**********************************************************************************************/
FBL_End_Namespace
