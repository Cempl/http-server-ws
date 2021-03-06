/*******************************************************************************/
#include "Response.h"

#include "ControlsDatabase.h"
#include "MyCover.h"
#include "LogFile.h"
#include "WSLexer.h"


/*******************************************************************************/
//Helping methods


/*******************************************************************************/
enum FileTypeList
{
	fErrorType = 0,

	fHtmlType,
	fCssType,
	fJSType,
	fImageType
};


/*******************************************************************************/
FileTypeList getFileType(string& inFileType, string& inFileName)
{
	FileTypeList res = fErrorType;

	// I think this is bad design. But I do not know how to do it differently
	if( inFileType == "html" || (inFileName == "" && inFileType == ""))
		res = fHtmlType;

	if( inFileType == "css" )
		res = fCssType;

	if( inFileType == "js" )
		res = fJSType;

	if( inFileType == "jpg"		||
		inFileType == "tiff"	||
		inFileType == "png"		||
		inFileType == "jpeg"	||
		inFileType == "gif"		||
		inFileType == "ico"		||
		inFileType == "svg")
		res = fImageType;

	return res;
}


/*******************************************************************************/
//Class methods


/*******************************************************************************/
int Response::ParseHttpHEAD( SSL* inSSL )
{
	string buf;

	bool isGET = false;
	bool isProtocolVersion_1_1 = false;
	bool isWebSocket = false;
	bool isWebSocketVersion_13 = false;
	string FileName;
	string FileType;
	string WebSocketKey;

	try
	{
		// We are getting string with HTTTP HEAD

		MyCover mySendRecv;
		mySendRecv.my_recv(inSSL, buf);

		cout << buf; 

		WSLexer mLexer;
		const char* buff_s = buf.c_str();
		mLexer.Put_HttpRequest( buff_s,  buff_s + buf.size() );

		bool ContinueToCycle = false;
		WSLexer::Token currToken;
		WSLexer::Token prevToken;

		ContinueToCycle = mLexer.GetNextToken(&currToken, true);
		while(ContinueToCycle)
		{
			string strToken(currToken.ps, currToken.mLen);

			// Find type of request
			if( currToken.mPosition == 0 && currToken.mLine == 1 && strToken == "GET" )
				isGET = true;

			// Find FileName
			if( currToken.mLine == 1 && prevToken.mType == wsDefaultType && strToken == "." )
			{
				ContinueToCycle = mLexer.GetNextToken(&currToken, true);
				strToken = string(currToken.ps, currToken.mLen);

				if( currToken.mType == wsDefaultType && currToken.mLine == 1 )
				{
					FileType = string(currToken.ps, currToken.mLen);
					FileName = string(prevToken.ps, prevToken.mLen);
				}	
				else
				{
					continue; // we have to go to the next iteration of the loop without using function GetNextToken
				}
			}

			// Check version of HTTP
			if( currToken.mLine == 1 && strToken == "HTTP")
			{
				ContinueToCycle = mLexer.GetNextToken(&currToken, true);
				if(currToken.mLine == 1 && currToken.mType == wsSymbolType && *(currToken.ps) == '/' )
				{
					ContinueToCycle = mLexer.GetNextToken(&currToken, true);
					strToken = string(currToken.ps, currToken.mLen);

					if( strToken != "2")
					{
						ContinueToCycle = mLexer.GetNextToken(&currToken, true);
						strToken += string(currToken.ps, currToken.mLen);
						ContinueToCycle = mLexer.GetNextToken(&currToken, true);
						strToken += string(currToken.ps, currToken.mLen);
						
						if( strToken == "1.1")
						{
							isProtocolVersion_1_1 = true;
						}
					}
				}
				else
				{
					continue; // we have to go to the next iteration of the loop without using function GetNextToken
				}

			}

			// This is WebSocket? If not we stop parcing
			if( strToken == "Upgrade" )
			{
				ContinueToCycle = mLexer.GetNextToken(&currToken, true);

				if( string(currToken.ps, currToken.mLen) == ":" )
				{
					ContinueToCycle = mLexer.GetNextToken(&currToken, true);
					strToken = string(currToken.ps, currToken.mLen);
					if( strToken == "websocket" || strToken == "WebSocket" )
						isWebSocket = true;
					else
						break;
				}
				else
				{
					continue; // we have to go to the next iteration of the loop without using function GetNextToken
				}
			}

			// Find WebSocket Key
			if( strToken == "Sec-WebSocket-Key" )
			{
				ContinueToCycle = mLexer.GetNextToken(&currToken, true);
				if( string(currToken.ps, currToken.mLen) == ":" )
				{
					string PartOfKey;

					do
					{
						WebSocketKey += PartOfKey;
						ContinueToCycle = mLexer.GetNextToken(&currToken, true);
						PartOfKey = string(currToken.ps, currToken.mLen);
					}
					while(PartOfKey != "\n" && PartOfKey != "\r");
				}
				else
				{
					continue; // we have to go to the next iteration of the loop without using function GetNextToken
				}
			}

			// Check version of WebSocket? we work only with version 13
			if( strToken == "Sec-WebSocket-Version" )
			{
				ContinueToCycle = mLexer.GetNextToken(&currToken, true);
				if( string(currToken.ps, currToken.mLen) == ":" )
				{
					ContinueToCycle = mLexer.GetNextToken(&currToken, true);
					if(string(currToken.ps, currToken.mLen) == "13")
						isWebSocketVersion_13 = true;
				}
				else
				{
					continue; // we have to go to the next iteration of the loop without using function GetNextToken
				}
			}

			prevToken = currToken;
			ContinueToCycle = mLexer.GetNextToken(&currToken, true);
		}

		GenerateResponse(
			inSSL, 
			isGET, 
			isProtocolVersion_1_1,
			isWebSocket,
			isWebSocketVersion_13,
			FileName,
			FileType,
			WebSocketKey);
	}
	catch(exception& e)
	{
		LogFile log;
		log.write(e.what());
	}

	return 0;
}


/*******************************************************************************/
int Response::ParseDataFromWebSocket(string& data)
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
}


/*******************************************************************************/
void Response::GenerateResponse(
					SSL* inSSL,
					bool isGET,
					bool isProtocolVersion_1_1,
					bool isWebSocket,
					bool isWebSocketVersion_13,
					string& inFileName,
					string& inFileType,
					string& inWebSocketKey)
{
	if(!isWebSocket)
	{
		string HTTPResponse;

		if(isGET && isProtocolVersion_1_1)
			HTTPResponse = "HTTP/1.1 200 OK\n";
		else
			throw exception("Error: Unknow type of request or protocol version");

		HTTPResponse += "Server: VaV/V2\n";

		switch(getFileType(inFileType, inFileName))
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

		if(inFileName == "" && inFileType == "")
			HTTPResponse += CD.get_file_from_db("index.html");
		else
			HTTPResponse += CD.get_file_from_db(inFileName + "." + inFileType);

		MyCover mySend;
		mySend.my_send(inSSL, HTTPResponse, true);
	}
	else
	{
		if(isWebSocketVersion_13)
		{
			SR.websocket_handshake(inSSL, inWebSocketKey);

			MyCover myCleanUp;
			myCleanUp.my_Cleanup_OpenSSL(inSSL);
		}
		else
		{
			throw exception("Error: Unknow WebSocket version");
		}
	}

}