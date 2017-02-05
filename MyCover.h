#pragma once


/*******************************************************************************/
#include "Server.h"


/*******************************************************************************/
class Cover 
{
	public: ////////////////////////////////////////////////////////////////////

				Cover() {};
				Cover(const Cover& InOther) = delete;
				~Cover() {};


	public: /////////////////////////////////////////////////////////////////////

		void		get_ssl_error(SSL* inSSL, int ret_code);


	protected:///////////////////////////////////////////////////////////////////

		void		my_send(SSL* inSSL, string& message);
		void		my_recv(SSL* inSSL, string& message);
};

extern Cover Cr;