/**********************************************************************************************/
/* FBL_Random.cpp  																			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Util_classes/FBL_Random.h>
#include <VShared/FBL/publ/Utilities/FBL_DateTimeUtilities.h>


/**********************************************************************************************/
FBL_Begin_Namespace 

/**********************************************************************************************/
Random::Random()
:
	mSeed1(0),
	mSeed2(0),
	mMax_value(0),
	mMax_value_dbl(0)
{
}


/**********************************************************************************************/
Random::~Random( void )
{
}


/**********************************************************************************************/
void Random::Init( void )
{
	// Some magic commonly used seeds:
	Init(
		(vuint32)((vuint32)GetTimeStamp_2() *0x10001L+55555555L),
		(vuint32)((vuint32)GetTimeStamp_2() *0x10000001L ) );
}


/**********************************************************************************************/
void Random::Init( vuint32 inSeed1, vuint32 inSeed2 )
{
	mMax_value = 0x3FFFFFFFL;
	mMax_value_dbl = (double)mMax_value;
	mSeed1 = inSeed1 % mMax_value;
	mSeed2 = inSeed2 % mMax_value;
}


/**********************************************************************************************/
double Random::Value( void )
{
	mSeed1 = ( mSeed1*3 + mSeed2 ) % mMax_value;
	mSeed2 = ( mSeed1 + mSeed2+33 ) % mMax_value;
	return ( ((double)mSeed1) / mMax_value_dbl );
}


/**********************************************************************************************/
FBL_End_Namespace


