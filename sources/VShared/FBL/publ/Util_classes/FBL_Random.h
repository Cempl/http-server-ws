/**********************************************************************************************/
/* FBL_Random.h   																		  	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_Random_h 
	#define _FBL_Random_h 

#include <VShared/FBL/publ/Headers/FBL.h>
	
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace

/**********************************************************************************************/
SMART_CLASS	( Random );

/**********************************************************************************************/
class FBL_SHARED_EXP_CLASS Random : public I_Unknown
{
		IMPLEMENT_SINGLE_UNKNOWN( Random )

	public://///////////////////////////////////////////////////////////////////////////////////

		// ---------------------
		// Construction:
	
							Random();

		virtual				~Random( void );
							

	public://///////////////////////////////////////////////////////////////////////////////////

		void				Init( void );

		void				Init( vuint32 inSeed1, vuint32 inSeed2 );
		
		double				Value( void );

	protected://////////////////////////////////////////////////////////////////////////////////

		vuint32				mSeed1;
		vuint32				mSeed2;
		vuint32				mMax_value;
		double				mMax_value_dbl;

};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_Random_h
