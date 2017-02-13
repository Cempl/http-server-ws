#ifndef _Response_H
	#define _Response_H
#pragma once


/*******************************************************************************/
#include "Server.h"
#include "SendRecv.h"


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
								bool isGET,
								bool isProtocolVersion_1_1,
								bool isWebSocket,
								bool isWebSocketVersion_13,
								string& inFileName,
								string& inFileType,
								string& inWebSocketKey);

	protected:///////////////////////////////////////////////////////////////////

	SendRecv SR;
};

#endif // _Response_H