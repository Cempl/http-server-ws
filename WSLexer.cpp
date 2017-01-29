/*******************************************************************************/
#include "WSLexer.h"


/*******************************************************************************/
WSLexer::WSLexer()
{
}


/*******************************************************************************/
WSLexer::~WSLexer()
{
}


/*******************************************************************************/
void WSLexer::Put_HttpRequest(const char* inHttpStr, const char* inHttpStrEnd)
{
	mpHttpStr		= inHttpStr;
	mpHttpStrEnd	= inHttpStrEnd  ? inHttpStrEnd 
									: inHttpStr + strlen(inHttpStr);
	mpCurrChar		= mpHttpStr;

	mLine			= 1;
}


/*******************************************************************************/
bool WSLexer::GetNextToken(Token* outToken)
{
	outToken->Clear();

	if( mpCurrChar == mpHttpStrEnd )
	{
		if(flagBracketsOpen || flagQuotesOpen)
		{
			throw exception("Error WSLexer: Don't closed brackets or quotes");
		}
		return false; // end of string
	}

	bool flagLongWord = false;

	outToken->ps = mpCurrChar;

	do
	{
		switch(*mpCurrChar)
		{
			case 32: // " "
			{
				if( !flagLongWord)
				{
					mpCurrChar = mpCurrChar + 1; // I only work with UTF-8 char, it has a length of 1.

					outToken->mType = wsSpaceType;
					outToken->pe = mpCurrChar;
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

					outToken->mType = wsSymbolType;
					outToken->pe = mpCurrChar;
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

					outToken->mType = wsQuotesSymbolType;
					outToken->pe = mpCurrChar;

					flagQuotesOpen = flagQuotesOpen ? false : true;
				}
				else
				{
					flagLongWord = false;
				}
			}break;

			case 28: // "("
			case 29: // ")"
			{
				if( !flagLongWord )
				{
					mpCurrChar = mpCurrChar + 1;

					outToken->mType = wsBracketsSymbolType;
					outToken->pe = mpCurrChar;

					flagBracketsOpen = flagBracketsOpen ? false : true;
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

					outToken->mType = wsNewLineSymbolType;
					outToken->pe = mpCurrChar;
					
					// if the next symbol is not \n or \r, increase Token::mLen by 1
					if( !(*(mpCurrChar) == 10 || *(mpCurrChar) == 13) )
					{
						outToken->mLine += 1;
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

				outToken->mType = wsDefaultType;
				outToken->pe = mpCurrChar;

				flagLongWord = true;
			}break;
		}
	}
	while(flagLongWord);

	outToken->mLen = (outToken->pe - outToken->ps) + 1;
	outToken->mPosition = (outToken->pe - mpHttpStr) + 1;

	return true;
}