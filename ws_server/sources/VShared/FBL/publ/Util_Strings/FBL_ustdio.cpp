/**********************************************************************************************/
/* FBL_ustdio.cpp                                                          					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Util_Strings/FBL_ustdio.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// detectUnicodeSignature
vu_detectUnicodeSignature_ptr pvu_detectUnicodeSignature;

// is...
vu_isalpha_ptr 		pvu_isalpha;
vu_isdigit_ptr 		pvu_isdigit;
vu_isspace_ptr 		pvu_isspace;

// sprintf
vu_sprintf_ptr		pvu_sprintf;

// strchr
vu_strchr_ptr		pvu_strchr;
vu_strrchr_ptr		pvu_strrchr;

// strcmp
vu_strcmp_uu_ptr	pvu_strcmp_uu;
vu_strcmp_ua_ptr	pvu_strcmp_ua;
vu_strcmp_au_ptr	pvu_strcmp_au;

// strcoll
vu_strcoll_uu_ptr	pvu_strcoll_uu;
vu_strcoll_ss_ptr	pvu_strcoll_ss;

// strcpy
vu_strcpy_uu_ptr	pvu_strcpy_uu;
vu_strcpy_ua_ptr	pvu_strcpy_ua;

// stricmp
vu_stricmp_uu_ptr	pvu_stricmp_uu;
vu_stricmp_ua_ptr	pvu_stricmp_ua;
vu_stricmp_au_ptr	pvu_stricmp_au;

// strlen
vu_strlen_ptr 		pvu_strlen;

// strncat
vu_strncat_ptr		pvu_strncat;

// strncmp
vu_strncmp_uu_ptr	pvu_strncmp_uu;
vu_strncmp_ua_ptr	pvu_strncmp_ua;
vu_strncmp_au_ptr	pvu_strncmp_au;

// strncpy
vu_strncpy_ptr		pvu_strncpy;

// vu_u2a, vu_a2u
vu_u2a_ptr			pvu_u2a;
vu_a2u_ptr			pvu_a2u;

// Unicode <> Default Code Page
vu_FromUnicodeToDefaultCP_ptr	pFromUnicodeToDefaultCP;
vu_FromDefaultCPToUnicode_ptr	pFromDefaultCPToUnicode;

// UChar <=> wchar_t
vu_strFromWCS_ptr	pvu_strFromWCS;
vu_strToWCS_ptr		pvu_strToWCS;

// toUpper
vu_toupper_ptr		pvu_toupper;


/**********************************************************************************************/
FBL_End_Namespace
