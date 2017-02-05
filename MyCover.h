#ifndef _MyCover_H
	#define _MyCover_H
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

#endif // _LogFile_H