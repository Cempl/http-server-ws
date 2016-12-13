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


	protected:///////////////////////////////////////////////////////////////////

		void		my_send(SOCKET& client_socket, string& message);
		void		my_recv(SOCKET& client_socket, string& message);
};