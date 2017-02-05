/**********************************************************************************************/
/* FBL_Mail.h																				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_Mail_h
	#define _FBL_Mail_h
#pragma once

#if !TARGET_OS_IPHONE

#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>

// STD
#include <string>
#include <vector>


/**********************************************************************************************/
FBL_Begin_Namespace

/**********************************************************************************************/
/// Helper class, to send emails using curl.
/// Usage: Create Mail object, put subject, body, attachments (arbitrarily order) and call send().
///
class FBL_SHARED_EXP_CLASS Mail
{
	public://///////////////////////////////////////////////////////////////////////////////////
 
 							Mail(
								const char*	inTo,
								const char*	inFrom,
								const char*	inSMTP,
								int			inSMTP_Port,
								bool		inUseSSL,
								int			inTimeOutInSec = 10 );

virtual						~Mail();
 
 	public://///////////////////////////////////////////////////////////////////////////////////

		void				put_AuthInfo(
								const std::string& inUsrName,
								const std::string& inUsrPswd );

		void				put_Subject( const char* inSubject );
		
							/** inIsFileName == true means that inBody is a file name and
								we have to read actual body text from that file.
								Also we will drop that file in case of successfull mail sending.
							*/
		void				put_Body( const char* inBody, bool inIsFileName );

							/** inAttachment - actual data to be sent or file name.
								inAttachmentName - attachment name in message.
								inIsFileName == true means that inAttachment is a file name and
								we have to read and base64 encoded actual attachment text from 
								that file.
								Also we will drop that file in case of successfull mail sending.
							*/		
		void				put_Attachment(
								const std::string& inAttachment,
								const std::string& inAttachmentName,
								bool inIsFileName );

		bool				send( void );

		std::string			get_mail_error( void );


	protected://////////////////////////////////////////////////////////////////////////////////

		bool				send_Alg( const std::string& inData );

		typedef unsigned char byte;

		void				base64_encode(
								std::string&	out,
								const byte*		data,
								size_t			size );

 
		void				bin2str( char* str, const unsigned char* bin, size_t len );
		
		std::string			get_mail_header( void );

		void				PrepareBody( std::string& outMail, bool inWriteMailHeader );
		
		void				PrepareAttachments( std::string& outMail );


	protected://////////////////////////////////////////////////////////////////////////////////
 
							// false if this object can not be used to send mails anymore.
		bool				mValid;
		
		std::string			mTo;
		std::string			mFrom;
		std::string			mSMTP;
		int					mSMTP_Port;
		bool				mUseSSL;
		int					mTimeOutInSec;
		std::string			mUsrName;
		std::string			mUsrPswd;

		std::string			mSubject;
		
		std::string			mBody;
		bool				mBodyIsFileName;
		
		std::vector<std::string>	mAttachments;
		std::vector<std::string>	mAttachmentNames;
		std::vector<bool>			mAttachmentIsFileNames;
		
		int					mSendError;
};


/**********************************************************************************************/
FBL_End_Namespace

#include <VShared/FBL/publ/Headers/FBL_post_header.h>

#endif // !TARGET_OS_IPHONE
#endif // _FBL_Mail_h

