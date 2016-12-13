/**********************************************************************************************/
/* FBL_MurMurHash.cpp			 	                                       					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Util_classes/FBL_MurMurHash.h>


/**********************************************************************************************/
FBL_Begin_Namespace

/**********************************************************************************************/
static const unsigned int	gMurMurHash_m = 0x5bd1e995;
static const int			gMurMurHash_r = 24;

#define mmix(h,k) { k *= gMurMurHash_m; k ^= k >> gMurMurHash_r; k *= gMurMurHash_m; h *= gMurMurHash_m; h ^= k; }


/**********************************************************************************************/
MurMurHash::MurMurHash()
{
	Init();
}


/**********************************************************************************************/
void MurMurHash::Add( const unsigned char* inpData, int inLen )
{
	mSize += inLen;
	MixTail( &inpData, &inLen );

	while( inLen >= 4 )
	{
		unsigned int k =		inpData[0] 
							|	(inpData[1] << 8)
							|	(inpData[2] << 16)
							|	(inpData[3] << 24);
							
		mmix( mHash, k );
		
		inpData += 4;
		inLen	-= 4;
	}
	
	MixTail( &inpData, &inLen );
}


/**********************************************************************************************/
unsigned int MurMurHash::Calculate( void )
{
	unsigned int res = 0;
	
	mmix( mHash, mTail );
	mmix( mHash, mSize );

	mHash ^= mHash >> 13;
	mHash *= gMurMurHash_m;
	mHash ^= mHash >> 15;

	res = mHash;
	
	Init();
	
	return res;
}


#pragma mark -

/**********************************************************************************************/
void MurMurHash::Init( void )
{
	mHash	= 0;
	mTail	= 0;
	mCount	= 0;
	mSize	= 0;
}


/**********************************************************************************************/
void MurMurHash::MixTail( const unsigned char **inppData, int* iopLen )
{
	while( *iopLen && ((*iopLen<4) || mCount) )
	{
		mTail |= (*(*inppData)++) << (mCount * 8);
		mCount++;

		(*iopLen)--;
		
		if( mCount == 4 )
		{
			mmix( mHash, mTail );
			mTail = 0;
			mCount = 0;
		}
	}
}


/**********************************************************************************************/
FBL_End_Namespace
