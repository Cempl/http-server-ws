/**********************************************************************************************/
/* FBL_Exceptions.h																		 	  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_Exceptions_h
	#define _FBL_Exceptions_h

#include <VShared/FBL/publ/Headers/FBL.h>

#include <VShared/FBL/publ/Errors/FBL_ErrorMap.h>


/**********************************************************************************************/
FBL_Begin_Namespace

/**********************************************************************************************/
FBL_SHARED_EXP void ThrowInternalError( 	
	ERROR_TYPE	inErrorCode,
	const char* inFile, 
	const char* inFunc, 
	vuint32		inLine,
	const char* inMsg );

FBL_SHARED_EXP void ThrowInternalError( 	
	ERROR_TYPE	inErrorCode,
	const char* inFile, 
	const char* inFunc, 
	vuint32		inLine,
	const UChar* inMsg );

FBL_SHARED_EXP void ThrowInternalError( 
	ERROR_TYPE	inErrorCode,
	const char* inFile, 
	const char* inFunc, 
	vuint32		inLine,
	const UChar* inMsg1,
	const UChar* inMsg2,
	const UChar* inMsg3 );


/**********************************************************************************************/
// RZ 3/18/10 
// These macros allows us to throw easy INTERNAL ERRORs with info about FILE, FUNC, LINE.
//

/**********************************************************************************************/
#if FBL_WIN
	#define THROW_ERROR_INTERNAL(code)\
		ThrowInternalError( code, __FILE__, __FUNCTION__, __LINE__, (UChar*)NULL);

	#define THROW_ERROR_INTERNAL_1(code,msg)\
		ThrowInternalError( code, __FILE__, __FUNCTION__, __LINE__, msg);

	#define THROW_ERROR_INTERNAL_3(code,msg1,msg2,msg3)\
		ThrowInternalError( code, __FILE__, __FUNCTION__, __LINE__, msg1, msg2, msg3);

#else
	#define THROW_ERROR_INTERNAL(code)\
		ThrowInternalError( code, __FILE__, __func__, __LINE__, (UChar*)NULL);

	#define THROW_ERROR_INTERNAL_1(code,msg)\
		ThrowInternalError( code, __FILE__, __func__, __LINE__, msg);

	#define THROW_ERROR_INTERNAL_3(code,msg1,msg2,msg3)\
		ThrowInternalError( code, __FILE__, __func__, __LINE__, msg1, msg2, msg3);

#endif //FBL_WIN


/**********************************************************************************************/
class String;

/**********************************************************************************************/
interface I_Connection;


/**********************************************************************************************/
SMART_ARRAY_OF_SIMPLE_PTRS( ArrayOfUChars, UChar* );


/**********************************************************************************************/
struct ErrorEntry
{
	ERROR_TYPE	mCode;
	const char*	mSymbol;
	ArrayOfUChars_Ptr	mDescriptions;
};


/**********************************************************************************************/
// We use mechanism of callbacks to get error message in appropriate language.
// gpFormatErrorMessage should be set by kernel.dll
// Because the language id is internal kernel.dll info.
//
typedef	const UChar* (*FormatErrorMessagePtr)(
	ErrorEntry*  inErrorEntry,
	const UChar* const inArgs[] );

FBL_SHARED_EXP extern FormatErrorMessagePtr gpFormatErrorMessage;


/**********************************************************************************************/
// This is a normal exception class to be used in the kernel.
// You can always catch xException exception to catch ANY exception from database kernel.
//
// We do not use String class here to break circular dependancy on it.
//
class FBL_SHARED_EXP_CLASS xException
{
	public://///////////////////////////////////////////////////////////////////////////////////

	//------------------
	// Construction

							xException(
								ERROR_TYPE 		inErrorCode = ERR_INTERNAL_ERROR,
								const UChar* 	inArg1 = nullptr,
								const UChar* 	inArg2 = nullptr,
								const UChar* 	inArg3 = nullptr,
								const UChar* 	inArg4 = nullptr);

							xException(
								ERROR_TYPE 		inErrorCode,
								const char* 	inArg1,
								const char* 	inArg2 = nullptr,
								const char* 	inArg3 = nullptr,
								const char* 	inArg4 = nullptr);

							xException(
								ERROR_TYPE 		inErrorCode,
								const String& 	inFinalMessage,
								I_Connection* 	inConn );

							xException( const xException& inExc );

virtual						~xException( void ) throw();

        xException&         operator=( const xException& inOther );


	public://///////////////////////////////////////////////////////////////////////////////////

		//------------------
		// ErrProperties

		ERROR_TYPE			get_ErrorCode( void ) const
								{ return mErrorCode; }

virtual String				get_ErrorString( void ) const;

		const String& 		get_Param( vuint32 inIndex ) const;

		I_Connection*		get_Connection( void ) const 
								{ return mpConnection; }


	protected://///////////////////////////////////////////////////////////////////////////

		void				Clear( void );
	
							// The purpose is to reject this class instantiation.
//virtual	void				DummyCall( void ) const  = 0;


	protected://///////////////////////////////////////////////////////////////////////////

		ERROR_TYPE 			mErrorCode;
		String*				mArgs[4];
		I_Connection*		mpConnection;	// Pointer to the connection object to get error description string.
};


#pragma mark -

/**********************************************************************************************/
// Special class to throw ERR_INSUFFICIENT_MEMORY error from operator new.
// 
class xInsufficientMemoryError
:
	public xException,
	public std::bad_alloc
{
	public://////////////////////////////////////////////////////////////////////////
	  
							xInsufficientMemoryError()
							:
								xException( FBL::ERR_INSUFFICIENT_MEMORY )
							{
							}

virtual						~xInsufficientMemoryError( void ) throw()
							{
							}
};


#pragma mark -


/**********************************************************************************************/
/**	Finds error entry in the map of existing error entries.
	@param inErrorCode - error code to find to.
	@return pointer to those entry if it was found or NULL,
		if there is no such error code. */
FBL_SHARED_EXP ErrorEntry* FindErrorEntry( ERROR_TYPE inErrorCode );
FBL_SHARED_EXP ErrorEntry* FindErrorEntry( const char*	inErrorStr );


/**********************************************************************************************/
#define DECLARE_EXCEPTION(NewExc, BaseExc, kErr)							\
	class NewExc : public BaseExc 											\
	{																		\
		public:																\
			NewExc( ERROR_TYPE inErr = (kErr),								\
				const UChar* inArg1 = 0,									\
				const UChar* inArg2 = 0,									\
				const UChar* inArg3 = 0,									\
				const UChar* inArg4 = 0 )									\
				: BaseExc( inErr, inArg1, inArg2, inArg3, inArg4 )			\
			{																\
				/*FBL_CHECK( mErrorCode >> 12 == kErr >> 12 );*/			\
			}																\
																			\
			NewExc( ERROR_TYPE inErr,										\
				const char* inArg1,											\
				const char* inArg2 = 0,										\
				const char* inArg3 = 0,  									\
				const char* inArg4 = 0 )									\
				: BaseExc( inErr, inArg1, inArg2, inArg3, inArg4 )			\
			{																\
				/*FBL_CHECK( mErrorCode >> 12 == kErr >> 12 );*/			\
			}																\
																			\
			NewExc( ERROR_TYPE inErr,										\
				const String& inFinalMessage,								\
				I_Connection* pConn)										\
				: BaseExc( inErr, inFinalMessage, pConn )					\
			{																\
				/*FBL_CHECK( mErrorCode >> 12 == kErr >> 12 );*/			\
			}																\
																			\
		protected:															\
																			\
	virtual	void			DummyCall( void ) const  {}				\
	};


/**********************************************************************************************/
#define IMPLEMENT_EXCEPTION(NewExc, BaseExc, kErr)\
	NewExc::NewExc( ERROR_TYPE err ) : BaseExc( err ) {}


#pragma mark -

/**********************************************************************************************/
FBL_SHARED_EXP const char* FormatErrorMessage (
	ErrorEntry*  inErrorEntry,
	const char* const inArgs[] );

FBL_SHARED_EXP void FormatErrorMessage_printf(
		UChar*	ioBuffer,
		vuint32	inBufferSize,
		const UChar* inFormatString,
		const UChar* inStr1 = nullptr,
		const UChar* inStr2 = nullptr,
		const UChar* inStr3 = nullptr,
		const UChar* inStr4 = nullptr);

/**********************************************************************************************/
FBL_End_Namespace


/**********************************************************************************************/
#include <VShared/FBL/publ/Errors/FBL_ErrorGroups.h>


/**********************************************************************************************/
#endif // _FBL_Exceptions_h








