/*******************************************************************************/
#include "ControlsDatabase.h"
#include "LogFile.h"
#include "OtherExceptions.h"
#include "ResponseRequest.h"


/*******************************************************************************/
void ResponseRequest::Cleanup_OpenSSL(SSL* inSSL)
{
	SOCKET client_socket = SSL_get_fd(inSSL);

	if (client_socket == -1)
	{
		throw exception("The operation failed, because the underlying BIO is not of the correct type.");
	}

	SSL_free(inSSL);

	if ((closesocket(client_socket)) != 0)
	{
		throw exception("ResponseRequest::Send_response::closesocket(2): " + WSAGetLastError());
	}
}


/*******************************************************************************/
void ResponseRequest::Send_response(SSL* inSSL, string &data)
{
	int ret_code = SSL_write(inSSL, data.c_str(), static_cast<int>(data.size()));

	if (ret_code <= 0)
	{
		get_ssl_error(inSSL, ret_code);
	}

	Cleanup_OpenSSL(inSSL);
}