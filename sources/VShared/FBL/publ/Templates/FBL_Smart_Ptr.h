/**********************************************************************************************/
/* FBL_Smart_Ptr.h                                                     					  	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_Smart_Ptr_h
	#define _FBL_Smart_Ptr_h
#pragma once

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
#ifdef _MSC_VER // moved here to work around VC++ compiler crash
# pragma warning(push)
# pragma warning(disable:4284) // odd return type for operator->
#endif


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
struct I_Unknown;


/**********************************************************************************************/
template<class T> class smart_ptr
{
protected:

    typedef smart_ptr<T> this_type;

public:

    typedef T element_type;

    smart_ptr(): p_(nullptr)
    {
    }

    smart_ptr(T * p, bool add_ref = true): p_(p)
    {
        if(p_ != nullptr && add_ref) p_->AddRef();
    }

    template<class U> smart_ptr(smart_ptr<U> const & rhs): p_(rhs.get())
    {
        if(p_ != nullptr) p_->AddRef();
    }

    smart_ptr(this_type const & rhs): p_(rhs.p_)
    {
        if(p_ != nullptr) p_->AddRef();
    }

	smart_ptr( this_type&& rhs ): p_(nullptr)
	{
		this_type(rhs).swap(*this);
	}

    ~smart_ptr()
    {
		if(p_ != nullptr) p_->Release();
    }

    template<class U> this_type & operator=(smart_ptr<U> const & rhs)
    {
        this_type(rhs).swap(*this);
        return *this;
    }

    this_type & operator=(this_type const & rhs)
    {
        this_type(rhs).swap(*this);
        return *this;
    }

	this_type & operator=( this_type&& rhs )
    {
        this_type(rhs).swap(*this);
        return *this;
    }

    this_type & operator=(T * rhs)
    {
		// Create tmp local stack object from rhs (copy of rhs) and swap its data with "this"
		// object.
		//
		// --------------
		// "p_" is a common data for "rhs" and "tmp-local" before calling swap and
		// common for "this" and "rhs" after swap.
		// The tmp object actually call AddRef() for "rhs->p_" and swaps "tmp-local.p_" and "this->p_".
		// The tmp object will be destroyed on exit, but now (after swap), it is not a copy of
		// "rhs" but copy of "old this" object.
		// It is important to call Release for "tmpObj.p_" (old "this" data) in general case.
		//
		// --------------
		// The tmp object finally keeps nullptr in most of cases:
		//
		//		...
		//		I_Object_Ptr pObj; // == nullptr
		//		...
		//		pObj = CreateObject(...);
		//		...
		//
		// So, it will be no tmpObj.Release() call at all.
		//
		// --------------
		// But even if pObj initially is not nullptr:
		//
		//		...						   // Assume inpObj.refCount == 1.
		//		I_Object_Ptr pObj = inpObj // != nullptr and refCount == 2.
		//		...
		//		pObj = CreateObject(...);
		//		...						   // inpObj.RefCount must be 1.
		//
		// tmpObj.Release() will be called for "inpObj.p_".
		// This is a reason to use "swap" instead of simply copy/move data!!!
		//
		//
		// --------------
		// Actually we don't remember the reason why we use "swap" everywhere instead of something
		// like this one:
		// operator = ()
		// {
		//		if( other != ref )
		//		{
		//			if( ref ) ref->Release();
		//			ref = other;
		//			if( ref ) ref->AddRef();
		//		}
		//		return *this;
		// }
		//
		// Probably we were worried of inlined code size or something like this...
		//
        this_type(rhs).swap(*this);
        return *this;
    }

	this_type & SetNull(void)
    {
        this_type(nullptr).swap(*this);
        return *this;
    }

	bool IsNull( void )
    {
        return p_ == 0;
    }

	bool IsNotNull( void )
    {
        return p_ != 0;
    }


    T * get() const
    {
        return p_;
    }

    T & operator*() const
    {
        return *p_;
    }

    T * operator->() const
    {
        return p_;
    }

    typedef T * (this_type::*unspecified_bool_type) () const;

    operator unspecified_bool_type () const
    {
        return p_ == nullptr ? nullptr : &this_type::get;
    }

    // operator! is a Borland-specific workaround
    bool operator! () const
    {
        return bool(p_ == nullptr);
    }

    void swap(this_type & rhs)
    {
        T * tmp = p_;
        p_ = rhs.p_;
        rhs.p_ = tmp;
    }

	T* free()
    {
        T * tmp = p_;
        p_ = 0;
        return tmp;
    }

protected:

    T * p_;
};


/**********************************************************************************************/
template<class T, class U> 
inline 
bool operator==(smart_ptr<T> const & a, smart_ptr<U> const & b)
{
    return a.get() == b.get();
}


/**********************************************************************************************/
template<class T, class U> 
inline 
bool operator!=(smart_ptr<T> const & a, smart_ptr<U> const & b)
{
    return a.get() != b.get();
}


/**********************************************************************************************/
template<class T> 
inline 
bool operator==(smart_ptr<T> const & a, T * b)
{
    return a.get() == b;
}


/**********************************************************************************************/
template<class T> 
inline 
bool operator!=(smart_ptr<T> const & a, T * b)
{
    return a.get() != b;
}


/**********************************************************************************************/
template<class T> 
inline 
bool operator==(T * a, smart_ptr<T> const & b)
{
    return a == b.get();
}


/**********************************************************************************************/
template<class T> 
inline 
bool operator!=(T * a, smart_ptr<T> const & b)
{
    return a != b.get();
}

#if __GNUC__ == 2 && __GNUC_MINOR__ <= 96


/**********************************************************************************************/
// Resolve the ambiguity between our op!= and the one in rel_ops
template<class T> 
inline 
bool operator!=(smart_ptr<T> const & a, smart_ptr<T> const & b)
{
    return a.get() != b.get();
}

#endif // __GNUC__ == 2 && __GNUC_MINOR__ <= 96


/**********************************************************************************************/
template<class T> 
inline 
bool operator<(smart_ptr<T> const & a, smart_ptr<T> const & b)
{
    return a.get() < b.get() ? a.get() : b.get();
}


/**********************************************************************************************/
template<class T> 
inline
void swap(smart_ptr<T> & lhs, smart_ptr<T> & rhs)
{
    lhs.swap(rhs);
}


/**********************************************************************************************/
// mem_fn support
template<class T> 
inline
T * get_pointer(smart_ptr<T> const & p)
{
    return p.get();
}


#pragma mark -
#pragma mark ====== Casts ======


/**********************************************************************************************/
template< class T > 
inline
smart_ptr< T > fbl_const_cast( const T* p )
{
	return const_cast<T *>(p);
}


/**********************************************************************************************/
template< class T >
inline 
smart_ptr< T > fbl_const_cast( smart_ptr< const T > p )
{
	return const_cast<T *>( p.get() );
}
 
/**********************************************************************************************/
template<class T, class U> 
inline
smart_ptr<T> fbl_static_cast(smart_ptr<U> const & p)
{
    return static_cast<T *>(p.get());
}


/**********************************************************************************************/
#define fbl_scast fbl_static_cast
#define fbl_ccast fbl_const_cast


#pragma mark -


/**********************************************************************************************/
// RZ: simple set it on/off right here.
//
#ifndef FBL_WATCH_DYNAMIC_CAST
#define FBL_WATCH_DYNAMIC_CAST 0
#endif


/**********************************************************************************************/
template<class T, class U> 
inline
smart_ptr<T> fbl_dynamic_cast(smart_ptr<U> const & p)
{
    #if FBL_WATCH_DYNAMIC_CAST
    FBL_CHECK(false);		// For ability to set breakpoint on any dynamic_cast.
    #endif 
    
    return dynamic_cast<T *>(p.get());
}


/**********************************************************************************************/
template< class T, class U > 
inline
smart_ptr< T > fbl_dynamic_cast( U* p )
{
    #if FBL_WATCH_DYNAMIC_CAST
    FBL_CHECK(false);		// For ability to set breakpoint on any dynamic_cast.
    #endif

	return dynamic_cast<T *>(p);
}


/**********************************************************************************************/
template< class T, class U > 
inline
T fbl_dynamic_cast_native( U* p )
{
	#if FBL_WATCH_DYNAMIC_CAST
    FBL_CHECK(false);    
	#endif
	return dynamic_cast<T>(p);
}


/**********************************************************************************************/
/*
template< class T, class U > 
inline
T fbl_dynamic_cast_native( U& p )
{
    FBL_CHECK(false);    
	return dynamic_cast<T>(p);
}
*/


/**********************************************************************************************/
#if FBL_WATCH_DYNAMIC_CAST && __CHECK
	#define dcast  		fbl_dynamic_cast_native
	#define fbl_dcast 	fbl_dynamic_cast
#else
	#define dcast 		dynamic_cast
	#define fbl_dcast 	fbl_dynamic_cast
#endif // FBL_WATCH_DYNAMIC_CAST


/**********************************************************************************************/
FBL_End_Namespace


/*******************************************************************************************/
#ifdef _MSC_VER
# pragma warning(pop)
#endif    


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_Smart_Ptr_h
