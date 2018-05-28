/**********************************************************************************************/
/* FBL_Value_Time.h 	                                                   					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_Value_Time_h
	#define _FBL_Value_Time_h
#pragma once

#include <VShared/FBL/publ/Interfaces/FBL_I_ValueTime.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Database.h>

// FBL:
#include <VShared/FBL/publ/Values/FBL_Value_Numeric.h>


#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_CLASS(Value_time);
SMART_CLASS(Value_time_null);


/**********************************************************************************************/
union TimeEncoded;
enum EDateFormat;


/**********************************************************************************************/
// FAST STR => TIME
//
FBL_SHARED_EXP void Convert_str_time_aa_fast( 
	const char*			inStr, 
	const DTFormat*		inDTFormat,
	TimeEncoded&		outTimeEnc );

FBL_SHARED_EXP void Convert_str_time_uu_fast( 
	const UChar*		inStr, 
	const UChar*		inStrEnd, 
	const DTFormat*		inDTFormat,
	TimeEncoded&		outTimeEnc );


/**********************************************************************************************/
// FAST TIME => STR
//
FBL_SHARED_EXP tslen Convert_time_str_aa_fast( 
	const TimeEncoded&	inTimeEnc, 
	const char			inSep, 
	char*				outString );

FBL_SHARED_EXP tslen Convert_time_str_uu_fast( 
	const TimeEncoded&	inTimeEnc, 
	const UChar			inSep, 
	UChar*				outString );


/**********************************************************************************************/
// CONVERT with CHECKS
//
FBL_SHARED_EXP void Convert_str_time_aa_aux( 
	const char*			inStr,
	const DTFormat* 	inDTFormat,
	TimeEncoded&		ioTime );

FBL_SHARED_EXP void Convert_str_time_uu_aux( 
	const UChar*		inStr,
	const UChar*		inStrEnd,
	const DTFormat* 	inDTFormat,
	TimeEncoded&		ioTime );


/**********************************************************************************************/
// VALUE STRING <=> VALUE TIME
//
FBL_SHARED_EXP void Convert_str_time( 
	const I_Value*	inValue, 
	I_Value*		outValue );

FBL_SHARED_EXP void Convert_time_str( 
	const I_Value*	inValue, 
	I_Value*		outValue );


#pragma mark === Value_time_imp ===


/**********************************************************************************************/
// This class implements I_ValueTime interface
// 
class FBL_SHARED_EXP_CLASS Value_time_imp : 
	public I_ValueTime, 
	public Value_Numeric< kTypeTime, vcTime, policyInPlace_ulong >
{
		IMPLEMENT_UNKNOWN(Value_time_imp)											
		BEGIN_INTERFACE_TABLE(Value_time_imp)										
			IMPLEMENT_INTERFACE(I_Value)										
			IMPLEMENT_INTERFACE(I_ValueTime)										
			IMPLEMENT_INTERFACE(I_Serializable)										
		END_INTERFACE_TABLE()

	public://///////////////////////////////////////////////////////////////////////////////////

							Value_time_imp( I_Database_Ptr inpDatabase = nullptr );
							Value_time_imp( const DTFormat* inDTFormat );
							Value_time_imp( const Value_time_imp& inOther );

virtual						~Value_time_imp( void );

	// ---------------------
	// DateTimeFormat:

		// <DateTimeFormat> [r/o]
virtual	const DTFormat*		get_DTFormat( void ) const override;

	// ---------------------
	// Time properties
	
		// <Hour> [r/w]
virtual	vuint16				get_Hours( void ) const override;
virtual	void				put_Hours( vuint16 inHours ) override;

		// <Minutes> [r/w]
virtual	vuint16				get_Minutes( void ) const override;
virtual	void				put_Minutes( vuint16 inMinutes ) override;

		// <Seconds> [r/w]
virtual	vuint16				get_Seconds( void ) const override;
virtual	void				put_Seconds( vuint16 inSeconds ) override;

		// <Millisecond> [r/w]
virtual	vuint16				get_Milliseconds( void ) const override;
virtual	void				put_Milliseconds( vuint16 inMilliseconds ) override;

		// <Time> [r/w]
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


	public://///////////////////////////////////////////////////////////////////////////////////

// I_Value API:

		using Value_Numeric< kTypeTime, vcTime, policyInPlace_ulong >::Assign;

	// ---------------------
	//  Value access:
	
virtual String				get_String( tslen inLimit = -1 ) const override;
virtual void				put_String( const String& inStr ) override;

virtual UChar*				get_String( 
								UChar*			outString, 
								tslen 			inBufferChars ) const override;

virtual void				put_String( 
								const UChar* 	inStart,  
								const UChar* 	inEnd = nullptr ) override;

virtual char*				get_String( 
								char*			outString, 
								tslen 			inBufferChars ) const override;

virtual void				put_String( 
								const char* 	inStart,  
								const char* 	inEnd = nullptr ) override;

virtual vuint32				get_MaxChars( void ) const override  ;


	// ---------------------
	// Numeric properties:


virtual void				put_Boolean( bool inValue ) override		{ argused1( inValue ); }
virtual bool				get_Boolean( void ) const override		{ return ( bool )( 0 ); }

virtual void				put_Byte( vuint8 inValue ) override		{ argused1( inValue ); }
virtual vuint8				get_Byte( void ) const override			{ return ( vuint8 )( 0 ); }

virtual void				put_Short( vint16 inValue ) override		{ argused1( inValue ); }
virtual vint16				get_Short( void ) const override			{ return ( vint16 )( 0 ); }

virtual void				put_UShort( vuint16 inValue ) override	{ argused1( inValue ); }
virtual vuint16				get_UShort( void ) const override		{ return ( vuint16 )( 0 ); }

virtual void				put_Medium( vint32 inValue ) override		{ argused1( inValue ); }
virtual vint32				get_Medium( void ) const override		{ return ( vint32 )( 0 ); }

virtual void				put_LLong( vint64 inValue ) override		{ argused1( inValue ); }
virtual vint64				get_LLong( void ) const override			{ return mValue; }

virtual void				put_ULLong( vuint64 inValue ) override	{ argused1( inValue ); }
virtual vuint64				get_ULLong( void ) const override		{ return ( vuint64 )( 0 ); }

virtual void				put_Float( float inValue ) override		{ argused1( inValue ); }
virtual float				get_Float( void ) const override			{ return ( float )( 0 ); }

virtual void				put_Double( double inValue ) override	{ argused1( inValue ); }
virtual double				get_Double( void ) const override		{ return ( double )( 0 ); }


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


#pragma mark === Value_time ===


/**********************************************************************************************/
// Implements Value time.
// 
class FBL_SHARED_EXP_CLASS Value_time : public Value_time_imp
{

	public://///////////////////////////////////////////////////////////////////////////////////

							Value_time( I_Database_Ptr inpDatabase = nullptr );
							Value_time( const DTFormat* inDTFormat );
							Value_time( const Value_time& inOther );

							Value_time( 
								vuint16			inHours, 
								vuint16			inMinutes,
								vuint16			inSeconds,
								vuint16			inMilliseconds,
								const DTFormat*	inDTFormat = nullptr );

virtual						~Value_time( void );


virtual	I_Value_Ptr			Clone( bool inCopyData = true ) const override;

};


#pragma mark === Value_time_null ===


/**********************************************************************************************/
/** Implements Value time with nullable.
*/ 
class FBL_SHARED_EXP_CLASS Value_time_null : 
	public Value_time,
	protected policyNullable<policyInPlace_ulong>
{

	public:///////////////////////////////////////////////////////////////////////////////////

		typedef policyNullable<policyInPlace_ulong>				PN;


	public:///////////////////////////////////////////////////////////////////////////////////

							Value_time_null( I_Database_Ptr inpDatabase = nullptr );
							Value_time_null( const DTFormat* inDTFormat );
							Value_time_null( const Value_time_null&	inOther );

							Value_time_null( 
								vuint16 		inHours, 
								vuint16 		inMinutes,
								vuint16 		inSeconds,
								vuint16			inMilliseconds,
								const DTFormat*	inDTFormat = nullptr );

virtual						~Value_time_null( void );


virtual	I_Value_Ptr			Clone( bool inCopyData = true ) const override;

virtual bool				get_IsNullable( void ) const override  { return true; }
virtual bool 				get_IsNull( void ) const override;
virtual void 				put_IsNull( bool inValue ) override;


	// ---------------------
	// Value compare methods:

virtual	int					Compare( 
								const I_Value& 	inOther,
								COMPARE_TYPE 	inCompareType = kNatureCompare ) const override;
														
virtual String				get_String( 
								tslen 	inLimit = -1 ) const override;

virtual UChar*				get_String( 
								UChar* 	outString, 
								tslen 	inBufferChars ) const override;

virtual char*				get_String( 
								char* 	outString, 
								tslen 	inBufferChars ) const override;
};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_Value_Time_h
