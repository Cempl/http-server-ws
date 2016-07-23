#include "MyCover.h"


const uint16_t gLengthMessage = 1024;


/*******************************************************************************/
int Cover::my_send(SOCKET& client_socket, string& message)
{
	int result = 0;

	result = send(client_socket, message.c_str(), static_cast<int>(message.size()), 0);

	return result;
}


/*******************************************************************************/
int Cover::my_recv(SOCKET& client_socket, string& message)
{
	char buffer[gLengthMessage] = {};

	int result = recv(client_socket, buffer, gLengthMessage, 0);
	message = buffer;

	return result;
}