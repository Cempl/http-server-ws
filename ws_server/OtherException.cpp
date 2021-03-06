/*******************************************************************************/
#include "OtherExceptions.h"


void get_ssl_error(SSL* inSSL, int ret_code)
{
	int ret = SSL_get_error(inSSL, ret_code);

	switch (ret)
	{
		case 0:
		{
			throw exception("The TLS/SSL I/O operation completed.");
		}

		case 1:
		{
			throw exception("A failure in the SSL library occurred, usually a protocol error.");
		}

		case 2:
		case 3:
		{
			throw exception("The operation did not complete; the same TLS/SSL I/O function should be called again later.");
		}

		case 4:
		{
			throw exception("The operation did not complete because an application callback set by SSL_CTX_set_client_cert_cb() has asked to be called again. The TLS/SSL I/O function should be called again later. Details depend on the application.");
		}

		case 5:
		{
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
			throw exception("The operation did not complete; the same TLS/SSL I/O function should be called again later. The SSL function should be called again when the connection is established.");		
		}

		// unknown error
		default:
		{
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