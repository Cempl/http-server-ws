#ifndef _WSLexer_H
	#define _WSLexer_H
#pragma once


/*******************************************************************************/
#include <QString>
#include <memory>
#include <list>

using namespace std;


/*******************************************************************************/
enum wsTokenTypes
{
	wsDefaultType = 0,

	wsSpaceType,
    wsSymbolType, // : ; . , / '\'
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


		// Setter
        void                setType(const wsTokenTypes& inType) { mType = inType; };
        void                setLen(const uint32_t inLen) { mLen = inLen; };
        void                setPosition(const uint32_t inPosition) { mPosition = inPosition; };
        void                setLine(const uint32_t inLine) { mLine = inLine; };
        void                setStart(const char* inStart) { ps = inStart; };
        void                setEnd(const char* inEnd) { pe = inEnd; };

        // Getter
        wsTokenTypes        getType() const { return mType; };
        int64_t             getLen() const { return mLen; };
        int64_t             getPosition() const { return mPosition; };
        int                 getLine() const { return mLine; };
        const char*         getStart() const { return ps; };
        const char*         getEnd() const { return pe; };

        private:////////////////////////////////////////////////////////////////////

		wsTokenTypes		mType				= wsDefaultType; // Type of the Token

        uint32_t			mLen				= 0; // Length in chars of this token
        uint32_t			mPosition			= 0; // position of token (in chars) from the beginning of string.

        uint32_t			mLine				= 1; // Number of current line

        const char*			ps					= nullptr; // start
        const char*			pe					= nullptr; // after the end char
	};

	public:////////////////////////////////////////////////////////////////////

							WSLexer();
                            WSLexer(const QString& inString);
							WSLexer(const WSLexer& inOther) = delete;
                            WSLexer(WSLexer&& inOther) = delete;
							~WSLexer();

							// This method resets Lexer to a new string
        void				Put_HttpRequest(const QString& inString);

							// This method GET next token
							// For GET first token you must use this method
							// This function can throw exceptions
		bool				NextToken( bool WithoutSpace = false );

        const shared_ptr<Token> getCurrToken() const;

        // inIndex == 0 - mean this function return current token
        // inIndex > 0 - mean this function return previous token whose index on "inIndex" less than current
        const shared_ptr<Token> getPrevToken(const uint32_t inIndex) const;

	protected://///////////////////////////////////////////////////////////////

        const char*			mpHttpStr;
        const char*			mpHttpStrEnd;

        const char*			mpCurrChar;

        list<shared_ptr<Token>>  listOfTokens;
};

#endif // _WSLexer_H
