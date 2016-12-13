/**********************************************************************************************/
/* FBL_Value_String.h 	                                                   					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_Value_Text_h
	#define _FBL_Value_Text_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Values/FBL_Value_String.h>


#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_CLASS( Value_text_null );


/**********************************************************************************************/
/**
	While Value_string never change its allocated buffer size on assigment,
	and do truncation of value if it is longer of buffer, the Value_text provides other semantic.
    
	If we assign longer value, then it allocates buffer enough to keep that value.
	If we assign value shorter than the current buffer, then we will reallocate it only if value
	is more than 2 times less. Otherwise we will use the same buffer to get better speed.
	
	Also this class introduce Resize() which we can use to prepare buffer of required size,
	then read into it from disk TEXT value.
	
	We have only Value_text_null, because not nullable have no many sense.
*/
class FBL_SHARED_EXP_CLASS Value_text_null : public Value_string_null
{
	public://///////////////////////////////////////////////////////////////////////////////////

		typedef	Value_text_null							this_type;

		typedef       smart_ptr<this_type>				pointer;
		typedef const pointer							const_pointer;
		typedef       this_type&						reference;
		typedef const this_type&						const_reference;


	public://///////////////////////////////////////////////////////////////////////////////////

	// --- Construction ---
							/** This ctor creates absolutly empty value with zero buffer.
								should be used only by internal algorithms. */
							Value_text_null( void );

							/**	This ctor allows to create an empty Value_text_null with buffer  
								in 128 of characters. Size of buffer will be changed on each assigned.
								See comment on this class. 
								@param inLocalizable - if NULL then deault Localizable is used. */
							Value_text_null( 
								I_Localizable_Ptr inLocalizable );

							/**	This ctor allows init Value by some string.
								Actually this form will be used only in tests. 
								@param inStart	- begin of initialization string.
								@param inEnd	- end of initialization string. */
							Value_text_null( 
								const UChar* inStart, 
								const UChar* inEnd = nullptr );

							Value_text_null( 
								const char* inStart, 
								const char* inEnd = nullptr );

							/**	This is a copy-ctor. 
								@param inOther - other Value_string.*/
							Value_text_null( const_reference inOther );

virtual						~Value_text_null( void );

							
	public://///////////////////////////////////////////////////////////////////////////////////

// interface I_Value:

	// ---------------------
	// Properties:
	

virtual VALUE_TYPE			get_Type( void ) const override
								{ return kTypeText; }

virtual String				get_TypeString( const char inLocale[] = "en_US" ) const override 
								{ return TypeCode2String(kTypeText, inLocale); }



virtual	I_Value_Ptr			Clone( bool inCopyData = true ) const override  ;

	// ---------------------
	// Value access:

virtual	void				Assign( const I_Value& inValue ) override;

virtual void				put_String( 
								const UChar* inBegin, 
								const UChar* inEnd = nullptr ) override;

virtual	void 				put_String( 
								const char* inBegin, 
								const char* inEnd,
								I_Converter* inConverter ) override;

virtual void				put_Boolean( bool inValue ) override;
virtual void				put_Byte( vuint8 inValue ) override;
virtual void				put_Short( vint16 inValue ) override;
virtual void				put_UShort( vuint16 inValue ) override;
virtual void				put_Medium( vint32 inValue ) override;
virtual void				put_Long( vint32 inValue ) override;
virtual void				put_ULong( vuint32 inValue ) override;
virtual void				put_LLong( vint64 inValue ) override;
virtual void				put_ULLong( vuint64 inValue ) override;
virtual void				put_Float( float inValue ) override;
virtual void				put_Double( double inValue ) override;

// Interface I_Serializable:

virtual void				From( I_IStream_Ptr inStream, bool inBlock = true ) override;


	public://///////////////////////////////////////////////////////////////////////////////////

// Value_String API:

virtual void 				DoAssignFromUTF16( 
								const Value_string* inValue,
								I_Converter* 		inConverter ) override;

	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// This class API
								
							// Changes the size of value. Old value is lost.
		void				Resize( vuint32 inBytes );

							// Changes the size of value. Old value is lost.
		void				ResizeChars( tslen inChars );


	protected://////////////////////////////////////////////////////////////////////////////////

		void				DoAssign( 
								const UChar* inStart, 
								const UChar* inEnd = nullptr) ;

		void				DoAssign( 
								const char* inStart, 
								const char* inEnd) ;
				
	public://///////////////////////////////////////////////////////////////////////////////////

// Section of class with TEST - helper methods.
// for internal use.
 
#if FBL_TEST_CODE
							/**	This ctor allow create empty value_String with buffer of for 
								specified count of character. 
								@param inChars	- count of character for allocating buffer.*/
							Value_text_null(
								tslen inChars,
								I_Localizable_Ptr inLocalizable = nullptr )
							:
								Value_string_null( inChars, inLocalizable )
							{
							}

#endif // FBL_TEST_CODE
				
};


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_Value_Text_h
