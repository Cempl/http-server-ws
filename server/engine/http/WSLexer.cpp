/*******************************************************************************/
#include "WSLexer.h"


/*******************************************************************************/
WSLexer::WSLexer() :
    mpHttpStr(nullptr),
    mpHttpStrEnd(nullptr),
    mpCurrChar(nullptr)
{
}


/*******************************************************************************/
WSLexer::WSLexer(const string& inString) :
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
void WSLexer::Put_HttpRequest(const string& inString)
{

	mpHttpStr		= inString.c_str();
	mpHttpStrEnd	= mpHttpStr + inString.size();
	mpCurrChar		= mpHttpStr;
}


/*******************************************************************************/
const shared_ptr<WSLexer::Token>& WSLexer::getToken() const
{
    return spToken;
}


/*******************************************************************************/
bool WSLexer::NextToken(bool WithoutSpace)
{
	//spToken->Clear();

    if( spToken )
    {
        spToken = make_shared<Token>(spToken);
    }
    else
    {
        spToken = make_shared<Token>();
    }

	bool res = false;
	bool flagLongWord = false;
	bool flagThisTokenIsSpace = false;

	if( mpCurrChar == mpHttpStrEnd )	
		return res; // end of string

	spToken->setStart(mpCurrChar);

	do
	{
		flagThisTokenIsSpace = false;

		switch(*mpCurrChar)
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
				if( !flagLongWord)
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