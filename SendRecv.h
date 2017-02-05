#ifndef _SendRecv_H
	#define _SendRecv_H
#pragma once


/*******************************************************************************/
#include "Generate_key.h"
#include "Interrupt.h"
#include "MyCover.h"
#include "Server.h"
#include "Var_for_threads.h"


/*******************************************************************************/
class SendRecv : protected Cover
{
	public://////////////////////////////////////////////////////////////////////

				SendRecv() {};
				SendRecv(const SendRecv& InOther) = delete;
				~SendRecv() {};

		void	websocket_handshake(SOCKET client_socket, string key);
		void	send_data(SOCKET& client_socket);


	protected:///////////////////////////////////////////////////////////////////

		void	generate(string& key);
		int		websocket_get_content(string& data, int data_length);
		void	websocket_set_content(string& data, int64_t data_length, int data_type);
		int		recv_data(string& data);
		int		Thread_recv(SOCKET client_socket);
};

#endif // _SendRecv_H