#pragma once


#include "Server.h"


class Cover 
{
	public: 

				Cover() {};
				Cover(const Cover& InOther) = delete;
				~Cover() {};


	protected:

		int		my_send(SOCKET& client_socket, string& message);
		int		my_recv(SOCKET& client_socket, string& message);
};