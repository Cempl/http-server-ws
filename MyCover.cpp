/*******************************************************************************/
#include "MyCover.h"


/*******************************************************************************/
Cover Cr;
const uint16_t gLengthMessage = 12288;


/*******************************************************************************/
void Cover::my_send(SSL* inSSL, string& message)
{
	int ret_code = SSL_write(inSSL, message.c_str(), static_cast<int>(message.size()));

	if (ret_code <= 0)
	{
		get_ssl_error(inSSL, ret_code);
	}
}


/*******************************************************************************/
void Cover::my_recv(SSL* inSSL, string& message)
{
	char buffer[gLengthMessage] = {};
	
	int ret_code = SSL_read(inSSL, buffer, gLengthMessage);
	
	if(ret_code < 0)
	{
		get_ssl_error(inSSL, ret_code);
	}

	if (ret_code == 0)
	{
		//SSL_shutdown(inSSL);
	}

	for (uint16_t i = 0; i < ret_code; i++)
	{
		message.push_back(buffer[i]);
	}
}


void Cover::get_ssl_error(SSL* inSSL, int ret_code)
{
	int ret = SSL_get_error(inSSL, ret_code);

	switch (ret)
	{
		case 0:
		{
			break;
			throw exception("The TLS/SSL I/O operation completed.");
		}

		case 1:
		{
			break;
			throw exception("A failure in the SSL library occurred, usually a protocol error.");
		}

		case 2:
		case 3:
		{
			break;
			throw exception("The operation did not complete; the same TLS/SSL I/O function should be called again later.");
		}

		case 4:
		{
			break;
			throw exception("The operation did not complete because an application callback set by SSL_CTX_set_client_cert_cb() has asked to be called again. The TLS/SSL I/O function should be called again later. Details depend on the application.");
		}

		case 5:
		{
			break;
			throw exception("Some I/O error occurred.");
			
		}

		case 6:
		{
			break;
			throw exception("The TLS/SSL connection has been closed. If the protocol version is SSL 3.0 or TLS 1.0, this result code is returned only if a closure alert has occurred in the protocol, i.e. if the connection has been closed cleanly. ");
		}

		case 7:
		case 8:
		{
			break;
			throw exception("The operation did not complete; the same TLS/SSL I/O function should be called again later. The SSL function should be called again when the connection is established.");
			
		}

		// unknown error
		default:
		{
			break;
			throw exception("unknown error SSL accept: " + ret);
			
		}
	}

	/*SOCKET client_socket = SSL_get_fd(inSSL);

	if (client_socket == -1)
	{
		throw exception("The operation failed, because the underlying BIO is not of the correct type.");
	}

	SSL_free(inSSL);

	if ((closesocket(client_socket)) != 0)
	{
		throw exception("Cover::closesocket: " + WSAGetLastError());
	}*/
}