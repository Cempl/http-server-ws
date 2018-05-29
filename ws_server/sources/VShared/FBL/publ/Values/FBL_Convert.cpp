/**********************************************************************************************/
/* FBL_Convert.cpp 	                                                      					  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Interfaces/FBL_I_Value.h>
#include <VShared/FBL/publ/Values/FBL_Convert.h>
#include <VShared/FBL/publ/Values/FBL_type_traits.h>
#include <VShared/FBL/publ/Factories/FBL_Value_Factory.h>

#include <VShared/FBL/publ/Values/FBL_Value_Enum.h>

#include <stdlib.h>
#include <stdio.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// Enum convert functions.
//
extern void Convert_str_enum	( const I_Value* inValue, I_Value* outValue );
extern void Convert_enum_str	( const I_Value* inValue, I_Value* outValue );


/**********************************************************************************************/
// Date, Time <=> DateTime convert functions.
//
extern void Convert_date_time( const I_Value* inValue, I_Value* outValue );
extern void Convert_time_date( const I_Value* inValue, I_Value* outValue );
extern void Convert_datetime_date( const I_Value* inValue, I_Value* outValue );
extern void Convert_datetime_time( const I_Value* inValue, I_Value* outValue );
extern void Convert_date_datetime( const I_Value* inValue, I_Value* outValue );
extern void Convert_time_datetime( const I_Value* inValue, I_Value* outValue );


/**********************************************************************************************/
// Date and time convert functions.
//
extern void Convert_str_date	( const I_Value* inValue, I_Value* outValue );
extern void Convert_str_time	( const I_Value* inValue, I_Value* outValue );
extern void Convert_str_datetime( const I_Value* inValue, I_Value* outValue );
extern void Convert_bin_date	( const I_Value* inValue, I_Value* outValue );
extern void Convert_bin_time	( const I_Value* inValue, I_Value* outValue );
extern void Convert_bin_datetime( const I_Value* inValue, I_Value* outValue );


/**********************************************************************************************/
// Date and time convert functions.
//
extern void Convert_date_str	( const I_Value* inValue, I_Value* outValue );
extern void Convert_time_str	( const I_Value* inValue, I_Value* outValue );
extern void Convert_datetime_str( const I_Value* inValue, I_Value* outValue );


/**********************************************************************************************/
extern void Convert_str_str		( const I_Value* inValue, I_Value* outValue );
extern void Convert_bin_str		( const I_Value* inValue, I_Value* outValue );
extern void Convert_bin_bin		( const I_Value* inValue, I_Value* outValue );


/**********************************************************************************************/
// Money convert functions.
//
extern void Convert_str_money	( const I_Value* inValue, I_Value* outValue );
extern void Convert_money_str	( const I_Value* inValue, I_Value* outValue );
extern void Convert_int_money	( const I_Value* inValue, I_Value* outValue );
extern void Convert_money_int	( const I_Value* inValue, I_Value* outValue );
extern void Convert_float_money	( const I_Value* inValue, I_Value* outValue );
extern void Convert_money_float	( const I_Value* inValue, I_Value* outValue );


/**********************************************************************************************/
// This one function do conversion for all numeric fundamental C types.
//
template<class T1, class T2>
void Convert_T_T( const I_Value* inValue, I_Value* outValue )
{
	*(T2*) outValue->begin() = (T2) *(T1*) inValue->begin();
}


#pragma mark -


/**********************************************************************************************/
template<class T> 
void Convert_str_T( const I_Value* inValue, I_Value* outValue )
{
	type_traits<T>::from_str( (const UChar*)inValue->begin(), (T*)outValue->begin() );
}


/**********************************************************************************************/
template<class T> 
void Convert_T_str( const I_Value* inValue, I_Value* outValue )  
{
	char* ps = const_cast<char*>( outValue->begin() );
	char* pe = (char*)type_traits<T>::to_str( (T*)inValue->begin(), (UChar*)ps, outValue->get_MaxChars() );
	outValue->put_ByteLength( vuint32( pe - ps ) );
}


/**********************************************************************************************/
template<class T> 
void Convert_bin_T( const I_Value* inValue, I_Value* outValue )
{
	// TODO:
	// Currently we assume inValue is a single-byted string.
	// After we make everything localizable we can improve this using IOConverter as we do
	// for string values.
	//

	type_traits<T>::from_str( inValue->begin(), (T*)outValue->begin() );
}


/**********************************************************************************************/
template<class T> 
void Convert_T_enum( const I_Value* inValue, I_Value* outValue )  
{
	outValue->put_Long( inValue->get_Long() );
}


/**********************************************************************************************/
// this macro convertion from inType type to other specified type:
// It is "general case".
// Not used for:
//	- float (specific to convert float->money)
//	- double (specific to convert double->money)
//	- date
//	- time
//	- datetime
//	- strings (string, varchar, text)
//	- binary (fixed/var binaries, blobs, picture, sound, movie)
//	- money
//
#define ARR_For(name, T)\
CONVERT_FUNC_PTR Arr_##name[39] =												\
{																				\
	NULL, 										/*  kTypeEmpty		*/			\
	Convert_T_enum<T>,							/*  kTypeEnum 		*/			\
	Convert_T_T<T, bool>,						/*	kTypeBoolean	*/			\
	Convert_T_T<T, vuint8>,						/*	kTypeByte		*/			\
	Convert_T_T<T, vint16>,						/*	kTypeShort		*/			\
	Convert_T_T<T, vuint16>,					/*	kTypeUShort		*/			\
	Convert_T_T<T, vint32>,						/*	kTypeMedium		*/			\
	Convert_T_T<T, vuint32>,					/*	kTypeUMedium	*/			\
	Convert_T_T<T, vint32>,						/*	kTypeLong		*/			\
	Convert_T_T<T, vuint32>,					/*	kTypeULong		*/			\
	Convert_T_T<T, vint64>,						/*	kTypeLLong		*/			\
	Convert_T_T<T, vuint64>,					/*	kTypeULLong		*/			\
																				\
	Convert_T_T<T, float>,						/*	kTypeFloat		*/			\
	Convert_T_T<T, double>,						/*	kTypeDouble		*/			\
	NULL,										/*	kTypeLDouble	*/			\
	Convert_T_T<T, double>,						/*	kTypeDecimal	*/			\
																				\
	Convert_T_T<T, vint32>,						/*	kTypeDate		*/			\
	Convert_T_T<T, vuint32>,					/*	kTypeTime		*/			\
	Convert_T_T<T, vint64>,						/*	kTypeDateTime	*/			\
																				\
	Convert_T_str<T>,							/*	kTypeString		*/			\
	Convert_T_str<T>,							/*	kTypeVarChar	*/			\
																				\
	NULL,										/*	kTypeFixedBinary*/			\
	NULL,										/*	kTypeVarBinary	*/			\
																				\
	NULL,										/*	kTypeBLOB		*/			\
	Convert_T_str<T>,							/*	kTypeText		*/			\
	NULL,										/*	kTypePicture	*/			\
	NULL,										/*	kTypeSound		*/			\
	NULL,										/*	kTypeMovie		*/			\
																				\
	Convert_T_T<T, vuint32>,					/*	kTypeRecID		*/			\
	Convert_T_T<T, vuint64>,					/*	kTypeOID		*/			\
	Convert_T_T<T, vuint32>,					/*	kTypeObjectPtr	*/			\
	Convert_T_T<T, vuint64>,					/*	kTypeObjectsPtr	*/			\
	Convert_T_T<T, vuint64>,					/*	kTypeTimeStamp	*/			\
	Convert_T_enum<T>,							/*  kTypeEnum8 		*/			\
	Convert_T_enum<T>,							/*  kTypeEnum16 	*/			\
																				\
	NULL,										/*	kTypeSerial32	*/			\
	NULL,										/*	kTypeSerial64	*/			\
																				\
	Convert_int_money,							/*	kTypeMoney		*/			\
	NULL										/*	kTypeVariant	*/			\
}													


/**********************************************************************************************/
ARR_For(Enum,vuint32);
ARR_For(Bool,bool);
ARR_For(Byte,vuint8);
ARR_For(Short,vint16);
ARR_For(UShort,vuint16);
ARR_For(Long,vint32);
ARR_For(ULong,vuint32);
ARR_For(LLong,vint64);
ARR_For(ULLong,vuint64);
ARR_For(Enum8,vuint8);
ARR_For(Enum16,vuint16);


/**********************************************************************************************/
// this array convertion from float to  other specified type:
// (Specific because of convertion to money type)
//
CONVERT_FUNC_PTR Arr_Float[39] =
{																				\
	nullptr, 									/*  kTypeEmpty		*/			\
	Convert_T_enum<float>,						/*  kTypeEnum 		*/			\
	Convert_T_T<float, bool>,					/*	kTypeBoolean	*/			\
	Convert_T_T<float, vuint8>,					/*	kTypeByte		*/			\
	Convert_T_T<float, vint16>,					/*	kTypeShort		*/			\
	Convert_T_T<float, vuint16>,				/*	kTypeUShort		*/			\
	Convert_T_T<float, vint32>,					/*	kTypeMedium		*/			\
	Convert_T_T<float, vuint32>,				/*	kTypeUMedium	*/			\
	Convert_T_T<float, vint32>,					/*	kTypeLong		*/			\
	Convert_T_T<float, vuint32>,				/*	kTypeULong		*/			\
	Convert_T_T<float, vint64>,					/*	kTypeLLong		*/			\
	Convert_T_T<float, vuint64>,				/*	kTypeULLong		*/			\
																				\
	Convert_T_T<float, float>,					/*	kTypeFloat		*/			\
	Convert_T_T<float, double>,					/*	kTypeDouble		*/			\
	nullptr,									/*	kTypeLDouble	*/			\
	Convert_T_T<float, double>,					/*	kTypeDecimal	*/			\
																				\
	Convert_T_T<float, vint32>,					/*	kTypeDate		*/			\
	Convert_T_T<float, vuint32>,				/*	kTypeTime		*/			\
	Convert_T_T<float, vint64>,					/*	kTypeDateTime	*/			\
																				\
	Convert_T_str<float>,						/*	kTypeString		*/			\
	Convert_T_str<float>,						/*	kTypeVarChar	*/			\
																				\
	nullptr,									/*	kTypeFixedBinary*/			\
	nullptr,									/*	kTypeVarBinary	*/			\
																				\
	nullptr,									/*	kTypeBLOB		*/			\
	Convert_T_str<float>,						/*	kTypeText		*/			\
	nullptr,									/*	kTypePicture	*/			\
	nullptr,									/*	kTypeSound		*/			\
	nullptr,									/*	kTypeMovie		*/			\
																				\
	Convert_T_T<float, vuint32>,				/*	kTypeRecID		*/			\
	Convert_T_T<float, vuint64>,				/*	kTypeOID		*/			\
	Convert_T_T<float, vuint32>,				/*	kTypeObjectPtr	*/			\
	Convert_T_T<float, vuint64>,				/*	kTypeObjectsPtr	*/			\
	Convert_T_T<float, vuint64>,				/*	kTypeTimeStamp	*/			\
	Convert_T_enum<float>,						/*  kTypeEnum8 		*/			\
	Convert_T_enum<float>,						/*  kTypeEnum16 	*/			\
																				\
	nullptr,									/*	kTypeSerial32	*/			\
	nullptr,									/*	kTypeSerial64	*/			\
																				\
	Convert_float_money,						/*	kTypeMoney		*/			\
	nullptr										/*	kTypeVariant	*/			\
};
										

/**********************************************************************************************/
// this array convertion from double to  other specified type:
// (Specific because of convertion to money type)
//
CONVERT_FUNC_PTR Arr_Double[39] =
{																				\
	nullptr, 									/*  kTypeEmpty		*/			\
	Convert_T_enum<double>,						/*  kTypeEnum 		*/			\
	Convert_T_T<double, bool>,					/*	kTypeBoolean	*/			\
	Convert_T_T<double, vuint8>,				/*	kTypeByte		*/			\
	Convert_T_T<double, vint16>,				/*	kTypeShort		*/			\
	Convert_T_T<double, vuint16>,				/*	kTypeUShort		*/			\
	Convert_T_T<double, vint32>,				/*	kTypeMedium		*/			\
	Convert_T_T<double, vuint32>,				/*	kTypeUMedium	*/			\
	Convert_T_T<double, vint32>,				/*	kTypeLong		*/			\
	Convert_T_T<double, vuint32>,				/*	kTypeULong		*/			\
	Convert_T_T<double, vint64>,				/*	kTypeLLong		*/			\
	Convert_T_T<double, vuint64>,				/*	kTypeULLong		*/			\
																				\
	Convert_T_T<double, float>,					/*	kTypeFloat		*/			\
	Convert_T_T<double, double>,				/*	kTypeDouble		*/			\
	nullptr,									/*	kTypeLDouble	*/			\
	Convert_T_T<double, double>,				/*	kTypeDecimal	*/			\
																				\
	Convert_T_T<double, vint32>,				/*	kTypeDate		*/			\
	Convert_T_T<double, vuint32>,				/*	kTypeTime		*/			\
	Convert_T_T<double, vint64>,				/*	kTypeDateTime	*/			\
																				\
	Convert_T_str<double>,						/*	kTypeString		*/			\
	Convert_T_str<double>,						/*	kTypeVarChar	*/			\
																				\
	nullptr,									/*	kTypeFixedBinary*/			\
	nullptr,									/*	kTypeVarBinary	*/			\
																				\
	nullptr,									/*	kTypeBLOB		*/			\
	Convert_T_str<double>,						/*	kTypeText		*/			\
	nullptr,									/*	kTypePicture	*/			\
	nullptr,									/*	kTypeSound		*/			\
	nullptr,									/*	kTypeMovie		*/			\
																				\
	Convert_T_T<double, vuint32>,				/*	kTypeRecID		*/			\
	Convert_T_T<double, vuint64>,				/*	kTypeOID		*/			\
	Convert_T_T<double, vuint32>,				/*	kTypeObjectPtr	*/			\
	Convert_T_T<double, vuint64>,				/*	kTypeObjectsPtr	*/			\
	Convert_T_T<double, vuint64>,				/*	kTypeTimeStamp	*/			\
	Convert_T_enum<double>,						/*  kTypeEnum8 		*/			\
	Convert_T_enum<double>,						/*  kTypeEnum16 	*/			\
																				\
	nullptr,									/*	kTypeSerial32	*/			\
	nullptr,									/*	kTypeSerial64	*/			\
																				\
	Convert_float_money,						/*	kTypeMoney		*/			\
	nullptr										/*	kTypeVariant	*/			\
};
										

/**********************************************************************************************/
// this array convertion from date to  other specified type:
//
CONVERT_FUNC_PTR Arr_Date[39] =
{																				\
	nullptr, 									/*  kTypeEmpty		*/			\
	Convert_T_enum<vint32>,						/*  kTypeEnum 		*/			\
	Convert_T_T<vint32, bool>,					/*	kTypeBoolean	*/			\
	Convert_T_T<vint32, vuint8>,				/*	kTypeByte		*/			\
	Convert_T_T<vint32, vint16>,				/*	kTypeShort		*/			\
	Convert_T_T<vint32, vuint16>,				/*	kTypeUShort		*/			\
	Convert_T_T<vint32, vint32>,				/*	kTypeMedium		*/			\
	Convert_T_T<vint32, vuint32>,				/*	kTypeUMedium	*/			\
	Convert_T_T<vint32, vint32>,				/*	kTypeLong		*/			\
	Convert_T_T<vint32, vuint32>,				/*	kTypeULong		*/			\
	Convert_T_T<vint32, vint64>,				/*	kTypeLLong		*/			\
	Convert_T_T<vint32, vuint64>,				/*	kTypeULLong		*/			\
																				\
	Convert_T_T<vint32, float>,					/*	kTypeFloat		*/			\
	Convert_T_T<vint32, double>,				/*	kTypeDouble		*/			\
	nullptr,									/*	kTypeLDouble	*/			\
	Convert_T_T<vint32, double>,				/*	kTypeDecimal	*/			\
																				\
	Convert_T_T<vint32, vint32>,				/*	kTypeDate		*/			\
	Convert_date_time,							/*	kTypeTime		*/			\
	Convert_date_datetime,						/*	kTypeDateTime	*/			\
																				\
	Convert_date_str,							/*	kTypeString		*/			\
	Convert_date_str,							/*	kTypeVarChar	*/			\
																				\
	nullptr,									/*	kTypeFixedBinary*/			\
	nullptr,									/*	kTypeVarBinary	*/			\
																				\
	nullptr,									/*	kTypeBLOB		*/			\
	Convert_date_str,							/*	kTypeText		*/			\
	nullptr,									/*	kTypePicture	*/			\
	nullptr,									/*	kTypeSound		*/			\
	nullptr,									/*	kTypeMovie		*/			\
																				\
	Convert_T_T<vint32, vuint32>,				/*	kTypeRecID		*/			\
	Convert_T_T<vint32, vuint64>,				/*	kTypeOID		*/			\
	Convert_T_T<vint32, vuint32>,				/*	kTypeObjectPtr	*/			\
	Convert_T_T<vint32, vuint64>,				/*	kTypeObjectsPtr	*/			\
	Convert_T_T<vint32, vuint64>,				/*	kTypeTimeStamp	*/			\
	Convert_T_enum<vint32>,						/*  kTypeEnum8 		*/			\
	Convert_T_enum<vint32>,						/*  kTypeEnum16 	*/			\
																				\
	nullptr,									/*	kTypeSerial32	*/			\
	nullptr,									/*	kTypeSerial64	*/			\
																				\
	nullptr,									/*	kTypeMoney		*/			\
	nullptr										/*	kTypeVariant	*/			\
};													
										

/**********************************************************************************************/
// this array convertion from time to  other specified type:
//
CONVERT_FUNC_PTR Arr_Time[39] =
{																				\
	nullptr, 									/*  kTypeEmpty		*/			\
	Convert_T_enum<vuint32>,					/*  kTypeEnum 		*/			\
	Convert_T_T<vuint32, bool>,					/*	kTypeBoolean	*/			\
	Convert_T_T<vuint32, vuint8>,				/*	kTypeByte		*/			\
	Convert_T_T<vuint32, vint16>,				/*	kTypeShort		*/			\
	Convert_T_T<vuint32, vuint16>,				/*	kTypeUShort		*/			\
	Convert_T_T<vuint32, vint32>,				/*	kTypeMedium		*/			\
	Convert_T_T<vuint32, vuint32>,				/*	kTypeUMedium	*/			\
	Convert_T_T<vuint32, vint32>,				/*	kTypeLong		*/			\
	Convert_T_T<vuint32, vuint32>,				/*	kTypeULong		*/			\
	Convert_T_T<vuint32, vint64>,				/*	kTypeLLong		*/			\
	Convert_T_T<vuint32, vuint64>,				/*	kTypeULLong		*/			\
																				\
	Convert_T_T<vuint32, float>,				/*	kTypeFloat		*/			\
	Convert_T_T<vuint32, double>,				/*	kTypeDouble		*/			\
	nullptr,									/*	kTypeLDouble	*/			\
	Convert_T_T<vuint32, double>,				/*	kTypeDecimal	*/			\
																				\
	Convert_time_date,							/*	kTypeDate		*/			\
	Convert_T_T<vuint32, vuint32>,				/*	kTypeTime		*/			\
	Convert_time_datetime,						/*	kTypeDateTime	*/			\
																				\
	Convert_time_str,							/*	kTypeString		*/			\
	Convert_time_str,							/*	kTypeVarChar	*/			\
																				\
	nullptr,									/*	kTypeFixedBinary*/			\
	nullptr,									/*	kTypeVarBinary	*/			\
																				\
	nullptr,									/*	kTypeBLOB		*/			\
	Convert_time_str,							/*	kTypeText		*/			\
	nullptr,									/*	kTypePicture	*/			\
	nullptr,									/*	kTypeSound		*/			\
	nullptr,									/*	kTypeMovie		*/			\
																				\
	Convert_T_T<vuint32, vuint32>,				/*	kTypeRecID		*/			\
	Convert_T_T<vuint32, vuint64>,				/*	kTypeOID		*/			\
	Convert_T_T<vuint32, vuint32>,				/*	kTypeObjectPtr	*/			\
	Convert_T_T<vuint32, vuint64>,				/*	kTypeObjectsPtr	*/			\
	Convert_T_T<vuint32, vuint64>,				/*	kTypeTimeStamp	*/			\
	Convert_T_enum<vuint32>,					/*  kTypeEnum8 		*/			\
	Convert_T_enum<vuint32>,					/*  kTypeEnum16 	*/			\
																				\
	nullptr,									/*	kTypeSerial32	*/			\
	nullptr,									/*	kTypeSerial64	*/			\
																				\
	nullptr,									/*	kTypeMoney		*/			\
	nullptr										/*	kTypeVariant	*/			\
};													
										

/**********************************************************************************************/
// this array convertion from datetime to  other specified type:
//
CONVERT_FUNC_PTR Arr_DateTime[39] =
{																				\
	nullptr, 									/*  kTypeEmpty		*/			\
	Convert_T_enum<vint64>,						/*  kTypeEnum 		*/			\
	Convert_T_T<vint64, bool>,					/*	kTypeBoolean	*/			\
	Convert_T_T<vint64, vuint8>,				/*	kTypeByte		*/			\
	Convert_T_T<vint64, vint16>,				/*	kTypeShort		*/			\
	Convert_T_T<vint64, vuint16>,				/*	kTypeUShort		*/			\
	Convert_T_T<vint64, vint32>,				/*	kTypeMedium		*/			\
	Convert_T_T<vint64, vuint32>,				/*	kTypeUMedium	*/			\
	Convert_T_T<vint64, vint32>,				/*	kTypeLong		*/			\
	Convert_T_T<vint64, vuint32>,				/*	kTypeULong		*/			\
	Convert_T_T<vint64, vint64>,				/*	kTypeLLong		*/			\
	Convert_T_T<vint64, vuint64>,				/*	kTypeULLong		*/			\
																				\
	Convert_T_T<vint64, float>,					/*	kTypeFloat		*/			\
	Convert_T_T<vint64, double>,				/*	kTypeDouble		*/			\
	nullptr,									/*	kTypeLDouble	*/			\
	Convert_T_T<vint64, double>,				/*	kTypeDecimal	*/			\
																				\
	Convert_datetime_date,						/*	kTypeDate		*/			\
	Convert_datetime_time,						/*	kTypeTime		*/			\
	Convert_T_T<vint64, vint64>,				/*	kTypeDateTime	*/			\
																				\
	Convert_datetime_str,						/*	kTypeString		*/			\
	Convert_datetime_str,						/*	kTypeVarChar	*/			\
																				\
	nullptr,									/*	kTypeFixedBinary*/			\
	nullptr,									/*	kTypeVarBinary	*/			\
																				\
	nullptr,									/*	kTypeBLOB		*/			\
	Convert_datetime_str,						/*	kTypeText		*/			\
	nullptr,									/*	kTypePicture	*/			\
	nullptr,									/*	kTypeSound		*/			\
	nullptr,									/*	kTypeMovie		*/			\
																				\
	Convert_T_T<vint64, vuint32>,				/*	kTypeRecID		*/			\
	Convert_T_T<vint64, vuint64>,				/*	kTypeOID		*/			\
	Convert_T_T<vint64, vuint32>,				/*	kTypeObjectPtr	*/			\
	Convert_T_T<vint64, vuint64>,				/*	kTypeObjectsPtr	*/			\
	Convert_T_T<vint64, vuint64>,				/*	kTypeTimeStamp	*/			\
	Convert_T_enum<vint64>,						/*  kTypeEnum8 		*/			\
	Convert_T_enum<vint64>,						/*  kTypeEnum16 	*/			\
																				\
	nullptr,									/*	kTypeSerial32	*/			\
	nullptr,									/*	kTypeSerial64	*/			\
																				\
	nullptr,									/*	kTypeMoney		*/			\
	nullptr										/*	kTypeVariant	*/			\
};													
										

/**********************************************************************************************/
// this array convertion from string to other specified type:
//
CONVERT_FUNC_PTR Arr_Str[39] =
{																				\
	nullptr, 									/*  kTypeEmpty		*/			\
	Convert_str_enum,							/*  kTypeEnum 		*/			\
	Convert_str_T<bool>,						/*	kTypeBoolean	*/			\
	Convert_str_T<vuint8>,						/*	kTypeByte		*/			\
	Convert_str_T<vint16>,						/*	kTypeShort		*/			\
	Convert_str_T<vuint16>,						/*	kTypeUShort		*/			\
	Convert_str_T<vint32>,						/*	kTypeMedium		*/			\
	Convert_str_T<vuint32>,						/*	kTypeUMedium	*/			\
	Convert_str_T<vint32>,						/*	kTypeLong		*/			\
	Convert_str_T<vuint32>,						/*	kTypeULong		*/			\
	Convert_str_T<vint64>,						/*	kTypeLLong		*/			\
	Convert_str_T<vuint64>,						/*	kTypeULLong		*/			\
																				\
	Convert_str_T<float>,						/*	kTypeFloat		*/			\
	Convert_str_T<double>,						/*	kTypeDouble		*/			\
	nullptr,									/*	kTypeLDouble	*/			\
	Convert_str_T<double>,						/*	kTypeDecimal	*/			\
																				\
	Convert_str_date,							/*	kTypeDate		*/			\
	Convert_str_time,							/*	kTypeTime		*/			\
	Convert_str_datetime,						/*	kTypeDateTime	*/			\
																				\
	Convert_str_str,							/*	kTypeString		*/			\
	Convert_str_str,							/*	kTypeVarChar	*/			\
																				\
	nullptr,									/*	kTypeFixedBinary*/			\
	nullptr,									/*	kTypeVarBinary	*/			\
																				\
	nullptr,									/*	kTypeBLOB		*/			\
	Convert_str_str,							/*	kTypeText		*/			\
	nullptr,									/*	kTypePicture	*/			\
	nullptr,									/*	kTypeSound		*/			\
	nullptr,									/*	kTypeMovie		*/			\
																				\
	Convert_str_T<vuint32>,						/*	kTypeRecID		*/			\
	Convert_str_T<vuint64>,						/*	kTypeOID		*/			\
	Convert_str_T<vuint32>,						/*	kTypeObjectPtr	*/			\
	Convert_str_T<vuint64>,						/*	kTypeObjectsPtr	*/			\
	Convert_str_T<vuint64>,						/*	kTypeTimeStamp	*/			\
	Convert_str_enum,							/*  kTypeEnum8 		*/			\
	Convert_str_enum,							/*  kTypeEnum16 	*/			\
																				\
	nullptr,									/*	kTypeSerial32	*/			\
	nullptr,									/*	kTypeSerial64	*/			\
																				\
	Convert_str_money,							/*	kTypeMoney		*/			\
	nullptr										/*	kTypeVariant	*/			\
};													
										

/**********************************************************************************************/
// this array convertion from binary to other specified type:
//
CONVERT_FUNC_PTR Arr_Bin[39] =
{																				\
	nullptr, 									/*  kTypeEmpty		*/			\
	Convert_str_enum,							/*  kTypeEnum 		*/			\
	Convert_bin_T<bool>,						/*	kTypeBoolean	*/			\
	Convert_bin_T<vuint8>,						/*	kTypeByte		*/			\
	Convert_bin_T<vint16>,						/*	kTypeShort		*/			\
	Convert_bin_T<vuint16>,						/*	kTypeUShort		*/			\
	Convert_bin_T<vint32>,						/*	kTypeMedium		*/			\
	Convert_bin_T<vuint32>,						/*	kTypeUMedium	*/			\
	Convert_bin_T<vint32>,						/*	kTypeLong		*/			\
	Convert_bin_T<vuint32>,						/*	kTypeULong		*/			\
	Convert_bin_T<vint64>,						/*	kTypeLLong		*/			\
	Convert_bin_T<vuint64>,						/*	kTypeULLong		*/			\
																				\
	Convert_bin_T<float>,						/*	kTypeFloat		*/			\
	Convert_bin_T<double>,						/*	kTypeDouble		*/			\
	nullptr,									/*	kTypeLDouble	*/			\
	Convert_bin_T<double>,						/*	kTypeDecimal	*/			\
																				\
	Convert_bin_date,							/*	kTypeDate		*/			\
	Convert_bin_time,							/*	kTypeTime		*/			\
	Convert_bin_datetime,						/*	kTypeDateTime	*/			\
																				\
	Convert_bin_str,							/*	kTypeString		*/			\
	Convert_bin_str,							/*	kTypeVarChar	*/			\
																				\
	Convert_bin_bin,							/*	kTypeFixedBinary*/			\
	Convert_bin_bin,							/*	kTypeVarBinary	*/			\
																				\
	nullptr,									/*	kTypeBLOB		*/			\
	Convert_bin_str,							/*	kTypeText		*/			\
	nullptr,									/*	kTypePicture	*/			\
	nullptr,									/*	kTypeSound		*/			\
	nullptr,									/*	kTypeMovie		*/			\
																				\
	Convert_bin_T<vuint32>,						/*	kTypeRecID		*/			\
	Convert_bin_T<vuint64>,						/*	kTypeOID		*/			\
	Convert_bin_T<vuint32>,						/*	kTypeObjectPtr	*/			\
	Convert_bin_T<vuint64>,						/*	kTypeObjectsPtr	*/			\
	Convert_bin_T<vuint64>,						/*	kTypeTimeStamp	*/			\
	Convert_str_enum,							/*  kTypeEnum8 		*/			\
	Convert_str_enum,							/*  kTypeEnum16 	*/			\
																				\
	nullptr,									/*	kTypeSerial32	*/			\
	nullptr,									/*	kTypeSerial64	*/			\
																				\
	nullptr,									/*	kTypeMoney		*/			\
	nullptr										/*	kTypeVariant	*/			\
};													
										

/**********************************************************************************************/
// this array convertion from money to other specified type:
//
CONVERT_FUNC_PTR Arr_Money[39] =
{																				\
	nullptr, 									/*  kTypeEmpty		*/			\
	Convert_money_int,							/*  kTypeEnum 		*/			\
	Convert_money_int,							/*	kTypeBoolean	*/			\
	Convert_money_int,							/*	kTypeByte		*/			\
	Convert_money_int,							/*	kTypeShort		*/			\
	Convert_money_int,							/*	kTypeUShort		*/			\
	Convert_money_int,							/*	kTypeMedium		*/			\
	Convert_money_int,							/*	kTypeUMedium	*/			\
	Convert_money_int,							/*	kTypeLong		*/			\
	Convert_money_int,							/*	kTypeULong		*/			\
	Convert_money_int,							/*	kTypeLLong		*/			\
	Convert_money_int,							/*	kTypeULLong		*/			\
																				\
	Convert_money_float,						/*	kTypeFloat		*/			\
	Convert_money_float,						/*	kTypeDouble		*/			\
	Convert_money_float,						/*	kTypeLDouble	*/			\
	Convert_money_float,						/*	kTypeDecimal	*/			\
																				\
	nullptr,									/*	kTypeDate		*/			\
	nullptr,									/*	kTypeTime		*/			\
	nullptr,									/*	kTypeDateTime	*/			\
																				\
	Convert_money_str,							/*	kTypeString		*/			\
	Convert_money_str,							/*	kTypeVarChar	*/			\
																				\
	nullptr,									/*	kTypeFixedBinary*/			\
	nullptr,									/*	kTypeVarBinary	*/			\
																				\
	nullptr,									/*	kTypeBLOB		*/			\
	Convert_money_str,							/*	kTypeText		*/			\
	nullptr,									/*	kTypePicture	*/			\
	nullptr,									/*	kTypeSound		*/			\
	nullptr,									/*	kTypeMovie		*/			\
																				\
	Convert_money_int,							/*	kTypeRecID		*/			\
	Convert_money_int,							/*	kTypeOID		*/			\
	Convert_money_int,							/*	kTypeObjectPtr	*/			\
	Convert_money_int,							/*	kTypeObjectsPtr	*/			\
	Convert_money_int,							/*	kTypeTimeStamp	*/			\
	Convert_money_int,							/*  kTypeEnum8 		*/			\
	Convert_money_int,							/*  kTypeEnum16 	*/			\
																				\
	nullptr,									/*	kTypeSerial32	*/			\
	nullptr,									/*	kTypeSerial64	*/			\
																				\
	Convert_T_T<vint64, vint64>,				/*	kTypeMoney		*/			\
	nullptr										/*	kTypeVariant	*/			\
};													
										

/**********************************************************************************************/
// this array convertion from variant to other specified type:
//
CONVERT_FUNC_PTR Arr_Variant[39] =
{																				\
	nullptr, 									/*  kTypeEmpty		*/			\
	Convert_str_enum,							/*  kTypeEnum 		*/			\
	Convert_bin_T<bool>,						/*	kTypeBoolean	*/			\
	Convert_bin_T<vuint8>,						/*	kTypeByte		*/			\
	Convert_bin_T<vint16>,						/*	kTypeShort		*/			\
	Convert_bin_T<vuint16>,						/*	kTypeUShort		*/			\
	Convert_bin_T<vint32>,						/*	kTypeMedium		*/			\
	Convert_bin_T<vuint32>,						/*	kTypeUMedium	*/			\
	Convert_bin_T<vint32>,						/*	kTypeLong		*/			\
	Convert_bin_T<vuint32>,						/*	kTypeULong		*/			\
	Convert_bin_T<vint64>,						/*	kTypeLLong		*/			\
	Convert_bin_T<vuint64>,						/*	kTypeULLong		*/			\
																				\
	Convert_bin_T<float>,						/*	kTypeFloat		*/			\
	Convert_bin_T<double>,						/*	kTypeDouble		*/			\
	nullptr,									/*	kTypeLDouble	*/			\
	Convert_bin_T<double>,						/*	kTypeDecimal	*/			\
																				\
	Convert_bin_date,							/*	kTypeDate		*/			\
	Convert_bin_time,							/*	kTypeTime		*/			\
	Convert_bin_datetime,						/*	kTypeDateTime	*/			\
																				\
	Convert_bin_str,							/*	kTypeString		*/			\
	Convert_bin_str,							/*	kTypeVarChar	*/			\
																				\
	nullptr,									/*	kTypeFixedBinary*/			\
	nullptr,									/*	kTypeVarBinary	*/			\
																				\
	nullptr,									/*	kTypeBLOB		*/			\
	Convert_bin_str,							/*	kTypeText		*/			\
	nullptr,									/*	kTypePicture	*/			\
	nullptr,									/*	kTypeSound		*/			\
	nullptr,									/*	kTypeMovie		*/			\
																				\
	Convert_bin_T<vuint32>,						/*	kTypeRecID		*/			\
	Convert_bin_T<vuint64>,						/*	kTypeOID		*/			\
	Convert_bin_T<vuint32>,						/*	kTypeObjectPtr	*/			\
	Convert_bin_T<vuint64>,						/*	kTypeObjectsPtr	*/			\
	Convert_bin_T<vuint64>,						/*	kTypeTimeStamp	*/			\
	Convert_str_enum,							/*  kTypeEnum8 		*/			\
	Convert_str_enum,							/*  kTypeEnum16 	*/			\
																				\
	nullptr,									/*	kTypeSerial32	*/			\
	nullptr,									/*	kTypeSerial64	*/			\
																				\
	nullptr,									/*	kTypeMoney		*/			\
	nullptr										/*	kTypeVariant	*/			\
};													
										

/**********************************************************************************************/
// this is Matrix of convertion from all type to all type:
//
CONVERT_FUNC_PTR* Arr_T_T[39] =
{																				
	nullptr, 									/*  kTypeEmpty		*/
	Arr_Enum,									/*  kTypeEnum 		*/					
	Arr_Bool,									/*	kTypeBoolean	*/			
	Arr_Byte,									/*	kTypeByte		*/			
	Arr_Short,									/*	kTypeShort		*/			
	Arr_UShort,									/*	kTypeUShort		*/			
	Arr_Long,									/*	kTypeMedium		*/			
	Arr_ULong,									/*	kTypeUMedium	*/			
	Arr_Long,									/*	kTypeLong		*/			
	Arr_ULong,									/*	kTypeULong		*/			
	Arr_LLong,									/*	kTypeLLong		*/			
	Arr_ULLong,									/*	kTypeULLong		*/			
																				
	Arr_Float,									/*	kTypeFloat		*/			
	Arr_Double,									/*	kTypeDouble		*/			
	nullptr,									/*	kTypeLDouble	*/
	Arr_Double,									/*	kTypeDecimal	*/			
																				
	Arr_Date,									/*	kTypeDate		*/			
	Arr_Time,									/*	kTypeTime		*/			
	Arr_DateTime,								/*	kTypeDateTime	*/			
																				
	Arr_Str,									/*	kTypeString		*/			
	Arr_Str,									/*	kTypeVarChar	*/			
																				
	Arr_Bin,									/*	kTypeFixedBinary*/			
	Arr_Bin,									/*	kTypeVarBinary	*/			
																				
	nullptr,									/*	kTypeBLOB		*/
	Arr_Str,									/*	kTypeText		*/			
	nullptr,									/*	kTypePicture	*/
	nullptr,									/*	kTypeSound		*/
	nullptr,									/*	kTypeMovie		*/
																				
	Arr_ULong,									/*	kTypeRecID		*/			
	Arr_ULLong,									/*	kTypeOID		*/			
	Arr_ULong,									/*	kTypeObjectPtr	*/			
	Arr_ULLong,									/*	kTypeObjectsPtr	*/			
	Arr_ULLong,									/*	kTypeTimeStamp	*/

	Arr_Enum8,									/*  kTypeEnum8 		*/	
	Arr_Enum16,									/*  kTypeEnum16		*/

	nullptr,									/*	kTypeSerial32	*/
	nullptr,									/*	kTypeSerial64	*/
	
	Arr_Money,									/*	kTypeMoney		*/
	Arr_Variant									/*	kTypeVariant	*/
};													
										

#pragma mark -
 

/**********************************************************************************************/
bool ConvertValue(
	vuint32 		inValueType,
	vuint32 		outValueType,
	const I_Value*	inValue,
		  I_Value*	outValue )
{
	CONVERT_FUNC_PTR* ppConvertFunc = Arr_T_T[inValueType];
	
	if( ppConvertFunc )
	{
		CONVERT_FUNC_PTR pConvertFunc = ppConvertFunc[outValueType];
	
		if( pConvertFunc )
		{
			pConvertFunc( inValue, outValue );
			return true;
		}
		else
		{
			// No convert particularly for inValueType to outValueType
			FBL_CHECK( pConvertFunc != nullptr );
			return false;
		}
	}
	else
	{
		// No convert for inValueType at all.
		return false;
	}	
}


/**********************************************************************************************/
bool ConvertValue(
	const I_Value*	inValue,
		  I_Value*	outValue )
{
	return ConvertValue( 
				inValue->get_Type(), outValue->get_Type(),
				inValue, 			outValue );
}


/**********************************************************************************************/
// if inValue is NULL, then we create new value with Nullable flag.
// New value on construction get NULL value.
//
I_Value_Ptr ConvertValue(
	const I_Value& 	inValue,
	VALUE_TYPE 		inResultType )
{
	bool IsNull = inValue.get_IsNull();

	// Create Value of required type:
	FieldAttributes flags = IsNull ? fNullable : fNone;

	void* pFirstParam = nullptr;
	tslen Len = 0;
	VALUE_CATEGORY val_Category = inValue.get_Category();
	switch( val_Category )
	{
		case vcString:
		case vcBinary:
		{
			Len = static_cast<tslen>(inValue.get_Length());
			pFirstParam = &Len;
		} break;

		case vcDate:
		case vcTime:
		case vcDateTime:
		{
			pFirstParam = nullptr;
		} break;
		
		default:
		{
			Len = static_cast<tslen>(inValue.get_MaxChars());
			pFirstParam = &Len;
		} break;
	}

	I_Value_Ptr result = internal::CreateValue( inResultType, static_cast<vuint16>(flags), pFirstParam );

	if( inValue.get_IsRemote() )
		result->put_IsRemote( true );

	if( IsNull )
		return result; // it is 'NULL' value.

	if( result )
	{
		if( ConvertValue( &inValue, result.get() ) )
			result->put_IsNull( false );
// IS: 7.04.2012
// We must not delete new value because of callers which mostly assumes that some value was 
// returned from this function.
// We should rather leave the result value in nullable state or throw some exception.
//		else
//			result.SetNull(); // delete new value.
			
	}

	return result;
}


#pragma mark -


/**********************************************************************************************/
I_Value_Ptr ConvertValue(
	const I_Value&			inValue,
	I_Type_Enumerated_Ptr	inResultType )
{
	bool IsNull = inValue.get_IsNull();
	FieldAttributes flags = IsNull ? fNullable : fNone;

	I_Value_Ptr result = internal::CreateValue_Enum( inResultType, static_cast<vuint16>(flags) );

	if( result && inValue.get_IsRemote() )
		result->put_IsRemote( true );

	if( IsNull )
		return result; // it is 'NULL' value.

	if( result )
	{
		if( ConvertValue( &inValue, result.get() ) )
			result->put_IsNull( false );
		else
			result.SetNull(); // delete new value.
	}

	return result;
}


#pragma mark -


/**********************************************************************************************/
TypeConverter::TypeConverter( vuint32 inTypeA, vuint32 inTypeB ) 
{
	mFuncPtr = Arr_T_T[inTypeA][inTypeB];
}


/**********************************************************************************************/
int SmartCompare( Const_I_Value_Ptr inFirstVal, Const_I_Value_Ptr inSecondVal )
{		
	int result;
				
	VALUE_TYPE frstValueType = inFirstVal->get_Type();
	VALUE_TYPE scndValueType = inSecondVal->get_Type();
	
	if( frstValueType != scndValueType )
	{
		if( frstValueType == kTypeEnum8 || frstValueType == kTypeEnum16 )
		{
			// If first value is enum - convert second value to enum also.
			//
			I_Type_Enumerated_Ptr pEnumType;
			if( frstValueType == kTypeEnum8 )
			{
				const Value_enum8* pEnumVal = dynamic_cast<const Value_enum8*>( inFirstVal.get() );
				FBL_CHECK( pEnumVal );
				pEnumType = pEnumVal->get_EnumType();
			}
			else
			{
				FBL_CHECK( frstValueType == kTypeEnum16 );
				const Value_enum16* pEnumVal = dynamic_cast<const Value_enum16*>( inFirstVal.get() );
				FBL_CHECK( pEnumVal );
				pEnumType = pEnumVal->get_EnumType();
			}

			I_Value_Ptr	pConvertedValue = ConvertValue( *inSecondVal, pEnumType );
			result = inFirstVal->Compare( *pConvertedValue );
		}
		else if( scndValueType == kTypeEnum8 || scndValueType == kTypeEnum16 )
		{
			// If second value is enum - convert first value to enum also.
			//
			I_Type_Enumerated_Ptr pEnumType;
			if( scndValueType == kTypeEnum8 )
			{
				const Value_enum8* pEnumVal = dynamic_cast<const Value_enum8*>( inSecondVal.get() );
				FBL_CHECK( pEnumVal );
				pEnumType = pEnumVal->get_EnumType();
			}
			else
			{
				FBL_CHECK( scndValueType == kTypeEnum16 );
				const Value_enum16* pEnumVal = dynamic_cast<const Value_enum16*>( inSecondVal.get() );
				FBL_CHECK( pEnumVal );
				pEnumType = pEnumVal->get_EnumType();
			}

			I_Value_Ptr pConvertedValue = ConvertValue( *inSecondVal, pEnumType );
			result = pConvertedValue->Compare( *inSecondVal );					
		}
		else
		{
			// Convert "least" value to "greatest" first
			// Because Compare does not care about types - it must be just the same.
			//
			if( inFirstVal->get_MaxChars() > inSecondVal->get_MaxChars() )
			{
				I_Value_Ptr pConvertedValue = ConvertValue( *inSecondVal, frstValueType );		
				result = inFirstVal->Compare( *pConvertedValue );
			}
			else
			{
				I_Value_Ptr pConvertedValue = ConvertValue( *inFirstVal, scndValueType );
				result = pConvertedValue->Compare( *inSecondVal );					
			}
		}
	}
	else
	{
		result = inFirstVal->Compare( *inSecondVal );
	}
	
	return result;
}


/**********************************************************************************************/
CONVERT_FUNC_PTR FBL_SHARED_EXP get_Convertor( vuint32 inValueType, vuint32 outValueType )
{
	if( Arr_T_T[inValueType] == nullptr )
		return nullptr;
	else
		return Arr_T_T[inValueType][outValueType];
}


/**********************************************************************************************/
FBL_End_Namespace
