#pragma once


/*******************************************************************************/
#include "Server.h"
#include "SendRecv.h"


/*******************************************************************************/
class ResponseRequest
{
	public://////////////////////////////////////////////////////////////////////

		ResponseRequest() {};
		ResponseRequest(const ResponseRequest& InOther) = delete;
		~ResponseRequest() {};

		int Request(SOCKET client_socket);


	protected:///////////////////////////////////////////////////////////////////

		void Response_js(SOCKET client_socket, string &file_name);
		void Response_image(SOCKET client_socket, string &file_name);
		void Response_css(SOCKET client_socket, string &file_name);
		void Response_default_html(SOCKET client_socket);
		void Response_html(SOCKET client_socket, string &file_name);


	protected:///////////////////////////////////////////////////////////////////

		SendRecv SR;
};