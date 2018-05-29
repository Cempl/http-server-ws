/**********************************************************************************************/
/* FBL_DTFormat.h      	                                                				 	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                       					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_DTFormat_h 
	#define _FBL_DTFormat_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Utilities/FBL_DateTimeUtilities.h>

#include <stdio.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// Type that specify format DateTime for database.
//
enum EDateFormat
{
	kMDY	= 0,
	kDMY	= 1,
	kYMD	= 2,
	kMYD	= 3,
	kDYM	= 4,
	kYDM	= 5
};

/**********************************************************************************************/
// Type that specify first day in the week.
//
enum EFirstWeekDay
{
	kSunday = 0,
	kMonday = 1
};


/**********************************************************************************************/
class DTFormat
{
	public:///////////////////////////////////////////////////////////////////////////////////// 

	//----------------------
	// Construction

							DTFormat() :
								mFirstDay( kSunday ),								
								mBound( 20 ),
								mCentury1( 1900 ),
								mCentury2( 2000 ),
								mMaxLength( 32 )
							{
								GetCurrentDateTimeFormat( *this );
							}

		DTFormat&			operator= ( const DTFormat& inDTFormat )
							{
								mDateSep = inDTFormat.mDateSep;
								mTimeSep = inDTFormat.mTimeSep;
								mDateFormat = inDTFormat.mDateFormat;
								mFirstDay = inDTFormat.mFirstDay;

								SetBound( inDTFormat.GetBound() );
								SetCentury1( inDTFormat.GetCentury1() );
								SetCentury2( inDTFormat.GetCentury2() );
								SetMaxLength( inDTFormat.GetMaxLength() );
								
								return *this;
							}

		bool				AssignChangesFrom( const DTFormat& inDTFormat )
							{
								bool result(false);

								if( mDateSep == inDTFormat.mDateSep 
									&& mTimeSep == inDTFormat.mTimeSep 
									&& mDateFormat == inDTFormat.mDateFormat
									&& mFirstDay == inDTFormat.mFirstDay
									&& GetBound() == inDTFormat.GetBound() 
									&& GetCentury1() == inDTFormat.GetCentury1() 
									&& GetCentury2() == inDTFormat.GetCentury2() 
									&& GetMaxLength() == inDTFormat.GetMaxLength() )
								{
									result = false;
								}
								else
								{
									result = true;
									operator=( inDTFormat );
								}

								return result;
							}

virtual						~DTFormat( void ) { }

							
	public:///////////////////////////////////////////////////////////////////////////////////// 
							
		vuint16				GetBound( void ) const { return mBound; }	
		void				SetBound( vuint16 inBound = 20 ) { mBound = inBound; }
				
		tslen				GetMaxLength( void ) const { return mMaxLength; }	
		void				SetMaxLength( tslen inMaxLength = 32 ) { mMaxLength = inMaxLength; }

		void 				GetCentury( vuint32& outCentury1, vuint32& outCentury2, vuint16& outBound ) const
							{
								outCentury1 = mCentury1;
								outCentury2	= mCentury2;
								outBound 	= mBound;
							}	

		vuint32				GetCentury1( void ) const { return mCentury1; }	
		void				SetCentury1( vuint32 inCentury1 = 1900) 
							{
								mCentury1 = inCentury1;
							}

		vuint32				GetCentury2( void ) const { return mCentury2; }	
		void				SetCentury2( vuint32 inCentury2 = 2000) 
							{
								mCentury2 = inCentury2;
							}

		void				SetCentury( vuint32 inCentury1 = 1900, vuint32 inCentury2 = 2000, vuint16 inBound = 20) 
							{
								FBL_CHECK( inCentury2 - inCentury1 == 100 ); 

								mCentury1 = inCentury1;
								mCentury2 = inCentury2;
								mBound 	  = inBound;
							}
	
							/** Peroforms autocorrection of Year from YY to YYYY format of full year.
								This is comfortable for user. 
								Auto correction works only for year in range 0..99.
								Note, that for negative year there is NO auto correction. 
								You can turn OFF auto correction at all if set mBound to ZERO. */
		void				AutoCentury( vint32& ioYear ) const
							{		
								if( mBound && 0 <= ioYear && ioYear <= 99 )
								{
									if( ioYear <= mBound )
										ioYear += (vint32) mCentury2;
									else if( ioYear > mBound )
										ioYear += (vint32) mCentury1;	
								}
							}


	public:///////////////////////////////////////////////////////////////////////////////////// 

		UChar				mDateSep;		// default is '/'
		UChar				mTimeSep;		// default is ':'
		EDateFormat			mDateFormat;	// default kDMY
		EFirstWeekDay		mFirstDay;		// default is Sunday	

	
	private://////////////////////////////////////////////////////////////////////////////////// 
	
		// The next 3 parameters allow us to do auto-adding of century: YY -> YYYY
		// If these parameters are zero then auto expanding is not performed.
		// This can be used if for historical database.
		
		vuint16				mBound;				// default 20			00 => 2000, 20 => 2020
												//						21 => 1921, 99 => 1999
												
		vuint32				mCentury1;			// default 1900
		vuint32				mCentury2;			// default 2000

		tslen				mMaxLength;			// default 32			"1999-01-01 12:15:16"
};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_DTFormat_h
