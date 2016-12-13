/**********************************************************************************************/
/* FBL_Debug_Macros.h	 	                                                      			  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_Debug_Macros_h 
	#define _FBL_Debug_Macros_h
#pragma once


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
// Assert macro used in all sources of kernel
//
#ifndef _FBL_CHECK
#define _FBL_CHECK 1
#endif //_FBL_CHECK

#if _FBL_CHECK == 0
	#define FBL_CHECK(p) (void(0))
	#define FBL_VERIFY(p) (static_cast<void>(p))
	#define FBL_CHECK_EX(p,cond) (void(0))
#else
	#define FBL_CHECK(p) CHECK(p)
	#define FBL_VERIFY(p) FBL_CHECK(p)
	#define FBL_CHECK_EX(p,cond) CHECK_EX(p,cond)
#endif


#define FBL_PRE_CONDITION(c)  FBL_CHECK(c)
#define FBL_POST_CONDITION(c) FBL_CHECK(c)


/**********************************************************************************************/
	// This macro is usefull for deletion of objects of some classes.
	// It shouldn't be used for deletion of array of objects.
#define ForgetObject(obj) \
		(void) ((obj) ? (delete (obj), (obj) = NULL) : 0 )


	// This macros can be used to suppress warning "Parameter is not used".
#define argused1(x1) 					(void)(&x1)
#define argused2(x1,x2) 				argused1(x1); (void)(&x2)
#define argused3(x1,x2,x3) 				argused2(x1,x2); (void)(&x3)
#define argused4(x1,x2,x3,x4) 			argused3(x1,x2,x3); (void)(&x4)
#define argused5(x1,x2,x3,x4,x5) 		argused4(x1,x2,x3,x4); (void)(&x5)
#define argused6(x1,x2,x3,x4,x5,x6) 	argused5(x1,x2,x3,x4,x5); (void)(&x6)
#define argused7(x1,x2,x3,x4,x5,x6,x7) 	argused6(x1,x2,x3,x4,x5,x6); (void)(&x7)

	// This macros can be used to suppress warning "Parameter is not used"
	// and in the same time demonstrate that this must be fixed yet.	
#define fixme_argused1(x1) 					argused1(x1)
#define fixme_argused2(x1,x2) 				argused2(x1,x2)
#define fixme_argused3(x1,x2,x3) 			argused3(x1,x2,x3)
#define fixme_argused4(x1,x2,x3,x4) 		argused4(x1,x2,x3,x4)
#define fixme_argused5(x1,x2,x3,x4,x5) 		argused5(x1,x2,x3,x4,x5)
#define fixme_argused6(x1,x2,x3,x4,x5,x6) 	argused6(x1,x2,x3,x4,x5,x6)


/**********************************************************************************************/
/**********************************************************************************************/
/**********************************************************************************************/
#define FBL_Throw(x)	throw x


/**********************************************************************************************/
#if __profile__
#	define PROFILER_ON	ProfilerSetStatus( true );
#	define PROFILER_OFF	ProfilerSetStatus( false );
#else
#	define PROFILER_ON	
#	define PROFILER_OFF	
#endif
 

/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#endif // _FBL_Debug_Macros_h
