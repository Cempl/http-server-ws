/**********************************************************************************************/
/* FBL_TestSystem_Values.h                                                    				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_TestSystem_Values_h
	#define _FBL_TestSystem_Values_h
#pragma once 

#include <VShared/FBL/publ/Headers/FBL_Macros.h>

#include <vector>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
#if FBL_MAC

	namespace std { typedef decltype(nullptr) nullptr_t; }

#endif // FBL_MAC


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
struct _date
{
	vint32		y;
    vuint16		m;
    vuint16		d;
};


/**********************************************************************************************/
struct _time
{
	vuint16		hours;
	vuint16		minutes;
	vuint16		seconds;
	vuint16		milliseconds;
};


/**********************************************************************************************/
struct _datetime
{
	vint32		y;
    vuint16		m;
    vuint16		d;
	vuint16		hours;
	vuint16		minutes;
	vuint16		seconds;
	vuint16		milliseconds;
};


/**********************************************************************************************/
template<class T>
struct Values
{
    template <typename... Args>
    Values( Args... items )
    {
        add( items... );
    }

    template<class U, class... Args>
    void add( const U& v, Args... tail )
    {
        add( v );
        add( tail... );
    }    

    template<class U>
    void add( const U& v )
    {
    	if( !mpItems )
        	mpItems = new ArrayOfValues();
    
        mpItems->AddItem( new T( v ) );
    }

    void add( std::nullptr_t )
    {
    	if( !mpItems )
        	mpItems = new ArrayOfValues();
    
        mpItems->AddItem( new T );
    }

    void add( _date d )
    {
    	if( !mpItems )
        	mpItems = new ArrayOfValues();
    
        mpItems->AddItem( new T( d.y, d.m, d.d, nullptr ) );
    }

    void add( _time t )
    {
    	if( !mpItems )
        	mpItems = new ArrayOfValues();
    
        mpItems->AddItem( new T( t.hours, t.minutes, t.seconds, t.milliseconds, nullptr ) );
    }

    void add( _datetime dt )
    {
    	if( !mpItems )
        	mpItems = new ArrayOfValues();
    
        mpItems->AddItem( new T( dt.y, dt.m, dt.d, dt.hours, dt.minutes, dt.seconds, dt.milliseconds, nullptr ) );
    }

    void add( void )
    {
    }

// cast operator:

    operator ArrayOfValues_Ptr() { return mpItems; }

// data:
    ArrayOfValues_Ptr mpItems;
};


/**********************************************************************************************/
template<typename...Args>
struct Columns
{
    explicit Columns( Values<Args>... values )
    {
        add( values... );
    }

    template<class U, class... Lists>
    void add( const U& v, Lists... tail )
    {
        add( v );
        add( tail... );
    }    

    template<class U>
    void add( const U& v )
    {
        mColumns.push_back( v.mpItems );
    }

// cast operator:

    operator std::vector<ArrayOfValues_Ptr>() { return mColumns; }

// data:

    std::vector<ArrayOfValues_Ptr> mColumns;

};


	
/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_TestSystem_Values_h
