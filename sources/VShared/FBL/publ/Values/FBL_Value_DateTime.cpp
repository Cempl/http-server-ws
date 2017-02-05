/**********************************************************************************************/
/* FBL_Value_DateTime.cpp                                                 					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Values/FBL_Value_DateTime.h>
#include <VShared/FBL/publ/Values/FBL_Value_Date.h>
#include <VShared/FBL/publ/Values/FBL_Value_Time.h>


// FBL:
#include <VShared/FBL/publ/Util_classes/FBL_ToUChar.h>
#include <VShared/FBL/publ/Util_classes/FBL_DTFormat.h>
#include <VShared/FBL/publ/Util_Strings/FBL_String_Len_Const.h>
#include <VShared/FBL/publ/Utilities/FBL_DateTimeEncode.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// Helper functions.
//
static void findLastYearSep( 
	const char* inString, 
	UChar		inDateSep, 
	vuint16&		yearDigits )
{
	const char* pds = strrchr(inString, inDateSep);
	if( pds )
	{
		const char* ps = strrchr(pds, ' ');
		yearDigits = static_cast<vuint16>(ps - pds - 1);
	}
	else
	{
		yearDigits = 0;
	}
}


#pragma mark === Convert FAST ===


/**********************************************************************************************/
// Convert functions. For HI perfomance.
//
vuint16 Convert_str_datetime_uu_fast( 
	const UChar*		inStr,
	const UChar*		inStrEnd,
	const DTFormat*		inFormat,
	DateTimeEncoded&	outDateTimeEnc )
{
	char strDateTime[kDateTimeStrMaxLength + 1];
	
	tslen len = tslen( inStrEnd - inStr );
	vu_u2a( inStr, strDateTime, len );

	return Convert_str_datetime_aa_fast( strDateTime, inFormat, outDateTimeEnc );
}


/**********************************************************************************************/
// This function expect that give string is in the CORRECT format according to current settings
// of its database.
// 
vuint16 Convert_str_datetime_aa_fast(
	const char*			inString,
	const DTFormat*		inFormat,
	DateTimeEncoded&	outDateTimeEnc )
{
	DateEncoded& dateEnc = outDateTimeEnc.decoded.date;
	TimeEncoded& timeEnc = outDateTimeEnc.decoded.time;

	if( !inString || *inString == 0 )
	{
		outDateTimeEnc.encoded = 0;
		
		return 4;
	}


	bool isNegative = false;
	char* inStr = (char*)inString;
	if( *inStr == '-' )
	{
		inStr++;
		isNegative = true;
	}

	vuint16 yearDigits = 0;
	char c;
	short unsigned int d, m; /// date
	long int y;
	short unsigned int h, mn, s, ms; /// time
	h = mn = s = ms = 0;
	switch( inFormat->mDateFormat )
	{
		case kMDY: 
		{
			sscanf( inStr, "%hu%c%hu%c%lu%c%hu%c%hu%c%hu%c%hu", 
				&m, &c, &d, &c, &y, &c, 
				&h, &c, &mn, &c, &s, &c, &ms );

			findLastYearSep(inString, inFormat->mDateSep, yearDigits);

		} break;

		case kDMY: 
		{
			sscanf( inStr, "%hu%c%hu%c%lu%c%hu%c%hu%c%hu%c%hu", 
				&d, &c, &m, &c, &y, &c, 
				&h, &c, &mn, &c, &s, &c, &ms );

			findLastYearSep(inString, inFormat->mDateSep, yearDigits);

		} break;

		case kYMD:	
		{
			sscanf( inStr, "%lu%c%hu%c%hu%c%hu%c%hu%c%hu%c%hu", 
				&y, &c, &m, &c, &d, &c, 
				&h, &c, &mn, &c, &s, &c, &ms );

			// Find first '-' symbol.
			const char* pds = strchr(inString, inFormat->mDateSep);
			yearDigits = static_cast<vuint16>(pds ? pds - inString : 0);

		} break;

		case kMYD:	
		{
			sscanf( inStr, "%hu%c%lu%c%hu%c%hu%c%hu%c%hu%c%hu", 
				&m, &c, &y, &c, &d, &c, 
				&h, &c, &mn, &c, &s, &c, &ms );

			// Find first and second '-' symbols.
			const char* pds = strchr(inString, inFormat->mDateSep);
			if( pds )
			{
				const char* pds2 = strchr(pds, inFormat->mDateSep);
				yearDigits = static_cast<vuint16>(pds2 ? pds2 - pds : 0);
			}

		} break;

		case kDYM:	
		{
			sscanf( inStr, "%hu%c%lu%c%hu%c%hu%c%hu%c%hu%c%hu", 
				&d, &c, &y, &c, &m, &c, 
				&h, &c, &mn, &c, &s, &c, &ms );

			// Find first and second '-' symbols.
			const char* pds = strchr(inString, inFormat->mDateSep);
			if( pds )
			{
				const char* pds2 = strchr(pds, inFormat->mDateSep);
				yearDigits = static_cast<vuint16>(pds2 ? pds2 - pds : 0);
			}

		} break;
		
		case kYDM:	
		{
			sscanf( inStr, "%lu%c%hu%c%hu%c%hu%c%hu%c%hu%c%hu", 
				&y, &c, &d, &c, &m, &c, 
				&h, &c, &mn, &c, &s, &c, &ms );

			// Find first '-' symbol.
			const char* pds = strchr(inString, inFormat->mDateSep);
			yearDigits = static_cast<vuint16>(pds ? pds - inString : 0);

		} break;

		default:
		{
			FBL_CHECK( false );
			d = m = 0;
			y = 0;
			h = mn = s = ms = 0;
			yearDigits = 0;
		}
		break;
	}

	if( isNegative )
		y *= -1;

	dateEnc.decoded.d = d;
	dateEnc.decoded.m = m;
	dateEnc.decoded.y = y;

	timeEnc.decoded.h = h;
	timeEnc.decoded.m = mn;
	timeEnc.decoded.s = s;
	timeEnc.decoded.ms = ms;

	return yearDigits;
}


#pragma mark -


/**********************************************************************************************/
tslen Convert_datetime_str_fast( 
	const DateTimeEncoded&	inDateTimeEnc, 
	EDateFormat				inDateFormat, 
	const UChar				inDateSep, 
	const UChar				inTimeSep, 
	UChar*					outString )
{
	// This is not safe to use such a buffer lenght.
	// Because sprintf() may exceed it.
	//char str_dt[kDateTimeStrMaxLength + 1];
	char strDateTime[128];

	tslen len = Convert_datetime_str_fast(inDateTimeEnc, 
							  inDateFormat, 
							  static_cast<char>(inDateSep), 
							  static_cast<char>(inTimeSep), 
							  strDateTime);
	vu_a2u(strDateTime, outString, len);
	
	return len;
}


/**********************************************************************************************/ 
tslen Convert_datetime_str_fast( 
	const DateTimeEncoded&	inDateTimeEnc, 
	EDateFormat				inDateFormat, 
	const char				inDateSep, 
	const char				inTimeSep, 
	char*					outString )
{
	const DateEncoded& dateEnc = inDateTimeEnc.decoded.date;
	const TimeEncoded& timeEnc = inDateTimeEnc.decoded.time;

	vuint32 Written = 0;
	
	vint32 year = dateEnc.decoded.y % 10000;
	unsigned int ms = (unsigned int) timeEnc.decoded.ms % 1000;

	switch( inDateFormat )
	{
		case kMDY: 
		{
			Written = sprintf( outString, "%.2u%c%.2u%c%.4ld %.2u%c%.2u%c%.2u%c%.3u", 
			dateEnc.decoded.m, inDateSep, dateEnc.decoded.d, inDateSep, (long)year, 
			timeEnc.decoded.h, inTimeSep, timeEnc.decoded.m, inTimeSep, timeEnc.decoded.s, inTimeSep, ms );
		
		} break;

		case kDMY:
		{
			Written = sprintf( outString, "%.2u%c%.2u%c%.4ld %.2u%c%.2u%c%.2u%c%.3u", 
			dateEnc.decoded.d, inDateSep, dateEnc.decoded.m, inDateSep, (long)year, 
			timeEnc.decoded.h, inTimeSep, timeEnc.decoded.m, inTimeSep, timeEnc.decoded.s, inTimeSep, ms );

		} break;

		case kYMD:
		{
			Written = sprintf( outString, "%.4ld%c%.2u%c%.2u %.2u%c%.2u%c%.2u%c%.3u", 
			(long)year, inDateSep, dateEnc.decoded.m, inDateSep, dateEnc.decoded.d, 
			timeEnc.decoded.h, inTimeSep, timeEnc.decoded.m, inTimeSep, timeEnc.decoded.s, inTimeSep, ms );

		} break;

		case kMYD:
		{
			Written = sprintf( outString, "%.2u%c%.4ld%c%.2u %.2u%c%.2u%c%.2u%c%.3u", 
			dateEnc.decoded.m, inDateSep, (long)year, inDateSep, dateEnc.decoded.d, 
			timeEnc.decoded.h, inTimeSep, timeEnc.decoded.m, inTimeSep, timeEnc.decoded.s, inTimeSep, ms );

		} break;

		case kDYM:
		{
			Written = sprintf( outString, "%.2u%c%.4ld%c%.2u %.2u%c%.2u%c%.2u%c%.3u", 
			dateEnc.decoded.d, inDateSep, (long)year, inDateSep, dateEnc.decoded.m, 
			timeEnc.decoded.h, inTimeSep, timeEnc.decoded.m, inTimeSep, timeEnc.decoded.s, inTimeSep, ms );

		} break;

		case kYDM:
		{
			Written = sprintf( outString, "%.4ld%c%.2u%c%.2u %.2u%c%.2u%c%.2u%c%.3u", 
			(long)year, inDateSep, dateEnc.decoded.d, inDateSep, dateEnc.decoded.m, 
			timeEnc.decoded.h, inTimeSep, timeEnc.decoded.m, inTimeSep, timeEnc.decoded.s, inTimeSep, ms );

		} break;

		default:
			FBL_CHECK( false );  
	}

	tslen ToMap = static_cast<tslen>(Written > kDateTimeStrMaxLength ? kDateTimeStrMaxLength : Written);
	outString[ToMap] = 0;

	return ToMap;
}


#pragma mark -
#pragma mark === Convert from UChar*  ===


/**********************************************************************************************/
void Convert_str_datetime_aa_aux( 
	const char*			inStr,
	const DTFormat* 	inDTFormat,
	DateTimeEncoded&	ioDateTime )
{
	if( !inStr || *inStr == 0 )
	{
		ioDateTime.encoded = 0;
		return;
	}
	
	// Check string format:
	vint32 BadCharIndex = CheckDateTimeStringFormat( inStr, inDTFormat );
	if( BadCharIndex )
	{
		throw xValueError( 
					ERR_DATETIME_VALUE_NOT_MATCH_FORMAT, 
					String(inStr).c_str(), ToUCharPtr(BadCharIndex).c_str() );
	}

	// ----------------------------
	// Now we know that string STRICTLY correspond to expected format,
	// so we can do fast extracting of numbers using scanf()
	vuint16 yearDigits = Convert_str_datetime_aa_fast( inStr, inDTFormat, ioDateTime );

	// ----------------------------
	// Final step - we check that extracted numbers fit limits: 
	if( CheckDateValueLimits( ioDateTime.decoded.date ) == false || 
		CheckTimeValueLimits( ioDateTime.decoded.time ) == false )
	{
		throw xValueError( ERR_DATETIME_VALUE_HAVE_WRONG_NUMBERS, inStr );
	}
	
	if( yearDigits < 4 )
	{
		vint32 year = ioDateTime.decoded.date.decoded.y;
		inDTFormat->AutoCentury( year );
		ioDateTime.decoded.date.decoded.y = year;
	}
}

/**********************************************************************************************/
void Convert_str_datetime_uu_aux( 
	const UChar*		inStr,
	const UChar*		inStrEnd,
	const DTFormat* 	inDTFormat,
	DateTimeEncoded&	ioDateTime )
{
	if( !inStr )
		return;

	char strDateTime[kDateTimeStrMaxLength + 1];
	
	tslen len = inStrEnd ? tslen(inStrEnd - inStr) : vu_strlen(inStr);
	vu_u2a( inStr, strDateTime, len );
	
	return Convert_str_datetime_aa_aux( 
					strDateTime, 
					inDTFormat,
					ioDateTime	);
}


#pragma mark -
#pragma mark === Date, Time <=> DateTime convert functions.

/**********************************************************************************************/
void Convert_date_time( const I_Value* inValue, I_Value* outValue );
void Convert_date_time( const I_Value* inValue, I_Value* outValue )
{
	argused1( inValue );
	outValue->put_Long(0);
}


/**********************************************************************************************/
void Convert_time_date( const I_Value* inValue, I_Value* outValue );
void Convert_time_date( const I_Value* inValue, I_Value* outValue )
{
	argused1( inValue );
	outValue->put_Long(0);
}


/**********************************************************************************************/
void Convert_datetime_date(
	const I_Value*	inValue, 
	I_Value*		outValue );
void Convert_datetime_date(
	const I_Value*	inValue, 
	I_Value*		outValue )
{
	const I_ValueDateTime*	pValueDateTime	= dcast< const I_ValueDateTime* >( inValue );
	I_ValueDate_Ptr			pValueDate		= fbl_dynamic_cast<I_ValueDate>( outValue );

	pValueDate->put_Date(
					pValueDateTime->get_Year(),
					pValueDateTime->get_Month(),
					pValueDateTime->get_Day() );
}


/**********************************************************************************************/
void Convert_datetime_time(
	const I_Value*	inValue, 
	I_Value*		outValue );
void Convert_datetime_time(
	const I_Value*	inValue, 
	I_Value*		outValue )
{
	const I_ValueDateTime*	pValueDateTime	= dcast< const I_ValueDateTime* >( inValue );
	I_ValueTime_Ptr			pValueTime		= fbl_dynamic_cast<I_ValueTime>( outValue );

	pValueTime->put_Time(
					pValueDateTime->get_Hours(),
					pValueDateTime->get_Minutes(),
					pValueDateTime->get_Seconds(),
					pValueDateTime->get_Milliseconds() );
}


/**********************************************************************************************/
void Convert_date_datetime(
	const I_Value*	inValue, 
	I_Value*		outValue );
void Convert_date_datetime(
	const I_Value*	inValue, 
	I_Value*		outValue )
{
	const I_ValueDate*		pValueDate			= dcast< const I_ValueDate* >( inValue );
	I_ValueDateTime_Ptr		pValueDateTime		= fbl_dynamic_cast<I_ValueDateTime>( outValue );

	pValueDateTime->put_Date(
					pValueDate->get_Year(),
					pValueDate->get_Month(),
					pValueDate->get_Day() );
}


/**********************************************************************************************/
void Convert_time_datetime(
	const I_Value*	inValue, 
	I_Value*		outValue );
void Convert_time_datetime(
	const I_Value*	inValue, 
	I_Value*		outValue )
{
	const I_ValueTime*		pValueTime			= dcast< const I_ValueTime* >( inValue );
	I_ValueDateTime_Ptr		pValueDateTime		= fbl_dynamic_cast<I_ValueDateTime>( outValue );

	pValueDateTime->put_Time(
					pValueTime->get_Hours(),
					pValueTime->get_Minutes(),
					pValueTime->get_Seconds(),
					pValueTime->get_Milliseconds() );
}


#pragma mark -
#pragma mark === Convert to/from string value ===

/**********************************************************************************************/
// WRAPPER: I_Value => UChar*
//
void Convert_str_datetime( 
	const I_Value*	inValue, 
	I_Value*		outValue )
{
	I_ValueDateTime* pValueDateTime = dcast< I_ValueDateTime* >( outValue );
	const DTFormat* pDTFormat = pValueDateTime->get_DTFormat();

	Convert_str_datetime_uu_aux( 
		(UChar*) inValue->begin(),
		(UChar*) inValue->end(),
		pDTFormat,
		*(DateTimeEncoded*) outValue->begin() );	
}


/**********************************************************************************************/
void Convert_datetime_str( const I_Value*	inValue, I_Value* outValue );
void Convert_datetime_str( const I_Value*	inValue, I_Value* outValue )
{
	const I_ValueDateTime* pValueDateTime = dcast< const I_ValueDateTime* >( inValue );
	const DTFormat* pDTFormat = pValueDateTime->get_DTFormat();

	char strDateTime[128];
	tslen len = Convert_datetime_str_fast(
								*(DateTimeEncoded*)inValue->begin(), 
								pDTFormat->mDateFormat, 
								static_cast<char>(pDTFormat->mDateSep), 
								static_cast<char>(pDTFormat->mTimeSep), 
								strDateTime);

	vuint32 outValueLen = outValue->get_MaxChars();
	len = ( (tslen)outValueLen < len ) ? outValueLen : len;
	vu_a2u(strDateTime, (UChar*) outValue->begin(), len);		
	outValue->put_ByteLength( len * sizeof(UChar) );				
}


/**********************************************************************************************/
// WRAPPER: I_Value => char*
//
void Convert_bin_datetime( 
	const I_Value*	inValue, 
	I_Value*		outValue );
void Convert_bin_datetime( 
	const I_Value*	inValue, 
	I_Value*		outValue )
{
	I_ValueDateTime* pValueDateTime = dcast< I_ValueDateTime* >( outValue );
	const DTFormat* pDTFormat = pValueDateTime->get_DTFormat();

	// TODO:
	// Currently we assume inValue is a single-byted string.
	// After we make everything localizable we can improve this using IOConverter as we do
	// for string values.
	//

	const char* pBinStart = inValue->begin();
	const char* pBinEnd = inValue->end();
	
	if( pBinStart )
	{
		char strDateTime[kDateTimeStrMaxLength + 1];
		
		tslen len = tslen(pBinEnd - pBinStart);
		memcpy( strDateTime, pBinStart, Min(len, (tslen)kDateTimeStrMaxLength) );

		Convert_str_datetime_aa_aux( 
			strDateTime,
			pDTFormat,
			*(DateTimeEncoded*) outValue->begin() );	
	}
}


#pragma mark -
#pragma mark === Value_datetime_imp ===


/**********************************************************************************************/
Value_datetime_imp::~Value_datetime_imp( void )
{
	if( mDTFormatOwner )
		ForgetObject(mDTFormat);
}


/**********************************************************************************************/
Value_datetime_imp::Value_datetime_imp( I_Database_Ptr inpDatabase )
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
Value_datetime_imp::Value_datetime_imp( const DTFormat* inDTFormat )
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
Value_datetime_imp::Value_datetime_imp( const Value_datetime_imp& inOther )
:
	Value_Numeric< kTypeDateTime, vcDateTime, policyInPlace_llong >( inOther )
{
	mpDatabase		= inOther.mpDatabase;
	mDTFormat		= inOther.mDTFormat;
	mDTFormatOwner	= false;	// Because of either inOther.mDTFormat or mpDatabase
}


#pragma mark -


/**********************************************************************************************/
const DTFormat* Value_datetime_imp::get_DTFormat( void ) const 
{
	FBL_CHECK( mpDatabase || mDTFormat );
	return (mpDatabase) ? mpDatabase->get_DTFormat() : mDTFormat;
}


/**********************************************************************************************/
vint32 Value_datetime_imp::get_Year( void ) const
{ 
	vint32 result = ((DateTimeEncoded&)mValue).decoded.date.decoded.y;
	return result;
}


/**********************************************************************************************/
void Value_datetime_imp::put_Year( vint32 inYear )
{
	const DTFormat* pDTFormat = get_DTFormat();

	// Year correction
	pDTFormat->AutoCentury( inYear );
	((DateTimeEncoded&)mValue).decoded.date.decoded.y = inYear; 
	put_IsNull( false );			
}


/**********************************************************************************************/
vuint16 Value_datetime_imp::get_Month( void ) const
{ 
	vuint16 result = ((DateTimeEncoded&)mValue).decoded.date.decoded.m; 
	return result;
}


/**********************************************************************************************/
void Value_datetime_imp::put_Month( vuint16 inMonth )
{ 
	((DateTimeEncoded&)mValue).decoded.date.decoded.m = inMonth; 
	put_IsNull( false );			
}


/**********************************************************************************************/
vuint16 Value_datetime_imp::get_Day( void ) const
{
	vuint16 result = ((DateTimeEncoded&)mValue).decoded.date.decoded.d; 
	return result;
}


/**********************************************************************************************/
void Value_datetime_imp::put_Day( vuint16 inDay )
{ 
	((DateTimeEncoded&)mValue).decoded.date.decoded.d = inDay; 
	put_IsNull( false );			
}


/**********************************************************************************************/
void Value_datetime_imp::get_Date( 
	vint32&	outYear, 
	vuint16& outMonth, 
	vuint16& outDay ) const
{ 
	DateEncoded& de = ((DateTimeEncoded&)mValue).decoded.date;

	outYear	 = de.decoded.y;
	outMonth = de.decoded.m;
	outDay	 = de.decoded.d;
}


/**********************************************************************************************/
void Value_datetime_imp::put_Date( 
	vint32	inYear, 
	vuint16	inMonth, 
	vuint16	inDay )
{ 
	DateEncoded& de = ((DateTimeEncoded&)mValue).decoded.date;

	const DTFormat* pDTFormat = get_DTFormat();

	// Year correction
	pDTFormat->AutoCentury( inYear );

	de.decoded.y = inYear;
	de.decoded.m = inMonth;
	de.decoded.d = inDay;
	put_IsNull( false );			
}


/**********************************************************************************************/
vuint16 Value_datetime_imp::get_Hours( void ) const
{ 
	vuint16 result = ((DateTimeEncoded&)mValue).decoded.time.decoded.h; 
	return result;
}


/**********************************************************************************************/
void Value_datetime_imp::put_Hours( vuint16 inHours )
{ 
	((DateTimeEncoded&)mValue).decoded.time.decoded.h = inHours; 
	put_IsNull( false );			
}


/**********************************************************************************************/
vuint16 Value_datetime_imp::get_Minutes( void ) const
{ 
	vuint16 result = ((DateTimeEncoded&)mValue).decoded.time.decoded.m; 
	return result;
}


/**********************************************************************************************/
void Value_datetime_imp::put_Minutes( vuint16 inMinutes )
{ 
	((DateTimeEncoded&)mValue).decoded.time.decoded.m = inMinutes; 
	put_IsNull( false );			
}


/**********************************************************************************************/
vuint16 Value_datetime_imp::get_Seconds( void ) const
{ 
	vuint32 res = (vuint32) ((DateTimeEncoded&)mValue).decoded.time.decoded.s;
	return static_cast<vuint16>(res);
}


/**********************************************************************************************/
void Value_datetime_imp::put_Seconds( vuint16 inSeconds )
{ 
	DateTimeEncoded& dte = ((DateTimeEncoded&)mValue);
	dte.decoded.time.decoded.s = inSeconds; 
	put_IsNull( false );			
}


/**********************************************************************************************/
vuint16 Value_datetime_imp::get_Milliseconds( void ) const
{ 
	vuint16 result = ((DateTimeEncoded&)mValue).decoded.time.decoded.ms; 
	return result;
}


/**********************************************************************************************/
void Value_datetime_imp::put_Milliseconds( vuint16 inMilliseconds )
{ 
	((DateTimeEncoded&)mValue).decoded.time.decoded.ms = inMilliseconds; 
	put_IsNull( false );			
}


/**********************************************************************************************/
void Value_datetime_imp::get_Time( 
	vuint16& outHours, 
	vuint16& outMinutes, 
	vuint16& outSeconds,
	vuint16& outMilliseconds ) const
{ 
	TimeEncoded& te = ((DateTimeEncoded&)mValue).decoded.time;

	outHours		= (vuint16) te.decoded.h;
	outMinutes		= (vuint16) te.decoded.m;
	
	vuint32 secs 		= (vuint32) te.decoded.s;	// CW WIN 8.x bug
	outSeconds		= (vuint16) secs;
	
	outMilliseconds = (vuint16) te.decoded.ms;
}


/**********************************************************************************************/
void Value_datetime_imp::put_Time( 
	vuint16 	inHours, 
	vuint16 	inMinutes,
	vuint16 	inSeconds, 
	vuint16	inMilliseconds )
{ 
	TimeEncoded& te = ((DateTimeEncoded&)mValue).decoded.time;

	te.decoded.h  = inHours;
	te.decoded.m  = inMinutes;
	te.decoded.s  = inSeconds;
	te.decoded.ms = inMilliseconds; 
	put_IsNull( false );			
}


/**********************************************************************************************/
void Value_datetime_imp::get_DateTime( 
	vint32&	outYear, 
	vuint16& outMonth, 
	vuint16& outDay,
	vuint16& outHours, 
	vuint16& outMinutes, 
	vuint16& outSeconds,
	vuint16& outMilliseconds ) const
{
	TimeEncoded& te = ((DateTimeEncoded&)mValue).decoded.time;

	outHours		= (vuint16) te.decoded.h;
	outMinutes		= (vuint16) te.decoded.m;
	
	vuint32 secs 		= (vuint32) te.decoded.s;	// CW WIN 8.x bug
	outSeconds		= (vuint16) secs;
	
	outMilliseconds = (vuint16) te.decoded.ms;

	DateEncoded& de = ((DateTimeEncoded&)mValue).decoded.date;

	outYear	 = (vint32)   de.decoded.y;
	outMonth = (vuint16) de.decoded.m;
	outDay	 = (vuint16) de.decoded.d;
}


/**********************************************************************************************/
void Value_datetime_imp::put_DateTime( 
	vint32	inYear, 
	vuint16 	inMonth, 
	vuint16 	inDay,
	vuint16 	inHours, 
	vuint16 	inMinutes,
	vuint16 	inSeconds, 
	vuint16	inMilliseconds )
{
	TimeEncoded& te = ((DateTimeEncoded&)mValue).decoded.time;

	te.decoded.h  = inHours;
	te.decoded.m  = inMinutes;
	te.decoded.s  = inSeconds;
	te.decoded.ms = inMilliseconds; 

	DateEncoded& de = ((DateTimeEncoded&)mValue).decoded.date;

	de.decoded.y = inYear;
	de.decoded.m = inMonth;
	de.decoded.d = inDay;
	put_IsNull( false );			
}


/**********************************************************************************************/
vuint32 Value_datetime_imp::get_MaxChars( void ) const  
{ 
	return ( ((DateTimeEncoded&)mValue).encoded < 0 ) ? 
								kDateTimeStrMaxLength : 
								kDateTimeStrMaxLength - 1;
}


#pragma mark -


/**********************************************************************************************/
String Value_datetime_imp::get_String( tslen inLimit ) const
{
	String str;

	if( inLimit != 0 )
	{
		UChar* p = str.getBuffer(kDateTimeStrMaxLength + 1);

		const DTFormat* pDTFormat = get_DTFormat();

		Convert_datetime_str_fast( 
			(DateTimeEncoded&)mValue, 
			pDTFormat->mDateFormat,
			pDTFormat->mDateSep,
			pDTFormat->mTimeSep,
			p );

		if( ((DateTimeEncoded&)mValue).encoded < 0 )
			str.releaseBuffer(kDateTimeStrMaxLength);		// 24 symbols for negative dates
		else
			str.releaseBuffer(kDateTimeStrMaxLength - 1);	// 23 symbols for positive dates

		// not -1 and less than maxLen
		if( inLimit > 0 && vuint32(inLimit) < kDateTimeStrMaxLength )
		{
			str.truncate( inLimit );
		}
	}

	return str;
}


/**********************************************************************************************/
UChar* Value_datetime_imp::get_String( 
	UChar*	outString, 
	tslen	inBufferChars ) const
{
	// The size of input buffer (inBufferChars) cannot be -1 !
	// Because this is unsafe then to write to  the memory 
	// location pointed by outString - because we don't know the size !
	if( outString == nullptr || inBufferChars <= 0 )
		return nullptr;

	UChar* pEnd = nullptr;

	if( inBufferChars < tslen(kDateTimeStrMaxLength) )
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
	
		Convert_datetime_str_fast( 
			(DateTimeEncoded&)mValue, 
			pDTFormat->mDateFormat,
			pDTFormat->mDateSep,
			pDTFormat->mTimeSep,
			outString );

		pEnd = ( ((DateTimeEncoded&)mValue).encoded < 0 ) ? 
						outString + kDateTimeStrMaxLength : 
						outString + kDateTimeStrMaxLength - 1;
	}

	return pEnd;
}	


/**********************************************************************************************/
char* Value_datetime_imp::get_String( 
	char* outString, 
	tslen inBufferChars ) const
{
	// The size of input buffer (inBufferChars) cannot be -1 !
	// Because this is unsafe then to write to  the memory 
	// location pointed by outString - because we don't know the size !
	if( outString == nullptr || inBufferChars <= 0 )
		return nullptr;

	char* pEnd = nullptr;

	if( inBufferChars < tslen(kDateTimeStrMaxLength) )
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
	
		Convert_datetime_str_fast( 
			(DateTimeEncoded&)mValue, 
			pDTFormat->mDateFormat,
			static_cast<char>(pDTFormat->mDateSep),
			static_cast<char>(pDTFormat->mTimeSep),
			outString );

		pEnd = ( ((DateTimeEncoded&)mValue).encoded < 0 ) ? 
						outString + kDateTimeStrMaxLength : 
						outString + kDateTimeStrMaxLength - 1;
	}

	return pEnd;
}

#pragma mark -


/**********************************************************************************************/
void Value_datetime_imp::put_String( const String& inStr )
{
	if( inStr.isSingleByte() )
		put_String( inStr.getBufferA() );
	else
		put_String( inStr.c_str() );
}


/**********************************************************************************************/
void Value_datetime_imp::put_String( 
	const UChar* inStart,  
	const UChar* inEnd )
{
	argused1( inEnd );
	
	const DTFormat* pDTFormat = get_DTFormat();
	
	Convert_str_datetime_uu_aux( 
							inStart, 
							inEnd,
							pDTFormat,
							(DateTimeEncoded&) mValue );

	put_IsNull( false );
}


/**********************************************************************************************/
void Value_datetime_imp::put_String( 
	const char* inStart, 
	const char* inEnd )
{
	argused1( inEnd );

	const DTFormat* pDTFormat = get_DTFormat();

	Convert_str_datetime_aa_aux( 
								inStart, 
								pDTFormat,
								(DateTimeEncoded&)mValue );

	put_IsNull(false);
}


#pragma mark -
#pragma mark === Value_datetime ===


/**********************************************************************************************/
Value_datetime::~Value_datetime( void )
{
}


/**********************************************************************************************/
Value_datetime::Value_datetime( I_Database_Ptr inpDatabase )
:
	Value_datetime_imp( inpDatabase )
{
}


/**********************************************************************************************/
Value_datetime::Value_datetime( const DTFormat* inDTFormat )
:
	Value_datetime_imp( inDTFormat )
{
}


/**********************************************************************************************/
Value_datetime::Value_datetime( const Value_datetime& inOther )
:
	Value_datetime_imp( inOther )
{
}


/**********************************************************************************************/
Value_datetime::Value_datetime( 
	vint32			inYear,
	vuint16			inMonth,
	vuint16			inDay,
	vuint16			inHours,
	vuint16			inMinutes,
	vuint16			inSeconds,
	vuint16			inMilliseconds,
	const DTFormat*	inDTFormat )
:
	Value_datetime_imp( inDTFormat )
{
	put_DateTime(
    	inYear, 
		inMonth, 
		inDay,
		inHours, 
		inMinutes,
		inSeconds, 
		inMilliseconds );
}


/**********************************************************************************************/
I_Value_Ptr Value_datetime::Clone( bool inCopyData ) const 
{
	I_Value_Ptr result = new Value_datetime( *this );

	if( inCopyData == false )
		result->put_IsNull( true );

	result->put_IsRemote(mIsRemote);
	return result;
}


#pragma mark -
#pragma mark === Value_datetime_null ===


/**********************************************************************************************/
Value_datetime_null::~Value_datetime_null( void )
{
}


/**********************************************************************************************/
Value_datetime_null::Value_datetime_null( I_Database_Ptr inpDatabase )
:
	Value_datetime( inpDatabase )
{
}


/**********************************************************************************************/
Value_datetime_null::Value_datetime_null( const DTFormat* inDTFormat )
:
	Value_datetime( inDTFormat )
{
}


/**********************************************************************************************/
Value_datetime_null::Value_datetime_null( const Value_datetime_null& inOther )
:
	Value_datetime( inOther )
{
	put_IsNull( inOther.get_IsNull() );
}


/**********************************************************************************************/
Value_datetime_null::Value_datetime_null( 
	vint32			inYear,
	vuint16			inMonth,
	vuint16			inDay,
	vuint16			inHours,
	vuint16			inMinutes,
	vuint16			inSeconds,
	vuint16			inMilliseconds,
	const DTFormat*	inDTFormat )
:
	Value_datetime( 
		inYear, 
		inMonth, 
		inDay,
		inHours, 
		inMinutes,
		inSeconds, 
		inMilliseconds,
		inDTFormat )
{
	put_IsNull(false);
}


#pragma mark -


/**********************************************************************************************/
I_Value_Ptr Value_datetime_null::Clone( bool inCopyData ) const 
{
	I_Value_Ptr result = new Value_datetime_null( *this );

	if( inCopyData == false )
		result->put_IsNull( true );

	result->put_IsRemote(mIsRemote);
	return result;
}


/**********************************************************************************************/
int Value_datetime_null::Compare( 
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
			return Value_datetime::Compare( inOther );    	
    	}
    }								
}


/**********************************************************************************************/
bool Value_datetime_null::get_IsNull( void ) const  	
{ 
	return PN::get_IsNull(); 
}


/**********************************************************************************************/
void Value_datetime_null::put_IsNull( bool inValue ) 	
{ 
	PN::put_IsNull( PS::get_ValuePtr(), inValue ); 
}


/**********************************************************************************************/
String Value_datetime_null::get_String( tslen inLimit ) const
{
	if( get_IsNull() )
		return String::sEmpty();

	return Value_datetime::get_String(inLimit);
}


/**********************************************************************************************/
UChar* Value_datetime_null::get_String( 
	UChar* outString, 
	tslen inBufferChars ) const
{
	if( get_IsNull() )
		if( inBufferChars > 0 )
		{
			outString[0] = 0;
			return outString;
		}

	return Value_datetime::get_String(outString, inBufferChars);
}


/**********************************************************************************************/
char* Value_datetime_null::get_String( 
	char* outString, 
	tslen inBufferChars ) const
{
	if( get_IsNull() )
		if( inBufferChars > 0 )
		{
			outString[0] = 0;
			return outString;
		}

	return Value_datetime::get_String(outString, inBufferChars);
}


/**********************************************************************************************/
//#if FBL_TEST_CODE

/**********************************************************************************************/
void Value_datetime_imp::Init( void ) 
{
	// Used in tests only so make it just for our datetime parser be happy.
	put_Day(1);
	put_Month(1);
	put_Year(1);
	put_Hours(0);
	put_Minutes(0);
	put_Seconds(0);
	put_Milliseconds(1);
}


/**********************************************************************************************/
void Value_datetime_imp::Increment( void ) 
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
		put_Milliseconds(1);
		put_Seconds( ++seconds );
	}
	else if( minutes < 59 )
	{
		put_Milliseconds(1);
		put_Seconds(0);
		put_Minutes( ++minutes );
	}
	else if( hours < 24 )
	{
		put_Milliseconds(1);
		put_Seconds(0);
		put_Minutes(0);
		put_Hours( ++hours );
	}
	else
	{
		// no nore time which makes parser happy. Actually we rarelly get here.

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
}

//#endif // FBL_TEST_CODE


/**********************************************************************************************/

/**********************************************************************************************/
FBL_End_Namespace


