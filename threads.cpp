/*******************************************************************************/
#include "Server.h"
#include "Interrupt.h"
#include "ResponseRequest.h"


/*******************************************************************************/
void Server::Processing_a_connection_request()
{
	ResponseRequest RR;

	while (client_socket = accept(server_socket, NULL, NULL))
	{
		if (client_socket == INVALID_SOCKET)
		{
			closesocket(server_socket);
			WSACleanup();

			throw OtherExceptions("Accept failed with error: " + WSAGetLastError());
		}

		thread thr(&ResponseRequest::Request, &RR, client_socket);
		thr.detach();
	}
}