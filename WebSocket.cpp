/*******************************************************************************/
#include "Server.h"
#include "Generate_key.h"
#include "Var_for_threads.h"


/*******************************************************************************/
void Response_websocket_handshake(SOCKET client_socket, string key)
{
	string response;

	generate(key);

	response += "HTTP/1.1 101 Switching Protocols\r\n";
	response += "Upgrade: websocket\r\n";
	response += "Connection: Upgrade\r\n";
	response += "Sec-WebSocket-Accept: " + key + "\r\n";
	response += "Sec-WebSocket-Version: 13\r\n\r\n";

	int result = 0;

	result = send(client_socket, response.c_str(), response.size(), 0);

	if (result == SOCKET_ERROR)
	{
		cout << "Error in send(): " << WSAGetLastError() << endl;
		system("pause");
		exit(15);
	}

	thread thr(Thread_recv, client_socket);
	thr.detach();

}


/*******************************************************************************/
void generate(string& key)
{
	unsigned char hash[SHA_DIGEST_LENGTH];

	key += guid;

	int hesh_size = key.size();

	SHA1((const unsigned char*)key.c_str(), strlen((const char*)key.c_str()), hash);

	key = (const char*)hash;

	if (key.size() > 20)
	{
		key = key.erase(20, key.length());

		key = base64_encode(reinterpret_cast<const unsigned char*>(key.c_str()), key.length());
	}
	else
	{
		key = base64_encode(reinterpret_cast<const unsigned char*>(key.c_str()), key.length());
	}
}


/*******************************************************************************/
int websocket_get_content(string& data, int data_length)
{
	u_char* mask = new u_char [4]; // длина маски 4 байта
	u_int i = 0;
	u_int j = 0;
	u_int length_code = 0;
	u_char check_value;
	string temp_str;
	int index_first_mask = 0;
	int index_first_data_byte = 0;
	int index_first_trash = 0;

	if((u_char)data[0] == 136)
	{
		return 1; // close connection
	}

	if ((u_char)data[0] == 130)
	{
		return 2; // binary data
	}

	length_code = (u_char) data[1] & 127;
	if(length_code <= 125)
	{
		index_first_mask = 2;

		for(int i =0; i < 4; ++i)
		{
			mask[i] = data[i+index_first_mask];
		}
	}
	else
		if(length_code == 126)
		{
			index_first_mask = 4;

			for(int i =0; i < 4; ++i)
			{
				mask[i] = data[i+index_first_mask];
			}
		}
		else
			if(length_code == 127)
			{
				index_first_mask = 10;

				for(int i =0; i < 4; ++i)
				{
					mask[i] = data[i+index_first_mask];
				}
			}
			else
			{
				// Exception
			}

	index_first_data_byte = index_first_mask + 4;

	for(i = index_first_data_byte; i < data_length; ++i)
	{
		check_value = (u_char)data[i] ^ mask[j % 4];
		if(data[j] != check_value)
		{
			temp_str.push_back(check_value);
		}
		++j;
	}

	data.swap(temp_str);

	delete mask;

	return 0; // // text data
}


/*******************************************************************************/
void websocket_set_content(string& data, int64_t data_length, int data_type)
{
	string message;
	int data_start_index = 0;

	if (data_type == 2)
	{
		message.push_back(130); // binary data
	}
	else
	{
		message.push_back(129);
	}

	if(data_length <= 125)
	{
		message.push_back(data_length);
		data_start_index = 2;
	}
	else
		if(data_length > 125 && data_length <= 65535)
		{
			message.push_back(126);
			for(int i = 8; i >= 0; i = i-8)
				message.push_back(( data_length >> i ) & 255);
			
			data_start_index = 4;
		}
		else
		{
			message.push_back(127);
			for(int i = 56; i >= 0; i = i-8)
				message.push_back(( data_length >> i ) & 255);

			data_start_index = 10;
		}

	for(int i = 0; i < data_length; i++) 
	{
		message.push_back(data[i]);
	}

	data.swap(message);
}