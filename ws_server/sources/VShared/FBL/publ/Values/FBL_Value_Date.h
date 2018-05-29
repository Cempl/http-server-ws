/**********************************************************************************************/
/* FBL_Value_Date.h 	                                                   					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_Value_Date_h
	#define _FBL_Value_Date_h
#pragma once

#include <VShared/FBL/publ/Interfaces/FBL_I_ValueDate.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Database.h>

// FBL:
#include <VShared/FBL/publ/Values/FBL_Value_Numeric.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_CLASS(Value_date);
SMART_CLASS(Value_date_null);


/**********************************************************************************************/
union DateEncoded;
enum EDateFormat;


/**********************************************************************************************/
// FAST STR => DATE
// Returns the number of digits in year representation.
//
FBL_SHARED_EXP vuint16 Convert_str_date_aa_fast( 	// char* => DATE
	const char*			inStr, 
	const DTFormat*		inFormat,
	DateEncoded&		outDateEnc );
	
FBL_SHARED_EXP vuint16 Convert_str_date_uu_fast( 	// UChar* => DATE
	const UChar*		inStr, 
	const UChar*		inStrEnd, 
	const DTFormat*		inFormat,
	DateEncoded&		outDateEnc );
	

/**********************************************************************************************/
// FAST DATE => STR
//
FBL_SHARED_EXP tslen Convert_date_str_fast( 		// DATE => char*
	const DateEncoded&	inDateEnc, 
	const EDateFormat	inDateFormat,
	const char			inSep, 
	char*				outString );

// DATE => UChar*
FBL_SHARED_EXP tslen Convert_date_str_fast( 		// DATE => UChar*
	const DateEncoded&	inDateEnc, 
	const EDateFormat	inDateFormat,
	const UChar			inSep, 
	UChar*				outString );


/**********************************************************************************************/
// CONVERT with CHECKS
//
FBL_SHARED_EXP void Convert_str_date_aa_aux( 					// char* => DATE
	const char*			inStr,
	const DTFormat* 	inDTFormat,
	DateEncoded&		ioDate );

FBL_SHARED_EXP void Convert_str_date_uu_aux( 					// UChar* => DATE
	const UChar*		inStr,
	const UChar*		inStrEnd,
	const DTFormat* 	inDTFormat,
	DateEncoded&		ioDate );


/**********************************************************************************************/
// VALUE STRING <=> VALUE DATE
//
FBL_SHARED_EXP void Convert_str_date( 					
	const I_Value*	inValue, 
	I_Value*		outValue );

FBL_SHARED_EXP void Convert_date_str( 
	const I_Value*	inValue, 
	I_Value*		outValue );


#pragma mark === Value_date_imp ===


/**********************************************************************************************/
// This class implements I_ValueDate interface
// 
class FBL_SHARED_EXP_CLASS Value_date_imp : 
	public I_ValueDate, 
	public Value_Numeric< kTypeDate, vcDate, policyInPlace_long >
{
		IMPLEMENT_UNKNOWN(Value_date_imp)											
		BEGIN_INTERFACE_TABLE(Value_date_imp)										
			IMPLEMENT_INTERFACE(I_Value)										
			IMPLEMENT_INTERFACE(I_Serializable)										
			IMPLEMENT_INTERFACE(I_ValueDate)										
		END_INTERFACE_TABLE()

	public://///////////////////////////////////////////////////////////////////////////////////

							Value_date_imp( I_Database_Ptr inpDatabase = nullptr );
							Value_date_imp( const DTFormat* inDTFormat );
							Value_date_imp( const Value_date_imp& inOther );

virtual						~Value_date_imp( void );


	// ---------------------
	// DateTimeFormat:
	
		// <DateTimeFormat> [r/o]
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



	public://///////////////////////////////////////////////////////////////////////////////////

// I_Value API:

		using Value_Numeric< kTypeDate, vcDate, policyInPlace_long >::Assign;

	// ---------------------
	// Properties:

virtual vuint32				get_MaxChars( void ) const override;


	// ---------------------
	// get/put string:

virtual String				get_String( tslen inLimit = -1 ) const override;
virtual void				put_String( const String& inStr ) override;

virtual UChar*				get_String( 
								UChar* 			outString, 
								tslen 			inBufferChars ) const override;

virtual void				put_String( 
								const UChar* 	inStart,
								const UChar* 	inEnd = nullptr ) override;
								
virtual char*				get_String( 
								char* 			outString, 
								tslen 			inBufferChars ) const override;

virtual void				put_String( 
								const char* 	inStart,
								const char* 	inEnd = nullptr ) override;


	// ---------------------
	// Numeric properties:

virtual void				put_Boolean( bool inValue ) override		{ argused1( inValue ); }
virtual bool				get_Boolean( void ) const override			{ return ( bool )( 0 ); }

virtual void				put_Byte( vuint8 inValue ) override			{ argused1( inValue ); }
virtual vuint8				get_Byte( void ) const override				{ return ( vuint8 )( 0 ); }

virtual void				put_Short( vint16 inValue ) override		{ argused1( inValue ); }
virtual vint16				get_Short( void ) const override			{ return ( vint16 )( 0 ); }

virtual void				put_UShort( vuint16 inValue ) override		{ argused1( inValue ); }
virtual vuint16				get_UShort( void ) const override			{ return ( vuint16 )( 0 ); }

virtual void				put_Medium( vint32 inValue ) override		{ argused1( inValue ); }
virtual vint32				get_Medium( void ) const override			{ return ( vint32 )( 0 ); }

virtual void				put_LLong( vint64 inValue ) override		{ argused1( inValue ); }
virtual vint64				get_LLong( void ) const override			{ return ( vint64 )( mValue ); }

virtual void				put_ULLong( vuint64 inValue ) override		{ argused1( inValue ); }
virtual vuint64				get_ULLong( void ) const override			{ return ( vuint64 )( 0 ); }

virtual void				put_Float( float inValue ) override			{ argused1( inValue ); }
virtual float				get_Float( void ) const override			{ return ( float )( 0 ); }

virtual void				put_Double( double inValue ) override		{ argused1( inValue ); }
virtual double				get_Double( void ) const override			{ return ( double )( 0 ); }


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


#pragma mark === Value_date ===


/**********************************************************************************************/
// Implements Value date.
// 
class FBL_SHARED_EXP_CLASS Value_date : public Value_date_imp
{

public://///////////////////////////////////////////////////////////////////////////////////

							Value_date( I_Database_Ptr inpDatabase = nullptr );
							Value_date( const DTFormat* inDTFormat );
							Value_date( const Value_date& inOther );

							Value_date( 
								vint32			inYear,
								vuint16			inMonth,
								vuint16			inDay,
								const DTFormat*	inDTFormat = nullptr );


virtual						~Value_date( void );


virtual	I_Value_Ptr			Clone( bool inCopyData = true ) const override;

};


#pragma mark === Value_date_null ===


/**********************************************************************************************/
// Implements nullable Value date.
// 
class FBL_SHARED_EXP_CLASS Value_date_null : 
	public Value_date,
	protected policyNullable< policyInPlace_long >
{						

	public:///////////////////////////////////////////////////////////////////////////////////

		typedef policyNullable<policyInPlace_long>					PN;


public:///////////////////////////////////////////////////////////////////////////////////

							Value_date_null( I_Database_Ptr inpDatabase = nullptr );
							Value_date_null( const DTFormat* inDTFormat );
							Value_date_null( const Value_date& inOther );

							Value_date_null( 
								vint32			inYear,
								vuint16			inMonth,
								vuint16			inDay,
								const DTFormat*	inDTFormat = nullptr );


virtual						~Value_date_null( void );


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
								tslen 		inLimit = -1 ) const override;

virtual UChar*				get_String( 
								UChar* 		outString, 
								tslen 		inBufferChars ) const override;

virtual char*				get_String( 
								char* 		outString, 
								tslen 		inBufferChars ) const override;

};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_Value_Date_h
