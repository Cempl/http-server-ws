/**********************************************************************************************/
/* FBL_I_Unknown.h		                                                      				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_Unknown_h 
	#define _FBL_I_Unknown_h

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>

#include <VShared/FBL/publ/Threads/FBL_ThreadSafe.h>
#include <VShared/FBL/publ/Headers/FBL_Types.h>

#include <VShared/FBL/publ/Interfaces/FBL_Public_IIDs.h>

#if __LP64__ && !defined(FBL_UNIX)
#include <Kernel/sys/types.h>
#endif // __LP64__

/**********************************************************************************************/
FBL_Begin_Namespace

/**********************************************************************************************/
// Atomic reference counter.
// Could be used as RefCount for MT builds or for some classes only (any build type)
// (see IMPLEMENT_ATOMIC_UNKNOWN).
//
typedef struct AtomicRefCount
{
	public://///////////////////////////////////////////////////////////////////////////////////

									AtomicRefCount( void ) : mValue( 0 ) {}
		Atomic_vuint32				mValue;
		
} AtomicRefCount;


/**********************************************************************************************/
// reference counter used in all I_Unknown classes
//
#if FBL_INTERNAL_THREADSAFE || FBL_MT_REINDEX || FBL_EXTERNAL_THREADSAFE
typedef AtomicRefCount RefCount;
#else
typedef struct RefCount
{
	volatile vuint32			mValue;
	RefCount( void ) : mValue( 0 ) {}
} RefCount;
#endif // FBL_INTERNAL_THREADSAFE || FBL_MT_REINDEX || FBL_EXTERNAL_THREADSAFE


/**********************************************************************************************/
#ifdef _MSC_VER
#pragma warning(disable: 4510)
#pragma warning(disable: 4610)
#endif //_MSC_VER

typedef struct INTERFACE_ITEM
{
	const InterfaceID	iid;
	intptr_t			offset;
	
} INTERFACE_ITEM;


/**********************************************************************************************/
FBL_SHARED_EXP 
void* InterfaceTableQueryInterface( 
	const void* 			inThis, 
	const INTERFACE_ITEM* 	inTable, 
	InterfaceID		 		inIID );


/**********************************************************************************************/
#define BEGIN_INTERFACE_TABLE(ClassName)								\
	typedef ClassName _ITCls;											\
	const INTERFACE_ITEM* GetInterfaceTable( void ) const {		\
		static const INTERFACE_ITEM table [] = {


// ----
#define END_INTERFACE_TABLE()											\
			{ 0, 0 } };													\
		return table; 													\
	}


/**********************************************************************************************/
#define BASE_OFFSET(ClassName, BaseName)\
	(intptr_t (static_cast<BaseName*>((ClassName*)0x10000000)) - 0x10000000)


// ----
#define IMPLEMENT_INTERFACE(Itf)										\
			{ IID_##Itf, BASE_OFFSET(_ITCls, Itf) },



/**********************************************************************************************/
#define BASE_OFFSET_AS(ClassName, BaseName)\
	(intptr_t ((BaseName*)(ClassName*)(_ITCls*)0x10000000) - 0x10000000)


// ----
#define IMPLEMENT_INTERFACE_AS(req, Itf)								\
{ IID_##req, BASE_OFFSET_AS(_ITCls::Itf, req) },



#pragma mark -


/**********************************************************************************************/
SMART_INTERFACE(I_Unknown);


/**********************************************************************************************/
// Base class for all interface FBL_SHARED_EXP_CLASS classes.
// Specify reference counting.
// This version provide hidden reference counting 
// with help of smart_ptr class.
//
interface FBL_SHARED_EXP_CLASS I_Unknown
{
	protected://////////////////////////////////////////////////////////////////////////////////

virtual 					~I_Unknown( void );


	public://///////////////////////////////////////////////////////////////////////////////////

							/// Increment reference counter for this interface FBL_SHARED_EXP_CLASS.
							/// Returns counter value.
virtual	vuint32				AddRef( void ) const  = 0;
			
							/// Release this interface FBL_SHARED_EXP_CLASS.
							/// Returns counter value.
virtual	vuint32				Release( void ) const  = 0;

							// Analog of COM QueryInterface.
							// Difference is that we return required interface 
							// not via parameter, but via function return.
							// Also we do not use GUID, instead simple numerate constants
							//
							// Returns NULL if required interface is not found in this object.
virtual void*				QueryInterface( InterfaceID inIID ) const  = 0;


	public://///////////////////////////////////////////////////////////////////////////////////

#if FBL_USE_MEMPOOL
		void*				operator new( size_t inSize );
#endif // FBL_USE_MEMPOOL

};


#pragma mark -


/**********************************************************************************************/
#define IMPLEMENT_UNKNOWN_NO_REFS(ClassName)										\
public:																				\
virtual	vuint32				AddRef( void ) const override							\
							{														\
								return ADDREF(mRefs);								\
							}														\
																			    	\
virtual	vuint32				Release( void ) const override							\
							{														\
								vuint32 res = RELEASE(mRefs);						\
								if (res == 0)								    	\
									delete this;							    	\
								return res;									    	\
							}														\
																					\
virtual void*				QueryInterface( InterfaceID inIID ) const override		\
							{														\
								return InterfaceTableQueryInterface(				\
											this, GetInterfaceTable(), inIID );		\
							}	



/**********************************************************************************************/
#define IMPLEMENT_UNKNOWN(ClassName)												\
public:																				\
mutable RefCount			mRefs;													\
IMPLEMENT_UNKNOWN_NO_REFS(ClassName)


/**********************************************************************************************/
#define IMPLEMENT_ATOMIC_UNKNOWN(ClassName)											\
public:																				\
mutable AtomicRefCount		mRefs;													\
IMPLEMENT_UNKNOWN_NO_REFS(ClassName)


/**********************************************************************************************/
#define IMPLEMENT_SINGLE_UNKNOWN(ClassName)											\
			IMPLEMENT_UNKNOWN(ClassName)											\
			BEGIN_INTERFACE_TABLE(ClassName)										\
				IMPLEMENT_INTERFACE(I_Unknown)										\
			END_INTERFACE_TABLE()


/**********************************************************************************************/
//#define QUERY_INTERFACE(p, InterfaceName) (InterfaceName*) p->QueryInterface( IID_##InterfaceName )
#define QUERY_INTERFACE(p, InterfaceName) _QueryInterface<InterfaceName*>( p, IID_##InterfaceName )

#define QI(p, InterfaceName) QUERY_INTERFACE(p, InterfaceName)


/**********************************************************************************************/
template <class InterfacePtr>
InterfacePtr _QueryInterface( Const_I_Unknown_Ptr inpObject, const vuint32 inInterfaceID )
{
	return ( inpObject ) ? (InterfacePtr)inpObject->QueryInterface( inInterfaceID ) : nullptr;
}


/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_I_Unknown_h
