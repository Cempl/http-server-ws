/**********************************************************************************************/
/* FBL_Value_Compound.h 	                                                   				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_Value_Compound_h
	#define _FBL_Value_Compound_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Interfaces/FBL_I_Value.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_ValueCompound.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_ARRAY_OF_SIMPLES( ArrayOfULongs, vuint32 );


/**********************************************************************************************/
SMART_CLASS(Value_Compound);


/**********************************************************************************************/
// Implements Compound Value (initial 12/30/11 for v5.x)
// 
// This is a value, which is made as set of few other simple values.
// Intended first of all for Compound Indexes, based on few fields.
// 
// This value can be considered IsNullable, if all its sub-values are such.
// This value should answer NULL if all its sub-values are NULLs.
//
// For this value have no sense get_ put_ some simple type methods.
// They all have assert and zero job ...
//
// Major job for this field is COMPARE of two compound values, when we search them in the index.
// Comparision just calls sub-values one by one. 
//
class FBL_SHARED_EXP_CLASS Value_Compound 
: 
    public I_Value,
    public I_ValueCompound
//  public I_Serializable	// Compound value only for internal use to build Compound indexes.
{
		IMPLEMENT_UNKNOWN(Value_Compound)											
		BEGIN_INTERFACE_TABLE(Value_Compound)										
			IMPLEMENT_INTERFACE(I_Value)										
			IMPLEMENT_INTERFACE(I_ValueCompound)										
		END_INTERFACE_TABLE()

	public://///////////////////////////////////////////////////////////////////////////////////

							Value_Compound( 
                            	ArrayOfValues_Ptr	inOriginalValues );

							Value_Compound( 
                            	const Value_Compound& inOther );


virtual	I_Value_Ptr			Clone( bool inCopyData = true ) const override;


	public://///////////////////////////////////////////////////////////////////////////////////

// I_Value API:

	// ---------------------
	// Properties:

virtual vuint32 			get_Allocated( void ) const override;
virtual bool				put_Allocated( vuint32 inSize ) override;

virtual vuint32 			get_ByteLength( void ) const override;
virtual void 				put_ByteLength( vuint32 inNewLength ) override;

virtual	vuint32				get_ByteLengthForIndex( void ) const override;
virtual VALUE_CATEGORY		get_Category( void ) const override;

virtual bool				get_IsNullable( void ) const override;
virtual bool 				get_IsNull( void ) const override;
virtual void 				put_IsNull( bool inValue ) override;
							
virtual bool				get_IsRemote( void ) const override;
virtual void 				put_IsRemote( bool inValue ) override;

virtual vuint32 			get_Length( void ) const override;

virtual vuint32				get_MaxChars( void ) const override;

virtual vuint32				get_StorageSize( void ) const override;

virtual VALUE_TYPE			get_Type( void ) const override;
virtual String				get_TypeString( const char inLocale[] = "en_US" ) const override;


	// ---------------------
	// Value Access Methods:
	
virtual const char*			begin( void ) const override;
virtual const char*			end( void ) const override;

virtual	void				Assign( const I_Value& inValue ) override;

virtual vuint32				CopyToIndex( void* outBuffer, bool inSwapBytes ) const override;
virtual void 				CopyFromIndex( const void* inBuffer, bool inSwapBytes ) override;


	// ---------------------
	// Value Compare Methods:

virtual	int					Compare( 
								const I_Value& 		inOther,
								COMPARE_TYPE 		inCompareType = kNatureCompare ) const override;

virtual	int					CompareIndexValues( 
								const void* 		inLeft,
								const void* 		inRight,
								bool				inSwapBytes) const override;

virtual	int					CompareToIndexValue( 
								Const_I_Value_Ptr 	inTestValue,
								const void* 		inIndexValue,
								vuint32				inParam,
								bool				inSwapBytes ) const override;

	// ---------------------
	// Get Value as String:

virtual String				get_String( 
								tslen 			inLimit = -1 ) const override;
                                
virtual UChar*				get_String( 
								UChar* 			outString, 
								tslen 			inBufferChars ) const override;

virtual char*				get_String( 
								char* 			outString, 
								tslen 			inBufferChars ) const override;

	// ---------------------
	// Put String into Value:

virtual void				put_String( 
								const String& 	inStr ) override;

virtual void				put_String( 
								const UChar* 	inStart,
								const UChar* 	inEnd = nullptr ) override;
								
virtual void				put_String( 
								const char* 	inStart,
								const char* 	inEnd = nullptr ) override;

	// ---------------------
	// Numeric properties:

virtual bool				get_Boolean( void ) const override;
virtual void				put_Boolean( bool inValue ) override;

virtual vuint8				get_Byte( void ) const override;
virtual void				put_Byte( vuint8 inValue ) override;

virtual vint16				get_Short( void ) const override;
virtual void				put_Short( vint16  inValue ) override;

virtual vuint16				get_UShort( void ) const override;
virtual void				put_UShort( vuint16 inValue ) override;

virtual vint32				get_Medium( void ) const override;
virtual void				put_Medium( vint32  inValue ) override;

virtual vint32				get_Long( void ) const override;
virtual void				put_Long  ( vint32  inValue ) override;

virtual vuint32				get_ULong( void ) const override;
virtual void				put_ULong ( vuint32 inValue ) override;

virtual vint64				get_LLong( void ) const override;
virtual void				put_LLong ( vint64  inValue ) override;

virtual vuint64				get_ULLong( void ) const override;
virtual void				put_ULLong( vuint64 inValue ) override;

virtual float				get_Float( void ) const override;
virtual void				put_Float ( float   inValue ) override;

virtual double				get_Double( void ) const override;
virtual void				put_Double( double  inValue ) override;


	// ---------------------
	// Byte Swapping:

virtual void				SwapBytes( void ) override;
virtual void				SwapBytes( void* inValue ) const override;
virtual void				SwapBytes( const void* inSrc, void* inDest ) override;


//#if FBL_TEST_CODE
virtual void				Init( void ) override;
virtual void 				Increment( void ) override; 
//#endif // FBL_TEST_CODE


	public://///////////////////////////////////////////////////////////////////////////////////

// I_ValueCompound API:

virtovr	bool				get_IsVariableLength( void ) const override
								{ return mIsVariable; }

virtovr vuint32				get_SubValueCount( void ) const override
								{ return mpValues->get_Count(); }
								
virtovr I_Value_Ptr			get_SubValue( vuint16 inValueIndex ) const override
								{ return mpValues->get_ItemAt( inValueIndex ); }


	protected:////////////////////////////////////////////////////////////////////////////////// 

// THIS CLASS Methods:

		void 				InitWithArray( 
                                ArrayOfValues_Ptr 	inValues,
                                bool				inCopyData );

		vuint32 			CalcSizeOfSubValue( 
        						I_Value_Ptr 		inSubValue );

		vuint32 			IndexPrefixLen( void ) const
        						{ return mIsVariable ? 2 : 0; }


	protected:////////////////////////////////////////////////////////////////////////////////// 
    
    	ArrayOfValues_Ptr	mpValues;	// Array of Values, which this value compounds.
                                        // These values are clones of original Field's values,
                                        // so they know info about e.g. Field Locale settings.
                                        
		ArrayOfULongs_Ptr	mpSizes;	// To implement storage of compund value into index.
                                        // For numeric types contains sizeof type
                                        // For strings/VarChar contains ZERO, so we must 
                                        // at runtime ask actual length from sub-value.
                                        
    	bool				mIsVariable; // TRUE if at least on subValue is string or such.		
                                        // Thanks to this info we can do optimisations.
};



/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_Value_Compound_h
