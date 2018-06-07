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

	class Token
	{
        public:////////////////////////////////////////////////////////////////////

							Token() :
                                mType(wsDefaultType),
                                mLen(0),
                                mPosition(0),
                                mLine(1),
                                ps(nullptr),
                                pe(nullptr)
							{
							}

                            Token(shared_ptr<Token*> inToken) :
                                mType(wsDefaultType),
                                mLen(0),
                                mPosition(0),
                                mLine(1),
                                ps(nullptr),
                                pe(nullptr)
                            {
                            }

							~Token()
							{
							}

		// Setter
        void                setType(const wsTokenTypes& inType) { mType = inType; };
        void                setLen(const __int64& inLen) { mLen = inLen; };
        void                setPosition(const __int64& inPosition) { mPosition = inPosition; };
        void                setLine(const int& inLine) { mLine = inLine; };
        void                setStart(const char* inStart) { ps = inStart; };
        void                setEnd(const char* inEnd) { pe = inEnd; };

        // Getter
        wsTokenTypes        getType() const { return mType; };
        __int64             getLen() const { return mLen; };
        __int64             getPosition() const { return mPosition; };
        int                 getLine() const { return mLine; };
        const char*         getStart() const { return ps; };
        const char*         getEnd() const { return pe; };

        const shared_ptr<Token>& getPrevToken() const { return prevToken; };

        private:////////////////////////////////////////////////////////////////////

		wsTokenTypes		mType				= wsDefaultType; // Type of the Token

		__int64				mLen				= 0; // Length in chars of this token
		__int64				mPosition			= 0; // position of token (in chars) from the beginning of string.

		int					mLine				= 1; // Number of current line

		const char*			ps					= nullptr; // start
		const char*			pe					= nullptr; // after the end char

        shared_ptr<Token>  prevToken;

	};

	public:////////////////////////////////////////////////////////////////////

							WSLexer();
                            WSLexer(const string& inString);
							WSLexer(const WSLexer& inOther) = delete;
                            WSLexer(WSLexer&& inOther) = delete;
							~WSLexer();

							// This method resets Lexer to a new string
		void				Put_HttpRequest(const string& inString);

							// This method GET next token
							// For GET first token you must use this method
							// This function can throw exceptions
		bool				NextToken( bool WithoutSpace = false );

        const shared_ptr<Token>& getToken() const;

	protected://///////////////////////////////////////////////////////////////

		const char*			mpHttpStr;
		const char*			mpHttpStrEnd;

		const char*			mpCurrChar;

        shared_ptr<Token>  spToken;
};

#endif // _WSLexer_H