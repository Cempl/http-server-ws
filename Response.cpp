/*******************************************************************************/
#include "ResponseRequest.h"


/*******************************************************************************/
string ResponseRequest::Path_folder()
{
	TCHAR szPath[MAX_PATH];
	size_t sz = strlen(szPath);

	GetModuleFileNameA(NULL, szPath, MAX_PATH);

	for (size_t i = sz - 1;; i--)
	{
		if (szPath[i] == '\\')
		{
			szPath[i] = '\0';
			break;
		}
	}

	return szPath;
}


/*******************************************************************************/
void ResponseRequest::Send_response(SOCKET client_socket, string &tmp_res, int &t_result)
{
	t_result = send(client_socket, tmp_res.c_str(), static_cast<int>(tmp_res.size()), 0);

	if (t_result == SOCKET_ERROR)
	{
		cout << "Error in send(): " << WSAGetLastError() << endl;

		system("pause");

		exit(15);
	}
}


/*******************************************************************************/
void ResponseRequest::Response_js(SOCKET client_socket, string &file_name)
{
	int result = int();
	string response = string();
	string path_to_file = string();
	string folder = "\\js\\";

	path_to_file = Path_folder() + folder + file_name;

	ifstream fin(path_to_file);

	if (!fin.is_open())
	{
		cout << "Error open file" << endl;
	}
	else
	{
		response += "HTTP/1.1 200 OK\n";
		response += "Server: VaV/V2\n";
		response += "Content-Type: application/javascript;\r\n\r\n";

		// Retrieves the character elements from the input stream buffer, to which it accesses
		string response_body((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());

		response += response_body;
	}

	fin.close();

	Send_response(client_socket, response, result);
}


/*******************************************************************************/
void ResponseRequest::Response_image(SOCKET client_socket, string &file_name)
{
	int result = int();
	string response = string();
	string path_to_file = string();
	string folder = "\\image\\";

	path_to_file = Path_folder() + folder + file_name;

	ifstream fin(path_to_file, ios::binary);

	if (!fin.is_open())
	{
		cout << "Error open file" << endl;
	}
	else
	{
		response += "HTTP/1.1 200 OK\n";
		response += "Server: VaV/V2\n";
		response += "Content-Type: image/png;\r\n\r\n";

		// Retrieves the character elements from the input stream buffer, to which it accesses
		string response_body((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());

		response += response_body;
	}

	fin.close();

	Send_response(client_socket, response, result);
}


/*******************************************************************************/
void ResponseRequest::Response_css(SOCKET client_socket, string &file_name)
{
	int result = int();
	string response = string();
	string path_to_file = string();
	string folder = "\\css\\";

	path_to_file = Path_folder() + folder + file_name;

	ifstream fin(path_to_file);

	if (!fin.is_open())
	{
		cout << "Error open file" << endl;
	}
	else
	{
		response += "HTTP/1.1 200 OK\n";
		response += "Server: VaV/V2\n";
		response += "Content-Type: text/css;\r\n\r\n";

		// Retrieves the character elements from the input stream buffer, to which it accesses
		string response_body((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());

		response += response_body;
	}

	fin.close();

	Send_response(client_socket, response, result);
}


/*******************************************************************************/
void ResponseRequest::Response_default_html(SOCKET client_socket)
{
	int result = int();
	string response = string();
	string path_to_file = string();
	string folder = "\\html\\index.html";

	path_to_file = Path_folder() + folder;

	ifstream fin(path_to_file);

		if (!fin.is_open())
		{
			cout << "Error open file" << endl;
		}
		else
		{
			response += "HTTP/1.1 202 OK\n";
			response += "Server: VaV/V2\n";
			response += "Content-Type: text/html;\n";
			response += "Connection: keep-alive\n";
			response += "X-Powered-By: c++\r\n\r\n";

			// Retrieves the character elements from the input stream buffer, to which it accesses
			string response_body((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());

			response += response_body;
		}

	fin.close();

	Send_response(client_socket, response, result);
}


/*******************************************************************************/
void ResponseRequest::Response_html(SOCKET client_socket, string &file_name)
{
	int result = int();
	string response = string();
	string path_to_file = string();
	string folder = "\\html\\";

	path_to_file = Path_folder() + folder + file_name;

	ifstream fin(path_to_file);

	if (!fin.is_open())
	{
		cout << "Error open file" << endl;
	}
	else
	{
		response += "HTTP/1.1 202 OK\n";
		response += "Server: VaV/V2\n";
		response += "Content-Type: text/html;\n";
		response += "Connection: keep-alive\n";
		response += "X-Powered-By: c++\r\n\r\n";

		// Retrieves the character elements from the input stream buffer, to which it accesses
		string response_body((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>()); 

		response += response_body;
	}

	fin.close();

	Send_response(client_socket, response, result);
}