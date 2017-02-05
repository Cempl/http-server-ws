/**********************************************************************************************/
/* FBL_File_RAM.cpp                                                  						  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                       					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/File_OS/ram/FBL_File_RAM.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
vuint32 File_RAM::sTotalSize = 0;


/**********************************************************************************************/
File_RAM::File_RAM( void )
: 
	mPageV( new ArrayOfRamPages() )
{
	mOpened = false;
}


/**********************************************************************************************/
File_RAM::~File_RAM( void )
{
	_Deallocate( ( vuint32 )mPageV->get_Count() );
}
 

#pragma mark -


/**********************************************************************************************/
bool File_RAM::get_IsOpen( void ) const 
{
	FBL_ENGINE_LOCK
	return mOpened;
}


/**********************************************************************************************/
bool File_RAM::get_IsReadOnly( void ) const 
{
	// Always RW.
	return false;
}

/**********************************************************************************************/
void File_RAM::put_IsReadOnly( bool inValue )
{ 
	argused1( inValue );
	FBL_Throw( xFeatureError( ERR_FEATURE_NOT_SUPPORTED, "Read-only access for RAM file" ) );
}

/**********************************************************************************************/
bool File_RAM::ExistsOnDisk( void ) const
{
	if( mPageV->get_Count() > 0 )
		return true;
	else
		return false;
}


/**********************************************************************************************/
void File_RAM::Flush( void )
{
}


/**********************************************************************************************/
vuint8* File_RAM::GetPageDataFromPos( vuint32 inPos ) 
{
	// Determine the page number from its position.
	vuint32 num = inPos / kPageSize + 1;
	if( num <= mPageV->get_Count() )
	{
		Page* p = mPageV->get_ItemAt( num );
		FBL_CHECK(p);
		return p->mem;
	}

	return nullptr;
}


#pragma mark -


/**********************************************************************************************/
void File_RAM::DoClose( void )
{
	mOpened = false;
}


/**********************************************************************************************/
void File_RAM::DoCreate( void )
{
	_Allocate( 1 );	
	mOpened = true;
}


/**********************************************************************************************/
void File_RAM::DoOpen( void )
{
	mOpened = true;
}


/**********************************************************************************************/
void File_RAM::DoPutLength( flength aLength )
{
	flength len = get_Length();
	if( len == aLength )
		return;

	// determine the count of pages
	vuint32 page_count = vuint32( aLength / kPageSize );
	if( page_count * kPageSize < aLength )
		++page_count;

	if( page_count > mPageV->get_Count() )
		_Allocate(page_count - mPageV->get_Count());
	else
		_Deallocate(static_cast<vuint32>(mPageV->get_Count() - page_count));
}


/**********************************************************************************************/
flength	File_RAM::DoGetLengthOfClosed( void ) const 
{
	//return mPageV->get_Count() * kPageSize;
	return 0;
}


/**********************************************************************************************/
flength	File_RAM::DoGetLengthOfOpened( void ) const 
{
	// I think that File already know the length...?
	return mPageV->get_Count() * kPageSize;
}


/**********************************************************************************************/
void File_RAM::DoThrowOut( void )
{
	_Deallocate( vuint32(mPageV->get_Count()) );
}


/**********************************************************************************************/
vuint32 File_RAM::DoRead( 
	char* 		inBuffer,
	flength		inFrom, 
	vuint32 		inHowMuch )
{
	vuint32 total = 0;
	
	// Calculate page number of the file
	vuint32 page_N 	 = vuint32(inFrom / kPageSize);
	vuint32 offset	 = vuint32(inFrom - page_N * kPageSize);
	vuint32 bytes_left = kPageSize - offset;

	// Then make actual reading
	vuint32 to_read = inHowMuch < bytes_left ? inHowMuch : bytes_left;
	
	vuint32 pages = mPageV->get_Count();
	while( to_read )
	{
		// IS: 23.05.2007
		// http://valentina-db.com/bt/view.php?id=2013
		//

		if( mPageV->get_Count() == 0 || (1 + page_N > pages) )
			throw xOSFileError( (ERROR_TYPE) -39 ); // simulate eof
			
		memcpy( inBuffer, mPageV->get_ItemAt(1 + page_N)->mem + offset, to_read );
		page_N++;
		offset = 0;
		
		inBuffer	+= to_read;
		total 		+= to_read;
		inHowMuch 	-= to_read;		

		to_read = inHowMuch < kPageSize ? inHowMuch : kPageSize;
	}

	return total;
}


/**********************************************************************************************/
vuint32 File_RAM::DoWrite( 
	const char* inBuffer, 
	flength		inFrom, 
	vuint32 		inHowMuch )
{
	// Calculate segment number within the file
	vuint32 page_N 	 = vuint32(inFrom / kPageSize);
	vuint32 offset 	 = vuint32(inFrom - page_N * kPageSize);
	vuint32 bytes_left = kPageSize - offset;

	// Then make actual writing
	vuint32 to_write = (inHowMuch < bytes_left) ? inHowMuch : bytes_left;

	vuint32 lastPage = (vuint32)(inFrom + inHowMuch) / kPageSize;
	if( lastPage*kPageSize < (inFrom + inHowMuch) )
	{
		++lastPage;
	}

	vuint32 s = mPageV->get_Count();
	if( s < lastPage )
	{
		_Allocate( lastPage - s );
	}

	memcpy( mPageV->get_ItemAt(1 + page_N)->mem + offset, inBuffer, to_write);
	vuint32 total = to_write;

	while( total < inHowMuch )
	{
		bytes_left = inHowMuch - total;
		to_write = bytes_left > kPageSize ? kPageSize : bytes_left;

		++page_N;
		memcpy( mPageV->get_ItemAt(1 + page_N)->mem, inBuffer + total, to_write );
		total += to_write;
	}

	return (total);
}


/**********************************************************************************************/
void File_RAM::_Allocate( vuint32 inCount )
{
	vuint32 Pages_1 = mPageV->get_Count();

	for( vuint32 i = 0; i < inCount; ++i )
	{
		Page* p = new Page;
		memset( p->mem, 0, kPageSize );
	
		mPageV->AddItem(p);
	}

	vuint32 Pages_2 = mPageV->get_Count();
	sTotalSize += (Pages_2 - Pages_1) * kPageSize;
}


/**********************************************************************************************/
void File_RAM::_Deallocate( vuint32 inCount )
{
	FBL_CHECK( inCount <= mPageV->get_Count() );	

	vuint32 Pages_1 = mPageV->get_Count();

	for(vuint32 i = 0; i < inCount; ++i )
	{
		Page* p = mPageV->LastItem();
		delete(p);
		mPageV->RemoveItemAt( mPageV->get_Count(), true );
	}

	vuint32 Pages_2 = mPageV->get_Count();
	sTotalSize -= (Pages_1 - Pages_2) * kPageSize;
}


/**********************************************************************************************/
FBL_End_Namespace
