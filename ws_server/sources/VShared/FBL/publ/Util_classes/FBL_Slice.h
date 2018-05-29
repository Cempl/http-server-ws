/**********************************************************************************************/
/* FBL_Slice.h      	                                                				 	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                       					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_Slice_h 
	#define _FBL_Slice_h
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
/* 
	DRAFT?

	This class added for KeyValue, following to API of LevelDB, RockDB.
	This class do NOT owns buffer in contras to all oter VDB files.

	It contains Compare(), which LevelDB uses to sort keys.
    But we ?
*/
class Slice
{
    public://///////////////////////////////////////////////////////////////////////////////////

                            Slice(void) {}

                            Slice(const char* inData, size_t inSize = 0)
                                : mpData(inData)
                                , mSize(inSize ? inSize : strlen(inData))
                            {
                            }


    public://///////////////////////////////////////////////////////////////////////////////////

        const char*			get_Data(void) const { return mpData; }
        size_t				get_Size(void) const { return mSize; }

        void				Clear(void) { mpData = nullptr; mSize = 0; }

        int 				Compare(const Slice& b) const
                            {
                                const size_t min_len = (mSize < b.mSize) ? mSize : b.mSize;
                                int r = memcmp(mpData, b.mpData, min_len);
                                if (r == 0)
                                {
                                    if (mSize < b.mSize) r = -1;
                                    else if (mSize > b.mSize) r = +1;
                                }

                                return r;
                            }


    private:////////////////////////////////////////////////////////////////////////////////////

        const char*			mpData = nullptr;
        size_t				mSize = 0;

}; // Intentionally copyable


/**********************************************************************************************/
inline bool operator==(const Slice& x, const Slice& y)
{
	return ((x.get_Size() == y.get_Size())
		&& (memcmp(x.get_Data(), y.get_Data(), x.get_Size()) == 0));
}


/**********************************************************************************************/
inline bool operator!=(const Slice& x, const Slice& y)
{
	return !(x == y);
}


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_Slice_h
