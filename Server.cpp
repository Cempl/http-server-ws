/*******************************************************************************/
#include "Server.h"
#include "LogFile.h"


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

	try
	{
		method = TLSv1_2_server_method();

		ctx = SSL_CTX_new(method);
		if (!ctx)
		{
			throw exception("Unable to create SSL context");
		}
	}
	catch(exception& e)
	{
		LogFile log;
		log.write(e.what());
		exit(EXIT_FAILURE);
	}

	return ctx;
}


/*******************************************************************************/
void Server::configure_openssl_context(SSL_CTX *inCtx)
{
	try
	{
		SSL_CTX_set_ecdh_auto(inCtx, 1);

		/* Set the key and cert */
		if (SSL_CTX_use_certificate_file(inCtx, PUBL_CERT, SSL_FILETYPE_PEM) < 0)
		{
			throw exception("Error load certificate");
		}

		if (SSL_CTX_use_PrivateKey_file(inCtx, PRIV_KEY, SSL_FILETYPE_PEM) < 0)
		{
			throw exception("Error load private key");
		}
	}
	catch (exception& e)
	{
		LogFile log;
		log.write(e.what());
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
	storage_addresses.sin_port = htons(PORT_CONNECTION);

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