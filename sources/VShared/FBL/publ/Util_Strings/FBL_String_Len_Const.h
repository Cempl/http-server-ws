/**********************************************************************************************/
/* FBL_String_Len_Const.h                                                      				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                       					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/
/*
 *	Defines constants for the length of string representation for the basic types.
 *	Assumes, that for the integral types radix is 10.
 */

#ifndef _FBL_String_Len_Const_h 
	#define _FBL_String_Len_Const_h 
#pragma once

#include <VShared/FBL/publ/Headers/FBL.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>
 

/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// For bool this is: "unknown" "true", "false", "yes", "no", "on", "off", "1", "0" 
const vuint32 kBoolStrMaxLen = 7;	

// this is 255, i.e. 3 chars
const vuint32 kUCharStrMaxLen = 3;

// this is -32768, i.e. 6 chars
const vuint32 kShortStrMaxLen = 6;

// this is "65535", i.e. 5 chars
const vuint32 kUShortStrMaxLen = 5;

// this is -2147483647, i.e. 11 chars
const vuint32 kLongStrMaxLen = 11;

// this is "4294967295", i.e. 10 chars
const vuint32 kULongStrMaxLen = 10;

// this is -9223372036854775807, i.e. 20 chars
const vuint32 kLLongStrMaxLen = 20;

// this is 18446744073709551613, i.e. 20 chars
const vuint32 kULLongStrMaxLen = 20;

// this is ???, i.e. 20 chars
const vuint32 kFloatStrMaxLen = 20;

// this is ???, i.e. 40 chars
const vuint32 kDoubleStrMaxLen = 40;

// the len of date in -dd/mm/yyyy format
const vuint32 kDateStrMaxLength = 11;

// the len of time in hh.mm.ss.nnn format (n - ms)
const vuint32 kTimeStrMaxLength = 12;

// the len of date-time in -dd.mm.yyyy hh.mm.ss.nnn format (n - ms)
const vuint32 kDateTimeStrMaxLength = 24;

// the len of money type
const vuint32 kMoneyStrMaxLength = 21; // MONEY_SCALE(18) + "." + MONEY_PRECISION(2)


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_String_Len_Const_h 
