/*******************************************************************************/
#include "Parser.h"

#include "ControlsDatabase.h"
#include "MyCover.h"
#include "LogFile.h"
#include "WSLexer.h"


/*******************************************************************************/
int Parser::ParseHttpHEAD( SSL* inSSL )
{
	string buf;

	try
	{
		// We are getting string with HTTTP HEAD

		MyCover mySendRecv;
		mySendRecv.my_recv(inSSL, buf);

		WSLexer mLexer;
		const char* buff_s = buf.c_str();
		mLexer.Put_HttpRequest( buff_s,  buff_s + buf.size() );

		bool isGET = false;
		bool isProtocolVersion_1_1 = false;
		bool isWebSocket = false;
		bool isWebSocketVersion_13 = false;
		string FileName;
		string WebSocketKey;


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
					FileName = string(prevToken.ps, prevToken.mLen) + "." + string(currToken.ps, currToken.mLen);
				else
					continue; // we have to go to the next iteration of the loop without using function GetNextToken
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
					ContinueToCycle = mLexer.GetNextToken(&currToken, true);
					WebSocketKey = string(currToken.ps, currToken.mLen);
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
	}
	catch(exception& e)
	{
		LogFile log;
		log.write(e.what());
	}

	return 0;
}


/*******************************************************************************/
void Parser::ParseDataFromWebSocket(string& data)
{
		string hash_login = string();
		string hash_pass = string();
		string hash_token = string();
		string login = "Login";
		string password = "Password";
		string s_token = "Token";
		string temp = string();
		string Name = string();

		bool dataEnd = false;
		bool check_login = false;
		bool check_pass = false;
		bool check_token = false;
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
					temp = string(token.ps, token.pe);

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

					if (s_token == temp)
					{
						check_token = true;
						temp.clear();

						break;
					}

					// if msg

					break;
				}

				case wsBracketsSymbolType:
				{
					
					if (check_login)
					{
						lexer.GetNextToken(&token);

						if (token.mType == wsDefaultType)
						{
							hash_login = string(token.ps, token.pe);
							check_login = false;
							//check db
						}
					}

					if (check_pass)
					{
						lexer.GetNextToken(&token);

						if (token.mType == wsDefaultType)
						{
							hash_pass = string(token.ps, token.pe);
							check_pass = false;
							//check db
						}
					}

					if (check_token)
					{
						if (lexer.GetNextToken(&token))
						{
							hash_token = string(token.ps, token.pe);
							check_token = false;
						}
					}

					break;
				}

				default:
				{
					// unknown data
					login.clear();
					password.clear();
					s_token.clear();
					temp.clear();

					break;
				}
			}
		} while (dataEnd);

		if (hash_login.size() > 100 && hash_pass.size() > 100 && (hash_token.size() > 10 && hash_token.size() < 50))
		{
			// Check the authorization data in DB
			authData = CD.FindAuthData(hash_login.c_str(), hash_pass.c_str(), hash_token.c_str());

			if (authData)
			{
				data = CD.get_file_from_db("chat.html");
			}
		}

		if (!authData && (hash_token.size() > 10 && hash_token.size() < 50))
		{
			authToken = CD.check_token(hash_token.c_str(), Name);

			if (authToken)
			{
				data = Name + ": " + data.erase(0, hash_token.size() + 7);
			}
		}

		if (!authData && !authToken)
		{
			data = "error";
		}
}