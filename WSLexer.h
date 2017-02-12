#ifndef _WSLexer_H
	#define _WSLexer_H
#pragma once

#include "Server.h"


/*******************************************************************************/
enum wsTokenTypes
{
	wsDefaultType = 0,

	wsSpaceType,
	wsSymbolType, // : ; . , / \ 
	wsNewLineSymbolType, // \n \r
	wsBracketsSymbolType, // () []
	wsQuotesSymbolType // ""
};


/*******************************************************************************/
class WSLexer
{
	public:////////////////////////////////////////////////////////////////////

	struct Token
	{
		wsTokenTypes		mType				= wsDefaultType; // Type of the Token

		__int64				mLen				= 0; // Length in chars of this token
		__int64				mPosition			= 0; // position of token (in chars) from the beginning of string.

		int					mLine				= 1; // Number of current line

		const char*			ps					= nullptr; // start
		const char*			pe					= nullptr; // after the end char

							Token()
							{
								Clear();

								mLine = 1;
							}

							~Token()
							{
							}

		void				Clear()
							{
								mType			= wsDefaultType;

								mLen			= 0;
								mPosition		= 0;

								ps				= nullptr;
								pe				= nullptr;
							}
	};

	public:////////////////////////////////////////////////////////////////////

							WSLexer();
							WSLexer(const WSLexer& inOther) = delete;
							~WSLexer();

							// This method resets Lexer to a new string
		void				Put_HttpRequest(const char* inHttpStr, const char* inHttpStrEnd);

							// This method GET next token
							// For GET first token you must use this method
							// This function can throw exceptions
		bool				GetNextToken( Token* outToken, bool WithoutSpace = false );

	protected://///////////////////////////////////////////////////////////////

		const char*			mpHttpStr			= nullptr;
		const char*			mpHttpStrEnd		= nullptr;

		const char*			mpCurrChar			= nullptr;

		int					mLine 				= 1; 		// the current line in command.

		// Flags
		bool				flagBracketsOpen	= false;
		bool				flagQuotesOpen		= false;
};

#endif // _WSLexer_H