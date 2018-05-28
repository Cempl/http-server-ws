#ifndef _OtherException_H
	#define _OtherException_H
#pragma once


/*******************************************************************************/
#include "Server.h"


/*******************************************************************************/
using namespace std;


/*******************************************************************************/
void		get_ssl_error(SSL* inSSL, int ret_code);


/*******************************************************************************/
class interruptException
{
	public://////////////////////////////////////////////////////////////////////

		interruptException()
		{
		}

		~interruptException()
		{
		}
};

#endif // _OtherException_H