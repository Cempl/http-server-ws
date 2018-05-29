/**********************************************************************************************/
/* FBL_Stack.h                                                 					  		 	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_Stack_h
	#define _FBL_Stack_h

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
template<class T> 
class Stack : private ArrayOfPtrs<T>
{
		typedef ArrayOfPtrs<T> inherited;

#if FBL_USE_MEMPOOL
		using inherited::operator new;
#endif // FBL_USE_MEMPOOL

	public://///////////////////////////////////////////////////////////////////////////////////

		using inherited::AddRef;
		using inherited::Release;

	public://///////////////////////////////////////////////////////////////////////////////////

							Stack( vuint32 inInitialItemCount = 10 ) :
								inherited( inInitialItemCount, kNotOwnItems )
							{
							}

virtual						~Stack( void )
							{  
							}


	public://///////////////////////////////////////////////////////////////////////////////////

		// -----------------
		// Properties:

		bool				get_IsEmpty( void ) const 
								{ return inherited::get_Count() == 0; }

		vuint32				get_Count( void ) const 
								{ return inherited::get_Count(); }

		const T&			get_Top( void ) const 
								{ return inherited::get_ItemAt(get_Count()); }

		// -----------------
		// Item methods:

		void				Push( const T& inVal ) 
								{ inherited::AddItem(inVal); }

		void				Pop( void ) 
								{ inherited::RemoveItemAt(get_Count());}

		void				Resize( vuint32 inNewSize )
								{ inherited::Resize( inNewSize ); }

};


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_Stack_h
