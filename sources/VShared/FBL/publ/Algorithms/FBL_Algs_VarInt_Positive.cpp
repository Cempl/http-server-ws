/**********************************************************************************************/
/* FBL_Algs_VarInt_Positive.cpp																  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include "FBL_Algs_VarInt_Positive.h"


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// Returns the number of bytes that will be needed to store the given 64-bit integer.
//
int VarIntLen64( vuint64 v )
{
    int i = 0;

    do
    {
        i++;
        v >>= 7;
    }while( v != 0 && (i<9) );

    return i;
}


/**********************************************************************************************/
// Write a 64-bit variable-length integer to memory starting at p[0].
// The length of data write will be between 1 and 9 bytes.  The number
// of bytes written is returned.
//
// A variable-length integer consists of the lower 7 bits of each byte
// for all bytes that have the 8th bit set and one byte with the 8th
// bit clear.  Except, if we get to the 9th byte, it stores the full
// 8 bits and is the last byte.
//
static int putVarInt64_( unsigned char *p, vuint64 v )
{
  int i, j, n;
  vuint8 buf[10];

  if( v & (((vuint64)0xff000000) << 32) )
  {
    p[8] = (vuint8)v;
    v >>= 8;
    
    for( i=7; i>=0; i-- )
    {
      p[i] = (vuint8)((v & 0x7f) | 0x80);
      v >>= 7;
    }
    return 9;
  }    
  
  n = 0;
  do
  {
    buf[n++] = (vuint8)((v & 0x7f) | 0x80);
    v >>= 7;
  }while( v!=0 );
  
  buf[0] &= 0x7f;
  FBL_CHECK( n<=9 );
  
  for( i=0, j = n-1; j >= 0; j--, i++ )
  {
    p[i] = buf[j];
  }
  
  return n;
}


/**********************************************************************************************/
int PutVarInt64( unsigned char *p, vuint64 v )
{
  if( v <= 0x7f )
  {
    p[0] = v&0x7f;
    
    return 1;
  }
  
  if( v <= 0x3fff )
  {
    p[0] = ((v>>7) & 0x7f) | 0x80;
    p[1] = v & 0x7f;
    
    return 2;
  }
  
  return putVarInt64_( p, v );
}



#pragma mark -


/**********************************************************************************************/
// Bitmasks used by GetVarInt().  These precomputed constants
// are defined here rather than simply putting the constant expressions
// inline in order to work around bugs in the RVT compiler.
//
// SLOT_2_0     A mask for  (0x7f<<14) | 0x7f
//
// SLOT_4_2_0   A mask for  (0x7f<<28) | SLOT_2_0
//
#define SLOT_2_0     0x001fc07f
#define SLOT_4_2_0   0xf01fc07f


/**********************************************************************************************/
// Read a 64-bit variable-length integer from memory starting at p[0].
// Return the number of bytes read. The value is stored in *v.
//
vuint8 GetVarInt64(const unsigned char *p, vuint64 *v)
{
  vuint32 a,b,s;

  a = *p;
  // a: p0 (unmasked)
  if (!(a&0x80))
  {
    *v = a;
    
    return 1;
  }

  p++;
  b = *p;
  // b: p1 (unmasked)
  if (!(b&0x80))
  {
    a &= 0x7f;
    a = a<<7;
    a |= b;
    *v = a;
    
    return 2;
  }

  // Verify that constants are precomputed correctly
  FBL_CHECK( SLOT_2_0 == ((0x7f<<14) | (0x7f)) );
  FBL_CHECK( SLOT_4_2_0 == ((0xfU<<28) | (0x7f<<14) | (0x7f)) );

  p++;
  a = a<<14;
  a |= *p;
  
  // a: p0<<14 | p2 (unmasked)
  if (!(a&0x80))
  {
    a &= SLOT_2_0;
    b &= 0x7f;
    
    b = b << 7;
    a |= b;
    *v = a;
    
    return 3;
  }

  // CSE1 from below
  a &= SLOT_2_0;
  p++;
  b = b<<14;
  b |= *p;
  
  // b: p1<<14 | p3 (unmasked)
  if (!(b&0x80))
  {
    b &= SLOT_2_0;
    
    // moved CSE1 up
    // a &= (0x7f<<14)|(0x7f);
    a = a << 7;
    a |= b;
    *v = a;
  
      return 4;
  }

  // a: p0<<14 | p2 (masked)
  // b: p1<<14 | p3 (unmasked)
  // 1:save off p0<<21 | p1<<14 | p2<<7 | p3 (masked)
  // moved CSE1 up
  // a &= (0x7f<<14)|(0x7f);
  b &= SLOT_2_0;
  s = a;
  // s: p0<<14 | p2 (masked)

  p++;
  a = a << 14;
  a |= *p;
  // a: p0<<28 | p2<<14 | p4 (unmasked)
  if (!(a&0x80))
  {
    // we can skip these cause they were (effectively) done above in calc'ing s
    // a &= (0x7f<<28)|(0x7f<<14)|(0x7f);
    // b &= (0x7f<<14)|(0x7f);
    b = b << 7;
    a |= b;
    s = s>>18;
    *v = ((vuint64)s)<<32 | a;
    return 5;
  }

  // 2:save off p0<<21 | p1<<14 | p2<<7 | p3 (masked)
  s = s << 7;
  s |= b;
  // s: p0<<21 | p1<<14 | p2<<7 | p3 (masked)

  p++;
  b = b << 14;
  b |= *p;
  // b: p1<<28 | p3<<14 | p5 (unmasked)
  if (!(b & 0x80))
  {
    // we can skip this cause it was (effectively) done above in calc'ing s
    // b &= (0x7f<<28)|(0x7f<<14)|(0x7f);
    a &= SLOT_2_0;
    a = a << 7;
    a |= b;
    s = s >> 18;
    *v = ((vuint64)s)<<32 | a;
    
    return 6;
  }

  p++;
  a = a << 14;
  a |= *p;
  // a: p2<<28 | p4<<14 | p6 (unmasked)
  if (!(a & 0x80))
  {
    a &= SLOT_4_2_0;
    b &= SLOT_2_0;
    b = b << 7;
    a |= b;
    s = s >> 11;
    *v = ((vuint64)s)<<32 | a;
    
    return 7;
  }

  // CSE2 from below
  a &= SLOT_2_0;
  p++;
  b = b << 14;
  b |= *p;
  // b: p3<<28 | p5<<14 | p7 (unmasked)
  if (!(b&0x80))
  {
    b &= SLOT_4_2_0;
    // moved CSE2 up
    // a &= (0x7f<<14)|(0x7f);
    a = a << 7;
    a |= b;
    s = s >> 4;
    *v = ((vuint64)s)<<32 | a;
    
    return 8;
  }

  p++;
  a = a << 15;
  a |= *p;
  // a: p4<<29 | p6<<15 | p8 (unmasked)

  // moved CSE2 up
  // a &= (0x7f<<29)|(0x7f<<15)|(0xff);
  b &= SLOT_2_0;
  b = b << 8;
  a |= b;

  s = s << 4;
  b = p[-4];
  b &= 0x7f;
  b = b >> 3;
  s |= b;

  *v = ((vuint64)s)<<32 | a;

  return 9;
}


/**********************************************************************************************/
// Read a 32-bit variable-length integer from memory starting at p[0].
// Return the number of bytes read.  The value is stored in *v.
//
// If the varint stored in p[0] is larger than can fit in a 32-bit unsigned
// integer, then set *v to 0xffffffff.
//
// A MACRO version, FBL_getVarInt32(), is provided, which inlines the
// single-byte case.  All code should use the MACRO version as 
// this function assumes the single-byte case has already been handled.
//
// This method only unroll the first 1-, 2-, and 3- byte cases.  The very
// rare larger cases can be handled by the slower 64-bit varint routine.
//
vuint8 GetVarInt32_(const unsigned char *p, vuint32 *v)
{
  vuint32 a,b;

  // The 1-byte case. Overwhelmingly the most common.  Handled inline
  // by the getVarin32() macro
  a = *p;
  // a: p0 (unmasked)
  
#ifndef FBL_getVarInt32
  if (!(a & 0x80) ) // Values between 0 and 127
  {
    *v = a;
    return 1;
  }
#endif // FBL_getVarInt32

  // The 2-byte case
  p++;
  b = *p;
  // b: p1 (unmasked)
  if (!(b & 0x80))
  {
    // Values between 128 and 16383
    a &= 0x7f;
    a = a << 7;
    *v = a | b;
    return 2;
  }

  // The 3-byte case
  p++;
  a = a << 14;
  a |= *p;
  // a: p0<<14 | p2 (unmasked)
  if (!(a & 0x80))
  {
    // Values between 16384 and 2097151
    a &= (0x7f << 14) | (0x7f);
    b &= 0x7f;
    b = b << 7;
    *v = a | b;
    return 3;
  }

  {
    p -= 2;
    vuint64 v64;
    vuint8 n = GetVarInt64( p, &v64 );
    FBL_CHECK( n > 3 && n <= 9 );

	*v = (v64 & vuint32_max) == v64 ? (vuint32)v64 : 0xffffffff;
    
    return n;
  }
}


/**********************************************************************************************/
FBL_End_Namespace
