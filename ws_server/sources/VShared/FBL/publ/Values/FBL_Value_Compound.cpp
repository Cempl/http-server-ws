/**********************************************************************************************/
/* FBL_Value_Compound.cpp                                                    				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#include <VShared/FBL/publ/Values/FBL_Value_Compound.h>


/**********************************************************************************************/
FBL_Begin_Namespace


#pragma mark -
#pragma mark === Value_Compound ===


/**********************************************************************************************/
Value_Compound::Value_Compound( 
	ArrayOfValues_Ptr	inOriginalValues ) 
:
	mpValues( new ArrayOfValues() ),
    mpSizes ( new ArrayOfULongs() ),
    mIsVariable( false )
{
	InitWithArray( inOriginalValues, false );
}


/**********************************************************************************************/
Value_Compound::Value_Compound( 
	const Value_Compound& inOther )  
:
	mpValues( new ArrayOfValues() ),
    mpSizes ( new ArrayOfULongs() ),
    mIsVariable( false )
{
	InitWithArray( inOther.mpValues, false );
}


#pragma mark -


/**********************************************************************************************/
// HELPER method
//
void Value_Compound::InitWithArray( 
	ArrayOfValues_Ptr 	inValues,
    bool				inCopyData )
{
	if( inValues )
	{
        vuint32 Count = inValues->get_Count();
        for( vuint32 i = 1; i <= Count ; ++i )
        {
            I_Value_Ptr pSubValue = inValues->get_ItemAt( i );
            mpValues->AddItem( pSubValue->Clone( inCopyData ) );
            
            vuint32 size = CalcSizeOfSubValue( pSubValue );
            mpSizes->AddItem( size );
            
            if( size == 0 ) // i.e. string or binary category.
            	mIsVariable = true;
        } 
	}
}


/**********************************************************************************************/
vuint32 Value_Compound::CalcSizeOfSubValue( 
	I_Value_Ptr 		inSubValue )
{
	vuint32 res = 0;

	switch( inSubValue->get_Category() ) 
    {
        case vcInteger:
        case vcUInteger:

		case vcReal:

        case vcDate:
        case vcTime:
        case vcDateTime:

        case vcEnum:
		
		case vcMoney:
        {
			res = inSubValue->get_StorageSize();
        }break;
    
		case vcString:
        case vcBinary:
        {
        	res = 0; // will be asked at runtime 
        }break;
                    
        default:
        {
        	// Some code before should check that sub-values are legal
            // and throw error otherwise. We cannot throw from ctor of this class.
        	CHECK( false );
        }break;
	}	

	return res;
}


#pragma mark -
#pragma mark === I_Value / Construction ===


/**********************************************************************************************/
// Virtual Constructor.
//
I_Value_Ptr Value_Compound::Clone( bool inCopyData ) const
{
	Value_Compound_Ptr pNewValue = new Value_Compound( nullptr );
    
    pNewValue->InitWithArray( mpValues, inCopyData );
    
    return pNewValue;
}


#pragma mark -
#pragma mark === I_Value / Properties ===


/**********************************************************************************************/
vuint32 Value_Compound::get_Allocated( void ) const
{
	vuint32 res = 0;

	vuint32 Count = mpValues->get_Count();
	for( vuint32 i = 1; i <= Count ; ++i )
    {
		I_Value_Ptr pValue = mpValues->get_ItemAt( i );        
        res += pValue->get_Allocated();
    } 
    
    return res;
}


/**********************************************************************************************/
bool Value_Compound::put_Allocated( vuint32 inSize )
{
	argused1( inSize );

	// NEVER CALLED
    FBL_CHECK( false );
    
	return false;
}


/**********************************************************************************************/
vuint32 Value_Compound::get_ByteLength( void ) const
{
	vuint32 res = 0;

	vuint32 Count = mpValues->get_Count();
	for( vuint32 i = 1; i <= Count ; ++i )
    {
		I_Value_Ptr pSubValue = mpValues->get_ItemAt( i );
        
        res += pSubValue->get_ByteLength();	
    } 
    
    return res;
}


/**********************************************************************************************/
void Value_Compound::put_ByteLength( vuint32 inNewLength )
{
	// NEVER CALLED
    FBL_CHECK( false );

	argused1( inNewLength );
}


/**********************************************************************************************/
// REPLACES parent alg, because compound index have
//	2-bytes PREFIX of length,  { Value, ... }
//
vuint32 Value_Compound::get_ByteLengthForIndex( void ) const
{
	vuint32 res = 0;

	vuint32 Count = mpValues->get_Count();
	for( vuint32 i = 1; i <= Count ; ++i )
    {
		I_Value_Ptr pSubValue = mpValues->get_ItemAt( i );
        
        res += pSubValue->get_ByteLengthForIndex();	
    } 
    
    // fixed-size compound value has 0, variable-length has 2 bytes prefix.
    vuint32 prefLen = IndexPrefixLen();
    res += prefLen;

    return res;
}


/**********************************************************************************************/
VALUE_CATEGORY Value_Compound::get_Category( void ) const
{
	return vcCompound;
}


/**********************************************************************************************/
// TRUE if all sub-values are Nullable.
//
bool Value_Compound::get_IsNullable( void ) const
{
	bool res = true;

	vuint32 Count = mpValues->get_Count();
	for( vuint32 i = 1; i <= Count ; ++i )
    {
		I_Value_Ptr pSubValue = mpValues->get_ItemAt( i );
        
        if( pSubValue->get_IsNullable() == false )
        {
        	res = false;
            break; // loop
        }	
    } 
    
    return res;
}


/**********************************************************************************************/
bool Value_Compound::get_IsNull( void ) const
{
	bool res = false;

	vuint32 Count = mpValues->get_Count();
	for( vuint32 i = 1; i <= Count ; ++i )
    {
		I_Value_Ptr pSubValue = mpValues->get_ItemAt( i );
        
        if( pSubValue->get_IsNull() )
        {
        	res = true;
            break; // loop
        }	
    } 
    
    return res;
}


/**********************************************************************************************/
void Value_Compound::put_IsNull( bool inValue )
{
	vuint32 Count = mpValues->get_Count();
	for( vuint32 i = 1; i <= Count ; ++i )
    {
		I_Value_Ptr pSubValue = mpValues->get_ItemAt( i );
        pSubValue->put_IsNull( inValue );
    } 
}


/**********************************************************************************************/
bool Value_Compound::get_IsRemote( void ) const
{
	// all sub-values must be of same kind, so we use the first to answer.
	bool res = mpValues->get_ItemAt( 1 )->get_IsRemote();

	return res;
}


/**********************************************************************************************/
void Value_Compound::put_IsRemote( bool inValue )
{
	// NEVER CALLED
    FBL_CHECK( false );
    
	argused1( inValue );
}


/**********************************************************************************************/
vuint32 Value_Compound::get_Length( void ) const
{
//	vuint32 Count = mpValues->get_Count();
//	for( vuint32 i = 1; i <= Count ; ++i )
//    {
//		// TODO
//        FBL_CHECK( false );
//    } 
	return 0;
}


/**********************************************************************************************/
vuint32 Value_Compound::get_MaxChars( void ) const
{
	int res = 0;

	vuint32 Count = mpValues->get_Count();
	for( vuint32 i = 1; i <= Count ; ++i )
    {
		// TODO
        FBL_CHECK( false );
    } 
    
    return res;
}


/**********************************************************************************************/
vuint32 Value_Compound::get_StorageSize( void ) const
{
	int res = 0;

	vuint32 Count = mpValues->get_Count();
	for( vuint32 i = 1; i <= Count ; ++i )
    {
		// TODO
        FBL_CHECK( false );
    } 

    return res;
}


/**********************************************************************************************/
VALUE_TYPE Value_Compound::get_Type( void ) const
{
	return kTypeCompound;
}


/**********************************************************************************************/
String Value_Compound::get_TypeString( const char inLocale[] ) const
{
	argused1(inLocale);

	return String( "Compound" );
}


#pragma mark -
#pragma mark === I_Value / Value Access Methods ===


/**********************************************************************************************/
const char* Value_Compound::begin( void ) const
{
	// NEVER CALLED
    FBL_CHECK( false );
   
	return nullptr;
}


/**********************************************************************************************/
const char* Value_Compound::end( void ) const
{
	// NEVER CALLED
    FBL_CHECK( false );
    
	return nullptr;
}


/**********************************************************************************************/
void Value_Compound::Assign( const I_Value& inValue )
{
	I_ValueCompound_Ptr pOtherCompound = QI( &inValue, I_ValueCompound );

	vuint16 Count = (vuint16)mpValues->get_Count();
	for( vuint16 i = 1; i <= Count ; ++i )
    {
		I_Value_Ptr pMySubValue    = mpValues->get_ItemAt( i );
		I_Value_Ptr pOtherSubValue = pOtherCompound->get_SubValue( i );
        
     	pMySubValue->Assign( *pOtherSubValue );     
    }     
}


/**********************************************************************************************/
// 1) CHOICE was to mofidy CopyToIndex() to return char* on byte AFTER the copied value.
// But then we need extra job to add it into numeric and string analogs.
//
// I think for us it is more important to have faster simple values,
// and do more job in the compound value itself, even if this is slower...
//
// So this class have array mpSizes and this class assumes to know LOGIC of Value_String::CopyToIndex().
//
// 2) We have TWO kinds of Value_Compound: FixedSize and VarSize.
// A VarSize value stores yet 2-bytes LENGTH PREFIX into index.
//
vuint32 Value_Compound::CopyToIndex( void* outBuffer, bool inSwapBytes ) const
{
	vuint32 res = 0;

	// VARIABLE COMPOUND VALUE - has TWO bytes prefix for length.
	char* p = (char*) outBuffer + IndexPrefixLen();

	vuint32 Count = mpValues->get_Count();
	for( vuint32 i = 1; i <= Count ; ++i )
    {
		I_Value_Ptr pMySubValue = mpValues->get_ItemAt( i );
        
     	vuint32 bytesAdded = pMySubValue->CopyToIndex( p, inSwapBytes );
        res += bytesAdded;
        
// TODO / FIXME:
// RZ 2014-09-03  It seems this can use now returned value bytesAdded from CopyToIndex().
        
        // Now we must calculate position for the next value in the index.
        vuint32 size = mpSizes->get_ItemAt( i );
        if( size )
        {
        	p += size;
        }
        else // this is a string
        {
        	size = pMySubValue->get_ByteLengthForIndex(); // for easy debugging.
            p += size;
        }
    } 
    
// FIXME-END
// ------------------
    
 	if( mIsVariable ) // then we calculate length of value stored into index. p - points end now.
		*(vuint16*)outBuffer = vuint16( p - (char*)outBuffer - sizeof(vuint16) );
    
    return 2 + res;
}


/**********************************************************************************************/
// See comments of Value_Compound::CopyToIndex();
//
void Value_Compound::CopyFromIndex( const void* inBuffer, bool inSwapBytes )
{
	// VARIABLE COMPOUND VALUE - has TWO bytes prefix for length.
	char* p = (char*) inBuffer + IndexPrefixLen();

	vuint32 Count = mpValues->get_Count();
	for( vuint32 i = 1; i <= Count ; ++i )
    {
		I_Value_Ptr pMySubValue = mpValues->get_ItemAt( i );
        
     	pMySubValue->CopyFromIndex( p, inSwapBytes );     

        // Now we must calculate position for the next value in the index.
        vuint32 size = mpSizes->get_ItemAt( i );
        if( size )
        {
        	p += size;
        }
        else // this is a string
        {
        	size = pMySubValue->get_ByteLengthForIndex();
            p += size;
        }
    } 
}


#pragma mark -
#pragma mark === I_Value / String Properties ===


/**********************************************************************************************/
// Calls one by one sub-values while each returns ZERO, i.e EQUAL.
// As only some sub-value returns NOT zero, we stop loop and return that result.
//
int Value_Compound::Compare( 
    const I_Value& 	inOther,
    COMPARE_TYPE 	inCompareType ) const
{
	int res = 0;

	I_ValueCompound_Ptr pOtherCompound = QI( &inOther, I_ValueCompound );

	vuint16 Count = (vuint16)mpValues->get_Count();
	for( vuint16 i = 1; i <= Count ; ++i )
    {
		I_Value_Ptr pMySubValue    = mpValues->get_ItemAt( i );
		I_Value_Ptr pOtherSubValue = pOtherCompound->get_SubValue( i );
        
     	res = pMySubValue->Compare( *pOtherSubValue, inCompareType );     
        if( res )
            break; // loop
    } 
    
    return res;
}


/**********************************************************************************************/
int Value_Compound::CompareIndexValues( 
    const void* 	inLeft,
    const void* 	inRight,
    bool			inSwapBytes) const
{
	int res = 0;

	char* pLValue = (char*) inLeft  + IndexPrefixLen();
	char* pRValue = (char*) inRight + IndexPrefixLen();

	vuint32 Count = mpValues->get_Count();
	for( vuint32 i = 1; i <= Count ; ++i )
    {
		I_Value_Ptr pMySubValue = mpValues->get_ItemAt( i );

     	res = pMySubValue->CompareIndexValues( pLValue, pRValue, inSwapBytes );     
        if( res )
            break; // loop
            
        // calculate pointer to the next sub-value in the index:
        vuint32 size = mpSizes->get_ItemAt( i );
        if( size )
        {
        	pLValue += size;
        	pRValue += size;
        }
        else // this is a string, we already know that both strings are equal.
        {
        	// This assumes that it is a single-byte string.
            //	FIXME if can be UTF16 here. Use as example Value_string::CompareIndexValues();
			vuint8 LenInBytes = *(vuint8*) pLValue;
        
        	pLValue += LenInBytes;
        	pRValue += LenInBytes;
        }
    } 

	return res;
}


/**********************************************************************************************/
// inParam - is used in simialr way as for VarChars: it indicates need of STAR WITH search,
//			just here is specify the First N sub-values, which must be compared. Rests are NULLs.
//
//	We have added support of START WITH for compound values, for KeyValues.
//
int Value_Compound::CompareToIndexValue( 
    Const_I_Value_Ptr 	inTestValue,
    const void* 		inIndexValue,
    vuint32				inParam,				// non-zero if START WITH search.
    bool				inSwapBytes ) const
{
	int res = 0;

	I_ValueCompound_Ptr pTestCompound = QI( inTestValue, I_ValueCompound );
    
	char* pIndexValue = (char*) inIndexValue + IndexPrefixLen();

	vuint16 Count = inParam ? (vuint16)inParam : (vuint16)mpValues->get_Count();
	for( vuint16 i = 1; i <= Count ; ++i )
    {
		I_Value_Ptr pMySubValue   = mpValues->get_ItemAt( i );
		I_Value_Ptr pTestSubValue = pTestCompound->get_SubValue( i );
        
        // 3d param is 0, because we do NOT do START WITHsearch for sub-values.
     	res = pMySubValue->CompareToIndexValue( pTestSubValue, pIndexValue, 0, inSwapBytes );
        if( res )
            break; // loop
            
        // calculate pointer to the next sub-value in the index:
        vuint32 size = mpSizes->get_ItemAt( i );
        if( size )
        {
        	pIndexValue += size;
        }
        else // this is a string
        {
        	pIndexValue += pTestSubValue->get_ByteLengthForIndex();
        }
    } 

	return res;
}


#pragma mark -
#pragma mark === I_Value / String Properties ===


/**********************************************************************************************/
String Value_Compound::get_String( 
    tslen 			inLimit ) const
{
	// NEVER CALLED
	argused1( inLimit );
	return String::sEmpty();
}    
    

/**********************************************************************************************/
void Value_Compound::put_String( 
    const 			String& inStr )
{
	// NEVER CALLED
	argused1( inStr );
}    


/**********************************************************************************************/
UChar* Value_Compound::get_String( 
    UChar* 			outString, 
    tslen 			inBufferChars ) const
{
	// NEVER CALLED
	argused2( outString, inBufferChars );
    return nullptr;
}    


/**********************************************************************************************/
void Value_Compound::put_String( 
    const UChar* 	inStart,
    const UChar* 	inEnd )
{
	// NEVER CALLED
	argused2( inStart, inEnd );
}    


/**********************************************************************************************/
char* Value_Compound::get_String( 
    char* 			outString, 
    tslen 			inBufferChars ) const
{
	// NEVER CALLED
	argused2( outString, inBufferChars );
    return nullptr;
}    


/**********************************************************************************************/
void Value_Compound::put_String( 
    const char* 	inStart,
    const char* 	inEnd )
{
	// NEVER CALLED
	argused2( inStart, inEnd );    
}    


#pragma mark -
#pragma mark === I_Value / Numeric Properties ===


/**********************************************************************************************/
bool Value_Compound::get_Boolean( void ) const
{ 
	// NEVER CALLED
	return false;
}


/**********************************************************************************************/
void Value_Compound::put_Boolean( bool inValue )
{ 
	// NEVER CALLED
    argused1( inValue );
}


/**********************************************************************************************/
vuint8 Value_Compound::get_Byte( void ) const
{ 
	// NEVER CALLED
    return 0;
}


/**********************************************************************************************/
void Value_Compound::put_Byte( vuint8 inValue )
{ 
	// NEVER CALLED
    argused1( inValue );
}


/**********************************************************************************************/
vint16 Value_Compound::get_Short( void ) const
{ 
	// NEVER CALLED
	return 0;
}


/**********************************************************************************************/
void Value_Compound::put_Short( vint16 inValue )
{ 
	// NEVER CALLED
    argused1( inValue );
}


/**********************************************************************************************/
vuint16 Value_Compound::get_UShort( void ) const
{ 
	// NEVER CALLED
    return 0;
}


/**********************************************************************************************/
void Value_Compound::put_UShort( vuint16 inValue )
{ 
	// NEVER CALLED
    argused1( inValue );
}


/**********************************************************************************************/
vint32 Value_Compound::get_Medium( void ) const
{ 
	// NEVER CALLED
    return 0;
}


/**********************************************************************************************/
void Value_Compound::put_Medium( vint32 inValue )
{ 
	// NEVER CALLED
    argused1( inValue );
}


/**********************************************************************************************/
vint32 Value_Compound::get_Long( void ) const
{ 
	// NEVER CALLED
    return 0;
}


/**********************************************************************************************/
void Value_Compound::put_Long( vint32 inValue )
{ 
	// NEVER CALLED
	argused1( inValue );
}


/**********************************************************************************************/
vuint32 Value_Compound::get_ULong( void ) const
{ 
	// NEVER CALLED
    return 0;
}


/**********************************************************************************************/
void Value_Compound::put_ULong( vuint32 inValue )
{ 
	// NEVER CALLED
    argused1( inValue );
}


/**********************************************************************************************/
vint64 Value_Compound::get_LLong( void ) const
{ 
	// NEVER CALLED
    return 0;
}


/**********************************************************************************************/
void Value_Compound::put_LLong( vint64 inValue )
{ 
	// NEVER CALLED
    argused1( inValue );
}


/**********************************************************************************************/
vuint64 Value_Compound::get_ULLong( void ) const
{ 
	// NEVER CALLED
    return 0;
}


/**********************************************************************************************/
void Value_Compound::put_ULLong( vuint64 inValue )
{ 
	// NEVER CALLED
	argused1( inValue );
}


/**********************************************************************************************/
float Value_Compound::get_Float( void ) const
{ 
	// NEVER CALLED
    return 0;
}


/**********************************************************************************************/
void Value_Compound::put_Float( float inValue )
{ 
	// NEVER CALLED
    argused1( inValue );
}


/**********************************************************************************************/
double Value_Compound::get_Double( void ) const
{ 
	// NEVER CALLED
    return 0;
}


/**********************************************************************************************/
void Value_Compound::put_Double( double inValue )
{ 
	// NEVER CALLED
    argused1( inValue );
}


#pragma mark -
#pragma mark === I_Value / Byte Swapping ===


/**********************************************************************************************/
void Value_Compound::SwapBytes( void )
{ 
	// NEVER CALLED
}


/**********************************************************************************************/
void Value_Compound::SwapBytes( void* inValue ) const
{ 
	argused1( inValue );
	// NEVER CALLED
}


/**********************************************************************************************/
void Value_Compound::SwapBytes( const void* inSrc, void* inDest )
{ 
	argused2( inSrc, inDest );
	// NEVER CALLED
}


#pragma mark -
#pragma mark === I_Value / Byte Swapping ===


/**********************************************************************************************/
void Value_Compound::Init( void )
{ 
	// NEVER CALLED
}


/**********************************************************************************************/
void Value_Compound::Increment( void )
{ 
	// NEVER CALLED
}


/**********************************************************************************************/
FBL_End_Namespace
