/**********************************************************************************************/
/* FBL_Starter.cpp	                                                 						  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Starter/FBL_Starter.h>
#include <VShared/FBL/publ/Util_classes/FBL_String.h>



/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// Define kernel DLL name.
//
const char* GetKernelDllName( void )
{
	const char* kernelDLLName = nullptr;

// ---------------------------------------------------------------------
#if FBL_WIN

	#if FBL_64

		#if __CHECK
			#if FBL_EXTERNAL_THREADSAFE
				kernelDLLName = "vkernel_mt_debug_x64.dll";
			#else
				kernelDLLName = "vkernel_debug_x64.dll";
			#endif // FBL_EXTERNAL_THREADSAFE
		#else
			#if FBL_EXTERNAL_THREADSAFE
				kernelDLLName = "vkernel_mt_release_x64.dll";
			#else
				kernelDLLName = "vkernel_release_x64.dll";
			#endif // FBL_EXTERNAL_THREADSAFE
		#endif //_DEBUG
	
	#else // FBL_64

		#if __CHECK
			#if FBL_EXTERNAL_THREADSAFE
				kernelDLLName = "vkernel_mt_debug_x86.dll";
			#else
				kernelDLLName = "vkernel_debug_x86.dll";
			#endif // FBL_EXTERNAL_THREADSAFE
		#else
			#if FBL_EXTERNAL_THREADSAFE
				kernelDLLName = "vkernel_mt_release_x86.dll";
			#else
				kernelDLLName = "vkernel_release_x86.dll";
			#endif // FBL_EXTERNAL_THREADSAFE
		#endif //_DEBUG

	#endif // FBL_64

#endif // FBL_WIN


// ---------------------------------------------------------------------
#if FBL_MAC_MACHO

	#if FBL_64

		#if __CHECK 
			#if FBL_EXTERNAL_THREADSAFE
				kernelDLLName = "libvkernel_mt_ntv_debug_x64.dylib";
			#else
				kernelDLLName = "libvkernel_ntv_debug_x64.dylib";
			#endif // FBL_EXTERNAL_THREADSAFE
		#else
			#if FBL_EXTERNAL_THREADSAFE
				kernelDLLName = "libvkernel_mt_fat_release_x64.dylib";
			#else
				kernelDLLName = "libvkernel_fat_release_x64.dylib";
			#endif // FBL_EXTERNAL_THREADSAFE
		#endif // __CHECK

	#else // FBL_64

		#if __CHECK 
			#if FBL_EXTERNAL_THREADSAFE
				kernelDLLName = "libvkernel_mt_ntv_debug.dylib";
			#else
				kernelDLLName = "libvkernel_ntv_debug.dylib";
			#endif // FBL_EXTERNAL_THREADSAFE
		#else
			#if FBL_EXTERNAL_THREADSAFE
				kernelDLLName = "libvkernel_mt_fat_release.dylib";
			#else
				kernelDLLName = "libvkernel_fat_release.dylib";
			#endif // FBL_EXTERNAL_THREADSAFE
		#endif // __CHECK

	#endif // FBL_64
	
#endif // FBL_MAC_MACHO


// ---------------------------------------------------------------------
#if FBL_UNIX

	#if FBL_64

		#if __CHECK
			#if FBL_EXTERNAL_THREADSAFE
				kernelDLLName = "libvkernel_mt_debug_x64.so";
			#else
				kernelDLLName = "libvkernel_debug_x64.so";
			#endif // FBL_EXTERNAL_THREADSAFE
		#else
			#if FBL_EXTERNAL_THREADSAFE
				kernelDLLName = "libvkernel_mt_release_x64.so";
			#else
				kernelDLLName = "libvkernel_release_x64.so";
			#endif // FBL_EXTERNAL_THREADSAFE			
		#endif // __CHECK

	#else // FBL_64

                #ifndef __arm__
	
                    #if __CHECK
                            #if FBL_EXTERNAL_THREADSAFE
                                    kernelDLLName = "libvkernel_mt_debug_x86.so";
                            #else
                                    kernelDLLName = "libvkernel_debug_x86.so";
                            #endif // FBL_EXTERNAL_THREADSAFE
                    #else
                            #if FBL_EXTERNAL_THREADSAFE
                                    kernelDLLName = "libvkernel_mt_release_x86.so";
                            #else
                                    kernelDLLName = "libvkernel_release_x86.so";
                            #endif // FBL_EXTERNAL_THREADSAFE
                    #endif // __CHECK

                #else // __arm__
                                    
                    #if __CHECK
                            #if FBL_EXTERNAL_THREADSAFE
                                    kernelDLLName = "libvkernel_mt_debug_armhf_32.so";
                            #else
                                    kernelDLLName = "libvkernel_debug_armhf_32.so";
                            #endif // FBL_EXTERNAL_THREADSAFE
                    #else
                            #if FBL_EXTERNAL_THREADSAFE
                                    kernelDLLName = "libvkernel_mt_release_armhf_32.so";
                            #else
                                    kernelDLLName = "libvkernel_release_armhf_32.so";
                            #endif // FBL_EXTERNAL_THREADSAFE
                    #endif // __CHECK
                                    
                #endif // __arm__

	#endif // FBL_64

#endif // FBL_UNIX

	return kernelDLLName;
}


#pragma mark -


/**********************************************************************************************/
// Define client DLL name.
//
const char* GetClientDllName( void )
{
	const char* clientDLLName = nullptr;

// ---------------------------------------------------------------------
#if FBL_WIN

	#if FBL_64

		#if __CHECK
			#if FBL_EXTERNAL_THREADSAFE
				clientDLLName = "vclient_mt_debug_x64.dll";
			#else
				clientDLLName = "vclient_debug_x64.dll";
			#endif // FBL_EXTERNAL_THREADSAFE
		#else
			#if FBL_EXTERNAL_THREADSAFE
				clientDLLName = "vclient_mt_release_x64.dll";
			#else
				clientDLLName = "vclient_release_x64.dll";
			#endif // FBL_EXTERNAL_THREADSAFE
		#endif //_DEBUG
		
	#else // FBL_64

		#if __CHECK
			#if FBL_EXTERNAL_THREADSAFE
				clientDLLName = "vclient_mt_debug_x86.dll";
			#else
				clientDLLName = "vclient_debug_x86.dll";
			#endif // FBL_EXTERNAL_THREADSAFE
		#else
			#if FBL_EXTERNAL_THREADSAFE
				clientDLLName = "vclient_mt_release_x86.dll";
			#else
				clientDLLName = "vclient_release_x86.dll";
			#endif // FBL_EXTERNAL_THREADSAFE
		#endif //_DEBUG
		
	#endif // FBL_64

#endif //FBL_WIN


// ---------------------------------------------------------------------
#if FBL_MAC_MACHO 

	#if FBL_64

		#if __CHECK 
			#if FBL_EXTERNAL_THREADSAFE
				clientDLLName = "libvclient_mt_ntv_debug_x64.dylib";
			#else
				clientDLLName = "libvclient_ntv_debug_x64.dylib";
			#endif // FBL_EXTERNAL_THREADSAFE
		#else
			#if FBL_EXTERNAL_THREADSAFE
				clientDLLName = "libvclient_fat_mt_release_x64.dylib";
			#else
				clientDLLName = "libvclient_fat_release_x64.dylib";
			#endif // FBL_EXTERNAL_THREADSAFE
		#endif // __CHECK
		
	#else // FBL_64

		#if __CHECK 
			#if FBL_EXTERNAL_THREADSAFE
				clientDLLName = "libvclient_mt_ntv_debug.dylib";
			#else
				clientDLLName = "libvclient_ntv_debug.dylib";
			#endif // FBL_EXTERNAL_THREADSAFE
		#else
			#if FBL_EXTERNAL_THREADSAFE
				clientDLLName = "libvclient_fat_mt_release.dylib";
			#else
				clientDLLName = "libvclient_fat_release.dylib";
			#endif // FBL_EXTERNAL_THREADSAFE
		#endif // __CHECK
		
	#endif // FBL_64


#endif // FBL_MAC_MACHO

// ---------------------------------------------------------------------
#if FBL_UNIX

	#if FBL_64

		#if __CHECK
			#if FBL_EXTERNAL_THREADSAFE
				clientDLLName = "libvclient_mt_debug_x64.so";
			#else
				clientDLLName = "libvclient_debug_x64.so";
			#endif // FBL_EXTERNAL_THREADSAFE
		#else
			#if FBL_EXTERNAL_THREADSAFE
				clientDLLName = "libvclient_mt_release_x64.so";
			#else
				clientDLLName = "libvclient_release_x64.so";
			#endif // FBL_EXTERNAL_THREADSAFE
		#endif // __CHECK

	#else // FBL_64

                #ifndef __arm__

                    #if __CHECK
			#if FBL_EXTERNAL_THREADSAFE
				clientDLLName = "libvclient_mt_debug_x86.so";
			#else
				clientDLLName = "libvclient_debug_x86.so";
			#endif // FBL_EXTERNAL_THREADSAFE
                    #else
			#if FBL_EXTERNAL_THREADSAFE
				clientDLLName = "libvclient_mt_release_x86.so";
			#else
				clientDLLName = "libvclient_release_x86.so";
			#endif // FBL_EXTERNAL_THREADSAFE
                    #endif // __CHECK

                #else // __arm__

                    #if __CHECK
			#if FBL_EXTERNAL_THREADSAFE
				clientDLLName = "libvclient_mt_debug_armhf_32.so";
			#else
				clientDLLName = "libvclient_debug_armhf_32.so";
			#endif // FBL_EXTERNAL_THREADSAFE
                    #else
			#if FBL_EXTERNAL_THREADSAFE
				clientDLLName = "libvclient_mt_release_armhf_32.so";
			#else
				clientDLLName = "libvclient_release_armhf_32.so";
			#endif // FBL_EXTERNAL_THREADSAFE
                    #endif // __CHECK

                #endif // __arm__
		
	#endif // FBL_64
	
#endif // FBL_UNIX


	return clientDLLName;
}


/**********************************************************************************************/
// Define VReport DLL name.
//
const char* GetVReportDllName( void )
{
	const char* vReportDLLName = nullptr;

// ---------------------------------------------------------------------
#if FBL_WIN

	#if FBL_64

		#if __CHECK
			#if FBL_EXTERNAL_THREADSAFE
				vReportDLLName = "vreport_mt_debug_x64.dll";
			#else
				vReportDLLName = "vreport_debug_x64.dll";
			#endif // FBL_EXTERNAL_THREADSAFE
		#else
			#if FBL_EXTERNAL_THREADSAFE
				vReportDLLName = "vreport_mt_release_x64.dll";
			#else
				vReportDLLName = "vreport_release_x64.dll";
			#endif // FBL_EXTERNAL_THREADSAFE
		#endif //_DEBUG	

	#else // FBL_64

		#if __CHECK
			#if FBL_EXTERNAL_THREADSAFE
				vReportDLLName = "vreport_mt_debug_x86.dll";
			#else
				vReportDLLName = "vreport_debug_x86.dll";
			#endif // FBL_EXTERNAL_THREADSAFE
		#else
			#if FBL_EXTERNAL_THREADSAFE
				vReportDLLName = "vreport_mt_release_x86.dll";
			#else
				vReportDLLName = "vreport_release_x86.dll";
			#endif // FBL_EXTERNAL_THREADSAFE
		#endif //_DEBUG	

	#endif // FBL_64

#endif // FBL_WIN


// ---------------------------------------------------------------------
#if FBL_MAC_MACHO

	#if FBL_64

		#if __CHECK 
			#if FBL_EXTERNAL_THREADSAFE
				vReportDLLName = "libvreport_mt_ntv_debug_x64.dylib";
			#else
				vReportDLLName = "libvreport_ntv_debug_x64.dylib";
			#endif // FBL_EXTERNAL_THREADSAFE
		#else
			#if FBL_EXTERNAL_THREADSAFE
				vReportDLLName = "libvreport_mt_fat_release_x64.dylib";
			#else
				vReportDLLName = "libvreport_fat_release_x64.dylib";
			#endif // FBL_EXTERNAL_THREADSAFE
		#endif // __CHECK
		
	#else // FBL_64

		#if __CHECK 
			#if FBL_EXTERNAL_THREADSAFE
				vReportDLLName = "libvreport_mt_ntv_debug.dylib";
			#else
				vReportDLLName = "libvreport_ntv_debug.dylib";
			#endif // FBL_EXTERNAL_THREADSAFE
		#else
			#if FBL_EXTERNAL_THREADSAFE
				vReportDLLName = "libvreport_mt_fat_release.dylib";
			#else
				vReportDLLName = "libvreport_fat_release.dylib";
			#endif // FBL_EXTERNAL_THREADSAFE
		#endif // __CHECK	

	#endif // FBL_64	
	
#endif // FBL_MAC_MACHO


// ---------------------------------------------------------------------
#if FBL_UNIX

	#if FBL_64

		#if __CHECK
			#if FBL_EXTERNAL_THREADSAFE
				vReportDLLName = "libvreport_mt_debug_x64.so";
			#else
				vReportDLLName = "libvreport_debug_x64.so";
			#endif // FBL_EXTERNAL_THREADSAFE
		#else
			#if FBL_EXTERNAL_THREADSAFE
				vReportDLLName = "libvreport_mt_release_x64.so";
			#else
				vReportDLLName = "libvreport_release_x64.so";
			#endif // FBL_EXTERNAL_THREADSAFE
		#endif // __CHECK

	#else // FBL_64

            #ifndef __arm__

                #if __CHECK
			#if FBL_EXTERNAL_THREADSAFE
				vReportDLLName = "libvreport_mt_debug_x86.so";
			#else
				vReportDLLName = "libvreport_debug_x86.so";
			#endif // FBL_EXTERNAL_THREADSAFE
		#else
			#if FBL_EXTERNAL_THREADSAFE
				vReportDLLName = "libvreport_mt_release_x86.so";
			#else
				vReportDLLName = "libvreport_release_x86.so";
			#endif // FBL_EXTERNAL_THREADSAFE
		#endif // __CHECK
                                
            #else // __arm__

                #if __CHECK
			#if FBL_EXTERNAL_THREADSAFE
				vReportDLLName = "libvreport_mt_debug_armhf_32.so";
			#else
				vReportDLLName = "libvreport_debug_armhf_32.so";
			#endif // FBL_EXTERNAL_THREADSAFE
		#else
			#if FBL_EXTERNAL_THREADSAFE
				vReportDLLName = "libvreport_mt_release_armhf_32.so";
			#else
				vReportDLLName = "libvreport_release_armhf_32.so";
			#endif // FBL_EXTERNAL_THREADSAFE
		#endif // __CHECK
                                
            #endif // __arm__
		
	#endif // FBL_64
	
#endif // FBL_UNIX

	return vReportDLLName;
}


/**********************************************************************************************/
FBL_End_Namespace
