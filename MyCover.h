#pragma once


/*******************************************************************************/
#include "Server.h"


/*******************************************************************************/
class MyCover 
{
	public: ////////////////////////////////////////////////////////////////////

					MyCover() {};
					MyCover(const MyCover& InOther) = delete;
					~MyCover() {};

		void		get_ssl_error(SSL* inSSL, int ret_code);

		void		my_send(SSL* inSSL, string& message);
		void		my_recv(SSL* inSSL, string& message);
};

// This is bullshit
extern MyCover Cr;