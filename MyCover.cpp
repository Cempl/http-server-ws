/*******************************************************************************/
#include "MyCover.h"
#include "OtherExceptions.h"


/*******************************************************************************/
const uint16_t gLengthMessage = 12288;


/*******************************************************************************/
void MyCover::my_send(SSL* inSSL, string& message)
{
	int ret_code = SSL_write(inSSL, message.c_str(), static_cast<int>(message.size()));

	if (ret_code <= 0)
	{
		get_ssl_error(inSSL, ret_code);
	}
}


/*******************************************************************************/
void MyCover::my_recv(SSL* inSSL, string& message)
{
	char buffer[gLengthMessage] = {};
	
	int ret_code = SSL_read(inSSL, buffer, gLengthMessage);
	
	if(ret_code <= 0)
	{
		get_ssl_error(inSSL, ret_code);
	}

	for (uint16_t i = 0; i < ret_code; i++)
	{
		message.push_back(buffer[i]);
	}
}