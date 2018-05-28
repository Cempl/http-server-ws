/**********************************************************************************************/
/* FBL_Value_String.h 	                                                   					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_Value_String_h 
	#define _FBL_Value_String_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Interfaces/FBL_I_Localizable.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Serializable.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Value.h>
#include <VShared/FBL/publ/Util_classes/FBL_StringConversion.h>
#include <VShared/FBL/publ/Values/FBL_type_traits.h>
#include <VShared/FBL/publ/Values/FBL_Type2String.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
/** if we do Value_String( NULL ) we get ambiguity between 1 and 2 form of ctors.
	To fix this we need Value_String( (char*) NULL ).
	To make things looks better we provide here pointer on NULL string that can be used as
	Value_String( pNullStr ).
*/
extern const UChar* const pNullStr;


/**********************************************************************************************/
/**	This is pointer to "" string.
	Why? Just for completness. :-). We have for all types (if its possible) default value.
	For example for String class the static member String.Empty - represent empty string value.
*/
extern const UChar* const pEmptyStr;


/**********************************************************************************************/
template<class T> 
void Convert_Str_To_Type( const UChar* inValue, T* outValue )
{
	type_traits<T>::from_str( inValue, outValue );
}


/**********************************************************************************************/
template<class T> 
char* Convert_Type_To_Str( T* inValue, UChar* outValue, vuint32 inBufferChars )  
{
	return (char*)type_traits<T>::to_str( inValue, outValue, inBufferChars );
}


/**********************************************************************************************/
#pragma mark class Value_string
//
class FBL_SHARED_EXP_CLASS Value_string : 
	public I_Value, 
	public I_Serializable
{
		IMPLEMENT_UNKNOWN(Value_string)											
		BEGIN_INTERFACE_TABLE(Value_string)										
			IMPLEMENT_INTERFACE(I_Value)										
			IMPLEMENT_INTERFACE(I_Serializable)										
		END_INTERFACE_TABLE()

	public://///////////////////////////////////////////////////////////////////////////////////

		typedef	Value_string									this_type;

		typedef       smart_ptr<this_type>						pointer;
		typedef const pointer									const_pointer;
		typedef       this_type&								reference;
		typedef const this_type&								const_reference;


	public://///////////////////////////////////////////////////////////////////////////////////

	// Construction:

							Value_string( void );

							/**	This ctor allows to create empty Value_string with buffer for 
								the specified number of characters. 
								@param inChars	- count of character for allocating buffer.*/
							Value_string( 
								tslen 				inChars,
								I_Localizable_Ptr 	inLocalizable = nullptr,
								COMPARE_TYPE		inCompareType = kNatureCompare );

							/**	This is a copy-ctor. 
								@param inOther - other Value_string.*/
							Value_string( 
								const_reference 	inOther );

virtual						~Value_string( void );
							

	public://///////////////////////////////////////////////////////////////////////////////////

// interface I_Value:

virtual	I_Value_Ptr			Clone( bool inCopyData = true ) const override;

							
	// ---------------------
	// Properties:
	
virtual vuint32 			get_Allocated( void ) const override;
virtual bool 				put_Allocated( vuint32 inByteSize ) override;
								
virtual vuint32 			get_ByteLength( void ) const override;
virtual void 				put_ByteLength( vuint32 inNewLength ) override;

virtual	vuint32				get_ByteLengthForIndex( void ) const override;	
							
virtual VALUE_CATEGORY		get_Category( void ) const override;

virtual bool				get_IsNullable( void ) const override	{ return false; }
virtual bool 				get_IsNull( void ) const override;
virtual void 				put_IsNull( bool inValue ) override;

virtual bool				get_IsRemote( void ) const override		{ return mIsRemote; }
virtual void 				put_IsRemote( bool inValue ) override	{ mIsRemote = inValue; }

virtual vuint32 			get_Length( void ) const override;
virtual vuint32				get_MaxChars( void ) const override;

virtual vuint32				get_StorageSize( void ) const override; 

virtual VALUE_TYPE			get_Type( void ) const override;

virtual String				get_TypeString( const char inLocale[] = "en_US" ) const override 
								{ return TypeCode2String(kTypeString, inLocale); }

virtual COMPARE_TYPE		get_CompareType( void ) const override;
virtual void				put_CompareType( COMPARE_TYPE inValue ) override;

	// ---------------------
	// Value access:

virtual const char*			begin( void ) const override;
virtual const char*			end	 ( void ) const override;

virtual	void				Assign( 
								const I_Value&		inValue ) override;

virtual vuint32				CopyToIndex( 
								void*				outBuffer, 
								bool				inSwapBytes ) const override;
								
virtual void 				CopyFromIndex( 
								const void*			inBuffer, 
								bool				inSwapBytes ) override;


	// ---------------------
	// Value compare methods:

virtual	int					Compare( 
								const I_Value&		inOther,
								COMPARE_TYPE		inCompareType = kNatureCompare ) const override;

virtual	int					CompareIndexValues( 
								const void*			inLeft,
								const void*			inRight,
								bool				inSwapBytes ) const override;

virtual	int					CompareToIndexValue( 
								Const_I_Value_Ptr 	inTestValue,
								const void* 		inIndexValue,
								vuint32				inParam,
								bool				inSwapBytes ) const override;


	// ---------------------
	// to/from string:

virtual	String				get_String( tslen inLimit = -1 ) const override;
virtual void				put_String( const String& inStr ) override;

virtual UChar*				get_String( 
								UChar*			outString, 
								tslen			inBufferChars ) const override;

virtual void				put_String( 
								const UChar*	inBegin, 
								const UChar*	inEnd = nullptr ) override;

	// ---------------------

virtual char*				get_String( 
								char*			outString, 
								tslen			inBufferChars ) const override;

virtual void				put_String( 
								const char*		inBegin, 
								const char*		inEnd = nullptr ) override;


	// ---------------------
	// Numeric properties:

virtual bool				get_Boolean( void ) const override;
virtual vuint8				get_Byte( void ) const override;
virtual vint16				get_Short( void ) const override;
virtual vuint16				get_UShort( void ) const override;
virtual vint32				get_Medium( void ) const override;
virtual vint32				get_Long( void ) const override;
virtual vuint32				get_ULong( void ) const override;
virtual vint64				get_LLong( void ) const override;
virtual vuint64				get_ULLong( void ) const override;
virtual float				get_Float( void ) const override;
virtual double				get_Double( void ) const override;

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


	// ---------------------
	// Byte swapping:

virtual void				SwapBytes( void ) override 							 { }
virtual void				SwapBytes( void* inValue ) const override 			 { argused1(inValue); }							
virtual void				SwapBytes( const void* inSrc, void* inDest ) override { argused2(inSrc, inDest); }


	// ---------------------
	// Serialization to/from char buffer (for ValueVariant):

virtual vuint32 			get_BinaryRepresentationByteLength( void ) const override;
virtual void				FromBinaryRepresentation( const char* inpBuffer ) override;
virtual void				ToBinaryRepresentation( char* outpBuffer ) const override;


	public://///////////////////////////////////////////////////////////////////////////////////
 
// interface I_Serializable:

virtual void				From( I_IStream_Ptr inStream, bool inBlock = true ) override;
virtual void				From( I_PacketRcv* 	inPacket, bool inBlock = true ) override;

virtual void				To( I_OStream_Ptr inStream, bool inBlock = true ) const override;
virtual void				To( I_PacketSnd*  inPacket, bool inBlock = true ) const override;

	
	public://///////////////////////////////////////////////////////////////////////////////////
 
// THIS CLASS API:

		// <Localizable> [r/o]

		I_Localizable*		get_Localizable( void ) const  
								{ return mpLocalizable; } 		
								
		void				put_Localizable( I_Localizable* inLocalzable )  
								{ mpLocalizable = inLocalzable; } 		

		// <IsSingleByte>
		bool				get_IsSingleByte( void ) const  { return mIsSingleByte; } 
		void				put_IsSingleByte( bool inValue )  { mIsSingleByte = inValue; } 


							/** Find the END ZERO byte in Value. 
								We need this after READ from a disk file for a FIXED SIZE type. */
		void				LocateEnd( void );
		

	public://///////////////////////////////////////////////////////////////////////////////////

// ---------------------
// THIS CLASS additional forms when we pass converter:
	
virtual void 				DoAssignFromUTF16( 
								const Value_string* inValue,
								I_Converter* 		inConverter );
 
virtual char*				get_String( 
								char* 				outString, 
								tslen 				inBufferChars,
								I_Converter* 		inConverter ) const;

virtual void				put_String( 
								const char* 		inBegin, 
								const char* 		inEnd,
								I_Converter* 		inConverter );


	protected:////////////////////////////////////////////////////////////////////////////////// 

// THIS CLASS API:

							// Allocate buffer that can keep inBytes including end-zero.
		void 				Allocate( vuint32 inBytes );
	
							/**	Assigns the specified string into this value. 
								Truncates the given string if needed .
								@param inStart	- begin of string buffer.
								@param inEnd	- end of string buffer.
								@return a pointer on the start of our string value.
								In the case inStart == NULL, we consider this as NULL 
								value assigment. */
		const UChar*		Assign( 
								const UChar* inStart, 
								const UChar* inEnd = nullptr );

		void				DoAssign( 
								const UChar* inStart, 
								const UChar* inEnd = nullptr);

		void				DoAssign( 
								const I_Value& inValue );

		void				SetupConversionFlag( void ) const;


		int					BinaryCompareToIndexValue_(
								const UChar*	inpBegin1,
								vuint8			inLen1InBytes,
								const UChar*	inpBegin2,
								vuint8			inLen2InBytes,
								vuint32			inParam ) const;

		int					NaturalCompareToIndexValue_(
								const UChar*	inpBegin1,
								tslen			inLen1InChars,
								const UChar*	inpBegin2,
								tslen			inLen2InChars,
								vuint32			inParam ) const;

							// Receiving value from server.
		void				From_OnClientSide( I_PacketRcv* inPacket );

							// Receiving value from client.
		void				From_OnServerSide( I_PacketRcv* inPacket );
							// Helper for From_OnServer_Side.
		void				From_OnServerSide_U( I_PacketRcv* inPacket, vuint32 inByteLength );


							// Sending value to the server.
		void				To_OnClientSide( I_PacketSnd* inPacket ) const;

							// Sending value to the client.
		void				To_OnServerSide( I_PacketSnd* inPacket ) const;
							// Helper for To_OnServerSide.
		void				To_OnServerSide_U( I_PacketSnd* inPacket, vuint32 inByteLength ) const;


	protected://////////////////////////////////////////////////////////////////////////////////

	/// --- Data members ---
	
		UChar*				m_pStartU;		///< Start of buffer.
		UChar*				m_pBufferEndU;	///< End of buffer.
		UChar*				m_pEndU;		///< End of the value. 

	protected://////////////////////////////////////////////////////////////////////////////////

		I_Localizable*		mpLocalizable;

		bool				mIsRemote; 		// implements <IsRemote> property.

		bool				mIsSingleByte;	// TRUE if this value keep single byte string.
		
mutable	bool				mNeedConvertedIsInited;			// guard for LAZY init of the following flag:
mutable bool				mNeedConvertToStorageEncoding;	// 

		COMPARE_TYPE		mCompareType;
		

	public://///////////////////////////////////////////////////////////////////////////////////

// Section of class with TEST - helper methods.
// for internal use.
 
#if FBL_TEST_CODE

							/**	This ctors allows init to Value by some string.
								This form will be used only in tests. 
								@param inStart	- begin of initialization string.
								@param inEnd	- end of initialization string. */
							Value_string( 
								const UChar* 		inStart, 
								const UChar* 		inEnd = nullptr,
                                I_Localizable_Ptr 	inLocalizable = nullptr );
							
							Value_string(
								const char* 		inStart, 
								const char* 		inEnd = nullptr,
                                I_Localizable_Ptr 	inLocalizable = nullptr );

							Value_string( 
                            	tslen 				inChars, 
                                int 				dummy );

#endif // FBL_TEST_CODE

virtual void				Init( void ) override;
virtual void 				Increment( void ) override;

//#endif // FBL_TEST_CODE
							
};


/**********************************************************************************************/
///
#pragma mark class Value_string_null

class FBL_SHARED_EXP_CLASS Value_string_null : public Value_string
{
	public://///////////////////////////////////////////////////////////////////////////////////

		typedef	Value_string_null								this_type;

		typedef       smart_ptr<this_type>						pointer;
		typedef const pointer									const_pointer;
		typedef       this_type&								reference;
		typedef const this_type&								const_reference;


	public://///////////////////////////////////////////////////////////////////////////////////

	// --- Construction ---

							Value_string_null( void );

							/**	This ctor allow create empty value_String with buffer of for 
								specified count of character. 
								@param inChars	- count of character for allocating buffer.*/
							Value_string_null( 
								tslen inChars,
								I_Localizable_Ptr	inLocalizable = nullptr,
								COMPARE_TYPE		inCompareType = kNatureCompare );

							/**	This is are copy-ctor. 
								@param inOther - other Value_string.*/
							Value_string_null( const_reference inOther );

virtual						~Value_string_null( void );
							

	public://///////////////////////////////////////////////////////////////////////////////////

// interface I_Value:

virtual	I_Value_Ptr			Clone( bool inCopyData = true ) const override;

	// ---------------------
	// Properties:

virtual bool				get_IsNullable( void ) const override  { return true; }	
virtual bool 				get_IsNull( void ) const override;
virtual void 				put_IsNull( bool inValue ) override;

	// ---------------------
	// Numeric properties:

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

	// ---------------------
	// Value access:

virtual	int					Compare( 
								const I_Value& 	inOther,
								COMPARE_TYPE 	inCompareType = kNatureCompare ) const override;


	protected://////////////////////////////////////////////////////////////////////////////////

		bool				mIsNull;		///< nullable flag.


	public://///////////////////////////////////////////////////////////////////////////////////

// Section of class with TEST - helper methods.
// for internal use.
 
#if FBL_TEST_CODE
							/**	This ctors allows init Value by some string.
								Actually this form will be used only in tests. 
								@param inStart	- begin of initialization string.
								@param inEnd	- end of initialization string. */
							Value_string_null( 
								const UChar* inStart, 
								const UChar* inEnd = nullptr,
                                I_Localizable_Ptr 	inLocalizable = nullptr );

							Value_string_null(
								const char* inStart, 
								const char* inEnd  = nullptr,
                                I_Localizable_Ptr 	inLocalizable = nullptr );

#endif // FBL_TEST_CODE

};


/**********************************************************************************************/
typedef Value_string Value_varchar;
typedef Value_string_null Value_varchar_null;


/**********************************************************************************************/
FBL_SMART_PTR( Value_string );
FBL_SMART_PTR( Value_varchar );
FBL_SMART_PTR( Value_string_null );
FBL_SMART_PTR( Value_varchar_null );


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_Value_String_h
