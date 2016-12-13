/**********************************************************************************************/
/* FBL_Value_Money.cpp                                                     					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Values/FBL_Value_Money.h>

// FBL:


/**********************************************************************************************/
FBL_Begin_Namespace

#pragma mark -
#pragma mark === Convert functions ===

/**********************************************************************************************/
void Convert_str_money(
	const I_Value*	inValue, 
	I_Value*		outValue );
void Convert_money_str(
	const I_Value*	inValue, 
	I_Value*		outValue );
void Convert_int_money(
	const I_Value*	inValue, 
	I_Value*		outValue );
void Convert_money_int(
	const I_Value*	inValue, 
	I_Value*		outValue );
void Convert_float_money(
	const I_Value*	inValue, 
	I_Value*		outValue );
void Convert_money_float(
	const I_Value*	inValue, 
	I_Value*		outValue );


/**********************************************************************************************/
void Convert_str_money(
	const I_Value*	inValue, 
	I_Value*		outValue )
{
	outValue->put_String( inValue->get_String() );
}


/**********************************************************************************************/
void Convert_money_str(
	const I_Value*	inValue, 
	I_Value*		outValue )
{	
	outValue->put_String( inValue->get_String() );
}


/**********************************************************************************************/
void Convert_int_money(
	const I_Value*	inValue, 
	I_Value*		outValue )
{
	outValue->put_LLong( inValue->get_LLong() );
}


/**********************************************************************************************/
void Convert_money_int(
	const I_Value*	inValue, 
	I_Value*		outValue )
{	
	outValue->put_LLong( inValue->get_LLong() );
}


/**********************************************************************************************/
void Convert_float_money(
	const I_Value*	inValue, 
	I_Value*		outValue )
{
	outValue->put_Double( inValue->get_Double() );
}


/**********************************************************************************************/
void Convert_money_float(
	const I_Value*	inValue, 
	I_Value*		outValue )
{	
	outValue->put_Double( inValue->get_Double() );
}


#pragma mark -
#pragma mark === Value_money_imp ===

/**********************************************************************************************/
vuint32	Value_money_imp::get_MaxChars( void ) const
{
	return kMoneyStrMaxLength;
}


/**********************************************************************************************/
String Value_money_imp::get_String( tslen inLimit ) const
{
	String str;
	
	if( inLimit != 0 )
	{
		tslen MaxLen = get_MaxChars();
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
void Value_money_imp::put_String( const String& inStr )
{
	if( inStr.isSingleByte() )
		put_String( inStr.getBufferA() );
	else
		put_String( inStr.c_str() );
}


/**********************************************************************************************/
UChar* Value_money_imp::get_String(
	UChar* 			outString,
	tslen 			inBufferChars ) const
{
	// The size of input buffer (inBufferChars) cannot be -1 !
	// Because this is unsafe then to write to  the memory 
	// location pointed by outString - because we don't know the size !
	if( outString == nullptr || inBufferChars <= 0 )
		return nullptr;

	char money_str[kMoneyStrMaxLength + 1];
	
	int num = snprintf(
				money_str,
				kMoneyStrMaxLength,
				"%.*f",
				(int) MONEY_PRECISION,
				get_Double() );

	tslen char_count = ( num < 0 ) ? kMoneyStrMaxLength : num;
	if( char_count > inBufferChars )
		char_count = inBufferChars;

	vu_a2u( money_str, outString, char_count );

	return outString + char_count;
}


/**********************************************************************************************/
void Value_money_imp::put_String(
	const UChar* 	inStart,
	const UChar* 	inEnd )
{
	argused1( inEnd );

	if( IsTrueStr( inStart ) )
		mValue = 1;
	else
		put_Double( u_utof( inStart ) );
	
	put_IsNull(false);
}


/**********************************************************************************************/
char* Value_money_imp::get_String(
	char* 			outString,
	tslen 			inBufferChars ) const
{
	// The size of input buffer (inBufferChars) cannot be -1 !
	// Because this is unsafe then to write to  the memory 
	// location pointed by outString - because we don't know the size !
	if( outString == nullptr || inBufferChars <= 0 )
		return nullptr;

	char money_str[kMoneyStrMaxLength + 1];

	int num = snprintf(
				money_str,
				kMoneyStrMaxLength,
				"%.*f",
				(int) MONEY_PRECISION,
				get_Double() );

	tslen char_count = ( num < 0 ) ? kMoneyStrMaxLength : num;
	if( char_count > inBufferChars )
		char_count = inBufferChars;

	memcpy( outString, money_str, char_count );
	*(outString + char_count) = 0;

	return outString + char_count;
}


/**********************************************************************************************/
void Value_money_imp::put_String(
	const char* 	inStart,
	const char* 	inEnd )
{
	argused1( inEnd );
	
	if( IsTrueStr( inStart ) )
		mValue = 1;
	else
		put_Double( atof(inStart) );

	put_IsNull(false);
}


/**********************************************************************************************/
void Value_money_imp::put_Boolean( bool inValue )
{
	argused1(inValue);
}


/**********************************************************************************************/
bool Value_money_imp::get_Boolean( void ) const
{
	return false;
}


/**********************************************************************************************/
void Value_money_imp::put_Byte( vuint8 inValue )
{
	mValue = DecimalFactor<MONEY_PRECISION>::value * static_cast<vint64>(inValue);
}


/**********************************************************************************************/
vuint8 Value_money_imp::get_Byte( void ) const
{
	return static_cast<vuint8>( round( get_Double() ) );
}


/**********************************************************************************************/
void Value_money_imp::put_Short( vint16 inValue )
{
	mValue = DecimalFactor<MONEY_PRECISION>::value * static_cast<vint64>(inValue);
}


/**********************************************************************************************/
vint16 Value_money_imp::get_Short( void ) const
{
	return static_cast<vint16>( round( get_Double() ) );
}

/**********************************************************************************************/
void Value_money_imp::put_UShort( vuint16 inValue )
{
	mValue = DecimalFactor<MONEY_PRECISION>::value * static_cast<vint64>(inValue);
}


/**********************************************************************************************/
vuint16 Value_money_imp::get_UShort( void ) const
{
	return static_cast<vuint16>( round( get_Double() ) );
}


/**********************************************************************************************/
void Value_money_imp::put_Medium( vint32 inValue )
{
	mValue = DecimalFactor<MONEY_PRECISION>::value * static_cast<vint64>(inValue);
}


/**********************************************************************************************/
vint32 Value_money_imp::get_Medium( void ) const
{
	return static_cast<vint32>( round( get_Double() ) );
}


/**********************************************************************************************/
void Value_money_imp::put_LLong( vint64 inValue )
{
	mValue = DecimalFactor<MONEY_PRECISION>::value * static_cast<vint64>(inValue);
}


/**********************************************************************************************/
vint64 Value_money_imp::get_LLong( void ) const
{
	return round( get_Double() );
}


/**********************************************************************************************/
void Value_money_imp::put_ULLong( vuint64 inValue )
{
	mValue = DecimalFactor<MONEY_PRECISION>::value * static_cast<vint64>(inValue);
}


/**********************************************************************************************/
vuint64 Value_money_imp::get_ULLong( void ) const
{
	return static_cast<vuint64>( round( get_Double() ) );
}


/**********************************************************************************************/
void Value_money_imp::put_Float( float inValue )
{
	mValue = round( static_cast<double>(DecimalFactor<MONEY_PRECISION>::value) * inValue );
}


/**********************************************************************************************/
float Value_money_imp::get_Float( void ) const
{
	return static_cast<float>(mValue) / DecimalFactor<MONEY_PRECISION>::value;
}


/**********************************************************************************************/
void Value_money_imp::put_Double( double inValue )
{
	mValue = round( static_cast<double>(DecimalFactor<MONEY_PRECISION>::value) * inValue );
}


/**********************************************************************************************/
double Value_money_imp::get_Double( void ) const
{
	return static_cast<double>(mValue) / DecimalFactor<MONEY_PRECISION>::value;
}


#pragma mark -
#pragma mark === Value_money ===

/**********************************************************************************************/
I_Value_Ptr Value_money::Clone( bool inCopyData ) const
{
	I_Value_Ptr result;

	if( inCopyData == true )
	{
		result = new Value_money( *this );
	}
	else
	{
		result = new Value_money();
		result->put_IsNull( true );
	}

	result->put_IsRemote(mIsRemote);
	return result;
}


#pragma mark -
#pragma mark === Value_money_null ===

/**********************************************************************************************/
bool Value_money_null::get_IsNull( void ) const
{
	return PN::get_IsNull(); 
}


/**********************************************************************************************/
void Value_money_null::put_IsNull( bool inValue )
{
	PN::put_IsNull( PS::get_ValuePtr(), inValue ); 
}
							

/**********************************************************************************************/
I_Value_Ptr Value_money_null::Clone( bool inCopyData ) const
{
	I_Value_Ptr result;

	if( inCopyData == true )
	{
		result = new Value_money_null( *this );
	}
	else
	{
		result = new Value_money_null();
		result->put_IsNull( true );
	}
	
	result->put_IsRemote(mIsRemote);
	return result;
}


/**********************************************************************************************/
int Value_money_null::Compare(
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
			return Value_money::Compare( inOther );
    	}
    }								
}


/**********************************************************************************************/
String Value_money_null::get_String( tslen	inLimit ) const
{
	if( get_IsNull() )
		return String::sEmpty();

	return Value_money::get_String(inLimit);
}


/**********************************************************************************************/
UChar* Value_money_null::get_String(
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

	return Value_money::get_String(outString, inBufferChars);
}


/**********************************************************************************************/
char* Value_money_null::get_String(
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

	return Value_money::get_String(outString, inBufferChars);
}


/**********************************************************************************************/
FBL_End_Namespace


