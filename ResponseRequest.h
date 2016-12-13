#pragma once


/*******************************************************************************/
#include "MyCover.h"
#include "SendRecv.h"
#include "Server.h"


/*******************************************************************************/
class ResponseRequest : protected Cover
{
	public://////////////////////////////////////////////////////////////////////

				ResponseRequest() {};
				ResponseRequest(const ResponseRequest& InOther) = delete;
				~ResponseRequest() {};

		int		Request(SOCKET client_socket);


	public:///////////////////////////////////////////////////////////////////

		void	Response_js(SOCKET client_socket, string &file_name);
		void	Response_image(SOCKET client_socket, string &file_name);
		void	Response_css(SOCKET client_socket, string &file_name);
		void	Response_default_html(SOCKET client_socket);
		void	Response_html(SOCKET client_socket, string &file_name);
		void	Send_response(SOCKET client_socket, string &tmp_res, int &t_result);
		void	Cleaning_refuse_in_buffer(const string& InBuf, vector<string>& OutEntire_query);
		void	Search_auth_data(vector<string>& entire_query, string& inLogin, string& inPass, int& secret_key);
		void	Search_reg_data(vector<string>& entire_query, string& inName, string& inLogin, string& inPass, int& secret_key);
		void	Search_DH_gen_and_mod(vector<string>& entire_query, int& secret_key, SOCKET& client_socket);
		void	Send_DH_res_mod(SOCKET& client_socket, int& out_mod_res);


	protected:///////////////////////////////////////////////////////////////////

		string	Path_folder();


	protected:///////////////////////////////////////////////////////////////////

		SendRecv SR;
};