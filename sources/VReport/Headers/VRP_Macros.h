/**********************************************************************************************/
/* VRP_Macros.h    		                                                      				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#pragma once


/**********************************************************************************************/
#ifdef _MSC_VER

	#if VREPORT_EXPORTS

		#define VRP_EXP			__declspec(dllexport)
		#define VRP_EXP_CLASS	__declspec(dllexport)

	#else // VREPORT_EXPORTS

		#define VRP_EXP			__declspec(dllimport)
		#define VRP_EXP_CLASS	__declspec(dllimport)

	#endif // VREPORT_EXPORT

#endif // _MSC_VER


/**********************************************************************************************/
#ifdef __GNUC__

	#if VREPORT_EXPORTS

	#if FBL_UNIX
		#define VRP_EXP				__attribute__((visibility("default")))

		#if __GNUC__ >= 4
			#define VRP_EXP_CLASS	__attribute__((visibility("default")))
		#else // __GNUC__ >= 4
			#define VRP_EXP_CLASS
		#endif //  __GNUC__ >= 4
	#else
		#define VRP_EXP				__attribute__((visibility("default")))

		#if __GNUC__ >= 4
			#define VRP_EXP_CLASS	__attribute__((visibility("default")))
		#else // __GNUC__ >= 4
			#define VRP_EXP_CLASS
		#endif //  __GNUC__ >= 4
	#endif // FBL_UNIX

	#else // VREPORT_EXPORTS
		#if FBL_UNIX
			#define VRP_EXP	__attribute__((visibility("default")))
			#define VRP_EXP_CLASS __attribute__((visibility("default")))
		#else
			#define VRP_EXP
			#define VRP_EXP_CLASS
		#endif // FBL_UNIX

	#endif // VREPORT_EXPORT

#endif // __GNUC__


/**********************************************************************************************/
#define CPAGE mpReportMain->mpPageCurrent
