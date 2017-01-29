/*******************************************************************************/
#include "MyCover.h"


/*******************************************************************************/
const uint16_t gLengthMessage = 12288;


/*******************************************************************************/
void Cover::my_send(SOCKET& client_socket, string& message)
{
	int result = send(client_socket, message.c_str(), static_cast<int>(message.size()), 0);

	if(result == SOCKET_ERROR)
	{
		throw exception("Error in send(): " + WSAGetLastError());
	}
}


/*******************************************************************************/
void Cover::my_recv(SOCKET& client_socket, string& message)
{
	char buffer[gLengthMessage] = {};
	int result = recv(client_socket, buffer, gLengthMessage, 0);

	if(result == SOCKET_ERROR)
	{
		throw exception("Error in recv(): " + WSAGetLastError());
	}
	message = string(buffer);
}