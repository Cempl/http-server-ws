/**********************************************************************************************/
/* FBL_Value_Time.cpp                                                     					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Values/FBL_Value_Time.h>

// FBL:
#include <VShared/FBL/publ/Util_classes/FBL_DTFormat.h>
#include <VShared/FBL/publ/Util_classes/FBL_ToUChar.h>
#include <VShared/FBL/publ/Util_Strings/FBL_String_Len_Const.h>
#include <VShared/FBL/publ/Util_Strings/FBL_ustdio.h>
#include <VShared/FBL/publ/Utilities/FBL_DateTimeEncode.h>


/**********************************************************************************************/
FBL_Begin_Namespace


#pragma mark === Convert functions ===


/**********************************************************************************************/
void Convert_str_time_aa_fast( 
	const char*		inString, 
	const DTFormat*	inDTFormat,
	TimeEncoded&	outTimeEnc )
{
	argused1( inDTFormat );

	if( !inString || *inString == 0 )
	{
		outTimeEnc.encoded = 0;
		return;
	}

	char c;
	short unsigned int h, mn, s, ms; /// time
	h = mn = s = ms = 0;

	sscanf( inString, "%hu%c%hu%c%hu%c%hu", &h, &c, &mn, &c, &s, &c, &ms );

	outTimeEnc.decoded.h = h;
	outTimeEnc.decoded.m = mn;
	outTimeEnc.decoded.s = s;
	outTimeEnc.decoded.ms = ms;
}


/**********************************************************************************************/
// WRAPPER, from UChar* to char* 
//
void Convert_str_time_uu_fast( 
	const UChar*		inStr, 
	const UChar*		inStrEnd, 
	const DTFormat*		inDTFormat,
	TimeEncoded&		outTimeEnc )
{
	char strTime[kTimeStrMaxLength + 1];
	
	tslen len = tslen( inStrEnd - inStr );
	vu_u2a(inStr, strTime, len);
	
	Convert_str_time_aa_fast(strTime, inDTFormat, outTimeEnc );
}


#pragma mark -


/**********************************************************************************************/
tslen Convert_time_str_uu_fast( 
	const TimeEncoded&	inTimeEnc, 
	const UChar			inSep, 
	UChar*				outString )
{
	char strTime[kTimeStrMaxLength + 1];
	
	tslen len = Convert_time_str_aa_fast(inTimeEnc, static_cast<char>(inSep), strTime);
	vu_a2u(strTime, outString, len);
	
	return len;	
}


/**********************************************************************************************/
tslen Convert_time_str_aa_fast( 
	const TimeEncoded&	inTimeEnc, 
	const char			inSep, 
	char*				outString )
{
	unsigned int ms = (unsigned int)inTimeEnc.decoded.ms % 1000;
	
	vuint32 Written = sprintf( outString, "%.2u%c%.2u%c%.2u%c%.3u", 
						inTimeEnc.decoded.h, (char)inSep, 
						inTimeEnc.decoded.m, (char)inSep, 
						inTimeEnc.decoded.s, (char)inSep, 
						ms );

	tslen ToMap = static_cast<tslen>(Written > kTimeStrMaxLength ? kTimeStrMaxLength : Written);
	outString[ToMap] = 0;
	
	return ToMap;
}


#pragma mark -
#pragma mark === Convert from UChar* ===


/**********************************************************************************************/
// Convert functions
//
void Convert_str_time_aa_aux( 
	const char*			inStr,
	const DTFormat* 	inDTFormat,
	TimeEncoded&		ioTime )
{
	if( !inStr  || *inStr == 0 )
	{
		ioTime.encoded = 0;
		return;
	}
	
	vint32 BadCharIndex = CheckTimeStringFormat( inStr, inDTFormat );
	if( BadCharIndex )
	{
		throw xValueError( 
					ERR_TIME_VALUE_NOT_MATCH_FORMAT, 
					String(inStr).c_str(), ToUCharPtr(BadCharIndex).c_str() );
	}

	// Now we know that string STRICTLY correspond to expected format,
	// so we can do fast extracting of numbers using scanf()
	Convert_str_time_aa_fast( inStr, inDTFormat, ioTime );

	// Final step - we check that extracted numbers fit limits: 
	if( CheckTimeValueLimits( ioTime ) == false )
		throw xValueError( ERR_TIME_VALUE_HAVE_WRONG_NUMBERS, inStr );
}


/**********************************************************************************************/
// WRAPPER: UChar* => char*
//
void Convert_str_time_uu_aux( 
	const UChar*		inStr,
	const UChar*		inStrEnd,
	const DTFormat* 	inDTFormat,
	TimeEncoded&		ioTime )
{
	if( !inStr )
		return;

	char strTime[kTimeStrMaxLength + 1];
	
	tslen len = inStrEnd ? tslen(inStrEnd - inStr) : vu_strlen(inStr);
	vu_u2a(inStr, strTime, len);

	Convert_str_time_aa_aux( strTime, inDTFormat, ioTime );
}


#pragma mark -
#pragma mark === Convert to/from string value ===


/**********************************************************************************************/
// Convert functions
//
void Convert_str_time( 
	const I_Value*	inValue, 
	I_Value*		outValue )
{
	I_ValueTime* pValueTime = dcast< I_ValueTime* >( outValue );
	const DTFormat* pDTFormat = pValueTime->get_DTFormat();

	Convert_str_time_uu_aux( 
		(UChar*) inValue->begin(),
		(UChar*) inValue->end(),
		pDTFormat,
		*(TimeEncoded*) outValue->begin() );	
}



/**********************************************************************************************/
void Convert_time_str( 
	const I_Value*	inValue, 
	I_Value*		outValue )
{
	const I_ValueTime* pValueTime = dcast< const I_ValueTime* >( inValue );
	const DTFormat* pDTFormat = pValueTime->get_DTFormat();

	char strTime[kTimeStrMaxLength + 1];	
	tslen len = Convert_time_str_aa_fast(*(TimeEncoded*)inValue->begin(), static_cast<char>(pDTFormat->mTimeSep), strTime);

	vuint32 outValueLen = outValue->get_MaxChars();
	len = ( (tslen)outValueLen < len ) ? outValueLen : len;
	vu_a2u(strTime, (UChar*) outValue->begin(), len);		
	outValue->put_ByteLength( len * sizeof(UChar) );		
}


/**********************************************************************************************/
void Convert_bin_time( 
	const I_Value*	inValue, 
	I_Value*		outValue );
void Convert_bin_time( 
	const I_Value*	inValue, 
	I_Value*		outValue )
{
	I_ValueTime* pValueTime = dcast< I_ValueTime* >( outValue );
	const DTFormat* pDTFormat = pValueTime->get_DTFormat();

	// TODO:
	// Currently we assume inValue is a single-byted string.
	// After we make everything localizable we can improve this using IOConverter as we do
	// for string values.
	//

	const char* pBinStart = inValue->begin();
	const char* pBinEnd = inValue->end();
	
	if( pBinStart )
	{
		char strTime[kTimeStrMaxLength + 1];
		
		tslen len = tslen(pBinEnd - pBinStart);
		memcpy( strTime, pBinStart, Min(len, (tslen)kTimeStrMaxLength) );

		Convert_str_time_aa_aux( 
			strTime,
			pDTFormat,
			*(TimeEncoded*) outValue->begin() );	
	}
}


#pragma mark -
#pragma mark === Value_time_imp ===


/**********************************************************************************************/
Value_time_imp::~Value_time_imp( void )
{
	if( mDTFormatOwner )
		ForgetObject(mDTFormat);
}


/**********************************************************************************************/
Value_time_imp::Value_time_imp( I_Database_Ptr inpDatabase )
{
	if( inpDatabase )
	{
		mpDatabase		= inpDatabase.get();
		mDTFormat		= nullptr;
		mDTFormatOwner	= false;		
	}
	else
	{
		mpDatabase		= nullptr;
		mDTFormat		= new DTFormat();
		mDTFormatOwner	= true;
	}	
}


/**********************************************************************************************/
Value_time_imp::Value_time_imp( const DTFormat* inDTFormat )
{
	mpDatabase			= nullptr;
	
	if (inDTFormat)
	{
		mDTFormat		= inDTFormat;
		mDTFormatOwner	= false;
	}
	else
	{
		mDTFormat		=  new DTFormat();
		mDTFormatOwner	= true;
	}
}


/**********************************************************************************************/
Value_time_imp::Value_time_imp( const Value_time_imp& inOther )
:
	Value_Numeric< kTypeTime, vcTime, policyInPlace_ulong >( inOther )
{
	mpDatabase		= inOther.mpDatabase;
	mDTFormat		= inOther.mDTFormat;
	mDTFormatOwner	= false;
}


#pragma mark -


/**********************************************************************************************/
const DTFormat* Value_time_imp::get_DTFormat( void ) const 
{
	FBL_CHECK( mpDatabase || mDTFormat );
	return (mpDatabase) ? mpDatabase->get_DTFormat() : mDTFormat;
}


/**********************************************************************************************/
vuint16 Value_time_imp::get_Hours( void ) const
{ 
	vuint16 result = ((TimeEncoded&)mValue).decoded.h; 
	return result;
}


/**********************************************************************************************/
void Value_time_imp::put_Hours( vuint16 inHours )
{ 
	((TimeEncoded&)mValue).decoded.h = inHours; 
	put_IsNull( false );
}


/**********************************************************************************************/
vuint16 Value_time_imp::get_Minutes( void ) const
{ 
	vuint16 result = ((TimeEncoded&)mValue).decoded.m; 
	return result;
}


/**********************************************************************************************/
void Value_time_imp::put_Minutes( vuint16 inMinutes )
{ 
	((TimeEncoded&)mValue).decoded.m = inMinutes; 
	put_IsNull( false );			
}


/**********************************************************************************************/
vuint16 Value_time_imp::get_Seconds( void ) const
{ 
	vuint32 result = (vuint32) ((TimeEncoded&)mValue).decoded.s;
	return static_cast<vuint16>(result);
}


/**********************************************************************************************/
void Value_time_imp::put_Seconds( vuint16 inSeconds )
{ 
	TimeEncoded& te = (TimeEncoded&) mValue;
	te.decoded.s = inSeconds; 
	put_IsNull( false );			
}


/**********************************************************************************************/
vuint16 Value_time_imp::get_Milliseconds( void ) const
{ 
	vuint16 result = ((TimeEncoded&)mValue).decoded.ms; 
	return result;
}


/**********************************************************************************************/
void Value_time_imp::put_Milliseconds( vuint16 inMilliseconds )
{ 
	((TimeEncoded&)mValue).decoded.ms = inMilliseconds; 
}


/**********************************************************************************************/
void Value_time_imp::get_Time( 
	vuint16& outHours, 
	vuint16& outMinutes, 
	vuint16& outSeconds,
	vuint16& outMilliseconds ) const
{ 
	TimeEncoded& te = (TimeEncoded&)mValue;

	outHours	= (vuint16) te.decoded.h;
	outMinutes	= (vuint16) te.decoded.m;
	
	vuint32 secs = (vuint32) te.decoded.s;
	outSeconds	= (vuint16)secs;
	
	outMilliseconds = te.decoded.ms;
}


/**********************************************************************************************/
void Value_time_imp::put_Time( 
	vuint16 	inHours, 
	vuint16 	inMinutes,
	vuint16 	inSeconds, 
	vuint16	inMilliseconds )
{ 
	TimeEncoded& te = (TimeEncoded&)mValue;

	te.decoded.h  = inHours;
	te.decoded.m  = inMinutes;
	te.decoded.s  = inSeconds;
	te.decoded.ms = inMilliseconds; 

	put_IsNull( false );			
}


/**********************************************************************************************/
String Value_time_imp::get_String( tslen inLimit ) const
{
	String str;

	if( inLimit != 0 )
	{
		UChar* p = str.getBuffer(kTimeStrMaxLength + 1);

		const DTFormat* pDTFormat = get_DTFormat();
	
		Convert_time_str_uu_fast( 
			(TimeEncoded&)mValue, pDTFormat->mTimeSep, p );
	
		str.releaseBuffer(kTimeStrMaxLength);

		// not -1 and less than maxLen
		if( inLimit > 0 && vuint32(inLimit) < kTimeStrMaxLength )
		{
			str.truncate( inLimit );
		}
	}

	return str;
}


/**********************************************************************************************/
void Value_time_imp::put_String( const String& inStr )
{
	if( inStr.isSingleByte() )
		put_String( inStr.getBufferA() );
	else
		put_String( inStr.c_str() );
}


/**********************************************************************************************/
UChar* Value_time_imp::get_String( 
	UChar*	outString, 
	tslen	inBufferChars ) const
{
	// The size of input buffer (inBufferChars) cannot be -1 !
	// Because this is unsafe then to write to  the memory 
	// location pointed by outString - because we don't know the size !
	if( outString == nullptr || inBufferChars <= 0 )
		return nullptr;

	UChar* pEnd = nullptr;

	if( inBufferChars < tslen(kTimeStrMaxLength) )
	{
		String res( get_String( inBufferChars ) );
		tslen len = res.length();
		memcpy( outString, res.begin(), len * sizeof(UChar) );
		*(outString + len) = 0;
		pEnd = outString + len;
	}
	else
	{
		const DTFormat* pDTFormat = get_DTFormat();
		Convert_time_str_uu_fast( 
			(TimeEncoded&)mValue, pDTFormat->mTimeSep, outString );
		pEnd = outString + kTimeStrMaxLength;
	}

	return pEnd;
}	


/**********************************************************************************************/
void Value_time_imp::put_String( 
	const UChar* inStart,  
	const UChar* inEnd )
{
	argused1( inEnd );
	
	const DTFormat* pDTFormat = get_DTFormat();
	Convert_str_time_uu_aux( inStart, inEnd, pDTFormat, (TimeEncoded&)mValue );
	
	put_IsNull( false );			
}


/**********************************************************************************************/
char* Value_time_imp::get_String( 
	char*	outString, 
	tslen 	inBufferChars ) const
{
	// The size of input buffer (inBufferChars) cannot be -1 !
	// Because this is unsafe then to write to  the memory 
	// location pointed by outString - because we don't know the size !
	if( outString == nullptr || inBufferChars <= 0 )
		return nullptr;

	char* pEnd = nullptr;

	if( inBufferChars < tslen(kTimeStrMaxLength) )
	{
		String res( get_String( inBufferChars ) );
		tslen len = res.length();
		memcpy( outString, res.getBufferA(), len );
		*(outString + len) = 0;
		pEnd = outString + len;
	}
	else
	{
		const DTFormat* pDTFormat = get_DTFormat();
		Convert_time_str_aa_fast( 
			(TimeEncoded&)mValue, static_cast<char>(pDTFormat->mTimeSep), outString );

		pEnd = outString + kTimeStrMaxLength;
	}

	return pEnd;
}


/**********************************************************************************************/
void Value_time_imp::put_String( const char* inStart, const char* inEnd )
{
	argused1( inEnd );
	
	const DTFormat* pDTFormat = get_DTFormat();
	Convert_str_time_aa_aux( inStart, pDTFormat, (TimeEncoded&)mValue );
	
	put_IsNull( false );			
}


/**********************************************************************************************/
vuint32 Value_time_imp::get_MaxChars( void ) const  
{ 
	return kTimeStrMaxLength;
}


#pragma mark -
#pragma mark === Value_time ===


/**********************************************************************************************/
Value_time::~Value_time( void )
{
}


/**********************************************************************************************/
Value_time::Value_time( I_Database_Ptr inpDatabase )
:
	Value_time_imp( inpDatabase )
{
}


/**********************************************************************************************/
Value_time::Value_time( const DTFormat* inDTFormat )
:
	Value_time_imp( inDTFormat )
{
}


/**********************************************************************************************/
Value_time::Value_time( const Value_time& inOther )
:
	Value_time_imp( inOther )
{
}


/**********************************************************************************************/
Value_time::Value_time( 
	vuint16			inHours,
	vuint16			inMinutes,
	vuint16			inSeconds,
	vuint16			inMilliseconds,
	const DTFormat*	inDTFormat )
:
	Value_time_imp( inDTFormat )
{
	put_Time( inHours, inMinutes, inSeconds, inMilliseconds );
}


/**********************************************************************************************/
I_Value_Ptr Value_time::Clone( bool inCopyData ) const 
{
	I_Value_Ptr result = new Value_time( *this );

	if( inCopyData == false )
		result->put_IsNull( true );

	result->put_IsRemote(mIsRemote);
	return result;
}


#pragma mark === Value_time_null ===


/**********************************************************************************************/
Value_time_null::~Value_time_null( void )
{
}


/**********************************************************************************************/
Value_time_null::Value_time_null( I_Database_Ptr inpDatabase )
:
	Value_time( inpDatabase )
{
}


/**********************************************************************************************/
Value_time_null::Value_time_null( const DTFormat* inDTFormat )
:
	Value_time( inDTFormat )
{
}


/**********************************************************************************************/
Value_time_null::Value_time_null( const Value_time_null& inOther )
:
	Value_time( inOther )
{
	put_IsNull( inOther.get_IsNull() );
}



/**********************************************************************************************/
Value_time_null::Value_time_null( 
	vuint16			inHours,
	vuint16			inMinutes,
	vuint16			inSeconds,
	vuint16			inMilliseconds,
	const DTFormat*	inDTFormat )
:
	Value_time( inHours, inMinutes, inSeconds, inMilliseconds, inDTFormat )
{
	put_IsNull(false);
}


#pragma mark -


/**********************************************************************************************/
I_Value_Ptr Value_time_null::Clone( bool inCopyData ) const 
{
	I_Value_Ptr result = new Value_time_null( *this );

	if( inCopyData == false )
		result->put_IsNull( true );

	result->put_IsRemote(mIsRemote);
	return result;
}


/**********************************************************************************************/
int Value_time_null::Compare( 
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
			return Value_time::Compare( inOther );    	
    	}
    }								
}


/**********************************************************************************************/
bool Value_time_null::get_IsNull( void ) const  	
{ 
	return PN::get_IsNull(); 
}


/**********************************************************************************************/
void Value_time_null::put_IsNull( bool inValue ) 	
{ 
	PN::put_IsNull( PS::get_ValuePtr(), inValue ); 
}


/**********************************************************************************************/
String Value_time_null::get_String( tslen inLimit ) const
{
	if( get_IsNull() )
		return String::sEmpty();

	return Value_time::get_String(inLimit);
}


/**********************************************************************************************/
UChar* Value_time_null::get_String( 
	UChar* outString, 
	tslen inBufferChars ) const
{
	if( get_IsNull() )
		if( inBufferChars > 0 )
		{
			outString[0] = 0;
			return outString;
		}

	return Value_time::get_String(outString, inBufferChars);
}


/**********************************************************************************************/
char* Value_time_null::get_String( 
	char* outString, 
	tslen inBufferChars ) const
{
	if( get_IsNull() )
		if( inBufferChars > 0 )
		{
			outString[0] = 0;
			return outString;
		}

	return Value_time::get_String(outString, inBufferChars);
}


/**********************************************************************************************/
//#if FBL_TEST_CODE

/**********************************************************************************************/
void Value_time_imp::Init( void ) 
{
	// Used in tests only so make it just for our datetime parser be happy.
	put_Hours(0);
	put_Minutes(0);
	put_Seconds(0);
	put_Milliseconds(1);
}


/**********************************************************************************************/
void Value_time_imp::Increment( void ) 
{
	// Used in tests only so make it just for our datetime parser be happy.

	if( mValue == 0 )
	{
		Init();
		return;
	}

	vuint16 hours = get_Hours();
	vuint16 minutes = get_Minutes();
	vuint16 seconds = get_Seconds();
	vuint16 ms = get_Milliseconds();

	if( ms < 999 )
	{
		put_Milliseconds( ++ms );
	}
	else if( seconds < 59 )
	{
		put_Milliseconds(0);
		put_Seconds( ++seconds );
	}
	else if( minutes < 59 )
	{
		put_Milliseconds(0);
		put_Seconds(0);
		put_Minutes( ++minutes );
	}
	else if( hours < 24 )
	{
		put_Milliseconds(0);
		put_Seconds(0);
		put_Minutes(0);
		put_Hours( ++hours );
	}
	else
	{
		// no nore time which makes parser happy. Actually we rarelly get here.
		Init();
	}
}

//#endif // FBL_TEST_CODE


/**********************************************************************************************/
FBL_End_Namespace
