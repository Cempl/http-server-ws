/**********************************************************************************************/
/* FBL_Interfaces.h		                  		                               				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

// This file include all FBL/publ/Interfaces.
//
// This allow to developer inlude only this one file.
// This is good idea because in case of chnages in interfaces your sources
// should not be corrected.


#ifndef _FBL_Interfaces_h
	#define _FBL_Interfaces_h
#pragma once


/**********************************************************************************************/
// VHARED/FBL:
//
#include <VShared/FBL/publ/Interfaces/FBL_I_Aspect.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_AspectHost.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_Connection.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Connectable.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Compressor.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_Database.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Disk_Location.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Encryptable.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_Field.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_File.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_FldBlob.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_FldObjectPtr.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_FldObjectsPtr.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_FldPicture.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_FldString.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_FldApproximate.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_ForeignPlugin.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_IndexStyle.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_IStream.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Link.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Link2.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_LinkOnValues.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_Localizable.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Location.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_LogFile.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_NotificationCenter.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_OStream.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Property.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_PropertyContainer.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_RemoteProxy.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_Serializable.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Storage.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Stream.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_String.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_StringFactory.h>


#include <VShared/FBL/publ/Interfaces/FBL_I_Table.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Task.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_TaskManager.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_TaskStatistic.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Type.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Type_Collection.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Type_Composite.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Type_Enumerated.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_Unknown.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Value.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_ValueBinary.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_ValueCompound.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_ValueDate.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_ValueDateTime.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_ValueTime.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_ValueApproximate.h>
#include <VShared/FBL/publ/Interfaces/fbl_i_valueformater.h> // KP In the SVN this file in the LOWER CASE. It is important for linux.


/**********************************************************************************************/
#endif // _FBL_Interfaces_h
