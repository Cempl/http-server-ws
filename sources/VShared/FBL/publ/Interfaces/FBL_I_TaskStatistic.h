/**********************************************************************************************/
/* FBL_I_TaskStatistic.h 		                                               				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#ifndef _FBL_I_TaskStatistic_h 
	#define _FBL_I_TaskStatistic_h

#include <VShared/FBL/publ/Sets/FBL_Set.h>

#include <VShared/FBL/publ/Headers/FBL_pre_header.h>


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
SMART_INTERFACE(I_TaskStatistic);


/**********************************************************************************************/
// interface FBL_SHARED_EXP_CLASS class
//
interface FBL_SHARED_EXP_CLASS I_TaskStatistic : public I_Unknown
{

virtual						~I_TaskStatistic( void );

	// ---------------------
	// Attributes:

	// <StartTime> [r/o]
							// Returns the time when this task was born.
virtual	vuint32				get_StartTime( void ) const = 0;

	// <EndTime> [r/o]
							// Returns the time when this task was finished.
virtual	vuint32				get_EndTime( void ) const = 0;

	// <ExecutionTime> [r/o]

							// Returns the time this task have take (in total).
virtual vuint32				get_ExecutionTime( void ) const = 0;


	// <ErrorCount> [r/o]
							// Returns the number of errors during execution.
virtual	vuint32				get_ErrorCount( void ) const = 0;

	// <WarningCount> [r/o]
							// Returns the number of warnings during execution.
virtual	vuint32				get_WarningCount( void ) const = 0;


	// ---------------------
	// Methods to change attributes:

virtual void				IncErrorCount( void ) = 0;	
virtual void				IncWarningCount( void ) = 0;	

};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // _FBL_I_TaskStatistic_h
