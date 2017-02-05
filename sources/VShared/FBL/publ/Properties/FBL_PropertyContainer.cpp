/**********************************************************************************************/
/* FBL_PropertyContainer_Impl.cpp	                                           				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <algorithm>

#include <VShared/FBL/publ/Properties/FBL_PropertyContainer.h>
#include <VShared/FBL/publ/Util_Strings/FBL_ustdio.h>
#include <VShared/FBL/publ/Util_classes/FBL_String.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
PropertyContainer::PropertyContainer( void )
: 
	mItems( new ArrayOfProperties(kOwnItems ))
{
}


/**********************************************************************************************/
PropertyContainer::~PropertyContainer( void )
{
}


#pragma mark -


/**********************************************************************************************/
vuint32 PropertyContainer::get_Count( void ) const 
{ 
	return mItems->get_Count(); 
}


/**********************************************************************************************/
I_Property_Ptr PropertyContainer::get_Property( vuint32 inIndex ) const 
{
	return inIndex > 0 && inIndex <= mItems->get_Count() ? mItems->get_ItemAt(inIndex) : nullptr;
}


/**********************************************************************************************/
I_Property_Ptr PropertyContainer::get_Property( const UChar* inName ) const 
{
	String s(inName);
	return get_Property(s);
}


/**********************************************************************************************/
I_Property_Ptr PropertyContainer::get_Property( const String& inName ) const 
{
	vuint32 index = mItems->get_Count() + 1;
	while(--index)
	{
		const String& name = mItems->get_ItemAt(index)->get_Name();
		if( name.caseCompare(inName, 0) == 0 ) {
			break;
		}
	}
	
	return index ? mItems->get_ItemAt(index) : nullptr;
}


/**********************************************************************************************/
void PropertyContainer::Add( I_Property_Ptr inProperty )
{
	if (inProperty)
	{
		vuint32 index = mItems->get_Count() + 1;
		const String& name = inProperty->get_Name();
	
		// check if we already have property with such name
		while( --index )
		{
			const String& str = mItems->get_ItemAt(index)->get_Name();
		
			if( str.caseCompare(name, 0) == 0 )
			{
				// found property with the same name
				break;
			}
		}
				
		if ( index )
		{
			mItems->put_ArrayItem( inProperty, index );
		}
		else
		{
			mItems->AddItem( inProperty );
		}
	}
}


/**********************************************************************************************/
void PropertyContainer::Remove( const I_Property_Ptr inProperty ) 
{
	if( inProperty )
	{
		vuint32 index = mItems->get_Count() + 1;
		while( --index )
		{
			if ( mItems->get_ItemAt(index) == inProperty )
			{
				mItems->get_ItemAt(index);
				mItems->RemoveItemAt( index );
				break;
			}
		}
	}
}


/**********************************************************************************************/
void PropertyContainer::Remove( const UChar* inName ) 
{
	if( inName )
	{
		const UChar* name;
		vuint32 index = mItems->get_Count() + 1;
		while( --index )
		{
			name = mItems->get_ItemAt(index)->get_Name().c_str();

			if( vu_stricmp( name, inName) == 0 )
			{
				mItems->RemoveItemAt( index );
				break;
			}
		}
	}
}


/**********************************************************************************************/
void PropertyContainer::Remove( const String& inName ) 
{
	return Remove(inName.c_str());
}


/**********************************************************************************************/
void PropertyContainer::Clear( void ) 
{
	vuint32 index = mItems->get_Count() + 1;
	while( --index )
		mItems->get_ItemAt(index);

	mItems->Resize(0);
}


/**********************************************************************************************/
FBL_End_Namespace
