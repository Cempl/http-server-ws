/**********************************************************************************************/
/* FBL_I_Value.h	                                                      					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_Value_h
	#define _FBL_I_Value_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_IStream.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_OStream.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Serializable.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_Type.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Type_Enumerated.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// forward declarations:
//
SMART_INTERFACE(I_Localizable);
//
class String;
class DTFormat;


/**********************************************************************************************/
// this file declarations:
//
SMART_INTERFACE(I_Value);
//
SMART_ARRAY_OF_SMART_PTRS( ArrayOfValues, I_Value_Ptr );
SMART_ARRAY_OF_SMART_PTRS( ArrayOfValueArrays, ArrayOfValues_Ptr );
SMART_ARRAY_OF_SMART_PTRS( ArrayOfConstValues, I_Value_Ptr );


/**********************************************************************************************/
// Declare the prototype of the function-factory of value.
// Param1 and Param2 depend on the type of value.
//
typedef I_Value*(*MAKE_VALUE_FUNC_PTR)( 
			bool inNullable, void* inParam1, void* inParam2 );


/**********************************************************************************************/
// Any value belongs to some of this category. 
// Each value must be able to return its category.
// Values of the same category can be compared without transformation to other format.
//
// Each category have some maximal value_type, which can store any other value of that category,
// for example: for vcInteger this is vint64.
//
enum VALUE_CATEGORY
{
	vcUndefined = 0,
	vcInteger	= 1,		// max value:  vint64
	vcUInteger,				// max value:  vuint64
	vcReal,					// max value:  double
	vcString,				// max value:  string
	vcDate, 
	vcTime,
	vcDateTime,
	vcBinary,				// max value - blob
	vcEnum,					// added in v5.0
    vcCompound,				// added in v5.0
    vcMoney,				// added in v6.0
    vcVariant				// added in v7.0
};


/**********************************************************************************************/
enum COMPARE_TYPE
{
	kNatureCompare,
	kBinaryCompare
};


/**********************************************************************************************/
// Interface class -- defines I_Value API.
// This class can be considered as a VARIANT class that can keep value of any kind.
// An Instance of this class know its type and size.
// 
interface FBL_SHARED_EXP_CLASS I_Value : public I_Unknown
{
	 	
virtual 					~I_Value( void );

							/** Virtual constructor. 
								If inCopyData is TRUE then we have virtual copy-ctor. */
virtual	I_Value_Ptr			Clone( bool inCopyData = true ) const  = 0;


	// ---------------------
	// Properties:

		// <Allocated> 
							/** Returns the maximal size of value in bytes.  
								For String values include zero end byte(s).
								It is >= get_Length(). */
virtual vuint32 			get_Allocated( void ) const  = 0;	

							/** Modify the maximal size of buffer that keep value. 
								Returns FALSE if value cannot change size, e.g. numeric values. 
								It does not copy old value, only allocates a new buffer. */
virtual bool				put_Allocated( vuint32 inSize )  = 0;

		// <ByteLength> [r/w]
							/** Returns the length in bytes of the _CURRENT_ value. */
virtual vuint32 			get_ByteLength( void ) const  = 0;

							/** Sets a new length in bytes of the _CURRENT_ value (if possible). */
virtual void 				put_ByteLength( vuint32 inNewLength ) = 0;

		// <ByteLengthForIndex> [r/o]
							// Returns the length in bytes of the _CURRENT_value
                            //	in the format of Index Storage.
                            // Maximal value can be 255 bytes, because Index for strings and binary values
                            // uses pascal format with 1-byte length prefix.
virtual	vuint32				get_ByteLengthForIndex( void ) const  = 0;	


		// <Category> [r/o]
							/** Returns the type of the value. */
virtual VALUE_CATEGORY		get_Category( void ) const  = 0;	

		// <IsNull>
							/** Returns TRUE if the value is nullable. Else returns FALSE. */							
virtual bool				get_IsNullable( void ) const  = 0;	

							/** Returns TRUE if the value is NULL. Else returns FALSE. */							
virtual bool 				get_IsNull( void ) const  = 0;	

							/** Clears value (free RAM) and set it to NULL.
								After this command get_IsNull() always returns TRUE. */ 
virtual void 				put_IsNull( bool inValue )  = 0;	

		// <IsRemote> [r/o]
							/// Returns TRUE if this value belongs to the remote Database.
virtual bool 				get_IsRemote( void ) const  = 0;	

							/// internal. Do not call it directly. 
							///  Used By FBL::CreateValue() factory.
virtual void 				put_IsRemote( bool inValue )  = 0;	

		// <Length> [r/o]
							/** Returns the length (in chars) of value's string representation. 
								Works for numeric types also. */
virtual vuint32 			get_Length( void ) const  = 0;

		// <MaxTextLength> [r/o]
 							/** Returns the maximal possible length of string representation 
 								for this value type. Works for numeric types also. */	
virtual vuint32				get_MaxChars( void ) const  = 0;

		// <StorageSize> [r/o]

							//! Returns the size in bytes which this value uses when it is stored 
							//! in a storage (e.g. disk file).
							//! It have sense only for FIXED SIZE values.
							//! For most values it is equal to get_Allocated().
							//! Value_string of VString field will not count end zero byte(s). 
virtual vuint32				get_StorageSize( void ) const  = 0; 

		// <Type> [r/o]
							/** Returns the type of the value. */
virtual VALUE_TYPE			get_Type( void ) const  = 0;	

		// <TypeString> [r/o]
							/** Returns the type string of the value. */
virtual String				get_TypeString( const char inLocale[] = "en_US" ) const  = 0;

		// <CompareType> [r/w]
							/** Relevant for string_value (used in KeyValue's keys). */
virtual COMPARE_TYPE		get_CompareType( void ) const = 0;
virtual void				put_CompareType( COMPARE_TYPE inValue ) = 0;


	public://///////////////////////////////////////////////////////////////////////////////////
	
	// ---------------------
	// Value Access Methods:

							/** Returns pointer on the first byte of value. */
virtual const char*			begin( void ) const  = 0;

							/** Returns pointer on the byte, next after last value byte. 
								For String values pointer after end-zero byte. */
virtual const char*			end( void ) const  = 0;

							/** Assign value to inValue. Convert type if needed.
								If convertion is not possible then value becomes NULL. */
virtual	void				Assign( const I_Value& inOther )  = 0;

							/** Copies value into outBuffer. Ignores IsNULL flag. 
								This method is used to store a value into an I_Index. */
virtual vuint32				CopyToIndex( void* outBuffer, bool inSwapBytes ) const  = 0;
virtual void 				CopyFromIndex( const void* inBuffer, bool inSwapBytes )  = 0;


	// ---------------------		
	// Value Compare Methods:

							/** Compare this value with inOther value of the same type.
								inCompareType defines the way, which will be used for compare.
								Returns 0 if values are equal.
								Returns >0 if this value > inOther.
								Returns <0 if this value < inOther. */
virtual	int					Compare( 
								const I_Value& 	inOther, 
								COMPARE_TYPE 	inCompareType = kNatureCompare ) const  = 0;
							
							/** This version assumes that inIndexValue points on a value of the same type.
								This function IGNORES NULL flag. It compares only value itself.
								It is used for comparing of values stored in the index files.
								Result is the same as in the above Compare() function.
								
								Important is that THIS value plays role only of a style-holder.
								For example if index is on a string field then THIS value will have
								all locale settings as field. This is important because inTestValue,
								most probably, will not have these settings.
								
								@param inTestValue this is value, which we search.
								@param inIndexValue direct void pointer to the place where value
									is stored on index page. This buffer is casted by this value
									oject to be the same type.
								@param inParam any additional info can be passed here or ignored. 
							*/
virtual	int					CompareToIndexValue( 
								Const_I_Value_Ptr 	inTestValue,
								const void* 		inIndexValue,
								vuint32				inParam, 
								bool				inSwapBytes ) const  = 0;

							/** Compares 2 values stored in the index. 
								@param inLeft - pointer to left value, stored in the index.
								@param inRight - pointer to right value, stored in the index. */
virtual	int					CompareIndexValues( 
								const void* inLeft,
								const void* inRight, 
								bool		inSwapBytes ) const  = 0;


	public://///////////////////////////////////////////////////////////////////////////////////
	
	// ---------------------
	// Get Value as String:

							/** Returns a string representation of value (Unicode).
								@param inLimit - limit of string size in characters. 
								Returns a String object (not a ref) because for some types (e.g.Numeric)
								we must create new String object as storage for string. */
virtual String				get_String( tslen inLimit = -1 ) const = 0;


							/** Get a string representation of value (ASCII).
								@param outString - Buffer for string representation of value.
								@param inBufferChars - the size of buffer in characters. */
virtual char*				get_String( 
								char* 	outString, 
								tslen 	inBufferChars ) const = 0;
								
							/** Get a string representation of value (Unicode).
								@param outString - Buffer for string representation of value.
								@param inBufferChars - the size of buffer in characters. */
virtual UChar*				get_String( 
								UChar* 	outString, 
								tslen 	inBufferChars ) const = 0;

								
	// ---------------------
	// Put String into Value:

							/** Sets the value from string */
virtual void				put_String( const String& inStr ) = 0;

							/** Sets the value from string (Unicode).
								@param inBegin	- begin of buffer.
								@param inEnd	- end of buffer.
								if inEnd is NULL, then calculates u_strlen() of string. */
virtual void				put_String( 
								const UChar* inBegin, 
								const UChar* inEnd = nullptr ) = 0;


							/** Sets the value from string (ASCII).
								@param inBegin	- begin of buffer.
								@param inEnd	- end of buffer.
								if inEnd is NULL, then calculates strlen() of string. */
virtual void				put_String( 
								const char* inBegin, 
								const char* inEnd = nullptr ) = 0;


	// ---------------------
	// Numeric Properties:

			// Boolean [r/w]
virtual void				put_Boolean( bool inValue )							= 0;
virtual bool				get_Boolean( void ) const							= 0;

			// Byte [r/w]
virtual void				put_Byte( vuint8 inValue )							= 0;
virtual vuint8				get_Byte( void ) const								= 0;

			// Short [r/w]
virtual void				put_Short( vint16 inValue )							= 0;
virtual vint16				get_Short( void ) const								= 0;

			// UShort [r/w]
virtual void				put_UShort( vuint16 inValue )						= 0;
virtual vuint16				get_UShort( void ) const							= 0;

			// Medium [r/w]
virtual void				put_Medium( vint32 inValue )						= 0;
virtual vint32				get_Medium( void ) const							= 0;

			// Long [r/w]
virtual void				put_Long( vint32 inValue )							= 0;
virtual vint32				get_Long( void ) const								= 0;

			// ULong [r/w]
virtual void				put_ULong( vuint32 inValue )						= 0;
virtual vuint32				get_ULong( void ) const								= 0;

			// LLong [r/w]
virtual void				put_LLong( vint64 inValue )							= 0;
virtual vint64				get_LLong( void ) const								= 0;

			// ULLong [r/w]
virtual void				put_ULLong( vuint64 inValue )						= 0;
virtual vuint64				get_ULLong( void ) const							= 0;

			// Float [r/w]
virtual void				put_Float( float inValue )							= 0;
virtual float				get_Float( void ) const								= 0;

			// Double [r/w]
virtual void				put_Double( double inValue )						= 0;
virtual double				get_Double( void ) const							= 0;


	// ---------------------
	// Byte swapping:

							/// Swaps bytes of value in-place.
virtual void				SwapBytes( void )									= 0;

							/// Swaps bytes of value in-place at address 
							/// pointed by inValue.
virtual void				SwapBytes( void* inValue ) const					= 0;

							/// Swaps bytes of value at address pointed by @inSrc,
							/// swapped value will be then copied to the @inDest address.
virtual void				SwapBytes( const void* inSrc, void* inDest )		= 0;


	// ---------------------
	// Serialization to/from char buffer (for ValueVariant):

virtual vuint32 			get_BinaryRepresentationByteLength( void ) const = 0;

virtual void				FromBinaryRepresentation( const char* inpBuffer ) = 0;

virtual void				ToBinaryRepresentation( char* outpBuffer ) const = 0;


	public://///////////////////////////////////////////////////////////////////////////////////
	
//#if FBL_TEST_CODE

	// ---------------------
	// TEST helpers:
	// We need this to build automated tests.

							// Set instance of I_Value to some initial value.
							// for example for numbers this can be 1.
							// for strings this can be "aaa".
virtual void				Init( void )  = 0;

							// Change value of instance to the next ordered value.							
virtual void 				Increment( void )  = 0; 

//#endif // FBL_TEST_CODE

}; 


/**********************************************************************************************/
// Returns TRUE if inValue type is one of unsigned integer types 
// { vuint8, vuint16, vuint32, vuint64 }. Time is ULONG also. Date is Long
//
inline bool IsUnsignedType( I_Value_Ptr inValue )
{
	switch( inValue->get_Type() )
	{
		case kTypeByte:
		case kTypeUShort:
		case kTypeUMedium:
		case kTypeULong:
		case kTypeULLong:
		case kTypeTime:		// time is vuint32 in fact.
		{
			return true;
		}
		
		default:
		{
			return false;
		}
	}
}


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>


/**********************************************************************************************/
#endif // _FBL_I_Value_h
