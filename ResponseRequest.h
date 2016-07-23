#pragma once


/*******************************************************************************/
#include "MyCover.h"
#include "SendRecv.h"
#include "Server.h"
#include "ControlsDatabase.h"


/*******************************************************************************/
class ResponseRequest : protected Cover
{
	public://////////////////////////////////////////////////////////////////////

				ResponseRequest() {};
				ResponseRequest(const ResponseRequest& InOther) = delete;
				~ResponseRequest() {};

		int		Request(SOCKET client_socket);


	protected:///////////////////////////////////////////////////////////////////

		void	Response_js(SOCKET client_socket, string &file_name);
		void	Response_image(SOCKET client_socket, string &file_name);
		void	Response_css(SOCKET client_socket, string &file_name);
		void	Response_default_html(SOCKET client_socket);
		void	Response_html(SOCKET client_socket, string &file_name);
		void	Send_response(SOCKET client_socket, string &tmp_res, int &t_result);
		void	processing(const string& InBuf, vector<string>& OutEntire_query);
		string	Path_folder();


	protected:///////////////////////////////////////////////////////////////////

		SendRecv SR;
};