/**********************************************************************************************/
/* FBL_Value_Approximate_Imp.h                                             					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_Value_Approximate_Imp_h
	#define _FBL_Value_Approximate_Imp_h
#pragma once

#include <VShared/FBL/publ/Interfaces/FBL_I_ValueApproximate.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// forward declarations:
//
extern bool IsTrueStr( const char* inString );
extern bool IsTrueStr( const UChar* inString );


/**********************************************************************************************/
// This class implements I_ValueApproximate interface
// 
class FBL_SHARED_EXP_CLASS Value_Approximate_Imp : 
	public I_ValueApproximate
{
	public://///////////////////////////////////////////////////////////////////////////////////

							Value_Approximate_Imp( vuint16 inPrecision, vuint16 inScale )
							:
								mPrecision( inPrecision ),
								mScale( inScale )
							{
							}

							Value_Approximate_Imp( const Value_Approximate_Imp& inOther )
                            :
								mPrecision( inOther.get_Precision() ),
								mScale    ( inOther.get_Scale()     )
							{
							}


	public://///////////////////////////////////////////////////////////////////////////////////

// I_ValueApproximate API:

virtual						~Value_Approximate_Imp( void ) {}


virtual	vuint16				get_Precision( void ) const override
								{ return mPrecision; }

virtual	void				put_Precision( vuint16 inPrecision ) override
								{ mPrecision = inPrecision; }
	
virtual	vuint16				get_Scale( void ) const override
								{ return mScale; }

virtual	void				put_Scale( vuint16 inScale ) override
								{ mScale = inScale; }


	protected://////////////////////////////////////////////////////////////////////////////////

		vuint16				mPrecision;
		vuint16				mScale;
};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_Value_Approximate_Imp_h
