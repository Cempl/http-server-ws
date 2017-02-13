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
class SendRecv : protected MyCover
{
	public://////////////////////////////////////////////////////////////////////

				SendRecv() {};
				SendRecv(const SendRecv& InOther) = delete;
				~SendRecv() {};

		void	websocket_handshake(SSL* inSSL, string key);
		void	send_data(SSL* inSSL);


	protected:///////////////////////////////////////////////////////////////////

		void	generate_key(string& key);
		void	websocket_set_content(string& data, int64_t data_length, int data_type);
		int		websocket_get_content(string& data, int data_length);
		int		recv_data(string& data, SSL* inSSL);
		int		Thread_recv(SSL* inSSL);
};

#endif // _SendRecv_H