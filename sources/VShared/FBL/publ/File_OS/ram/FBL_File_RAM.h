/**********************************************************************************************/
/* FBL_File_RAM.h																			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                       					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_File_RAM_h
	#define _FBL_File_RAM_h

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>

#include <VShared/FBL/publ/File_OS/FBL_File.h>

// STD:
#include <vector>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
class FBL_SHARED_EXP_CLASS File_RAM : public File
{
		IMPLEMENT_UNKNOWN(File_RAM)											
		BEGIN_INTERFACE_TABLE(File_RAM)										
			IMPLEMENT_INTERFACE(I_File)										
			IMPLEMENT_INTERFACE(I_AspectHost)
		END_INTERFACE_TABLE()
 
		friend class FileIterator_RAM;


	public://///////////////////////////////////////////////////////////////////////////////////

							File_RAM( void );

virtual						~File_RAM( void );


	public://///////////////////////////////////////////////////////////////////////////////////

// I_File

virtual bool				get_Exists( void ) const override
							{
								FBL_ENGINE_LOCK
								return ExistsOnDisk();
							}
    
		bool  		   		get_IsOpen( void ) const override;
		bool				get_IsReadOnly( void ) const override;
		void				put_IsReadOnly( bool inValue ) override;

		bool				ExistsOnDisk( void ) const;
		void 				Flush( void ) override;


	public://///////////////////////////////////////////////////////////////////////////////////

// THIS CLASS' API:
							
virtual vuint8*				GetPageDataFromPos( vuint32 inPos );

static	vuint32				get_TotalSize( void ) { return sTotalSize; }

	
	protected:////////////////////////////////////////////////////////////////////////////////// 

// File

		void				DoClose( void ) override;
		void				DoCreate( void ) override;
		void				DoOpen( void ) override;
		
		void				DoPutLength( flength aLength ) override;
		flength				DoGetLengthOfClosed( void ) const override;
		flength				DoGetLengthOfOpened( void ) const override;
		void          		DoThrowOut( void ) override;
		
		vuint32				DoRead( 
								char* 		inBuffer,
								flength		inFrom, 
								vuint32 	inHowMuch ) override;
								
		vuint32				DoWrite( 
								const char* inBuffer, 
								flength		inFrom, 
								vuint32 	inHowMuch ) override;


	private:////////////////////////////////////////////////////////////////////////////////// 

		void				_Allocate(vuint32 inCount);
		void				_Deallocate(vuint32 inCount);


	private:////////////////////////////////////////////////////////////////////////////////// 

		typedef vuint8 byte;

		/// struct represents a single page 
		struct Page
		{
			byte mem[kPageSize];	///< page memory
		};

		SMART_ARRAY_OF_SIMPLE_PTRS( ArrayOfRamPages, Page* );


	private:////////////////////////////////////////////////////////////////////////////////// 


//		typedef std::vector<Page*> TPageV;

		ArrayOfRamPages_Ptr	mPageV;				///< vector of pages
		bool				mOpened;			///< open flag


	private://////////////////////////////////////////////////////////////////////////////////

static 	vuint32				sTotalSize;		// Total Size of all existed File_Ram objects.

};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_File_RAM_h
