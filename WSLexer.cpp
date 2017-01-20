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
void WSLexer::GetNextToken(Token* outToken)
{
	outToken->Clear();


}