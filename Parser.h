#ifndef _Parser_H
	#define _Parser_H
#pragma once


/*******************************************************************************/
#include "Server.h"


/*******************************************************************************/
class Parser
{
	public://////////////////////////////////////////////////////////////////////

		Parser() {};
		Parser(const Parser& inOther) = delete;
		~Parser() {};


	public://////////////////////////////////////////////////////////////////////

		int					ParseHttpHEAD( SSL* inSSL );
		void				ParseDataFromWebSocket(string& data);

};

#endif // _Parser_H