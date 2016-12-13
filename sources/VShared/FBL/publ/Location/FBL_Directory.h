/**********************************************************************************************/
/* FBL_Directory.h			                                                      			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_Directory_h 
	#define _FBL_Directory_h
#pragma once

#include <VShared/FBL/publ/Interfaces/FBL_I_Disk_Location.h>

#if FBL_POSIX_API
#	include <dirent.h>
#endif 		


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
class FBL_SHARED_EXP_CLASS Directory
{
	public://///////////////////////////////////////////////////////////////////////////////////
 	

	public://///////////////////////////////////////////////////////////////////////////////////
 	
 							Directory( I_Disk_Location_Ptr inLocation );
							~Directory( void );	


	public://///////////////////////////////////////////////////////////////////////////////////
 
		// API that allows iterate directories in cross-platform way.
		// This Location must be a directory.

		bool				StartIteration( void );
		void				StopIteration( void );
		
							// Returns pointer on next item of this directory.
		I_Disk_Location_Ptr	NextItem( void );


	private:////////////////////////////////////////////////////////////////////////////////////
 
		I_Disk_Location_Ptr		mpLocation;


	private:////////////////////////////////////////////////////////////////////////////////////

		// Data that represent directory:
 
#if FBL_WIN
		HANDLE				mDirent;
		bool				mStarted;
#endif // FBL_WIN


#if FBL_POSIX_API
		DIR*				mDirent;
#endif // FBL_POSIX_API		


#if FBL_MAC && !FBL_MAC_MACHO
		void*				mDirent;
#endif // FBL_MAC && !FBL_MAC_MACHO

};


/**********************************************************************************************/
FBL_End_Namespace

#endif // _FBL_Directory_h
