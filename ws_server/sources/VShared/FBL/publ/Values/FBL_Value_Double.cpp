/**********************************************************************************************/
/* FBL_Value_Double.cpp                                                    					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Values/FBL_Value_Double.h>

/**********************************************************************************************/
FBL_Begin_Namespace

#pragma mark -


#pragma mark -
#pragma mark === Value_double_imp ===

/**********************************************************************************************/
String Value_double_imp::get_String( tslen inLimit ) const
{
	String str;

	if( inLimit != 0 )
	{
		tslen MaxLen = static_cast<tslen>(ps_traits::max_text_length());
		tslen BuffLength = MaxLen + 1;
		UChar* pBuffer = str.getBuffer(BuffLength);

		UChar* pEnd = get_String( pBuffer, MaxLen );

		str.releaseBuffer(static_cast<tslen>(pEnd - pBuffer));

		// not -1 and less than maxLen
		if( inLimit > 0 && inLimit < MaxLen )
		{
			str.truncate( inLimit );
		}
	}
	
	return str;
}


/**********************************************************************************************/
UChar* Value_double_imp::get_String( 
	UChar* outString, 
	tslen inBufferChars ) const
{
	// The size of input buffer (inBufferChars) cannot be -1 !
	// Because this is unsafe then to write to  the memory 
	// location pointed by outString - because we don't know the size !
	if( outString == nullptr || inBufferChars <= 0 )
		return nullptr;

	char double_str[kDoubleStrMaxLen + 1];

	int num = snprintf( double_str, kDoubleStrMaxLen, "%*.*f", (int) mPrecision, (int) mScale, mValue );
	
	tslen char_count = ( num < 0 ) ? kDoubleStrMaxLen : num;
	if( char_count > inBufferChars )
		char_count = inBufferChars;

	vu_a2u( double_str, outString, char_count );
	
	return outString + char_count;

/*
	MemPtr<char> double_str(inBufferChars);

	// Trying to get double in fixed format.
	int num = snprintf( double_str, inBufferChars, "%*.*f", (int) mPrecision, (int) mScale, mValue );

	// If str result does not fit into the buffer then try to get scientific notation
	if( num < 0 )
	{
		num = snprintf( double_str, inBufferChars, "%*.*G", (int) mPrecision, (int) mScale, mValue );
	}
	

	// num == -1 : even scientific notation does not fit into the buffer.
	tslen char_count = ( num < 0 ) ? 0 : num;
	FBL_CHECK( char_count <= inBufferChars );

	vu_a2u( double_str, outString, char_count );

	return outString + char_count;
*/	
}	


/**********************************************************************************************/
char* Value_double_imp::get_String( 
	char* outString, 
	tslen inBufferChars ) const
{
	// The size of input buffer (inBufferChars) cannot be -1 !
	// Because this is unsafe then to write to  the memory 
	// location pointed by outString - because we don't know the size !
	if( outString == nullptr || inBufferChars <= 0 )
		return nullptr;

	char double_str[kDoubleStrMaxLen + 1];
	int num = snprintf( double_str, kDoubleStrMaxLen, "%*.*f", (int) mPrecision, (int) mScale, mValue );

	tslen char_count = ( num < 0 ) ? kDoubleStrMaxLen : num;
	if( char_count > inBufferChars )
		char_count = inBufferChars;

	memcpy( outString, double_str, char_count );
	*(outString + char_count) = 0;

	return outString + char_count;
}


#pragma mark -


/**********************************************************************************************/
void Value_double_imp::put_String( const String& inStr )
{
	if( inStr.isSingleByte() )
		put_String( inStr.getBufferA() );
	else
		put_String( inStr.c_str() );
}


/**********************************************************************************************/
void Value_double_imp::put_String( 
	const UChar* inStart,  
	const UChar* inEnd )
{
	argused1( inEnd );

	if( IsTrueStr( inStart ) )
		mValue = (double) 1;
	else
		mValue = (double) u_utof( inStart );
	
	put_IsNull(false);
}


/**********************************************************************************************/
void Value_double_imp::put_String( 
	const char* inStart,
	const char* inEnd )
{
	argused1( inEnd );
	
	if( IsTrueStr( inStart ) )
		mValue = (double) 1;
	else
		mValue = static_cast<double>(atof(inStart));

	put_IsNull(false);
}


#pragma mark -
#pragma mark === Value_double ===

/**********************************************************************************************/
I_Value_Ptr Value_double::Clone( bool inCopyData ) const 
{
	I_Value_Ptr result;

	if( inCopyData == true )
	{
		result = new Value_double( *this );
	}
	else
	{
		result = new Value_double( mPrecision, mScale );
		result->put_IsNull( true );
	}

	result->put_IsRemote(mIsRemote);
	return result;
}


#pragma mark -
#pragma mark === Value_double_null ===

/**********************************************************************************************/
I_Value_Ptr Value_double_null::Clone( bool inCopyData ) const 
{
	I_Value_Ptr result;

	if( inCopyData == true )
	{
		result = new Value_double_null( *this );
	}
	else
	{
		result = new Value_double_null( mPrecision, mScale );
		result->put_IsNull( true );
	}
	
	result->put_IsRemote(mIsRemote);
	return result;
}


/**********************************************************************************************/
bool Value_double_null::get_IsNull( void ) const 
{
	return PN::get_IsNull(); 
}


/**********************************************************************************************/
void Value_double_null::put_IsNull( bool inValue ) 
{
	PN::put_IsNull( PS::get_ValuePtr(), inValue ); 
}
							
/**********************************************************************************************/
int Value_double_null::Compare( 
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
			return Value_double::Compare( inOther );    	
    	}
    }								
}


/**********************************************************************************************/
String Value_double_null::get_String( tslen	inLimit ) const
{
	if( get_IsNull() )
		return String::sEmpty();

	return Value_double::get_String(inLimit);
}


/**********************************************************************************************/
UChar* Value_double_null::get_String( 
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

	return Value_double::get_String(outString, inBufferChars);
}


/**********************************************************************************************/
char* Value_double_null::get_String( 
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

	return Value_double::get_String(outString, inBufferChars);
}


/**********************************************************************************************/
FBL_End_Namespace


