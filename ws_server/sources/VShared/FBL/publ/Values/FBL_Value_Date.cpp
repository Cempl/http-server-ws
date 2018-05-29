/**********************************************************************************************/
/* FBL_Value_Date.cpp                                                      					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Values/FBL_Value_Date.h>

// FBL:
#include <VShared/FBL/publ/Util_classes/FBL_DTFormat.h>
#include <VShared/FBL/publ/Util_classes/FBL_ToUChar.h>
#include <VShared/FBL/publ/Util_Strings/FBL_String_Len_Const.h>
#include <VShared/FBL/publ/Utilities/FBL_DateTimeEncode.h>


/**********************************************************************************************/
FBL_Begin_Namespace


#pragma mark === Internal convert functions ===


/**********************************************************************************************/
// WRAPPER, from UChar* to char* 
//
vuint16 Convert_str_date_uu_fast( 
	const UChar*	inStr, 
	const UChar*	inStrEnd, 
	const DTFormat*	inDateFormat,
	DateEncoded&	outDateEnc )
{
	/// maybe this is a temporary solution...
	char strDate[kDateStrMaxLength + 1];

	tslen len = tslen( inStr - inStrEnd );
	vu_u2a( inStr, strDate, len );

	return Convert_str_date_aa_fast( strDate, inDateFormat, outDateEnc );
}


/**********************************************************************************************/
// Internal convert functions. For HI perfomance.
//
vuint16 Convert_str_date_aa_fast( 
	const char*			inString, 
	const DTFormat*		inFormat,
	DateEncoded&		outDateEnc )
{
	if( !inString  || *inString == 0 )
	{
		outDateEnc.encoded = 0;
		return 4;
	}

	char c;
	bool isNegative = false;

	/// check if a date is negative, then skip "-".
	if( *inString == '-' )
	{
		inString++;
		isNegative = true;
	}
		
	vuint16 yearDigits = 0; // how much digits present in the YEAR part: e.g. 2 or 4.
	short unsigned int d = 0, m = 0;
	long int y = 0;

	switch( inFormat->mDateFormat )
	{
		case kMDY: 
		{
			sscanf( inString, "%hu%c%hu%c%ld", &m, &c, &d, &c, &y );
			
			// Find last '-' symbol.
			const char* pc = strrchr(inString, inFormat->mDateSep);
			yearDigits = static_cast<vuint16>(pc ? inString + (strlen(inString) - 1) - pc : 0);

		} break;

		case kDMY: 
		{
			sscanf( inString, "%hu%c%hu%c%ld", &d, &c, &m, &c, &y );

			// Find last '-' symbol.
			const char* pc = strrchr(inString, inFormat->mDateSep);
			yearDigits = static_cast<vuint16>(pc ? inString + (strlen(inString) - 1) - pc : 0);

		} break;

		case kYMD: 
		{
			sscanf( inString, "%ld%c%hu%c%hu", &y, &c, &m, &c, &d );
			
			// Find now first '-' symbol.
			const char* pc = strchr(inString, inFormat->mDateSep);
			yearDigits = static_cast<vuint16>(pc ? pc - inString : 0);

		} break;

		case kMYD: 
		{
			sscanf( inString, "%hu%c%ld%c%hu", &m, &c, &y, &c, &d );
			
			// Find now first '-' symbol.
			const char* pc = strchr(inString, inFormat->mDateSep);
			if( pc )
			{
				const char* pc2 = strchr(pc, inFormat->mDateSep);
				yearDigits = static_cast<vuint16>(pc2 ? pc2 - pc : 0);
			}

		} break;

		case kDYM: 
		{
			sscanf( inString, "%hu%c%ld%c%hu", &d, &c, &y, &c, &m );
			
			// Find now first '-' symbol.
			const char* pc = strchr(inString, inFormat->mDateSep);
			if( pc )
			{
				const char* pc2 = strchr(pc, inFormat->mDateSep);
				yearDigits = static_cast<vuint16>(pc2 ? pc2 - pc : 0);
			}

		} break;


		case kYDM: 
		{
			sscanf( inString, "%ld%c%hu%c%hu", &y, &c, &d, &c, &m );
			
			// Find now first '-' symbol.
			const char* pc = strchr(inString, inFormat->mDateSep);
			yearDigits = static_cast<vuint16>(pc ? pc - inString : 0);

		} break;

 
		default:
		{
			FBL_CHECK( false );
			d  = m = 0;
			y = 0;
			yearDigits = 4;
		}
		break;
	}

	if( isNegative )
		y *= -1;

	outDateEnc.decoded.d = d;
	outDateEnc.decoded.m = m;
	outDateEnc.decoded.y = y;

	return yearDigits;
}


#pragma mark -


/**********************************************************************************************/
tslen Convert_date_str_fast( 
	const DateEncoded&	inDateEnc, 
	const EDateFormat	inDateFormat,
	const UChar			inSep, 
	UChar*				outString )
{
	/// maybe this is temporary solution...
	char strDate[kDateStrMaxLength+1];
	
	tslen len = Convert_date_str_fast(inDateEnc, inDateFormat, (char)inSep, strDate);
	vu_a2u(strDate, outString, len);

	return len;
}


/**********************************************************************************************/
tslen Convert_date_str_fast( 
	const DateEncoded&	inDateEnc, 
	const EDateFormat	inDateFormat,
	const char			inSep, 
	char*				outString )
{
	vuint32 Written = 0;

	vint32 year = inDateEnc.decoded.y % 10000;
	
	switch( inDateFormat )
	{
		case kMDY:	Written = sprintf( outString, "%.2u%c%.2u%c%.4li", 
			inDateEnc.decoded.m, inSep, inDateEnc.decoded.d, inSep, (long)year );
			break;
			
		case kDMY:	Written = sprintf( outString, "%.2u%c%.2u%c%.4li", 
			inDateEnc.decoded.d, inSep, inDateEnc.decoded.m, inSep, (long)year );
			break;
			
		case kYMD:	Written = sprintf( outString, "%.4li%c%.2u%c%.2u", 
			(long)year, inSep, inDateEnc.decoded.m, inSep, inDateEnc.decoded.d );
			break;

		case kMYD:	Written = sprintf( outString, "%.2u%c%.4li%c%.2u", 
			inDateEnc.decoded.m, inSep, (long)year, inSep, inDateEnc.decoded.d );
			break;

		case kDYM:	Written = sprintf( outString, "%.2u%c%.4li%c%.2u", 
			inDateEnc.decoded.d, inSep, (long)year, inSep, inDateEnc.decoded.m );
			break;

		case kYDM:	Written = sprintf( outString, "%.4li%c%.2u%c%.2u", 
			(long)year, inSep, inDateEnc.decoded.d, inSep, inDateEnc.decoded.m );
			break;
	}
	
	tslen ToMap = static_cast<tslen>(Written > kDateStrMaxLength ? kDateStrMaxLength : Written);
	outString[ToMap] = 0;
	
	return ToMap;	
}


#pragma mark -
#pragma mark === Convert from UChar*  ===


/**********************************************************************************************/
void Convert_str_date_aa_aux( 
	const char*			inStr,
	const DTFormat* 	inDTFormat,
	DateEncoded&		ioDate )
{
	if( !inStr || *inStr == 0 )
	{
		ioDate.encoded = 0;
		return;
	}

	vint32 BadCharIndex = CheckDateStringFormat( inStr, inDTFormat );
	if( BadCharIndex )
	{
		throw xValueError( 
					ERR_DATE_VALUE_NOT_MATCH_FORMAT, 
					String(inStr).c_str(), ToUCharPtr(BadCharIndex).c_str() );
	}

	// Now we know that string STRICTLY correspond to expected format,
	// so we can do fast extracting of numbers using scanf()
	vuint16 yearDigits = Convert_str_date_aa_fast( inStr, inDTFormat, ioDate );
		
	// Final step - we check that extracted numbers fit limits: 
	if( CheckDateValueLimits( ioDate ) == false )
		throw xValueError( ERR_DATE_VALUE_HAVE_WRONG_NUMBERS, inStr );
	
	if( yearDigits < 4 )
	{
		vint32 year = ioDate.decoded.y;
		inDTFormat->AutoCentury( year );
		ioDate.decoded.y = year;
	}		
}


/**********************************************************************************************/
// WRAPPER: UChar* => char*
//
void Convert_str_date_uu_aux( 
	const UChar*		inStr,
	const UChar*		inStrEnd,
	const DTFormat* 	inDTFormat,
	DateEncoded&		ioDate )
{
	if( !inStr )
		return;

	char strDate[kDateStrMaxLength + 1];

	tslen len = inStrEnd ? tslen(inStrEnd - inStr) : vu_strlen(inStr);
	vu_u2a( inStr, strDate, len );

	return Convert_str_date_aa_aux( 
					strDate, 
					inDTFormat,
					ioDate );
}


#pragma mark -
#pragma mark === Convert to/from string value ===


/**********************************************************************************************/
// WRAPPER: I_Value => UChar*
//
void Convert_str_date( 
	const I_Value*	inValue, 
	I_Value*		outValue )
{
	I_ValueDate* pValueDate = dcast< I_ValueDate* >( outValue );
	const DTFormat* pDTFormat = pValueDate->get_DTFormat();

	Convert_str_date_uu_aux( 
		(UChar*) inValue->begin(),
		(UChar*) inValue->end(),
		pDTFormat,
		*(DateEncoded*) outValue->begin() );	
}


/**********************************************************************************************/
void Convert_date_str( 
	const I_Value*	inValue, 
	I_Value*		outValue)
{
	const I_ValueDate* pValueDate = dcast< const I_ValueDate* >( inValue );
	const DTFormat* pDTFormat = pValueDate->get_DTFormat();

	/// maybe this is temporary solution...
	char strDate[kDateStrMaxLength+1];
	
	tslen len = Convert_date_str_fast(	*(DateEncoded*)inValue->begin(), 
										pDTFormat->mDateFormat, 
										(char)pDTFormat->mDateSep, 
										strDate);

	vuint32 outValueLen = outValue->get_MaxChars();
	len = ( (tslen)outValueLen < len ) ? outValueLen : len;
	vu_a2u(strDate, (UChar*) outValue->begin(), len);		
	outValue->put_ByteLength( len * sizeof(UChar) );		
}


/**********************************************************************************************/
// WRAPPER: I_Value => char*
//
void Convert_bin_date( 
	const I_Value*	inValue, 
	I_Value*		outValue );
void Convert_bin_date( 
	const I_Value*	inValue, 
	I_Value*		outValue )
{
	// TODO:
	// Currently we assume inValue is a single-byted string.
	// After we make everything localizable we can improve this using IOConverter as we do
	// for string values.
	//
	I_ValueDate* pValueDate = dcast< I_ValueDate* >( outValue );
	const DTFormat* pDTFormat = pValueDate->get_DTFormat();

	const char* pBinStart = inValue->begin();
	const char* pBinEnd = inValue->end();
	
	if( pBinStart )
	{
		char strDate[kDateStrMaxLength + 1];

		tslen len = tslen(pBinEnd - pBinStart);
		memcpy( strDate, pBinStart, Min(len, (tslen)kDateStrMaxLength) );

		Convert_str_date_aa_aux( 
			strDate,
			pDTFormat,
			*(DateEncoded*) outValue->begin() );	
	}
}


#pragma mark -
#pragma mark === Value_date_imp ===


/**********************************************************************************************/
Value_date_imp::~Value_date_imp( void )
{
	if( mDTFormatOwner )
		ForgetObject(mDTFormat);
}


/**********************************************************************************************/
Value_date_imp::Value_date_imp( I_Database_Ptr inpDatabase )
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
Value_date_imp::Value_date_imp( const DTFormat* inDTFormat )
{
	mpDatabase			= nullptr;
	
	if (inDTFormat)
	{
		mDTFormat		= inDTFormat;
		mDTFormatOwner	= false;
	}
	else
	{
		mDTFormat		= new DTFormat();
		mDTFormatOwner	= true;
	}
}


/**********************************************************************************************/
Value_date_imp::Value_date_imp( const Value_date_imp& inOther )
:
	Value_Numeric< kTypeDate, vcDate, policyInPlace_long >( inOther )
{
	mpDatabase		= inOther.mpDatabase;
	mDTFormat		= inOther.mDTFormat;
	mDTFormatOwner	= false;	// Because of either inOther.mDTFormat or mpDatabase
}


#pragma mark -


/**********************************************************************************************/
const DTFormat* Value_date_imp::get_DTFormat( void ) const
{
	FBL_CHECK( mpDatabase || mDTFormat );
	return (mpDatabase) ? mpDatabase->get_DTFormat() : mDTFormat;
}


/**********************************************************************************************/
vint32 Value_date_imp::get_Year( void ) const
{ 
	vint32 result = ((DateEncoded&)mValue).decoded.y;
	return result;
}


/**********************************************************************************************/
void Value_date_imp::put_Year( vint32 inYear )
{
	const DTFormat* pDTFormat = get_DTFormat();
	
	// Year correction
	pDTFormat->AutoCentury( inYear );
	((DateEncoded&)mValue).decoded.y = inYear; 
	put_IsNull( false );			
}


/**********************************************************************************************/
vuint16 Value_date_imp::get_Month( void ) const
{ 
	vuint16 result = ((DateEncoded&)mValue).decoded.m; 
	return result;
}


/**********************************************************************************************/
void Value_date_imp::put_Month( vuint16 inMonth )
{ 
	((DateEncoded&)mValue).decoded.m = inMonth;
	put_IsNull( false );			
}


/**********************************************************************************************/
vuint16 Value_date_imp::get_Day( void ) const
{
	vuint16 result = ((DateEncoded&)mValue).decoded.d; 
	return result;
}


/**********************************************************************************************/
void Value_date_imp::put_Day( vuint16 inDay )
{ 
	((DateEncoded&)mValue).decoded.d = inDay; 
	put_IsNull( false );			
}


/**********************************************************************************************/
void Value_date_imp::get_Date( 
	vint32&	outYear, 
	vuint16& outMonth, 
	vuint16& outDay ) const
{ 
	DateEncoded& de = (DateEncoded&)mValue;

	outYear	 = de.decoded.y;
	outMonth = de.decoded.m;
	outDay	 = de.decoded.d;
}


/**********************************************************************************************/
void Value_date_imp::put_Date( 
	vint32	inYear, 
	vuint16	inMonth, 
	vuint16	inDay )
{ 
	DateEncoded& de = (DateEncoded&)mValue;

	de.decoded.y = inYear;
	de.decoded.m = inMonth;
	de.decoded.d = inDay;
	put_IsNull( false );			
}


/**********************************************************************************************/
vuint32 Value_date_imp::get_MaxChars( void ) const  
{ 
	return ( ((DateEncoded&)mValue).encoded < 0 ) ? 
								kDateStrMaxLength : 
								kDateStrMaxLength - 1;
}


#pragma mark -


/**********************************************************************************************/
String Value_date_imp::get_String( tslen inLimit ) const
{
	String str;
	if( inLimit != 0 )
	{
		const DTFormat* pDTFormat = get_DTFormat();

		UChar* p = str.getBuffer(kDateStrMaxLength + 1);
		Convert_date_str_fast( 
			(DateEncoded&)mValue, pDTFormat->mDateFormat, pDTFormat->mDateSep, p );
	
		if( ((DateEncoded&)mValue).encoded < 0 )
			str.releaseBuffer(kDateStrMaxLength);		// 11 symbols for negative dates
		else
			str.releaseBuffer(kDateStrMaxLength - 1);	// 10 symbols for positive dates

		// not -1 and less than maxLen
		if( inLimit > 0 && vuint32(inLimit) < kDateStrMaxLength )
		{
			str.truncate( inLimit );
		}
	}

	return str;
}


/**********************************************************************************************/
UChar* Value_date_imp::get_String( 
	UChar* outString, 
	tslen inBufferChars ) const
{
	// The size of input buffer (inBufferChars) cannot be -1 !
	// Because this is unsafe then to write to  the memory 
	// location pointed by outString - because we don't know the size !
	if( outString == nullptr || inBufferChars <= 0 )
		return nullptr;

	UChar* pEnd = nullptr;

	if( inBufferChars < tslen(kDateStrMaxLength) )
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

		Convert_date_str_fast( 
			(DateEncoded&)mValue, pDTFormat->mDateFormat, pDTFormat->mDateSep, outString );

		pEnd = ( ((DateEncoded&)mValue).encoded < 0 ) ? 
						outString + kDateStrMaxLength : 
						outString + kDateStrMaxLength - 1;
	}

	return pEnd;
}	


/**********************************************************************************************/
char* Value_date_imp::get_String( 
	char* outString, 
	tslen inBufferChars ) const
{
	// The size of input buffer (inBufferChars) cannot be -1 !
	// Because this is unsafe then to write to  the memory 
	// location pointed by outString - because we don't know the size !
	if( outString == nullptr || inBufferChars <= 0 )
		return nullptr;

	char* pEnd = nullptr;

	if( inBufferChars < tslen(kDateStrMaxLength) )
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

		Convert_date_str_fast( 
	 		(DateEncoded&)mValue, pDTFormat->mDateFormat, static_cast<char>(pDTFormat->mDateSep), outString );

		pEnd = ( ((DateEncoded&)mValue).encoded < 0 ) ? 
						outString + kDateStrMaxLength : 
						outString + kDateStrMaxLength - 1;
	}

	return pEnd;
}


#pragma mark -


/**********************************************************************************************/
void Value_date_imp::put_String( const String& inStr )
{
	if( inStr.isSingleByte() )
		put_String( inStr.getBufferA() );
	else
		put_String( inStr.c_str() );
}


/**********************************************************************************************/
void Value_date_imp::put_String( 
	const UChar* inStart,  
	const UChar* inEnd )
{
	argused1( inEnd );

	const DTFormat* pDTFormat = get_DTFormat();

	Convert_str_date_uu_aux( 
							inStart, 
							inEnd,
							pDTFormat,
							(DateEncoded&) mValue );
	
	put_IsNull(false);
}


/**********************************************************************************************/
void Value_date_imp::put_String( 
	const char* inStart,
	const char* inEnd )
{
	argused1( inEnd );
	
	const DTFormat* pDTFormat = get_DTFormat();

	Convert_str_date_aa_aux( 
							inStart, 
							pDTFormat,
							(DateEncoded&) mValue );	

	put_IsNull(false);
}


#pragma mark -
#pragma mark === Value_date ===



/**********************************************************************************************/
Value_date::~Value_date( void )
{
}


/**********************************************************************************************/
Value_date::Value_date( I_Database_Ptr inpDatabase )
:
	Value_date_imp( inpDatabase )
{
}


/**********************************************************************************************/
Value_date::Value_date( const DTFormat* inDTFormat )
:
	Value_date_imp( inDTFormat )
{
}


/**********************************************************************************************/
Value_date::Value_date( const Value_date& inOther )
:
	Value_date_imp( inOther )
{
}


/**********************************************************************************************/
Value_date::Value_date( 
	vint32			inYear,
	vuint16			inMonth,
	vuint16			inDay,
	const DTFormat*	inDTFormat )
:
	Value_date_imp( inDTFormat )
{
	put_Date( inYear, inMonth, inDay );
}


/**********************************************************************************************/
I_Value_Ptr Value_date::Clone( bool inCopyData ) const 
{
	I_Value_Ptr result = new Value_date( *this );

	if( inCopyData == false )
		result->put_IsNull( true );

	result->put_IsRemote(mIsRemote);
	return result;
}


#pragma mark -
#pragma mark === Value_date_null ===


/**********************************************************************************************/
Value_date_null::~Value_date_null( void )
{
}
							

/**********************************************************************************************/
Value_date_null::Value_date_null( I_Database_Ptr inpDatabase )
:
	Value_date( inpDatabase )
{
}


/**********************************************************************************************/
Value_date_null::Value_date_null( const DTFormat* inDTFormat )
:
	Value_date( inDTFormat )
{
}


/**********************************************************************************************/
Value_date_null::Value_date_null( 
	const Value_date&	inOther )
:
	Value_date( inOther )
{
	put_IsNull( inOther.get_IsNull() );
}


/**********************************************************************************************/
Value_date_null::Value_date_null( 
	vint32			inYear,
	vuint16			inMonth,
	vuint16			inDay,
	const DTFormat*	inDTFormat )
:
	Value_date( inYear, inMonth, inDay, inDTFormat )
{
	put_IsNull(false);
}


#pragma mark -


/**********************************************************************************************/
I_Value_Ptr Value_date_null::Clone( bool inCopyData ) const 
{
	I_Value_Ptr result = new Value_date_null( *this );

	if( inCopyData == false )
		result->put_IsNull( true );
	
	result->put_IsRemote(mIsRemote);
	return result;
}


/**********************************************************************************************/
int Value_date_null::Compare( const I_Value& inOther, COMPARE_TYPE inCompareType ) const 
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
			return Value_date::Compare( inOther );    	
    	}
    }								
}


/**********************************************************************************************/
bool Value_date_null::get_IsNull( void ) const  	
{ 
	return PN::get_IsNull(); 
}


/**********************************************************************************************/
void Value_date_null::put_IsNull( bool inValue ) 	
{ 
	PN::put_IsNull( PS::get_ValuePtr(), inValue ); 
}


/**********************************************************************************************/
String Value_date_null::get_String( tslen inLimit ) const
{
	if( get_IsNull() )
		return String::sEmpty();

	return Value_date::get_String(inLimit);
}


/**********************************************************************************************/
UChar* Value_date_null::get_String( 
	UChar* outString, 
	tslen inBufferChars ) const
{
	if( get_IsNull() )
		if( inBufferChars > 0 )
		{
			outString[0] = 0;
			return outString;
		}

	return Value_date::get_String(outString, inBufferChars);
}


/**********************************************************************************************/
char* Value_date_null::get_String( 
	char* outString, 
	tslen inBufferChars ) const
{
	if( get_IsNull() )
		if( inBufferChars > 0 )
		{
			outString[0] = 0;
			return outString;
		}

	return Value_date::get_String(outString, inBufferChars);
}


/**********************************************************************************************/
//#if FBL_TEST_CODE

/**********************************************************************************************/
void Value_date_imp::Init( void ) 
{
	// Used in tests only so make it just for our datetime parser be happy.
	put_Day(1);
	put_Month(1);
	put_Year(1);
}


/**********************************************************************************************/
void Value_date_imp::Increment( void ) 
{
	// Used in tests only so make it just for our datetime parser be happy.

	if( mValue == 0 )
	{
		Init();
		return;
	}


	vuint16 day = get_Day();
	vuint16 month = get_Month();
	vint32 year = get_Year();

	if( day < 31 )
	{
		put_Day( ++day );
	}
	else if( month < 12 )
	{	
		put_Day( 1 );
		put_Month( ++month );
	}
	else if( year < 9999 )
	{		
		put_Day(1);
		put_Month(1);
		put_Year( ++year );
	}
	else
	{
		// no nore date which makes parser happy. Actually we rarelly get here.
		Init();
	}
}

//#endif // FBL_TEST_CODE


/**********************************************************************************************/
FBL_End_Namespace


