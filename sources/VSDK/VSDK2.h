/**********************************************************************************************/
/* VSDK2.h				                                                      				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/
//
// This file plays a role of the SINGLE HEADER file that
// a C++ developer should use as
// 	#include <VSDK/VSDK2.h>
// to get including all public headers of Valentina C++ SDK.
// 
// It is a good idea to use this file always and avoid including headers of VSDK directly,
// because then you get some kind of incapsulation. Future Paradigma Software can 
// add/remove/rename public headers, but your code still will be okay in this regard.
//
#ifndef __VSDK_h 
	#define __VSDK_h


/**********************************************************************************************/
// VSHARED.DLL
//
#include <VShared/FBL/publ/Interfaces/FBL_Interfaces.h>

	// and non-interfaces helper public classes of VSHARED.DLL

#include <VShared/FBL/publ/Algorithms/FBL_Algs_Table.h>

#include <VShared/FBL/publ/Errors/FBL_ErrorMap.h>

#include <VShared/FBL/publ/Location/FBL_Location.h>

#include <VShared/FBL/publ/Properties/FBL_ExportProperties.h>
#include <VShared/FBL/publ/Properties/FBL_IndexStyleProperties.h>
#include <VShared/FBL/publ/Properties/FBL_FieldProperties.h>

#include <VShared/FBL/publ/Sets/FBL_ArraySet.h>
#include <VShared/FBL/publ/Sets/FBL_BitSet.h>
#include <VShared/FBL/publ/Sets/FBL_ArraySetIterator.h>
#include <VShared/FBL/publ/Sets/FBL_BitSetIterator.h>

#include <VShared/FBL/publ/Util_classes/FBL_DatabaseManager.h>

#include <VShared/FBL/publ/Util_Paths/FBL_Util_FileExt.h>

#include <VShared/FBL/publ/Util_Strings/FBL_EscapeString.h>

#include <VShared/FBL/publ/Utilities/FBL_Util_Path.h>
#include <VShared/FBL/publ/Utilities/FBL_Util_PathA.h>

#include <VShared/FBL/publ/Values/Factories/FBL_Value_Factories.h>

#include <VShared/FBL/publ/Util_classes/FBL_NotificationQueue.h>
#include <VShared/FBL/publ/Util_classes/FBL_Notification.h>

#include <VShared/FBL/publ/Util_Strings/FBL_ParseEnumType.h>


// -----------------------------
// VSHARED / VSQL:
//
#include <VShared/VSQL/Interfaces/VSQL_Interfaces.h>

#include <VShared/VSQL/Database/VSQL_QueryResult.h>
#include <VShared/Sqlite/VSQL_SqliteQueryResult.h>
#include <VShared/VSQL/Utilities/VSQL_Cursor_ImportExport.h>

#include <VShared/VSQL/Errors/VSQL_ErrorMap.h>


/**********************************************************************************************/
// VKERNEL.DLL
//
#include <VKernel/FBL/publ/Headers/VKernel.h>
#include <VKernel/FBL/publ/Algorithms/FBL_Algs_Database.h>

// Header(s) to support OO Class-style of coding for VSDK. 
// Read the next header for details. 
//
#include <VSDK/VSDK_Objects.h>


/**********************************************************************************************/
// VCLIENT.DLL
//
#include <VClient/publ/Headers/VClient.h>
#include <VClient/publ/Headers/VC_URL.h>


/**********************************************************************************************/
// VREPORT.DLL
//
#include <VReport/Headers/VRP.h>


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // __VSDK_h
