#include "Server.h"

/*******************************************************************************/
int my_send(SOCKET& client_socket, string& message)
{
	int result = 0;

	result = send(client_socket, message.c_str(), message.size(), 0);

	return result;
}

/*******************************************************************************/
int my_recv(SOCKET& client_socket, string& message)
{
	int result = 0;
	char buffer[1024] = {0};

	result = recv(client_socket, buffer, 1024, 0);

	message = buffer;
	//delete buffer;

	return result;
}
