/**********************************************************************************************/
/* FBL_DynamicLibrary.h	                                                  					  */
/*                                                                       					  */
/* Copyright Paradigma, 1993-2s003															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_DynamicLibrary_h
	#define _FBL_DynamicLibrary_h
#pragma once

#include <VShared/FBL/publ/Util_Strings/FBL_ustdio.h>

#include <VShared/FBL/publ/Utilities/FBL_Util_Path.h>

#if FBL_MAC_MACHO || FBL_UNIX
#define DLOPEN_NO_WARN
#include <dlfcn.h>	
#endif // FBL_MAC_MACHO 

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// The set of policies provides the functionality, for different platform, allowing to load
// unload a dynamic libraries and to receive a pointer on a function from dll.


/**********************************************************************************************/
// Policy for WIN platform.
//

#if FBL_WIN

class WindowsPolicy
{

protected://///////////////////////////////////////////////////////////////////////////////////

						// library type 
		typedef			HMODULE DllType;

						// load dynamic library
inline	DllType			LoadLibrary( const char* inName )
						{
							// only ASCII names allowed so...
							return ::LoadLibraryA(inName);
						}

						// load dynamic library
inline	DllType			LoadLibrary( const UChar* inName )
						{
							// only ASCII names allowed so...
							return ::LoadLibraryW(inName);
						}

						// unload library
inline	bool			UnloadLibrary( DllType inLibrary )
						{
							return !!::FreeLibrary(inLibrary);
						}

						// getting function from library
inline	void*			GetFunction( DllType inLibrary, const char* inName )
						{
							return ::GetProcAddress( inLibrary, inName );
						}
};

#endif // FBL_WIN


/**********************************************************************************************/
// Policy for Mac CARBON platform
//

#if FBL_MAC && !FBL_MAC_MACHO

class MacPolicy
{

protected://///////////////////////////////////////////////////////////////////////////////////

		// library type 
		typedef			CFragConnectionID DllType;

						// load dynamic library
inline	DllType			LoadLibrary( const char* inName )
						{
							DllType handle;
							FSSpec myFSSpec ;
							Ptr myMainAddr ;
							Str255 myErrName ;

							/*#ifdef TARGET_API_MAC_OSX
								FSRef theRef;
								// get the FSRef associated with the POSIX path
								(void) FSPathMakeRef((const UInt8 *) inName, &theRef, NULL);
								// convert the FSRef to an FSSpec
								(void) FSGetCatalogInfo(&theRef, kFSCatInfoNone, NULL, NULL, &myFSSpec, NULL);
							#else*/
								Path2Spec_Carbon( inName, &myFSSpec );
							//#endif

							OSErr err = GetDiskFragment( &myFSSpec, 0, kCFragGoesToEOF, "\p", kPrivateCFragCopy, &handle, &myMainAddr, myErrName );
							if( err != noErr )
								handle = NULL ;
								
							return handle;
						}

						// load dynamic library
inline	DllType			LoadLibrary( const UChar* inName )
						{
							DllType handle;
							FSSpec myFSSpec ;
							Ptr myMainAddr ;
							Str255 myErrName ;

							char buff[512 + 1];
							vu_u2a( inName, buff, 512 );

							/*#ifdef TARGET_API_MAC_OSX
								FSRef theRef;
								// get the FSRef associated with the POSIX path
								(void) FSPathMakeRef((const UInt8 *) inName, &theRef, NULL);
								// convert the FSRef to an FSSpec
								(void) FSGetCatalogInfo(&theRef, kFSCatInfoNone, NULL, NULL, &myFSSpec, NULL);
							#else*/
								Path2Spec_Carbon( buff, &myFSSpec );
							//#endif

							OSErr err = GetDiskFragment( &myFSSpec, 0, kCFragGoesToEOF, "\p", kPrivateCFragCopy, &handle, &myMainAddr, myErrName );
							if( err != noErr )
								handle = NULL ;
								
							return handle;
						}
						
#if FBL_MAC
inline	DllType			LoadLibrary( const FSSpec* inSpec )
						{
							DllType handle;
							Ptr myMainAddr ;
							Str255 myErrName ;

							OSErr err = GetDiskFragment( 
								inSpec, 0, kCFragGoesToEOF, "\p", kPrivateCFragCopy, &handle, &myMainAddr, myErrName );
								
							if( err != noErr )
								handle = NULL ;
								
							return handle;
						}
#endif // FBL_MAC

						// unload library
inline	bool			UnloadLibrary( DllType inLibrary )
						{
							return CloseConnection(&inLibrary) != noErr;
						}

						// getting function from library
inline	void*			GetFunction( DllType inLibrary, const char* inName )
						{
							void *symbol = NULL;    // return value
							Ptr symAddress ;
							CFragSymbolClass symClass ;
							Str255 symName ;							

							FBL::c2pstrcpy( inName, (StringPtr) symName );	

							if ( FindSymbol( inLibrary , symName , &symAddress , &symClass ) == noErr )
								symbol = (void *)symAddress ;

							return symbol;
						}
};

#endif // FBL_MAC && !FBL_MAC_MACHO


/**********************************************************************************************/
// Policy for UNIX and MacOS X MACHO-BSD platform. 
//

#if FBL_MAC_MACHO || FBL_UNIX

class UnixPolicy
{

protected://///////////////////////////////////////////////////////////////////////////////////

						// library type 
		typedef			void* DllType;

						// load dynamic library
inline	DllType			LoadLibrary( const UChar* inName )
						{
							tslen len = vu_strlen( inName );

							MemPtr<char> buff( (vuint32)(len + 1) );
							vu_u2a( inName, buff, len );

							return dlopen( buff.begin(), 0 );
						}

						// load dynamic library
inline	DllType			LoadLibrary( const char* inName )
						{
							return dlopen( inName, 0 );
						}

						// unload library
inline	bool			UnloadLibrary( DllType inLibrary )
						{
							return dlclose(inLibrary) == 0;
						}

						// getting function from library
inline	void*			GetFunction( DllType inLibrary, const char* inName )
						{
							return (void*)dlsym( inLibrary, inName );	// ???
						}
};

#endif // FBL_MAC_MACHO


/**********************************************************************************************/
// The template class which implements platform-independent work with dynamic libraries 
// by means of the necessary policy
//
template <class OsPolicy>
class DynamicLibrary_T : 
	public OsPolicy, 
	public I_Unknown
{
		IMPLEMENT_SINGLE_UNKNOWN( DynamicLibrary_T )

	protected://////////////////////////////////////////////////////////////////////////////////

		typename OsPolicy::DllType mLibrary; // library handle


	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// Construction:

							DynamicLibrary_T( bool inUnloadInDestructor = true ) 
								: mUnloadInDestructor(inUnloadInDestructor)
							{
								mLibrary = nullptr;
							}


virtual						~DynamicLibrary_T()
							{
								if( mUnloadInDestructor )
									Unload();
							}
							
							
							// load library bu path and filename
		bool				Load( const char* inName )
							{
								try
								{
									if ( mLibrary && !Unload() )
										return false;

									mLibrary = OsPolicy::LoadLibrary( inName );
								}
								catch(...)
								{
								}

								return mLibrary != NULL;
							}

							// load library bu path and filename
		bool				Load( const UChar* inName )
							{
								try
								{
									if ( mLibrary && !Unload() )
										return false;

									mLibrary = OsPolicy::LoadLibrary( inName );
								}
								catch(...)
								{
								}

								return mLibrary != nullptr;
							}

#if FBL_SUPPORT_FSSPEC
							// load library by path and filename
		bool				Load( const FSSpec* inSpec )
							{
								try
								{
									if ( mLibrary && !Unload() )
										return false;

									mLibrary = OsPolicy::LoadLibrary( inSpec );
								}
								catch(...)
								{
								}

								return mLibrary != NULL;
							}
#endif // FBL_SUPPORT_FSSPEC

							// unload library 
							// check references counter t olibrary
		bool				Unload( void )
							{
								bool retVal = true;
								try
								{
									if ( mLibrary ) 
										retVal = OsPolicy::UnloadLibrary( mLibrary );
								
								}
								catch(...)
								{
									retVal = false;
								}

								mLibrary = nullptr;
								return retVal;
							}

							// getting function from library
		void*				GetFunc( const UChar* inName )
							{
								void* retVal;
								try
								{
									retVal = mLibrary 
										? OsPolicy::GetFunction( mLibrary, inName )
										: NULL;
								}
								catch(...)
								{
									retVal = nullptr;
								}

								return retVal;
							}

		void*				GetFunc( const char* inName )
							{
								void* retVal;
								try
								{
									retVal = mLibrary 
										? OsPolicy::GetFunction( mLibrary, inName )
										: nullptr;
								}
								catch(...)
								{
									retVal = nullptr;
								}

								return retVal;
							}

	private:////////////////////////////////////////////////////////////////////////////////////


		bool				mUnloadInDestructor;
};


/**********************************************************************************************/
// Declare platform independent class to work with dynamic libraries.
//
#if FBL_WIN
	typedef DynamicLibrary_T<WindowsPolicy> DynamicLibrary;
#endif // FBL_WIN	
	
#if FBL_MAC && !FBL_MAC_MACHO
	typedef DynamicLibrary_T<MacPolicy> DynamicLibrary;
#endif // FBL_MAC && !FBL_MAC_MACHO

#if FBL_UNIX || FBL_MAC_MACHO
	typedef DynamicLibrary_T<UnixPolicy> DynamicLibrary;
#endif

FBL_SMART_PTR( DynamicLibrary );


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_DynamicLibrary_h
