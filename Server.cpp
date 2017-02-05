/*******************************************************************************/
#include "Server.h"


/*******************************************************************************/
void Server::start_server()
{
	initialization_wsa();
	init_openssl();
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
		throw exception("Error WSAStartup: " + result);
	}
}


/*******************************************************************************/
void Server::init_openssl()
{
	SSL_load_error_strings();
	OpenSSL_add_ssl_algorithms();

	myContextOpenSSL = create_openssl_context();

	configure_openssl_context(myContextOpenSSL);
}


/*******************************************************************************/
SSL_CTX* Server::create_openssl_context()
{
	const SSL_METHOD *method;
	SSL_CTX *ctx;

	method = TLSv1_2_server_method();

	ctx = SSL_CTX_new(method);
	if (!ctx) 
	{
		perror("Unable to create SSL context");
		ERR_print_errors_fp(stderr);

		exit(EXIT_FAILURE);
	}

	return ctx;
}


/*******************************************************************************/
void Server::configure_openssl_context(SSL_CTX *inCtx)
{
	SSL_CTX_set_ecdh_auto(inCtx, 1);

	/* Set the key and cert */
	if (SSL_CTX_use_certificate_file(inCtx, "127.0.0.1.crt", SSL_FILETYPE_PEM) < 0) 
	{
		ERR_print_errors_fp(stderr);
		exit(EXIT_FAILURE);
	}
	
	if (SSL_CTX_use_PrivateKey_file(inCtx, "127.0.0.1.key", SSL_FILETYPE_PEM) < 0) 
	{
		ERR_print_errors_fp(stderr);

		exit(EXIT_FAILURE);
	}
}


/*******************************************************************************/
void Server::create_socket()
{
	server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (server_socket == INVALID_SOCKET)
		{
			WSACleanup();
			
			throw exception("Error at socket(): " + WSAGetLastError());
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
		throw exception("Bind failed with error: " + WSAGetLastError());
	}
}


/*******************************************************************************/
void Server::listening_connection()
{
	if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR)
	{
		closesocket(server_socket);
		WSACleanup();
		throw exception("Bind failed with error: " + WSAGetLastError());
	}
}