#ifndef _ResponseRequest_H
	#define _ResponseRequest_H
#pragma once


/*******************************************************************************/
#include "MyCover.h"
#include "SendRecv.h"
#include "Server.h"


/*******************************************************************************/
class ResponseRequest : protected MyCover
{
	public://////////////////////////////////////////////////////////////////////

				ResponseRequest() {};
				ResponseRequest(const ResponseRequest& InOther) = delete;
				~ResponseRequest() {};


	public:///////////////////////////////////////////////////////////////////

		int		Request(SSL* inSSL);
		void	Parse_name_page(string& inNamePage, string& outNamePage, string& outExtensionFile, bool& flag);
		void	Send_response(SSL* inSSL, string &data);
		void	Cleaning_refuse_in_buffer(const string& InBuf, vector<string>& OutEntire_query);
		void	Cleanup_OpenSSL(SSL* inSSL);


	protected:///////////////////////////////////////////////////////////////////

		SendRecv SR;
};

#endif // _ResponseRequest_H