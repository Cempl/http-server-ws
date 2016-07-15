#pragma once


/*******************************************************************************/
#include "Server.h"
#include "Generate_key.h"
#include "Var_for_threads.h"
#include "Interrupt.h"


/*******************************************************************************/
class SendRecv
{
	public://////////////////////////////////////////////////////////////////////

				SendRecv() {};
				SendRecv(const SendRecv& InOther) = delete;
				~SendRecv() {};

		void	websocket_handshake(SOCKET client_socket, string key);
		int		my_send(SOCKET& client_socket, string& message);
		int		my_recv(SOCKET& client_socket, string& message);
		void	send_data(SOCKET& client_socket);


	protected:///////////////////////////////////////////////////////////////////

		void	generate(string& key);
		int		websocket_get_content(string& data, int data_length);
		void	websocket_set_content(string& data, int64_t data_length, int data_type);
		int		recv_data(string& data);
		int		Thread_recv(SOCKET client_socket);
};