/**********************************************************************************************/
/* FBL_File.cpp                                                         					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/File_OS/FBL_File.h>


/**********************************************************************************************/
FBL_Begin_Namespace


#pragma mark -


/**********************************************************************************************/
File::File( void ) : 	
	AspectHost_imp( 3 )
{
	ClearDataMembers();
}


/**********************************************************************************************/
File::~File( void ) 
{
}

/**********************************************************************************************/
void File::put_IsReadOnly( bool inValue )
{
	FBL_ENGINE_LOCK
	if( inValue )
		mReadOnly |= fInternalRO;
	else
		mReadOnly &= ~fInternalRO;
}


/**********************************************************************************************/
vuint32 File::Read( 
	char* 		inBuffer,
	flength		inFrom, 
	vuint32 		inHowMuch  )
{
	// DoRead() will throw EOF in case of error.
	vuint32 count = DoRead( inBuffer, inFrom, inHowMuch );

	mPosition = inFrom + count;
	FBL_CHECK( mPosition <= mLength ); 

	return count;
}


/**********************************************************************************************/
vuint32 File::Write( 
	 const char*	inBuffer,
	 flength		inFrom, 
	 vuint32 			inHowMuch  )
{
	if( mLength == flength(-1) || mLength < inFrom )
	{
		if(inFrom != 0) 
			put_Length( inFrom );
	}

	vuint32 count;
	
/*	if( inFrom == mPosition )
		count = DoWrite( inBuffer, inHowMuch );			
	else	*/
		count = DoWrite( inBuffer, inFrom, inHowMuch );

	if( mLength == flength(-1) || mLength < inFrom + inHowMuch )
		mLength = inFrom + count;

	mPosition = inFrom + count;
	
	return count;
}


#pragma mark -
#pragma mark === I_Storage ====


/**********************************************************************************************/
flength	File::get_Size( void ) const  
{
	FBL_ENGINE_LOCK
	if( mLength == flength(-1) )
		return 0;
	else
		return mLength; 
}
 
/**********************************************************************************************/
void File::Close( void )
{
	FBL_ENGINE_LOCK
	if( get_IsOpen() )
	{
		DoClose();
	}
	else
	{
		//String path = get_Path();
	 	//FBL_Throw( xOSFileError(ERR_OS_FILE_NOT_OPEN, path.c_str() ) );
	}
}


/**********************************************************************************************/
void File::Create( void )
{
	FBL_ENGINE_LOCK
	if( get_IsOpen() == true )
	{
		String path = get_Path();
	 	FBL_Throw( xOSFileError(ERR_OS_FILE_OPEN, path.c_str() ) );	
	}

	if( get_Exists() )
		ThrowOut();

	ClearDataMembers();
	DoCreate();
		
	FBL_CHECK( get_IsOpen() == true );
}


/**********************************************************************************************/
void File::Open( void )
{
	FBL_ENGINE_LOCK
	if( get_IsOpen() == true )
	{
		String path = get_Path();
	 	FBL_Throw( xOSFileError(ERR_OS_FILE_OPEN, path.c_str() ) );	
	}

	DoOpen();
		
	mLength = DoGetLengthOfOpened();	
}


/**********************************************************************************************/
void File::ThrowOut( void )
{
	FBL_ENGINE_LOCK
	// If there is no such file, there is nothing to delete...
	if( !get_Exists() )	
		//FBL_Throw( xOSFileError(ERR_OS_FILE_NOT_FOUND, mpLocation ? mpLocation->get_Path().c_str() : NULL) );
		return;
	
	// We can't delete in ReadOnly file.
	if( get_IsReadOnly() )
	{
		throw xOSFileError( ERR_OS_FILE_READ_ONLY, 
								 mpLocation ? mpLocation->get_Path().c_str() : nullptr	);	
	}
	
	if( get_IsOpen() == true )
	{
	 	throw xOSFileError( ERR_OS_FILE_OPEN,
								 mpLocation ? mpLocation->get_Path().c_str() : nullptr	);	
	}

	DoThrowOut();

	ClearDataMembers();
}


/**********************************************************************************************/
FBL_End_Namespace


