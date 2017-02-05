/**********************************************************************************************/
/* FBL_Mail.cpp																				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>

#if !TARGET_OS_IPHONE

#include <VShared/FBL/publ/Util_classes/FBL_Mail.h>

// STD
#include <sys/stat.h>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <locale>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#include <DbgHelp.h>
#include <io.h>
#include <malloc.h>
#else // _WIN32
#include <alloca.h>
#include <dirent.h>
#include <execinfo.h>
#include <unistd.h>
#endif // _WIN32

// CURL
#if defined(__APPLE__) || defined(__linux__)
#include <curl.h>
#else // defined(__APPLE__) || defined(__linux__) 
#include <curl/curl.h>
#endif // __APPLE__


/**********************************************************************************************/		
#ifdef _WIN32
#pragma warning( disable : 4996 4127 )
#if _MSC_VER < 1900
#define snprintf _snprintf
#endif // MSC_VER
#endif // _WIN32


/**********************************************************************************************/
FBL_Begin_Namespace


/**********************************************************************************************/
struct mail_data
{
	size_t len;
	const char* ptr;
};


/**********************************************************************************************/
// CALLBACK func for CURL.
//
static size_t get_mail_data( void* ptr, size_t size, size_t nmemb, void* user_data )
{
	mail_data* mdata = (mail_data*) user_data;
	
	size_t read = std::min( mdata->len, size * nmemb );
	memcpy( ptr, mdata->ptr, read );
	mdata->len -= read;
	mdata->ptr += read;
	
	return read;
}


#pragma mark -

/**********************************************************************************************/
Mail::Mail(
	const char*		inTo,
	const char*		inFrom,
	const char*		inSMTP,
	int				inSMTP_Port,
	bool			inUseSSL,
	int				inTimeOutInSec )
:
	mValid(true),
	mTo( inTo ),
	mFrom( inFrom ),
	mSMTP( inSMTP ),
	mSMTP_Port( inSMTP_Port ),
	mUseSSL( inUseSSL ),
	mTimeOutInSec( inTimeOutInSec ),
	mBodyIsFileName(false),
	mSendError( CURLE_OK )
{
}


/**********************************************************************************************/
Mail::~Mail()
{
}


/**********************************************************************************************/
void Mail::put_AuthInfo(
	const std::string& inUsrName,
	const std::string& inUsrPswd )
{
	mUsrName = inUsrName;
	mUsrPswd = inUsrPswd;
}


/**********************************************************************************************/
void Mail::put_Subject( const char* inSubject )
{
	mSubject = inSubject;
}


/**********************************************************************************************/
void Mail::put_Body( const char* inBody, bool inIsFileName )
{
	mBody = inBody;
	mBodyIsFileName = inIsFileName;
}


/**********************************************************************************************/
void Mail::put_Attachment(
	const std::string&	inAttachment,
	const std::string&	inAttachmentName,
	bool				inIsFileName )
{
	// This implementation guaranties that all 3 vectors are synched.
	//
	mAttachments.push_back( inAttachment );
	mAttachmentNames.push_back( inAttachmentName );
	mAttachmentIsFileNames.push_back( inIsFileName );
}


/**********************************************************************************************/
bool Mail::send( void )
{
	bool res = false;

	if( mValid )
	{
		std::string mail = get_mail_header();

		if( mAttachments.empty() )
		{
			// Optional body + no attachment.
			PrepareBody( mail, true );
		}
		else
		{
			// Optional body + at least one attachment.
			PrepareAttachments( mail );	
		}
		
		res = send_Alg( mail );
		
		// Drop files.
		if( res )
		{
			if( mBodyIsFileName )
			{
				remove( mBody.c_str() );
				// Because we have to drop files and no much sense to keep 3 vectors synched
				// thinking of multiple "send()" calls.
				mValid = false;
			}
			
			size_t count = mAttachmentIsFileNames.size();
			for( size_t i = 0 ; i < count ; ++i )
			{
				if( mAttachmentIsFileNames[i] )
				{
					remove( mAttachments[i].c_str() );
					// Because we have to drop files and no much sense to keep 3 vectors synched
					// thinking of multiple "send()" calls.
					mValid = false;					
				}
			}
		}
	}
	
	return res;
}


/**********************************************************************************************/		
std::string Mail::get_mail_error( void )
{
	return curl_easy_strerror( (CURLcode)mSendError );
}


#pragma mark -

/**********************************************************************************************/		
bool Mail::send_Alg( const std::string& inData )
{
	mSendError = CURLE_SEND_ERROR;

	mail_data data;
	data.ptr = inData.c_str();
	data.len = inData.length();
	
	CURL* curl = curl_easy_init();
	if( curl )
	{
		curl_slist* recipients = nullptr;
		
		// We believe that "," is a delimitter in case of multiple recepients
		//
		const char delimiter = ',';
		int startPos = 0;
		int endPos = 0;

		// Add all recepients excluding last one.
		while( (endPos = (int)mTo.find( delimiter, startPos )) > -1 )
		{
			recipients = curl_slist_append(
								recipients,
								mTo.substr( startPos, (endPos-startPos)).c_str() );
			startPos = endPos + 1;
		}
		
		// Add last (or single) recepient.
		recipients = curl_slist_append(
								recipients,
								mTo.substr( startPos, size_t(-1) ).c_str() );


		curl_easy_setopt( curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t) 64 );
		curl_easy_setopt( curl, CURLOPT_URL, mSMTP.c_str() );
		curl_easy_setopt( curl, CURLOPT_UPLOAD, 1 );
		curl_easy_setopt( curl, CURLOPT_NETRC, 2L );
		curl_easy_setopt( curl, CURLOPT_MAXREDIRS, 50 );

		#ifndef _WIN32
			curl_easy_setopt( curl, CURLOPT_TCP_KEEPALIVE, 1 );
		#endif // _WIN32

		curl_easy_setopt( curl, CURLOPT_MAIL_FROM, mFrom.c_str() );
		curl_easy_setopt( curl, CURLOPT_MAIL_RCPT, recipients );
		curl_easy_setopt( curl, CURLOPT_TIMEOUT, mTimeOutInSec );
		curl_easy_setopt( curl, CURLOPT_PORT, mSMTP_Port );
		curl_easy_setopt( curl, CURLOPT_VERBOSE, 1 );
		
		if( mUseSSL )
		{
			curl_easy_setopt( curl, CURLOPT_USE_SSL, (long)CURLUSESSL_TRY );
			curl_easy_setopt( curl, CURLOPT_SSL_VERIFYPEER, 0L );
			curl_easy_setopt( curl, CURLOPT_SSL_VERIFYHOST, 0L );
			curl_easy_setopt( curl, CURLOPT_SSLVERSION, 0L );
			curl_easy_setopt( curl, CURLOPT_SSL_SESSIONID_CACHE, 1L );
		}
   
		if( mUsrName.empty() == false )
		{
			curl_easy_setopt( curl, CURLOPT_USERNAME, mUsrName.c_str() );
			curl_easy_setopt( curl, CURLOPT_PASSWORD, mUsrPswd.c_str() );
		}
		
		curl_easy_setopt( curl, CURLOPT_READDATA, &data );
		curl_easy_setopt( curl, CURLOPT_READFUNCTION, get_mail_data );

		mSendError = curl_easy_perform( curl );

		curl_easy_cleanup( curl );
		curl_slist_free_all( recipients );
	}
	
	return mSendError == CURLE_OK;
}


/**********************************************************************************************/
static const char base64chars[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


/**********************************************************************************************/
void Mail::base64_encode(
	std::string&	out,
	const byte*		data,
	size_t			size )
{
	if( !size || !data )
		return;

	long i = 0;
	for( ; i < (long) size - 2 ; i += 3 )
	{
		if( !( i % 54 ) )
			out += '\n';

		out += base64chars[ byte( (   data[ i ] >> 2 ) & 0x3F ) ];
		out += base64chars[ byte( ( ( data[ i ] & 0x3 ) << 4 ) | ( (int) ( data[ i + 1 ] & 0xF0 ) >> 4 ) ) ];
		out += base64chars[ byte( ( ( data[ i + 1 ] & 0xF ) << 2 ) | ( (int) ( data[ i + 2 ] & 0xC0 ) >> 6 ) ) ];
		out += base64chars[ byte(     data[ i + 2 ] & 0x3F ) ];
	}

	if( i < (long) size )
	{
		out += base64chars[ byte( ( data[ i ] >> 2 ) & 0x3F ) ];
        
		if( i == (long) size - 1 )
		{
			out += base64chars[ byte( ( ( data[ i ] & 0x3 ) << 4 ) ) ];
			out += '=';
		}
		else 
		{
			out += base64chars[ byte( ( ( data[ i ] & 0x3 ) << 4 ) | ( (int) ( data[ i + 1 ] & 0xF0 ) >> 4 ) ) ];
			out += base64chars[ byte( ( ( data[ i + 1 ] & 0xF ) << 2 ) ) ];
		}

		out += '=';
	}
}


/**********************************************************************************************/
void Mail::bin2str( char* str, const unsigned char* bin, size_t len )
{
	for( ; len-- ; ++bin )
	{
		*str++ = "0123456789abcdef"[ *bin >> 4 ];
		*str++ = "0123456789abcdef"[ *bin & 0x0F ];
	}
	
	*str = '\0';
}


/**********************************************************************************************/		
std::string Mail::get_mail_header( void )
{
	time_t t = time( nullptr );
	struct tm* gmt = gmtime( &t );
	
	char tbuf[ 512 ];
	
	try
	{
		static std::locale deflocale( "C" );
		std::locale old = std::locale::global( deflocale );

		strftime( tbuf, 512, "%a, %d %b %Y %H:%M:%S +0300", gmt );

		std::locale::global( old );
	}
	catch(...)
	{
		strftime( tbuf, 512, "%a, %d %b %Y %H:%M:%S +0300", gmt );
	}	

	char id[ 64 ];
	bin2str( id, (unsigned char*) tbuf, 8 );
    
	std::string str;
	str += "Subject: " + mSubject + "\n";
	str += "Return-Path: " + mFrom + "\n";
	str += std::string("Message-ID: <") + id + "@valentina-db.com>\n";
	str += std::string("Date: ") + tbuf + "\n";
	str += "From: " + mFrom + "\n";
	str += "X-Mailer: Valentina Soft\n";
	str += "MIME-Version: 1.0\n";
	str += std::string("To: ") + mTo + "\n";
	
	return str;
}


/**********************************************************************************************/
void Mail::PrepareBody( std::string& outMail, bool inWriteMailHeader )
{
	if( mBody.empty() == false )
	{
		if( inWriteMailHeader )
			outMail += "Content-Type: text/plain; charset=us-ascii\n\n";

		// If body contains file name we have to read content from that file
		// and put body "as is" otherwise.
		//
		if( mBodyIsFileName )
		{
			#ifdef _WIN32
				FILE* f = fopen( mBody.c_str(), "rb" );
			#else // _WIN32
				FILE* f = fopen( mBody.c_str(), "rt" );
			#endif // _WIN32

			if( f )
			{
				char buf[ 1024 ];
				while( !feof( f ) )
				{
					fgets( buf, 1024, f );
					outMail += buf;
				}
				
				fclose( f );
			}
		}
		else
		{
			outMail += mBody;		
		}
	}
}


/**********************************************************************************************/		
void Mail::PrepareAttachments( std::string& outMail )
{
	size_t count = mAttachments.size();
	
	// One time for all attachments if any.
	if( count )
	{
		outMail += "Content-Type: multipart/mixed;\n boundary=\"------------000407030207070300090807\"\n\n";
		outMail += "This is a multi-part message in MIME format.\n";
		outMail += "--------------000407030207070300090807\n";
		outMail += "Content-Type: text/plain; charset=UTF-8; format=flowed\n";
		outMail += "Content-Transfer-Encoding: 8bit\n\n";

		// BODY before first attachment only.
		PrepareBody( outMail, false );
		outMail += "\n";
	}
	
	// For each attachment.
	for( size_t i = 0 ; i < count ; ++i )
	{
		outMail += "\n--------------000407030207070300090807\n";
		outMail += "Content-Type: application/octet-stream;\n name=\"";
		outMail += (mAttachmentNames[i].empty()) ? "file" : mAttachmentNames[i];
		outMail += "\"\n";
		outMail += "Content-Transfer-Encoding: base64\n";
		outMail += "Content-Disposition: attachment;\n filename=\"";
		outMail += (mAttachmentNames[i].empty()) ? "file" : mAttachmentNames[i];
		outMail += "\"\n";

		// If current attachment contains file name we have to read content from that file
		// and put attachment "as is" otherwise.
		//
		if( mAttachmentIsFileNames[i] )
		{
			#ifdef _WIN32
				FILE* f = fopen( mAttachments[i].c_str(), "rb" );
			#else // _WIN32
				FILE* f = fopen( mAttachments[i].c_str(), "rt" );
			#endif // _WIN32

			if( f )
			{
				fseek( f, 0, SEEK_END );
				size_t size = ftell( f );
				char* buf = (char*) malloc( size );
				if( buf )
				{
					rewind( f );
					size = fread( buf, 1, size, f );
							
					base64_encode( outMail, (const byte*) buf, size );
					free( buf );
				}
				fclose( f );
			}
		}
		else
		{
			// We believe that attachment is base64-encoded
			outMail += mAttachments[i];
		}
	}

	// One time for all attachments if any.
	if( count )
		outMail += "\n--------------000407030207070300090807--\n";
}


/**********************************************************************************************/
FBL_End_Namespace

#endif // !TARGET_OS_IPHONE