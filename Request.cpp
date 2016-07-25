/*******************************************************************************/
#include "ResponseRequest.h"
#include "LogFile.h"


/*******************************************************************************/
int ResponseRequest::Request(SOCKET client_socket)
{
			string				buf(1024, 0);	
			string				file_name = { 0 }; //Имя файла
			string				extension_file = { 0 }; // Расширение файла
			vector<string>		entire_query; // весь запрос
	const	vector<string>		additional_type_list = { "css" };
	const	vector<string>		image_type_list = { "jpg", "tiff", "png", "jpeg", "gif", "ico" };
	const	vector<string>		web_type_list = { "html" };
	const	vector<string>		js_type_list = { "js" };
	
	try
	{
		my_recv(client_socket, buf);
		cout << buf;

		processing(buf, entire_query);
		if (entire_query.size() == 0)
		{
			return 0;
		}

		if (entire_query.at(0) == "GET")
		{
			size_t value_search = entire_query.at(1).find('.'); // Позиция точки в строке

			if (value_search == -1)
			{
				vector<string>::iterator key_find = find(entire_query.begin(), entire_query.end(), "Sec-WebSocket-Key:");

				if ((key_find - entire_query.begin()) != entire_query.size())
				{
					SR.websocket_handshake(client_socket, entire_query.at((key_find - entire_query.begin()) + 1));
					closesocket(client_socket);
					return 0;
				}

				Response_default_html(client_socket);
				closesocket(client_socket);
				return 0;
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
				return 0;
			}

			for (int count = 0; count < additional_type_list.size(); ++count)
				{
					if (extension_file == additional_type_list.at(count))
					{
						Response_css(client_socket, file_name);
						closesocket(client_socket);

						return 0;
					}
				}

			for (int count = 0; count < image_type_list.size(); ++count)
				{
					if (extension_file == image_type_list.at(count))
					{
						Response_image(client_socket, file_name);
						closesocket(client_socket);

						return 0;
					}
				}

			for (int count = 0; count < web_type_list.size(); ++count)
				{
					if (extension_file == web_type_list.at(count))
					{
						Response_html(client_socket, file_name);
						closesocket(client_socket);

						return 0;
					}
				}

			for (int count = 0; count < js_type_list.size(); ++count)
				{
					if (extension_file == js_type_list.at(count))
					{
						Response_js(client_socket, file_name);
						closesocket(client_socket);

						return 0;
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
	}
	catch(exception& e)
	{
		LogFile log;
		log.write(e.what());
	}

	return 0;
}


void ResponseRequest::processing(const string& InBuf, vector<string>& OutEntire_query)
{
	vector<string> temp_entire_query;
	size_t first_pos = 0;
	size_t last_pos = 0;

	while( (last_pos = InBuf.find("\r\n", first_pos)) != string::npos )
	{
		temp_entire_query.push_back( InBuf.substr(first_pos, (last_pos - first_pos)) );
		first_pos = last_pos+2;
	}
	temp_entire_query.push_back(InBuf.substr(first_pos));

	for(int i = 0; i < temp_entire_query.size(); ++i)
	{
		first_pos = 0;
		last_pos = 0;

		while( (last_pos = temp_entire_query.at(i).find(' ', first_pos)) != string::npos )
		{
			OutEntire_query.push_back( temp_entire_query.at(i).substr(first_pos, (last_pos - first_pos)) );
			first_pos = last_pos+1;
		}	
		OutEntire_query.push_back(temp_entire_query.at(i).substr(first_pos));
	}	
}