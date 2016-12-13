/**********************************************************************************************/
/* FBL_MemPtr.h																				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                       					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_MemPtr_h 
	#define _FBL_MemPtr_h
#pragma once

// ANSI-C
#include <string.h> // memcpy

// VSHARED
#include <VShared/FBL/publ/Headers/FBL.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
template<class T> class MemPtr
{
	public://///////////////////////////////////////////////////////////////////////////////////

							MemPtr( vuint32 inSize = 0 ) : p( nullptr )
							{ 
								Allocate( inSize );
							}

							MemPtr( const T* inData, vuint32 inSize )  : p( nullptr )
							{ 
								Allocate( inSize );
								if( mSize )
								{
									memcpy(p, inData, sizeof(T) * mSize);
								}
							}

							~MemPtr( void )
							{ 
								if(p) delete [] p; 
							}


							operator T* (void) 				{ return p; }
							operator const T* (void) const 	{ return p; }
					
		const T*			begin( void ) const 			{ return p; }
		const T*			end( void ) const 				{ return p + mSize; }
				
		T*					begin( void ) 					{ return p; }
		T*					end( void ) 	 				{ return p + mSize; }
				
		T&					operator[]( vuint32 inIndex ) 	{ return p[inIndex]; }		

		void				FillWith(int ch)
							{
								memset(p, ch, (vuint32)((sizeof(T) * mSize)));
							}

		vuint32				get_Size(void) const 	{ return mSize; }
		void				put_Size( vuint32 inSize ) 
							{ 
								Allocate( inSize );
							}

		T*					Dismiss( void ) 
							{
								T* pT = p;
								mSize = 0;
								p = nullptr;
								return pT;
							}
		
		void				Grow( vuint32 inSize )
							{
								if( inSize > mSize )
								{
									T* pNewBuffer = new T[ inSize ];
									memset( pNewBuffer, 0, vuint32(sizeof(T) * inSize) );

									if( mSize )
									{
										FBL_CHECK( p );
										memcpy(pNewBuffer, p, sizeof(T) * mSize);
										delete [] p;
									}

									p = pNewBuffer;
									mSize = inSize;
								}
								else
								{
									// http://www.valentina-db.com/bt/view.php?id=6960
									// Asquiring size is less (or equal) than existed.
									// Just do nothing. Probably caller must check needness of
									// buffer growing but anyway "old" FBL_CHECK(inSize > mSize)
									// is not enough.
								}
							}

	private:///////////////////////////////////////////////////////////////////////////////////

		void				Allocate( vuint32 inSize )
							{
								if( p != nullptr )
                                {    
									delete [] p;
                                    p = nullptr;
                                }
                                    
								mSize = inSize;
                                if( mSize )
                                {    
                                    p = new T[mSize];
                                    memset( p, 0, vuint32(sizeof(T) * mSize) ); 
                                }
							}


	private:///////////////////////////////////////////////////////////////////////////////////

		T* 					p;
		vuint32	 			mSize;
};


/**********************************************************************************************/
typedef MemPtr<char> CharPtr;


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_MemPtr_h
