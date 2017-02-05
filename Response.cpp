/*******************************************************************************/
#include "ResponseRequest.h"
#include "LogFile.h"
#include "ControlsDatabase.h"


/*******************************************************************************/
string ResponseRequest::Path_folder()
{
	char szPath[MAX_PATH] = {};

	GetModuleFileNameA(NULL, szPath, MAX_PATH);

	// Pointer to the last occurrence of "\"
	char *lstChr = strrchr(szPath, '\\');

	// replaced by zero (truncate)
	*lstChr = '\0';

	return szPath;
}


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
string ResponseRequest::Read_symbolic_content(ifstream& inFin)
{
	string response_body((istreambuf_iterator<char>(inFin)), istreambuf_iterator<char>());
	
	return response_body;
}


/*******************************************************************************/
void ResponseRequest::Send_response(SSL* inSSL, string &data)
{
	int ret_code = SSL_write(inSSL, data.c_str(), static_cast<int>(data.size()));

	if (ret_code <= 0)
	{
		Cr.get_ssl_error(inSSL, ret_code);
	}

	Cleanup_OpenSSL(inSSL);
}


/*******************************************************************************/
void ResponseRequest::Response_js(SSL* inSSL, string &file_name)
{
	string response = string();
	string path_to_file = string();
	string folder = "\\js\\";

	path_to_file = Path_folder() + folder + file_name;

	ifstream fin(path_to_file, ios::binary);

	if (!fin.is_open())
	{
		throw exception( ("Missing " + path_to_file).c_str() );
	}
	else
	{
		response += "HTTP/1.1 200 OK\n";
		response += "Access-Control-Allow-Origin: *\n";
		response += "Server: VaV/V2\n";
		response += "Content-Type: application/javascript;\r\n\r\n";

		response += Read_symbolic_content(fin);
	}

	fin.close();

	Send_response(inSSL, response);
}


/*******************************************************************************/
void ResponseRequest::Response_image(SSL* inSSL, string &file_name)
{
	string response = string();
	string path_to_file = string();
	string folder = "\\image\\";

	path_to_file = Path_folder() + folder + file_name;

	ifstream fin(path_to_file, ios::binary);

	if (!fin.is_open())
	{
		throw exception( ("Missing " + path_to_file).c_str() );
	}
	else
	{
		response += "HTTP/1.1 200 OK\n";
		response += "Server: VaV/V2\n";
		response += "Content-Type: image/png;\r\n\r\n";

		response += Read_symbolic_content(fin);
	}

	fin.close();

	Send_response(inSSL, response);
}


/*******************************************************************************/
void ResponseRequest::Response_css(SSL* inSSL, string &file_name)
{
	string response = string();
	string path_to_file = string();
	string folder = "\\css\\";

	path_to_file = Path_folder() + folder + file_name;

	ifstream fin(path_to_file, ios::binary);

	if (!fin.is_open())
	{
		throw exception( ("Missing " + path_to_file).c_str() );
	}
	else
	{
		response += "HTTP/1.1 200 OK\n";
		response += "Server: VaV/V2\n";
		response += "Content-Type: text/css;\r\n\r\n";

		response += Read_symbolic_content(fin);
	}

	fin.close();

	Send_response(inSSL, response);
}


/*******************************************************************************/
void ResponseRequest::Response_default_html(SSL* inSSL)
{
	//string response = CD.get_file_from_db("index.html");
	
	string response = string();
	string path_to_file = string();
	string folder = "\\html\\index.html";

	path_to_file = Path_folder() + folder;

	ifstream fin(path_to_file, ios::binary);

		if (!fin.is_open())
		{
			throw exception( ("Missing " + path_to_file).c_str() );
		}
		else
		{
			response += "HTTP/1.1 200 OK\n";
			response += "Server: VaV/V2\n";
			response += "Content-Type: text/html;\n";
			response += "Connection: keep-alive\n";
			response += "X-Powered-By: c++\r\n\r\n";

			response += Read_symbolic_content(fin);
		}

	fin.close();

	Send_response(inSSL, response);
}


/*******************************************************************************/
void ResponseRequest::Response_html(SSL* inSSL, string &file_name)
{
	string response = string();
	string path_to_file = string();
	string folder = "\\html\\";

	path_to_file = Path_folder() + folder + file_name;

	ifstream fin(path_to_file, ios::binary);

	if (!fin.is_open())
	{
		throw exception( ("Missing " + path_to_file).c_str() );
	}
	else
	{
		response += "HTTP/1.1 200 OK\n";
		response += "Server: VaV/V2\n";
		response += "Content-Type: text/html;\n";
		response += "Connection: keep-alive\n";
		response += "X-Powered-By: c++\r\n\r\n";

		response += Read_symbolic_content(fin);
	}

	fin.close();

	Send_response(inSSL, response);
}