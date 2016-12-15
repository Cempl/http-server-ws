/*******************************************************************************/
#include "Server.h"


/*******************************************************************************/
void Server::start_server()
{
	initialization_wsa();
	create_socket();
	bundle_socket_adresse();
	listening_connection();
	Processing_a_connection_request();
}


/*******************************************************************************/
void Server::initialization_wsa()
{
	int result;

	result = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (result != 0)
	{
		throw OtherExceptions("Error WSAStartup: " + result);
	}
}


/*******************************************************************************/
void Server::create_socket()
{
	server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // открываем серверный сокет

		if (server_socket == INVALID_SOCKET)
		{
			WSACleanup();

			throw OtherExceptions("Error at socket(): " + WSAGetLastError());
		}
}


/*******************************************************************************/
void Server::bundle_socket_adresse()
{
	memset(&storage_addresses, 0, sizeof(storage_addresses));

	storage_addresses.sin_family = AF_INET;
	storage_addresses.sin_addr.S_un.S_addr = INADDR_ANY;
	storage_addresses.sin_port = htons(port_connection);

	if (bind(server_socket, (LPSOCKADDR)&storage_addresses, sizeof(storage_addresses)) != 0)
	{
		closesocket(server_socket);
		WSACleanup();
		throw OtherExceptions("Bind failed with error: " + WSAGetLastError());
	}
}


/*******************************************************************************/
void Server::listening_connection()
{
	if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR)
	{
		closesocket(server_socket);
		WSACleanup();
		throw OtherExceptions("Bind failed with error: " + WSAGetLastError());
	}
}