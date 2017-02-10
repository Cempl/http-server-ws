/**********************************************************************************************/
/* VRP.h				                                                      				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#pragma once

// VSHARED
#include <VShared/FBL/publ/Location/FBL_Location.h>
#include <VShared/FBL/publ/Util_classes/FBL_ForeignPlugin_Imp.h>

// VSHARED INTERFACES
#include <VShared/FBL/publ/Interfaces/FBL_I_Disk_Location.h>
#include <VShared/VSQL/Interfaces/VSQL_I_Cursor.h>
#include <VShared/VSQL/Interfaces/VSQL_I_SqlDatabase.h>
#include <VShared/VSQL/Interfaces/VSQL_I_VProject.h>
#include <VShared/VSQL/Interfaces/VSQL_I_VReport.h>

// VREPORT
#include <VReport/Headers/VRP_Macros.h>

/**********************************************************************************************/
FBL_Using_Namespace

/**********************************************************************************************/
VSQL_Begin_Namespace


/**********************************************************************************************/
// Init method for VREPORT.dll
//
VRP_EXP void				VReport_Init(
								const char*	inMacSerial,
								const char*	inWinSerial,
								const char*	inLinuxSerial );

VRP_EXP void 				VReport_Shutdown( void );


/**********************************************************************************************/
// Factory of I_VProject.
// Creates a new Valentina Project to be used by a Valentina ADK.
// Valentina Studio will use another factory to create complete Project instance.
//
// inLocation - this is path to a .vsp file.
//
VRP_EXP	I_VProject_Ptr		CreateVProject( I_Disk_Location_Ptr inLocation );
VRP_EXP	I_VProject_Ptr		CreateVProject( void );


/**********************************************************************************************/
VSQL_End_Namespace
