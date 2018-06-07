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

        enum FileTypeList
        {
            fErrorType = 0,

            fHtmlType,
            fCssType,
            fJSType,
            fImageType
        };

        enum RequestMethodsList
        {
            ErrorType = 0,

            GET,
            HEAD,
            POST,
            PUT,
            DEL, // This is an abbreviated name because DELETE is macros
            CONNECT,
            OPTIONS,
            TRACE
        };

        enum HTTPVersionsList
        {
            ErrorType = 0,

            HTTP09,
            HTTP10,
            HTTP11,
            HTTP20
        };

        enum WebSocketFieldType
        {
            ErrorType = 0,

            Upgrade,
            WebSockKey,
            WebSockVersion
        };

        struct
        {
            FileTypeList fileType = FileTypeList::fErrorType;
            RequestMethodsList requestMethod = RequestMethodsList::ErrorType;
            HTTPVersionsList HTTTPVersion = HTTPVersionsList::ErrorType;

            string fileName;

            bool isWebSocket = false;
            bool isCorretVersion = false; // Version of websocket
            string keyWebSocket;

        }cfgResponse;


	public://////////////////////////////////////////////////////////////////////

		                    Response() {};
		                    Response(const Response& inOther) = delete;
		                    ~Response() {};


	public://////////////////////////////////////////////////////////////////////

        void                sendResponse(SSL* inSSL);
							/*
							Return code:
							0 - incoming data error
							1 - successful processing of incoming data
							2 - authorization successful
							3 - 
							4 -
							5 -
							*/
		//int					ParseDataFromWebSocket( string& data );


	private:///////////////////////////////////////////////////////////////////

		void				ParseHttpHEAD( SSL* inSSL );
        void				GenerateResponse( SSL* inSSL );

        FileTypeList        getFileType(const string& inFileType);
        RequestMethodsList  getRequestMethod(const string& inRequestMethod);
        HTTPVersionsList    getHTTPVersion(const string& inHTTPVersion);
        WebSocketFieldType  getWebSockFieldType(const string& inStrField);

        void                setWebSockFieldData(const WebSocketFieldType& inType, const string& inInform);

	private:///////////////////////////////////////////////////////////////////

		SendRecv			SR;


};

#endif // _Response_H