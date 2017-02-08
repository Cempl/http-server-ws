#pragma once


/*******************************************************************************/
#include "MyCover.h"
#include "SendRecv.h"
#include "Server.h"


/*******************************************************************************/
class ResponseRequest : protected MyCover
{
	public://////////////////////////////////////////////////////////////////////

				ResponseRequest() {};
				ResponseRequest(const ResponseRequest& InOther) = delete;
				~ResponseRequest() {};

		int		Request(SSL* inSSL);


	public:///////////////////////////////////////////////////////////////////

		void	Parse_name_page(string& inNamePage, string& outNamePage, string& outExtensionFile, bool& flag);
		void	Response_js(SSL* inSSL, string &file_name);
		void	Response_image(SSL* inSSL, string &file_name);
		void	Response_css(SSL* inSSL, string &file_name);
		void	Response_default_html(SSL* inSSL);
		void	Response_html(SSL* inSSL, string &file_name);
		void	Send_response(SSL* inSSL, string &data);
		void	Cleaning_refuse_in_buffer(const string& InBuf, vector<string>& OutEntire_query);
		void	Search_auth_data(vector<string>& entire_query, string& inLogin, string& inPass, int& secret_key, string& file_name,SSL* inSSL);
		void	Search_reg_data(vector<string>& entire_query, string& inName, string& inLogin, string& inPass, int& secret_key, string& file_name, SSL* inSSL);
		void	Search_DH_gen_and_mod(vector<string>& entire_query, int& secret_key, SSL* inSSL);
		void	Send_DH_res_mod(SSL* inSSL, int& out_mod_res);
		void	Cleanup_OpenSSL(SSL* inSSL);


	public://////////////////////////////////////////////////////////////////////

		string	Path_folder();
		string	Read_symbolic_content(ifstream& inFin);


	protected:///////////////////////////////////////////////////////////////////

		SendRecv SR;
};