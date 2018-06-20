/*******************************************************************************/
#include "WSLexer.h"

#include <QTextCodec>


/*******************************************************************************/
WSLexer::WSLexer() :
    mpHttpStr(nullptr),
    mpHttpStrEnd(nullptr),
    mpCurrChar(nullptr)
{
}


/*******************************************************************************/
WSLexer::WSLexer(const QString& inString) :
    mpHttpStr(nullptr),
    mpHttpStrEnd(nullptr),
    mpCurrChar(nullptr)
{
    Put_HttpRequest(inString);
}


/*******************************************************************************/
WSLexer::~WSLexer()
{
}


/*******************************************************************************/
void WSLexer::Put_HttpRequest(const QString& inString)
{
    mpHttpStr		= inString.data(); //inString.toUtf8().data();
	mpHttpStrEnd	= mpHttpStr + inString.size();
	mpCurrChar		= mpHttpStr;
}


/*******************************************************************************/
const shared_ptr<WSLexer::Token> WSLexer::getCurrToken() const
{
    return listOfTokens.front();
}


/*******************************************************************************/
const shared_ptr<WSLexer::Token> WSLexer::getPrevToken(const uint32_t inIndex) const
{
    shared_ptr<Token> res(nullptr);

    if(inIndex < listOfTokens.size())
    {
        list<shared_ptr<Token>>::const_iterator it = listOfTokens.begin();
        advance(it, inIndex);

        res = *(it);
    }

    return res;
}


/*******************************************************************************/
bool WSLexer::NextToken(bool WithoutSpace)
{
    listOfTokens.push_front(make_shared<Token>());

    bool res = false;
    if( mpCurrChar == mpHttpStrEnd )
        return res; // end of string

    shared_ptr<Token> spToken = getCurrToken();

    spToken->setStart(mpCurrChar);

    bool flagLongWord = false;
    bool flagThisTokenIsSpace = false;

	do
	{
		flagThisTokenIsSpace = false;

        switch((*mpCurrChar).unicode())
		{
			case 32: // " "
			{
				if( !flagLongWord )
				{
					if(WithoutSpace)
					{
						flagThisTokenIsSpace = true;
						mpCurrChar = mpCurrChar + 1;
						spToken->setStart(mpCurrChar);
					}
					else
					{
						mpCurrChar = mpCurrChar + 1; // I only work with ASCII char, it has a length of 1.
						spToken->setType(wsSpaceType);
						spToken->setEnd(mpCurrChar);
					}
				}
				else
				{
					flagLongWord = false;
				}
			}break;

			case 42: // "*"
			case 43: // "+"
			case 46: // "."
			case 47: // "/"
			case 58: // ":"
			case 59: // ";"
			case 61: // "="
			case 92: // "\"
			{
                if( !flagLongWord )
				{
					mpCurrChar = mpCurrChar + 1;

					spToken->setType(wsSymbolType);
					spToken->setEnd(mpCurrChar);
				}
				else
				{
					flagLongWord = false;
				}
			}break;

			case 34: // ""
			{
				if( !flagLongWord )
				{
					mpCurrChar = mpCurrChar + 1;

					spToken->setType(wsQuotesSymbolType);
					spToken->setEnd(mpCurrChar);
				}
				else
				{
					flagLongWord = false;
				}
			}break;

			case 91: // "["
			case 93: // "]"
			case 28: // "("
			case 29: // ")"
			{
				if( !flagLongWord )
				{
					mpCurrChar = mpCurrChar + 1;

					spToken->setType(wsBracketsSymbolType);
					spToken->setEnd(mpCurrChar);
				}
				else
				{
					flagLongWord = false;
				}
			}break;

			case 10: // "\n"
			case 13: // "\r"
			{
				if( !flagLongWord )
				{
					mpCurrChar = mpCurrChar + 1;

					spToken->setType(wsNewLineSymbolType);
					spToken->setEnd(mpCurrChar);
					
					// if the next symbol is not \n or \r, increase Token::mLen by 1
					if( !(*(mpCurrChar) == 10 || *(mpCurrChar) == 13) )
					{
						spToken->setLine( spToken->getLen() + 1);
					}
				}
				else
				{
					flagLongWord = false;
				}
			}break;

			// All other symbol
			default:
			{
				mpCurrChar = mpCurrChar + 1;

				spToken->setType(wsDefaultType);
				spToken->setEnd(mpCurrChar);

				flagLongWord = true;

			}break;
		}

		if (mpCurrChar == mpHttpStrEnd)
		{
			flagLongWord = false;
			res = false; // end of string
		}
		else
		{
			res = true;
		}
	}
	while(flagLongWord || flagThisTokenIsSpace);

	spToken->setLen( spToken->getEnd() - spToken->getStart() );
	spToken->setPosition(spToken->getStart() - mpHttpStr);

	return res;
}
