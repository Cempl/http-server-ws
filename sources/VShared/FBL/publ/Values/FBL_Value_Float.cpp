/**********************************************************************************************/
/* FBL_Value_Float.cpp                                                     					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Values/FBL_Value_Float.h>

// FBL:


/**********************************************************************************************/
FBL_Begin_Namespace

#pragma mark -


#pragma mark -
#pragma mark === Value_float_imp ===

/**********************************************************************************************/
String Value_float_imp::get_String( tslen inLimit ) const
{
	String str;

	if( inLimit != 0 )
	{
		tslen MaxLen = static_cast<tslen>( ps_traits::max_text_length() );
		tslen BuffLength = MaxLen + 1;
		UChar* pBuffer = str.getBuffer( BuffLength );
		UChar* pEnd    = get_String( pBuffer, MaxLen ); // copy str into buffer.

		str.releaseBuffer( static_cast<tslen>(pEnd - pBuffer) );

		// not -1 and less than maxLen
		if( inLimit > 0 && inLimit < MaxLen )
		{
			str.truncate( inLimit );
		}
	}

	return str;
}


/**********************************************************************************************/
UChar* Value_float_imp::get_String( 
	UChar* outString, 
	tslen inBufferChars ) const
{
	// The size of input buffer (inBufferChars) cannot be -1 !
	// Because this is unsafe then to write to  the memory 
	// location pointed by outString - because we don't know the size !
	if( outString == nullptr || inBufferChars <= 0 )
		return nullptr;

	char float_str[kFloatStrMaxLen + 1];
	
//	tslen num = ( mScale == gFloatDefaultScale ) ?
//		static_cast<tslen>(sprintf( float_str, "%G", mValue )) :
//		static_cast<tslen>(sprintf( float_str, "%.*f", (int) mScale, mValue ));

	int num = snprintf( float_str, kFloatStrMaxLen, "%*.*f", (int) mPrecision, (int) mScale, mValue );

	tslen char_count = ( num < 0 ) ? kFloatStrMaxLen : num;
	if( char_count > inBufferChars )
		char_count = inBufferChars;

	vu_a2u( float_str, outString, char_count );

	return outString + char_count;
}	


/**********************************************************************************************/
char* Value_float_imp::get_String( 
	char* outString, 
	tslen inBufferChars ) const
{
	// The size of input buffer (inBufferChars) cannot be -1 !
	// Because this is unsafe then to write to  the memory 
	// location pointed by outString - because we don't know the size !
	if( outString == nullptr || inBufferChars <= 0 )
		return nullptr;

	char float_str[kFloatStrMaxLen + 1];

	int num = snprintf( float_str, kFloatStrMaxLen, "%*.*f", (int) mPrecision, (int) mScale, mValue );			

	tslen char_count = ( num < 0 ) ? kFloatStrMaxLen : num;
	if( char_count > inBufferChars )
		char_count = inBufferChars;

	memcpy( outString, float_str, char_count );
	*(outString + char_count) = 0;

	return outString + char_count;
}


#pragma mark -


/**********************************************************************************************/
void Value_float_imp::put_String( const String& inStr )
{
	if( inStr.isSingleByte() )
		put_String( inStr.getBufferA() );
	else
		put_String( inStr.c_str() );
}


/**********************************************************************************************/
void Value_float_imp::put_String( 
	const UChar* inStart,  
	const UChar* inEnd )
{
	argused1( inEnd );

	if( IsTrueStr( inStart ) )
		mValue = (float) 1;
	else
		mValue = (float) u_utof( inStart );
	
	put_IsNull(false);
}


/**********************************************************************************************/
void Value_float_imp::put_String( 
	const char* inStart,
	const char* inEnd )
{
	argused1( inEnd );
	
	if( IsTrueStr( inStart ) )
		mValue = (float) 1;
	else
		mValue = static_cast<float>(atof(inStart));

	put_IsNull(false);
}


#pragma mark -
#pragma mark === Value_float ===

/**********************************************************************************************/
I_Value_Ptr Value_float::Clone( bool inCopyData ) const 
{
	I_Value_Ptr result;

	if( inCopyData == true )
	{
		result = new Value_float( *this );
	}
	else
	{
		result = new Value_float( mPrecision, mScale );
		result->put_IsNull( true );
	}

	result->put_IsRemote(mIsRemote);
	return result;
}


#pragma mark -
#pragma mark === Value_float_null ===

/**********************************************************************************************/
I_Value_Ptr Value_float_null::Clone( bool inCopyData ) const 
{
	I_Value_Ptr result;

	if( inCopyData == true )
	{
		result = new Value_float_null( *this );
	}
	else
	{
		result = new Value_float_null( mPrecision, mScale );
		result->put_IsNull( true );
	}
	
	result->put_IsRemote(mIsRemote);
	return result;
}


/**********************************************************************************************/
bool Value_float_null::get_IsNull( void ) const 
{
	return PN::get_IsNull(); 
}


/**********************************************************************************************/
void Value_float_null::put_IsNull( bool inValue ) 
{
	PN::put_IsNull( PS::get_ValuePtr(), inValue ); 
}
							
/**********************************************************************************************/
int Value_float_null::Compare( 
	const I_Value& 	inOther, 
	COMPARE_TYPE 	inCompareType ) const 
{
	argused1( inCompareType );
	FBL_CHECK( get_Type() == inOther.get_Type() ); 

	// If one of values (this or inOther) have null...
	if( get_IsNull() )
    {
	    if( inOther.get_IsNull() )
	        return 0; // Both are NULL
	    else 
	        return -1; // Any NULL is less then NOT NULL                               
    }
    else 
    {
    	if( inOther.get_IsNull() )
           	return 1; // Any NOT NULL is greater then NULL
    	else
    	{
			// ... have not null values - compare them.
			return Value_float::Compare( inOther );    	
    	}
    }								
}


/**********************************************************************************************/
String Value_float_null::get_String( tslen	inLimit ) const
{
	if( get_IsNull() )
		return String::sEmpty();

	return Value_float::get_String(inLimit);
}


/**********************************************************************************************/
UChar* Value_float_null::get_String( 
	UChar* 		outString, 
	tslen 		inBufferChars ) const
{
	if( get_IsNull() )
	{
		if( inBufferChars > 0 )
		{
			outString[0] = 0;
			return outString;
		}
	}

	return Value_float::get_String(outString, inBufferChars);
}


/**********************************************************************************************/
char* Value_float_null::get_String( 
	char* 		outString, 
	tslen 		inBufferChars ) const
{
	if( get_IsNull() )
	{
		if( inBufferChars > 0 )
		{
			outString[0] = 0;
			return outString;
		}
	}

	return Value_float::get_String(outString, inBufferChars);
}


/**********************************************************************************************/
FBL_End_Namespace


