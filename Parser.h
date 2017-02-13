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

							/*
							Return code:
							0 - incoming data error
							1 - successful processing of incoming data
							2 - authorization successful
							3 - 
							4 -
							5 -
							*/
		int					ParseDataFromWebSocket( string& data );


	protected:///////////////////////////////////////////////////////////////////

		void				GenerateResponse(
								SSL* inSSL,
								string& inFileName,
								string& inFileType,
								string& inWebSocketKey,
								bool isWebSocket);

};

#endif // _Parser_H