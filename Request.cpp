/*******************************************************************************/
#include "ResponseRequest.h"


/*******************************************************************************/
int ResponseRequest::Request(SOCKET client_socket)
{
	char buf[1024] = { 0 };
	int res = 0;
	string file_name = { 0 }; //Имя файла
	string extension_file = { 0 }; // Расширение файла
	stringstream buffer_request;
	string temp_buffer_request = { 0 };

	vector<string> entire_query; // весь запрос
	const vector<string> additional_type_list = { "css" };
	const vector<string> image_type_list = { "jpg", "tiff", "png", "jpeg", "gif", "ico" };
	const vector<string> web_type_list = { "html" };
	const vector<string> js_type_list = { "js" };

	res = recv(client_socket, buf, 1024, 0);

	if (res == SOCKET_ERROR)
	{
		cout << "Error in recv(): " << WSAGetLastError() << endl;
		system("pause");
	}
	else
	{
		cout << "**************************************************************************";
		cout << "\n\n" << buf << "\n";
		cout << "**************************************************************************" << "\n";
	}

	buffer_request << buf;

	while (buffer_request >> temp_buffer_request)
	{
		entire_query.push_back(temp_buffer_request);
	}

	if (entire_query.size() == 0)
	{
		return 1;
	}

	if (entire_query.at(0) == "GET")
	{
		//Кидать exception
		size_t value_search = entire_query.at(1).find('.'); // Позиция точки в строке

		if (value_search == -1)
		{
			vector<string>::iterator key_find = find(entire_query.begin(), entire_query.end(), "Sec-WebSocket-Key:");

			if ((key_find - entire_query.begin()) != entire_query.size())
			{
				SR.websocket_handshake(client_socket, entire_query.at((key_find - entire_query.begin()) + 1));
				return 3;
			}

			Response_default_html(client_socket);
			closesocket(client_socket);
			return 7;
		}

		file_name.append(entire_query.at(1), 1, value_search); // Запись имени файла с точкой в конце

		file_name.erase(0, 1);

		extension_file.append(entire_query.at(1), value_search + 1, -1);

		extension_file.erase(0, 1);

		file_name += extension_file;

		if (file_name == "chat.html")
		{
			file_name = "authorization.html";
			Response_html(client_socket, file_name);
			closesocket(client_socket);
			return 4;
		}

		for (int count = 0; count < additional_type_list.size(); ++count)
			{
				if (extension_file == additional_type_list.at(count))
				{
					Response_css(client_socket, file_name);
					closesocket(client_socket);

					break;
				}
			}

		for (int count = 0; count < image_type_list.size(); ++count)
			{
				if (extension_file == image_type_list.at(count))
				{
					Response_image(client_socket, file_name);
					closesocket(client_socket);

					break;
				}
			}

		for (int count = 0; count < web_type_list.size(); ++count)
			{
				if (extension_file == web_type_list.at(count))
				{
					Response_html(client_socket, file_name);
					closesocket(client_socket);

					break;
				}
			}

		for (int count = 0; count < js_type_list.size(); ++count)
			{
				if (extension_file == js_type_list.at(count))
				{
					Response_js(client_socket, file_name);
					closesocket(client_socket);

					break;
				}
			}
	}


	if (entire_query.at(0) == "POST")
	{
		string temp_login;

		ifstream fout("login.txt");

		fout >> temp_login;

		if (temp_login == entire_query[entire_query.size() - 1])
		{
			file_name = "chat.html";
			Response_html(client_socket, file_name);
			closesocket(client_socket);
		}
		else
		{
			file_name = entire_query[1];
			Response_html(client_socket, file_name);
			closesocket(client_socket);
		}

		fout.close();
	}

	return 0;
}