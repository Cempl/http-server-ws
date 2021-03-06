#ifndef _MyCover_H
	#define _MyCover_H
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

		void		my_Cleanup_OpenSSL(SSL* inSSL);
		void		my_send(SSL* inSSL, string& message,  bool CleanUp);
		void		my_recv(SSL* inSSL, string& message);
};

#endif // _MyCover_H