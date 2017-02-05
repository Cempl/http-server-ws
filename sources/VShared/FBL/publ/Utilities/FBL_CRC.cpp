/**********************************************************************************************/
/* FBL_CRC.cpp																				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Utilities/FBL_CRC.h>


/**********************************************************************************************/
FBL_Begin_Namespace

/**********************************************************************************************/
vuint32 crc32B( 
	const vuint8*	inpData, 
	unsigned		inLen, 
	vuint32			inCRC )
{
    inCRC = ~inCRC;

    vuint8 octet;

	unsigned i, j;
    for( i = 0; i < inLen; i++ )
    {
        octet = *(inpData++);
        for( j = 0; j < 8; j++ )
        {
            if( (octet >> 7) ^ (inCRC >> 31) )
                inCRC = (inCRC << 1) ^ CRC32_QUOTIENT;
            else
                inCRC = (inCRC << 1);
				
            octet <<= 1;
        }
    }

    return ~inCRC;             /* The complement of the remainder */
}


/**********************************************************************************************/
vuint32 crc32DW( 
	const vuint8*	inpData, 
	unsigned		inLen, 
	vuint32			inCRC )
{
static CRC32_Table table;
	
	vuint32* pCRCTable = table.mpCRC_Table;

    vuint32* p = (vuint32*)(inpData);
    vuint32* e = (vuint32*)(inpData + inLen);

	inCRC = HTON32( ~inCRC );

    while( p < e )
    {
        inCRC ^= *p++;

#if FBL_LITTLE_ENDIAN
        inCRC = pCRCTable[inCRC & 0xff] ^ inCRC >> 8;
        inCRC = pCRCTable[inCRC & 0xff] ^ inCRC >> 8;
        inCRC = pCRCTable[inCRC & 0xff] ^ inCRC >> 8;
        inCRC = pCRCTable[inCRC & 0xff] ^ inCRC >> 8;
#else
        inCRC = pCRCTable[inCRC >> 24] ^ inCRC << 8;
        inCRC = pCRCTable[inCRC >> 24] ^ inCRC << 8;
        inCRC = pCRCTable[inCRC >> 24] ^ inCRC << 8;
        inCRC = pCRCTable[inCRC >> 24] ^ inCRC << 8;
#endif // FBL_LITTLE_ENDIAN

    }

    return HTON32( ~inCRC );
}


#pragma mark -

/**********************************************************************************************/
vuint32 CRC32( 
	const vuint8*	inpData, 
	unsigned		inLen )

{
	vuint32 res = 0;
	
	unsigned len1 = inLen & 0x03;
	unsigned len2 = inLen - len1;

    if( len2 > 0 ) 
    	res = crc32DW( inpData, len2, res );

    if( len1 > 0 ) 
    	res = crc32B( inpData+len2, len1, res );
	
	return res;
}


/**********************************************************************************************/
FBL_End_Namespace
