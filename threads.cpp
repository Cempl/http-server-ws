/*******************************************************************************/
#include "Interrupt.h"
#include "OtherExceptions.h"
#include "ResponseRequest.h"
#include "Server.h"


/*******************************************************************************/
void Server::Processing_a_connection_request()
{
	ResponseRequest RR;

	while (client_socket = accept(server_socket, (struct sockaddr*)&addr, &addrLen))
	{
		if (client_socket == INVALID_SOCKET)
		{
			if ((closesocket(client_socket)) != 0)
			{
				throw exception("Server::Processing_a_connection_request::closesocket(1): " + WSAGetLastError());
			}

			WSACleanup();

			throw exception("Accept failed with error: " + WSAGetLastError());
		}
		
		if ((mySSL = SSL_new(myContextOpenSSL)) == NULL)
		{
			SSL_free(mySSL);

			throw exception("The creation of a new SSL structure failed. Check the error stack to find out the reason.");
		}

		if ( (SSL_set_fd(mySSL, (int)client_socket)) != 1 )
		{
			SSL_free(mySSL);
			
			if ((closesocket(client_socket)) != 0)
			{
				throw exception("Server::Processing_a_connection_request::closesocket(2): " + WSAGetLastError());
			}

			throw exception("The operation failed. Check the error stack to find out why.");
		}

		int ret_code = SSL_accept(mySSL);

		if ( ret_code <= 0 )
		{
			get_ssl_error(mySSL, ret_code);
		}
		
		//unlock thread thr(ParseHttpHEAD, mySSL); this code and lock thread thr(&ResponseRequest::Request, &RR, mySSL); for using new parser
		thread thr(&ResponseRequest::Request, &RR, mySSL);
		//thread thr(ParseHttpHEAD, mySSL);
		thr.detach();
	}
}