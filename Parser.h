#ifndef _Parser_H
	#define _Parser_H
#pragma once


/*******************************************************************************/
#include "Server.h"


/*******************************************************************************/
class Response
{
	public://////////////////////////////////////////////////////////////////////

		Response() {};
		Response(const Response& inOther) = delete;
		~Response() {};


	public://////////////////////////////////////////////////////////////////////

		int					ParseHttpHEAD( SSL* inSSL );
		void				ParseDataFromWebSocket(string& data);


	protected:///////////////////////////////////////////////////////////////////
		void				GenerateResponse(
								SSL* inSSL,
								string& inFileName,
								string& inFileType,
								string& inWebSocketKey,
								bool isWebSocket);

};

#endif // _Parser_H