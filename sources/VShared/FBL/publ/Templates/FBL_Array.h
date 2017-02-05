/**********************************************************************************************/
/* FBL_Array.h                                                     					  		  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_Array_h
	#define _FBL_Array_h

#include <VShared/FBL/publ/Headers/FBL.h>

#include <algorithm>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>

#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif


/**********************************************************************************************/
// The system_header pragma can be used to mark the current file as being a system header.
// No warnings will be produced from the location of the pragma onwards within the same file.
#pragma clang system_header


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
template <class T>
struct NativeItems
{
	void init_elemenets( T* s, vuint32 count ) 		{ memset(s, 0, count * sizeof(T) ); }
	void destroy_elem( T elem ) 					{ argused1(elem); }
};


/**********************************************************************************************/
template <class T>
struct ClassItems
{
	void init_elemenets( T* s, vuint32 count )		{ argused2(s, count); }
	void destroy_elem( T elem ) 					{ argused1(elem); }
};


/**********************************************************************************************/
template <class T>
struct PtrItems
{
	void init_elemenets( T* s, vuint32 count ) 		{ memset(s, 0, count * sizeof(T) ); }
	void destroy_elem( T elem ) 					{ delete elem; }
};


/**********************************************************************************************/
template <class T>
struct SmartPtrItems
{
	void init_elemenets( T* s, vuint32 count )		{ argused2(s, count); }
	void destroy_elem( T& elem ) 					{ elem = nullptr; }
};


/**********************************************************************************************/
/** Remove policy used when item removed from array. For example, we can before remove item 
	from array call specified method. Or after remove write specified value to end of the array.
	Also this need if we have array of smart pointers - we must set write zero into last item.
	Because after copy existing elements method addref called and references counter changed.
		Example:
			We have array with 2 smart pointers
				     1			2			mItems = 2
				E1(mRefs=1), E2(mRefs=1)

			Delete first element. Second element go into first position. Method AddRef called.
			And we have next array
					1			2			mItems = 1
				E2(mRefs=2), E2(mRefs=2)

			We must put zero into elemnt 2.
			After do this we have good array.
					1			2			mItems = 1
				E1(mRefs=1), NULL	
*/


/**********************************************************************************************/
template <class T>
struct RPNothing
{
	void before_remove_elem( T& elem ) 				{ argused1(elem); }
	void after_remove_elem ( T* s, vuint32 count ) 	{ { argused2(s, count); } }
};

/**********************************************************************************************/
template <class T>
struct RPSetZero
{
	void before_remove_elem( T& elem ) 				{ argused1(elem); }
	void after_remove_elem ( T* s, vuint32 count ) 	{ s[ count - 1 ] = nullptr; }
};


/**********************************************************************************************/
enum Ownership
{
	kNotOwnItems 	= 0,	// == false
	kOwnItems 		= 1		// == true
};


/**********************************************************************************************/
// This is universal array template class.
// It is universal because it can be container of 
// 1) simple types (int, vint32, ...) 					FBL::Array<vint16> a(10);
// 2) pointers on simple types (int*, vint32*, ... )	FBL::Array<vint16*> a(10);
// 3) instances of classes ( A, STD::string, ... )		FBL::Array<A> a(10);
// 4) pointers of classes ( A*, STD::string*, ... ) 	FBL::Array<A*> a(10);
// 5) pointers of smart classes ( A_Ptr )				FBL::Array<A_Ptr> a(10);
//
// NOTE: IN THIS file are defined few helper TYPEDEFs below after class Array.
// NOTE: range of this class is 1-based.
//
template
<
	class T, 
	class elem_traits = PtrItems<T>,
	class remove_policy = RPNothing<T>
> 
class Array : 
	protected elem_traits, 
	protected remove_policy, 
	public I_Unknown
{
		IMPLEMENT_SINGLE_UNKNOWN(Array)		

	public://///////////////////////////////////////////////////////////////////////////////////
 
 		typedef T*       iterator;
 		typedef const T* const_iterator;


	public://///////////////////////////////////////////////////////////////////////////////////
 
 		// Construction:
 
							Array(
								vuint32 	inInitialItemCount = 10,
								Ownership 	inOwnsItems = kNotOwnItems )
							:
								mData( nullptr ),
								mItems( 0 ),
								mMaxItems( inInitialItemCount ),
								mOwnsItems( inOwnsItems )
							{ 
								if( mMaxItems )
								{
									mData = new T[mMaxItems];
									elem_traits::init_elemenets( mData, mMaxItems );
								}
							}
										
							Array( const Array& inOther );


	protected://////////////////////////////////////////////////////////////////////////////////

virtual 					~Array( void ) 
							{ 
								if( mOwnsItems )
									DisposeItems();

								if( mData )
									delete [] mData;
							}


	public://///////////////////////////////////////////////////////////////////////////////////

		void				DisposeItems( void );

							// make copy of this array on the heap and clone its items. 
		Array*				Clone( void );


	public://///////////////////////////////////////////////////////////////////////////////////

		// Accessories:

		bool				get_IsEmpty( void ) const 		{ return (mItems == 0) ? true : false; }

		vuint32 			get_Count( void ) const 		{ return mItems;		}	
		void				put_Count( vuint32 inCount ) 	{ mItems = inCount;		} 
		
		vuint32 			get_MaxCount( void ) const 		{ return mMaxItems;		}
				
		Ownership			get_OwnsItem( void ) const		{ return mOwnsItems;	}
		void				put_OwnsItem( Ownership inOwns ){ mOwnsItems = inOwns;	}
		
		const_iterator		begin( void ) const 			{ return mData; }
		const_iterator		end( void ) const				{ return mData + mItems; }


	public://///////////////////////////////////////////////////////////////////////////////////

		// Methods for items manipulations:

		void				AddItem( T inItem ) 
							{ 
								if( mItems == mMaxItems )
									Resize( mMaxItems ? mMaxItems * 2 : 10 );

								mData[mItems++] = inItem;
							}
		
		void				put_ArrayItem( T inItem, vuint32 inPos ) 
							{ 
								FBL_CHECK( inPos && inPos <= mItems );
								mData[inPos-1] = inItem; 		
							}
										
		void 				InsertAfter( T inNewItem, T inAfterItem ) 
							{
								// if inAfterItem is not found then we append a new item
								// to the end of the array.
								vuint32 afterIndex = inAfterItem ? FindIndex(inAfterItem) : mItems; 
								InsertAfterAt( inNewItem, afterIndex );
							}

		void 				InsertAfterAt( T inNewItem, vuint32 inAfterIndex );

							
		void				RemoveItem( T inItem, bool inDontDestroy = false ) 
							{ 
								vuint32 index = FindIndex(inItem);
								if( index > 0 )
									RemoveItemAt( index, inDontDestroy );	
							}		

		void				RemoveItemAt( vuint32 inPos, bool inDontDestroy = false );	

		const T&			get_ItemAt( vuint32 inPos ) const 
							{ 
								//FBL_CHECK( inPos && inPos <= mItems ); 
								return mData[inPos - 1];	
							}

		const T& 			FirstItem( void ) const 
							{ 
								FBL_CHECK( mItems > 0 );
								return mData[0]; 
							}
				
		const T&			LastItem( void ) const 
							{ 
								FBL_CHECK( mItems > 0 );
								return mData[mItems - 1]; 
							}
		
		vuint32				FindIndex( const T inItem ) const; 
		
		bool				Include( const T inItem ) const  
								{ return bool(FindIndex(inItem) > 0); }		
	
		void				Resize( vuint32 inNewSize );

		void				SetItemsCount( vuint32 inItemsCount );
		
		void				Sort( void );


	private:////////////////////////////////////////////////////////////////////////////////////

		void				fromArray( const Array& inSrc );

		void				Swap( vuint32 inPos1, vuint32 inPos2 );

		int					Compare( const T& inIntem1, const T& inIntem2 );


	protected:////////////////////////////////////////////////////////////////////////////////// 

		T*					mData;

		vuint32				mItems;
		vuint32				mMaxItems;

		Ownership			mOwnsItems;
}; 


#pragma mark -
#pragma mark  === Specific classes


/**********************************************************************************************/
template<class T> 
class ArrayOfSimples : public Array<T, NativeItems<T>, RPNothing<T> >
{
	public://///////////////////////////////////////////////////////////////////////////////////
 
			ArrayOfSimples(
				vuint32 	inInitialItemCount = 10,
				Ownership 	inOwnsItems = kNotOwnItems )
			:
			 	Array<T, NativeItems<T>, RPNothing<T> >( inInitialItemCount, inOwnsItems )	
			{	
			}
};


/**********************************************************************************************/
template<class T> 
class ArrayOfClasses : public Array<T, ClassItems<T>, RPNothing<T> >
{
	public://///////////////////////////////////////////////////////////////////////////////////
								
			ArrayOfClasses(
				vuint32 	inInitialItemCount = 10,
				Ownership 	inOwnsItems = kNotOwnItems )
			:
			 	Array<T, ClassItems<T>, RPNothing<T> >( inInitialItemCount, inOwnsItems )	
			{	
			}	
};


/**********************************************************************************************/
template<class T> 
class ArrayOfPtrs : public Array<T, PtrItems<T>, RPNothing<T> >
{
	public://///////////////////////////////////////////////////////////////////////////////////
 
			ArrayOfPtrs(
				vuint32 	inInitialItemCount = 10,
				Ownership 	inOwnsItems = kNotOwnItems )
			:
			 	Array<T, PtrItems<T>, RPNothing<T> >( inInitialItemCount, inOwnsItems )	
			{	
			}	
};


/**********************************************************************************************/
template<class T> 
class ArrayOfSmartPtrs : public Array<T, SmartPtrItems<T>, RPSetZero<T> >
{
	public://///////////////////////////////////////////////////////////////////////////////////
 
			ArrayOfSmartPtrs(
				vuint32 	inInitialItemCount = 10,
				Ownership 	inOwnsItems = kOwnItems )
			:
			 	Array<T, SmartPtrItems<T>, RPSetZero<T> >( inInitialItemCount, inOwnsItems )	
			{	
			}
};


#pragma mark -
#pragma mark  === inline methods


/**********************************************************************************************/
template <class T, class elem_traits, class remove_policy >
Array<T, elem_traits, remove_policy>::Array( const Array<T, elem_traits, remove_policy>& inSrc )
{
	mOwnsItems 	= kNotOwnItems;
	mItems		= inSrc.mItems;
	mMaxItems	= inSrc.mMaxItems;

	mData = new T [mMaxItems];

	for( vuint32 i = 1; i <= mItems; ++i )
	{
		mData[i-1] = inSrc.get_ItemAt(i);
	}
}


/**********************************************************************************************/
template <class T, class elem_traits, class remove_policy >
void Array<T, elem_traits, remove_policy>::DisposeItems( void ) 
{ 
	if( mOwnsItems )
	{
		for( vuint32 i = 0; i < mItems ; ++i )
		{
			elem_traits::destroy_elem( mData[i] ); 
		}
	}

	delete [] mData;
	mData = nullptr;

	mItems = 0;
	mMaxItems = 0;
}


/**********************************************************************************************/
template <class T, class elem_traits, class remove_policy >
void Array<T, elem_traits, remove_policy>::InsertAfterAt( T inNewItem, vuint32 inAfterIndex ) 
{
	if( !(0 <= inAfterIndex && inAfterIndex <= mItems) )
		inAfterIndex = mItems;

	if( mItems == mMaxItems )
		Resize( mMaxItems ? mMaxItems * 2 : 10  );

	T* last = mData + mItems;
	std::copy_backward( mData + inAfterIndex, last, last + 1 );

	mData[inAfterIndex] = inNewItem;
	++mItems;
}


/**********************************************************************************************/
template <class T, class elem_traits, class remove_policy >
void Array<T, elem_traits, remove_policy>::RemoveItemAt( vuint32 pos, bool inDontDestroy ) 
{ 
	argused1( inDontDestroy );

	//FBL_CHECK( pos && pos <= mItems  ); 

	if( pos == 0 || pos > mItems )
		return;	// nothing to remove

	remove_policy::before_remove_elem( mData[pos-1] );

	if( mOwnsItems && inDontDestroy == false )
	{	
		elem_traits::destroy_elem( mData[pos-1] );
	}

	T* first = mData + pos;
	std::copy( first, mData + mItems, first - 1 );

	remove_policy::after_remove_elem( mData, mItems );

	--mItems;
}				


/**********************************************************************************************/
template <class T, class elem_traits, class remove_policy >
vuint32 Array<T, elem_traits, remove_policy>::FindIndex( const T inItem ) const 
{
	T* pItem	= mData;
	T* pEndItem	= mData + mItems;

	while( pItem < pEndItem )
	{
		if( *pItem == inItem )
			return (vuint32)(pItem - mData + 1);

		++pItem;
	}

	return 0L;	// bad index.
}


/**********************************************************************************************/
template <class T, class elem_traits, class remove_policy >
void Array<T, elem_traits, remove_policy>::Resize( vuint32 inNewSize ) 
{
	if( inNewSize == 0 && mData ) // total colapse.
	{
		if( mOwnsItems )
			DisposeItems();
	
		delete [] mData;
		mData = nullptr;
	
		mItems = 0;
		mMaxItems = 0;
	}
	else // can be as greater so smaller
	{
		T* pNewArray = new T [inNewSize];
		vuint32 ToCopy = mItems < inNewSize ? mItems : inNewSize; 

		if( mData )
		{
			STD::copy( begin(), begin() + ToCopy, pNewArray );

			delete [] mData;
			mData = nullptr;
		}

		// fill by zero rest items if any. 
		elem_traits::init_elemenets( pNewArray + ToCopy, inNewSize - ToCopy );

		mData = pNewArray;
		mItems = mItems < inNewSize ? mItems : inNewSize;
		mMaxItems = inNewSize;
	}
}


/**********************************************************************************************/
// This function allow easy change 
//
template <class T, class elem_traits, class remove_policy >
void Array<T, elem_traits, remove_policy>::SetItemsCount( vuint32 inItemsCount )
{
	if( inItemsCount > mMaxItems )	
	{
		Resize( inItemsCount );
	}
	else if( inItemsCount < mMaxItems )	 // we reduce the number of items.
	{
		if( mOwnsItems )
		{
			for( vuint16 i = (vuint16) inItemsCount; i < mItems ; ++i ) // use zero-based indexes.
			{
				elem_traits::destroy_elem( mData[i] ); 
			}
		}

	}
		
	mItems = inItemsCount;
}


/**********************************************************************************************/
template <class T, class elem_traits, class remove_policy >
void Array<T, elem_traits, remove_policy>::Swap( vuint32 inPos1, vuint32 inPos2 )
{
	T lItemTmp = get_ItemAt( inPos1 );
	put_ArrayItem( get_ItemAt(inPos2), inPos1 );
	put_ArrayItem( lItemTmp, inPos2 );
}


/**********************************************************************************************/
template <class T, class elem_traits, class remove_policy >
int Array<T, elem_traits, remove_policy>::Compare( const T& inIntem1, const T& inIntem2 )
{
	return (inIntem1 > inIntem2) ? 1 : (inIntem1 == inIntem2) ? 0 : -1;
}


/**********************************************************************************************/
template <class T, class elem_traits, class remove_policy >
void Array<T, elem_traits, remove_policy>::Sort( void )
{
	vuint32 count = get_Count();
	if( count < 2 )
		return;

	vuint32 jItemIndex;
	vuint32 iItemIndex;
	vuint32 lItemIndex = 1 + count/2;
	vuint32 rItemIndex = count;

	for(;;)
	{
		// lIem > firstItem
		if( lItemIndex > 1 )
		{
			--lItemIndex;
		}
		else
		{
			Swap( lItemIndex, rItemIndex );
			
			--rItemIndex; 
			if( rItemIndex == 1 )
				return;
		}

		jItemIndex = lItemIndex;

		while( jItemIndex + jItemIndex <= rItemIndex )
		{
			iItemIndex = jItemIndex;
			jItemIndex += jItemIndex;	// jItemIndex *= 2; 
			
			if( jItemIndex < rItemIndex )
			{
				// if ( *jItem < *(jItem+1) )
				if( Compare( get_ItemAt(jItemIndex), get_ItemAt(jItemIndex+1) ) == -1 )
					++jItemIndex; 
			}
			
			//if( *iItem < *jItem )
			if( Compare( get_ItemAt(iItemIndex), get_ItemAt(jItemIndex) ) == -1 )
			{	
				Swap( iItemIndex, jItemIndex );
			}
			else
			{
				break;
			}
		}
	}	
}


/**********************************************************************************************/
SMART_ARRAY_OF_SIMPLES( ArrayOfUllong, vuint64 );


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_Array_h
