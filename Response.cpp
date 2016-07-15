/*******************************************************************************/
#include "ResponseRequest.h"


/*******************************************************************************/
void ResponseRequest::Response_js(SOCKET client_socket, string &file_name)
{
	TCHAR szPath[MAX_PATH];
	int result = 0;
	size_t sz = strlen(szPath);
	string response = { 0 };
	string path_to_file; // Полный путь к файлу
	string folder = "\\js\\"; // Папка с файлами рядом с .exe

	GetModuleFileNameA(NULL, szPath, MAX_PATH);

	for (size_t i = sz - 1;; i--)
	{
		if (szPath[i] == '\\')
		{
			szPath[i] = '\0';
			break;
		}
	}

	path_to_file = szPath + folder + file_name;

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

		//извлекает элементы символов из буфера потока ввода, к которому он получает доступ
		//через хранимый им объект, относящийся к типу указателя
		string response_body((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());

		response += response_body;
	}

	fin.close();

	result = send(client_socket, response.c_str(), static_cast<int>(response.size()), 0);

	if (result == SOCKET_ERROR)
	{
		cout << "Error in send(): " << WSAGetLastError() << endl;
		system("pause");
		exit(15);
	}
}


///*******************************************************************************/
//void Response_video(SOCKET client_socket, string &file_name)
//{
//	TCHAR szPath[MAX_PATH];
//	int result = 0;
//	size_t sz = strlen(szPath);
//	string response = { 0 };
//	string path_to_file; // Полный путь к файлу
//	string folder = "\\video\\"; // Папка с файлами рядом с .exe
//
//	GetModuleFileNameA(NULL, szPath, MAX_PATH);
//
//	for (size_t i = sz - 1;; i--)
//	{
//		if (szPath[i] == '\\')
//		{
//			szPath[i] = '\0';
//			break;
//		}
//	}
//
//	path_to_file = szPath + folder + file_name;
//
//	ifstream fin(path_to_file, ios::binary);
//
//	if (!fin.is_open())
//	{
//		cout << "Error open file" << endl;
//	}
//	else
//	{
//		response += "HTTP/1.1 200 OK\n";
//		response += "Server: VaV/V2\n";
//		response += "Content-Type: video/x-msvideo;\r\n\r\n";
//
//		//извлекает элементы символов из буфера потока ввода, к которому он получает доступ
//		//через хранимый им объект, относящийся к типу указателя
//		string response_body((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
//
//		response += response_body;
//	}
//
//	fin.close();
//
//	result = send(client_socket, response.c_str(), static_cast<int>(response.size()), 0);
//
//	if (result == SOCKET_ERROR)
//	{
//		cout << "Error in send(): " << WSAGetLastError() << endl;
//		system("pause");
//		exit(15);
//	}
//}


/*******************************************************************************/
void ResponseRequest::Response_image(SOCKET client_socket, string &file_name)
{
	TCHAR szPath[MAX_PATH];
	int result = 0;
	size_t sz = strlen(szPath);
	string response = { 0 };
	string path_to_file; // Полный путь к файлу
	string folder = "\\image\\"; // Папка с файлами рядом с .exe

	GetModuleFileNameA(NULL, szPath, MAX_PATH);

	for (size_t i = sz - 1;; i--)
	{
		if (szPath[i] == '\\')
		{
			szPath[i] = '\0';
			break;
		}
	}

	path_to_file = szPath + folder + file_name;

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

		//извлекает элементы символов из буфера потока ввода, к которому он получает доступ
		//через хранимый им объект, относящийся к типу указателя
		string response_body((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());

		response += response_body;
	}

	fin.close();

	result = send(client_socket, response.c_str(), static_cast<int>(response.size()), 0);

	if (result == SOCKET_ERROR)
	{
		cout << "Error in send(): " << WSAGetLastError() << endl;
		system("pause");
		exit(15);
	}
}


/*******************************************************************************/
void ResponseRequest::Response_css(SOCKET client_socket, string &file_name)
{
	TCHAR szPath[MAX_PATH];
	int result = 0;
	size_t sz = strlen(szPath);
	string response = { 0 };
	string path_to_file; // Полный путь к файлу
	string folder = "\\css\\"; // Папка с файлами рядом с .exe

	GetModuleFileNameA(NULL, szPath, MAX_PATH);

	for (size_t i = sz - 1;; i--)
	{
		if (szPath[i] == '\\')
		{
			szPath[i] = '\0';
			break;
		}
	}

	path_to_file = szPath + folder + file_name;

	ifstream fin(path_to_file);

	if (!fin.is_open())
	{
		cout << "Error open file" << endl;
	}
	else
	{
		string temp;

		response += "HTTP/1.1 200 OK\n";
		response += "Server: VaV/V2\n";
		response += "Content-Type: text/css;\r\n\r\n";

		while (getline(fin, temp))
		{
			response += temp;
			response += "\n";

			fin.sync();
		}
	}

	fin.close();

	result = send(client_socket, response.c_str(), static_cast<int>(response.size()), 0);

	if (result == SOCKET_ERROR)
	{
		cout << "Error in send(): " << WSAGetLastError() << endl;
		system("pause");
		exit(16);
	}
}


/*******************************************************************************/
void ResponseRequest::Response_default_html(SOCKET client_socket)
{
	TCHAR szPath[MAX_PATH];
	int result = 0;
	size_t sz = strlen(szPath);
	string response = { 0 };
	string path_to_file; // Полный путь к файлу
	string folder = "\\html\\index.html"; // Папка с файлами рядом с .exe

	GetModuleFileNameA(NULL, szPath, MAX_PATH);

	for (size_t i = sz - 1;; i--)
	{
		if (szPath[i] == '\\')
		{
			szPath[i] = '\0';
			break;
		}
	}

	path_to_file = szPath + folder;

	ifstream fin(path_to_file);

		if (!fin.is_open())
		{
			cout << "Error open file" << endl;
		}
		else
		{
			string temp;

			response += "HTTP/1.1 202 OK\n";
			response += "Server: VaV/V2\n";
			response += "Content-Type: text/html;\n";
			response += "Connection: keep-alive\n";
			response += "X-Powered-By: c++\r\n\r\n";

			while (getline(fin, temp))
			{
				response += temp;
				response += "\n";

				fin.sync();
			}
		}

	fin.close();

	result = send(client_socket, response.c_str(), static_cast<int>(response.size()), 0);

	if (result == SOCKET_ERROR)
	{
		cout << "Error in send(): " << WSAGetLastError() << endl;
		system("pause");
		exit(17);
	}
}


/*******************************************************************************/
void ResponseRequest::Response_html(SOCKET client_socket, string &file_name)
{
	TCHAR szPath[MAX_PATH];
	int result = 0;
	size_t sz = strlen(szPath);
	string response = { 0 };
	string path_to_file; // Полный путь к файлу
	string folder = "\\html\\"; // Папка с файлами рядом с .exe

	GetModuleFileNameA(NULL, szPath, MAX_PATH);

	for (size_t i = sz - 1;; i--)
	{
		if (szPath[i] == '\\')
		{
			szPath[i] = '\0';
			break;
		}
	}

	path_to_file = szPath + folder + file_name;

	ifstream fin(path_to_file);

	if (!fin.is_open())
	{
		cout << "Error open file" << endl;
	}
	else
	{
		string temp;

		response += "HTTP/1.1 202 OK\n";
		response += "Server: VaV/V2\n";
		response += "Content-Type: text/html;\n";
		response += "Connection: keep-alive\n";
		response += "X-Powered-By: c++\r\n\r\n";

		while (getline(fin, temp))
		{
			response += temp;
			response += "\n";

			fin.sync();
		}
	}

	fin.close();

	result = send(client_socket, response.c_str(), static_cast<int>(response.size()), 0);

	if (result == SOCKET_ERROR)
	{
		cout << "Error in send(): " << WSAGetLastError() << endl;
		system("pause");
		exit(17);
	}
}