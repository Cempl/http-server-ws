/*******************************************************************************/
#include "MyCover.h"
#include "LogFile.h"
#include "Server.h"
#include "WSLexer.h"


/*******************************************************************************/
int ParseHttpHEAD( SSL* inSSL )
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