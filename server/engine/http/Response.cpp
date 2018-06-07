/*******************************************************************************/
#include "Response.h"

#include "ControlsDatabase.h"
#include "MyCover.h"
#include "LogFile.h"
#include "WSLexer.h"


/*******************************************************************************/
void Response::sendResponse(SSL* inSSL)
{
    ParseHttpHEAD(inSSL);
    GenerateResponse(inSSL);
}


/*******************************************************************************/
void Response::ParseHttpHEAD(SSL* inSSL)
{
    try
    {
        // TODO: Have to update after changed internal strucutre of project.
        MyCover mySendRecv;
        string buf;
        mySendRecv.my_recv(inSSL, buf);

        cout << buf;

        WSLexer mLexer(buf);

        while( mLexer.NextToken(true) )
        {
            string strCurrToken(mLexer.getToken()->getStart(), mLexer.getToken()->getLen());

            if( cfgResponse.requestMethod == RequestMethodsList::ErrorType )
            {
                if( cfgResponse.requestMethod = getRequestMethod(strCurrToken) )
                {
                    continue;
                }
            }

            if(     cfgResponse.fileType == FileTypeList::fErrorType
                &&  mLexer.getToken()->getPrevToken()->getType() == wsTokenTypes::wsSymbolType
                &&  *(mLexer.getToken()->getPrevToken()->getStart()) == '.'
                )
            {
                if( cfgResponse.fileType = getFileType(strCurrToken) )
                {
                    cfgResponse.fileName = string(mLexer.getToken()->getPrevToken()->getPrevToken()->getStart(),
                        mLexer.getToken()->getPrevToken()->getPrevToken()->getLen());
                    cfgResponse.fileName += "." + strCurrToken;
                    continue;
                }
            }

            if( cfgResponse.requestMethod == RequestMethodsList::ErrorType )
            {
                if( cfgResponse.requestMethod = getRequestMethod(strCurrToken) )
                {
                    continue;
                }
            }

            if(     false == cfgResponse.isWebSocket
                ||  false == cfgResponse.isCorretVersion
                ||  cfgResponse.keyWebSocket.empty() )
            {
                WebSocketFieldType fieldType = WebSocketFieldType::ErrorType;

                if( fieldType = getWebSockFieldType(strCurrToken) )
                {
                    mLexer.NextToken(true);
                    if(     *(mLexer.getToken()->getStart()) == ':' 
                        &&  mLexer.getToken()->getType() == wsTokenTypes::wsSymbolType)
                    {
                        mLexer.NextToken(true);

                        string strInform;
                        while(mLexer.getToken()->getType() != wsTokenTypes::wsNewLineSymbolType);
                        {
                            strInform += string(mLexer.getToken()->getStart(), mLexer.getToken()->getLen());
                            mLexer.NextToken(true);
                        }

                        setWebSockFieldData(fieldType, strInform);
                    }
                }
            }
        }

        // Generate response
    }
    catch (exception& e)
    {
        LogFile log;
        log.write(e.what());
    }
}


/*******************************************************************************/
/*int Response::ParseDataFromWebSocket(string& data)
{
	int res = 0;

	// restrict data length
	if (data.size() <= 5000)
	{
		string hash_login = string();
		string hash_pass = string();
		string hash_token = string();
		string current_message = data;
		string login = "Login";
		string password = "Password";
		string session_token = "Token";
		string message = "Message";
		string temp = string();
		string Name = string();

		bool dataEnd = false;
		bool check_login = false;
		bool check_pass = false;
		bool check_token = false;
		bool check_message = false;
		bool valid_login = false;
		bool valid_pass = false;
		bool valid_token = false;
		bool presence_token = false;
		bool authToken = false;
		bool authData = false;

		WSLexer lexer;

		lexer.Put_HttpRequest(data.c_str(), (data.size() + data.c_str()));

		WSLexer::Token token;

		do
		{
			dataEnd = lexer.GetNextToken(&token);

			switch (token.mType)
			{
				case wsDefaultType:
				{
					temp = string(token.ps, token.mLen);

					if (login == temp)
					{

						check_login = true;
						temp.clear();

						break;
					}

					if (password == temp)
					{

						check_pass = true;
						temp.clear();

						break;
					}

					if (session_token == temp)
					{
						check_token = true;
						temp.clear();

						break;
					}

					if (message == temp)
					{
						check_message = true;
						temp.clear();
					}

					break;
				}

				case wsBracketsSymbolType:
				{

					if (check_login)
					{
						lexer.GetNextToken(&token);

						if (token.mType == wsDefaultType)
						{
							if (token.mLen > 100 && token.mLen < 150)
							{
								hash_login = string(token.ps, token.pe);
								check_login = false;
								valid_login = true;
							}
						}
					}

					if (check_pass)
					{
						lexer.GetNextToken(&token);

						if (token.mType == wsDefaultType)
						{
							if (token.mLen > 100 && token.mLen < 150)
							{
								hash_pass = string(token.ps, token.pe);
								check_pass = false;
								valid_pass = true;
							}
						}
					}

					if (check_token)
					{
						if (lexer.GetNextToken(&token))
						{
							if (token.mLen > 30 && token.mLen < 50)
							{
								hash_token = string(token.ps, token.pe);

								// erase current token and mark "Token[]" of data
								current_message.erase(0, token.mLen + 7);

								check_token = false;
								valid_token = true;
							}
						}
					}

					if (check_message)
					{
						if (lexer.GetNextToken(&token))
						{	
							// erase mark "Message[" of data
							current_message.erase(0, 8);

							// erase mark "]" of data
							current_message.erase(current_message.end() - 1);

							check_message = false;
						}
					}

					break;
				}

				default:
				{
					// unknown data
					login.clear();
					password.clear();
					session_token.clear();
					temp.clear();

					break;
				}
			}
		} while (dataEnd);

		if (valid_login && valid_pass && valid_token)
		{
			// Check the authorization data in DB
			authData = CD.AcceptAuthData(hash_login.c_str(), hash_pass.c_str(), hash_token.c_str());

			if (authData)
			{
				data = CD.get_file_from_db("chat.html");

				res = 2;
			}
		}

		if (!authData && valid_token && !valid_login && !valid_pass)
		{
			authToken = CD.check_token(hash_token.c_str(), Name);

			if (authToken)
			{
				data = Name + "  " + current_message;

				res = 1;
			}
		}

		if (!authData && !authToken)
		{
			data = "error";
		}
	}
	else
	{
		data = "error data length";
	}

	return res;
}*/


/*******************************************************************************/
void Response::GenerateResponse( SSL* inSSL )
{
	if(!cfgResponse.isWebSocket)
	{
		string HTTPResponse;

		if(     cfgResponse.requestMethod == RequestMethodsList::GET
            &&  cfgResponse.HTTTPVersion == HTTPVersionsList::HTTP11)
			HTTPResponse = "HTTP/1.1 200 OK\n";
		else
			throw exception("Error: Unknow type of request or protocol version");

		HTTPResponse += "Server: VaV/V2\n";

		switch(cfgResponse.fileType)
		{
			case fHtmlType:
			{
				HTTPResponse += "Content-Type: text/html;\n";
				HTTPResponse += "Connection: keep-alive\n";
				HTTPResponse += "X-Powered-By: c++\r\n\r\n";

			}break;

			case fCssType:
			{
				HTTPResponse += "Content-Type: text/css;\r\n\r\n";

			}break;

			case fJSType:
			{
				HTTPResponse += "Content-Type: application/javascript;\r\n\r\n";
			
			}break;

			case fImageType:
			{
				HTTPResponse += "Content-Type: image/png;\r\n\r\n";
		
			}break;

			case fErrorType:
			default:
			{
				throw exception("Error: Unknow type of file");
		
			}break;
		}

		HTTPResponse += CD.get_file_from_db(cfgResponse.fileName);

		MyCover mySend;
		mySend.my_send(inSSL, HTTPResponse, true);
	}
	else
	{
		if(cfgResponse.isCorretVersion)
		{
			SR.websocket_handshake(inSSL, cfgResponse.keyWebSocket);

			MyCover myCleanUp;
			myCleanUp.my_Cleanup_OpenSSL(inSSL);
		}
		else
		{
			throw exception("Error: Unknow WebSocket version");
		}
	}

}


/*******************************************************************************/
Response::FileTypeList Response::getFileType(const string& inFileType)
{
    FileTypeList res = FileTypeList::fErrorType;

    // I think this is bad design. But I don't know how to do it differently
    // || (inFileName == "" && inFileType == ""). Browser should guarantee that every time we receive some name of file and extension.
    if(inFileType == "html" )
    {
        res = FileTypeList::fHtmlType;
        goto getFileTypeExitMark;
    }

    if(inFileType == "css")
    {
        res = FileTypeList::fCssType;
        goto getFileTypeExitMark;
    }

    if(inFileType == "js")
    {
        res = FileTypeList::fJSType;
        goto getFileTypeExitMark;
    }

    if( inFileType == "jpg"		||
        inFileType == "tiff"	||
        inFileType == "png"		||
        inFileType == "jpeg"	||
        inFileType == "gif"		||
        inFileType == "ico"		||
        inFileType == "svg")
    {
        res = FileTypeList::fImageType;
        goto getFileTypeExitMark;
    }


getFileTypeExitMark:
    return res;
}


/*******************************************************************************/
Response::RequestMethodsList Response::getRequestMethod(const string& inRequestMethod)
{
    RequestMethodsList res = RequestMethodsList::ErrorType;

    if(inRequestMethod == "GET")
    {
        res = RequestMethodsList::GET;
        goto getRequestMethodExitMark;
    }

    if(inRequestMethod == "HEAD")
    {
        res = RequestMethodsList::HEAD;
        goto getRequestMethodExitMark;
    }

    if(inRequestMethod == "POST")
    {
        res = RequestMethodsList::POST;
        goto getRequestMethodExitMark;
    }

    if(inRequestMethod == "PUT")
    {
        res = RequestMethodsList::PUT;
        goto getRequestMethodExitMark;
    }

    if(inRequestMethod == "DELETE")
    {
        res = RequestMethodsList::DEL;
        goto getRequestMethodExitMark;
    }

    if(inRequestMethod == "CONNECT")
    {
        res = RequestMethodsList::CONNECT;
        goto getRequestMethodExitMark;
    }

    if(inRequestMethod == "OPTIONS")
    {
        res = RequestMethodsList::OPTIONS;
        goto getRequestMethodExitMark;
    }

    if(inRequestMethod == "TRACE")
    {
        res = RequestMethodsList::TRACE;
        goto getRequestMethodExitMark;
    }

getRequestMethodExitMark:
    return res;
}


/*******************************************************************************/
Response::HTTPVersionsList Response::getHTTPVersion(const string& inHTTPVersion)
{
    HTTPVersionsList res = HTTPVersionsList::ErrorType;

    if(inHTTPVersion == "0.9")
    {
        res = HTTPVersionsList::HTTP09;
        goto getHTTPVersionExitMark;
    }

    if(inHTTPVersion == "1.0")
    {
        res = HTTPVersionsList::HTTP10;
        goto getHTTPVersionExitMark;
    }

    if(inHTTPVersion == "1.1")
    {
        res = HTTPVersionsList::HTTP11;
        goto getHTTPVersionExitMark;
    }

    if(inHTTPVersion == "2")
    {
        res = HTTPVersionsList::HTTP20;
        goto getHTTPVersionExitMark;
    }

getHTTPVersionExitMark:
    return res;
}


/*******************************************************************************/
Response::WebSocketFieldType Response::getWebSockFieldType(const string& inStrField)
{
    WebSocketFieldType res = WebSocketFieldType::ErrorType;

    if( inStrField == "Upgrade")
    {
        res = WebSocketFieldType::Upgrade;
        goto getWebSockFieldTypenExitMark;
    }

    if( inStrField == "Sec-WebSocket-Key")
    {
        res = WebSocketFieldType::WebSockKey;
        goto getWebSockFieldTypenExitMark;
    }

    if( inStrField == "Sec-WebSocket-Version")
    {
        res = WebSocketFieldType::WebSockVersion;
        goto getWebSockFieldTypenExitMark;
    }

getWebSockFieldTypenExitMark:
    return res;
}


/*******************************************************************************/
void Response::setWebSockFieldData(const WebSocketFieldType& inType, const string& inInform)
{
    switch( inType )
    {
        case WebSocketFieldType::Upgrade:
        {
            // TODO: When std::string change to QString need use function compare
            cfgResponse.isWebSocket = inInform == "WebSocket";
            break;
        }

        case WebSocketFieldType::WebSockVersion:
        {
            // TODO: When std::string change to QString need use function compare
            cfgResponse.isCorretVersion = inInform == "13";
            break;
        }

        case WebSocketFieldType::WebSockKey:
        {
            cfgResponse.keyWebSocket = inInform;
            break;
        }

        case WebSocketFieldType::ErrorType:
        default:
        {
            break;
        }
    }
}