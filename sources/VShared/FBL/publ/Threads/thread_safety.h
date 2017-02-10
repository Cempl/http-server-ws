/**********************************************************************************************/
/* thread_safety.h                                                                            */
/*                                                                       					  */
/* Copyright Conceptual Software, 2015-2016													  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#pragma once


/**********************************************************************************************/
// Enable thread safety attributes only with clang.
// The attributes can be safely erased when compiling with other compilers.
//
#if defined(__clang__) && (!defined(SWIG)) && FBL_THREAD_SAFETY_ANALYSIS
#define THREAD_ANNOTATION_ATTRIBUTE(x)   __attribute__((x))
#else // defined(__clang__) && (!defined(SWIG))
#define THREAD_ANNOTATION_ATTRIBUTE(x)   // no-op
#endif // defined(__clang__) && (!defined(SWIG))


/**********************************************************************************************/
#define CAPABILITY(x) \
  THREAD_ANNOTATION_ATTRIBUTE(capability(x))

/**********************************************************************************************/
#define SCOPED_CAPABILITY \
  THREAD_ANNOTATION_ATTRIBUTE(scoped_lockable)

/**********************************************************************************************/
#define GUARDED_BY(x) \
  THREAD_ANNOTATION_ATTRIBUTE(guarded_by(x))

/**********************************************************************************************/
#define PT_GUARDED_BY(x) \
  THREAD_ANNOTATION_ATTRIBUTE(pt_guarded_by(x))

/**********************************************************************************************/
#define ACQUIRED_BEFORE(...) \
  THREAD_ANNOTATION_ATTRIBUTE(acquired_before(__VA_ARGS__))

/**********************************************************************************************/
#define ACQUIRED_AFTER(...) \
  THREAD_ANNOTATION_ATTRIBUTE(acquired_after(__VA_ARGS__))

/**********************************************************************************************/
#define REQUIRES(...) \
  THREAD_ANNOTATION_ATTRIBUTE(requires_capability(__VA_ARGS__))

/**********************************************************************************************/
#define REQUIRES_SHARED(...) \
  THREAD_ANNOTATION_ATTRIBUTE(requires_shared_capability(__VA_ARGS__))

/**********************************************************************************************/
#define ACQUIRE(...) \
  THREAD_ANNOTATION_ATTRIBUTE(acquire_capability(__VA_ARGS__))

/**********************************************************************************************/
#define ACQUIRE_SHARED(...) \
  THREAD_ANNOTATION_ATTRIBUTE(acquire_shared_capability(__VA_ARGS__))

/**********************************************************************************************/
#define RELEASE_(...) \
  THREAD_ANNOTATION_ATTRIBUTE(release_capability(__VA_ARGS__))

/**********************************************************************************************/
#define RELEASE_SHARED(...) \
  THREAD_ANNOTATION_ATTRIBUTE(release_shared_capability(__VA_ARGS__))

/**********************************************************************************************/
#define TRY_ACQUIRE(...) \
  THREAD_ANNOTATION_ATTRIBUTE(try_acquire_capability(__VA_ARGS__))

/**********************************************************************************************/
#define TRY_ACQUIRE_SHARED(...) \
  THREAD_ANNOTATION_ATTRIBUTE(try_acquire_shared_capability(__VA_ARGS__))

/**********************************************************************************************/
#define EXCLUDES(...) \
  THREAD_ANNOTATION_ATTRIBUTE(locks_excluded(__VA_ARGS__))

/**********************************************************************************************/
#define ASSERT_CAPABILITY(x) \
  THREAD_ANNOTATION_ATTRIBUTE(assert_capability(x))

/**********************************************************************************************/
#define ASSERT_SHARED_CAPABILITY(x) \
  THREAD_ANNOTATION_ATTRIBUTE(assert_shared_capability(x))

/**********************************************************************************************/
#define RETURN_CAPABILITY(x) \
  THREAD_ANNOTATION_ATTRIBUTE(lock_returned(x))

/**********************************************************************************************/
#define NO_THREAD_SAFETY_ANALYSIS \
  THREAD_ANNOTATION_ATTRIBUTE(no_thread_safety_analysis)
