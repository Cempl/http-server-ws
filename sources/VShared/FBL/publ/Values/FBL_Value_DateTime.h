/**********************************************************************************************/
/* FBL_Value_DateTime.h 	                                               					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_Value_DateTime_h
	#define _FBL_Value_DateTime_h
#pragma once

#include <VShared/FBL/publ/Interfaces/FBL_I_ValueDateTime.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Database.h>

// FBL:
#include <VShared/FBL/publ/Values/FBL_Value_Numeric.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_CLASS(Value_datetime);
SMART_CLASS(Value_datetime_null);


/**********************************************************************************************/
union DateTimeEncoded;


/**********************************************************************************************/
// FAST STR => DATETIME
//
FBL_SHARED_EXP vuint16 Convert_str_datetime_aa_fast(
	const char*			inString,
	const DTFormat*		inFormat,
	DateTimeEncoded&	outDateTimeEnc );

FBL_SHARED_EXP vuint16 Convert_str_datetime_uu_fast( 
	const UChar*		inString,
	const UChar*		inStrEnd,
	const DTFormat*		inFormat,
	DateTimeEncoded&	outDateTimeEnc );


/**********************************************************************************************/
// FAST DATETIME => STR
//
FBL_SHARED_EXP tslen Convert_datetime_str_fast( 
	const DateTimeEncoded&	inDateTimeEnc, 
	EDateFormat				inDateFormat, 
	const char				inDateSep, 
	const char				inTimeSep, 
	char*					outString );

FBL_SHARED_EXP tslen Convert_datetime_str_fast( 
	const DateTimeEncoded&	inDateTimeEnc,
	EDateFormat				inDateFormat,
	const UChar				inDateSep,
	const UChar				inTimeSep,
	UChar*					outString );


/**********************************************************************************************/
// CONVERT with CHECKS
//
FBL_SHARED_EXP void Convert_str_datetime_aa_aux( 
	const char*			inStr,
	const DTFormat* 	inDTFormat,
	DateTimeEncoded&	ioDateTime );

FBL_SHARED_EXP void Convert_str_datetime_uu_aux( 
	const UChar*		inStr,
	const UChar*		InStrEnd,
	const DTFormat* 	inDTFormat,
	DateTimeEncoded&	ioDateTime );


/**********************************************************************************************/
// VALUE STRING <=> VALUE DATETIME
//
FBL_SHARED_EXP void Convert_str_datetime( 
	const I_Value*	inValue, 
	I_Value*		outValue );


FBL_SHARED_EXP void Convert_str_datetime( 
	const I_Value*	inValue, 
	I_Value*		outValue );


#pragma mark === Value_datetime_imp ===


/**********************************************************************************************/
// This class implements I_ValueDateTime interface
// 
class FBL_SHARED_EXP_CLASS Value_datetime_imp : 
	public I_ValueDateTime, 
	public Value_Numeric< kTypeDateTime, vcDateTime, policyInPlace_llong >
{
		IMPLEMENT_UNKNOWN(Value_datetime_imp)											
		BEGIN_INTERFACE_TABLE(Value_datetime_imp)										
			IMPLEMENT_INTERFACE(I_Value)										
			IMPLEMENT_INTERFACE(I_ValueDateTime)										
			IMPLEMENT_INTERFACE(I_Serializable)										
		END_INTERFACE_TABLE()

	public://///////////////////////////////////////////////////////////////////////////////////

							Value_datetime_imp( I_Database_Ptr inpDatabase = nullptr );
							Value_datetime_imp( const DTFormat* inDTFormat );
							Value_datetime_imp( const Value_datetime_imp& inOther );

virtual						~Value_datetime_imp( void );


	public://///////////////////////////////////////////////////////////////////////////////////

	// ---------------------
	// DateTimeFormat:

virtual	const DTFormat*		get_DTFormat( void ) const override;

	// ---------------------
	// Date properties:
	
virtual	vint32				get_Year( void ) const override;
virtual	void				put_Year( vint32 inYear ) override;

virtual	vuint16				get_Month( void ) const override;
virtual	void				put_Month( vuint16 inMonth ) override;

virtual	vuint16				get_Day( void ) const override;
virtual	void				put_Day( vuint16 inDay ) override;

virtual	void				get_Date( 
								vint32&	outYear, 
								vuint16& outMonth, 
								vuint16& outDay ) const override;

virtual	void				put_Date( 
								vint32	inYear, 
								vuint16	inMonth, 
								vuint16	inDay ) override;

	// ---------------------
	// Time properties:

virtual	vuint16				get_Hours( void ) const override;
virtual	void				put_Hours( vuint16 inHours ) override;

virtual	vuint16				get_Minutes( void ) const override;
virtual	void				put_Minutes( vuint16 inMinutes ) override;

virtual	vuint16				get_Seconds( void ) const override;
virtual	void				put_Seconds( vuint16 inSeconds ) override;

virtual	vuint16				get_Milliseconds( void ) const override;
virtual	void				put_Milliseconds( vuint16 inMilliseconds ) override;

virtual	void				get_Time( 
								vuint16& outHours, 
								vuint16& outMinutes, 
								vuint16& outSeconds,
								vuint16& outMilliseconds ) const override;

virtual	void				put_Time( 
								vuint16 	inHours, 
								vuint16 	inMinutes,
								vuint16 	inSeconds, 
								vuint16	inMilliseconds ) override;

	// ---------------------
	// DateTime properties:
	
virtual	void				get_DateTime( 
								vint32&	outYear, 
								vuint16& outMonth, 
								vuint16& outDay,
								vuint16& outHours, 
								vuint16& outMinutes, 
								vuint16& outSeconds,
								vuint16& outMilliseconds ) const override;
			
virtual	void				put_DateTime( 
								vint32	inYear, 
								vuint16 	inMonth, 
								vuint16 	inDay,
								vuint16 	inHours, 
								vuint16 	inMinutes,
								vuint16 	inSeconds, 
								vuint16	inMilliseconds ) override;
							

	public://///////////////////////////////////////////////////////////////////////////////////

// I_Value API:

		using Value_Numeric< kTypeDateTime, vcDateTime, policyInPlace_llong >::Assign;


	// ---------------------
	// To/From string

virtual String				get_String( tslen inLimit = -1 ) const override;
virtual void				put_String( const String& inStr ) override;

virtual UChar*				get_String( 
								UChar*			outString, 
								tslen			inBufferChars ) const override;

virtual void				put_String( 
								const UChar* 	inStart,  
								const UChar* 	inEnd = nullptr ) override;

virtual char*				get_String( 
								char*			outString, 
								tslen			inBufferChars ) const override;

virtual void				put_String( 
								const char* 	inStart,  
								const char* 	inEnd = nullptr ) override;

virtual vuint32				get_MaxChars( void ) const override;

virtual void				put_Boolean( bool inValue ) override 	{ argused1( inValue ); }
virtual bool				get_Boolean( void ) const override 		{ return false; }

virtual void				put_Byte( vuint8 inValue ) override		{ argused1( inValue ); }
virtual vuint8				get_Byte( void ) const override			{ return ( vuint8 )( 0 ); }

virtual void				put_Short( vint16 inValue ) override		{ argused1( inValue ); }
virtual vint16				get_Short( void ) const override			{ return ( vint16 )0; }

virtual void				put_UShort( vuint16 inValue ) override	{ argused1( inValue ); }
virtual vuint16				get_UShort( void ) const override		{ return ( vuint16 )( 0 ); }

virtual void				put_Medium( vint32 inValue ) override		{ argused1( inValue ); }
virtual vint32				get_Medium( void ) const override		{ return ( vint32 )( 0 ); }

virtual void				put_Long( vint32 inValue ) override		{ argused1( inValue ); }
virtual vint32				get_Long( void ) const override			{ return ( vint32 )( 0 ); }

virtual void				put_ULong( vuint32 inValue ) override		{ argused1( inValue ); }
virtual vuint32				get_ULong( void ) const override			{ return ( vuint32 )( 0 ); }

//virtual void				put_LLong( vint64 inValue )		{ argused1( inValue ); }
//virtual vint64				get_LLong( void ) const			{ return mValue; }

virtual void				put_ULLong( vuint64 inValue ) override	{ argused1( inValue ); }
virtual vuint64				get_ULLong( void ) const override		{ return ( vuint64 )( 0 ); }

virtual void				put_Float( float inValue ) override		{ argused1( inValue ); }
virtual float				get_Float( void ) const override			{ return ( float )( 0 ); }

virtual void				put_Double( double inValue ) override	{ argused1( inValue ); }
virtual double				get_Double( void ) const override		{ return ( double )( 0 ); }

							
	public://///////////////////////////////////////////////////////////////////////////////////

// interface I_Serializable:

virtual void				From( I_IStream_Ptr inStream, bool inBlock = true ) override
							{
								argused1(inBlock);
								value_type* pv = PS::get_ValuePtr();
								inStream->get(*pv);
							}

virtual void				To( I_OStream_Ptr inStream, bool inBlock = true ) const override
							{
								argused1(inBlock);
								const value_type* pv = PS::get_ValuePtr();
								inStream->put(*pv);
							}


	public://///////////////////////////////////////////////////////////////////////////////////

//#if FBL_TEST_CODE

virtual void				Init( void ) override;
virtual void 				Increment( void ) override; 

//#endif // FBL_TEST_CODE


	private://////////////////////////////////////////////////////////////////////////////////

		I_Database*			mpDatabase;			///< If set dateTime format must be taken from db.
		
		const DTFormat*		mDTFormat;			///< Date and time format (own or passed).
		bool				mDTFormatOwner;
};


#pragma mark === Value_datetime ===


/**********************************************************************************************/
// Implements Value DateTime.
// 
class FBL_SHARED_EXP_CLASS Value_datetime : public Value_datetime_imp
{

	public://///////////////////////////////////////////////////////////////////////////////////

							Value_datetime( I_Database_Ptr inpDatabase = nullptr );
							Value_datetime( const DTFormat* inDTFormat );
							Value_datetime( const Value_datetime& inOther );

							Value_datetime( 
								vint32			inYear,
								vuint16			inMonth,
								vuint16			inDay,
								vuint16			inHours,
								vuint16			inMinutes,
								vuint16			inSeconds,
								vuint16			inMilliseconds,
								const DTFormat*	inDTFormat = nullptr );

virtual						~Value_datetime( void );

virtual	I_Value_Ptr			Clone( bool inCopyData = true ) const override;
};


#pragma mark === Value_datetime_null ===


/**********************************************************************************************/
// Implements nullable Value DateTime.
// 
class FBL_SHARED_EXP_CLASS Value_datetime_null : 
	public Value_datetime,
	protected policyNullable<policyInPlace_llong>
{

	public:///////////////////////////////////////////////////////////////////////////////////

		typedef policyNullable<policyInPlace_llong> PN;


	public:///////////////////////////////////////////////////////////////////////////////////

							Value_datetime_null( I_Database_Ptr inpDatabase = nullptr );
							Value_datetime_null( const DTFormat* inDTFormat );
							Value_datetime_null( const Value_datetime_null&	inOther );

							Value_datetime_null( 
								vint32			inYear,
								vuint16			inMonth,
								vuint16			inDay,
								vuint16			inHours,
								vuint16			inMinutes,
								vuint16			inSeconds,
								vuint16			inMilliseconds,
								const DTFormat*	inDTFormat = nullptr );

virtual						~Value_datetime_null( void );


virtual	I_Value_Ptr			Clone( bool inCopyData = true ) const override;

virtual bool				get_IsNullable( void ) const override  { return true; }
virtual bool 				get_IsNull( void ) const override;
virtual void 				put_IsNull( bool inValue ) override;

	// ---------------------
	// Value compare methods:

virtual	int					Compare( 
								const I_Value& inOther,
								COMPARE_TYPE inCompareType = kNatureCompare ) const override;
							
virtual String				get_String( tslen inLimit = -1 ) const override;
virtual UChar*				get_String( 
								UChar* outString, 
								tslen inBufferChars ) const override;

virtual char*				get_String( 
								char* outString, 
								tslen inBufferChars ) const override;
};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_Value_DateTime_h
