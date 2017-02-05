/**********************************************************************************************/
/* FBL_Instantenation.cpp                  		                               				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/
/*
	This file resolve odd problem with linker.
	
	PROBLEM is that some interfaces declared as FBL_SHARED_EXP.
	But any .cpp file from VShared project do not include them, 
	so inline destructor of these interface-classes	is not instantiated as VSHARED.dll part.
	This cause problem of linking for vkernel_win.dll, for example.
	
	To resolve this problem, we add this special dummy .cpp file,
	and simply include here problematic interfaces. SOLVED.
*/

#include <VShared/FBL/publ/Headers/StdAfx.h>


#include <VShared/FBL/publ/Interfaces/FBL_Interfaces.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
I_Aspect::~I_Aspect( void )									{}
I_AspectHost::~I_AspectHost( void )							{}

I_Connectable::~I_Connectable( void )						{}
I_Connection::~I_Connection( void )							{}

I_Database::~I_Database( void )								{}
I_Disk_Location::~I_Disk_Location( void )					{}
I_Encryptable::~I_Encryptable( void )						{}

I_Field::~I_Field( void )									{}
I_File::~I_File( void )										{}

I_FldBlob::~I_FldBlob( void )								{}
I_FldObjectPtr::~I_FldObjectPtr( void )						{}
I_FldObjectsPtr::~I_FldObjectsPtr( void )					{}
I_FldPicture::~I_FldPicture( void )							{}
I_FldString::~I_FldString( void )							{}
I_FldApproximate::~I_FldApproximate( void )					{}

I_ForeignPlugin::~I_ForeignPlugin( void )					{}

I_IndexStyle::~I_IndexStyle( void )							{}
I_IStream::~I_IStream( void )								{}

I_Link::~I_Link( void )										{}
I_Link2::~I_Link2( void )									{}
I_LinkOnValues::~I_LinkOnValues( void )						{}
I_Location::~I_Location( void )								{}

I_LogFile::~I_LogFile( void )								{}
I_OStream::~I_OStream( void )								{}

I_NotificationCenter::~I_NotificationCenter(void)			{}

I_Property::~I_Property( void )								{}
I_PropertyContainer::~I_PropertyContainer( void )			{}

I_Collator::~I_Collator( void )								{}
I_Converter::~I_Converter( void )							{}
I_Locale::~I_Locale( void )									{}
I_Localizable::~I_Localizable( void )						{}

I_RemoteProxy::~I_RemoteProxy( void )						{}
I_Serializable::~I_Serializable( void )						{}
I_Storage::~I_Storage( void )								{}
I_Stream::~I_Stream( void )									{}

I_String::~I_String( void )									{}
I_StringFactory::~I_StringFactory( void )					{}

I_Table::~I_Table( void )									{}
I_Task::~I_Task( void )										{}
I_TaskManager::~I_TaskManager( void )						{}
I_TaskStatistic::~I_TaskStatistic( void )					{}
I_Type::~I_Type( void )										{}
I_Type_Composite::~I_Type_Composite( void )					{}
I_Type_Enumerated::~I_Type_Enumerated( void )				{}
I_Type_Collection::~I_Type_Collection( void )				{}
I_Task_MT::~I_Task_MT( void )								{}

I_Unknown::~I_Unknown( void )								{}

I_Value::~I_Value( void )									{}
I_ValueBinary::~I_ValueBinary( void )						{}
I_ValueCompound::~I_ValueCompound( void )					{}
I_ValueDate::~I_ValueDate( void )							{}
I_ValueDateTime::~I_ValueDateTime( void )					{}
I_ValueTime::~I_ValueTime( void )							{}
I_ValueApproximate::~I_ValueApproximate( void )				{}
I_ValueFormater::~I_ValueFormater( void )					{}


   
/**********************************************************************************************/
FBL_End_Namespace


