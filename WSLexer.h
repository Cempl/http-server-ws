#ifndef _WSLexer_H
	#define _WSLexer_H
#pragma once

#include "Server.h"


enum wsTokenTypes
{
	wsDefaultType = 0,

	wsSpaceType,
	wsSymbolType,
	wsDoublesSymbol
};


/*******************************************************************************/
class WSLexer
{
	public:////////////////////////////////////////////////////////////////////

	struct Token
	{
		wsTokenTypes		mType			= wsDefaultType; // Type of the Token

		int					mLen			= 0; // Length in chars of this token
		int					mPosition		= 0; // position of token (in chars) from the beginning of string.

		const char*			ps				= nullptr; // start
		const char*			pe				= nullptr; // after the end char

		void				Clear()
							{
								mType		= wsDefaultType;
								mLen		= 0;
								mPosition	= 0;

								ps			= nullptr;
								pe			= nullptr;
							}
	};

	public:////////////////////////////////////////////////////////////////////

							WSLexer();
							~WSLexer();

							// This method resets Lexer to a new string
		void				Put_HttpRequest(string inHttpRequest);

							// This method read http request and create Tokens
							// For GET tokens you must use method GetNextToken
		void				GetAllTokens();

							// This method GET next token
							// For GET first token you must use this method
		void				GetNextToken( Token* outToken);

	protected://///////////////////////////////////////////////////////////////

		const char*			mpHttpStr		= nullptr;
		const char*			mpHttpStrEnd	= nullptr;

		const char*			mpCurrChar		= nullptr;

		int					mLine 			= 1; 		// the current line in command.
};

#endif // _WSLexer_H