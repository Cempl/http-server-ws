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

		void		my_send(SSL* inSSL, string& message);
		void		my_recv(SSL* inSSL, string& message);
};